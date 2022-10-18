#include "stdafx.h"
//==================================================================================================
#include "MissionRoom.h"
//==================================================================================================
#include <PacketStruct_AG.h>
#include <PacketStruct_AW.h>
#include <MapInfoParser.h>

//==================================================================================================
#include <Zone/ZoneManager.h>
#include <ServerSessions/ServerSessionManager.h>
#include <ServerSessions/ServerSessionEx.h>
#include <UserSessions/User.h>

#include <AgentParty/AgentPartyManager.h>
//_NA_20110630_BATTLE_ZONE_RENEWAL
#include "MapPartialLoadingParser.h"
#include <AgentParty/AgentParty.h>
#include "Party/PartyRoom.h"

#ifdef _NA_007156_20140703_TOP_OF_TRIAL_LIGHT
#include "BattleOpenTimeManager.h"
#endif // _NA_007156_20140703_TOP_OF_TRIAL_LIGHT

_IMPL_POOL(MissionRoom)
MissionRoom::MissionRoom(void)
{
}

MissionRoom::~MissionRoom(void)
{
}

RC::eROOM_RESULT MissionRoom::Migrate(KEYTYPE key, RoomInterface* pPreviousRoom, ServerSession* pServerSession)
{
	if(pPreviousRoom->GetType() != eZONETYPE_MISSION && pPreviousRoom->GetType() != eZONETYPE_LOBBY)
		return RC::RC_ROOM_INVALIDROOMTYPE;

	RoomInterface::Migrate(key, pPreviousRoom, pServerSession);

	On_create();

	// 릴레이 유저 등록
	RelayUser(pPreviousRoom);
	//pPreviousRoom->SetReusedKey(TRUE);
    g_ZoneManager.ChangeBattlePlayersCount(this, GetNumberOfExpectedPlayers());

	return RC::RC_ROOM_SUCCESS;
}

//_NA_20110630_BATTLE_ZONE_RENEWAL
RC::eROOM_RESULT
MissionRoom::_SetBaseRoomInfo(User* user, BASE_ROOMINFO& base_room_info, MAPCODE map_code)
{
    const sMAPINFO* map_info = MapInfoParser::Instance()->FindMapInfo(map_code);
    if (map_info == NULL) 
    {
        return RC::RC_ROOM_INVALID_MAPCODE;
    }

    if(map_info->byMKind != eZONETYPE_MISSION)
    {
        return RC::RC_ROOM_INVALID_MAPCODE;
    }

    base_room_info.m_ClassLimit = map_info->byClass;
    base_room_info.m_MinLV = static_cast<BYTE>(map_info->minLV);
    base_room_info.m_MaxLV = static_cast<BYTE>(map_info->maxLV);    

    if (user->CanApplyPCBangBenefit(TRUE))
    {
        base_room_info.m_SpecialType = eROOM_SPECIAL_PCBANG_SUPER_ROOM;
    }
    else if (user->GetItemOwnFlag().FindItemInfo(eITEMTYPE_SUPERMASTER))
    {
        base_room_info.m_SpecialType = eROOM_SPECIAL_ITEM1_SUPER_ROOM;
    }

    return RC::RC_ROOM_SUCCESS;
}

RC::eROOM_RESULT
MissionRoom::Create(User* user, MAPCODE map_code, KEYTYPE mission_key, ServerSession* server_session)
{
    BASE_ROOMINFO base_room_info;
    RC::eROOM_RESULT result_code = _SetBaseRoomInfo(user, base_room_info, map_code);
    if (result_code != RC::RC_ROOM_SUCCESS)
    {
        return result_code;
    }

    SetMaster(user);
    LinkParty(user->GetPartyState().GetPartyKey());

    result_code = _CanCreate(user, map_code, base_room_info, eZONETYPE_MISSION);
    if (result_code != RC::RC_ROOM_SUCCESS)
    {
        return result_code;    
    }

#ifdef _NA_007156_20140703_TOP_OF_TRIAL_LIGHT
    AgentPartyManager* const party_manager = AgentPartyManager::Instance();
    const PartyRoom* party_room = party_manager->FindPartyRoom(GetLinkParty());
    if(party_room)
    {
        PartyZoneType party_zone_type = party_room->GetPartyRoomType();
        if (party_zone_type == PartyOfTopOfTrialLight_cash ||
            party_zone_type == PartyOfTopOfTrialLight_normal)
        {
            WORD partyKey = user->GetPartyState().GetPartyKey();
            result_code = AgentPartyManager::Instance()->TopOfTrialCheck(partyKey);
            if (result_code != RC::RC_ROOM_SUCCESS) {
                return result_code;
            }
        }

        if (party_zone_type == PartyOfTopOfTrialLight_normal) 
        {
            const sMAPINFO* map_info = MapInfoParser::Instance()->FindMapInfo(map_code);
            if (map_info == NULL)
            {
                SUNLOG(eCRITICAL_LOG, __FUNCTION__" : Not found map info parser : %d", map_code);
                return RC::RC_ROOM_FAILED;
            }

            if (!(g_BattleOpenTimeMng.IsOpenDungeon(map_info->wTimeLimit))) {
                return RC::RC_ROOM_FAILED;
            }
        }
    }
#endif //_NA_007156_20140703_TOP_OF_TRIAL_LIGHT

    RoomInterface::Create(user, mission_key, map_code, eZONEPUBLIC_PUBLIC, 
                          "", "", base_room_info, server_session);

    On_create();

    return result_code;
}

