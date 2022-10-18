#include "StdAfx.h"
#include "GameDominationField.h"
#include <MapInfoParser.h>
#include "TriggerManagerEx.h"
#include "CrystalWarp.h"
#include "SkillInfoParser.h"
#include "StatusManager.h"
#include <SolarDateTime.h>
#include "GameDominationManager.h"
#include "GameGuild.h"
#include "GameGuildManager.h"

//_NA_0_20091118_GAMEROOM_BATTLE_RESULT
#include "DominationBattleResult.h"

#ifdef _NA_008512_20151001_MODIFY_DOMINATION_RENEWAL
#include "PlayerManager.h"
#endif //_NA_008512_20151001_MODIFY_DOMINATION_RENEWAL

__IMPL_CUSTOMPOOL_PTR(GameDominationField)

GameDominationField::GameDominationField(void) 
: fieldPvPRule_(PTA_DOMINATION_WAR_TILE)
, curEventState_(DOMINATION_EVENT_MAX)
, dwWarTime_(0)
, pCheckFilters_(NULL)
, event_stats_(DOMINATION_REWARD_EVENT_INFO::all_active) //_NA_0_20100705_DOMINATION_EVENT
, dwEventTriggerID_(0) //_NA_0_20100705_DOMINATION_EVENT
, dwStartTriggerID_(0)
, dwEndTriggerID_(0)
, bGateHPTunning_(FALSE)
, stamp_start_time_(0)
{
    _Init(offense_guild);
    _Init(defense_guild);

    //_NA_0_20100705_DOMINATION_EVENT
    ZeroMemory(&reward_event_, sizeof(reward_event_));

    //_NA_0_20091118_GAMEROOM_BATTLE_RESULT
    m_pBattleResult = new DominationBattleResult;
}

GameDominationField::~GameDominationField(void)
{
    //_NA_0_20091118_GAMEROOM_BATTLE_RESULT
    SAFE_DELETE(m_pBattleResult);

    if(pCheckFilters_)
    {
        TAllocDelete(MatchCheckCodeFilters, pCheckFilters_);
        pCheckFilters_ = NULL;
    }
}


//_NA_20100604_BETTER_DOMINATION_MANAGER
void 
GameDominationField::UpdateGuildInfoByDomination(MAPCODE map_code)
{

    GameDominationManager* domination_manager = GameDominationManager::Instance();

    const DOMINATION_INFO* domination_info = domination_manager->GetDominationInfo(map_code);
    if (domination_info == NULL)
    {
        return;
    }

    domination_manager->UpdateGuildInfoByDomination(domination_info->m_inGuildInfo.m_GuildGuid,
        DOMINATION_HUNTING_COUNT);
}

void 
GameDominationField::ResetTrigger()
{
    if(Policyment.FIELD & Policyment.ENABLE_TRIGGER)
    {
        GameZone::ResetTrigger();
    }
}

void 
GameDominationField::OnCreate()
{
    ChangeHandler( eZONETYPE_DOMINATION_FIELD );
    GameZone::OnCreate();

    const sDOMINATION_INFO* pInfoScript = 
        DominationInfoParser::Instance()->GetDominationInfo(GetMapCode());

    if (NULL == pInfoScript)
        return;

    if(Policyment.FIELD & Policyment.ENABLE_TRIGGER)
    {
        CreateTrigger();

        dwEventTriggerID_ = pInfoScript->m_dwEventTriggerID;
        dwStartTriggerID_ = pInfoScript->m_StartTriggerID;
        dwEndTriggerID_   = pInfoScript->m_EndTriggerID;
    }

    _SetNotifyTime(DOMINATION_WAR_TIME, 4, 10);
    //_NA_20110406_CHANGE_RULE_FOR_DOMINATION_JOIN
    _SetNotifyTime(DOMINATION_JOIN_TIME, 7, 10);

    pCheckFilters_ = TAllocNew(MatchCheckCodeFilters);
    new (&pCheckFilters_->SkillGroupFilter) 
        MatchCheckCodeFilterArray(sDOMINATION_INFO::LIMIT_MAX, pInfoScript->m_LimitSkillGroup);

    new (&pCheckFilters_->ItemTypeFilter)
        MatchCheckCodeFilterArray(sDOMINATION_INFO::LIMIT_MAX, pInfoScript->m_LimitItemGroup);

    const DOMINATION_INFO* domination_info = 
        GameDominationManager::Instance()->GetDominationInfo(GetMapCode());
    if (domination_info != NULL)
    {
        InsertDominationInfo(domination_info);
    }

    //_NA_0_20091118_GAMEROOM_BATTLE_RESULT
    m_pBattleResult->Init(true);
#ifdef _NA_007128_20140326_MONDSCHIEN_DOMINATION
    _SetEventInfos(GetMapCode());
#endif  //_NA_007128_20140326_MONDSCHIEN_DOMINATION
}

#ifdef _NA_007128_20140326_MONDSCHIEN_DOMINATION
VOID GameDominationField::_SetEventInfos(MAPCODE map_code)
{
    DominationInfoParser* pParser = DominationInfoParser::Instance();
    if(pParser == NULL)
    {
        return;
    }

    event_infos_ = pParser->GetEventInfosFromMapCode(map_code);
}
#endif  //_NA_007128_20140326_MONDSCHIEN_DOMINATION

void 
GameDominationField::_SetNotifyTime(const eDOMINATION_NOTIFY_TIME notify_type, 
                                         const uint8 count_notify, const uint8 interval)
{
    NotifyTimeInfo notify_time_info;
    for (int i = 0; i < count_notify; ++i)
    {
        notify_time_info.type_of_time_ = notify_type;
        notify_time_info.notify_time_ = interval * i;//set : 0, 10, 20, 30 ��

        startNotifyTime_.insert(std::make_pair(notify_type, notify_time_info));
    }
}

void 
GameDominationField::InsertDominationInfo(const DOMINATION_INFO* domination_info)
{
    if (domination_info == NULL)
    {
        return;
    }

    const DOMINATION_BASIC_GUILD_INFO& inGuildInfo = domination_info->m_inGuildInfo;
    const DOMINATION_BASIC_GUILD_INFO& disGuildInfo = domination_info->m_disGuildInfo;

    SetGuildGuid(inGuildInfo.m_GuildGuid, disGuildInfo.m_GuildGuid, DOMINATION_INIT);

    reward_event_ = domination_info->m_rewardEvent;
}

void 
GameDominationField::OnDestroy()
{
    //_NA_0_20091118_GAMEROOM_BATTLE_RESULT
    m_pBattleResult->Release();

    _DestroyCrystalWarp(std::make_pair(offense_guild, crystalwarp_1st));
    _DestroyCrystalWarp(std::make_pair(offense_guild, crystalwarp_2nd));
    _DestroyCrystalWarp(std::make_pair(defense_guild, crystalwarp_1st));
    _DestroyCrystalWarp(std::make_pair(defense_guild, crystalwarp_2nd));

    tupleWarp[offense_guild].get<crystalwarp_cooltime>().DisableCheckTime();
    tupleWarp[defense_guild].get<crystalwarp_cooltime>().DisableCheckTime();

    tupleWarp[offense_guild].get<idx_guildguid>() = 0;
    tupleWarp[defense_guild].get<idx_guildguid>() = 0;

    bGateHPTunning_ = FALSE;

    if(Policyment.FIELD & Policyment.ENABLE_TRIGGER)
    {
        // _NA_0_20100705_DOMINATION_EVENT
        event_stats_ = DOMINATION_REWARD_EVENT_INFO::all_active;
        dwEventTriggerID_ = 0;
        dwStartTriggerID_ = 0;
        dwEndTriggerID_   = 0;

        DestroyTrigger();
    }

    //_NA_0_20100705_DOMINATION_EVENT
    ZeroMemory(&reward_event_, sizeof(reward_event_));

    if(pCheckFilters_)
    {
        TAllocDelete(MatchCheckCodeFilters, pCheckFilters_);
        pCheckFilters_ = NULL;
    }

    startNotifyTime_.clear();
    dwWarTime_ = 0;
    warTimer_.DisableCheckTime();
    curEventState_ = DOMINATION_EVENT_MAX;
   
    GameZone::OnDestroy();
}

void 
GameDominationField::Ready(Player * pPlayer, MAPCODE FieldCode,
                                WzID AreaID, WzVector * pwvStartPos)
{
    DEBUG_CODE(
        GameZone * pEnterZone	= NULL;
    MAPCODE EnterFieldCode	= 0;
    eZONE_STATE eRoomState	= pPlayer->GetGameZone( pEnterZone, EnterFieldCode );
    ASSERT( !(eRoomState & ePRS_AT_ZONE) );
    );

    pPlayer->SetGameZone( ePRS_BEFORE_ENTER_DOMINATION_FIELD, this, FieldCode, AreaID, pwvStartPos );
}

void 
GameDominationField::SetZone(Object *pObject, MAPCODE FieldCode,
                                  WzID AreaID, WzVector * pwvStartPos)
{
    pObject->SetGameZone( ePRS_AT_DOMINATION_FIELD, this, FieldCode, AreaID, pwvStartPos );
}

