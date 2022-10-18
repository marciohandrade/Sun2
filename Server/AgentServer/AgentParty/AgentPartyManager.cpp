#include "StdAfx.h"
#include <boost/type_traits/is_same.hpp>
#include "AgentPartyManager.h"
#include "UserSessions/User.h"
#include "AgentPartyOperator.h"
#include "AgentParty.h"
#include <PacketStruct_CG.h>
#include <PacketStruct_AW.h>
#include <PacketStruct_AG.h>
#include <Zone/PortalManager.h>
#include <Zone/InstanceDungeon.h>

//_NA_20110630_BATTLE_ZONE_RENEWAL
#include "MapInfoParser.h"
#include "PartyRoomFinder.h"
#include "PartyRoomList.h"
#include "Party/PartyRoom.h"
#include "BattlezoneMissionTreeParser.h"
#include "BattleOpenTimeManager.h"
#include "PartyRoomWaitingList.h"
#include "PacketHandler/PacketHandler.h"
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#include <ChaosZoneSystem/BattleGroundManager.h>
#endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND


AgentPartyManager::AgentPartyManager()
{
    // size 50000 -> 6000으로 수정...
	key_generater_.Create(1, 6000);
    //_NA_20110630_BATTLE_ZONE_RENEWAL
    party_room_finder_ = new PartyRoomFinder;
    party_room_list_ = new PartyRoomList;
    check_waiting_room_timer_.SetTimer(1000*1);
}

AgentPartyManager::~AgentPartyManager()
{
    //_NA_20110630_BATTLE_ZONE_RENEWAL
    SAFE_DELETE(party_room_finder_);
    SAFE_DELETE(party_room_list_);
}

void
AgentPartyManager::Initialize(DWORD PartyPoolSize)
{
	party_pool_router_.Register<AgentParty>(ePARTY_FIELD);
	party_pool_router_.Initialize(ePARTY_FIELD, PartyPoolSize, PartyPoolSize/4);

    //_NA_20110630_BATTLE_ZONE_RENEWAL
    party_pool_router_.Register<AgentParty>(ePARTY_ROOM);
    party_pool_router_.Initialize(ePARTY_ROOM, PartyPoolSize, PartyPoolSize/4);

    party_pool_router_.Register<AgentParty>(ePARTY_INSTANCE);
    party_pool_router_.Initialize(ePARTY_INSTANCE, PartyPoolSize, PartyPoolSize/4);
}

void
AgentPartyManager::Update()
{
	PARTY_MAP::iterator end(party_map_.end());
	for(PARTY_MAP::iterator it = party_map_.begin(); it != end; ++it)
	{
		AgentParty* pAgentParty = (AgentParty*)it->second;
		pAgentParty->Update();
	}
}

#ifdef _NA_000068_20100419_PARTY_FINDING_SYSTEM
namespace {
inline CHARGUID scoped_get_char_guid(const DWORD user_key)
{
    const UserManager* const user_manager = UserManager::Instance();
    User* const user = user_manager->GetUserByObjKey(user_key);      
    if (user == NULL)
    {
        SUNLOG(eCRITICAL_LOG, __FUNCTION__" : Not found user : %d", user_key);
        return;
    }

    return user->GetSelectedCharGuid();
}

inline PartyFindingNumOfMemberConst::ValueType scoped_to_party_num_of_member(const DWORD num_of_member)
{
    return static_cast<PartyFindingNumOfMemberConst::ValueType>(num_of_member);
}
} // namespace
#endif // _NA_000068_20100419_PARTY_FINDING_SYSTEM

BaseParty*
AgentPartyManager::CreateParty(DWORD MasterKey, ePARTY_TYPE party_type)
{
	WORD partyKey = (WORD)key_generater_.GetKey();

	BaseParty* pBaseParty = BasePartyManager::CreateParty(party_type, partyKey, MasterKey);

	if(pBaseParty)
	{
		MSG_AW_PARTY_MAKE_SYN msgAW;
		msgAW.wPartyKey = partyKey;
#ifdef _NA_000068_20100419_PARTY_FINDING_SYSTEM
        msgAW.party_master_id_ = scoped_get_char_guid(MasterKey);
        msgAW.num_of_joined_member_ = scoped_to_party_num_of_member(pBaseParty->GetMemberNum());
#endif // _NA_000068_20100419_PARTY_FINDING_SYSTEM

		AgentServer::GetInstance()->SendToWorldServer(&msgAW, sizeof(msgAW));
	}
	else
	{
		key_generater_.RestoreKey(partyKey);
	}

	return pBaseParty;
}

RC::ePARTY_RESULT
AgentPartyManager::DestroyParty(WORD partyKey)
{
	BaseParty* pBaseParty = FindEditableParty(partyKey);
	if(!pBaseParty)
		return RC::RC_PARTY_PARTY_NOTEXIST;

	key_generater_.RestoreKey(partyKey);

	// 파티해체를 링크된 서버들에게 알린다.
	MSG_AG_PARTY_DESTROY_CMD msgAG;
	msgAG.m_wPartykey = partyKey;
	SendPacketAllLinkedServer(partyKey, &msgAG, sizeof(msgAG));

	// 다른 파티원에게 파티가 해체되었음을 알린다.
	MSG_CG_PARTY_DESTROY_BRD msgCG;
	SendPacketAllMember(partyKey, &msgCG, sizeof(msgCG));

	// WorldServer에 파티소멸 알림
	MSG_AW_PARTY_DESTROY_SYN msgAW;
	msgAW.wPartyKey = partyKey;
#ifdef _NA_000068_20100419_PARTY_FINDING_SYSTEM
    msgAW.party_master_id_ = scoped_get_char_guid(pBaseParty->GetMasterKey());
    msgAW.num_of_joined_member_ = scoped_to_party_num_of_member(pBaseParty->GetMemberNum());
#endif // _NA_000068_20100419_PARTY_FINDING_SYSTEM

	AgentServer::GetInstance()->SendToWorldServer(&msgAW, sizeof(msgAW));

	// 모든 파티원의 파티상태 초기화
	static MemberOperator Opr;
	Opr.Init(ePARTY_OPERATOR_INIT, 0, 0);
	pBaseParty->For_eachMembers(Opr);

	BasePartyManager::DestroyParty(partyKey);
    //_NA_20110630_BATTLE_ZONE_RENEWAL
    DestroyPartyRoom(partyKey);

	return RC::RC_PARTY_SUCCESS;
}

RC::ePARTY_RESULT
AgentPartyManager::CanJoinParty(User* pUser)
{
	// 해당 멤버가 다른 파티에 소속되어 있는지 체크
	if(pUser->GetPartyState().IsPartyMember())
		return RC::RC_PARTY_ALREADY_IS_MEMBEROFPARTY;

	// 유저의 상태 체크
	if(pUser->GetBehaveState() != PLAYER_BEHAVE_IDLE_STATE) 
		return RC::RC_PARTY_INVALID_PLAYER_STATE;

#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
    BattleGroundManager* const battle_ground_manager = BattleGroundManager::Instance();
    if (battle_ground_manager)
    {
        // 전장을 예약하고 매칭 대기열에 있는 상태라면
        if (battle_ground_manager->IsValidStandbyGroupMember(pUser->GetUserKey()))
        {
            return RC::RC_PARTY_ALREADY_USE_IN_CHAOS_ZONE;
        }
    }
#endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND

	return RC::RC_PARTY_SUCCESS;
}

RC::ePARTY_RESULT
AgentPartyManager::JoinParty(WORD partyKey, User* pUser)
{
	RC::ePARTY_RESULT result_code = CanJoinParty(pUser);
	if (result_code != RC::RC_PARTY_SUCCESS)
    {
		return result_code;
    }

    AgentParty* party = static_cast<AgentParty*>(FindEditableParty(partyKey));
    if (party == NULL)
    {
        return RC::RC_PARTY_PARTY_NOTEXIST;
    }

    BOOST_STATIC_ASSERT(MAX_FIELD_PARTYMEMBER_NUM == MAX_PARTYMEMBER_NUM);
    if ((party->GetMemberNum() < MAX_PARTYMEMBER_NUM) == false)
    {
        if (party->IsClosingUser(pUser) == false)
        {
            return RC::RC_PARTY_PARTYMEMBER_NUM_FULL;
        }
    }

	static _PARTY_MEMBER_INFO MemberInfo;
	pUser->SerializePartyInfo(MemberInfo);

	result_code = BasePartyManager::JoinParty(partyKey, MemberInfo);
	if(result_code != RC::RC_PARTY_SUCCESS)
    {
		return result_code;
    }
	pUser->GetPartyState().JoinParty(0, partyKey);

	if (!party->GetSessionMemberMgr().JoinMember(pUser->GetPlayerKey()))
    {
		SUNLOG(eCRITICAL_LOG, 
            __FUNCTION__" : Fail SessionMemberMgr().JoinMember()");
    }

	AddMemberToSession(party, pUser->GetServerSessionIndex(), pUser);

    //_NA_20110630_BATTLE_ZONE_RENEWAL
    if (party->GetType() == ePARTY_ROOM)
    {
        const PartyRoom* const party_room = FindPartyRoom(partyKey);
        if (party_room == NULL)
        {
            SUNLOG(eCRITICAL_LOG, __FUNCTION__"Not found party : %d", partyKey);
            return RC::RC_PARTY_NOT_FOUND_PARTYROOM;
        }

        result_code = JoinPartyRoom(pUser, partyKey, party_room->GetPartyRoomType());
        if (result_code != RC::RC_PARTY_SUCCESS)
        {
            return result_code;
        }
    }

	// 조인한 파티원에 파티 전체 정보 알림
	MSG_CG_PARTY_CREATE_CMD msgCG;
	msgCG.m_PartyKey = partyKey;
	msgCG.m_MasterKey = party->GetMasterKey();
	msgCG.m_TargetKey = party->GetTargetKey();
	msgCG.m_DistributionType = party->GetDistributionType();
	party->GetMemberInfoAll(msgCG.m_MemberTotalInfo);
	pUser->SendPacket(&msgCG, msgCG.GetSize());

	// 기존 파티원에 조인한 파티원 정보 알림
	MSG_CG_PARTY_JOIN_BRD msgCGBrd;
	msgCGBrd.m_PartyKey = partyKey;
	msgCGBrd.m_NewMemberInfo = MemberInfo;
	SendPacketAllMember(partyKey, &msgCGBrd, sizeof(msgCGBrd), pUser->GetPlayerKey());

	// WorldServer에 파티가입 알림
	MSG_AW_PARTY_JOIN_MEMBER_SYN msgAW;
	msgAW.wPartyKey = partyKey;
	msgAW.m_CharGUID = pUser->GetSelectedCharGuid();
#ifdef _NA_000068_20100419_PARTY_FINDING_SYSTEM
    msgAW.party_master_id_ = scoped_get_char_guid(party->GetMasterKey());
    msgAW.num_of_joined_member_ = scoped_to_party_num_of_member(party->GetMemberNum());
#endif // _NA_000068_20100419_PARTY_FINDING_SYSTEM

	pUser->SendToWorldServer(&msgAW, sizeof(msgAW));

	return RC::RC_PARTY_SUCCESS;
}

RC::ePARTY_RESULT
AgentPartyManager::ChangeMaster(WORD partyKey, DWORD nextMasterKey)
{
	User* pNextMaster = UserManager::Instance()->GetUserByObjKey(nextMasterKey);
	if(!pNextMaster)
		return RC::RC_PARTY_NEXTMASTER_NOTEXIST;

	RC::ePARTY_RESULT rcResult = BasePartyManager::ChangeMaster(partyKey, nextMasterKey);
	if(rcResult != RC::RC_PARTY_SUCCESS)
		return rcResult;

	// 파티장이 변경되었음을 멤버들이 링크된 서버들에게 알린다.
	MSG_AG_PARTY_CHANGE_MASTER_CMD msgAG;
	msgAG.m_wPartykey = partyKey;
	msgAG.m_dwMemberKey = nextMasterKey; //object_key
	SendPacketAllLinkedServer(partyKey, &msgAG, sizeof(msgAG));

	// 다른 파티원에게 파티장이 변경되었음을 알린다.
	MSG_CG_PARTY_CHANGE_MASTER_BRD msgCGBrd;
	msgCGBrd.m_dwObjKey = nextMasterKey; //pNextMaster->GetPlayerKey();
	SendPacketAllMember(partyKey, &msgCGBrd, sizeof(msgCGBrd));

    //_NA_20110630_BATTLE_ZONE_RENEWAL
    const PartyRoom* party_room = FindPartyRoom(partyKey);
    if (party_room == NULL)
    {
        SUNLOG(eCRITICAL_LOG, __FUNCTION__" : Not found party room : %d", partyKey);
        return RC::RC_PARTY_NOT_FOUND_PARTYROOM;
    }

    if (party_room->GetIsMissionStart() == false)
    {
        ResetPartyRoomReadyState(pNextMaster, partyKey);
    }

    ChangeMasterOfPartyRoom(pNextMaster);

#ifdef _NA_000068_20100419_PARTY_FINDING_SYSTEM
    // 월드 서버에도 알려준다.
    MSG_AW_PARTY_CHANGE_MASTER_SYN aw_change_master_syn;
    aw_change_master_syn.party_key_ = partyKey;
    aw_change_master_syn.new_party_master_id_ = scoped_get_char_guid(nextMasterKey);
    pNextMaster->SendToWorldServer(&aw_change_master_syn, sizeof(aw_change_master_syn));
#endif // _NA_000068_20100419_PARTY_FINDING_SYSTEM

	return RC::RC_PARTY_SUCCESS;
}

