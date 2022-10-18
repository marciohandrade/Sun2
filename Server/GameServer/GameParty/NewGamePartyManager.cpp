#include "StdAfx.h"

#include "NewGameParty.h"
#include "NewGamePartyManager.h"
#include "Player.h"
#include "PlayerManager.h"
#include "GamePartyOperator.h"
#include "GameRoom.h"
#include ".\FriendInvitation\GFriendInvitationController.h" //친구 초대 이벤트

//_NA_20110630_BATTLE_ZONE_RENEWAL
#include "SlotManager.h"
#include "ItemManager.h"
#include "BattlezoneMissionTreeParser.h"
#include "MapInfoParser.h"
#include "MissionManager.h"

NewGamePartyManager::NewGamePartyManager()
{
}


NewGamePartyManager::~NewGamePartyManager()
{
}

RC::ePARTY_RESULT
NewGamePartyManager::CanRequestParty(DWORD request_member_key, DWORD recv_member_key)
{
    PlayerManager* player_manager = PlayerManager::Instance();

    Player* request_player = player_manager->FindPlayerByObjKey(request_member_key);
    Player* recv_player = player_manager->FindPlayerByObjKey(recv_member_key);
    if (request_player == NULL || recv_player == NULL)
    {
        return RC::RC_PARTY_PLAYER_NOTEXIST_TO_FIELD;
    }

    CHARGUID request_guild_guid = request_player->GetGuildGuid();
    CHARGUID recv_guild_guid = recv_player->GetGuildGuid();

    if (GuildSmallContentEntry::RelationSystem::GetRelationType(
            request_guild_guid, recv_guild_guid) >= eGUILD_RELATION_BOTH_HOSTILITY)
    {
        return RC::RC_PARTY_CANNOT_INVITE_HOSTILITY;
    }

    if (IsExistHostilityMember(request_player->GetPartyState().GetPartyKey(), recv_guild_guid))
    {
        return RC::RC_PARTY_CANNOT_INVITE_HOSTILITY;
    }

    return RC::RC_PARTY_SUCCESS;
}


void
NewGamePartyManager::Initialize(DWORD partyPoolSize)
{
	party_pool_router_.Register<NewGameParty>(ePARTY_FIELD);
	party_pool_router_.Initialize(ePARTY_FIELD, partyPoolSize, partyPoolSize/4);
    //_NA_20110630_BATTLE_ZONE_RENEWAL
    party_pool_router_.Register<NewGameParty>(ePARTY_ROOM);
    party_pool_router_.Initialize(ePARTY_ROOM, partyPoolSize, partyPoolSize/4);

#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
    party_pool_router_.Register<NewGameParty>(ePARTY_INSTANCE);
    party_pool_router_.Initialize(ePARTY_INSTANCE, partyPoolSize, partyPoolSize/4);
#endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
    m_UpdateTimer.SetTimer(300); //__NA001336_090514_PARTY_NAVI_SUPPORT__
}

void
NewGamePartyManager::DestroyAllParty()
{
    GameMemberOperator<> Opr;
	PARTY_MAP::iterator it = party_map_.begin(), 
                        end = party_map_.end();
	while(it != end)
	{
        Opr.Init(eGAMEPARTY_OPERATOR_INIT, 0);
        NewGameParty* pNewGameParty = static_cast<NewGameParty*>(it->second);
        pNewGameParty->For_eachMembers(Opr);

        BaseParty* pBaseParty = it->second;
        const ePARTY_TYPE party_type = pBaseParty->GetType();
        pBaseParty->Release();

        party_pool_router_.Free(party_type, pBaseParty);
        
        party_map_.erase(it++);
	}
}