BOOL 
GameDominationField::Join( Player * pPlayer, MAPCODE FieldCode,
                               WzID AreaID, WzVector * pwvStartPos )
{
    if (FALSE == (ePRS_BEFORE_ENTER_DOMINATION_FIELD & pPlayer->GetGameZoneState()))
    {
        SUNLOG( eCRITICAL_LOG, 
            "GameDominationField::Join eRoomState[%d] ZoneType[%d]",
            pPlayer->GetGameZoneState(), GetZoneType() );

        return FALSE;
    }

    SetZone( pPlayer, FieldCode, AreaID, pwvStartPos );

    if (!GameZone::Join(pPlayer, FieldCode, AreaID, pwvStartPos))
    {
        pPlayer->SetGameZone( ePRS_NOT_AT_ZONE, NULL, 0 );
        return FALSE;
    }

    if (Policyment.FIELD & Policyment.ENABLE_SAVE_LOCATION)
        pPlayer->SaveStartLocation();
    if (Policyment.FIELD & Policyment.ENABLE_TRIGGER)
    {
        GameField* pField = FindGameField(FieldCode);
        pField->GetTriggerManager()->MAP_STATE.SendInfo(pPlayer);
    }

    //_NA001385_20090924_DOMINATION_EVENT_CONTROLL
    StatusManager* const pStatusManager = pPlayer->GetStatusManager();   
    pStatusManager->Status.On_FilterControl(FALSE, TRUE, pCheckFilters_);

    // (CHANGES) (WOPS:5607) ���������� ����ÿ��� 'Ż ��' ��ȯ ������ �ʿ��ϴ�.
    pPlayer->Rider.Command_SummonRelease(IVehicleEvent::EnterField());
    //.//

    //_NA_003457_20111013_EXCEPT_DOMINATION_REWARD
    if (DOMINATION_EVENT_START == curEventState_ ||
        DOMINATION_EVENT_WAIT == curEventState_)
    {

#ifdef _NA_008334_20150608_SONNENSCHEIN
        GameGuild* player_guild = pPlayer->GetGuild();
        if (player_guild != NULL) {
            player_guild->ApplyGuildRewardOption(pPlayer, FALSE);
        }
#else
        GameDominationManager::Instance()->ApplyAllRewardOption(pPlayer, FALSE);
#endif //_NA_008334_20150608_SONNENSCHEIN
        GameDominationManager::Instance()->SendAreaInfoToPlayer(pPlayer, FALSE);
    }

    if (DOMINATION_EVENT_WAIT  == curEventState_ ||
        DOMINATION_EVENT_START == curEventState_)
    {
        _SetAttrGate();

        MSG_CG_DOMINATION_TIME_SYNC_BRD brdTimeSync;
        brdTimeSync.m_byEventState = curEventState_;

        if (DOMINATION_EVENT_WAIT == curEventState_)
            brdTimeSync.m_dwRemainedTime = dwWarTime_;
        else
            brdTimeSync.m_dwRemainedTime = warTimer_.GetRemainedTime();

        pPlayer->SendPacket(&brdTimeSync, sizeof(brdTimeSync));
    }

    if (DOMINATION_EVENT_WAIT != curEventState_ )
    {
        if (DOMINATION_EVENT_START != curEventState_)
            _EndTriggerActive();

        _StartTriggerActive();
    }

#ifdef _NA_006826_20130722_DOMINATION_RENEWAL
    if (DOMINATION_EVENT_WAIT  == curEventState_ ||
        DOMINATION_EVENT_START == curEventState_)
    {
        MSG_CG_DOMINATION_DEFENSE_GUILD_ALLIENCE_LIST_BRD brd_msg;
        ZeroMemory(&brd_msg.allience_list, sizeof(brd_msg.allience_list));

        const GUILDGUID defense_guild_guid = GetDefenseGuild();
        GameGuild* const defense_guild = g_GameGuildManager.FindGuild(defense_guild_guid);
        if (defense_guild)
        {
            GuildRelationSystem::GuildRelationEntry* relation_entry = \
                defense_guild->GetRelationEntry();
            STLX_VECTOR<GUILDGUID> relation_guild_guids;
            int allience_guild_count = \
                relation_entry->GetRelationGuid(relation_guild_guids, eGUILD_RELATION_ALLIENCE);
            if (allience_guild_count != 0)
            {
                int index = 0;
                STLX_VECTOR<GUILDGUID>::const_iterator const_itr = relation_guild_guids.begin();
                for (;const_itr != relation_guild_guids.end(); ++const_itr )
                {
                    GUILDGUID guild_guid = *const_itr;
                    if (guild_guid)
                    {
                        brd_msg.allience_list[index++] = guild_guid;

                        if (index >= MAX_ALLIENCE_GUILD_NUM) {
                            SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"] Guild allience MaxCount MisMatched!!");
                            break;
                        }
                    }
                }
            }
        }

        pPlayer->SendPacket(&brd_msg, sizeof(brd_msg));

    }
#endif // _NA_006826_20130722_DOMINATION_RENEWAL

    SUNLOG(eDEV_LOG, "GameDominationField::Join %s", pPlayer->GetCharName());
    GAMELOG->LogDomination(ACT_DOMINATION_FIELD_JOIN, pPlayer, NULL);

    return TRUE;
}

void 
GameDominationField::Leave(Player * pPlayer)
{
    StatusManager* const pStatusManager = pPlayer->GetStatusManager();
    pStatusManager->Status.On_FilterControl( FALSE, FALSE, NULL );

    if(Policyment.FIELD & Policyment.ENABLE_SAVE_LOCATION)
        pPlayer->SaveStartLocation();

    GameZone::Leave(pPlayer);

    //_NA_003457_20111013_EXCEPT_DOMINATION_REWARD
    if (DOMINATION_EVENT_START == curEventState_ ||
        DOMINATION_EVENT_WAIT == curEventState_)
    {
#ifdef _NA_008334_20150608_SONNENSCHEIN
        GameGuild* player_guild = pPlayer->GetGuild();
        if (player_guild != NULL) {
            player_guild->ApplyGuildRewardOption(pPlayer);
        }
#else
        GameDominationManager::Instance()->ApplyAllRewardOption(pPlayer, TRUE);
#endif //_NA_008334_20150608_SONNENSCHEIN

        GameDominationManager::Instance()->SendAreaInfoToPlayer(pPlayer, TRUE);
    }

    SUNLOG(eDEV_LOG, "GameDominationField::Leave %s", pPlayer->GetCharName());
    GAMELOG->LogDomination(ACT_DOMINATION_FIELD_LEAVE, pPlayer, NULL);
}

void 
GameDominationField::ReqKickPlayer(Player* pPlayer)
{
    MSG_AG_ZONE_DOMINATION_REQ_KICK_CMD cmdMsg;
    cmdMsg.m_mapCode  = GetMapCode();
    cmdMsg.m_userGuid = pPlayer->GetUserGuid();
    g_pGameServer->SendToServer(AGENT_SERVER, &cmdMsg, sizeof(cmdMsg));
}

//////////////////////////////////////////////////////////////////////////


BOOL 
GameDominationField::MoveField(Player * pPlayer, MAPCODE FieldCode,
                                    WzID AreaID, WzVector * pwvStartPos)
{
    DEBUG_CODE(
        eZONE_STATE eRoomState	= pPlayer->GetGameZoneState();
    ASSERT( (eRoomState & ePRS_BEFORE_ENTER_DOMINATION_FIELD) );
    );

    SetZone( pPlayer, FieldCode, AreaID, pwvStartPos );

    if( !GameZone::Join( pPlayer, FieldCode, AreaID, pwvStartPos ) )
    {
        pPlayer->SetGameZone( ePRS_NOT_AT_ZONE, NULL, 0, NULL );
        return FALSE;
    }

    if(Policyment.FIELD & Policyment.ENABLE_SAVE_LOCATION)
        pPlayer->SaveStartLocation();
    if(Policyment.FIELD & Policyment.ENABLE_TRIGGER)
    {
        GameField* pField = FindGameField(FieldCode);
        pField->GetTriggerManager()->MAP_STATE.SendInfo(pPlayer);
    }

    return TRUE;
}

void 
GameDominationField::LeaveForWarp(Player* pPlayer)
{
    if(Policyment.FIELD & Policyment.ENABLE_SAVE_LOCATION)
        pPlayer->SaveStartLocation();

    GameZone::Leave(pPlayer);
}

void 
GameDominationField::GetArea(GameField* pField, Player* pPlayer, 
                                  MAPCODE FieldCode, WzID& AreaID, WzVector& rwvStartPos)
{
    AreaID = _GetStartingArea(pPlayer->GetGuildGuid());
    pField->GetAreaRandomStartingVector(AreaID, rwvStartPos);
}

WzID 
GameDominationField::_GetStartingArea(const GUILDGUID guildGuid)
{
    const sMAPINFO* psMAPINFO = MapInfoParser::Instance()->FindMapInfo(GetMapCode());
    const int guildIdx = _FindGuildIdx(guildGuid);

    if (defense_guild == guildIdx)
    {
        return psMAPINFO->wStartAreaID;
    }
#ifdef _NA_006826_20130722_DOMINATION_RENEWAL
    else if(defense_guild != guildIdx)
    {
        GameGuild* const defense_game_guild = g_GameGuildManager.FindGuild(GetDefenseGuild());
        // ���� ��尡 ���� ���..
        if (defense_game_guild)
        {
            GuildRelationSystem::GuildRelationEntry* defense_guild_entry = \
                defense_game_guild->GetRelationEntry();
            if (defense_guild_entry == NULL)
            {
                return INVALID_WZID;
            }

            eGUILD_RELATION relation = defense_guild_entry->GetRelationType(guildGuid);
            if (relation == eGUILD_RELATION_ALLIENCE) {
                return psMAPINFO->wStartAreaID;
            }
            else {
                return psMAPINFO->wStartAreaID2;
            }
        }

        // ���� ��尡 ���� ���..(�� ������ ����..)
        else {
            return psMAPINFO->wStartAreaID2;
        }
    }
#else
    else if(offense_guild == guildIdx)
    {
        return psMAPINFO->wStartAreaID2;
    }
#endif // _NA_006826_20130722_DOMINATION_RENEWAL
    else
    {
        SUNLOG( eCRITICAL_LOG, "[GameDominationField::GetArea] MapCode[%d] guildIdx[%d]", 
            GetMapCode(), guildIdx);
    }

    return INVALID_WZID;
}

