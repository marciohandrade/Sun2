#include "stdafx.h"
#include <PacketStruct_AG.h>
#include <GameInstanceDungeon.h>

//==================================================================================================
//
#include <Status/StatusManager.h>
#include <World/GameWarControlManager.h>

// for debugging control
#undef WAVERIX_DEBUGGING_OUTPUTn
#undef WAVERIX_DEBUGGING_OUTPUTnV

#define WAVERIX_DEBUGGING_OUTPUTn(msg)    \
    SUNLOG(eCRITICAL_LOG, "[%s][%u][%u] %s\n", __FUNCTION__, __LINE__, \
    (DWORD)((GetTickCount()-tickvalue)/(1000)), msg);                  \
    tickvalue = GetTickCount()

#define WAVERIX_DEBUGGING_OUTPUTnV(value) \
{ CHAR _message[10];                    \
    WAVERIX_DEBUGGING_OUTPUTn(_itoa(value, _message, sizeof(_message))); }

static DWORD tickvalue = GetTickCount();

#include "GameSSQControlInfos.hxx"
#include "GameSSQPacketProcess.hxx"
#include "GameSSQUpdateCtrl.hxx"
#include "GameSSQControlManagerImpl.hxx"
#include "SSQHandlerInGame.h"

#ifdef _NA_001990_ACSYSTEM_ADD
#include "AchievementManager.h"
#endif //_NA_001990_ACSYSTEM_ADD

#ifdef _NA_002950_20110627_SSQ_NOTICE
#include "GameGuildManager.h"
#endif

//==================================================================================================
//  <GameSSQUpdateCtrl_xxx><Implementation>
//

VOID
GameSSQUpdateCtrl::MigrationDefault(const SSQCODE CodeOfSSQ,
                                    GameSSQControlManagerImpl* const pMgrImpl,
                                    SSQMsgRouterInGame* const pMsgParser,
                                    SSQInfo* const pSSQInfo)
{
    m_CodeOfSSQ  = CodeOfSSQ;
    m_CurType    = GameSSQControlInfo::TT_MAX;
    m_Timer.Stop();
    m_pManager   = pMgrImpl;
    m_pMsgParser = pMsgParser;
    m_pSSQInfo   = pSSQInfo;
    m_pSSQField  = 0;

    if(m_pMsgParser)
        m_pMsgParser->SetUpdateCtrl(this);

    //  SharedConstVariables
};


//--------------------------------------------------------------------------------------------------


VOID
GameSSQUpdateCtrl::RegisterUpdateCtrl(GameSSQUpdateCtrl* const pTarget)
{
    GameSSQControlManagerImpl* const pImpl = pTarget->m_pManager;
    pImpl->_RegisterUpdateCtrl(pTarget->m_CodeOfSSQ, pTarget);
}


//--------------------------------------------------------------------------------------------------


GameSSQUpdateCtrl*
GameSSQUpdateCtrl::ChangeToNextUpdateCtrl(GameSSQUpdateCtrl* const pCurUpdater,
                                          const GameSSQControlInfo::eTIME_TYPE nextType)
{
    GameSSQControlManagerImpl* const pImpl = pCurUpdater->m_pManager;
    const eTIME_TYPE curType = pCurUpdater->m_CurType;

    //  단일 Interface로 진행이 가능한지 여부 판단할 것.
    GameSSQUpdateCtrl* pNextUpdater = NULL;
    switch(nextType)
    {
#undef SSQ_UPDATE_CTRL_NODE
#define SSQ_UPDATE_CTRL_NODE(_type)   \
    case _type::value:  pNextUpdater = TAllocNew(_type::value_type);   break;

        SSQ_UPDATE_CTRL_LIST();
    }

    if(!FlowControl::FCAssert(pNextUpdater))
    {   // 이후 대책을 강구할 것.
        return 0;
    }

    // Shared Logic
    pNextUpdater->Migration(pNextUpdater, pCurUpdater);
    pNextUpdater->Init();
    pNextUpdater->RegisterUpdateCtrl(pNextUpdater);

    switch(curType)
    {
#undef SSQ_UPDATE_CTRL_NODE
#define SSQ_UPDATE_CTRL_NODE(_type)   \
    case _type::value:  TAllocDelete(_type::value_type, pCurUpdater); break;

        SSQ_UPDATE_CTRL_LIST();
    }

    //pImpl->_RegisterUpdateCtrl(pCurUpdater->m_ID, pTarget);
    return pNextUpdater;
}


//--------------------------------------------------------------------------------------------------


BOOL
GameSSQUpdateCtrl::UpdateMsg(const curtime_info& tmInfo)
{
    if(m_pMsgParser->GetUpdateCtrl())
        m_pMsgParser->Update(m_CodeOfSSQ);
    return TRUE;
}


//--------------------------------------------------------------------------------------------------


VOID
GameSSQUpdateCtrl::RecvedMsgProcess(const WAR_MSG_HDR* IN pStream)
{
    if(m_pMsgParser->GetUpdateCtrl())
        m_pMsgParser->OnRecv(pStream);
}


//--------------------------------------------------------------------------------------------------