void
NewGamePartyManager::CreateGameParty(WORD partyKey, DWORD masterKey, 
                                     eITEM_DISTRIBUTION_TYPE type, ePARTY_TYPE partyType)
{
	BaseParty* pBaseParty = BasePartyManager::CreateParty(partyType, partyKey, masterKey);

	if(!pBaseParty)
	{
		SUNLOG(eCRITICAL_LOG, "[CreateGameParty] Can't Create GameParty[%u]", partyKey);
		return;
	}

	pBaseParty->SetDistributionType(type);
}

void
NewGamePartyManager::DestroyGameParty(WORD party_key)
{
	BaseParty* base_party = FindEditableParty(party_key);
	if (base_party == NULL)
	{
        SUNLOG(eCRITICAL_LOG, __FUNCTION__" : Not found party : %u", party_key);
		return;
	}

	static GameMemberOperator<> Opr;
	Opr.Init(eGAMEPARTY_OPERATOR_INIT, 0);
	base_party->For_eachMembers(Opr);

	BasePartyManager::DestroyParty(party_key);
}

void
NewGamePartyManager::JoinGameParty(WORD party_key, 
                                   const _PARTY_MEMBER_INFO& join_member_info, 
                                   bool is_master)
{
	RC::ePARTY_RESULT rcResult = BasePartyManager::JoinParty(party_key, join_member_info);
	if(rcResult != RC::RC_PARTY_SUCCESS)
	{
        SUNLOG(eCRITICAL_LOG, __FUNCTION__" : Fail join party : %u : %u", party_key, rcResult);
		return;
	}

    Player* player = PlayerManager::Instance()->FindPlayerByObjKey(join_member_info.m_dwMemberKey);
    if (player)
    {
        player->GetPartyState().JoinParty(0, party_key);
        GFriendInvitationController::Instance()->OnParty_Join(party_key, 
                                                              join_member_info.m_dwMemberKey);
    }
}

void
NewGamePartyManager::LeaveGameParty(WORD party_key, DWORD member_key)
{
	RC::ePARTY_RESULT result_code = BasePartyManager::LeaveParty(party_key, member_key);
	if (result_code != RC::RC_PARTY_SUCCESS)
	{
        SUNLOG(eCRITICAL_LOG, __FUNCTION__" : Fail leave party : %u : %u", party_key, result_code);
		return;
	}

	Player* player = PlayerManager::Instance()->FindPlayerByObjKey(member_key);
	if(player)
    {
        GFriendInvitationController::Instance()->OnParty_Leave(party_key, member_key);
		player->GetPartyState().Init();
    }
}

void
NewGamePartyManager::ChangeGameMaster(WORD partyKey, DWORD nextMasterKey)
{
	RC::ePARTY_RESULT rcResult = BasePartyManager::ChangeMaster(partyKey, nextMasterKey);
	if(rcResult != RC::RC_PARTY_SUCCESS)
	{
		SUNLOG(eCRITICAL_LOG, "[ChangeMaster] Fail ChangeMaster[%u]! ErrorCode[%u]",
               partyKey, rcResult);
		return;
	}

    //{__NA1335_090407_PORTAL_MISSION_ROOM_CONSTRAINTS__
    Player* pPlayer = PlayerManager::Instance()->FindPlayerByObjKey(nextMasterKey);
    GameZone* pGameZone = 0;
    if(pPlayer == 0 || !(pGameZone = pPlayer->GetGameZonePtr()))
        return;
    eZONETYPE zoneType = pGameZone->GetZoneType();
    if(zoneType == eZONETYPE_MISSION || zoneType == eZONETYPE_INSTANCE)
        static_cast<GameRoom*>(pGameZone)->ChangeMaster(pPlayer->GetUserKey());
    //}
}

void
NewGamePartyManager::ChangeItemDistributionType(WORD partyKey, eITEM_DISTRIBUTION_TYPE type)
{
	BaseParty* pBaseParty = FindEditableParty(partyKey);
	if(!pBaseParty)
	{
		SUNLOG(eCRITICAL_LOG, "[ChangeItemDistributionType] Can't Find GameParty[%u]", partyKey);
		return;
	}

	if(type >= eITEM_DISTRIBUTION_MAX)
	{
		SUNLOG(eCRITICAL_LOG, "[ChangeItemDistributionType] Invalid DistributionType[%u]", type);
		return;
	}

	pBaseParty->SetDistributionType(type);
}