RC::eSTATUS_RESULT 
GameDominationField::ResurrectionAtSafezone(Player *pPlayer)
{
    if (pPlayer->IsAlive())
        return RC::RC_STATUS_CHAR_IS_ALIVE;

    GameField* pField = pPlayer->GetField();
    if (!pField)
        return RC::RC_STATUS_FIELD_IS_NULL;

    DWORD dwAreaID = 0;	
    WzVector vStartPos; 
    WzID AreaID = _GetStartingArea(pPlayer->GetGuildGuid());
    pField->GetAreaRandomStartingVector(AreaID, vStartPos);

    if (!pField->TeleportObject(pPlayer, &vStartPos))
        return RC::RC_STATUS_TELEPORT_FAIL;

    // ���������� ��Ȱ�� ȸ�� [12/14/2009 lst1024]
    if (!pPlayer->OnResurrection(0, 0.5f, 0.5f))
    {
        return RC::RC_STATUS_RESURRECTION_FAIL;
    }

#ifdef _NA_006826_20130722_DOMINATION_RENEWAL
    // ��Ȱ�� �����ϸ� ��õ��� �������¸� �ɾ��ش�
    // ������ ��Ȱ �� ���̾�ٵ�(����) 3.5��
    StatusManager* const status_manager = pPlayer->GetStatusManager();
    ALLOC_STATUS_PAIR steel = status_manager->AllocStatus(eCHAR_STATE_IMMUNITY_DAMAGE, 3500);
    if (steel.second) {
        steel.second->SendStatusAddBRD();
    }
#endif //_NA_006826_20130722_DOMINATION_RENEWAL

    return RC::RC_STATUS_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////
// 
  //���׸� HP Ʃ��
void 
GameDominationField::_SetAttrGate()
{
    if (FALSE != bGateHPTunning_)
        return;

    const MAPCODE mapCode = GetMapCode();

    const sDOMINATION_INFO* pInfoScript = 
        DominationInfoParser::Instance()->GetDominationInfo(mapCode);

    const DOMINATION_INFO* pInfo = 
        GameDominationManager::Instance()->GetDominationInfo(mapCode);

    if (NULL == pInfo ||
        NULL == pInfoScript)
        return;

    GameField* pField = FindGameField(GetMapCode());
    if (NULL == pField)
        return;

    DWORD dwTunningHP[sDOMINATION_INFO::GATEID_MAX] = {
        pInfoScript->m_GateHPTunning * pInfo->m_DeffenseWallA,
            pInfoScript->m_GateHPTunning * pInfo->m_DeffenseWallB,
            pInfoScript->m_GateHPTunning * pInfo->m_DeffenseWallC };

    MapNpc* pMapNpc[sDOMINATION_INFO::GATEID_MAX];

    for(int i = 0; i < sDOMINATION_INFO::GATEID_MAX; ++i)
    {
        const DWORD objKey = Conv_MapObjectKey(pInfoScript->m_GateID[i]);
        pMapNpc[i] = pField->FindMapNpc(objKey);
        if (NULL == pMapNpc[i])
            return;
    }

    for(int i = 0; i < sDOMINATION_INFO::GATEID_MAX; ++i)
    {
        pMapNpc[i]->SetAttr(eATTR_MAX_HP, eATTR_KIND_SKILL, dwTunningHP[i]);

        const DWORD dwMaxHP = pMapNpc[i]->GetMaxHP();
        pMapNpc[i]->SetHP(dwMaxHP);
        pMapNpc[i]->SendHPChange();
    }

    bGateHPTunning_ = TRUE;
}

// ������ ��� ������ ���� ��� �������� ����ȭ ���� ������ �ִ�.   
// ��� �������� ����ȭ �Ǵ� ������ ���� ������ ��� ������ ���� �ϰ�,
// �� ������ ���� Agent �����͵� ����ȭ�� �Ѵ�. 

void 
GameDominationField::SetGuildGuid(GUILDGUID defense, GUILDGUID offense,
                                       eDOMINATION_EVENT_TYPE eSetType)
{
    _Init(defense_guild);
    _Init(offense_guild);

    switch(eSetType)
    {
    case DOMINATION_DEFFENSEWALL_SET:
    case DOMINATION_INIT:
    case DOMINATION_INFO_SET:
    case DOMINATION_DISGUILD_SET:
        tupleWarp[defense_guild].get<idx_guildguid>() = defense;
        tupleWarp[offense_guild].get<idx_guildguid>() = offense;
        break;
    case DOMINATION_SUCCESS_DEFENCE:
        tupleWarp[defense_guild].get<idx_guildguid>() = defense;
        break;
#ifdef _NA_007128_20140326_MONDSCHIEN_DOMINATION 
    case DOMINATION_TURN_OVER:  //�Ͽ����� �߻��Ѱ��� ������ ������ ���̴�.
#endif  //_NA_007128_20140326_MONDSCHIEN_DOMINATION
    case DOMINATION_SUCCESS_OFFENCE:
        tupleWarp[defense_guild].get<idx_guildguid>() = offense;
        break;
    }

    //.//

    MSG_AG_ZONE_DOMINATION_FIELD_INFO_ACK ackMsg;
    ackMsg.m_RoomKey = GetKey();
    ackMsg.m_offenceGuild = offense;
    ackMsg.m_defenceGuild = defense;
    ackMsg.m_eSetType = eSetType;

    g_pGameServer->SendToServer(AGENT_SERVER, &ackMsg, sizeof(ackMsg));
}

//////////////////////////////////////////////////////////////////////////
// ũ����Ż ����
//
// . �ʴ� 2���� ��ȯ �� �� �ִ�.
// . ���� ������ ���� �����ϴ�.
// . �� �Ѿ�� ������ �Ұ����ϴ�.
// . Ÿ ������ �̵� �Ұ���.
// . �ش� �ʿ��� �������� ���ȯ �ð��� �����Ѵ�.
// . ���� �ʿ��� Ÿ ��忡 ���� ������ �ִ�.(�Ǿƽĺ� �Ѵ�.)
// . 
// . CrystalWarpNPCManager(multi_index map) �� ���� ����
// . ��Ŀ��� ���������� ���� ������ ����. (�ð� �˹�, ���� ����, �躸�� ���.)

void 
GameDominationField::_Init(int idxWarp)
{
    DWORD delayTime = 0; 
    const SLOTCODE wSkillCode = eACTION_START_SKILL_CODE+eACTION_CRYSTAL_WARP_CREATE;
    const SkillScriptInfo* pBaseSkillInfo = 
        SkillInfoParser::Instance()->GetSkillInfo(wSkillCode);

    if( pBaseSkillInfo )
        delayTime = pBaseSkillInfo->m_dwCoolTime;   

    _DestroyCrystalWarp(std::make_pair(eGuildType(idxWarp), crystalwarp_1st));
    _DestroyCrystalWarp(std::make_pair(eGuildType(idxWarp), crystalwarp_2nd));
    tupleWarp[idxWarp].get<crystalwarp_cooltime>().SetTimer(delayTime);

    tupleWarp[idxWarp].get<idx_guildguid>() = 0;
}

BOOL 
GameDominationField::InsertCrystalWarp(GUILDGUID guildGuid, CrystalWarp* pCrystalWarp)
{
    ASSERT(pCrystalWarp);

    if (!guildGuid)
        return FALSE;

    int idxGuild = _FindGuildIdx(guildGuid);
    if (empty_idx == idxGuild)
        return FALSE;

    if (NULL == tupleWarp[idxGuild].get<crystalwarp_1st>())
        tupleWarp[idxGuild].get<crystalwarp_1st>() = pCrystalWarp;
    else if (NULL == tupleWarp[idxGuild].get<crystalwarp_2nd>())
        tupleWarp[idxGuild].get<crystalwarp_2nd>() = pCrystalWarp;
    else
        return FALSE;

    const DWORD dwCrystalWarpKey = pCrystalWarp->GetObjectKey();
    _NotifyCrystalWarpState(guildGuid, dwCrystalWarpKey, eCRYSTALWARP_STATE_CREATE);

    return TRUE;
}


GameDominationField::CrystalWarpIndex
GameDominationField::_FindCrystalWarpIdx(GUILDGUID guildGuid, DWORD dwObjKey)
{
    ASSERT(guildGuid);

    for(int i = 0; i < max_guild ; ++i)
    {
        if(tupleWarp[i].head == guildGuid)
        { 
            const CrystalWarp* pCrystalWarp1 = tupleWarp[i].get<crystalwarp_1st>();
            const CrystalWarp* pCrystalWarp2 = tupleWarp[i].get<crystalwarp_2nd>();

            if(NULL != pCrystalWarp1 && 
                pCrystalWarp1->GetObjectKey() == dwObjKey)
                return std::make_pair(eGuildType(i),crystalwarp_1st);

            if(NULL != pCrystalWarp2 && 
                pCrystalWarp2->GetObjectKey() == dwObjKey)
                return std::make_pair(eGuildType(i),crystalwarp_2nd);

            return std::make_pair(eGuildType(i),eDataType(empty_idx));
        }
    }

    return std::make_pair((eGuildType)empty_idx,(eDataType)empty_idx);
}


BOOL 
GameDominationField::_IsCrystalCountFull(int idxWarp) 
{
    if (NULL != tupleWarp[idxWarp].get<crystalwarp_1st>() &&
        NULL != tupleWarp[idxWarp].get<crystalwarp_2nd>() )
        return TRUE;

    return FALSE;
}

RC::eSUMMON_RESULT 
GameDominationField::SetCrystalWarpCommand(eCRYSTALWARP_COMMAND eCommand, GUILDGUID guildGuid, 
                                           WzVector& wvPos, DWORD dwObjKey, Player* pPlayer)
{
    ASSERT(pPlayer);
    ASSERT(pPlayer->GetField());

    if (!guildGuid)
        return RC::RC_SUMMON_INVALID_GUILD;

    CrystalWarpIndex pairIdx = _FindCrystalWarpIdx(guildGuid,dwObjKey);

    if (empty_idx == pairIdx.first)
        return RC::RC_SUMMON_INVALID_CRYSTALWARP;

    switch(eCommand)
    {
    case eCRYSTALWARP_COMMAND_CREATE:
        {
            if (FALSE != _IsCrystalCountFull(pairIdx.first))
                return RC::RC_SUMMON_CRYSTALWARP_COUNTFULL;

            util::Timer& rTimer = tupleWarp[pairIdx.first].get<crystalwarp_cooltime>();
            if (FALSE == rTimer.IsExpired())
                return RC::RC_SUMMON_COOLTIME_ERROR;

            const SLOTCODE skillCode = eACTION_START_SKILL_CODE+eACTION_CRYSTAL_WARP_CREATE;
            const DWORD dwMainTargetKey = pPlayer->GetObjectKey();

            if (false == _CrystalWarpCommandSkill(pPlayer, skillCode, dwMainTargetKey, wvPos))
                return RC::RC_SUMMON_FAIL_CREATE_SKILL;
        }
        break;

    case eCRYSTALWARP_COMMAND_DESTROY:
        {
            if (empty_idx == pairIdx.second)
                return RC::RC_SUMMON_INVALID_CRYSTALWARP;

            const SLOTCODE skillCode = eACTION_START_SKILL_CODE+eACTION_CRYSTAL_WARP_DESTROY;
            const DWORD dwMainTargetKey = dwObjKey;

            if (false == _CrystalWarpCommandSkill(pPlayer, skillCode, dwMainTargetKey, wvPos))
                return RC::RC_SUMMON_FAIL_CREATE_SKILL;
        }
        break;

    case eCRYSTALWARP_COMMAND_EXECUTE:
        {
            if (empty_idx == pairIdx.second)
                return RC::RC_SUMMON_INVALID_CRYSTALWARP;

            CrystalWarp* pCrystalWarp = _GetCrystalWarp(_Flip(pairIdx));
            if (NULL == pCrystalWarp)
                return RC::RC_SUMMON_CANNOT_WARP_CMD;
            
            WzVector vWarpPos = pCrystalWarp->GetRandomPosInAround(dist_randomposinaround);

            GameField*  pField = pPlayer->GetField();
            if (NULL != pField)
            {
                if (FALSE == pField->TeleportObject(pPlayer, &vWarpPos))
                    return RC::RC_SUMMON_FAIL_WARP_CMD;
            }
        }
        break;
    }

    return RC::RC_SUMMON_SUCCESS;
}

// CoolTimer : ���ϸʿ��� ���� ��忡 ���� ���������� ��Ÿ���� �����Ѵ�.
// 
util::Timer* 
GameDominationField::GetCoolTimer(Character* pOwnerChar)
{
    if (FALSE == pOwnerChar->IsEqualObjectKind(PLAYER_OBJECT))
        return NULL;

    const Player* pPlayer = (Player*)pOwnerChar;
    const int guildIdx    = _FindGuildIdx(pPlayer->GetGuildGuid());
    util::Timer& rTimer   = tupleWarp[guildIdx].get<crystalwarp_cooltime>();
    
    return &rTimer;
}

bool 
GameDominationField::_CrystalWarpCommandSkill(Player* pPlayer, SLOTCODE skillCode, 
                                                   DWORD dwMainTargetKey, WzVector& wvPos) 
{
    const WzVector* cur_pos = pPlayer->GetPosPtr();
    SkillInfo skill_info_data(pPlayer, skillCode, 0);
    skill_info_data.SetArgsObjectInfo(dwMainTargetKey, cur_pos, cur_pos, &wvPos);
    skill_info_data.SetArgsSkillExt(skill_info_data.kDefaultSequence,
                                    skill_info_data.kDefaultSkillEffect,
                                    skill_info_data.kDefaultSkillDelay,
                                    SKILL_FACTOR_DOMINATION,
                                    ATTACK_PROPENSITY_NORMAL,
                                    skill_info_data.kDefaultEvent);

    const RC::eSKILL_RESULT rc = pPlayer->UseSkill(&skill_info_data, ATTACK_PROPENSITY_NORMAL);
    if (RC::RC_SKILL_SUCCESS != rc)
    {
        SUNLOG(eDEV_LOG, "[SetCrystalWarpCommand] RC_SKILL_FAILED : rc[%d]", rc);

        MSG_CG_SKILL_ACTION_NAK msgNAK;
        msgNAK.m_byErrorCode = (BYTE)rc;
        pPlayer->SendPacket(&msgNAK, sizeof(msgNAK));

        return false;
    }

    return true;
}

//��ȯ ������ (eCRYSTALWARP_COMMAND_DESTROY)
bool 
GameDominationField::DestroyCrystalWarp(Player* pPlayer, DWORD dwObjkey) 
{
    ASSERT(pPlayer);

    const GUILDGUID guildGuid = pPlayer->GetGuildGuid();
    CrystalWarpIndex pairIdx = _FindCrystalWarpIdx(guildGuid, dwObjkey);

    if (empty_idx != pairIdx.first &&
        empty_idx != pairIdx.second)
        _NotifyCrystalWarpState(guildGuid, dwObjkey, eCRYSTALWARP_STATE_DESTROY);

    CrystalWarp* pCrystalWarp = _GetCrystalWarp(pairIdx);
    if (pCrystalWarp)
        GAMELOG->LogDomination(ACT_DOMINATION_CRYSTALWARP_DESTROY, pPlayer, pCrystalWarp);

    return _DestroyCrystalWarp(pairIdx);
}

bool 
GameDominationField::_DestroyCrystalWarp(const CrystalWarpIndex& pairIdx) 
{
    ASSERT(empty_idx != pairIdx.first);
    ASSERT(empty_idx != pairIdx.second);

    CrystalWarp* pCrystalWarp = _GetCrystalWarp(pairIdx);
    if (pCrystalWarp)
    {
        const DWORD dwCrystalWarpKey = pCrystalWarp->GetObjectKey();
        const GUILDGUID guildGuid = tupleWarp[pairIdx.first].get<idx_guildguid>();

        GameField* pField = pCrystalWarp->GetField();
        pField->DestroyObject(pCrystalWarp);

        switch(pairIdx.second)
        {
        case crystalwarp_1st:
            tupleWarp[pairIdx.first].get<crystalwarp_1st>() = NULL;
            break;

        case crystalwarp_2nd:
            tupleWarp[pairIdx.first].get<crystalwarp_2nd>() = NULL;
            break;
        }

        return true;
    }

    return false;
}

void 
GameDominationField::OnDeadCrystalWarp(DWORD dwObjkey)
{
    for(int i = 0; i < max_guild ; ++i)
    {
        const CrystalWarp* pCrystalWarp1 = tupleWarp[i].get<crystalwarp_1st>();
        const CrystalWarp* pCrystalWarp2 = tupleWarp[i].get<crystalwarp_2nd>();

        if (pCrystalWarp1 != NULL &&
            pCrystalWarp1->GetObjectKey() == dwObjkey)
        {
            tupleWarp[i].get<crystalwarp_1st>() = NULL;
            _NotifyCrystalWarpState(tupleWarp[i].get<idx_guildguid>(), 
                                    dwObjkey, eCRYSTALWARP_STATE_DEAD);
        }

        if (pCrystalWarp2 != NULL &&
            pCrystalWarp2->GetObjectKey() == dwObjkey)
        {
            tupleWarp[i].get<crystalwarp_2nd>() = NULL;
            _NotifyCrystalWarpState(tupleWarp[i].get<idx_guildguid>(), 
                                    dwObjkey, eCRYSTALWARP_STATE_DEAD);
        }
    }
}

void 
GameDominationField::_NotifyCrystalWarpState(GUILDGUID guildGuid,
                                                  DWORD dwCrystalWarpKey, eCRYSTALWARP_STATE eState)
{
    GameGuild* pGuild = g_GameGuildManager.FindGuild(guildGuid);

    if (NULL != pGuild)
    {
        MSG_CG_DOMINATION_CRYSTALWARP_STATE_BRD brdMsg;
        brdMsg.m_dwCrystalWarpKey = dwCrystalWarpKey;
        brdMsg.m_byState = (BYTE)eState;

        pGuild->SendPacketMember(brdMsg);
    }
}

//////////////////////////////////////////////////////////////////////////
// update, event
//
// . agent�� ��� ������ ����(ObserverControl, WarEventControl)���� ���� ����.(��ɿ� ���� ������ ����)


void 
GameDominationField::SetEventState(eDOMINATION_EVENT_STATE eventState)
{
    if (curEventState_ == eventState)
        return;

#ifdef _NA_007128_20140326_MONDSCHIEN_DOMINATION 
    const MAPCODE field_map_code = 
        DominationInfoParser::Instance()->GetDominationLinkMapCode(GetMapCode());
    const eDOMINATION_TYPE domination_type = 
        DominationInfoParser::Instance()->GetDominationTypeFromFieldCode(field_map_code);
#endif //_NA_007128_20140326_MONDSCHIEN_DOMINATION 

    switch(eventState)
    {
    case DOMINATION_EVENT_AUCTION_START:    // ��� ���� ~
        reward_event_timer_.SetTimer(event_reward);
        break;

    case DOMINATION_EVENT_AUCTION_END:      // ��� ��
        reward_event_timer_.DisableCheckTime();
        break;

    case DOMINATION_EVENT_SELECT_ATTACKGUILD://���ݱ�� ���� �̺�Ʈ �߰�
        GameDominationManager::Instance()->SetDominationInfoDisGuild(GetMapCode());
        //_NA_20110406_CHANGE_RULE_FOR_DOMINATION_JOIN
        GameDominationManager::Instance()->InitMemberList();
        break;

    case DOMINATION_EVENT_SELECT_ATTACKGUILD_END:
        break;

    case DOMINATION_EVENT_SYSTEM_RESET: 
        _SendKickNotify();
#ifdef _NA_007128_20140326_MONDSCHIEN_DOMINATION 
        _SetUpdatedResult(false);
#endif //_NA_007128_20140326_MONDSCHIEN_DOMINATION 
#ifdef _NA_007488_20140811_MONDSCHUEN_INITIALIZE_N_REWARD
        // ��Ʈ���� �������� DOMINATION_EVENT_SYSTEM_RESET �� ���ֱ�尡 �ʱ�ȭ �ȴ�.
        if (domination_type == DOMINATION_TYPE_MONDSCHIEN)
        {
            DOMINATION_INFO domination_info;
            memset(&domination_info, 0, sizeof(domination_info));
            domination_info.m_FieldCode = GetMapCode();
            domination_info.m_rewardEvent.m_byEventStats =domination_info.m_rewardEvent.EventStats::all_Inactive;
            GameDominationManager::Instance()->SendUpdateDominationInfo(domination_info, DOMINATION_INIT);
        }
#endif // _NA_000000_20140811_MONDSCHUEN_INITIALIZE_N_REWARD
#ifdef _NA_008012_20150204_SUN_RANKING_SYSTEM
        m_pBattleResult->Release();
#endif // _NA_008012_20150204_SUN_RANKING_SYSTEM
        break;
       
    case DOMINATION_EVENT_WAIT: // �ش� ������ ���� �ð��� ms�� ��ȯ �صд�.
        _SetWarTime();
        synkTimer_.SetTimer(time_synk);
        //_NA_0_20091118_GAMEROOM_BATTLE_RESULT
        m_pBattleResult->Clear();
    
#ifdef _NA_006826_20130722_DOMINATION_RENEWAL
        GameDominationManager::Instance()->ChangeAreaProgressState(GetMapCode(), DOMINATION_PROGRESS_STATE_READY);
#endif //_NA_006826_20130722_DOMINATION_RENEWAL
        break;

    case DOMINATION_EVENT_START:
        if (DOMINATION_EVENT_WAIT != curEventState_)    // ������ ���۽� START ���� �� ��� wait ���¸� ��ģ��.
            SetEventState(DOMINATION_EVENT_WAIT);

        _StartTriggerActive();

        warTimer_.SetTimer(dwWarTime_);
        _SendAllPlayerTimeSynk(eventState);
#ifdef _NA_006826_20130722_DOMINATION_RENEWAL
        GameDominationManager::Instance()->ChangeAreaProgressState(GetMapCode(), DOMINATION_PROGRESS_STATE_START);
#endif //_NA_006826_20130722_DOMINATION_RENEWAL

#ifdef _NA_007128_20140326_MONDSCHIEN_DOMINATION 
        //��Ʈ���� �������� DOMINATION_EVENT_START�� ���ÿ� ��� �÷��̾ ������������ �̵���Ų��.
        if (domination_type == DOMINATION_TYPE_MONDSCHIEN)
        {
            _AllPlayerTeleportToStartingArea(); 
        }
#endif //_NA_007128_20140326_MONDSCHIEN_DOMINATION 
        break;

    case DOMINATION_EVENT_END:
#ifdef _NA_006826_20130722_DOMINATION_RENEWAL
        // �������� ���ݱ�忡 ���� ���ɵǸ� ������°� END�� �ȴ�.
        GameDominationManager::Instance()->ChangeAreaProgressState(GetMapCode(), DOMINATION_PROGRESS_STATE_END);
#endif //_NA_006826_20130722_DOMINATION_RENEWAL
        synkTimer_.DisableCheckTime();
        _EndTriggerActive();
        _SendAllPlayerTimeSynk(eventState);
        reward_event_timer_.SetTimer(event_reward);
        //_NA_20110406_CHANGE_RULE_FOR_DOMINATION_JOIN
        GameDominationManager::Instance()->DeleteMemberList(GetMapCode());
    //_NA_003457_20111013_EXCEPT_DOMINATION_REWARD
        ApplyAllRewardOption(TRUE);
#ifdef _NA_008012_20150204_SUN_RANKING_SYSTEM
        DominationBattleResult* domi_battle_result = static_cast<DominationBattleResult*>(m_pBattleResult);
        if (domi_battle_result != NULL)
        {
            domi_battle_result->ProcessSunRanking();
        }
#endif // _NA_008012_20150204_SUN_RANKING_SYSTEM
        break;
    }

    // ���̺꿡�� �̺�Ʈ �αװ� �ʿ��Ҷ��� �߻� �ϹǷ� eCRITICAL_LOG �� ������ �����.
    const char* event_string = eventState == DOMINATION_EVENT_AUCTION_START ? "AUCTION_START" 
                             : eventState == DOMINATION_EVENT_AUCTION_START2? "AUCTION_START2"
                             : eventState == DOMINATION_EVENT_SYSTEM_RESET ? "SYSTEM_RESET"
                             : eventState == DOMINATION_EVENT_AUCTION_END  ? "AUCTION_END"
                             : eventState == DOMINATION_EVENT_SELECT_ATTACKGUILD ? "SELECT_ATTACKGUILD"
                             : eventState == DOMINATION_EVENT_SELECT_ATTACKGUILD_END ? "SELECT_ATTACKGUILD_END"
                             : eventState == DOMINATION_EVENT_WAIT  ? "WAIT"
                             : eventState == DOMINATION_EVENT_START ? "START"
                             : eventState == DOMINATION_EVENT_END   ? "END"
                             : eventState == DOMINATION_EVENT_MAX   ? "MAX" 
                             : "EMPTY";

    SUNLOG( eCRITICAL_LOG, "[GameDominationField::SetEventState] map[%d] Event[%s] off[%d] def[%d]"
                            ,m_MapCode, event_string, GetOffenseGuild(), GetDefenseGuild() );

    curEventState_ = eventState;

#ifdef _NA_006826_20130722_DOMINATION_RENEWAL
    // �������� �����Ȳ ������Ʈ
    if (curEventState_ == DOMINATION_EVENT_END)
    {
        bool is_domination_progressing = GameDominationManager::Instance()->CheckDominationIsProgressing();
        if (!is_domination_progressing) {
            GameDominationManager::Instance()->ChangeAllAreaProgressState(DOMINATION_PROGRESS_STATE_NONE);
        }
    }
#endif //_NA_006826_20130722_DOMINATION_RENEWAL
}


void 
GameDominationField::Update(DWORD dwDeltaTick)
{
    GameZone::Update(dwDeltaTick);
    if (synkTimer_.IsExpired())
    {
        switch(curEventState_)
        {
        case DOMINATION_EVENT_WAIT:
            _SendDominationNotify(DOMINATION_WAR_TIME);
            break;

        case DOMINATION_EVENT_START:
            _SendAllPlayerTimeSynk(curEventState_);
            break;
        //_NA_20110406_CHANGE_RULE_FOR_DOMINATION_JOIN
        case DOMINATION_EVENT_SELECT_ATTACKGUILD_END:
            _SendDominationNotify(DOMINATION_WAR_TIME);
            break;
        }
    }

    if (warTimer_.IsExpiredManual(FALSE))
    {
#ifdef _NA_007128_20140326_MONDSCHIEN_DOMINATION
        MAPCODE field_map_code = 
            DominationInfoParser::Instance()->GetDominationLinkMapCode(GetMapCode());
        const eDOMINATION_TYPE domination_type =
            DominationInfoParser::Instance()->GetDominationTypeFromFieldCode(field_map_code);

        switch(domination_type)
        {
        case DOMINATION_TYPE_MONDSCHIEN:
            ResultUpdateForMondshien();     // ��Ʈ���� ������ ��
            break;
        case DOMINATION_TYPE_NORMAL:
            _ResultUpdate(DOMINATION_SUCCESS_DEFENCE);  // ��� ����
            break;
        }
#else
        _ResultUpdate(DOMINATION_SUCCESS_DEFENCE);  // ��� ����
#endif  //_NA_007128_20140326_MONDSCHIEN_DOMINATION
    }

    // added by _NA_0_20100705_DOMINATION_EVENT
    if (reward_event_timer_.IsExpired())
    {
        switch(reward_event_.m_byEventStats)
        {
        case DOMINATION_REWARD_EVENT_INFO::all_active: //����ڿ� ���� �̺�Ʈ�� ����� ����
            break;

        case DOMINATION_REWARD_EVENT_INFO::all_Inactive: 
            {
                DWORD dwTime = 0;
                const WORD wDayOfWeek = util::GetDateTime_Week_HHMMSS(dwTime);
                if (wDayOfWeek == reward_event_.m_wEventDayOfWeek &&
                    dwTime > reward_event_.m_dwEventTime)
                {
                    _StartRewardEvent(wDayOfWeek, dwTime);
                }
            }
            break;

        case DOMINATION_REWARD_EVENT_INFO::event_started:
            {
                // �̺�Ʈ ������
                if (event_stats_ != DOMINATION_REWARD_EVENT_INFO::event_started)
                {
                    if (_EventTriggerActive() != FALSE)
                        event_stats_ = DOMINATION_REWARD_EVENT_INFO::event_started;
                }
            }
            break;
        }
    }
}

void 
GameDominationField::_StartRewardEvent(const WORD wDayOfWeek, const DWORD dwTime)
{
    if (_EventTriggerActive() != FALSE)
        event_stats_ = DOMINATION_REWARD_EVENT_INFO::event_started;

    reward_event_.m_byEventStats = DOMINATION_REWARD_EVENT_INFO::event_started;
    reward_event_.m_dwEventTime = dwTime;
    reward_event_.m_wEventDayOfWeek = wDayOfWeek;

    const MAPCODE mapCode = GetMapCode();
    GameDominationManager* manager = GameDominationManager::Instance();
    manager->SetDominationGuildInfo(mapCode, DOMINATION_INFO_SET, reward_event_);

    GAMELOG->LogDomination(ACT_DOMINATION_REWORD_EVENT_START, mapCode);

    SUNLOG(eFULL_LOG, 
           _T("|%s|������ ���� �̺�Ʈ�� �����մϴ�.|�̺�Ʈ���� = %d, �̺�Ʈ���� = %d, �̺�Ʈ�ð� = %u, ���ڵ� = %d, ����� = %d, ���ݱ�� = %d|"), 
           __FUNCTION__, 
           reward_event_.m_byEventStats, 
           reward_event_.m_wEventDayOfWeek, 
           reward_event_.m_dwEventTime, 
           mapCode, 
           GetDefenseGuild(), 
           GetOffenseGuild());
}


void 
GameDominationField::GMRewardEventReset(const WORD wDayOfWeek, const DWORD dwTime)
{
    event_stats_ = DOMINATION_REWARD_EVENT_INFO::all_Inactive;

    reward_event_.m_byEventStats = DOMINATION_REWARD_EVENT_INFO::all_Inactive;
    reward_event_.m_dwEventTime = dwTime;
    reward_event_.m_wEventDayOfWeek = wDayOfWeek;

    const MAPCODE mapCode = GetMapCode();
    GameDominationManager* manager = GameDominationManager::Instance();
    manager->SetDominationGuildInfo(mapCode, DOMINATION_INFO_SET, reward_event_);
}

#ifdef _NA_007128_20140326_MONDSCHIEN_DOMINATION
const sDOMINATION_EVENT_INFO* GameDominationField::_GetCurEventInfo(const WORD day_of_week, const DWORD time) const
{
    //GM�̺�Ʈ�� ��ϵǾ��ִٸ� ó���Ѵ�.
    DominationInfoParser* pParser = DominationInfoParser::Instance();
    const sDOMINATION_EVENT_INFO* event_info = pParser->GetGMEventInfo(day_of_week, time, GetMapCode());
    if (event_info != NULL)
    {
        return event_info;
    }

    EventInfos::const_iterator event_infos_it =  event_infos_.begin();
    EventInfos::const_iterator event_infos_end = event_infos_.end();

    //��ϵǾ��� �̺�Ʈ�� ��ȸ�Ѵ�.
    for ( ;event_infos_it != event_infos_end; ++event_infos_it)
    {
        const sDOMINATION_EVENT_INFO& event_info = event_infos_it->second;
        //���ð� ���� �������� üũ�Ѵ�.
        if (event_info.m_wDayOfWeek != day_of_week)
        {
            continue;
        }
        //���� �ð��� �̺�Ʈ�� �ð��� ���ԵǴ��� Ȯ���Ѵ�.
        const DWORD start_time = event_info.m_dwTime[DOMINATION_EVENT_AUCTION_START];
        const DWORD end_time = event_info.m_dwTime[DOMINATION_EVENT_AUCTION_START2];
        const bool is_include = Between(time, start_time, end_time);
        if (is_include == false)
        {
            continue;
        }
        return &event_infos_it->second;
    }
    return NULL;
}
#endif  //_NA_007128_20140326_MONDSCHIEN_DOMINATION
//////////////////////////////////////////////////////////////////////////
// ���� �ð�

// ��ũ��Ʈ�� �ش� ������ ���� �ð��� ms�� ��ȯ
// agent�� ��û�ϴ����� ������.. game server���� ���� ���Ѵ�.
void 
GameDominationField::_SetWarTime()
{
    DWORD  dwTime = 0;
    const WORD wDayOfWeek = util::GetDateTime_Week_HHMMSS(dwTime);
#ifdef _NA_007128_20140326_MONDSCHIEN_DOMINATION
    const DWORD wzTime = _GetWarTime(wDayOfWeek,dwTime);
#else
    DominationInfoParser* pParser = DominationInfoParser::Instance();

    const DWORD wzTime = pParser->GetWarTime(wDayOfWeek,dwTime);
#endif  //_NA_007128_20140326_MONDSCHIEN_DOMINATION
    dwWarTime_ = util::HHMMSSToSecond(wzTime) * 1000;

#ifdef _DEBUG
    // ���� ������ �˸���. ����׿�
    MSG_CG_DOMINATION_START_NOTIFY_BRD brdStart;
    brdStart.m_byTime  = UCHAR_MAX;

#ifdef _NA_008512_20151001_MODIFY_DOMINATION_RENEWAL
    //PlayerManager::Instance()->SendPacketToAllPlayer(&brdStart, sizeof(brdStart));
    GameGuild* pDefGuild = g_GameGuildManager.FindGuild(GetDefenseGuild());
    if (NULL != pDefGuild)
    {
        pDefGuild->SendPacketMember(brdStart);
    }
#else
    GameGuild* pDefGuild = g_GameGuildManager.FindGuild(GetDefenseGuild());
    GameGuild* pOffGuild = g_GameGuildManager.FindGuild(GetOffenseGuild());

    if (NULL != pDefGuild)
        pDefGuild->SendPacketMember(brdStart);

    if (NULL != pOffGuild)
        pOffGuild->SendPacketMember(brdStart);
#endif //_NA_008512_20151001_MODIFY_DOMINATION_RENEWAL
#endif
}

#ifdef _NA_007128_20140326_MONDSCHIEN_DOMINATION
DWORD GameDominationField::_GetWarTime(const WORD day_of_week, const DWORD time) const
{
    const sDOMINATION_EVENT_INFO* const event_info = _GetCurEventInfo(day_of_week, time);
    if (event_info != NULL)
    {
        const DWORD start_time = event_info->m_dwTime[DOMINATION_EVENT_START];
        const DWORD end_time = event_info->m_dwTime[DOMINATION_EVENT_END];
        return util::CarcHHMMSS(end_time, max(start_time, time), false);
    }
    return 0;
}

DWORD GameDominationField::_GetRemindEventTime(
    const WORD day_of_week, const DWORD time, const eDOMINATION_EVENT_STATE event_state) const
{
    const sDOMINATION_EVENT_INFO* const event_info = _GetCurEventInfo(day_of_week, time);
    if (event_info != NULL)
    {
        const DWORD start_time = event_info->m_dwTime[event_state];
        return util::CarcHHMMSS(start_time, time, false);
    }
    return 0;
}
#endif  //_NA_007128_20140326_MONDSCHIEN_DOMINATION

void 
GameDominationField::_SendAllPlayerTimeSynk(eDOMINATION_EVENT_STATE eventState)
{
    MSG_CG_DOMINATION_TIME_SYNC_BRD brdTimeSync;
    brdTimeSync.m_byEventState = eventState;
    brdTimeSync.m_dwRemainedTime = warTimer_.GetRemainedTime();

    GameField* pGameField = FindGameField(GetMapCode());
    if (pGameField)
        pGameField->SendPacketToAll(&brdTimeSync, sizeof(brdTimeSync));
}

//_NA_0_20100705_DOMINATION_EVENT
BOOL 
GameDominationField::_EventTriggerActive()
{
    if (0 != dwEventTriggerID_)
    {
        if (FALSE == TriggerActive(dwEventTriggerID_))
            return FALSE;
    }

    return TRUE;
}

//_NA_0_20100705_DOMINATION_EVENT
void 
GameDominationField::EndEventTrigger()
{
    if (event_stats_ != DOMINATION_REWARD_EVENT_INFO::event_started ||
        reward_event_.m_byEventStats != DOMINATION_REWARD_EVENT_INFO::event_started)
    {
        SUNLOG(eCRITICAL_LOG, "GameDominationField::event_stats_Error" 
                              "event_stats_[%d] reward_event_.m_byEventStats[%d] ",
                              event_stats_, reward_event_.m_byEventStats);
       
        // all_active ó���� ���� ���� ��� ���� �� �����Ƿ� ���� ó�� �Ѵ�..
    }

    event_stats_ = DOMINATION_REWARD_EVENT_INFO::all_active;
    reward_event_.m_byEventStats = DOMINATION_REWARD_EVENT_INFO::all_active;

    const MAPCODE mapCode = GetMapCode();
    GameDominationManager* manager = GameDominationManager::Instance();
    manager->SetDominationGuildInfo(mapCode, DOMINATION_INFO_SET, reward_event_);
}

void 
GameDominationField::_StartTriggerActive()
{
    if (0 != dwStartTriggerID_)
    {
        if (FALSE != TriggerActive(dwStartTriggerID_))
            dwStartTriggerID_ = 0;
    }
}

// ���� Ʈ���� �ߵ��� MAPNPC�� ��������(������� �ʴ»���) �̾���Ѵ�.
// ���� ���·� ����� �ߵ��� Ʈ���Ŵ� InActive ��Ų��.  

void 
GameDominationField::_EndTriggerActive()
{
    if (0 != dwEndTriggerID_)
    {
        AllTriggerInactive();       
        _InitAllNPC();

        if (FALSE != TriggerActive(dwEndTriggerID_))
           dwEndTriggerID_ = 0;
    }
}

void 
GameDominationField::_InitAllNPC()
{
    const _GAMEFIELD_HASH& rFields = GetFieldHash();
    FOREACH_CONTAINER(const _GAMEFIELD_HASH::value_type& rField, rFields, _GAMEFIELD_HASH)
    {
        GameField* const pField = rField.second;

        typedef GameField::_NPC_HASH    _NPC_HASH;
        const _NPC_HASH& rMobs = pField->GetRefNPCList();
        FOREACH_CONTAINER(const _NPC_HASH::value_type& rMob, rMobs, _NPC_HASH)
        {
            NPC* const pNPC = rMob.second;
//#ifdef _NA_004126_20120111_CHANGE_GAME_SERVER_OBJECT_FACTORY
            if (pNPC != NULL && pNPC->IsDeleted())
            {
                const Object::Key key = pNPC->GetObjectKey();
                SUNLOG(
                    eCRITICAL_LOG, 
                    _T("|%s|%d|Object is already deleted.|ObjectKey = %d|"), 
                    __FUNCTION__, 
                    __LINE__, 
                    key
                );
                continue;
            }
//#endif
            if (eNPC_DOMINATION_MAPOBJECT_NPC == pNPC->GetGrade())
            {
                pNPC->IncreaseHP(pNPC->GetMaxHP());
                pNPC->SendHPChange();
            }
        }
    }
}
//////////////////////////////////////////////////////////////////////////
// ����

void 
GameDominationField::_SendKickNotify()
{
    MSG_CG_DOMINATION_KICK_NOTIFY_BRD brdKick;

    GameField* pGameField = FindGameField(GetMapCode());
    if (pGameField)
        pGameField->SendPacketToAll(&brdKick, sizeof(brdKick));
}

const NotifyTimeInfo* 
GameDominationField::_FindNotifyTimeInfo(eDOMINATION_NOTIFY_TIME notify_type,
                                                               uint8 notify_time)const
{
    NotifyTime::_Paircc range = startNotifyTime_.equal_range(notify_type);
    NotifyTime::const_iterator begin_iterator = range.first,
                         end_iterator = range.second;

    for (;begin_iterator != end_iterator; ++begin_iterator)
    {
        const NotifyTimeInfo& member_info = begin_iterator->second;
        if (member_info.notify_time_ == notify_time)
        {
            return &(member_info);
        }
    }
    return NULL;
}

void 
GameDominationField::_DeleteNotifyTimeInfo(eDOMINATION_NOTIFY_TIME notify_type,
                                                uint8 notify_time)
{
    NotifyTime::_Pairii range = startNotifyTime_.equal_range(notify_type);
    NotifyTime::iterator begin_iterator = range.first,
                         end_iterator = range.second;

    // CHANGES: f110512.3L, fixed try to invalid operation after deleted iterator.
    while (begin_iterator != end_iterator)
    {
        const NotifyTimeInfo& member_info = begin_iterator->second;
        if (member_info.notify_time_ != notify_time) {
            ++begin_iterator;
        }
        else {
            begin_iterator = startNotifyTime_.erase(begin_iterator);
        };
    }
}

eDOMINATION_EVENT_STATE 
GameDominationField::_GetEventbyNotifytype(const eDOMINATION_NOTIFY_TIME notify_type)const
{
    switch(notify_type)
    {
    case DOMINATION_WAR_TIME:
        return DOMINATION_EVENT_START;

    case DOMINATION_JOIN_TIME:
        return DOMINATION_EVENT_SYSTEM_RESET;
    }

    return DOMINATION_EVENT_MAX;
}

void 
GameDominationField::_SendDominationNotify(const eDOMINATION_NOTIFY_TIME notify_type)
{
    DWORD  current_time = 0;
    const WORD current_day_of_week = util::GetDateTime_Week_HHMMSS(current_time);

    DominationInfoParser* domination_parser = DominationInfoParser::Instance();

    const eDOMINATION_EVENT_STATE event_stat = _GetEventbyNotifytype(notify_type);
#ifdef _NA_007128_20140326_MONDSCHIEN_DOMINATION
    const DWORD remain_time = _GetRemindEventTime(current_day_of_week, current_time, event_stat);
#else
    const DWORD remain_time = domination_parser->GetRemindEventTime(current_day_of_week, 
        current_time,
        event_stat);
#endif  //_NA_007128_20140326_MONDSCHIEN_DOMINATION

    const DWORD remain_minute = util::HHMMSSToSecond(remain_time) / 60;

    const NotifyTimeInfo* notify_time_info = _FindNotifyTimeInfo(notify_type, remain_minute);
    if (notify_time_info == NULL)
    {
        return;
    }

    _DeleteNotifyTimeInfo(notify_type, remain_minute);
    
    MSG_CG_DOMINATION_START_NOTIFY_BRD brd_msg;
    //_NA_20110406_CHANGE_RULE_FOR_DOMINATION_JOIN
    brd_msg.notify_type = notify_type;
    brd_msg.m_byTime  = static_cast<BYTE>(remain_minute);
#ifdef _NA_007128_20140326_MONDSCHIEN_DOMINATION
    brd_msg.m_domination_type = domination_parser->GetDominationTypeFromFieldCode(GetEntryFieldCode());
#endif  //_NA_007128_20140326_MONDSCHIEN_DOMINATION

#ifdef _NA_008512_20151001_MODIFY_DOMINATION_RENEWAL
    //PlayerManager::Instance()->SendPacketToAllPlayer(&brd_msg, sizeof(brd_msg));
    GameGuild* defense_guild = g_GameGuildManager.FindGuild(GetDefenseGuild());
    if (defense_guild != NULL)
    {
        defense_guild->SendPacketMember(brd_msg);
    }
#else
    GameGuild* defense_guild = g_GameGuildManager.FindGuild(GetDefenseGuild());
    GameGuild* offense_guild = g_GameGuildManager.FindGuild(GetOffenseGuild());

    if (defense_guild != NULL)
    {
        defense_guild->SendPacketMember(brd_msg);
    }

    if (offense_guild != NULL)
    {
        offense_guild->SendPacketMember(brd_msg);
    }
#endif //_NA_008512_20151001_MODIFY_DOMINATION_RENEWAL
    
}

//////////////////////////////////////////////////////////////////////////
//��.��

#ifdef _NA_006826_20130722_DOMINATION_RENEWAL
void GameDominationField::SuccessStamp( Player* player )
{
    #ifdef _NA_007128_20140326_MONDSCHIEN_DOMINATION
    GameField* pField = player->GetField();
    if (!pField)
    {
        SUNLOG(eCRITICAL_LOG, __FUNCTION__" : ���� �÷��̾�(%d)�� �ʵ带 ã�� �� �����ϴ�. ",
            player->GetUserGuid());
        return;
    }

    const eDOMINATION_TYPE domination_type = 
            DominationInfoParser::Instance()->GetDominationTypeFromFieldCode(pField->GetFieldCode());
    switch(domination_type)
    {
    case DOMINATION_TYPE_MONDSCHIEN:
        if (!_TurnOver(player))
        {
            SUNLOG(eCRITICAL_LOG, __FUNCTION__": �Ͽ����� �����Ͽ����ϴ�. (���� �÷��̾� : %d)", 
                player->GetUserGuid());
            return;
        }
        break;
    case DOMINATION_TYPE_NORMAL:
        _ResultUpdate(DOMINATION_SUCCESS_OFFENCE, player);
        break;
    }
    #else
    _ResultUpdate(DOMINATION_SUCCESS_OFFENCE, player);
    #endif  //_NA_007128_20140326_MONDSCHIEN_DOMINATION
}
#else
void 
GameDominationField::SuccessStamp()
{
    _ResultUpdate(DOMINATION_SUCCESS_OFFENCE);
}
#endif // _NA_006826_20130722_DOMINATION_RENEWAL

// implemented by _NA_0_20100705_DOMINATION_EVENT
void 
GameDominationField::_ResultUpdate(eDOMINATION_EVENT_TYPE result_type)
{
    ASSERT(result_type == DOMINATION_SUCCESS_OFFENCE ||
           result_type == DOMINATION_SUCCESS_DEFENCE);

    warTimer_.DisableCheckTime();
    const MAPCODE mapCode = GetMapCode();

    const DominationInfoParser* const parser = DominationInfoParser::Instance();
    GameDominationManager* manager = GameDominationManager::Instance();

    const sDOMINATION_INFO* const info = parser->GetDominationInfo(mapCode);
    if (info == NULL)
    {
        SUNLOG(eCRITICAL_LOG, "���������� �� �ڵ�[%d] ������ ã���� �����ϴ�. ", mapCode);
        return;
    }

    if (info && info->m_byUseFixEvent)
    {
        reward_event_.m_wEventDayOfWeek = info->m_wEventDayOfWeek;
        reward_event_.m_dwEventTime = info->m_dwEventTime;
    }
    else
    {
        reward_event_.m_wEventDayOfWeek = random(0,6);
        reward_event_.m_dwEventTime = parser->GetRandomAuctionTime(reward_event_.m_wEventDayOfWeek);
    }
    reward_event_.m_byEventStats = DOMINATION_REWARD_EVENT_INFO::all_Inactive;

    manager->SetDominationGuildInfo(mapCode, result_type, reward_event_);

    const int log_type = (result_type == DOMINATION_SUCCESS_OFFENCE ? ACT_DOMINATION_SUCCESS_OFFENCE 
                                                                    : ACT_DOMINATION_SUCCESS_DEFENCE);

    GAMELOG->LogDomination(log_type, mapCode);
}

#ifdef _NA_006826_20130722_DOMINATION_RENEWAL
void GameDominationField::_ResultUpdate(eDOMINATION_EVENT_TYPE result_type, Player* player)
{
    ASSERT(result_type == DOMINATION_SUCCESS_OFFENCE ||
        result_type == DOMINATION_SUCCESS_DEFENCE);

    warTimer_.DisableCheckTime();
    const MAPCODE mapCode = GetMapCode();

    const DominationInfoParser* const parser = DominationInfoParser::Instance();
    GameDominationManager* manager = GameDominationManager::Instance();

    const sDOMINATION_INFO* const info = parser->GetDominationInfo(mapCode);
    if (info == NULL)
    {
        SUNLOG(eCRITICAL_LOG, "���������� �� �ڵ�[%d] ������ ã���� �����ϴ�. ", mapCode);
        return;
    }

    if (info && info->m_byUseFixEvent)
    {
        reward_event_.m_wEventDayOfWeek = info->m_wEventDayOfWeek;
        reward_event_.m_dwEventTime = info->m_dwEventTime;
    }
    else
    {
        reward_event_.m_wEventDayOfWeek = random(0,6);
        reward_event_.m_dwEventTime = parser->GetRandomAuctionTime(reward_event_.m_wEventDayOfWeek);
    }
    reward_event_.m_byEventStats = DOMINATION_REWARD_EVENT_INFO::all_Inactive;

    manager->SetDominationGuildInfo(mapCode, result_type, reward_event_, player);

    const int log_type = (result_type == DOMINATION_SUCCESS_OFFENCE ? ACT_DOMINATION_SUCCESS_OFFENCE 
        : ACT_DOMINATION_SUCCESS_DEFENCE);

    GAMELOG->LogDomination(log_type, mapCode);
}
#endif // _NA_006826_20130722_DOMINATION_RENEWAL

#ifdef _NA_007128_20140326_MONDSCHIEN_DOMINATION
bool GameDominationField::_TurnOver(Player* player)
{
    //������ ����� ������Ʈ �Ǿ��ٸ� �������� ����Ǿ��ٰ� �Ǵ��ϰ� ���̻� �Ͽ��� ��������
    if (_GetUpdatedResult() == true)
    {
        return false;
    }

    //���� �÷��̾� ����������
    GUILDGUID stamped_guild_guid = player->GetGuildGuid();
    if (stamped_guild_guid == NULL)
    {
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"]���� �÷��̾�(%d)�� ��� GUID�� ã�� �� �����ϴ�.", 
            player->GetUserGuid());
        return false;
    }
    
    GameDominationManager* manager = GameDominationManager::Instance();
    if(manager == NULL)
    {
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"]GameDominationManager�� ã�� �� �����ϴ�.");
        return false;
    }

    const DominationInfoParser* const parser = DominationInfoParser::Instance();
    if(parser == NULL)
    {
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"]DominationInfoParser�� ã�� �� �����ϴ�.");
        return false;
    }

    const MAPCODE mapCode = GetMapCode();
    //DOMINATION_TURN_OVER Event������ event ���¸� ��ȭ��Ű�� �ʰ� ������Ʈ�� �����Ѵ�
    manager->SetDominationGuildInfo(mapCode, DOMINATION_TURN_OVER, reward_event_, player);

    //�Ͽ��� Ʈ���� ȣ��
    WzID trjgger_id = StrToWzID("TURN");    //���� �ϵ��ڵ� ������
    if(TriggerActive(trjgger_id) == false)
    {
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"]�Ͽ��� Ʈ����(%g)�� ��Ƽ���ϴµ� �����Ͽ����ϴ�.", trjgger_id);
    }

    //������ ���� ������Ʈ
    const DOMINATION_INFO* domi_info = manager->GetDominationInfo(mapCode);
    if (domi_info == NULL)
    {
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"]Domination Info�� ã�� �� �����ϴ�.(Map Code : %d) ", mapCode);
    }
    manager->UpdateDominationInfo(*domi_info,DOMINATION_TURN_OVER);
    GUILDGUID defense_guild_guid = GetDefenseGuild();

    //�̴ϸ� ����Ʈ ACK ��Ŷ ���� //Ŭ���̾�Ʈ���� �̴ϸ� ������ ���� ���踦 ���� ����Ѵ�.
    MSG_CG_DOMINATION_MINIMAP_LIST_ACK msg_domi_minimap;
    msg_domi_minimap.m_ResultCode = RC::DOMINATION_RESULT_SUCCESS;
    msg_domi_minimap.m_NumDominationGuild = 
        manager->GetMiniMapInfo(msg_domi_minimap.m_GuildInfo, _countof(msg_domi_minimap.m_GuildInfo));
    SendPacketToAll(&msg_domi_minimap, msg_domi_minimap.GetSize());

    //��� �÷��̾� ������������ �̵�
    _AllPlayerTeleportToStartingArea();

    //���� ���� ���� ����
    _SendDefenseGuildAllienceListToAll(defense_guild_guid);

    //�Ͽ��� ��Ŷ ����
    MSG_CG_DOMINATION_TURNOVER_BRD msg_stamped_guild_info;
    msg_stamped_guild_info.m_winGuid = defense_guild_guid;
    msg_stamped_guild_info.m_mapCode = mapCode;
    memset(msg_stamped_guild_info.win_guild_name, 0, sizeof(msg_stamped_guild_info.win_guild_name));
    _tcsncpy(msg_stamped_guild_info.win_guild_name, player->GetCharInfo()->m_tszGuildName, MAX_GUILDNAME_LENGTH);
    SendPacketToAll(&msg_stamped_guild_info, sizeof(msg_stamped_guild_info));

    SUNLOG(eCRITICAL_LOG, "[�Ͽ���]%d ��尡 �Ͽ����� �����Ͽ����ϴ�.(Map Code : %d) ", stamped_guild_guid, mapCode);

    return true;
}
#endif  //_NA_007128_20140326_MONDSCHIEN_DOMINATION