VOID
GameSSQUpdateCtrl::ConstVarsInit(GameSSQUpdateCtrl* const pUpdateCtrl)
{
    const SSQInfo* const pSSQInfo = pUpdateCtrl->m_pSSQInfo;

    pUpdateCtrl->m_ConstVars.pCheckFilters = TAllocNew(MatchCheckCodeFilters);
    MatchCheckCodeFilters& rFilters = *pUpdateCtrl->m_ConstVars.pCheckFilters;

    new (&rFilters.SkillGroupFilter)
        MatchCheckCodeFilterArray(pSSQInfo->m_RestrictSkillGroupInfo.Count,
                                  pSSQInfo->m_RestrictSkillGroupInfo.SkillGroupCodes);

    new (&rFilters.ItemTypeFilter)
        MatchCheckCodeFilterArray(pSSQInfo->m_RestrictItemTypeInfo.Count,
                                  pSSQInfo->m_RestrictItemTypeInfo.ItemTypes);
}


//--------------------------------------------------------------------------------------------------


VOID
GameSSQUpdateCtrl::ConstVarsRelease(GameSSQUpdateCtrl* const pUpdateCtrl)
{
    if(pUpdateCtrl->m_ConstVars.pCheckFilters)
    {
        TAllocDelete(MatchCheckCodeFilters, pUpdateCtrl->m_ConstVars.pCheckFilters);
        pUpdateCtrl->m_ConstVars.pCheckFilters = 0;
    }
}


//==================================================================================================
//


VOID
GameSSQUpdateCtrl_None::Init()
{
    m_CurType = value;
    m_Timer.SetTimer(interval);

    ZeroMemory(&m_SharedVars, sizeof(m_SharedVars));
    m_SharedVars.UpdaterState = SharedVariables::eBattle_NotStarted;
}


//--------------------------------------------------------------------------------------------------


BOOL
GameSSQUpdateCtrl_None::Update(const curtime_info& tmInfo)
{
    if(m_Timer.IsExpired())
    {
        //WAVERIX_DEBUGGING_OUTPUTnV(m_CodeOfSSQ);
    }
    return TRUE;
}


//==================================================================================================
//

GameSSQReport::GameSSQReport()
: m_pUpdater(0)
//, m_LastUpdatedChrNum(0)
{
}


VOID
GameSSQReport::Init(GameSSQUpdateCtrl* const pUpdater)
{
    m_pUpdater = pUpdater;
    m_ReportTimer.SetTimer(UPDATE_INTERVAL);
}


VOID
GameSSQReport::Update(const curtime_info& tmInfo, const BOOLEAN bForced)
{
    if(bForced || m_ReportTimer.IsExpired())
    {   //  일단 5초마다 보내는 걸로 하자.
        BROADCASTER_MSG_AG_ZONE_SSQ_ROOM_CTRL_CMD& rMsgCMD = m_pUpdater->GetMsgRouter()->GetCMD_AG();
        WAR_MSG_HDR* const pHeader = &rMsgCMD.MSG_HEADER;

        {
            const SSQInfo::COUNT_T remained_num = m_pUpdater->SharedVars.RemainedTotal;
            MSGSUB_SSQ_PROTOCOL_INFORMATION_COUNT_OF_PLAYERS msg;
            msg.Remains = remained_num;
            msg.SerializeTo(pHeader);
        };

        //const eTIME_TYPE updater_type = m_pUpdater->GetTimeType();
        //if(updater_type == TT_BATTLE)
        //{
        //  const SSQInfo::MINUTE_T remained_playing_time = m_pUpdater->SharedVars.RemainedTime;
        //  MSGSUB_SSQ_PROTOCOL_INFORMATION_REMAINED_TIME msg;
        //  msg.Remains = remained_playing_time;
        //  msg.SerializeTo(pHeader);
        //}
    }
}


//==================================================================================================
//

VOID
GameSSQUpdateCtrl_Created::Init()
{
    m_CurType = value;
    m_Timer.SetTimer(interval);
    m_Reporter.Init(this);

    typedef SSQTraps ST;
    m_Traps.Init(this, ST::eTrapFlag(ST::TRAP_ENABLED | ST::TRAP_ENABLED_THUNDERBOLT_SHOW));
}

BOOL
GameSSQUpdateCtrl_Created::Update(const curtime_info& tmInfo)
{
    if(m_Timer.IsExpired())
    {
        WAVERIX_DEBUGGING_OUTPUTnV(m_CodeOfSSQ);
        m_Reporter.Update(tmInfo);
        m_Traps.Update(tmInfo);
    }
    return TRUE;
}


//==================================================================================================
//

SSQTraps::SSQTraps()
    : pUpdateCtrl(0)
{
    TrapUpdateTimer.Stop();
}