RC::eROOM_RESULT 
MissionRoom::_CanCreate(User* user, MAPCODE map_code, 
                        const BASE_ROOMINFO& base_room_info, eZONETYPE zone_type)
{
    if (GetRoomState() != eROOMSTATE_DESTROY)
    {
        return RC::RC_ROOM_INVALIDPREVLOBBY;
    }

    const sMAPINFO* map_info = MapInfoParser::Instance()->FindMapInfo(map_code);
    if (map_info == NULL)
    {
        return RC::RC_ROOM_INVALID_MAPCODE;
    }

    if (map_info->GetZoneType4SharedCode() != zone_type)
    {
        return RC::RC_ROOM_INVALID_MAPCODE;
    }

    MAP_PARTIAL_LOADING* loading_info = NULL;
    loading_info = MapPartialLoadingParser::Instance()->FindMapInfo(map_info->MapKind);
    if (loading_info && !(loading_info->m_bLoading))
    {
        return RC::RC_ROOM_INVALID_MAPCODE;
    }

    if (base_room_info.m_SpecialType == eROOM_SPECIAL_PCBANG_SUPER_ROOM)
    {
        if (user->CanApplyPCBangBenefit(TRUE) == FALSE)
        {
            return RC::RC_ROOM_ISNOT_PCBANG_USER;
        }
    }
    else if (base_room_info.m_SpecialType == eROOM_SPECIAL_ITEM1_SUPER_ROOM)
    {
        if (user->GetItemOwnFlag().FindItemInfo(eITEMTYPE_SUPERMASTER) == FALSE)
        {
            return RC::RC_ROOM_NOT_HAVE_SUPER_MASTER_ITEM;
        }
    }

    bool is_valid_limit_level = (base_room_info.m_MinLV <= base_room_info.m_MaxLV) &&
                                (map_info->minLV        <= base_room_info.m_MinLV) && 
                                (base_room_info.m_MaxLV <= map_info->maxLV);

    if (is_valid_limit_level == false)
    {
        return RC::RC_ROOM_INVALID_LIMITLEVEL;
    }

    LEVELTYPE user_level = user->GetSelectedCharLV();
    is_valid_limit_level = base_room_info.m_MinLV <= user_level &&
                           user_level <= base_room_info.m_MaxLV;
    if (is_valid_limit_level == false)
    {
        return RC::RC_ROOM_INVALID_LIMITLEVEL;
    }

    for(int i = 0; i < eCHAR_TYPE_MAX - 1; ++i)
    {
        if((map_info->byClass & (eCLASS_BERSERKER << i)) == 0)
        {
            if(base_room_info.m_ClassLimit & (eCLASS_BERSERKER << i))
            {
                return RC::RC_ROOM_INVALID_LIMITCLASS;
            }
        }
    }

    return RC::RC_ROOM_SUCCESS;
}

RC::eROOM_RESULT
MissionRoom::MigratePortalUserFirstEnter(KEYTYPE key,
                                         const RoomArgumentForRoomSetup& rRoomArgument,
                                         User* pUser,
                                         ServerSession* pServerSession)
{
    const sMAPINFO* pMapInfo = rRoomArgument.m_pMapInfo;
    const eZONETYPE enterZoneType = eZONETYPE(pMapInfo->byMKind);
    if(enterZoneType != eZONETYPE_MISSION)
        return RC::RC_ROOM_INVALIDROOMTYPE;

    //RoomInterface::Create(pUser, key, pMapInfo->MapCode,
    RoomInterface::MigratePortalUserFirstEnter(key, rRoomArgument, pUser, pServerSession);
    // Portal정보를 참조해서 방장정보를 확인해야 한다. 일단 최초 유저를 마스터로 설정한 뒤
    // 실제 마스터 입장시 ChangeMaster를 수행하자.
    // <- 기획 확인 결과 현 시점 계획 없음. (방장 바뀌어도 별 상관 없다는 의미인데...
    // 일단 수퍼 방장과 같은 문제는 충분히 거론될 수 있을 거라 생각됨. 방장 기준 미션 생성후
    // 방장이 탈퇴해도 수퍼방이 될 수 있기도 하고...
    // TODO(WAVERIX)
    //VestUserWithMaster(pUser);
    //VestUserWithMaster(pPreviousRoom->GetMaster());
    On_create();

    AgentPartyManager* pPartyManager = AgentPartyManager::Instance();
    {
        WORD partyKey = pUser->GetPartyState().GetPartyKey();
        BYTE number_of_expected_players = partyKey != 0
                                        ?   (BYTE)pPartyManager->GetMemberNum(partyKey) //pre-checked
                                        :   1;
        SetNumberOfExpectedPlayers(number_of_expected_players);
        g_ZoneManager.ChangeBattlePlayersCount(this, number_of_expected_players);
    }
    return RC::RC_ROOM_SUCCESS;
}