BOOL 
GameDominationField::CanPVP( Player* pAttacker, Player* pTarget, OUT RC::eSKILL_RESULT* pErrCode ) {
    return fieldPvPRule_.CanPVP(pAttacker,pTarget,pErrCode);
}

BOOL 
GameDominationField::CanPVP( Player* pAttacker, Player* pTarget, OUT RC::eBATTLE_RESULT* pErrCode ) {
    return fieldPvPRule_.CanPVP(pAttacker,pTarget,pErrCode); 
}

eUSER_RELATION_TYPE	
GameDominationField::IsFriendPVP( Player* pAttacker,Player* pTarget ) { 
    return fieldPvPRule_.IsFriendPVP(pAttacker,pTarget); 
}

//_NA_008706_20160229_TEXTLOG_PK
void 
GameDominationField::KillPlayer( Player* pAttacker, Player* pDeadPlayer, BOOL bSummoned, DAMAGETYPE damage_value /*= 0*/ ) 
{ 
    fieldPvPRule_.KillPlayer(pAttacker,pDeadPlayer,bSummoned, damage_value); 
}

BOOL 
GameDominationField::IsPvPAtt( Player* pPlayer, WORD PTATroubld) {
    return fieldPvPRule_.IsPvPAtt(pPlayer, PTATroubld); 
}