RC::ePARTY_RESULT AgentPartyManager::ChangeMaster_inInstanceDungeon( InstanceDungeon* dungeon, User* user )
{
    RC::ePARTY_RESULT result = RC::RC_PARTY_UNKNOWN_REASON;
    const AgentParty* party = static_cast<const AgentParty*>(FindParty(dungeon->GetLinkParty()));
    if (party != NULL)
    {
        if (party->GetType() != ePARTY_ROOM)
        {
            User* next_master = dungeon->ChangeSpecialType(false, user);
            if (next_master)
            {
                result = ChangeMaster(next_master->GetPartyState().GetPartyKey(), next_master->GetPlayerKey());
            }
        }
        else {
            result = RC::RC_PARTY_INVALID_ROOMTYPE;
        }
    }
    else {
        result = RC::RC_PARTY_PARTY_NOTEXIST;
    }

    return result;
}

RC::ePARTY_RESULT
AgentPartyManager::CanRequestJoin(User* pRequestUser, User* pResponseUser)
{
	if(pResponseUser == 0)
		return RC::RC_PARTY_PLAYER_NOTEXIST;

	RC::ePARTY_RESULT rcResult = CanJoinParty(pResponseUser);
	if(rcResult != RC::RC_PARTY_SUCCESS)
		return rcResult;

    if(PortalManager::CanJoinParty(pRequestUser, pResponseUser) == false)
        return RC::RC_PARTY_PORTAL_ENTERANCE_MEMBER;

	if(pRequestUser->GetBehaveState() != PLAYER_BEHAVE_IDLE_STATE || pResponseUser->GetBehaveState() != PLAYER_BEHAVE_IDLE_STATE)
		return RC::RC_PARTY_INVALID_PLAYER_STATE;

#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
    // 타락한 사원일 경우 파티 허용이 안된다!!.
    if (pRequestUser->GetZoneType() == eZONETYPE_INSTANCE)
    {
        const KEYTYPE zone_key = pRequestUser->GetZoneKey();
        InstanceDungeon* const instance_dungeon = \
            static_cast<InstanceDungeon*>(g_ZoneManager.FindZone(zone_key));
        if (instance_dungeon->GetDungeonType() == INDUN_KIND_SS_QUEST)
        {
            return RC::RC_PARTY_INVALID_PLAYER_STATE;
        }
    }
#endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND

	// 다음의 zonetype들에서 파티가 허용된다
	if( pRequestUser->GetZoneType() != eZONETYPE_VILLAGE &&
        pRequestUser->GetZoneType() != eZONETYPE_DOMINATION_FIELD &&
        pRequestUser->GetZoneType() != eZONETYPE_FIELD &&
        pRequestUser->GetZoneType() != eZONETYPE_INSTANCE &&    // 인스턴스 던전에서 파티 허용
        pRequestUser->GetZoneType() != eZONETYPE_SPA_FIELD
        )
		return RC::RC_PARTY_INVALID_PLAYER_STATE;

#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
    BattleGroundManager* const battle_ground_manager = BattleGroundManager::Instance();
    if (battle_ground_manager)
    {
        // 전장을 예약하고 매칭 대기열에 있는 상태라면
        if (battle_ground_manager->IsValidReservationMember(pResponseUser->GetUserKey()))
        {
            return RC::RC_PARTY_ALREADY_USE_IN_CHAOS_ZONE;
        }
    }
#endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND

	return RC::RC_PARTY_SUCCESS;
}

RC::ePARTY_RESULT
AgentPartyManager::InviteParty(User* pMaster, User* pJoinUser)
{
	// 파티요청이 가능한지 체크
	RC::ePARTY_RESULT rcResult = CanRequestJoin(pMaster, pJoinUser);
	if(rcResult != RC::RC_PARTY_SUCCESS)
		return rcResult;

    DWORD masterPlayerKey = pMaster->GetPlayerKey();
	BaseParty* pBaseParty = FindEditableParty(pMaster->GetPartyState().GetPartyKey());

	// 요청자가 파티원이라면
	if(pBaseParty)
	{
		if(pBaseParty->GetMasterKey() != masterPlayerKey)
			return RC::RC_PARTY_ISNOT_MASTEROFPARTY;
	}
	// 파티원이 아니면 파티를 새로 생성한다.
	else
	{
		pBaseParty = CreateParty(masterPlayerKey, ePARTY_FIELD);
		if(pBaseParty == 0)
			return RC::RC_PARTY_INVALID_ROOMTYPE;

		// 파티를 새로 생성할 경우 파티가입 요청자도 파티에 가입시킨다.
		JoinParty(pBaseParty->GetPartyKey(), pMaster);
	}

    //_NA_20110630_BATTLE_ZONE_RENEWAL
    WORD join_party_key = pBaseParty->GetPartyKey();

    if (pBaseParty->GetType() == ePARTY_ROOM)
    {
        const PartyRoom* party_room = FindPartyRoom(join_party_key);
        if (party_room == NULL)
        {
            SUNLOG(eCRITICAL_LOG, __FUNCTION__" : Not found party room : %d", join_party_key);
            return RC::RC_PARTY_FAIL_JOIN_PARTY_ROOM;
        }

        if (party_room->GetMapCode() != 0 && 
            party_room->CheckLevel(pJoinUser->GetCharLV()) == false)
        { 
            MSG_CG_PARTY_JOIN_ROOM_ACK nak_msg;
            nak_msg.result_code_ = RC::RC_PARTY_NOT_MATCHING_USER_LEVEL;
            pJoinUser->SendPacket(&nak_msg, nak_msg.GetSize());
            return RC::RC_PARTY_NOT_MATCHING_USER_LEVEL;
        }

        if (party_room->CanJoinPartyRoomByMemberCount() == false)
        {
            MSG_CG_PARTY_JOIN_ROOM_ACK nak_msg;
            nak_msg.result_code_ = RC::RC_PARTY_FAIL_MAX_MEMBER_COUNT;
            pJoinUser->SendPacket(&nak_msg, nak_msg.GetSize());
            return RC::RC_PARTY_PARTYMEMBER_NUM_FULL;
        }

        MSG_AG_PARTY_CAN_JOIN_ROOM_SYN syn_msg;
        syn_msg.map_code_ = party_room->GetMapCode();
        syn_msg.party_room_type_ = party_room->GetZoneType();
        syn_msg.join_party_key_ = join_party_key;
        pJoinUser->SendToLinkedServer(&syn_msg, sizeof(syn_msg));

        return RC::RC_PARTY_SUCCESS;
    }
    else
    {
        return JoinParty(join_party_key, pJoinUser);
    }
}

RC::ePARTY_RESULT
AgentPartyManager::CanRequestLeave(User* pRequestUser, User* pLeaveUser)
{
	// 나가려는 캐릭터가 파티에 소속되어 있는지 체크
	BaseParty* pBaseParty = FindEditableParty(pRequestUser->GetPartyState().GetPartyKey());
	if(!pBaseParty)
		return RC::RC_PARTY_PARTY_NOTEXIST;

	if(!pBaseParty->GetMemberInfo(pLeaveUser->GetPlayerKey()))
		return RC::RC_PARTY_ISNOT_MEMBEROFPARTY;

	// 다른 사람을 내보내려면 파티장이어야 한다.
	if(pRequestUser != pLeaveUser)
	{
		if(pBaseParty->GetMasterKey() != pRequestUser->GetPlayerKey())
			return RC::RC_PARTY_ISNOT_MASTEROFPARTY;
	}

	return RC::RC_PARTY_SUCCESS;
}

RC::ePARTY_RESULT
AgentPartyManager::ForcedExpulsion(User* pMaster, User* pLeaveUser)
{
	RC::ePARTY_RESULT rcResult = CanRequestLeave(pMaster, pLeaveUser);
	if(rcResult != RC::RC_PARTY_SUCCESS)
		return rcResult;

	return LeaveParty(pLeaveUser, eREASON_LEAVE_PARTY_BAN);
}

RC::ePARTY_RESULT
AgentPartyManager::SelectTarget(User* pMaster, DWORD dwObjectKey)
{
	WORD partyKey = pMaster->GetPartyState().GetPartyKey();

	// 파티장인지 체크
	BaseParty* pBaseParty = FindEditableParty(partyKey);
	if(!pBaseParty)
		return RC::RC_PARTY_PARTY_NOTEXIST;

	if(pBaseParty->GetMasterKey() != pMaster->GetPlayerKey())
		return RC::RC_PARTY_ISNOT_MASTEROFPARTY;

	// 파티 타겟을 설정한다.
	pBaseParty->SetTargetKey(dwObjectKey);

	// 설정된 타겟을 브로드로 알려준다.
	MSG_CG_PARTY_SELECT_TARGET_BRD msgCGBrd;
	msgCGBrd.m_dwObjectKey = dwObjectKey;
	SendPacketAllMember(partyKey, &msgCGBrd, sizeof(msgCGBrd));

	return RC::RC_PARTY_SUCCESS;
}

RC::ePARTY_RESULT
AgentPartyManager::SelectMinimap(User* pUser, float PosX, float PosY)
{
	WORD partyKey = pUser->GetPartyState().GetPartyKey();

	BaseParty* pBaseParty = FindEditableParty(partyKey);
	if(!pBaseParty)
		return RC::RC_PARTY_PARTY_NOTEXIST;

	if(pUser->GetZoneType() == eZONETYPE_VILLAGE)	
		return RC::RC_PARTY_INVALID_ROOMTYPE;

	// 설정된 타겟을 브로드로 알려준다.
	MSG_CG_PARTY_MINIMAP_POINTING_BRD msgCGBrd;
	msgCGBrd.m_fx = PosX;
	msgCGBrd.m_fy = PosY;
	SendPacketAllMember(partyKey, &msgCGBrd, sizeof(msgCGBrd), 0, pUser->GetZoneKey());

	return RC::RC_PARTY_SUCCESS;
}

RC::ePARTY_RESULT
AgentPartyManager::ChangeDistributionType(User* pUser, eITEM_DISTRIBUTION_TYPE Type)
{
	WORD partyKey = pUser->GetPartyState().GetPartyKey();

	BaseParty* pBaseParty = FindEditableParty(partyKey);
	if(!pBaseParty)
		return RC::RC_PARTY_PARTY_NOTEXIST;

	if(Type >= eITEM_DISTRIBUTION_MAX)
		return RC::RC_PARTY_CHANGE_ITEM_DISTRIBUTION_FAIL;

	pBaseParty->SetDistributionType(Type);

	MSG_AG_PARTY_ITEM_DISTRIBUTION_CHANGE_CMD msgAG;
	msgAG.m_wPartykey = partyKey;
	msgAG.m_byItemDistributionType = Type;
	SendPacketAllLinkedServer(partyKey, &msgAG, sizeof(msgAG));

	MSG_CG_PARTY_ITEM_DISTRIBUTION_CHANGE_BRD msgCGBrd;
	msgCGBrd.m_byItemDistributionType = Type;
	SendPacketAllMember(partyKey, &msgCGBrd, sizeof(msgCGBrd));

	return RC::RC_PARTY_SUCCESS;
}

bool
AgentPartyManager::MemberChangeState(User* pUser)
{
	WORD partyKey = pUser->GetPartyState().GetPartyKey();

	BaseParty* pBaseParty = FindEditableParty(partyKey);
	if(!pBaseParty)	return false;

	if(!pBaseParty->GetMemberInfo(pUser->GetPlayerKey()))
		return false;

	static _PARTY_MEMBER_INFO MemberInfo;
	pUser->SerializePartyInfo(MemberInfo);
	pBaseParty->SetMemberInfo(pUser->GetPlayerKey(), MemberInfo);
	
	return true;
}

void
AgentPartyManager::MemberLevelUp(User* pUser, FLOAT fHPRatio, FLOAT fMPRatio)
{
	if(MemberChangeState(pUser))
	{
		MSG_CG_PARTY_MEMBER_LEVEL_UP_BRD msgCGBrd;
		msgCGBrd.m_dwObjKey = pUser->GetPlayerKey();
		msgCGBrd.m_wLevel = pUser->GetSelectedCharLV();
		msgCGBrd.m_byHPRatio = (BYTE)(fHPRatio*100);
		msgCGBrd.m_byMPRatio = (BYTE)(fMPRatio*100);
		SendPacketAllMember(pUser->GetPartyState().GetPartyKey(), &msgCGBrd, sizeof(msgCGBrd));
	}
}

void
AgentPartyManager::MemberResurrection(User* pUser, FLOAT fHPRatio, FLOAT fMPRatio)
{
	if(MemberChangeState(pUser))
	{
		MSG_CG_PARTY_MEMBER_RESURRECTION_BRD msgCGBrd;
		msgCGBrd.m_dwObjKey = pUser->GetPlayerKey();
		msgCGBrd.m_byHPRatio = (BYTE)(fHPRatio*100);
		msgCGBrd.m_byMPRatio = (BYTE)(fMPRatio*100);
		SendPacketAllMember(pUser->GetPartyState().GetPartyKey(), &msgCGBrd, sizeof(msgCGBrd));
	}
}

void
AgentPartyManager::MemberChangeChaoState(User* pUser)
{
	if(MemberChangeState(pUser))
	{
		MSG_CG_PARTY_MEMBER_CHAO_STATE_CHANGE_BRD msgCGBrd;
		msgCGBrd.m_dwObjKey = pUser->GetPlayerKey();
		msgCGBrd.m_byChaoState = pUser->GetSelectedCharChaoState();
		SendPacketAllMember(pUser->GetPartyState().GetPartyKey(), &msgCGBrd, sizeof(msgCGBrd));
	}
}