void
NewGamePartyManager::MemberLevelUp(WORD partyKey, DWORD memberKey)
{
	NewGameParty* pNewGameParty = (NewGameParty*)FindEditableParty(partyKey);
	if(!pNewGameParty)
	{
		if(partyKey)
			SUNLOG(eCRITICAL_LOG, "[MemberLevelUp] Can't Find GameParty[%u]", partyKey);
		return;
	}

	pNewGameParty->MemberLevelUp(memberKey);   
}

void
NewGamePartyManager::GetAreaLivePartyInfo(WORD partyKey, ZONEKEY zoneKey,
                                          const WzVector* pNpcPos, BYTE byRadius,
                                          OUT WORD& totalNum, OUT LEVELTYPE& totalLevel)
{
	NewGameParty* pNewGameParty = (NewGameParty*)FindEditableParty(partyKey);
	if(pNewGameParty == 0)
        return;

	totalNum = 0;
    totalLevel = 0;

	if(pNpcPos == NULL || byRadius == 0)
		return;

	static GameMemberOperator<> Opr;
	Opr.Init(eGAMEPARTY_OPERATOR_AREA_LIVE_PARTYINFO, 0);
	Opr.SetAreaInfo(zoneKey, pNpcPos, byRadius);
	pNewGameParty->For_eachMembers(Opr);

	Opr.GetAreaInfo(totalNum, totalLevel);
}

void
NewGamePartyManager::AddExpToAllMember(WORD partyKey, EXPTYPE Exp)
{
	NewGameParty* pNewGameParty = (NewGameParty*)FindEditableParty(partyKey);
	if(!pNewGameParty)	return;

	static GameMemberOperator<> Opr;
	Opr.Init(eGAMEPARTY_OPERATOR_ADD_EXP, 0);
	Opr.SetExp(Exp);

	pNewGameParty->For_eachMembers(Opr);
}

BOOL
NewGamePartyManager::IsMaster(Player* pPlayer)
{
	NewGameParty* pNewGameParty = (NewGameParty*)FindEditableParty(pPlayer->GetPartyState().GetPartyKey());
	if(!pNewGameParty)	
		return FALSE;

	return pNewGameParty->GetMasterKey() == pPlayer->GetObjectKey() ? TRUE : FALSE;
}

Player*
NewGamePartyManager::FindMasterPlayer(WORD partyKey)
{
	NewGameParty* pNewGameParty = (NewGameParty*)FindEditableParty(partyKey);
	if(pNewGameParty == 0)
        return NULL;

	Player* pMaster = PlayerManager::Instance()->FindPlayerByObjKey(pNewGameParty->GetMasterKey());

	return pMaster;
}

Player*
NewGamePartyManager::FindMemberPlayer(WORD partyKey, DWORD memberKey)
{
	NewGameParty* pNewGameParty = (NewGameParty*)FindEditableParty(partyKey);
	if(pNewGameParty == 0)
        return NULL;

	Player* pMember = PlayerManager::Instance()->FindPlayerByObjKey(memberKey);

	return pMember;
}

BYTE
NewGamePartyManager::GetDistributionType(WORD partyKey)
{
	NewGameParty* pNewGameParty = (NewGameParty*)FindEditableParty(partyKey);
	if(pNewGameParty == 0)
        return NULL;

	return pNewGameParty->GetDistributionType();
}