//_NA_20110406_CHANGE_RULE_FOR_DOMINATION_JOIN
void 
GameDominationField::SetStampStartTime()
{
    stamp_start_time_ = GetTickCount();
}

void 
GameDominationField::InitStampCheckTime()
{
    stamp_start_time_ = 0;
}
bool 
GameDominationField::IsSuccessStamp()
{
    if (stamp_start_time_ == 0)
    {
        assert(!"Fail stamp");
        SUNLOG(eCRITICAL_LOG, __FUNCTION__":Fail stamp");
        return false;
    }

    //���������� �ð��� ����Ǹ� ���� ����..2013.03.14 ������..
    if(warTimer_.IsExpiredManual(FALSE))
    {
        SUNLOG(eCRITICAL_LOG, __FUNCTION__":Fail stamp");
        return false;
    }

    //��ȹ�󿡴� 10�������� ���� ���������� �����Ͽ� 9�ʷ� ����
    const uint8  CheckSecond = 9;
    DWORD sub_value = GetTickCount() - stamp_start_time_;
        
    return (sub_value >= (CheckSecond * 1000)) ? true : false; 
}

//_NA_003457_20111013_EXCEPT_DOMINATION_REWARD
void GameDominationField::ApplyAllRewardOption(const bool is_reward) const
{
    const GameField* const field = FindGameField(GetMapCode());
    if (field == NULL)
    {
        return;
    }
        
    const GameField::_PLAYER_HASH& players = field->GetRefPlayerList();
    FOREACH_CONTAINER(
        const GameField::_PLAYER_HASH::value_type& player_pair, players, GameField::_PLAYER_HASH)
    {
        Player* const player = player_pair.second;
        if (obj_util::IsValid(player, __FUNCTION__, __LINE__) == false)
        {
            continue;
        }

#ifdef _NA_008334_20150608_SONNENSCHEIN
        GameGuild* player_guild = player->GetGuild();
        if (player_guild != NULL) {
            player_guild->ApplyGuildRewardOption(player, is_reward);
        }
#else
        GameDominationManager::Instance()->ApplyAllRewardOption(player, is_reward);
        GameDominationManager::Instance()->SendAreaInfoToPlayer(player, is_reward);
#endif //_NA_008334_20150608_SONNENSCHEIN
    }
}