VOID
SSQTraps::Init(GameSSQUpdateCtrl* const pUpdater, const eTrapFlag control_flags)
{
    pUpdateCtrl = pUpdater;

    TrapUpdateTimer.SetTimer(UPDATE_INTERVAL);

    SSQInfoParser* const pSSQInfoParser = SSQInfoParser::Instance();
    const SSQInfo* const pSSQInfo = pUpdateCtrl->GetSSQInfo();

    {
        sTIME_INFO timeInfo;
        sTIME_INFO::SetTimeValue(timeInfo);
        StartAcmTime = timeInfo.AcmTime;
    };

    {   //  SSQDistributionChart Initialize
        TrapFlag = TRAP_NONE;

        GameInstanceDungeon* const pSSQField = pUpdateCtrl->GetSSQField();
        GameField* const pField = DamageExecuter.SetAndGetSSQField(pSSQField);
        pGameField              = pField;
        const SectorGrid& rGrid = pField->SectorGridRef;
        DamageExecuter.SetProjectionGrid(&rGrid);
    };

    {
        TrapFlag |= TRAP_ENABLED;
    };

    if(control_flags & TRAP_ENABLED_EARTHQUAKE)
    {   //  Earthquake Pattern Initialize
        typedef SSQInfoParser::SSQPATTERN_TABLE     SSQPATTERN_TABLE;
        const SSQPATTERN_TABLE& rPatterns = pSSQInfoParser->GetRefSSQPattern();

        const DWORD patterns_upperbound = (DWORD)rPatterns.size();
        const DWORD selected_pattern =
            RandomNumberGenerator::GetRandomNumberRange(0, patterns_upperbound);

        SSQPATTERN_TABLE::const_iterator where = rPatterns.begin();
        advance(where, selected_pattern);
        SSQPattern* const pPattern = where->second;

        EarthquakeInfo.pPattern          = pPattern;
        EarthquakeInfo.Mode              = EARTHQUAKE_NONE;
        EarthquakeInfo.PatternIndex      = pPattern->m_Index;
        EarthquakeInfo.PatternArrayIndex = 0;
        EarthquakeInfo.LastUpdateCounter = 0;

        EarthquakeInfo.UpdateTimer.SetTimer(UPDATE_EARTHQUAKE_CHECK_INTERVAL);
        TrapFlag |= TRAP_ENABLED_EARTHQUAKE;
    };

    if(control_flags & (TRAP_ENABLED_THUNDERBOLT|TRAP_ENABLED_THUNDERBOLT_SHOW))
    {   //  Thunderbolt Initialize
        ThunderboltInfo.ThunderboltData = pSSQInfo->m_ThunderboltInfo;
        const DWORD interval = ThunderboltInfo.ThunderboltData.m_RaisePerXsec * second;
        SetVector(&ThunderboltInfo.SearchRangeWeight,
                  SSQThunderbolt_DamageRadius, SSQThunderbolt_DamageRadius, 0.f);

        ThunderboltInfo.UpdateTimer.SetTimer(interval);
        TrapFlag |= TRAP_ENABLED_THUNDERBOLT;
    };

    if(control_flags & TRAP_ENABLED_THUNDERBOLT_SHOW)
        TrapFlag |= TRAP_ENABLED_THUNDERBOLT_SHOW;

    if(control_flags & TRAP_ENABLED_CLOSEDSPACE)
    {   //  ClosedSpace
        ClosedSpaceInfo.ClosedSpaceData = pSSQInfo->m_ClosedSpaceInfo;
        const DWORD interval = ClosedSpaceInfo.ClosedSpaceData.m_RaisePerXsec * second;

        ClosedSpaceInfo.UpdateTimer.SetTimer(interval);
        TrapFlag |= TRAP_ENABLED_CLOSEDSPACE;
    };
}


//--------------------------------------------------------------------------------------------------

#ifdef __WAVERIX_TEST_LOGIC_TASK_MACRO__
extern BOOLEAN Variation_SSQ_Trap_Art_R1(const GameSSQUpdateCtrl* const pUpdateCtrl);
#endif