BOOL
NewGamePartyManager::IsExistHostilityMember(WORD party_key, DWORD hostility_guild_guid)
{
	NewGameParty* new_game_party = (NewGameParty*)FindEditableParty(party_key);
	if (new_game_party == NULL)
    {
        return FALSE;
    }

	static GameMemberOperator<> Opr;

	Opr.Init(eGAMEPARTY_OPERATOR_HOSTILITY_CHECK, 0);
	Opr.SetHostilityGuildGuid(hostility_guild_guid);

	new_game_party->For_eachMembers(Opr);

	return Opr.IsExistHostilityMember();
}

DWORD
NewGamePartyManager::GetDistributionMemberKey(WORD party_key, DWORD defaultOwnerKey, eCHAR_TYPE classType)
{
	NewGameParty* pNewGameParty = (NewGameParty*)FindEditableParty(party_key);
	if(pNewGameParty == 0)
        return defaultOwnerKey;

	eITEM_DISTRIBUTION_TYPE type = (eITEM_DISTRIBUTION_TYPE)pNewGameParty->GetDistributionType();
	switch(type)
	{
	case eITEM_DISTRIBUTION_DEFAULT:			return defaultOwnerKey;
	case eITEM_DISTRIBUTION_FIFO_PRIORITY:		return pNewGameParty->GetFIFOMemberKey();
	case eITEM_DISTRIBUTION_MASTER_PRIORITY:	return pNewGameParty->GetMasterKey();
	case eITEM_DISTRIBUTION_RANDOM:				return pNewGameParty->GetRandomMemberKey();
	case eITEM_DISTRIBUTION_CLASS_PRIORITY:		return pNewGameParty->GetRandomMemberKey(classType);
#ifdef _NA_003033_20110831_DICECAST_ITEM_ROUTING
    case eITEM_DISTRIBUTION_DICE_AND_FIFO:      return pNewGameParty->GetFIFOMemberKey();
    case eITEM_DISTRIBUTION_DICE_AND_DEFAULT:   return defaultOwnerKey;
#endif //_NA_003033_20110831_DICECAST_ITEM_ROUTING
	default:
		SUNLOG(eCRITICAL_LOG, "[GetDistributionMemberKey] Invalid DistributionType[%u]", type);
	}

	return defaultOwnerKey;
}

DWORD
NewGamePartyManager::GetValidReferencePlayerKeyForReturn(WORD party_key)
{
    NewGameParty* pGameParty = (NewGameParty*)FindEditableParty(party_key);
    if(pGameParty == 0)
        return FALSE;

    static GameMemberOperator<> Opr;
    Opr.Init(eGAMEPARTY_OPERATOR_VALID_PLAYER_INFO_FOR_PARTY_RETURN, 0);
    pGameParty->For_eachMembers(Opr);
    DWORD playerKey = Opr.GetFirstValidPlayerForPartyReturn();
    return playerKey;
}

void
NewGamePartyManager::GetEnterRoomConditionInfoCollect(GameMemberEnterConditonOperator& oper,
                                                      Player* pRequester)
{
    const WORD partyKey = pRequester->GetPartyState().GetPartyKey();
    BaseParty* pParty = NULL;
    if(partyKey && (pParty = FindEditableParty(partyKey)))
        pParty->For_eachMembers(oper);
    else
        oper(pRequester, pRequester->GetObjectKey());
}

BOOL
NewGamePartyManager::SendPacketAllMember(WORD partyKey, MSG_BASE* pMsg, WORD wSize, DWORD exceptKey)
{
	NewGameParty* pNewGameParty = (NewGameParty*)FindEditableParty(partyKey);
	if(pNewGameParty == 0)
        return FALSE;

	static GameMemberOperator<> Opr;
	Opr.Init(eGAMEPARTY_OPERATOR_SEND, exceptKey);
	Opr.SetSendPacket(pMsg, wSize);

	pNewGameParty->For_eachMembers(Opr);

	return TRUE;
}