#ifdef _NA_007128_20140326_MONDSCHIEN_DOMINATION
void GameDominationField::_SendDefenseGuildAllienceListToAll( GUILDGUID defense_guild_guid )
{
    MSG_CG_DOMINATION_DEFENSE_GUILD_ALLIENCE_LIST_BRD msg_stamped_guild_allience_list;
    ZeroMemory(&msg_stamped_guild_allience_list.allience_list, 
        sizeof(msg_stamped_guild_allience_list.allience_list));

    GameGuild* const defense_guild = g_GameGuildManager.FindGuild(defense_guild_guid);

    if (defense_guild != NULL)
    {
        GuildRelationSystem::GuildRelationEntry* relation_entry = defense_guild->GetRelationEntry();
        STLX_VECTOR<GUILDGUID> relation_guild_guids;
        int allience_guild_count = relation_entry->GetRelationGuid(relation_guild_guids, eGUILD_RELATION_ALLIENCE);
        if (allience_guild_count != 0)
        {
            int index = 0;
            STLX_VECTOR<GUILDGUID>::const_iterator const_itr = relation_guild_guids.begin();
            for (;const_itr != relation_guild_guids.end(); ++const_itr )
            {
                GUILDGUID guild_guid = *const_itr;
                if (guild_guid)
                {
                    msg_stamped_guild_allience_list.allience_list[index++] = guild_guid;
                }
            }
        }
        SendPacketToAll(&msg_stamped_guild_allience_list, sizeof(msg_stamped_guild_allience_list));
    }
}