VOID
SSQTraps::Update(const curtime_info& tmInfo)
{
    if(!TrapUpdateTimer.IsExpired())
        return;

    const SSQInfo* const pSSQInfo = pUpdateCtrl->GetSSQInfo();
    BROADCASTER_MSG_AG_ZONE_SSQ_ROOM_CTRL_CMD& rMsgCMD = pUpdateCtrl->GetMsgRouter()->GetCMD_AG();
    WAR_MSG_HDR* const pHeader = &rMsgCMD.MSG_HEADER;

    if((TrapFlag & TRAP_ENABLED_EARTHQUAKE) && EarthquakeInfo.UpdateTimer.IsExpired())
    {
        switch(EarthquakeInfo.Mode)
        {
        case EARTHQUAKE_NONE:
            if(EarthquakeInfo.LastUpdateCounter == EarthquakeInfo.PatternArrayIndex &&
               EarthquakeInfo.LastUpdateCounter <  EarthquakeInfo.pPattern->m_BanLiftStep)
            {
                GameSSQControlInfo::AcmTmFmt elapsedTm = tmInfo.AcmTime - StartAcmTime;
                // ASSERT(tmInfo.AcmTime>=StartAcmTime);
                const SSQInfo::COUNT_T characters_number = pUpdateCtrl->SharedVars.RemainedTotal;
                SSQInfo::EARTHQUAKE_NODE node = { (SSQInfo::MINUTE_T)elapsedTm, characters_number };

                const BOOLEAN bOkEarthquakeCond =
                    pSSQInfo->m_EarthquakeInfo[EarthquakeInfo.PatternArrayIndex].OnSatisfiedCondition(node);
                if(bOkEarthquakeCond)
                {
                    EarthquakeInfo.Mode = EARTHQUAKE_INDICATION;
                    EarthquakeInfo.UpdateTimer.SetTimer(UPDATE_EARTHQUAKE_CHANGE_TO_INDICATION);
                }
            }
            break;
        case EARTHQUAKE_INDICATION:
            {
                {
                    MSGSUB_SSQ_PROTOCOL_TRAP_EARTHQUAKE_INDICATION msg;
                    msg.PatternIndex    = EarthquakeInfo.PatternIndex;
                    msg.StepArrayIndex  = EarthquakeInfo.PatternArrayIndex;
                    msg.SerializeTo(pHeader);
                };
                EarthquakeInfo.Mode = EARTHQUAKE_EXECUTE;
                EarthquakeInfo.UpdateTimer.SetTimer(UPDATE_EARTHQUAKE_INDICATION_INTERVAL);

                DamageExecuter.Earthquake_Indication(EarthquakeInfo.pPattern,
                                                     EarthquakeInfo.PatternArrayIndex);
                if((EarthquakeInfo.LastUpdateCounter + 1) == EarthquakeInfo.pPattern->m_BanLiftStep)
                    DamageExecuter.ClosedSpace_RemoveDanger(pSSQInfo);
            }
            break;
        case EARTHQUAKE_EXECUTE:
            {
                SSQMsgRouterInGame* const pMsgRouter = pUpdateCtrl->GetMsgRouter();
                {
                    const SSQCODE CodeOfSSQ = pUpdateCtrl->GetCodeOfSSQ();
                    pMsgRouter->Update(CodeOfSSQ);

                    MSGSUB_SSQ_PROTOCOL_TRAP_EARTHQUAKE_EXECUTE msg;
                    msg.SerializeTo(pHeader);

                    MSGSUB_SSQ_PROTOCOL_CONTROL_FORCED_POSITION_CORRECTION msgFPC;
                    DamageExecuter.Earthquake(EarthquakeInfo.pPattern,
                                              EarthquakeInfo.PatternArrayIndex, msgFPC);
                    msgFPC.SerializeTo(pHeader);

                    pMsgRouter->Update(CodeOfSSQ);
                };

                ++EarthquakeInfo.PatternArrayIndex;
                ++EarthquakeInfo.LastUpdateCounter;
                EarthquakeInfo.Mode = EARTHQUAKE_NONE;
                EarthquakeInfo.UpdateTimer.SetTimer(UPDATE_EARTHQUAKE_CHECK_INTERVAL);

                if(EarthquakeInfo.LastUpdateCounter == EarthquakeInfo.pPattern->m_BanLiftStep)
                    TrapFlag = TrapFlag & (~TRAP_ENABLED_EARTHQUAKE);
            }
            break;
        }
    }

    if((TrapFlag & (TRAP_ENABLED_THUNDERBOLT|TRAP_ENABLED_THUNDERBOLT_SHOW)) &&
       ThunderboltInfo.UpdateTimer.IsExpired())
    {
        {
            MSGSUB_SSQ_PROTOCOL_TRAP_THUNDERBOLT_EXECUTE msgTTE;
            MSGSUB_SSQ_PROTOCOL_CONTROL_DAMAGE_PROCESS msgCDP;

            const BOOLEAN bShowmanship = !!(TrapFlag&TRAP_ENABLED_THUNDERBOLT_SHOW);
            const BOOLEAN bProcOutRangeClosedSpace = bShowmanship
                                                   ?   TRUE
                                                   :   !!(TrapFlag & TRAP_ENABLED_EARTHQUAKE);

            DamageExecuter.Thunderbolt(pSSQInfo, bShowmanship,
                                       &ThunderboltInfo.SearchRangeWeight,
                                       bProcOutRangeClosedSpace,
                                       bProcOutRangeClosedSpace ? msgTTE.MAX_SIZE
                                                                : (msgTTE.MAX_SIZE >> 1),
                                       msgTTE, msgCDP);

            if(msgTTE.Count)  msgTTE.SerializeTo(pHeader);
            if(msgCDP.Count)  msgCDP.SerializeTo(pHeader);
        };
    }

    if((TrapFlag & TRAP_ENABLED_CLOSEDSPACE) && ClosedSpaceInfo.UpdateTimer.IsExpired())
    {
        {
            MSGSUB_SSQ_PROTOCOL_TRAP_CLOSEDSPACE_EXECUTE msgTCE;
            DamageExecuter.ClosedSpace(pSSQInfo, EarthquakeInfo.LastUpdateCounter, msgTCE);

            if(msgTCE.Count)  msgTCE.SerializeTo(pHeader);
        }
        if(!(TrapFlag&TRAP_ENABLED_EARTHQUAKE))
        {
            MSGSUB_SSQ_PROTOCOL_TRAP_CLOSEDSPACE_DISABLED msgTCD;
            msgTCD.SerializeTo(pHeader);

            TrapFlag = TrapFlag & (~TRAP_ENABLED_CLOSEDSPACE);
            //DamageExecuter.ClosedSpace_Remove(pSSQInfo);
        }
    }

#ifdef __WAVERIX_TEST_LOGIC_TASK_MACRO__
    Variation_SSQ_Trap_Art_R1(pUpdateCtrl);
#endif
} //end SSQTraps::Update


//==================================================================================================
//


GameSSQUpdateCtrl_Battle::~GameSSQUpdateCtrl_Battle()
{
}