VOID MissionRoom::On_create()
{
	RoomInterface::On_create();

	MSG_AG_ZONE_MISSION_CREATE_CMD msg;
	msg.m_dwKey = GetMaster()->GetUserKey();
	msg.m_MapCode = GetMapCode();
	msg.m_RoomKey = GetKey();
    // (WAVERIX) (NOTE) 매번 맵코드에 기반한 sMAPINFO찾는 것 대단히 귀찮다. 객체 맴버로 등록하자.
    // 일단 시간상 찾는 걸로 한다.
    if(RoomPolicyment.FIELD & RoomPolicyment.QUEST_PORTAL_CONTROL)
    {
        const sMAPINFO* pMapInfo = MapInfoParser::Instance()->FindMapInfo(GetMapCode());
        const eZONE_RULESET& rRuleSet = pMapInfo->Ruleset;
        msg.m_Rules = eZONE_RULESET::eRules(rRuleSet.rules | rRuleSet.ZONE_RULE_OPTION_ROOM_BY_PORTAL);
    }
    else
    {
        msg.m_Rules = eZONE_RULESET::ZONE_RULE_EMPTY;
    }
	memcpy(msg.m_pszRoomTitle, GetRoomTitle(), _countof(msg.m_pszRoomTitle)*sizeof(msg.m_pszRoomTitle[0]));
	memcpy(msg.m_pszRoomPWD, GetRoomPWD(), _countof(msg.m_pszRoomPWD)*sizeof(msg.m_pszRoomPWD[0]));
	msg.m_RoomInfo = *(GetRoomInfo());//생성할 방 정보
#ifdef _NA_003649_20111115_BATTLE_ZONE_HARDMODE
    AgentPartyManager* const party_manager = AgentPartyManager::Instance();
    const PartyRoom* party_room = party_manager->FindPartyRoom(GetLinkParty());
    if(party_room) {
        msg.party_zone_type = party_room->GetPartyRoomType();
    }
#endif //_NA_003649_20111115_BATTLE_ZONE_HARDMODE

	SendToLinkedServer(&msg, sizeof(msg));

}

VOID MissionRoom::On_destroy()
{
	if(!IsReusedServerSession())
		ServerSessionManager::Instance()->Decr_LOF_BattleServer(GetLinkedServer()->GetSessionIndex());

	RoomInterface::On_destroy();
}

VOID MissionRoom::On_readyuser(User* pUser)
{
	RoomInterface::On_readyuser(pUser);
	pUser->SetZoneState(ePRS_BEFORE_ENTER_MISSION);
	pUser->SetZoneKey(GetKey());
	pUser->SetZoneType(eZONETYPE_MISSION);
}

VOID MissionRoom::On_enteruser(User* pUser)
{
	RoomInterface::On_enteruser(pUser);
	pUser->SetZoneState(ePRS_AT_MISSION);
}

VOID MissionRoom::On_leaveuser(User* user)
{
	BOOL is_master = GetMaster() == user;

	RoomInterface::On_leaveuser(user);

    //_NA_20110630_BATTLE_ZONE_RENEWAL
    if(is_master)
    {
        AgentPartyManager* const party_manager = AgentPartyManager::Instance();

        const AgentParty* party = 
            static_cast<const AgentParty*>(party_manager->FindParty(GetLinkParty()));

        if (party)
        {
            if (party->GetType() != ePARTY_ROOM)
            {
                User* next_master = RoomInterface::ChangeSpecialType(false, user);
                if (next_master)
                {
                    party_manager->ChangeMaster(next_master->GetPartyState().GetPartyKey(), 
                        next_master->GetPlayerKey());
                }
            }
        }
        else
        {
            SUNLOG(eCRITICAL_LOG, __FUNCTION__" : Not found party : %d", GetLinkParty());
        }
    }
}

RC::eROOM_RESULT MissionRoom::_CanJoinEach(User* pUser, eZONEPUBLIC RoomPublic, TCHAR* pszRoomPWD)
{
	// 카오상태 체크
	if(!GetServerOptChaoStateUseZone(GetType()) && pUser->IsChaoState())
	{
		return RC::RC_ROOM_UNABLE_FUNCTION_FOR_CHAOSTATE;
	}

	return RC::RC_ROOM_SUCCESS;
}