void GameDominationField::_AllPlayerTeleportToStartingArea()
{
    _PLAYER_HASH::const_iterator player_hash_it = m_PlayerHash.begin();
    _PLAYER_HASH::const_iterator player_hash_end = m_PlayerHash.end();
    for(; player_hash_it != player_hash_end ; ++player_hash_it)
    {
        Player* pPlayer = player_hash_it->second;
        if (obj_util::IsValid(pPlayer, __FUNCTION__, __LINE__) == false)
        {
            continue;
        }

        GUILDGUID user_guild_guid = pPlayer->GetGuildGuid();
        if(user_guild_guid == NULL)
        {
            SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"]�÷��̾�(%d)�� ��� GUID�� ã�� �� �����ϴ�. ",
                pPlayer->GetUserGuid());
            continue;
        }
        GameField* pField = pPlayer->GetField();
        if (!pField)
        {
            SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"]�÷��̾�(%d)�� �ʵ带 ã�� �� �����ϴ�. ",
                pPlayer->GetUserGuid());
            continue;
        }

        DWORD dwAreaID = 0;	
        WzVector vStartPos; 
        WzID AreaID = _GetStartingArea(pPlayer->GetGuildGuid());
        pField->GetAreaRandomStartingVector(AreaID, vStartPos);

        if (!pField->TeleportObject(pPlayer, &vStartPos))
        {
            SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"]�÷��̾�(%d)�� �ڷ���Ʈ�� ���� �Ͽ����ϴ�. ",
                pPlayer->GetUserGuid());
            continue;
        }
    }
}
bool GameDominationField::ResultUpdateForMondshien()
{
    //������ ������ ���� ���� �÷��� Ȱ��ȭ
    _SetUpdatedResult(true);

#if !defined(_NA_007488_20140811_MONDSCHUEN_INITIALIZE_N_REWARD)
    //������ �⼮�� ���� ��û
    MSG_DG_EVENT_UPDATE_ATTENDANCE_MONDSCHIEN_SYN send_msg;
    send_msg.guild_guid = GetDefenseGuild();
    g_pGameServer->SendToServer(GAME_DBPROXY, &send_msg, sizeof(send_msg));
#endif // _NA_000000_20140811_MONDSCHUEN_INITIALIZE_N_REWARD

    MAPCODE map_code = GetMapCode();

    GameDominationManager* domination_manager = GameDominationManager::Instance();
    if(domination_manager == NULL)
    {
        SUNLOG(eCRITICAL_LOG, "domination_manager�� ã�� �� �����ϴ�.");
        return false;
    }

    const DOMINATION_INFO* domination_info = domination_manager->GetDominationInfo(map_code);
    if(domination_info == NULL)
    {
        SUNLOG(eCRITICAL_LOG, "domination_info[%d]�� ã�� �� �����ϴ�.",map_code);
        return false;
    }

    //Ÿ�̸� ����
    warTimer_.DisableCheckTime();

    //��Ʈ������ �׻� ������ ������尡 �¸��ϴ� ����� ������
    domination_manager->SendUpdateDominationInfo(*domination_info, DOMINATION_SUCCESS_DEFENCE);

    GAMELOG->LogDomination(ACT_DOMINATION_SUCCESS_DEFENCE, map_code);

    return true;
}

void GameDominationField::_SetUpdatedResult( const bool updated )
{
    updated_result_ = updated;
}

bool GameDominationField::_GetUpdatedResult()
{
    return updated_result_;
}

#endif //_NA_007128_20140326_MONDSCHIEN_DOMINATION