void
NewGamePartyManager::Display()
{
	DWORD dwMemberZeroCount = 0;

	PARTY_MAP::iterator end(party_map_.end());
	for(PARTY_MAP::iterator it = party_map_.begin(); it != end; ++it)
	{
		NewGameParty* pNewGameParty = (NewGameParty*)it->second;

		if(!pNewGameParty->GetMemberNum())
			++dwMemberZeroCount;
	}

	// 파티원이 0인 파티 수, 전체 파티 개수, 파티에 소속된 전체 멤버수
	SUNLOG(eCRITICAL_LOG, "TotPartyNum[%d] MemberZeroPartyNum[%d]",
           party_map_.size(), dwMemberZeroCount);
}

void
NewGamePartyManager::Update()
{
    BOOLEAN isExpired = !!m_UpdateTimer.IsExpired();
    if(isExpired == 0)
        return;

    FOREACH_CONTAINER(const PARTY_MAP::value_type& rParty, party_map_, PARTY_MAP)
    {
        NewGameParty* pGameParty = static_cast<NewGameParty*>(rParty.second);
        pGameParty->Update();
    }
}

//_NA_20110630_BATTLE_ZONE_RENEWAL
BYTE
NewGamePartyManager::GetMemberConditionForEnterRoom(CheckMemberConditionToEnterRoomOperator& oper,
                                                    WORD check_party_key)
{
    BaseParty* base_party = NULL;
    if (!check_party_key || !(base_party = FindEditableParty(check_party_key)))
    {
        SUNLOG(eCRITICAL_LOG, __FUNCTION__" : Not found party : %d", check_party_key);
        return 0;
    }

    base_party->For_eachMembers(oper);

    return static_cast<BYTE>(base_party->GetMemberNum());
}

BYTE
NewGamePartyManager::GetMemberConditionForChangeOption(CheckMemberConditionToChangeOptionOperator& oper,
                                                       WORD change_party_key)
{
    BaseParty* base_party = NULL;
    if (!change_party_key || !(base_party = FindEditableParty(change_party_key)))
    {
        SUNLOG(eCRITICAL_LOG, __FUNCTION__" : Not found party : %d", change_party_key);
        return 0;
    }

    base_party->For_eachMembers(oper);

    return static_cast<BYTE>(base_party->GetMemberNum());
}

bool
NewGamePartyManager::RemoveMissionItem(Player* master_player, 
                                       SLOTCODE item_code,
                                       BYTE item_num)
{
    if (item_code == 0)
    {
        return  false;
    }

    ItemManager* item_manager = master_player->GetItemManager();
    POSTYPE num = static_cast<POSTYPE>(item_num);
    SCItemSlot deleted_item_slot;
    MSG_CG_ITEM_LOSE_ACK ack_msg;
    {
        RC::eITEM_RESULT result_value = item_manager->DeleteAllKindOfItem( \
            1, &item_code, &num, &(ack_msg.m_ItemInfo), &deleted_item_slot);

        if (result_value != RC::RC_ITEM_SUCCESS)
        {
            SUNLOG(eCRITICAL_LOG,
                   __FUNCTION__" : Fail Delete item : %d", 
                   master_player->GetCharGuid());
            return false;
        }
        //배틀존 미션, 저탑, 이벤트 방 생성시 소모되는 아이템 로그
        GAMELOG->LogItem(ITEM_USE, master_player, &deleted_item_slot, 0, item_num);
    }
    master_player->SendPacket(&ack_msg, ack_msg.GetSize());
    return true;
}

RC::ePARTY_RESULT 
NewGamePartyManager::SetMemberReadyState(Player* player, WORD party_key, bool ready_state)
{
    MissionManager* mission_manager = player->GetMissionManager();
    if (mission_manager->EnableWindowOpen() == true)
    {
        return RC::RC_PARTY_NOT_MISSION_REWARD;
    }

    NewGameParty* party = static_cast<NewGameParty*>(FindEditableParty(party_key));
    if (party == NULL)
    {
        SUNLOG(eCRITICAL_LOG, __FUNCTION__" : Not found party : %d", party_key);
        return RC::RC_PARTY_NOT_EXIST_PARTY;
    }

    party->SetMemeberReadyState(player->GetObjectKey(), ready_state);

    return RC::RC_PARTY_SUCCESS;
}