VOID
GameSSQUpdateCtrl_Battle::Init()
{
    m_CurType = value;
    m_Timer.SetTimer(interval);

    m_Reporter.Init(this);

    typedef SSQTraps ST;
    m_Traps.Init(this, ST::eTrapFlag(ST::TRAP_ENABLED |
                                     ST::TRAP_ENABLED_EARTHQUAKE |
                                     ST::TRAP_ENABLED_THUNDERBOLT |
                                     ST::TRAP_ENABLED_CLOSEDSPACE));

    m_PlayersUpdateTimer.SetTimer(player_event_update_interval);

    m_SharedVars.BattleState    = SharedVariables::eBattle_Init;
    m_SharedVars.UpdaterState   = SharedVariables::eBattle_Init;
    m_SharedVars.RemainedTime   = m_pSSQInfo->m_QuestPlayTime;  // 이건 사실상 Agent에서 담당할 문제...
    GameField* const pGameField = m_pSSQField->GetEntryField();

    typedef GameField   GF;
    const GF::_PLAYER_HASH& rPlayers = pGameField->GetRefPlayerList();
    new (&ItemDistributer) GameWarSSQItemDistribution(m_pSSQInfo, (DWORD)rPlayers.size());
    //typedef GameField GF;
    //const GF::_PLAYER_HASH& rPlayers = pGameField->GetRefPlayerList();
    //m_SharedVars.RemainedTotal    = (SSQInfo::COUNT_T)SSQ_MAX_CHARACTERS; //rCharacters.size();
    //m_SharedVars.RemainedPlayer   = (SSQInfo::COUNT_T)rPlayers.size();

    new (&ObserverControl) GameWarObserverMode(m_pSSQField);
}

BOOL
GameSSQUpdateCtrl_Battle::Update(const curtime_info& tmInfo)
{
    //  (종료조건)
    //  -   전투시간 끝난 경우 (플레이어 복수명일 수 있음) <- Agent에서 담당할 것.
    //  -   전투가 아직 끝나지 않은 상태에서 플레이어 1명 남는 경우
    //  -   전투가 아직 끝나지 않은 상태에서 플레이어 0명 되는 경우

    if(m_Timer.IsExpired())
    {
        //WAVERIX_DEBUGGING_OUTPUTnV(m_CodeOfSSQ);
        switch(m_SharedVars.BattleState)
        {
        case Ret::eBattle_Init:
            _Process_Init(tmInfo);
            break;
        case Ret::eBattle_Progress:
            _Process_Battle(tmInfo);
            break;
        case Ret::eBattle_Timeout:
        case Ret::eBattle_1_Remained:
        case Ret::eBattle_PlayerExtermination:
        case Ret::eBattle_AllDead:
        case Ret::eBattle_EndReserved:
            //  종료 대기인 상태일 확률이 높다.
            break;
        }

        m_Reporter.Update(tmInfo);
    }
    return TRUE;
}


