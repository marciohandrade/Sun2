#pragma once

#include "BasePartyManager.h"
#include <Singleton.h>
#include "GamePartyOperator.h"


class NewGameParty;
class Player;

//_NA_20110630_BATTLE_ZONE_RENEWAL
#include "QuestManager.h"
#include "MissionManager.h"
#include "TriggerManagerEx.h"
#include "QuestStruct.h"
#include "QuestManager_Concrete.h"

namespace nsPartyRoomMission{
;
typedef MSG_CG_ZONE_EX_QUEST_PORTAL_ENTER_SYN   ENTER_SYNC_TYPE;
struct EnterSynArg
{
    const sMAPINFO* map_info_;
    const sMAP_ENTERENCE_PORTAL* map_portal_;
};

struct QuestOrMissionResult
{
    bool quest_check_skip_;
    bool mission_check_skip_;
    bool is_quest_pass_;
    bool is_mission_pass_;

    QuestOrMissionResult()
    {
        quest_check_skip_ = false;
        mission_check_skip_ = false;
        is_quest_pass_ = false;
        is_mission_pass_ = false;
    }
};

static bool
IsMissionOrQuestPass(Player* plyaer, EnterSynArg* enter_syn_arg, 
                     QuestOrMissionResult* quest_mission_result)
{
    QuestManager_Concrete* quest_manager = plyaer->GetQuestManager();
    MissionManager* mission_manager = plyaer->GetMissionManager();

    quest_mission_result->quest_check_skip_ = (enter_syn_arg->map_info_->completeQCode == 0);
    if(enter_syn_arg->map_info_->completeQCode)
    {
        QUESTPART_COMPL* quest_complete = 
            quest_manager->FindComplQuest(enter_syn_arg->map_info_->completeQCode);
        quest_mission_result->is_quest_pass_ = quest_complete && quest_complete->m_Num;
    }

    quest_mission_result->mission_check_skip_ = enter_syn_arg->map_info_->completeMCode == 0;
    if(enter_syn_arg->map_info_->completeMCode)
    {
        quest_mission_result->is_mission_pass_ =
            !!mission_manager->GetMissionPoint((BYTE)enter_syn_arg->map_info_->completeMCode);
    }

    if(quest_mission_result->is_quest_pass_ || quest_mission_result->is_mission_pass_)
    {
        return TRUE;
    }

    if(quest_mission_result->quest_check_skip_ && quest_mission_result->mission_check_skip_)
    {
        return TRUE;
    }

    return FALSE;
}

static bool
IsFreePassLevel(Player* player, EnterSynArg* enter_syn_arg)
{
    const LEVELTYPE player_level = player->GetLevel();
    return (enter_syn_arg->map_info_->freePassLV <= player_level);
}

static bool
ProcessEnterRestrictCondition(Player* player, EnterSynArg* enter_arg)
{
    MissionManager* mission_manager = player->GetMissionManager();
    bool is_opened = !!mission_manager->EnableWindowOpen();
    if (is_opened)
    {
        TriggerManagerEx::Process_RewardWindow(player);
    }

    return is_opened == false;
}

static bool
IsLevelPass(Player* player, EnterSynArg* enter_arg)
{
    const LEVELTYPE player_level = player->GetLevel();
    LEVELTYPE min_level = enter_arg->map_info_->minLV;
    LEVELTYPE max_level = enter_arg->map_info_->maxLV;

    return min_level <= player_level && player_level <= max_level;;
}

static bool
IsEnterableMap(EnterSynArg* enter_arg)
{
    if (enter_arg->map_info_ == NULL || 
        !(enter_arg->map_info_->byMKind == eZONETYPE_MISSION || 
        enter_arg->map_info_->byMKind == eZONETYPE_INSTANCE || 
        enter_arg->map_info_->byMKind == eZONETYPE_PVP))
    {
        return false;
    }
    return true;
}

}; // end of namespace PartyRoomMission

class NewGamePartyManager : public util::Singleton<NewGamePartyManager>, public BasePartyManager
{
public:
    //_NA_003033_20110831_DICECAST_ITEM_ROUTING
    typedef _DICECAST_RESULT DicecastResultArray[MAX_PARTYMEMBER_NUM];
    //
	NewGamePartyManager();
	~NewGamePartyManager();

	virtual void Initialize(DWORD partyPoolSize);

	void    CreateGameParty(WORD partyKey, DWORD masterKey, eITEM_DISTRIBUTION_TYPE type, ePARTY_TYPE partyType);
	void    DestroyGameParty(WORD partyKey);
	void    DestroyAllParty();