void
NewGamePartyManager::ResetMemberReadyState(WORD party_key)
{
    NewGameParty* party = static_cast<NewGameParty*>(FindEditableParty(party_key));
    if (party == NULL)
    {
        SUNLOG(eCRITICAL_LOG, __FUNCTION__" : Not found party : %d", party_key);
        return;
    }

    party->ResetReadyStateAllMember();
    return;
}

//gamelulu's note : 현재 상황에서의 RoomType별 모든 건을 다루다 보니 인자수가 많아진 상태이며
//앞으로 추가 될 수도 있는 인자가 존재할 가능성도 많다.
//각 RoomType별 객체에 방을 생성 할 수 있는지 조건 채크에 관한 로직을 추가하여 아래와 같은 함수
//생성을 막는 편이 좋을 것 같다. (2011.09.16)
RC::ePARTY_RESULT
NewGamePartyManager::CheckRoomTerms(Player* player, SLOTCODE item_code, BYTE item_num, 
                                    PartyZoneType party_room_type, MAPCODE map_code)
{
    BattlezoneMissionTreeParser* const parser = BattlezoneMissionTreeParser::Instance();

    //step 1 : 아이템이 필요한 미션인지 체크
    if (parser->IsMissionOfRequireItem(map_code, party_room_type) == true)
    {
        //step 2 : 전달된 아이템이 해당 미션에 필요한 아이템인지 검사
        if (parser->IsMissionItem(map_code, party_room_type, item_code) == false)
        {
            return RC::RC_PARTY_INVALID_ITEM;
        }

        //stemp 3 : 전달된 아이템을 유저가 소유하고 있는지 검사
#ifdef _NA_007734_20141027_ADD_ROOMKIND_SPECIALMISSION
        RC::ePARTY_RESULT result_code = ExistPartyRoomItem(player, item_code, item_num, party_room_type);
#else
        RC::ePARTY_RESULT result_code = ExistPartyRoomItem(player, item_code, item_num);
#endif //_NA_007734_20141027_ADD_ROOMKIND_SPECIALMISSION
        if (result_code != RC::RC_PARTY_SUCCESS)
        {
            return result_code;
        }
    }

    switch (parser->GetZoneType(map_code))
    {
    case eZONETYPE_INSTANCE:
        {
            const sMAPINFO* const map_info = MapInfoParser::Instance()->FindMapInfo(map_code);
            if (map_info == NULL)
            {
                return RC::RC_PARTY_NOT_INCORRECT_MAPCODE;
            }

            const eZONE_RULESET& ruleset = map_info->Ruleset;
            if (ruleset.rules & ruleset.ZONE_RULE_INDUN_CANT_EQUIP_WEAPON_AND_BLOCK)
            {
                const BASE_ITEMINFO* const equip_info = 
                    player->GetEquipItemInfo(eEQUIPCONTAINER_WEAPON);
                if (equip_info != NULL)
                {
                    return RC::RC_PARTY_ROOM_NEED_UNEQUIP_WEAPON;
                }
            }
        }
        break;

    case eZONETYPE_MISSION:
        {
            MapInfoParser* const map_info_parser = MapInfoParser::Instance();
            nsPartyRoomMission::EnterSynArg arg = { 0, };
            arg.map_info_ = map_info_parser->FindMapInfo(map_code);
            if (arg.map_info_ == NULL) {
                return RC::RC_PARTY_NOT_INCORRECT_MAPCODE;
            }

            const bool is_free_pass = nsPartyRoomMission::IsFreePassLevel(player, &arg);
            if (is_free_pass == false)
            {
                nsPartyRoomMission::QuestOrMissionResult result;
                if (nsPartyRoomMission::IsMissionOrQuestPass(player, &arg, &result) == false)
                {
                    if (result.quest_check_skip_ == false && result.is_quest_pass_ == false)
                    {
                        return RC::RC_PARTY_PREVIOUS_QUEST_CLEAR;
                    }
                    else if (result.mission_check_skip_ == false && result.is_mission_pass_ == false)
                    {
                        return RC::RC_PARTY_NEED_CLEAR_PRE_MISSION;
                    }
                }
            }
        }
        break;

    case eZONETYPE_PVP:
        break;

    case eZONETYPE_FIELD:
        break;

    default:
        SUNLOG(eCRITICAL_LOG, __FUNCTION__" : Invalid party room type : %d", party_room_type);
        return RC::RC_PARTY_INVALID_ROOMTYPE;
    }

    return RC::RC_PARTY_SUCCESS;
}