BOOLEAN
GameSSQUpdateCtrl_Battle::RequestObserverModeCtrl(const MSGSUB_SSQ_S2S_OBSERVER_MODE_CTRL& rMsg)
{
    using namespace Observer;

    Player* const pPlayer = PlayerManager::Instance()->FindPlayerByObjKey(rMsg.ObjectKey);
    if(!FlowControl::FCAssert(pPlayer))
        return FALSE;

    //MSGSUB_SSQ_PROTOCOL_REQUEST_OBSERVER_CTRL* const pSub =
    //  const_cast<MSGSUB_SSQ_PROTOCOL_REQUEST_OBSERVER_CTRL*>(&rMsg.C2GMsg);

    const Info::eRequest request = rMsg.Request;
    const Rules::eC2G_ReqRule rules = Rules::GetC2G_RequestRule(request);

    BROADCASTER_MSG_AG_ZONE_SSQ_ROOM_CTRL_CMD& rMsgCMD = m_pMsgParser->GetCMD_AG();
    WAR_MSG_HDR* const pHeader = &rMsgCMD.MSG_HEADER;

    MSGSUB_SSQ_S2S_OBSERVER_MODE_RESULT msg;
    msg.C2GMsg.Init();
    msg.ObjectKey = pPlayer->GetObjectKey();

    //////////////////////////////////////////////////////////////////////////
    //  <LOGIC CONTROL>
    FlowControl flow;
    FLOWCONTROL_START()
    {
        IfFailThenBreak(!(rules & Rules::eC2G_ReqRule_BlockRequest), FALSE);

        if(rules & Rules::eC2G_ReqRule_CondInFieldServer)
        {
            // in FIELD_SERVER
            IfFailThenBreak(g_pGameServer->GetServerType() != FIELD_SERVER, FALSE);

            SetFailFc(!"not supported", FALSE);
            break;
        }


        //////////////////////////////////////////////////////////////////////////
        //  In SSQ Field

        StatusManager* const pStatusManager = pPlayer->GetStatusManager();
        StatusField& rStatus = pStatusManager->Status;
        IfFailThenBreak(rules & Rules::eC2G_ReqRule_CondInSSQField, FALSE);

        const BOOLEAN isObserver = !!(rStatus.FIELD & rStatus.ENABLE_OBSERVER_MODE);
        SSQCharacterStatusInGame& rSSQStatus = pPlayer->SSQHandler->ControlStatus;
        const BOOLEAN isDead = !!(rSSQStatus.FIELD & rSSQStatus.CHECKED_DEAD);

        if(rules & Rules::eC2G_ReqRule_CondObserverModeOn)
            IfFailThenBreak(isObserver, FALSE);
        if(rules & Rules::eC2G_ReqRule_CondObserverModeOff)
            IfFailThenBreak(!isObserver, FALSE);
        if(rules & Rules::eC2G_ReqRule_CondDeadState)
            IfFailThenBreak(isDead, FALSE);

        switch(request)
        {
        case Info::eRequest_Observer        :       // not Observer
            IfFailThenBreak(ObserverControl.Enter(rules, pPlayer), FALSE);
            break;
        case Info::eRequest_Leave           :       // always
            {
                MSG_AG_ZONE_MISSION_LEAVE_CMD msgCMD;
                msgCMD.m_LeaveType = msgCMD.eLeaveType_Default;
                msgCMD.m_ReturnMapCode = 0;
                pPlayer->SendPacket(&msgCMD, sizeof(msgCMD));
            }
            break;
        case Info::eRequest_ChangeCoord     :       // in ObserverMode
            IfFailThenBreak(
                ObserverControl.ChangeCoordinates(rules, pPlayer, rMsg.ObserverCmd.PressedKey), FALSE);
            break;
        default:
            SetFailFc(!"undefined", FALSE);
            break;
        }

        if(flow.IsErrorOccured())
            break;

        MSGSUB_SSQ_PROTOCOL_REQUEST_OBSERVER_CTRL_RESULT& rCGMsg = msg.C2GMsg;
        rCGMsg.Result = MSGSUB_SSQ_PROTOCOL_REQUEST_OBSERVER_CTRL_RESULT::eNtfCmd_ResultByRequest_Success;
        ISSQHandlerInGame& rHandler = pPlayer->SSQHandler;
        rCGMsg.Coordinates = rHandler->GetObserverCoordinateIndex();
        msg.SerializeTo(pHeader);

        return TRUE;
    }
    FLOWCONTROL_END;

    MSGSUB_SSQ_PROTOCOL_REQUEST_OBSERVER_CTRL_RESULT& rCGMsg = msg.C2GMsg;
    rCGMsg.Result = MSGSUB_SSQ_PROTOCOL_REQUEST_OBSERVER_CTRL_RESULT::eNtfCmd_ResultByRequest_Fail;
    rCGMsg.Coordinates = Observer::Info::eCoord_Slots;
    msg.SerializeTo(pHeader);

    return FALSE;
}

SharedVariables::eBattleState
GameSSQUpdateCtrl_Battle::_CheckBattleEnd(const curtime_info& tmInfo)
{
    // 플레이어, 몬스터 남은 인원처리는 GameField리스트에서 참조할 수 없다. 소환수 문제도 있고, 플레이어, 죽은 상태 검출도 필요하다.
    if(0 == m_SharedVars.RemainedTotal)
        return Ret::eBattle_AllDead;            // 종료 조건
    if(1 == m_SharedVars.RemainedTotal)
        return Ret::eBattle_1_Remained;         // 종료 조건
    if(0 == m_SharedVars.RemainedPlayer)      // 플레이어만 전멸하는 상황도 있을 것이다.
        return Ret::eBattle_PlayerExtermination;

    //  시간은 고려대상이 아니다. Agent에서 관리해줘야 한다.
    return Ret::eBattle_Progress;
}


VOID
GameSSQUpdateCtrl_Battle::_Process_Init(const curtime_info& tmInfo)
{
    m_SharedVars.BattleState = Ret::eBattle_Progress;
}


SharedVariables::eBattleState
GameSSQUpdateCtrl_Battle::_Process_Battle(const curtime_info& tmInfo)
{
    const Ret::eBattleState checked_state = _CheckBattleEnd(tmInfo);  // 이 상태에 따라 종료 처리도 수행해야 한다.
    if(checked_state != Ret::eBattle_Progress)
    {
        if(m_SharedVars.bTransaction)
            return Ret::eBattle_EndReserved;

        _Process_EndReserve(tmInfo, checked_state);
    }

    if(Ret::eBattle_Progress == m_SharedVars.BattleState)
    {
        m_Traps.Update(tmInfo);
        _Process_Players(tmInfo);
    }

    return m_SharedVars.BattleState;
}


BOOLEAN
GameSSQUpdateCtrl_Battle::_Process_EndReserve(const curtime_info& tmInfo,
                                              const Ret::eBattleState battle_state)
{
    BROADCASTER_MSG_AG_ZONE_SSQ_ROOM_CTRL_CMD& rMsgCMD = m_pMsgParser->GetCMD_AG();
    WAR_MSG_HDR* const pHeader = &rMsgCMD.MSG_HEADER;
    {
        MSGSUB_SSQ_S2S_BATTLE_END_REQUEST msg;
        msg.CodeOfSSQ = m_CodeOfSSQ;
        msg.SerializeTo(pHeader);
        m_SharedVars.bTransaction = TRUE;
    };
    {   // Last Update
        m_Reporter.Update(tmInfo, TRUE);
    };

    m_SharedVars.BattleState = battle_state;

    return TRUE;
}