	void    JoinGameParty(WORD party_key, const _PARTY_MEMBER_INFO& join_member_info, bool is_master);
	void    LeaveGameParty(WORD partyKey, DWORD memberKey);

	void    ChangeGameMaster(WORD partyKey, DWORD nextMasterKey);
	void    ChangeItemDistributionType(WORD partyKey, eITEM_DISTRIBUTION_TYPE type);
	void    MemberLevelUp(WORD partyKey, DWORD memberKey);

	void    GetAreaLivePartyInfo(WORD partyKey, ZONEKEY zoneKey, const WzVector* pNpcPos, 
                                 BYTE byRadius, OUT WORD& wTotalNum, OUT LEVELTYPE& TotalLevel);
	void    AddExpToAllMember(WORD partyKey, EXPTYPE Exp);
	BOOL    IsMaster(Player* pPlayer);
	Player* FindMasterPlayer(WORD partyKey);
	Player* FindMemberPlayer(WORD partyKey, DWORD memberKey);
	BYTE    GetDistributionType(WORD partyKey);
	BOOL    IsExistHostilityMember(WORD partyKey, DWORD hostilityGuildGuid);
	DWORD   GetDistributionMemberKey(WORD partyKey, DWORD defaultOwnerKey, eCHAR_TYPE classType);
    DWORD   GetValidReferencePlayerKeyForReturn(WORD partyKey);
    void    GetEnterRoomConditionInfoCollect(GameMemberEnterConditonOperator& oper, 
                                             Player* pRequester);
    void    Update();
	BOOL    SendPacketAllMember(WORD partyKey, MSG_BASE* pMsg, WORD wSize, DWORD exceptKey = 0);
	void    Display();

    RC::ePARTY_RESULT CanRequestParty(DWORD request_member_key, DWORD recv_member_key);

	template< class Operator >
    BOOL    For_eachPlayers(WORD partyKey, Operator& playerOpr, DWORD exceptKey = 0);

    //_NA_20110630_BATTLE_ZONE_RENEWAL
    bool RemoveMissionItem(Player* master_player, SLOTCODE item_code, BYTE item_num);

    RC::ePARTY_RESULT SetMemberReadyState(Player* player, WORD party_key, bool ready_state);

    BYTE    GetMemberConditionForEnterRoom(CheckMemberConditionToEnterRoomOperator& oper,
                                           WORD check_party_key);
    BYTE    GetMemberConditionForChangeOption(CheckMemberConditionToChangeOptionOperator& oper,
                                              WORD change_party_key);
    RC::ePARTY_RESULT CheckRoomTerms(Player* player, SLOTCODE item_code, BYTE item_num, 
                                     PartyZoneType party_room_type, MAPCODE map_code);
    RC::ePARTY_RESULT CheckJoinRoomTerms(Player* player, SLOTCODE item_code, BYTE item_num, 
                                         eZONETYPE party_room_type, MAPCODE map_code);
    void    ResetMemberReadyState(WORD party_key);
#ifdef _NA_007734_20141027_ADD_ROOMKIND_SPECIALMISSION
    RC::ePARTY_RESULT ExistPartyRoomItem(Player* player, SLOTCODE item_code, BYTE item_num, 
                                         PartyZoneType party_room_type = PartyOfMax)const;
#else
    RC::ePARTY_RESULT ExistPartyRoomItem(Player* player, SLOTCODE item_code, BYTE item_num)const;
#endif //_NA_007734_20141027_ADD_ROOMKIND_SPECIALMISSION

#ifdef _NA_003033_20110831_DICECAST_ITEM_ROUTING
    DWORD GetDiceCastDistributionMember(Player* player, WORD party_key, DicecastResultArray& result);
#endif //_NA_003033_20110831_DICECAST_ITEM_ROUTING

#ifdef _NA_0_20111021_EDIT_HEIM_DROP_RATIO_BY_PARTY_USER_COUNT
    int FindNearPartyUserCount(Player* player);
#endif

private:
    ITimerBase  m_UpdateTimer; //__NA001336_090514_PARTY_NAVI_SUPPORT__
};



template< class Operator >
BOOL NewGamePartyManager::For_eachPlayers(WORD partyKey, Operator& playerOpr, DWORD exceptKey)
{
	static GameMemberOperator<Operator> Opr;
	Opr.Init(eGAMEPARTY_OPERATOR_PLAYER, exceptKey);
	Opr.SetPlayerOpr(&playerOpr);

	return For_eachMembers(partyKey, Opr);
}