RC::ePARTY_RESULT
NewGamePartyManager::CheckJoinRoomTerms(Player* player, SLOTCODE item_code, BYTE item_num, 
                                        eZONETYPE party_room_type, MAPCODE map_code)
{
    //타입별 추가 조건 검사
    switch (party_room_type)
    {
    case eZONETYPE_INSTANCE:
        break;

    case eZONETYPE_MISSION:
        {
            MapInfoParser* const map_info_parser = MapInfoParser::Instance();
            nsPartyRoomMission::EnterSynArg arg = { 0, };
            arg.map_info_ = map_info_parser->FindMapInfo(map_code);

            const bool is_free_pass = nsPartyRoomMission::IsFreePassLevel(player, &arg);
            if (is_free_pass == false)
            {
                nsPartyRoomMission::QuestOrMissionResult result;
                if (nsPartyRoomMission::IsMissionOrQuestPass(player, &arg, &result) == false)
                {
                    if (result.quest_check_skip_ == false && result.is_quest_pass_ == false)
                    {
                        return RC::RC_PARTY_PREVIOUS_QUEST_CLEAR;
                    }
                    else if (result.mission_check_skip_ == false && result.is_mission_pass_ == false)
                    {
                        return RC::RC_PARTY_NEED_CLEAR_PRE_MISSION;
                    }
                }
            }
        }
        break;

    case eZONETYPE_PVP:
        break;

    case eZONETYPE_FIELD:
        break;

    default:
        SUNLOG(eCRITICAL_LOG, __FUNCTION__" : Invalid party room type : %d", party_room_type);
        return RC::RC_PARTY_INVALID_ROOMTYPE;
    }

    return RC::RC_PARTY_SUCCESS;
}

#ifdef _NA_007734_20141027_ADD_ROOMKIND_SPECIALMISSION
RC::ePARTY_RESULT 
NewGamePartyManager::ExistPartyRoomItem(Player* player, SLOTCODE item_code, BYTE item_num, 
                                        PartyZoneType party_room_type /*= PartyOfMax*/)const
{
    //아이템 구한다
    BASE_ITEMINFO* item_info = ItemInfoParser::Instance()->GetItemInfo(item_code);
    if (item_info == NULL)
    {
        return RC::RC_PARTY_INVALID_ITEM;
    }

    PartyState& party_state = player->GetPartyState();
    const BaseParty* const party = FindParty(party_state.GetPartyKey());

    if (party != NULL && party_room_type == PartyOfSpecialMission) 
    {
        //모든 파티원들의 아이템을 검사
        PlayerManager* player_manager = PlayerManager::Instance();
        ItemManager* item_manager = NULL;
        
        const BaseParty::MEMBER_MAP& member_map = party->GetMemberList();
        BaseParty::MEMBER_MAP::const_iterator itr = member_map.begin();
        for( ; itr != member_map.end(); ++itr)
        {
            Player* party_member = player_manager->FindPlayerByObjKey(itr->first);
            if (party_member != NULL) {
                item_manager = party_member->GetItemManager();
            }

            if (item_manager->ExistItem(SI_INVENTORY, item_code, item_num) < item_num)
            {
                return RC::RC_PARTY_NOT_EXIST_ITEM;
            }
        }
    }
    else 
    {
        // 대상 플레이어의 아이템을 검사
        ItemManager* item_manager = player->GetItemManager();

        if (item_manager->ExistItem(SI_INVENTORY, item_code, item_num) < item_num)
        {
            return RC::RC_PARTY_NOT_EXIST_ITEM;
        }
    }

    return RC::RC_PARTY_SUCCESS;
}
#else
RC::ePARTY_RESULT 
NewGamePartyManager::ExistPartyRoomItem(Player* player, SLOTCODE item_code, BYTE item_num)const
{
    ItemManager* item_manager = player->GetItemManager();

    BASE_ITEMINFO* item_info = ItemInfoParser::Instance()->GetItemInfo(item_code);
    if (item_info == NULL)
    {
        return RC::RC_PARTY_INVALID_ITEM;
    }

    if (item_manager->ExistItem(SI_INVENTORY, item_code, item_num) < item_num)
    {
        return RC::RC_PARTY_NOT_EXIST_ITEM;
    }

    return RC::RC_PARTY_SUCCESS;
}
#endif //_NA_007734_20141027_ADD_ROOMKIND_SPECIALMISSION