VOID
GameSSQUpdateCtrl_Battle::_Process_Players(const curtime_info& tmInfo)
{
    if(!m_PlayersUpdateTimer.IsExpired())
        return;

    typedef GameField           GF;
    typedef GF::_PLAYER_HASH    _PLAYER_HASH;

    BROADCASTER_MSG_AG_ZONE_SSQ_ROOM_CTRL_CMD& rMsgCMD = m_pMsgParser->GetCMD_AG();
    WAR_MSG_HDR* const pHeader = &rMsgCMD.MSG_HEADER;

    GF* const pGameField = m_pSSQField->GetEntryField();
    const _PLAYER_HASH& rPlayers = pGameField->GetRefPlayerList();
    FOREACH_CONTAINER(const _PLAYER_HASH::value_type& rPlayer, rPlayers, _PLAYER_HASH)
    {
        Player* const pPlayer = rPlayer.second;
        if (obj_util::IsValid(pPlayer, __FUNCTION__, __LINE__) == false)
        {
            continue;
        }
        ISSQHandlerInGame& rHandler = pPlayer->SSQHandler;
        SSQCharacterStatusInGame& rSSQStatus = rHandler->ControlStatus;
        if(rSSQStatus.FIELD & rSSQStatus.CHECKED_DEAD)
        {
            if(rHandler->QueryEvent_NeedLeaveProcess(pPlayer))  // 한번 처리되면 다시 처리되지 않는다.
            {
                MSGSUB_SSQ_S2S_OBSERVER_MODE_SELECT_WINDOW msg;
                msg.ObjectKey = pPlayer->GetObjectKey();
                msg.SerializeTo(pHeader);
            }
        }
    }
    //m_SharedVars.RemainedTotal    = (SSQInfo::COUNT_T)SSQ_MAX_CHARACTERS; //rCharacters.size();
    //m_SharedVars.RemainedPlayer   = (SSQInfo::COUNT_T)rPlayers.size();
}


//==================================================================================================━━━━━━━━━
//


VOID
GameSSQUpdateCtrl_Clear::Init()
{
    m_CurType = value;
    m_Timer.SetTimer(interval);
    m_ClearStep = STEP_INIT;
}


BOOL
GameSSQUpdateCtrl_Clear::Update(const curtime_info& tmInfo)
{
    if(!m_Timer.IsExpired())
        return TRUE;

    WAVERIX_DEBUGGING_OUTPUTnV(m_CodeOfSSQ);

    switch(m_ClearStep)
    {
    case STEP_INIT:
        m_ClearStep = (eSTEP)(m_ClearStep+1);
    case STEP_WINNER_PROC:
        Winner_Process(tmInfo);
        break;
    case STEP_LEAVE_PROCESS:
        Leave_Process(tmInfo);
        break;
    case STEP_WAIT:
        m_Timer.SetTimer(interval_for_leave_wait);
        m_ClearStep = (eSTEP)(m_ClearStep+1);
        break;
    case STEP_MAX:
        {
            BROADCASTER_MSG_AG_ZONE_SSQ_ROOM_CTRL_CMD& rMsgCMD = m_pMsgParser->GetCMD_AG();
            WAR_MSG_HDR* const pHeader = &rMsgCMD.MSG_HEADER;

            MSGSUB_SSQ_S2S_BATTLE_CLEARED msg;
            msg.CodeOfSSQ = m_CodeOfSSQ;
            msg.SerializeTo(pHeader);
        }
        break;
    }
    return TRUE;
}


//--------------------------------------------------------------------------------------------------


struct sPointNode
{
    DWORD   point;
    DWORD   exp;    //동점자에 대한 추가 비교 목적
    Player* pPlayer;

    bool operator > (const sPointNode& rRight) const
    {
        if(point > rRight.point)  return true;
        if(rRight.point > point)  return false;
        return (exp > rRight.exp);
    }
};


//--------------------------------------------------------------------------------------------------