void
AgentPartyManager::MemberDead(User* pUser)
{
	if(MemberChangeState(pUser))
	{
		MSG_CG_PARTY_MEMBER_DEAD_BRD msgCGBrd;
		msgCGBrd.m_dwObjKey = pUser->GetPlayerKey();
		SendPacketAllMember(pUser->GetPartyState().GetPartyKey(), &msgCGBrd, sizeof(msgCGBrd));
	}
}

//{__NA001336_090514_PARTY_NAVI_SUPPORT__
void
AgentPartyManager::ChangedMembersInfo(WORD partyKey,
                                      const MSG_AG_PARTY_NAVI_INFO_BRD* navi,
                                      const MSG_AG_STATUS_CHAR_STAT_CHANGE_CMD* hpmp)
{
    AgentParty* pAgentParty = (AgentParty*)FindEditableParty(partyKey);
    if(pAgentParty == 0)
        return;

    MSG_CG_PARTY_NAVI_INFO_BRD msgNavi;
    MSG_CG_PARTY_MEMBER_HP_CHANGE_BRD msgHP;
    MSG_CG_PARTY_MEMBER_MP_CHANGE_BRD msgMP;
#ifdef _NA_006638_20120415_ADD_SHIELD_INFO_IN_TARGET_INFO
    MSG_CG_PARTY_MEMBER_SD_CHANGE_BRD msgSD;
#endif // _NA_006638_20120415_ADD_SHIELD_INFO_IN_TARGET_INFO

    msgNavi.m_NumberOfNodes = 0;
    msgHP.m_NumberOfNodes = 0;
    msgMP.m_NumberOfNodes = 0;
#ifdef _NA_006638_20120415_ADD_SHIELD_INFO_IN_TARGET_INFO
    msgSD.number_of_nodes = 0;
#endif // _NA_006638_20120415_ADD_SHIELD_INFO_IN_TARGET_INFO

#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    MSG_CG_PARTY_MEMBER_FP_CHANGE_BRD msgFP;
    msgFP.number_of_nodes = 0;
#endif //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE

    UserManager* pUserManager = UserManager::Instance(); //나중에 좀 더 개선할 것.

    if(navi)
    {
        msgNavi.m_CodeType = navi->m_CodeType;
        const BYTE upperbound = navi->m_NumberOfNodes < _countof(hpmp->m_Nodes)
                              ? navi->m_NumberOfNodes
                              : _countof(hpmp->m_Nodes);
        msgNavi.m_NumberOfNodes = upperbound;
        BOOST_STATIC_ASSERT((boost::is_same<MSG_CG_PARTY_NAVI_INFO_BRD::Node,
                                            MSG_AG_PARTY_NAVI_INFO_BRD::Node>::value));

        MSG_CG_PARTY_NAVI_INFO_BRD::Node* pDestIt  = &msgNavi.m_Nodes[0];
        const MSG_AG_PARTY_NAVI_INFO_BRD::Node* pSrcIt  = &navi->m_Nodes[0];
        CopyMemory(pDestIt, pSrcIt, sizeof(*pDestIt) * upperbound);

#ifdef _NA002676_WORLD_SERVER_RENEWAL
        MSG_AW_USER_STADING_POSITION_CMD msg_pos_cmd;
        msg_pos_cmd.number_of_nodes = upperbound;

        User* pole_user = NULL;
        for (uint32_t index = 0; index < msg_pos_cmd.number_of_nodes; ++index)
        {
            MSG_AW_USER_STADING_POSITION_CMD::Node* dest = &msg_pos_cmd.nodes[index];
            const MSG_AG_PARTY_NAVI_INFO_BRD::Node& src = navi->m_Nodes[index];
            User* user = pUserManager->GetUserByObjKey(src.objectKey);
            dest->user_guid = user ? user->GetUserKey() : 0;
            dest->pos_x = src.posX;
            dest->pos_y = src.posY;
            //
            if (pole_user != NULL) {
                pole_user = user;
            };
        };
        if (pole_user != NULL) {
            pole_user->SendToWorldServer(&msg_pos_cmd, msg_pos_cmd.GetSize());
        };
#endif
    };


    if(hpmp)
    {
        const BYTE upperbound = hpmp->m_NumberOfNodes < _countof(hpmp->m_Nodes)
                              ? hpmp->m_NumberOfNodes
                              : _countof(hpmp->m_Nodes);
        typedef MSG_AG_STATUS_CHAR_STAT_CHANGE_CMD::uNode StatChangeNode;
        const StatChangeNode* pSrcIt = &hpmp->m_Nodes[0],
                            * pSrcEnd = &hpmp->m_Nodes[upperbound];
        MSG_CG_PARTY_MEMBER_HP_CHANGE_BRD::Node* pHpIt = &msgHP.m_Nodes[0];
        MSG_CG_PARTY_MEMBER_MP_CHANGE_BRD::Node* pMpIt = &msgMP.m_Nodes[0];
#ifdef _NA_006638_20120415_ADD_SHIELD_INFO_IN_TARGET_INFO
        MSG_CG_PARTY_MEMBER_SD_CHANGE_BRD::Node* pSdIt = &msgSD.nodes[0];
#endif // _NA_006638_20120415_ADD_SHIELD_INFO_IN_TARGET_INFO
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
        MSG_CG_PARTY_MEMBER_FP_CHANGE_BRD::Node* pFPIt = &msgFP.nodes[0];
#endif //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE

        for( ; pSrcIt != pSrcEnd; ++pSrcIt)
        {
            User* pUser = pUserManager->GetUserByObjKey(pSrcIt->m_HP.objectKey);
            if(pUser == 0)
                continue;
            // 나중에 Agent가 HP,MP관리하는 루틴을 제거할 수 있도록 유도하자.

            if(pSrcIt->m_HP.event == hpmp->eEvent_ChangedHP)
            {
                const StatChangeNode::HP& rHP = pSrcIt->m_HP;
                if(msgHP.m_NumberOfNodes < _countof(msgHP.m_Nodes))
                {
                    //assert(rHP.maxHP != 0)
                    pHpIt->objectKey = rHP.objectKey;
                    pHpIt->ratio = rHP.currentHP ? BYTE(rHP.currentHP * 100 / rHP.maxHP) : 0;
                    if(pHpIt->ratio == 0 && rHP.currentHP)
                        pHpIt->ratio = 1;
                    ++msgHP.m_NumberOfNodes;
                    ++pHpIt;

                    pUser->SetSelectedCharCurHP(rHP.currentHP);
                    pUser->SetSelectedCharMaxHP(rHP.maxHP);
                    pUser->SetReservedHP(rHP.reservedHP);
                    pUser->SetNeedChangedStatus(PlayerStatEvent::eFlag_ChangedHP);
                }
            }
            if(pSrcIt->m_MP.event == hpmp->eEvent_ChangedMP)
            {
                const StatChangeNode::MP& rMP = pSrcIt->m_MP;
                if(msgMP.m_NumberOfNodes < _countof(msgMP.m_Nodes))
                {
                    //assert(rMP.maxMP != 0)
                    pMpIt->objectKey = rMP.objectKey;
                    pMpIt->ratio = rMP.currentMP ? BYTE(rMP.currentMP * 100 / rMP.maxMP) : 0;
                    ++msgMP.m_NumberOfNodes;
                    ++pMpIt;

                    pUser->SetSelectedCharCurMP(rMP.currentMP);
                    pUser->SetSelectedCharMaxMP(rMP.maxMP);
                    pUser->SetNeedChangedStatus(PlayerStatEvent::eFlag_ChangedMP);
                }
            }
            if(pSrcIt->m_SD.event == hpmp->eEvent_ChangedSD)
            {
#ifdef _NA_006638_20120415_ADD_SHIELD_INFO_IN_TARGET_INFO
                const StatChangeNode::SD& rSD = pSrcIt->m_SD;
                if(msgSD.number_of_nodes < _countof(msgSD.nodes))
                {
                    pSdIt->object_key = rSD.objectKey;
                    pSdIt->ratio = rSD.currentSD ? BYTE(rSD.currentSD * 100 / rSD.maxSD) : 0;
                    ++msgSD.number_of_nodes;
                    ++pSdIt;

                    pUser->SetSelectedCharCurSD(rSD.currentSD);
                    pUser->SetSelectedCharMaxSD(rSD.maxSD);
                    pUser->SetNeedChangedStatus(PlayerStatEvent::eFlag_ChangedSD);
                }
#else // _NA_006638_20120415_ADD_SHIELD_INFO_IN_TARGET_INFO
                const StatChangeNode::SD& rSD = pSrcIt->m_SD;
                pUser->SetSelectedCharCurSD(rSD.currentSD);
                pUser->SetSelectedCharMaxSD(rSD.maxSD);
                pUser->SetNeedChangedStatus(PlayerStatEvent::eFlag_ChangedSD);
#endif // _NA_006638_20120415_ADD_SHIELD_INFO_IN_TARGET_INFO
            }
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
            if (pSrcIt->m_FP.event == hpmp->eEvent_ChangedFP)
            {
               const StatChangeNode::FP& rFP = pSrcIt->m_FP;
                if (msgFP.number_of_nodes < _countof(msgFP.nodes))
                {
                    pFPIt->object_key = rFP.objectKey;
                    ++msgFP.number_of_nodes;
                    ++pFPIt;

                    pUser->SetSelectedCharCurFP(rFP.currentFP);
                    //FP는 변화가 없으므로 MaxFP를 갱신해줄 필요가 없다
                    //pUser->SetSelectedCharMaxFP(rFP.maxFP);
                    pUser->SetNeedChangedStatus(PlayerStatEvent::eFlag_ChangedFP);
                }
            }
        }
    }
#endif //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    // Special Operator: packet packing control ... 에이~ 일단 나중으로 미루자.

    typedef AgentParty::MEMBER_MAP  MEMBER_MAP;
    const MEMBER_MAP& members = pAgentParty->GetMemberList();
    FOREACH_CONTAINER(const MEMBER_MAP::value_type& member, members, MEMBER_MAP)
    {
        const _PARTY_MEMBER_INFO& memberInfo = member.second;
        User* pMember = pUserManager->GetUserByObjKey(memberInfo.m_dwMemberKey);
        if (pMember == NULL)
        {
            SUNLOG(eCRITICAL_LOG, __FUNCTION__" : Not found user : %d", memberInfo.m_dwMemberKey);
            continue;
        }
#ifdef _NA_007825_20141118_MODIFY_PARTY_CHARACTER_STATUSUPDATE
        // 이제 파티상태에서도 '자신'의 상태업데이트는 자신이 하도록 변경한다
#else
        DWORD playerKey = pMember->GetPlayerKey();
        BYTE statChangedEvent = pMember->GetNeedChangedStatus();
        if(statChangedEvent & PlayerStatEvent::eFlag_ChangedHP)
        {
            MSG_CG_STATUS_CHANGE_HP_BRD msgBRD;
            msgBRD.m_dwObjectKey = playerKey;
            msgBRD.m_dwTargetHP = pMember->GetSelectedCharCurHP();
            msgBRD.m_dwReserveHP = pMember->GetReservedHP();
            pMember->SendPacket(&msgBRD, sizeof(msgBRD));
        }
        if(statChangedEvent & PlayerStatEvent::eFlag_ChangedMP)
        {
            MSG_CG_STATUS_CHANGE_MP_BRD msgBRD;
            msgBRD.m_dwObjectKey = playerKey;
            msgBRD.m_dwTargetMP = pMember->GetSelectedCharCurMP();
            pMember->SendPacket(&msgBRD, sizeof(msgBRD));
        }
        if(statChangedEvent & PlayerStatEvent::eFlag_ChangedSD)
        {
            MSG_CG_STATUS_CHANGE_SD_CMD msgCMD;
            msgCMD.m_dwSD = pMember->GetSelectedCharCurSD();
            pMember->SendPacket(&msgCMD, sizeof(msgCMD));
        }
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
        if (statChangedEvent & PlayerStatEvent::eFlag_ChangedFP)
        {
            MSG_CG_STATUS_CHANGE_FP_CMD msgCMD;
            msgCMD.object_key = playerKey;
            msgCMD.m_dwTargetFP = pMember->GetSelectedCharCurFP();
            pMember->SendPacket(&msgCMD, sizeof(msgCMD));
        }
#endif //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
#endif //_NA_007825_20141118_MODIFY_PARTY_CHARACTER_STATUSUPDATE
        //
        pMember->SetNeedChangedStatus(PlayerStatEvent::eFlag_None);

        if(msgNavi.m_NumberOfNodes)
            pMember->SendPacket(&msgNavi, msgNavi.GetSize());
        if(msgHP.m_NumberOfNodes)
            pMember->SendPacket(&msgHP, msgHP.GetSize());
        if(msgMP.m_NumberOfNodes)
            pMember->SendPacket(&msgMP, msgMP.GetSize());
#ifdef _NA_006638_20120415_ADD_SHIELD_INFO_IN_TARGET_INFO
        if(msgSD.number_of_nodes)
            pMember->SendPacket(&msgSD, msgSD.GetSize());
#endif // _NA_006638_20120415_ADD_SHIELD_INFO_IN_TARGET_INFO
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
        if (msgFP.number_of_nodes) {
            pMember->SendPacket(&msgFP, msgFP.GetSize());
        }
#endif //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    }
}
//}__NA001336_090514_PARTY_NAVI_SUPPORT__

void
AgentPartyManager::AddMemberToSession(AgentParty* party, 
                                      DWORD to_server_index, 
                                      User* user)
{
    //AgentServer* const agent_server = AgentServer::GetInstance();

	if (!party->GetSessionMemberMgr().AddMemberToSession(user->GetPlayerKey(), to_server_index))
    {
		return;
    }

    DWORD member_num = party->GetSessionMemberMgr().GetSessionMemberNum(to_server_index);
    if (member_num == 1)
    {
        MSG_AG_PARTY_CREATE_CMD create_msg;
        create_msg.m_wPartykey = party->GetPartyKey();
        create_msg.m_MasterKey = party->GetMasterKey();
        create_msg.m_byItemDistributionType = party->GetDistributionType();
        //_NA_20110630_BATTLE_ZONE_RENEWAL
        create_msg.m_PartyType = party->GetType();
        user->SendToLinkedServer(&create_msg, sizeof(create_msg));
    }

    MSG_AG_PARTY_JOIN_CMD join_msg;
    join_msg.m_wPartykey = party->GetPartyKey();
    join_msg.m_MemberInfo = *(party->GetMemberInfo(user->GetPlayerKey()));
    join_msg.m_bMaster = party->GetMasterKey() == user->GetPlayerKey() ? true : false;
    user->SendToLinkedServer(&join_msg, sizeof(join_msg));
}

void
AgentPartyManager::RemoveMemberToSession(AgentParty* party, 
                                         DWORD from_server_index, 
                                         DWORD to_server_index, 
                                         User* user)
{
    //AgentServer* const agent_server = AgentServer::GetInstance();

	if (!party->GetSessionMemberMgr().RemoveMemberToSession(user->GetPlayerKey(), to_server_index))
    {
		return;
    }

	DWORD member_num = party->GetSessionMemberMgr().GetSessionMemberNum(from_server_index);
	if (member_num == 0)
	{
		MSG_AG_PARTY_DESTROY_CMD destroy_msg;
		destroy_msg.m_wPartykey = party->GetPartyKey();
        user->SendToLinkedServer(&destroy_msg, sizeof(destroy_msg));
	}
	else
	{
		MSG_AG_PARTY_LEAVE_CMD leave_msg;
		leave_msg.m_wPartykey = party->GetPartyKey();
		leave_msg.m_dwMemberKey = user->GetPlayerKey();
        user->SendToLinkedServer(&leave_msg, sizeof(leave_msg));
	}
}

void
AgentPartyManager::SendPacketAllLinkedServer(WORD partyKey, MSG_BASE* pMsg, WORD wSize)
{
	AgentParty* pAgentParty = (AgentParty*)FindEditableParty(partyKey);
	if(!pAgentParty)	return;

	LinkServerOperator Opr(pMsg, wSize);
	pAgentParty->GetSessionMemberMgr().For_eachSessions(Opr);
}

void
AgentPartyManager::SendPacketAllMember(WORD partyKey, MSG_BASE* pMsg, WORD wSize, DWORD ExceptKey, KEYTYPE CheckZoneKey)
{
	AgentParty* pAgentParty = (AgentParty*)FindEditableParty(partyKey);
	if(!pAgentParty)	return;

	pAgentParty->SendPacketAll(pMsg, wSize, ExceptKey, CheckZoneKey);
}

bool
AgentPartyManager::IsMaster(User* pUser)
{
	AgentParty* pAgentParty = (AgentParty*)FindEditableParty(pUser->GetPartyState().GetPartyKey());
	if(!pAgentParty)	
		return false;

	return pAgentParty->GetMasterKey() == pUser->GetPlayerKey() ? true : false;
}

void
AgentPartyManager::AddRelayUser(RoomInterface * pRoom)
{
	AgentParty* pAgentParty = (AgentParty*)FindEditableParty(pRoom->GetLinkParty());
	if(!pAgentParty)	
		return;

	static MemberOperator Opr;
	Opr.Init(ePARTY_OPERATOR_RELAY_USER, 0, 0);
	Opr.SetZone(pRoom);
	pAgentParty->For_eachMembers(Opr);

    //__NA000000_090622_MOB_ABILITY_CONTROL_CHANGE__
    BYTE number_of_players = BYTE(pRoom->GetRelayHash().GetDataNum());
    pRoom->SetNumberOfExpectedPlayers(number_of_players);
}

DWORD
AgentPartyManager::GetMemberNum(WORD partyKey)
{
	AgentParty* pAgentParty = (AgentParty*)FindEditableParty(partyKey);
	if(!pAgentParty)	
		return 0;

	return pAgentParty->GetMemberNum();
}

RC::eROOM_RESULT
AgentPartyManager::InstanceDungeonCheck(WORD partyKey, const sMAPINFO* psMAPINFO)
{
	AgentParty* pAgentParty = (AgentParty*)FindEditableParty(partyKey);
	if(!pAgentParty)
	{
		// 파티소속이 아니면 입장시켜 준다.
		return RC::RC_ROOM_SUCCESS;
	}

	// 최소, 최대 인원제한 체크
	if(pAgentParty->GetMemberNum() < psMAPINFO->byMinUserNum || psMAPINFO->byMaxUserNum < pAgentParty->GetMemberNum())
		return RC::RC_ROOM_MISMATCH_NUMBER_IN_PARTY;

	// 모든 파티원의 레벨이 입장 가능한 레벨인지 체크
	static MemberOperator Opr;
	Opr.Init(ePARTY_OPERATOR_FITNESS_LEVEL_CHECK, 0, 0);
	Opr.SetFitnessLV(psMAPINFO->minLV, psMAPINFO->maxLV);
	pAgentParty->For_eachMembers(Opr);

	if(Opr.IsSuccess())
		return RC::RC_ROOM_SUCCESS;
	else
		return RC::RC_ROOM_MISMATCH_LEVEL_IN_PARTY;
}

RC::eROOM_RESULT
AgentPartyManager::TopOfTrialCheck(WORD partyKey)
{
	AgentParty* pAgentParty = (AgentParty*)FindEditableParty(partyKey);
	if(!pAgentParty)
	{
		// 파티소속이 아니면 입장시켜 준다.
		return RC::RC_ROOM_SUCCESS;
	}

	static MemberOperator Opr;
    /*
    // removed a rule by __NA001319_SAME_CLASS_LIMIT_REMOVE__
	// 같은 클래스 5명 이상일 수 없음
	Opr.Init(ePARTY_OPERATOR_SAME_CLASS_CHECK, 0, 0);
	pAgentParty->For_eachMembers(Opr);
	if(!Opr.IsSuccess())
		return RC::RC_ROOM_MISMATCH_SAME_CLASS_IN_PARTY;
    */
	// 파티장과 레벨 10 이상 차이나면 안됨
	User* pMaster = UserManager::Instance()->GetUserByObjKey(pAgentParty->GetMasterKey());
	if(!pMaster)	return RC::RC_ROOM_SUCCESS;

	LEVELTYPE MinLevel = pMaster->GetCharLV() < 10 ? 1 : pMaster->GetCharLV() - 10;
	LEVELTYPE MaxLevel = pMaster->GetCharLV() + 10;
	Opr.Init(ePARTY_OPERATOR_FITNESS_LEVEL_CHECK, 0, 0);
	Opr.SetFitnessLV(MinLevel, MaxLevel);
	pAgentParty->For_eachMembers(Opr);
	if(!Opr.IsSuccess())
		return RC::RC_ROOM_MISMATCH_LEVEL_10_WITH_MASTER;

	return RC::RC_ROOM_SUCCESS;
}

// implemented by _NA_0_20110210_INVITE_PARTY_BUG
RC::ePARTY_RESULT AgentPartyManager::RequestInviteParty(User* request_user, User* response_user)
{
    static uint generated_party_key = 0;
    uint current_time = ::GetTickCount();
    uint user_party_key = 0;

    user_party_key = request_user->GetPartyState().get_invite_party_key();
    InvitePartyKeyHashmap::iterator iter_request = invite_party_key_.find(user_party_key);
    if (iter_request != invite_party_key_.end())
    {
        InvitePartyKey& invite_request = iter_request->second;
        if ((current_time - invite_request.response_time) > InvitePartyKey::Response_Limit_Time)
            invite_party_key_.erase(iter_request);
        else
            return RC::RC_PARTY_BEING_INVITE_REQUEST;
    }

    user_party_key = response_user->GetPartyState().get_invite_party_key();
    InvitePartyKeyHashmap::iterator iter_response = invite_party_key_.find(user_party_key);
    if (iter_response != invite_party_key_.end())
    {
        InvitePartyKey& invite_response = iter_response->second;
        if ((current_time - invite_response.response_time) > InvitePartyKey::Response_Limit_Time)
            invite_party_key_.erase(iter_response);
        else
            return RC::RC_PARTY_BEING_INVITE_RESPONSE;
    }


    ++generated_party_key;

    InvitePartyKey party_key;
    party_key.request_user_key = request_user->GetPlayerKey();
    party_key.response_user_key = response_user->GetPlayerKey();
    party_key.response_time = current_time;
    invite_party_key_[generated_party_key] = party_key;

    request_user->GetPartyState().set_invite_party_key(generated_party_key);
    response_user->GetPartyState().set_invite_party_key(generated_party_key);

    return RC::RC_PARTY_SUCCESS;
}

RC::ePARTY_RESULT AgentPartyManager::ResponseInviteParty(User* response_user)
{
    uint user_party_key = response_user->GetPartyState().get_invite_party_key();
    InvitePartyKeyHashmap::iterator iter_request = invite_party_key_.find(user_party_key);
    if (iter_request != invite_party_key_.end())
    {
        RC::ePARTY_RESULT result = RC::RC_PARTY_SUCCESS;
        InvitePartyKey& invite_response = iter_request->second;
        if ((::GetTickCount() - invite_response.response_time) > InvitePartyKey::Response_Limit_Time)
        {
            result = RC::RC_PARTY_INVITE_RESPONSE_TIME_OVER;
        }
        else
        {
            if (invite_response.response_user_key != response_user->GetPlayerKey())
                result = RC::RC_PARTY_INVALID_PLAYER_STATE;
        }

        // 파티요청자
        User* request_user = UserManager::Instance()->GetUserByObjKey(invite_response.request_user_key);
        if (request_user)
            request_user->GetPartyState().set_invite_party_key(0);
        else
            result = RC::RC_PARTY_INVALID_PLAYER_STATE;

        response_user->GetPartyState().set_invite_party_key(0);

        invite_party_key_.erase(iter_request);

        return result;
    }
    return RC::RC_PARTY_WASNOT_REQUEST_PARTY;
}

void
AgentPartyManager::SetOtherMemberKey(User* pRequestUser, User* pResponseUser)
{
	// 상대방 UserKey를 서로 등록해 놓는다.
	pRequestUser->GetPartyState().SetOtherMemberKey(pResponseUser->GetPlayerKey());
	pResponseUser->GetPartyState().SetOtherMemberKey(pRequestUser->GetPlayerKey());
}

void
AgentPartyManager::ResetOtherMemberKey(User* pRequestUser, User* pResponseUser)
{
	// 상대방 UserKey를 서로 등록해 놓는다.
	pRequestUser->GetPartyState().SetOtherMemberKey(0);
	pResponseUser->GetPartyState().SetOtherMemberKey(0);
}

void
AgentPartyManager::Display()
{
	PARTY_MAP::iterator it = party_map_.begin(),
                        end = party_map_.end();
	for( ; it != end; ++it)
	{
		AgentParty* pAgentParty = (AgentParty*)it->second;
		pAgentParty->Display();
	}

	SUNLOG(eCRITICAL_LOG, "TotPartyNum[%d]", party_map_.size());
}

RC::eROOM_RESULT
AgentPartyManager::MigrateToInstanceDungeon(User* pRequestUser, InstanceDungeon* indun)
{
    // TODO(WAVERIX) (WARNING) 이것을 사용하는 시점, 각 유저 Transaction상태 검사가 필요하다.
    // (차후 이 검사도 여기로 옮겨버리자.)

    static MemberQueryResultOperator op;
    op.Init(ePARTY_OPERATOR_TOWEROFCURSE_MIGRATE, 0, 0);
    op.SetTransaction(TR_LEAVE_FIELD_FOR_INSTANCE);
    op.SetZone(indun);
    // (WAVERIX) (090906) (CHANGES) 이 함수는 Static이 아니다. AgentPartyManager::Instance제거
    WORD partyKey = pRequestUser->GetPartyState().GetPartyKey();
    if(partyKey)
        For_eachMembers(partyKey, op);
    else
        op.MigrateTowerOfCurse(pRequestUser);

    // migrate operation
    KEYTYPE zoneKey = indun->GetKey();
    DWORD linkedServerIndex = indun->GetLinkedServerIndex();
    eZONETYPE zoneType = indun->GetType();
    //UserManager* pUserManager = UserManager::Instance();

    typedef MemberQueryResultOperator::UserStateList UserStateList;
    typedef MemberQueryResultOperator::UserState UserState;
    const UserStateList& stateList = op.GetUserStateList();
    FOREACH_CONTAINER(const UserState& state, stateList, UserStateList)
    {
        if(state.state == 0)
            continue;

        ZoneInterface* pFromZone = g_ZoneManager.FindZone(state.pUser->GetZoneKey());
        if(!pFromZone)      // 선행 조건에 의해있으면 안되는 상황이긴 하지만...
            continue;
        state.pUser->BeginTransaction(TR_LEAVE_FIELD_FOR_INSTANCE);
        pFromZone->LeaveUser(state.pUser, eSEND_SYN, zoneType, linkedServerIndex);
        state.pUser->SetTRRoomKey(zoneKey);
    }

    return RC::RC_ROOM_SUCCESS;
}

//_NA_20110630_BATTLE_ZONE_RENEWAL

void AgentPartyManager::FixStringOfPartyRoomInfo(BasePartyRoomInfo* raw_msg)
{
    raw_msg->party_room_title_[_countof(raw_msg->party_room_title_) - 1] = '\0';
    raw_msg->party_room_password_[_countof(raw_msg->party_room_password_) - 1] = '\0';
}

PartyRoomFinder*
AgentPartyManager::GetPartyRoomFinder()
{
    return party_room_finder_;
}

PartyRoomList*
AgentPartyManager::GetPartyRoomList()
{
    return party_room_list_;
}

RC::ePARTY_RESULT
AgentPartyManager::FoceLeavePartyRoom(User* request_user, User* leave_user)
{
    WORD party_key = request_user->GetPartyState().GetPartyKey();
    AgentParty* party = static_cast<AgentParty*>(FindEditableParty(party_key));
    if (party == NULL)
    {
        return RC::RC_PARTY_NOT_EXIST_PARTY;
    }

    if (party->GetMasterKey() != request_user->GetPlayerKey())
    {
        return RC::RC_PARTY_ISNOT_MASTEROFPARTY;
    }

    return LeaveParty(leave_user, eREASON_LEAVE_PARTY_BAN);
}

void
AgentPartyManager::SetMemberReadyState(User* user, bool ready_state)
{
    WORD party_key = user->GetPartyState().GetPartyKey();

    AgentParty* party = static_cast<AgentParty*>(FindEditableParty(party_key));
    if (party == NULL)
    {
        SUNLOG(eCRITICAL_LOG, __FUNCTION__" : Not found party : %d", party_key);
        return;
    }

    party->SetMemeberReadyState(user->GetPlayerKey(), ready_state);

    MSG_AG_PARTY_READY_STATE_SYN cmd_msg;
    cmd_msg.ready_state_ = ready_state;
    cmd_msg.party_key_ = party_key;
    cmd_msg.member_key_ = user->GetPlayerKey();
    user->SendToLinkedServer(&cmd_msg, sizeof(cmd_msg));
}


bool
AgentPartyManager::GetMemberReadyState(User* user)
{
    WORD party_key = user->GetPartyState().GetPartyKey();

    AgentParty* party = static_cast<AgentParty*>(FindEditableParty(party_key));
    if (party == NULL)
    {
        return false;
    }

    return !!(party->GetMemeberReadyState(user->GetPlayerKey()));
}

void
AgentPartyManager::SetMemberTeam(User* user, BYTE selected_team)
{
    WORD party_key = user->GetPartyState().GetPartyKey();

    AgentParty* party = static_cast<AgentParty*>(FindEditableParty(party_key));
    if (party == NULL)
    {
        SUNLOG(eCRITICAL_LOG, __FUNCTION__" : Not found party : %d", party_key);
        return;
    }

    party->SetMemeberTeam(user->GetPlayerKey(), selected_team);
}


BYTE
AgentPartyManager::GetMemberTeam(User* user)
{
    WORD party_key = user->GetPartyState().GetPartyKey();

    AgentParty* party = static_cast<AgentParty*>(FindEditableParty(party_key));
    if (party == NULL)
    {
        SUNLOG(eCRITICAL_LOG, __FUNCTION__" : Not found party : %d", party_key);
        return false;
    }

    return party->GetMemeberTeam(user->GetPlayerKey());
}

RC::ePARTY_RESULT
AgentPartyManager::CanCreatePartyRoom(User* user, 
                                      const BasePartyRoomInfo* party_room_info)const
{
    if (user->GetBehaveState() != PLAYER_BEHAVE_IDLE_STATE)
    {
        return RC::RC_PARTY_INVALID_PLAYER_STATE;
    }

    BattlezoneMissionTreeParser* battle_zone_parser = BattlezoneMissionTreeParser::Instance();
    const BattlezoneMissionTreeInfo* parser_info = 
        battle_zone_parser->FindData(party_room_info->map_code_, party_room_info->party_room_type_);
    if (parser_info == NULL)
    {
        return RC::RC_PARTY_NOT_INCORRECT_MAPCODE;
    }

    const sMAPINFO* map_info = MapInfoParser::Instance()->FindMapInfo(party_room_info->map_code_);
    if (map_info == NULL)
    {
        return RC::RC_PARTY_NOT_INCORRECT_MAPCODE;
    }

    if (user->GetSelectedCharLV() < map_info->minLV || user->GetSelectedCharLV() > map_info->maxLV)
    {
        return RC::RC_PARTY_ISNOT_QUALIFIED_LEVEL;
    }

    RC::ePARTY_RESULT result_value = CheckTermsByPartyRoomType(user, party_room_info);
    if (result_value != RC::RC_PARTY_SUCCESS)
    {
        return result_value;
    }

    return RC::RC_PARTY_SUCCESS;
}

RC::ePARTY_RESULT
AgentPartyManager::CheckTermsByPartyRoomType(User* request_user,
                                             const BasePartyRoomInfo* party_room_info)const
{
    BattlezoneMissionTreeParser* const parser = BattlezoneMissionTreeParser::Instance();
    const eZONETYPE party_zone_type = parser->GetZoneType(party_room_info->map_code_);
    switch(party_zone_type)
    {
    case eZONETYPE_INSTANCE:
        {
            const sMAPINFO* const map_info = 
                MapInfoParser::Instance()->FindMapInfo(party_room_info->map_code_);
            if (map_info == NULL)
            {
                return RC::RC_PARTY_NOT_INCORRECT_MAPCODE;
            }
            const eZONE_RULESET& rule_set = map_info->Ruleset;
            static MemberOperator member_operator;
            AgentPartyManager* const party_manager = AgentPartyManager::Instance();

            member_operator.Init(ePARTY_OPERATOR_MIGRATE_CHECK, 0, 0);
            party_manager->For_eachMembers(request_user->GetPartyState().GetPartyKey(), member_operator);
            if (member_operator.IsSuccess() == false)
            {
                return RC::RC_PARTY_CANNOT_JOIN_SOMEONE_MEMBER;
            }

            if (rule_set.rules & rule_set.ZONE_RULE_INDUN_SHADOW_NOT_PLAYING_MODE)
            {
                if (request_user->GetCharClass() == eCHAR_SHADOW)
                {
                    return RC::RC_PARTY_CANNOT_JOIN_SOMEONE_MEMBER;
                }

                member_operator.Init(ePARTY_OPERATOR_SHADOW_CHECK, 0, 0);
                party_manager->For_eachMembers(request_user->GetPartyState().GetPartyKey(), member_operator);
                if (member_operator.IsSuccess() == false)
                {
                    return RC::RC_PARTY_CANNOT_JOIN_SOMEONE_MEMBER;
                }
            }

            if (party_room_info->party_room_type_ == PartyOfTopOfTrialByNormal)
            {
                if (!(g_BattleOpenTimeMng.IsOpenDungeon(map_info->wTimeLimit)))
                {
                    return RC::RC_PARTY_NOT_OPEN_TIME;
                }
            }
        }
        break;
    case eZONETYPE_MISSION:
        {
#ifdef _NA_007156_20140703_TOP_OF_TRIAL_LIGHT
            const sMAPINFO* const map_info = 
                MapInfoParser::Instance()->FindMapInfo(party_room_info->map_code_);
            if (map_info == NULL) {
                return RC::RC_PARTY_NOT_INCORRECT_MAPCODE;
            }

            const eZONE_RULESET& rule_set = map_info->Ruleset;
            static MemberOperator member_operator;
            AgentPartyManager* const party_manager = AgentPartyManager::Instance();

            member_operator.Init(ePARTY_OPERATOR_MIGRATE_CHECK, 0, 0);
            party_manager->For_eachMembers(request_user->GetPartyState().GetPartyKey(), member_operator);
            if (member_operator.IsSuccess() == false) {
                return RC::RC_PARTY_CANNOT_JOIN_SOMEONE_MEMBER;
            }

            if (party_room_info->party_room_type_ == PartyOfTopOfTrialLight_normal) 
            {
                if (!(g_BattleOpenTimeMng.IsOpenDungeon(map_info->wTimeLimit))) {
                    return RC::RC_PARTY_NOT_OPEN_TIME;
                }
            }
#endif //_NA_007156_20140703_TOP_OF_TRIAL_LIGHT
        }
        break;

    case eZONETYPE_PVP:
        break;

    case eZONETYPE_FIELD:
        break;

    default:
        SUNLOG(eCRITICAL_LOG, 
               __FUNCTION__" : Invalid party room type : %d", 
               party_zone_type);
        return RC::RC_PARTY_INVALID_ROOMTYPE;
    }   

    return RC::RC_PARTY_SUCCESS;
}

RC::ePARTY_RESULT
AgentPartyManager::CreatePartyRoom(BasePartyRoomInfo& party_room_info, User* user)
{ 
    RC::ePARTY_RESULT party_result = RC::RC_PARTY_SUCCESS;
    MAPCODE map_code = party_room_info.map_code_;

    BaseParty* base_party = CreateParty(user->GetPlayerKey() , ePARTY_ROOM);
    if (base_party == NULL)
    {
        return RC::RC_PARTY_INVALID_ROOMTYPE;
    }

    WORD party_key = base_party->GetPartyKey();

    //파티룸 생성 이전에 파티에 소속된 파티장이라면 이전 파티원들에게 초청 메세지를 보낸다.
    if (user->GetPartyState().IsPartyMember() == true)
    {
        WORD previous_party_key = user->GetPartyState().GetPartyKey();
        const BaseParty* previous_party = FindParty(previous_party_key);
        if (previous_party)
        {
            if (previous_party->GetMasterKey() == user->GetPlayerKey())
            {
                //Master
                MSG_CG_PARTY_INVITE_PARTY_ROOM_REQ req_msg;
                req_msg.invite_party_key_ = party_key;
                SendPacketAllMember(previous_party_key, 
                    &req_msg, 
                    sizeof(req_msg), 
                    previous_party->GetMasterKey());

                LeaveParty(user, eREASON_LEAVE_PARTY_INTEND);
            }
            else
            {
                //Member
                LeaveParty(user, eREASON_LEAVE_PARTY_INTEND);
            }
        }
    }

    party_room_info.special_type_ = eROOM_SPECIAL_NORMAL;
    if (user->CanApplyPCBangBenefit(true))
    {
        party_room_info.special_type_ = eROOM_SPECIAL_PCBANG_SUPER_ROOM;
    }
    else if (user->GetItemOwnFlag().FindItemInfo(eITEMTYPE_SUPERMASTER))
    {
        party_room_info.special_type_ = eROOM_SPECIAL_ITEM1_SUPER_ROOM;
    }

#ifdef _NA_004694_20120412_1202_PARTY_ROOM_KEY
    WORD room_key = 0;
    if (party_room_list_->CreatePartyRoom(party_room_info, base_party->GetPartyKey() ,room_key) == false)
    {
        return RC::RC_PARTY_FAIL_CREATE_PARTY_ROOM;
    }
    else
    {
        MSG_AW_PARTY_ROOM_KEY_SYN message_aw;
        message_aw.party_key_ = party_key;
        message_aw.room_key_ = room_key;
        AgentServer::GetInstance()->SendToWorldServer(&message_aw, sizeof(message_aw));
    }
#else
    PartyRoomList* party_room_list = GetPartyRoomList();
    if (party_room_list->CreatePartyRoom(party_room_info, base_party->GetPartyKey()) == false)
    {
        return RC::RC_PARTY_FAIL_CREATE_PARTY_ROOM;
    }
#endif  //_NA_004694_20120412_1202_PARTY_ROOM_KEY
    party_result = JoinParty(party_key, user);
    if (party_result != RC::RC_PARTY_SUCCESS)
    {
        DestroyParty(party_key);
        return party_result;
    }

    if (map_code != 0)
    {
#ifdef _NA_004706_20120413_1202_SECRET_ROOM_DONOT_SEND_MESSAGE
        const bool is_private_room = (strlen(party_room_info.party_room_password_) == 0) ? false : true;
        party_room_finder_->UpdateSelectedMapCodeOfParty(0, map_code, party_key,is_private_room);
#else
        PartyRoomFinder* party_room_finder = GetPartyRoomFinder();
        party_room_finder->UpdateSelectedMapCodeOfParty(0, map_code, party_key);
#endif  // _NA_004706_20120413_1202_SECRET_ROOM_DONOT_SEND_MESSAGE
    }

    return party_result;
}

#ifdef _NA_20111220_CHANGE_DISCONNECTED_USER_PROCESS
RC::ePARTY_RESULT
AgentPartyManager::LeavePartyRoom(User* user, eREASON_LEAVE_PARTY reason_to_leave)
#else
RC::ePARTY_RESULT
AgentPartyManager::LeavePartyRoom(User* user)
#endif
{
    WORD party_key = user->GetPartyState().GetPartyKey();
    if (party_room_list_->LeavePartyRoom(party_key) == false )
    {
        return RC::RC_PARTY_FAIL_LEAVE_PARTY_ROOM;
    }

    AgentParty* party = static_cast<AgentParty*>(FindEditableParty(party_key));
    if (party == NULL)
    {
        return RC::RC_PARTY_NOT_EXIST_PARTY;
    }

    if (user->GetTeam() != 0)
    {
        user->SetTeam(0);
    }

    user->SetIsRecvInvitePartyRoom(true);

    MSG_CG_PARTY_LEAVE_ROOM_BRD brd_msg;
#ifdef _NA_20111220_CHANGE_DISCONNECTED_USER_PROCESS
    brd_msg.reason_to_leave_ = reason_to_leave;
#endif
    brd_msg.member_key_ = user->GetPlayerKey();
    party->SendPacketAll(&brd_msg, sizeof(brd_msg));

    return RC::RC_PARTY_SUCCESS;
}

void
AgentPartyManager::DestroyPartyRoom(WORD party_key)
{
    MAPCODE map_code = party_room_list_->GetMapCodeOfPartyRoom(party_key);
    if (map_code != 0)
    {
        party_room_finder_->DeletePartyKeyFromMissionList(map_code, party_key);
    }
    party_room_list_->DestroyPartyRoom(party_key);    
}

RC::ePARTY_RESULT
AgentPartyManager::JoinPartyRoom(User* user, WORD party_key, PartyZoneType party_room_type)
{
    PartyRoomList* room_list = GetPartyRoomList();
    if (room_list->JoinPartyRoom(user, party_key) == false)
    {
        return RC::RC_PARTY_FAIL_JOIN_PARTY_ROOM;
    }

    if (party_room_type == PartyOfPvPByTeam)
    {
        SetMemberTeam(user, user->GetTeam());
    }

    user->SetIsRecvInvitePartyRoom(false);

    return RC::RC_PARTY_SUCCESS;
}

void
AgentPartyManager::ResetRoomMissionCode(WORD party_key)
{
    const PartyRoom* party_room = FindPartyRoom(party_key);
    if (party_room == NULL)
    {
        SUNLOG(eCRITICAL_LOG, __FUNCTION__" : Not found party room : %d", party_key);
        return;
    }
    MAPCODE map_code = party_room->GetMapCode();

#ifdef _NA_004706_20120413_1202_SECRET_ROOM_DONOT_SEND_MESSAGE
    party_room_finder_->UpdateSelectedMapCodeOfParty(map_code, 0, 
                                                     party_key, party_room->IsPrivateRoom());
#else
    party_room_finder_->UpdateSelectedMapCodeOfParty(map_code, 0, party_key);
#endif  // _NA_004706_20120413_1202_SECRET_ROOM_DONOT_SEND_MESSAGE
    
    party_room_list_->SetMissionCode(party_key, 0);
}

void                
AgentPartyManager::CheckLevelForMission(WORD party_key, MAPCODE map_code)
{
    AgentParty* party = static_cast<AgentParty*>(FindEditableParty(party_key));
    if (party == NULL)
    {
        SUNLOG(eCRITICAL_LOG, __FUNCTION__" : Not found party : %d", party_key);
        return;
    }

    const sMAPINFO* map_info = MapInfoParser::Instance()->FindMapInfo(map_code);
    if (map_info == NULL)
    {
        SUNLOG(eCRITICAL_LOG, __FUNCTION__" : Not found map info : %d", map_code);
        return;
    }
   
    static MemberOperator member_operator;
    member_operator.Init(ePARTY_OPERATOR_KICK_USER_AFTER_CHECK_LEVEL, 0, 0);
    member_operator.SetFitnessLV(map_info->minLV, map_info->maxLV);
    party->For_eachMembers(member_operator);
}

RC::ePARTY_RESULT
AgentPartyManager::ChangePartyRoomOption(User* user, WORD party_key, 
                                         const MSG_AG_PARTY_CAN_CHANGE_OPTION_ACK* recv_msg)
{
    AgentParty* party = static_cast<AgentParty*>(FindEditableParty(party_key));
    if (party == NULL)
    {
        return RC::RC_PARTY_NOT_EXIST_PARTY;
    }

    if (party->GetMasterKey() != user->GetPlayerKey())
    {
        return RC::RC_PARTY_ISNOT_MASTEROFPARTY;
    }

    const BasePartyRoomInfo& room_info = recv_msg->party_room_info_;
    MAPCODE delete_map_code = party_room_list_->GetMapCodeOfPartyRoom(party_key);

#ifdef _NA_004706_20120413_1202_SECRET_ROOM_DONOT_SEND_MESSAGE
    bool is_private_room = (strlen(room_info.party_room_password_) == 0) ? false : true;

    bool result_value = party_room_finder_->UpdateSelectedMapCodeOfParty(
        delete_map_code, 
        room_info.map_code_, 
        party_key, 
        is_private_room
        );
#else
    bool result_value = party_room_finder_->UpdateSelectedMapCodeOfParty(delete_map_code, 
                                                                         room_info.map_code_, 
                                                                         party_key);
#endif  // _NA_004706_20120413_1202_SECRET_ROOM_DONOT_SEND_MESSAGE

    if (result_value == false)
    {
        return RC::RC_PARTY_FAIL_DESTROY_PARTY_LIST;
    }

    const PartyRoom* party_room = party_room_list_->FindPartyRoom(party_key);
    if (party_room == NULL)
    {
        return RC::RC_PARTY_NOT_FOUND_PARTYROOM;
    }

    RC::ePARTY_RESULT result_code = CheckTermsByPartyRoomType(user, &(room_info));
    if (result_code != RC::RC_PARTY_SUCCESS)
    {
        return result_code;
    }

    if (party_room_list_->UpdateBasePartyRoomInfo(room_info, party_key) == false)
    {
        return RC::RC_PARTY_FAIL_CHANGE_ROOM_OPTION;
    }

    KickPartyMemberForChangeOption(recv_msg, user);
    ResetPartyRoomReadyState(user, party_key);

    return RC::RC_PARTY_SUCCESS;
}

KEYTYPE
AgentPartyManager::GetPartyRoomKey(WORD party_key)
{
    const PartyRoom* party_room =  party_room_list_->FindPartyRoom(party_key);
    if (party_room == NULL)
    {
        SUNLOG(eCRITICAL_LOG, __FUNCTION__" : Not found party : %d", party_key);
        return 0;
    }

    return party_room->GetPartyRoomInfo()->room_key_;
}

const PartyRoomInfo*
AgentPartyManager::GetPartyRoomInfoByMemberKey(DWORD member_key)
{
    User* user = UserManager::Instance()->GetUserByObjKey(member_key);
    if (user == NULL)
    {
        SUNLOG(eCRITICAL_LOG, __FUNCTION__" : Not found user");
        return NULL;
    }

    WORD party_key = user->GetPartyState().GetPartyKey();
    const PartyRoom* party_room = party_room_list_->FindPartyRoom(party_key);
    if (party_room == NULL)
    {
        SUNLOG(eCRITICAL_LOG, __FUNCTION__" : Not found party room : %d", party_key);
        return NULL;
    }

    return party_room->GetPartyRoomInfo();
}

RC::ePARTY_RESULT
AgentPartyManager::CanStartRoom(User* user)
{
    if (user->IsBeginTransaction())
    {
        return RC::RC_PARTY_ALREADY_DOING_TRANSACTION;
    }

    WORD party_key = user->GetPartyState().GetPartyKey();
    AgentParty* party = static_cast<AgentParty*>(FindEditableParty(party_key));
    if (party == NULL)
    {
        return RC::RC_PARTY_NOT_EXIST_PARTY;
    }

    if (party->GetType() != ePARTY_ROOM)
    {
        return RC::RC_PARTY_IS_NOT_PARTY_ROOM;
    }

    if (IsMaster(user) == false)
    {
        return RC::RC_PARTY_ISNOT_MASTEROFPARTY;        
    }

    if (user->GetBehaveState() != PLAYER_BEHAVE_IDLE_STATE)
    {
        return RC::RC_PARTY_INVALID_PLAYER_STATE;
    }

    if (party->IsReadyToAllMember() == false)
    {
        return RC::RC_PARTY_NOT_ALLMEMBER_READY;
    }

    const PartyRoom* party_room = party_room_list_->FindPartyRoom(party_key);
    if (party_room == NULL)
    {
        return RC::RC_PARTY_NOT_FOUND_PARTYROOM;
    }

    if (party_room->IsStartWaitingRoom() == true)
    {
        return RC::RC_PARTY_FAIL_CHECK_TERMS;
    }

    MAPCODE map_code = party_room->GetMapCode();
    BattlezoneMissionTreeParser* battle_zone_parser = BattlezoneMissionTreeParser::Instance();
    const BattlezoneMissionTreeInfo* parser_info = battle_zone_parser->FindData(map_code);
    if (parser_info == NULL)
    {
        SUNLOG(eCRITICAL_LOG, __FUNCTION__" : Not found battle zone parser info : %d", map_code);
        return RC::RC_PARTY_NOT_INCORRECT_MAPCODE;
    }

    const sMAPINFO* map_info = MapInfoParser::Instance()->FindMapInfo(map_code);
    if (map_info == NULL)
    {
        SUNLOG(eCRITICAL_LOG, __FUNCTION__" : Not found map info parser : %d", map_code);
        return RC::RC_PARTY_NOT_INCORRECT_MAPCODE;
    }

    if (user->GetSelectedCharLV() < map_info->minLV || user->GetSelectedCharLV() > map_info->maxLV)
    {
        return RC::RC_PARTY_ISNOT_QUALIFIED_LEVEL;
    }

    return RC::RC_PARTY_SUCCESS;
}

bool 
AgentPartyManager::ExceptionProcessByPartyMember(const MSG_AG_ZONE_CAN_CREATE_ROOM_OF_PARTY_ACK* check_msg,
                                                 User* request_user)
{
    typedef MSG_AG_ZONE_CAN_CREATE_ROOM_OF_PARTY_ACK CheckMsg;

    const CheckMsg::MemberInfo* check_members = check_msg->member_info_;
    BYTE number_of_partymember = check_msg->number_of_partymember_;
    if (number_of_partymember <= 0 || number_of_partymember > _countof(check_msg->member_info_))
    {
        return false;
    }

    const CheckMsg::MemberInfo* it_src = check_members;
    const CheckMsg::MemberInfo* it_end = &check_members[number_of_partymember];

    CheckMsg::MemberInfo member_result_list[_countof(check_msg->member_info_)];
    ZeroMemory(&member_result_list, sizeof(member_result_list));
    CheckMsg::MemberInfo* it_dest = member_result_list;

    typedef AgentParty::MEMBER_MAP MemberList;
    WORD party_key = check_msg->party_key_;
    AgentParty* party = static_cast<AgentParty*>(FindEditableParty(party_key));
    if (party == NULL)
    {
        SUNLOG(eCRITICAL_LOG, __FUNCTION__" : Not found party : %d", party_key);
        return false;
    }

    const MemberList& memeber_list = party->GetMemberList();
    MSG_CG_ZONE_MISSION_CREATE_NAK nak_msg;

    DWORD search_number = 0, result_number = 0, valid_number = 0;
    BOOLEAN requester_notified = false;
    for (; it_src != it_end; ++it_src, ++it_dest)
    {
        it_dest->object_key_ = it_src->object_key_;
        MemberList::const_iterator find_it = memeber_list.find(it_src->object_key_);
        const _PARTY_MEMBER_INFO* memeber_info = 
            (find_it != memeber_list.end()) ? &(find_it->second) : NULL;

        User* user = NULL;
        if (memeber_info == NULL)
        {
            it_dest->status_ = check_msg->eStatus_PartialFail;
        }
        else
        {
            user = UserManager::Instance()->GetUserByObjKey(memeber_info->m_dwMemberKey);
            if (user == NULL)
            {
                it_dest->status_ = check_msg->eStatus_PartialFail;                
            }
            else
            {
                it_dest->status_ = it_src->status_;
                ++result_number;
            }
        }
        ++search_number;

        if (it_dest->status_ == check_msg->eStatus_PartialOk)
        {
            if(user->IsBeginTransaction())
                it_dest->status_ = check_msg->eStatus_Transaction;
            else
                ++valid_number;
        }

        if (it_dest->status_ != check_msg->eStatus_PartialOk)
        {
            switch (it_dest->status_)
            {
            case CheckMsg::eStatus_PartialFail:
                nak_msg.m_dwErrorCode = RC::RC_ROOM_INVALID_PLAYER_STATE;
                break;
            case CheckMsg::eStatus_Transaction:
                nak_msg.m_dwErrorCode = RC::RC_ROOM_ALREADY_DOING_TRANSACTION;
                break;
            case CheckMsg::eStatus_LevelViolation:
                nak_msg.m_dwErrorCode = RC::RC_ROOM_INVALID_LIMITLEVEL_FOR_ME;
                break;
            case CheckMsg::eStatus_RequireQuestClear:
                nak_msg.m_dwErrorCode = RC::RC_ROOM_PREVIOUS_QUEST_CLEAR;
                break;
            case CheckMsg::eStatus_RequireMissionClear:
                nak_msg.m_dwErrorCode = RC::RC_ROOM_NEED2_CLEAR_PRE_MISSION;
                break;
            }
            if (user)
            {
                requester_notified = user == request_user;
                user->SendPacket(&nak_msg, sizeof(nak_msg));
            }
        }
    }

    if (search_number == 0 || search_number != result_number || result_number != valid_number)
    {
        if (requester_notified == false)
        {
            nak_msg.m_dwErrorCode = RC::RC_ROOM_CANNOT_JOIN_SOMEONE_PARTY_MEMBER;
            request_user->SendPacket(&nak_msg, sizeof(nak_msg));
        }
        return false;
    }
    return true;
}

void 
AgentPartyManager::KickPartyMemberForChangeOption(const MSG_AG_PARTY_CAN_CHANGE_OPTION_ACK* check_msg,
                                                  User* master_user)
{
    typedef MSG_AG_PARTY_CAN_CHANGE_OPTION_ACK CheckMsg;

    const CheckMsg::MemberState* check_members = check_msg->member_state_;
    BYTE number_of_partymember = check_msg->number_of_partymember_;
    if (number_of_partymember <= 0 || number_of_partymember > _countof(check_msg->member_state_))
    {
        SUNLOG(eCRITICAL_LOG, __FUNCTION__" : Invalid number of member count : %d", 
               number_of_partymember);
        return;
    }

    const CheckMsg::MemberState* begin_list = check_members;
    const CheckMsg::MemberState* end_list = &check_members[number_of_partymember];

    typedef AgentParty::MEMBER_MAP MemberList;
    WORD party_key = master_user->GetPartyState().GetPartyKey();
    AgentParty* party = static_cast<AgentParty*>(FindEditableParty(party_key));
    if (party == NULL)
    {
        SUNLOG(eCRITICAL_LOG, __FUNCTION__" : Not found party : %d", party_key);
        return;
    }

    const MemberList& memeber_list = party->GetMemberList();
    for (; begin_list != end_list; ++begin_list)
    {
        MemberList::const_iterator find_iterator = memeber_list.find(begin_list->object_key_);
        if (find_iterator == memeber_list.end())
        {
            continue;
        }

        if (begin_list->status_ != check_msg->eStatus_PartialOk)
        {
            const UserManager* const user_manager = UserManager::Instance();
            User* const kick_user = user_manager->GetUserByObjKey(begin_list->object_key_);
            if (kick_user)
            {
                eREASON_LEAVE_PARTY reason_to_leave;

                switch(begin_list->status_)
                {
                case CheckMsg::eStatus_LevelViolation:
                    reason_to_leave = eREASON_LEAVE_PARTY_NOT_MATCH_LEVEL;
                    break;
                case CheckMsg::eStatus_RequireMissionClear:
                    reason_to_leave = eREASON_LEAVE_PARTY_NOT_CLEAR_MISSION;
                    break;
                case CheckMsg::eStatus_RequireQuestClear:
                    reason_to_leave = eREASON_LEAVE_PARTY_NOT_CLEAR_QUEST;
                    break;
                default:
                    reason_to_leave = eREASON_LEAVE_PARTY_NOREASON;
                    break;
                }

                LeaveParty(kick_user, reason_to_leave);                
            }
        }
    }
}

const PartyRoom*
AgentPartyManager::FindPartyRoom(WORD party_key)const
{
    const PartyRoom* party_room = party_room_list_->FindPartyRoom(party_key);
    if (party_room == NULL)
    {
        return NULL;
    }

    return party_room;

}

void
AgentPartyManager::SetIsMissionStart(WORD party_key, bool is_start)
{
    PartyRoom* party_room = party_room_list_->_FindEditableNode(party_key);
    if (party_room == NULL)
    {
        SUNLOG(eCRITICAL_LOG, __FUNCTION__" : Not found party room : %d", party_key);
        return;
    }

    party_room->SetIsMissionStart(is_start);    
}

void 
AgentPartyManager::CreateNoneTypePartyRoom(User* master_user, const BasePartyRoomInfo& base_info)
{
    BasePartyRoomInfo copied_party_room_info = base_info;
    FixStringOfPartyRoomInfo(&copied_party_room_info);
    RC::ePARTY_RESULT result_code = CreatePartyRoom(copied_party_room_info, master_user);
    if (result_code != RC::RC_PARTY_SUCCESS)
    {
        MSG_CG_PARTY_CREATE_ROOM_ACK nak_msg;
        nak_msg.result_code_ = result_code;
        master_user->SendPacket(&nak_msg, sizeof(nak_msg));
        SUNLOG(eFULL_LOG, __FUNCTION__" : CreatePartyRoom() Process Fail : %d", result_code);
        return;
    }

    const PartyRoomInfo* party_room_info = GetPartyRoomInfoByMemberKey(master_user->GetPlayerKey());
    if (party_room_info == NULL)
    {
        SUNLOG(eCRITICAL_LOG, __FUNCTION__" : Not found room info");
        return;
    }

    MSG_CG_PARTY_CREATE_ROOM_ACK ack_msg;
    ack_msg.result_code_ = result_code;
    ack_msg.party_room_info_ = *(party_room_info);
    master_user->SendPacket(&ack_msg, sizeof(ack_msg));
}

void
AgentPartyManager::ResetPartyRoomReadyState(User* master_user, WORD party_key)
{
    BaseParty* party = FindEditableParty(party_key);
    if (party == NULL)
    {
        SUNLOG(eCRITICAL_LOG, __FUNCTION__" : Not found party : %d", party_key);
        return;
    }

    party->ResetReadyStateAllMember();

    MSG_AG_PARTY_RESET_READY_STATE_CMD cmd_msg;
    cmd_msg.master_key_ = master_user->GetPlayerKey();
    cmd_msg.party_key_ = party_key;
    master_user->SendToLinkedServer(&cmd_msg, sizeof(cmd_msg));
    
    MSG_CG_PARTY_RESET_READY_STATE_CMD brd_msg;
    SendPacketAllMember(party_key, &brd_msg, sizeof(brd_msg));
}

void
AgentPartyManager::NotifyMemberMoveArea(User* user, MAPCODE map_code)
{
    WORD party_key = user->GetPartyState().GetPartyKey();
    DWORD member_key = user->GetPlayerKey();

    BaseParty* party = FindEditableParty(party_key);
    if (party == NULL)
    {
        SUNLOG(eCRITICAL_LOG, __FUNCTION__" : Not found party : %d", party_key);
        return;
    }
    party->SetMemberMapCode(member_key, map_code);

    MSG_CG_PARTY_PLAYER_MOVE_FIELD_BRD brd_msg;
    brd_msg.member_key_ = member_key;
    brd_msg.map_code_ = map_code;
    SendPacketAllMember(party_key, &brd_msg, sizeof(brd_msg));
}

void 
AgentPartyManager::AddPartyRoomToWaitList(WORD party_key)
{
    party_room_list_->SetPartyRoomStateForWaitingToStart(party_key);
}

void                
AgentPartyManager::StartPartyRoom(User* request_user)
{
    WORD party_key = request_user->GetPartyState().GetPartyKey();
    const PartyRoom* party_room = FindPartyRoom(party_key);
    if (party_room == NULL)
    {
        SUNLOG(eCRITICAL_LOG, 
               __FUNCTION__" : Not found party room : Key : %d, User : %s",
               party_key, request_user->GetSelectedCharName());
        return;
    }

    eZONETYPE party_room_type = party_room->GetZoneType();

    switch(party_room_type)
    {
    case eZONETYPE_MISSION:
        g_ZoneManager.ProcessCreateMissionRoom(request_user);
        break;

    case eZONETYPE_INSTANCE:
        g_ZoneManager.ProcessCreateInstanceDungeonRoom(request_user);
        break;

    case eZONETYPE_PVP:
        g_ZoneManager.ProcessCreatePVPRoom(request_user);
        break;

    default:
        SUNLOG(eCRITICAL_LOG, __FUNCTION__" : Invalid party room type : %d", party_room_type);
        return;
    }
}

void AgentPartyManager::DoStartToWaitingRoom()
{
    if (check_waiting_room_timer_.IsExpired() == true)
    {
        typedef PartyRoomWaitingList::StartWaitingRoomList WaitRoomLIst;

        PartyRoomList* party_room_list = GetPartyRoomList();
        PartyRoomWaitingList* wait_room_list = party_room_list->GetStartWaitingRoomList();
        WaitRoomLIst* room_list = wait_room_list->GetEditableWaitingRoomList();

        WaitRoomLIst::iterator begin_iterator = room_list->begin(),
                               end_iterator = room_list->end();
        while (begin_iterator != end_iterator)
        {
            const WORD find_party_key = *(begin_iterator);
            PartyRoom* party_room = const_cast<PartyRoom*>(FindPartyRoom(find_party_key));
            if (party_room && party_room->IsEndRoomForWaitingToStart() == true)
            {
                const BaseParty* party = FindParty(party_room->GetPartyKey());
                if (party == NULL)
                {
                    SUNLOG(eCRITICAL_LOG, 
                           __FUNCTION__" : Not found party : key : %d",
                           party_room->GetPartyKey());
                    continue;
                }

                UserManager* user_manager = UserManager::Instance();
                User* master_user = user_manager->GetUserByObjKey(party->GetMasterKey());
                if (master_user == NULL)
                {
                    SUNLOG(eCRITICAL_LOG, 
                           __FUNCTION__" : Not found user : key : %d",
                           party->GetMasterKey());
                    continue;
                }
                StartPartyRoom(master_user);
                begin_iterator = room_list->erase(begin_iterator);
                continue;
            }
            ++begin_iterator;
        }
    }
}

void
AgentPartyManager::AddClosingUser(User* user, const _PARTY_MEMBER_INFO& member_info)
{
    WORD party_key = user->GetPartyState().GetPartyKey();
    AgentParty* party = static_cast<AgentParty*>(FindEditableParty(party_key));
    if (party == NULL)
    {
        SUNLOG(
            eCRITICAL_LOG, 
            __FUNCTION__" : Not found party : key : %d",
            party->GetPartyKey()
            );
        return;
    }

    PartyReturnInfo& party_return_info = user->GetPartyReturnInfo();

    ClosingUserList& closing_user_list = party->GetClosingUserList();
    closing_user_list.AddClosingUser(user->GetSelectedCharGuid(),
                                     user->GetSelectedCharName(),
                                     party_return_info,
                                     member_info);
}

void 
AgentPartyManager::ChangeMasterOfPartyRoom(User* next_party_master)
{
    const WORD party_key = next_party_master->GetPartyState().GetPartyKey();
    PartyRoom* party_room = party_room_list_->_FindEditableNode(party_key);
    if (party_room == NULL)
    {
        SUNLOG(
            eCRITICAL_LOG,
            __FUNCTION__" : Not found party room to edit : request char : %s, party key : %d",
            next_party_master->GetSelectedCharName(),
            party_key
            );
        return;
    }

    PartyRoomInfo* party_room_info = party_room->GetEditablePartyRoomInfo();

    party_room_info->base_party_room_info.special_type_ = eROOM_SPECIAL_NORMAL;
    if (next_party_master->CanApplyPCBangBenefit(true))
    {
        party_room_info->base_party_room_info.special_type_ = eROOM_SPECIAL_PCBANG_SUPER_ROOM;
    }
    else if (next_party_master->GetItemOwnFlag().FindItemInfo(eITEMTYPE_SUPERMASTER))
    {
        party_room_info->base_party_room_info.special_type_ = eROOM_SPECIAL_ITEM1_SUPER_ROOM;
    }
}


RC::ePARTY_RESULT
AgentPartyManager::LeaveParty(User* user, eREASON_LEAVE_PARTY reason_to_leave)
{
    WORD party_key = user->GetPartyState().GetPartyKey();
    AgentParty* party = static_cast<AgentParty*>(FindEditableParty(party_key));
    if (party == NULL)
    {
        return RC::RC_PARTY_PARTY_NOTEXIST;
    }

    RC::ePARTY_RESULT result_code = RC::RC_PARTY_SUCCESS;
    //_NA_20110630_BATTLE_ZONE_RENEWAL
    if (party->GetType() == ePARTY_ROOM)
    {
  #ifdef _NA_20111220_CHANGE_DISCONNECTED_USER_PROCESS 
        result_code = LeavePartyRoom(user, reason_to_leave);
  #else
        result_code = LeavePartyRoom(user);
  #endif
        if (result_code != RC::RC_PARTY_SUCCESS)
        {
            SUNLOG(
                eCRITICAL_LOG, 
                __FUNCTION__" : Fail leave party room : name : %s, result code : %d", 
                user->GetSelectedCharName(), result_code
                );
            return result_code;
        }
    }

    const _PARTY_MEMBER_INFO* member_info = party->GetMemberInfo(user->GetPlayerKey());
    if (member_info == NULL)
    {
        SUNLOG(
            eCRITICAL_LOG,
            __FUNCTION__" : Not found party member : party key : %d, member name : %s",
            party_key, user->GetSelectedCharName()
            );
        return RC::RC_PARTY_ISNOT_MEMBEROFPARTY;
    }

    _PARTY_MEMBER_INFO copied_member_info = *(member_info);

    result_code = BasePartyManager::LeaveParty(party_key, user->GetPlayerKey());
    if (result_code != RC::RC_PARTY_SUCCESS)
    {
        return result_code;
    }

    if (reason_to_leave == eREASON_LEAVE_PARTY_ABNORMAL)
    {
        if (user->IsDead() == true)
        {
            reason_to_leave = eREASON_LEAVE_PARTY_INTEND;
        }
        else
        {
            AddClosingUser(user, copied_member_info);
        }
    }
    else if (reason_to_leave == eREASON_LEAVE_PARTY_INTEND)
    {
        PortalManager::OnLeaveParty(user);
    }

    MSG_CG_PARTY_LEAVE_BRD brd_msg;
    brd_msg.m_dwObjKey = user->GetPlayerKey();
    brd_msg.m_byReason = static_cast<BYTE>(reason_to_leave);
    user->SendPacket(&brd_msg, sizeof(brd_msg));

    //user->GetPartyState().Init();    // 파티장 위임 전에 초기화 하면 안됨;;

    BYTE number_of_member = party->GetActiveMemberCount();
    if (number_of_member == 0)
    {
        user->GetPartyState().Init();
        DestroyParty(party_key);
        return RC::RC_PARTY_SUCCESS;
    }

    const _PARTY_MEMBER_INFO* head_member_info = party->GetHeadUser();
    if (head_member_info == NULL)
    {
        user->GetPartyState().Init();
        return RC::RC_PARTY_INVALID_PARTY_STATE;
    }

    UserManager* const user_manager = UserManager::Instance();
    User* member_user = user_manager->GetUserByObjKey(head_member_info->m_dwMemberKey);
    if (member_user == NULL)
    {
        user->GetPartyState().Init();
        return RC::RC_PARTY_NOT_FOUND_USER;
    }

    const eZONETYPE zone_type = member_user->GetZoneType();

    //_NA_20110630_BATTLE_ZONE_RENEWAL
    if (party->GetType() != ePARTY_ROOM && number_of_member == 1 && member_user)
    {
        //다음의 존타입에서는 파티멤버가 한명뿐일 경우 파티를 해체시킨다
        if (zone_type == eZONETYPE_VILLAGE ||
            zone_type == eZONETYPE_DOMINATION_FIELD ||
            zone_type == eZONETYPE_FIELD ||
            zone_type == eZONETYPE_SPA_FIELD
            )
        {
            user->GetPartyState().Init();
            DestroyParty(party_key);
            return RC::RC_PARTY_SUCCESS;
        }
    }

    RemoveMemberToSession(party, user->GetServerSessionIndex(), -1, user);
    SessionMemberMgr& session_member_manager = party->GetSessionMemberMgr();
    if (session_member_manager.LeaveMember(user->GetPlayerKey()) == false)
    {
        SUNLOG(eCRITICAL_LOG, 
            __FUNCTION__" : Fail session member manager's leave member : name : %s",
            user->GetSelectedCharName());
    }
    SendPacketAllMember(party_key, &brd_msg, sizeof(brd_msg));

    MSG_AW_PARTY_WITHDRAW_MEMBER_SYN aw_mag;
    aw_mag.wPartyKey = party_key;
    aw_mag.m_CharGUID = user->GetSelectedCharGuid();
#ifdef _NA_000068_20100419_PARTY_FINDING_SYSTEM
    aw_mag.party_master_id_ = scoped_get_char_guid(party->GetMasterKey());
    aw_mag.num_of_joined_member_ = scoped_to_party_num_of_member(party->GetMemberNum());
#endif // _NA_000068_20100419_PARTY_FINDING_SYSTEM
    user->SendToWorldServer(&aw_mag, sizeof(aw_mag));

    KEYTYPE zone_key = user->GetZoneKey();
    ZoneInterface* zone = g_ZoneManager.FindZone(zone_key);

    //파티 마스터가 파티를 탈퇴 했을 경우 처리
    if (party->GetMasterKey() == user->GetPlayerKey())
    {
        if (zone)
        {
            User* next_master_user = 
                static_cast<RoomInterface*>(zone)->ChangeSpecialType(false, user);

            if (next_master_user)
            {
                //우선적으로 미션에 존재 하는 인원에게 파티 마스터권한을 준다.
                ChangeMaster(party_key, next_master_user->GetPlayerKey());
            }
            else
            {
                ChangeMaster(party_key, party->GetHeadUser()->m_dwMemberKey);
            }
        }
        //_NA_20110630_BATTLE_ZONE_RENEWAL
        ResetRoomMissionCode(party_key);
    }

    // 유저의 파티상태 초기화
    user->GetPartyState().Init();

    //파티 탈퇴시 인원변경에 관한 처리
    if (reason_to_leave != eREASON_LEAVE_PARTY_ABNORMAL && zone != NULL)
    {
        if (PolicymentForRoom::MonsterAbilityChangableRoomsBit & (1 << zone_type))
        {
            RoomInterface* room = static_cast<RoomInterface*>(zone);
            if (room == NULL)
            {
                return RC::RC_PARTY_NOT_FOUND_PARTYROOM;
            }
            BYTE number_of_players = room->GetNumberOfExpectedPlayers();
            if (number_of_players)
            {
                room->SetNumberOfExpectedPlayers(--number_of_players);
                g_ZoneManager.ChangeBattlePlayersCount(room, number_of_players);
            }
        }
    }

    return RC::RC_PARTY_SUCCESS;
}


bool
AgentPartyManager::CanJoinReturnParty(WORD party_key, User* user)
{
    if(user->GetPartyState().IsPartyMember() == true)
    {
        return false;
    }

    AgentParty* party = static_cast<AgentParty*>(FindEditableParty(party_key));
    if (party == NULL)
    {
        return false;
    }

    const CHARGUID char_guid = user->GetSelectedCharGuid();

    ClosingUserList& closing_user_list = party->GetClosingUserList();
    const ClosingUserInfo* closing_user = closing_user_list.FindClosingUser(char_guid);
    if (closing_user == NULL)
    {
        return false;
    }

    return true;
}

RC::ePARTY_RESULT
AgentPartyManager::AcceptReturnParty(WORD party_key, User* user)
{
    if (user->IsBeginTransaction())
    {
        return RC::RC_PARTY_ALREADY_DOING_TRANSACTION;
    }

    AgentPartyManager* const party_manager = AgentPartyManager::Instance();

    AgentParty* party = static_cast<AgentParty*>(FindEditableParty(party_key));
    if (party == NULL)
    {
        return RC::RC_PARTY_NOT_EXIST_PARTY;
    }

    ClosingUserList& closing_user_list = party->GetClosingUserList();
    const ClosingUserInfo* closing_user_info = 
        closing_user_list.FindClosingUser(user->GetSelectedCharGuid());
    if (closing_user_info == NULL)
    {
        SUNLOG(
            eCRITICAL_LOG, 
            __FUNCTION__" : Not found closing user : char name : %s, char guid : %d",
            user->GetSelectedCharName(), 
            user->GetSelectedCharGuid()
            );
        return RC::RC_PARTY_NOT_FOUND_USER;
    }

    if (closing_user_info->IsStateReturnTransaction() == true)
    {
        return RC::RC_PARTY_ALREADY_DOING_TRANSACTION;
    }
    const_cast<ClosingUserInfo*>(closing_user_info)->return_transaction_ =  true;

    user->SetBehaveState(PLAYER_BEHAVE_IDLE_STATE);

    //step 1 : 파티 가입
    if (party->GetType() == ePARTY_ROOM)
    {
        const PartyRoom* party_room = FindPartyRoom(party_key);
        if (party_room == NULL)
        {
            SUNLOG(
                eCRITICAL_LOG,
                __FUNCTION__" : Not found party room : %d", 
                party_key
                );
            return RC::RC_PARTY_FAIL_JOIN_PARTY_ROOM;
        }

        if (party_room->GetMapCode() != 0 && 
            party_room->CheckLevel(user->GetCharLV()) == false)
        { 
            MSG_CG_PARTY_JOIN_ROOM_ACK nak_msg;
            nak_msg.result_code_ = RC::RC_PARTY_NOT_MATCHING_USER_LEVEL;
            user->SendPacket(&nak_msg, nak_msg.GetSize());
            return RC::RC_PARTY_NOT_MATCHING_USER_LEVEL;
        }

        if (party_room->CanReturnJoinPartyRoomByMemberCount() == false)
        {
            MSG_CG_PARTY_JOIN_ROOM_ACK nak_msg;
            nak_msg.result_code_ = RC::RC_PARTY_FAIL_MAX_MEMBER_COUNT;
            user->SendPacket(&nak_msg, nak_msg.GetSize());
            return RC::RC_PARTY_PARTYMEMBER_NUM_FULL;
        }

        MSG_AG_PARTY_CAN_JOIN_ROOM_SYN syn_msg;
        syn_msg.map_code_ = party_room->GetMapCode();
        syn_msg.party_room_type_ = party_room->GetZoneType();
        syn_msg.join_party_key_ = party_key;
        user->SendToLinkedServer(&syn_msg, sizeof(syn_msg));
    }
    else
    {
        party_manager->JoinParty(party_key, user);
    }


    //step 2 : 이전 미션으로 복귀 요청
    if (closing_user_info->return_zone_type_ == eZONETYPE_MISSION ||
        closing_user_info->return_zone_type_ == eZONETYPE_INSTANCE||
        closing_user_info->return_zone_type_ == eZONETYPE_EVENT)
    {
        ZoneInterface* find_zone = g_ZoneManager.FindZone(closing_user_info->return_zone_key_);
        if (find_zone == NULL)
        {
            SUNLOG(
                eFULL_LOG,
                __FUNCTION__" : Not exist zone to find : find key : %d",
                closing_user_info->return_zone_key_
                );
            return RC::RC_PARTY_NOT_FOUND_ROOM;
        }

        const ZoneInterface::ZONE_USER_LIST& return_zone_user_list = find_zone->GetUserList();
        if (return_zone_user_list.size() == 0)
        {
            SUNLOG(
                eCRITICAL_LOG,
                __FUNCTION__" : Not exist party member to room : return member name : %s",
                user->GetSelectedCharName()
                );
            return RC::RC_PARTY_NOT_FOUND_USER;
        }
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
        User* const mission_user = find_zone->FindUser(party->GetMasterKey());
#else
        User* const mission_user = return_zone_user_list.begin()->second;
#endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
        
        if (mission_user == NULL)
        {
            SUNLOG(
                eCRITICAL_LOG,
                __FUNCTION__" : Invalid invite user data : target member name : %s",
                user->GetSelectedCharName()
                );
            return RC::RC_PARTY_NOT_FOUND_USER;
        }

        user->BeginTransaction(TR_LEAVE_FIELD_FOR_ROOM);

        MSG_AG_ZONE_RETURN_TO_ROOM_INFO_SYN syn_msg;
        syn_msg.m_dwRequestUserKey = user->GetUserGUID();
        syn_msg.m_wPartyKey = party_key;
        mission_user->SendToLinkedServer(&syn_msg, sizeof(syn_msg));
    }
    else
    {
        party_manager->RemoveClosingUser(party_key, user->GetSelectedCharGuid());
    }

    return RC::RC_PARTY_SUCCESS;
}

RC::ePARTY_RESULT
AgentPartyManager::RejectReturnParty(WORD party_key, User* user)
{
    AgentParty* party = static_cast<AgentParty*>(FindEditableParty(party_key));
    if (party == NULL)
    {
        return RC::RC_PARTY_NOT_EXIST_PARTY;
    }

    ClosingUserList& closing_members = party->GetClosingUserList();
    const CHARGUID char_guid = user->GetSelectedCharGuid();
    const ClosingUserInfo* closing_user = closing_members.FindClosingUser(char_guid);
    if (closing_user == NULL)
    {
        return RC::RC_PARTY_NOT_FOUND_USER;
    }

    MSG_CG_PARTY_RETURN_A2C_MEMBER_REJECT_CMD cmd_msg;
    cmd_msg.CopyFrom(closing_user->GetCharName());
    SendPacketAllMember(party_key, &cmd_msg, cmd_msg.GetSize(), user->GetPlayerKey());

    user->SetBehaveState(PLAYER_BEHAVE_IDLE_STATE);

    RemoveClosingUser(party_key, user->GetSelectedCharGuid());

    return RC::RC_PARTY_SUCCESS;
}

void
AgentPartyManager::UpdateReturnUser(WORD partyKey, ClosingUserInfo& rCUser)
{
    AgentParty* pAgentParty = (AgentParty*)FindEditableParty(partyKey);
    if(!pAgentParty)
    {
        SUNLOG(eCRITICAL_LOG, 
            __FUNCTION__" : Not found party info : party key : %d",
            partyKey);
        return;
    }

    pAgentParty->GetClosingUserList().UpdateChar(rCUser);
}

void
AgentPartyManager::RemoveClosingUser(WORD party_key, const DWORD char_guid)
{
    AgentParty* party = static_cast<AgentParty*>(FindEditableParty(party_key));
    if (party == NULL)
    {
        SUNLOG(eCRITICAL_LOG, 
            __FUNCTION__" : Not found party : party key : %d",
            party_key);
        return;
    }

    ClosingUserList& closing_user_list = party->GetClosingUserList();
    closing_user_list.RemoveClosingUser(char_guid);
}

void
AgentPartyManager::RemoveClosingUser(WORD party_key, const TCHAR* char_name)
{
    AgentParty* party = static_cast<AgentParty*>(FindEditableParty(party_key));
    if (party == NULL)
    {
        SUNLOG(eCRITICAL_LOG, 
            __FUNCTION__" : Not found party : party key : %d",
            party_key);
        return;
    }

    ClosingUserList& closing_user_list = party->GetClosingUserList();
    closing_user_list.RemoveClosingUser(char_name);
}


bool
AgentPartyManager::CheckReturnPartyInfo(User* pole_user, User* return_user)
{
    if (return_user->closing_user_trans_controller_.IsTransaction())
    {
        return false;
    }

    // 현재 유저가 있는 위치에서 마스터유저가 있는 위치로 이동 준비
    ZoneInterface * from_zone = g_ZoneManager.FindZone(return_user->GetZoneKey());
    ZoneInterface * to_zone = g_ZoneManager.FindZone(pole_user->GetZoneKey());
    if (from_zone == NULL || to_zone == NULL)
        return false;

    if (from_zone->FromZone_Check(CHECK_RETURN_PARTY) == false)
    {
        SUNLOG(eCRITICAL_LOG, __FUNCTION__" : From zone check fail");
        return_user->EndTransaction();
        return false;
    }

    if (to_zone->ToZone_Check(CHECK_RETURN_PARTY) == false)
    {
        SUNLOG(eCRITICAL_LOG, __FUNCTION__" : To zone check fail");
        return_user->EndTransaction();
        return false;
    }

    WORD party_key = pole_user->GetPartyState().GetPartyKey();
    AgentParty* party = static_cast<AgentParty*>(FindEditableParty(party_key));
    if (party == NULL)
    {
        return false;
    }

    const ClosingUserInfo* closing_user = 
        party->GetClosingUserList().FindClosingUser(return_user->GetSelectedCharGuid());
    if (closing_user == NULL)
    {
        return false;
    }

    if (pole_user->GetZoneKey() != closing_user->return_zone_key_)
    {
        return false;
    }

    ClosingUserTransController& trans_controller = return_user->closing_user_trans_controller_;
    bool result = trans_controller.SetClosingUserTransaction(to_zone->GetMapCode(), 
        closing_user->return_field_code_, 
        closing_user->return_pos_);
    if (result == false)
    {
        return false;
    }

    return true;
}