#ifdef _NA_003033_20110831_DICECAST_ITEM_ROUTING
DWORD NewGamePartyManager::GetDiceCastDistributionMember(Player* player,
    WORD party_key, DicecastResultArray& result)
{
    NewGameParty* party = static_cast<NewGameParty*>(FindEditableParty(party_key));
    if (party == NULL)
        return 0;

    return party->GetDicecastResultKey(player, result);
}
#endif //_NA_003033_20110831_DICECAST_ITEM_ROUTING


#ifdef _NA_0_20111021_EDIT_HEIM_DROP_RATIO_BY_PARTY_USER_COUNT
int 
NewGamePartyManager::FindNearPartyUserCount(Player* player)
{
    int member_count = 1; // 자기 자신 포함.

    WORD party_key = player->GetPartyState().GetPartyKey();
    const BaseParty* pParty = FindParty(party_key);
    if(!pParty)
        return member_count;

    DWORD number_of_party_members = pParty->GetMemberNum();
    if(number_of_party_members < 1)
        return member_count;

    GameZone * game_zone = player->GetGameZonePtr();
    if(!game_zone)
        return member_count;

    float near_radius = 0.0f;
    if (game_zone->GetZoneType() == eZONETYPE_MISSION)
        near_radius = 0.0f;
    else
        near_radius = 60.0f;

    GameField* owner_field = player->GetField();
    if (!owner_field)
        return member_count;


    // 조건에 맞는 멤버를 찾는다
    BaseParty::MEMBER_MAP tempParty = pParty->GetMemberList();
    for(BaseParty::MEMBER_MAP::iterator iter = tempParty.begin(); iter != tempParty.end(); ++iter)
    {
        const _PARTY_MEMBER_INFO& memberInfo = iter->second;

        Player *pMember = PlayerManager::Instance()->FindPlayerByObjKey(memberInfo.m_dwMemberKey);
        if(!pMember) 
            continue;

        if(pMember->IsDead()) 
            continue;

        if (player == pMember)
            continue;

        GameZone *pZone = pMember->GetGameZonePtr();
        if(!pZone)
            continue;

        GameField* member_field	= pMember->GetField();
        if(member_field == NULL)
            continue;

        if(owner_field->GetFieldCode() != member_field->GetFieldCode())
            continue;

        if (near_radius != 0.0f)
        {
            if(Math_VectorLength(*player->GetPosPtr(), *pMember->GetPosPtr()) > near_radius)
                continue;
        }

        ++member_count;
    }

    return member_count;
}
#endif //_NA_0_20111021_EDIT_HEIM_DROP_RATIO_BY_PARTY_USER_COUNT