VOID
GameSSQUpdateCtrl_Clear::Winner_Process(const curtime_info& tmInfo)
{
    typedef STLX_VECTOR< sPointNode >   RankOrderList;
    DWORD proced_players = 0;
    // (CHANGES) (f100427.1L) change to more cheap routine
    static RankOrderList rankOrders;
    rankOrders.resize(0);

    //  퀘스트 성공한 이들 설정
    typedef GameField::_PLAYER_HASH     _PLAYER_HASH;
    const _PLAYER_HASH& rPlayers = m_pSSQField->GetEntryField()->GetRefPlayerList();
    FOREACH_CONTAINER(const _PLAYER_HASH::value_type& rPlayer, rPlayers, _PLAYER_HASH)
    {
        Player* const pPlayer = rPlayer.second;
        if (obj_util::IsValid(pPlayer, __FUNCTION__, __LINE__) == false)
        {
            continue;
        }

        ISSQHandlerInGame& rRewarder = pPlayer->SSQHandler;
        const eEVENT_MODE_MULTIUSE reward_type = rRewarder->GetSSQEventID();
        // 사실 테스트 목적으로 전투중 중간 개입시 환불 정보가 기재될 것이다. 이 점은 감안하도록 하자.
        if(reward_type == eEVENT_MULTIUSE_SSQ_REFUND)
            continue;

        m_pManager->SettleAccounts(this, pPlayer);

        SSQ_RESERVED_REWARD_INFO info;
        if(!FlowControl::FCAssert(rRewarder->GetRewardInfo(info)))
            continue;

        ++proced_players;

        StatusField& rStatus = pPlayer->GetStatusManager()->Status;
        SSQCharacterStatusInGame& rSSQStatus = rRewarder->ControlStatus;
        const BOOLEAN bLoser = (rStatus.FIELD & rStatus.ENABLE_OBSERVER_MODE) ||
                               (rSSQStatus.FIELD & rSSQStatus.CHECKED_DEAD);

        // 처리 여부는 확인할 것. //m_pManager->UpdateToRankingAndExtra(this, pPlayer);
        if(bLoser)
            continue;

        //  Design Miss
        //  -   SettleAccounts는 실패 정보를 기반으로 계산되게 되어 있다.
        rRewarder->UpdateInfo_ChangeToWinner();

        sPointNode node;
        node.point  = info.m_WinnerReward.Point;
        node.exp    = info.m_WinnerReward.Exp;
        node.pPlayer= pPlayer;
        rankOrders.push_back(node);
    }

    const BOOLEAN isExist = !rankOrders.empty();
    Player* top_winner = 0;
    if(isExist)
    {
        std::sort(rankOrders.begin(), rankOrders.end(), std::greater< sPointNode >());

        SSQHandlerInGame::eOrder order = SSQHandlerInGame::eOrder_1st;
        const DWORD upperbound = (DWORD)rankOrders.size();  // 더 좋은 알고리즘을 생각하라.

        FOREACH_CONTAINER(const RankOrderList::value_type& rOrder, rankOrders, RankOrderList)
        {
            Player* const pPlayer = rOrder.pPlayer;
            if(SSQHandlerInGame::eOrder_1st == order)
                top_winner = pPlayer;
            if(SSQHandlerInGame::eOrder_None != order)
            {
                m_pManager->SettleAccounts(this, pPlayer, order);
                m_pManager->RewardForWinner(this, pPlayer, order);
                order = (SSQHandlerInGame::eOrder)(order+1);
            }
            m_pManager->UpdateToRankingAndExtra(this, pPlayer);

#ifdef _NA_001990_ACSYSTEM_ADD
            pPlayer->GetACManager()->IncConditionValue(ACHIEVEMENT_OBJECT_WIN, pPlayer->GetGameZonePtr()->GetMapCode());
#endif //_NA_001990_ACSYSTEM_ADD
        }
    }

    m_ClearStep = (eSTEP)(m_ClearStep+1);
    m_Timer.SetTimer(interval_for_leave);

    BROADCASTER_MSG_AG_ZONE_SSQ_ROOM_CTRL_CMD& rMsgCMD = m_pMsgParser->GetCMD_AG();
    WAR_MSG_HDR* const pHeader = &rMsgCMD.MSG_HEADER;
    if(isExist && top_winner)
    {
        MSGSUB_SSQ_PROTOCOL_INFORMATION_QUEST_SUCCESS_RESULT msg;
        msg.WinnerName.CopyFrom(top_winner->GetCharName());
#ifdef _NA_002950_20110627_SSQ_NOTICE
        const GameGuild* const guild = g_GameGuildManager.FindGuild(top_winner->GetGuildGuid());
        if (guild != NULL)
        {
            typedef MSGSUB_SSQ_PROTOCOL_INFORMATION_QUEST_SUCCESS_RESULT::GuildNameBuffer GuildNameBuffer;
            BOOST_STATIC_ASSERT(GuildNameBuffer::SZ_STRING_LENGTH_WITH_NULL >= 
                _countof(top_winner->GetCharInfo()->m_tszGuildName));
            msg.guild_name.CopyFrom(top_winner->GetCharInfo()->m_tszGuildName);
        }
#endif
        msg.SerializeTo(pHeader);
    }
    else
    {
        MSGSUB_SSQ_PROTOCOL_INFORMATION_QUEST_FAIL_RESULT msg;
        msg.bIsNotStarted = m_SharedVars.UpdaterState == SharedVariables::eBattle_NotStarted;
        msg.SerializeTo(pHeader);
    };
}


//--------------------------------------------------------------------------------------------------


VOID
GameSSQUpdateCtrl_Clear::Leave_Process(const curtime_info& tmInfo)
{
    MSG_AG_ZONE_MISSION_LEAVE_CMD msgCMD;
    msgCMD.m_LeaveType = msgCMD.eLeaveType_Default;
    msgCMD.m_ReturnMapCode = 0;

    typedef GameField::_PLAYER_HASH     _PLAYER_HASH;
    const _PLAYER_HASH& rPlayers = m_pSSQField->GetEntryField()->GetRefPlayerList();
    FOREACH_CONTAINER(const _PLAYER_HASH::value_type& rPlayer, rPlayers, _PLAYER_HASH)
    {
        Player* const pPlayer = rPlayer.second;
        if (obj_util::IsValid(pPlayer, __FUNCTION__, __LINE__) == false)
        {
            continue;
        }
        pPlayer->SendPacket(&msgCMD, sizeof(msgCMD));
    }
    m_ClearStep = (eSTEP)(m_ClearStep+1);
}


