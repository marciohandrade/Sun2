#include "stdafx.h"
#include <PacketStruct_AG.h>
#include <PacketStruct_AW.h>

#include <UserSessions/User.h>
#include <UserSessions/UserManager.h>

#include <ServerSessions/ServerSessionManager.h>
#include <ServerSessions/BattleServerSession.h>
#include <ServerSessions/FieldServerSession.h>

#include <Zone/DungeonSSQ.h>
#include <Zone/ZoneManager.h>

#include <ssq/SSQInfoParser.h>

//==================================================================================================
//  for debugging control
#define __WAVERIX_LOG_FORMAT_0S(msg)    \
{    SUNLOG(eCRITICAL_LOG, "[Func:%s][Line:%3u] %s", __FUNCTION__, __LINE__, msg);    }

#define __WAVERIX_LOG_FORMAT_0S1N(msg, value)    \
{    SUNLOG(eCRITICAL_LOG, "[Func:%s][Line:%3u] %s (%u)", __FUNCTION__, __LINE__, msg, value);    }


#ifdef _DEBUG
#define WAVERIX_DEBUG_LOG_0S(msg)           __WAVERIX_LOG_FORMAT_0S(msg)
#define WAVERIX_DEBUG_LOG_0S1N(msg, value)  __WAVERIX_LOG_FORMAT_0S1N(msg, value)
#else
#define WAVERIX_DEBUG_LOG_0S(msg)
#define WAVERIX_DEBUG_LOG_0S1N(msg, value)
#endif

//
//==================================================================================================
//
//
#include <Zone/WarControlManager.h>
#include "ISSQHandler.h"
#include "SSQControlInfos.hxx"
#include "SSQUpdateCtrl.hxx"
#include "SSQPacketProcess.hxx"
#include "SSQControlManagerImpl.hxx"

//==================================================================================================
//  <SSQUpdateCtrl_xxx><Implementation>
//

VOID
SSQUpdateCtrl::MigrationDefault(const SSQCODE CodeOfSSQ,
                                SSQControlManagerImpl* const pMgrImpl,
                                SSQMsgRouterInAgent* const pMsgParser,
                                SSQInfo* const pSSQInfo)
{
    m_CodeOfSSQ = CodeOfSSQ;
    m_CurType = SSQControlInfo::TT_MAX;
    m_Timer.Stop();
    m_pManager = pMgrImpl;
    m_pMsgRouter= pMsgParser;
    m_pSSQInfo = pSSQInfo;
    m_pSSQField = 0;
    m_ControlField = RCS_NONE;
    m_ProcedIndex = 0;
    m_IsFastTrack = 0;
    if(m_pMsgRouter)
        m_pMsgRouter->SetUpdateCtrl(this);
};

VOID
SSQUpdateCtrl::RegisterUpdateCtrl(SSQUpdateCtrl* const pTarget)
{
    SSQControlManagerImpl* const pImpl = pTarget->m_pManager;
    pImpl->_RegisterUpdateCtrl(pTarget->m_CodeOfSSQ, pTarget);
}

BOOL
SSQUpdateCtrl::UpdateMsg(const curtime_info& tmInfo)
{
    __UNUSED(tmInfo);
    if(m_pMsgRouter->GetUpdateCtrl())
        m_pMsgRouter->Update(m_CodeOfSSQ);
    return TRUE;
}

VOID
SSQUpdateCtrl::RecvedMsgProcess(const WAR_MSG_HDR* IN pStream)
{
    if(m_pMsgRouter->GetUpdateCtrl())
        m_pMsgRouter->OnRecv(pStream);
}



VOID
SSQUpdateCtrl::RecvedMsgProcessSpecial(const WAR_MSG_HDR* IN pStream)
{
    if(m_pMsgRouter->GetUpdateCtrl())
        m_pMsgRouter->OnRecvSpecial(pStream);
}

VOID
SSQUpdateCtrl::UserEnter(User* const pUser)
{
    //  특수 조건 설정할 것... enter enabled state
    const DWORD playerKey = pUser->GetPlayerKey();
    //
    ISSQHandler* const pHandler = m_pMsgRouter->FindSlot(playerKey);
    //_NA_000000_20151109_BUG_SSQ_HANDLER_NULL
    if (pHandler != NULL)
    {
        // ASSERT (pHandler) ... ?
        pHandler->ChangeStatus(ISSQHandler::State<ISSQHandler::USR_ENTERED_SSQ_FIELD>());
        //WAVERIX_SSQ_PORTAL_TRANSACTION_MODIFY
        //{
        //  pUser->SSQHandler = ISSQHandlerD(pUser, pHandler);    // Link
        //  m_pMsgRouter->RemoveSlot(playerKey);
        //}
    }
    else
    {
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"][CriticalBug] Cannot Find ISSQHandler [PlayerKey:%d]", 
               playerKey);
    }
}

VOID
SSQUpdateCtrl::UserLeave(User* const pUser)
{
    // UserLeave시 Zone::users->Slot(insert) 처리가 이뤄지도록 하자.??
    //WAVERIX_SSQ_PORTAL_TRANSACTION_MODIFY
    const DWORD playerKey = pUser->GetPlayerKey();
    ISSQHandler* const pHandler = m_pMsgRouter->FindSlot(playerKey);
    pHandler->ChangeStatus(ISSQHandler::State<ISSQHandler::USR_LEAVED_SSQ_FIELD>());
    //m_pMsgRouter->SlotClear(pUser);
}

//==================================================================================================
//  <SSQUpdateCtrl><None State>
//
void
SSQUpdateCtrl_None::Init(const curtime_info& tmInfo)
{
    __UNUSED(tmInfo);
    m_CurType = value;
    m_Timer.SetTimer(interval);

    m_ControlField = RCS_NONE;
    m_IsFastTrack = 0;
}

BOOL
SSQUpdateCtrl_None::Update(const curtime_info& tmInfo)
{
    if(m_Timer.IsExpired())
    {
        DWORD findedIndex = 0;
        if(m_pSSQInfo->IsNoticeTime(&tmInfo.SysTime, &findedIndex))
        {
            __WAVERIX_LOG_FORMAT_0S1N("Notice Time!", m_CodeOfSSQ);
            m_ProcedIndex = (BYTE)findedIndex;    // 현재 활성화된 탭
            SSQUpdateCtrl_Notice* const pCtrl = TAllocNew(SSQUpdateCtrl_Notice_Info::value_type);
            pCtrl->Migration(pCtrl, this);
            pCtrl->Init(tmInfo);
            pCtrl->RegisterUpdateCtrl(pCtrl);

            m_pManager->SSQSingletonNoticeCtrl.EnterSingletonNotice(m_pSSQInfo, findedIndex);
            TAllocDelete(updater_info::value_type, this);
            return TRUE;
        }
    }
    return TRUE;
}

//==================================================================================================
//  <SSQUpdateCtrl><Notice State>
//
VOID
SSQUpdateCtrl_Notice::Init(const curtime_info& tmInfo)
{
    m_CurType = value;
    m_Timer.SetTimer(interval);

    // 초기화 되는 시점 broadcast 처리 시간
    m_StartTick = m_NextTimeout = GetTickCount();
    SSQInfo::TimeRange range = m_pSSQInfo->m_CTimeNotice[m_ProcedIndex];
    _ConvertWzTmFmt2AcmTime(range);

    // Init시키는 주체가 SSQUpdateCtrl_None이고, 공지시간으로 평가하므로 항상 0<=x
    ASSERT(tmInfo.AcmTime >= range.Start);
    m_NextAcmTime = tmInfo.AcmTime; // 처음에는 바로 enter
    const AcmTmFmt tmFmt = tmInfo.AcmTime - range.Start; // tmFmt = [0, 5)
    m_StartCountdown = (BYTE)(broadcast_countdown_init-tmFmt);
    //m_StartCountdown = (BYTE)(tmFmt); // (BYTE)(broadcast_countdown_init-tmFmt);
    m_Countdown = m_StartCountdown;
}

BOOL
SSQUpdateCtrl_Notice::Update(const curtime_info& tmInfo)
{
    if(!m_Timer.IsExpired())
        return TRUE;

    if(tmInfo.AcmTime >= m_NextAcmTime)
    {
        __WAVERIX_LOG_FORMAT_0S1N("Post 1 minute", m_CodeOfSSQ);

        if(m_IsFastTrack)
            m_Countdown = 0;

        // 5회 전송 이후 대기시간으로 전환 - (WARNING) SSQ Field생성 여부 확인할 것
        if(broadcast_changeto_waiting_countdown_value /*0*/ == m_Countdown)
        {
            Process_CreateSSQField();    // 연관 이벤트를 통해 성공시 m_pSSQField이 설정된다.

            SSQUpdateCtrl* pCtrl = NULL;
            if(!m_pSSQField)
            {    // 더 이상 어찌할 수 없는 상황이다. 이번 SSQ Dungeon open은 스킵하기로 한다.
                __WAVERIX_LOG_FORMAT_0S1N("Fail change to NextState by BattleServer Problem", m_CodeOfSSQ);
                pCtrl = TAllocNew(SSQUpdateCtrl_None_Info::value_type);
            }
            else
            {    //  Success Processing
                WAVERIX_DEBUG_LOG_0S1N("Change to Waiting", m_CodeOfSSQ);
                pCtrl = TAllocNew(SSQUpdateCtrl_Waiting_Info::value_type);
            }

            pCtrl->Migration(pCtrl, this);
            pCtrl->Init(tmInfo);
            pCtrl->RegisterUpdateCtrl(pCtrl);
            TAllocDelete(updater_info::value_type, this);
            return TRUE;
        }

        //  5,4,3,2,1 분 전일 경우 여기에 도달하게 될 것이다.
        SSQInfo::TimeRange range = m_pSSQInfo->m_CTimeNotice[m_ProcedIndex];
        _ConvertWzTmFmt2AcmTime(range);
        const BYTE remain_countdown = range.End >= tmInfo.AcmTime
                                    ?   BYTE(range.End - tmInfo.AcmTime)
                                    :   0;
        if(FlowControl::FCAssert((remain_countdown <= broadcast_countdown_init) &&
                                 "어떻게 처리해야할 지를 고민하라..."))
        {
            // 조건에 만족하는 다음 next timeout을 구하라.
            BYTE offset = m_StartCountdown - remain_countdown;__TOUCHED(offset);
            //m_NextTimeout = m_StartTick + interval_broadcast;// * (offset));
            m_Countdown = remain_countdown;
            m_NextAcmTime += 1; // 1'
        }
        else
        {
            __WAVERIX_LOG_FORMAT_0S1N("Proc Error", remain_countdown);
            // 에러 처리를 해야할 것이다.
            m_NextTimeout = 0;
            m_Countdown = broadcast_changeto_waiting_countdown_value;
        }

        switch(m_Countdown)
        {
        case broadcast_countdown_init - 0: // (5-0)분 전
        case broadcast_countdown_init - 1: // (5-1)분 전
        case broadcast_countdown_init - 2: // (5-2)분 전
        case broadcast_countdown_init - 3: // (5-3)분 전
        case broadcast_countdown_init - 4: // (5-4)분 전
            {
                // 와중에 BattleServer가 죽었거나 나중에 뜨는 경우를 감안한다.
                Process_CreateSSQField();
                SendNoticeBroadcast(m_Countdown);
                --m_Countdown;
            }
            break;
        }
    }

    return TRUE;
}



VOID
SSQUpdateCtrl_Notice::SendNoticeBroadcast(const SSQInfo::COUNT_T countdown_value)
{
    // Send Broadcasting
    m_pManager->SSQSingletonNoticeCtrl.Broadcast_Notice(this,
                                                        SSQSingletonNotice::eNotice_OpenCountdown,
                                                        countdown_value);
}

VOID
SSQUpdateCtrl_Notice::Process_CreateSSQField()
{
    WAVERIX_DEBUG_LOG_0S1N("Enter", m_CodeOfSSQ);
    const DWORD RESULT_FAIL = 0;
    //////////////////////////////////////////////////////////////////////////
    //  <LOGIC CONTROL>
    FlowControl flow;
    FLOWCONTROL_START()
    {
        if(GetSSQField())
            return;

        //  먼저 찾도록 하자.... CodeOfSSQ로...
        //  -    Key의 유무, Zone IO 등등을 종합적으로 평가해서
        InstanceDungeon* pInstanceDungeon = NULL;
        const RC::eROOM_RESULT rt = g_ZoneManager.CreateStaticDungeon(m_pSSQInfo->m_MapCode,
                                                                      pInstanceDungeon,
                                                                      INDUN_KIND_SS_QUEST,
                                                                      FALSE);
        //  이것이 성공한 뒤에 this->GetSSQField는 자동 설정된다.
        IfFailThenBreak(RC::RC_ROOM_SUCCESS == rt, RESULT_FAIL);

        // 해당 정보에 따라 object정보가 할당되어야 한다.
        WAVERIX_DEBUG_LOG_0S1N("SUCCESS", m_CodeOfSSQ);

        return;
    }
    FLOWCONTROL_END;
    
    //////////////////////////////////////////////////////////////////////////
    //  <ERROR CONTROL>
    __WAVERIX_LOG_FORMAT_0S1N("FAIL", m_CodeOfSSQ);
    return;
}


//==================================================================================================
//  <SSQUpdateCtrl><Waiting State>
//
inline    VOID
SSQUpdateCtrl_Waiting::Clear_WaitingState_Vars()
{
    m_EventType = SWET_UPPERBOUND;
    m_EventArg0 = 0;
    m_EventArg1 = 0;
}

VOID
SSQUpdateCtrl_Waiting::Init(const curtime_info& tmInfo)
{
    m_CurType = value;
    m_Timer.SetTimer(interval);
    m_ControlField = RCS_ENABLE_ENTER | RCS_ENABLE_CREATE_PORTAL;
    // 대기 상태의 시작은 입장 가능

    // < 이후 스크립트에서 제한사항이 변경되었을 경우 여기에 설정하는 방식을 사용해야 할 것이다.

    {    // event: 입장 가능 공지(instance)
        State_Data data = {
            SWET_START_WAITING_NODE,
            0, 2 * minute, //, 0, enterable_limit_enter_notice
            SWET_NOTICE_COUNTDOWN
        };
        m_StateData[SWET_START_WAITING_NODE            ] = data;
    };
    {    // event: 입장 종료 countdown 공지(2x)
        State_Data data = {
            SWET_NOTICE_COUNTDOWN,
            2 * minute, 5 * minute, //, enterable_limit_enter_notice, enterable_limit_countdown_notice
            SWET_ENTERENCE_CLOSED
        };
        m_StateData[SWET_NOTICE_COUNTDOWN            ] = data;
    };
    {    // event: 입장 종료 공지(instance), 입장 요청 블록 처리 <- skip process <-
        State_Data data = {
            SWET_ENTERENCE_CLOSED,
            4 * minute + 5 * second, 5 * minute, //, enterable_limit_countdown_notice, enterable_limit_with_portal_delay_tick
            SWET_FINAL_ENTERENCE_TIMEOUT
        };
        m_StateData[SWET_ENTERENCE_CLOSED            ] = data;
    };
    {    // event: wait for client enterable period
        State_Data data = {
            SWET_FINAL_ENTERENCE_TIMEOUT,
            5 * minute, 6 * minute, //, enterable_limit_with_portal_delay_tick, enter_ack_timeout_limit
            SWET_GAMBLE_PROCESS
        };
        m_StateData[SWET_FINAL_ENTERENCE_TIMEOUT    ] = data;
    };
    {    // event: gamble process
        State_Data data = {
            SWET_GAMBLE_PROCESS,
            6 * minute, 6 * minute + 15 * second, //, enter_ack_timeout_limit, gameble_timeout_limit
            SWET_BATTLE_GRACE_PERIOD
        };
        m_StateData[SWET_GAMBLE_PROCESS                ] = data;
    };
    {    // event: grace period
        State_Data data = {
            SWET_BATTLE_GRACE_PERIOD,
            6 * minute + 15 * second, 6 * minute + 50 * second, //, gameble_timeout_limit, battle_grace_period
            SWET_BATTLE_START_JUST_BEFORE
        };
        m_StateData[SWET_BATTLE_GRACE_PERIOD        ] = data;
    };
    {
        State_Data data = {
            SWET_BATTLE_START_JUST_BEFORE,
            6 * minute + 50 * second, 7 * minute, //, battle_grace_period, battle_countdown_timeout
            SWET_BATTLE_COUNTDOWN
        };
        m_StateData[SWET_BATTLE_START_JUST_BEFORE    ] = data;
    };

    if(m_IsFastTrack)
    {
        m_StateData[SWET_START_WAITING_NODE         ].m_Begin   = 0;
        m_StateData[SWET_START_WAITING_NODE         ].m_End     = 5 * second;
        m_StateData[SWET_NOTICE_COUNTDOWN           ].m_Begin   = 5 * second;
        m_StateData[SWET_NOTICE_COUNTDOWN           ].m_End     = 1 * minute;
        m_StateData[SWET_ENTERENCE_CLOSED           ].m_Begin   = 50 * second;
        m_StateData[SWET_ENTERENCE_CLOSED           ].m_End     = 1 * minute;
        m_StateData[SWET_FINAL_ENTERENCE_TIMEOUT    ].m_Begin   = 1 * minute;
        m_StateData[SWET_FINAL_ENTERENCE_TIMEOUT    ].m_End     = 1 * minute +  1 * second;
        m_StateData[SWET_GAMBLE_PROCESS             ].m_Begin   = 1 * minute +  1 * second;
        m_StateData[SWET_GAMBLE_PROCESS             ].m_End     = 1 * minute + 18 * second;
        m_StateData[SWET_BATTLE_GRACE_PERIOD        ].m_Begin   = 1 * minute + 18 * second;
        m_StateData[SWET_BATTLE_GRACE_PERIOD        ].m_End     = 1 * minute + 20 * second;
        m_StateData[SWET_BATTLE_START_JUST_BEFORE   ].m_Begin   = 1 * minute + 20 * second;
        m_StateData[SWET_BATTLE_START_JUST_BEFORE   ].m_End     = 1 * minute + 25 * second;
    }

    {
        State_Data data = {
            SWET_BATTLE_COUNTDOWN,
            0, 0,
            SWET_UPPERBOUND
        };
        m_StateData[SWET_BATTLE_COUNTDOWN    ] = data;
    };

    //─────────────────────────────────────────
    m_StartTick = GetTickCount();

    m_CCUReport.Init(tmInfo, this);
    //  SWET_START_WAITING_NODE : Initial Values
    Clear_WaitingState_Vars();
    m_EventType = SWET_START_WAITING_NODE;    // first enter state

    //
    ZeroMemory(&m_GambleResult, sizeof(m_GambleResult));
}

WAITING_STATE_PROCNODE(SWET_START_WAITING_NODE, SWET_NOTICE_COUNTDOWN)
{
    const State_Data& rData = m_StateData[m_EventType];

    const DWORD NOT_PROCESSED = 0;
    const DWORD NOTICED = !NOT_PROCESSED;
    // m_EventArg 0:FALSE, 1:TRUE(noticed)
    if(NOTICED == m_EventArg0)    // processed
    {
        const BOOLEAN inSection =
            State_Data::IsAcceptSection(tmInfo, m_StartTick, rData.m_Begin, rData.m_End);
        if(!inSection)
        {    // goto next_state
            Clear_WaitingState_Vars();
            m_EventType = rData.m_Next;
            return;
        }
        return;
    }

    WAVERIX_DEBUG_LOG_0S1N("SWET_START_WAITING_NODE", m_CodeOfSSQ);
    ASSERT(NOT_PROCESSED == m_EventArg0);
    // first process -or- long term skip
    m_EventArg0 = NOTICED;

    m_pManager->SSQSingletonNoticeCtrl.Broadcast_Notice(this, SSQSingletonNotice::eNotice_Open);
}

WAITING_STATE_PROCNODE(SWET_NOTICE_COUNTDOWN, SWET_ENTERENCE_CLOSED)
{
    const State_Data& rData = m_StateData[m_EventType];

    const DWORD NOT_PROCESSED = 0;
    const DWORD NOTICE_0_2 = 1;
    const DWORD NOTICE_2_3 = 2;    // 입장 종료 2분 전
    const DWORD NOTICE_3_4 = 3;    // 입장 종료 1분 전
    const DWORD NOTICE_4__ = 4;    // 입장 종료
    const DWORD GOTO_NEXT  = 5;

    if(NOT_PROCESSED != m_EventArg0)
    {
        const BOOLEAN inSection =
            State_Data::IsAcceptSection(tmInfo, m_StartTick, rData.m_Begin, rData.m_End);
        if(!inSection)
        {    // goto next_state - 뭔가의 이유로 관련 프로세스를 껑충 뛸 경우를 감안하자.
            //  그럴 경우라면 바로 아래의 상태로 넘어가는 편이 나을 지도 모르겠다.
            WAVERIX_DEBUG_LOG_0S1N("Change to NextState - 이후 입장 불가", m_CodeOfSSQ);
            Clear_WaitingState_Vars();
            m_EventType = rData.m_Next;
            return;
        }

        //SSQ_PROTOCOL_NOTICE_ENABLE_ENTER_COUNTDOWN
        if(tmInfo.CurTick >= m_EventArg1)
        {
            switch(const DWORD current_mode = m_EventArg0)
            {
            case NOTICE_0_2:
                {    //  2분전 공지 발송
                    WAVERIX_DEBUG_LOG_0S1N("(입장 종료 공지) 2분 전", m_CodeOfSSQ);
                    m_EventArg0 = NOTICE_2_3;
                    m_EventArg1 = m_StartTick + 3 * minute;
                    if(m_IsFastTrack)
                        m_EventArg1 = m_StartTick + 10*second;
                    m_pManager->SSQSingletonNoticeCtrl.Broadcast_Notice(
                        this, SSQSingletonNotice::eNotice_CloseCountdown, 2);
                }
                break;
            case NOTICE_2_3:
                {    //  1분전 공지 발송
                    WAVERIX_DEBUG_LOG_0S1N("(입장 종료 공지) 1분 전", m_CodeOfSSQ);
                    m_EventArg0 = NOTICE_3_4;
                    m_EventArg1 = m_StartTick + 4 * minute;
                    if(m_IsFastTrack)
                        m_EventArg1 = m_StartTick + 15*second;
                    m_pManager->SSQSingletonNoticeCtrl.Broadcast_Notice(
                        this, SSQSingletonNotice::eNotice_CloseCountdown, 1);
                }
                break;
            case NOTICE_3_4:
                {    //  입장 종료까지의 1분 처리 (50')
                    __WAVERIX_LOG_FORMAT_0S1N("(입장 종료 공지) 포탈 생성 가능시간 끝", m_CodeOfSSQ);
                    //  <- Update : disable create portal
                    m_EventArg0 = NOTICE_4__;
                    m_EventArg1 = m_StartTick + 4 * minute + 50 * second;
                    if(m_IsFastTrack)
                        m_EventArg1 = m_StartTick + 20*second;
                    m_pManager->SSQSingletonNoticeCtrl.Broadcast_Notice(
                        this, SSQSingletonNotice::eNotice_ClosePortal);
                    m_ControlField &= ~RCS_ENABLE_CREATE_PORTAL;
                }
                break;
            case NOTICE_4__:
                {
                    WAVERIX_DEBUG_LOG_0S1N("(입장 종료) 10초 전", m_CodeOfSSQ);
                    m_EventArg0 = GOTO_NEXT;
                    m_EventArg1 = m_StartTick + 4 * minute + 59 * second;
                    if(m_IsFastTrack)
                        m_EventArg1 = m_StartTick + 25*second;
                    return;
                }
                break;
            case GOTO_NEXT:
                {
                    WAVERIX_DEBUG_LOG_0S1N("(입장 종료 공지) 입장 종료", m_CodeOfSSQ);
                    Clear_WaitingState_Vars();
                    m_EventType = rData.m_Next;
                    m_pManager->SSQSingletonNoticeCtrl.Broadcast_Notice(
                        this, SSQSingletonNotice::eNotice_Close);
                    m_ControlField &= ~RCS_ENABLE_ENTER;

                    return; // notice!
                }
                break;
            }
        }
        return;
    }

    __WAVERIX_LOG_FORMAT_0S1N("SWET_NOTICE_COUNTDOWN", m_CodeOfSSQ);
    ASSERT(NOT_PROCESSED == m_EventArg0);
    // first process -or- long term skip
    //  GNM_SSQ_START_COUNTDOWN
    m_EventArg0 = NOTICE_0_2;    // 0~2분 사이
    m_EventArg1 = m_StartTick + 2 * minute;
    if(m_IsFastTrack)
        m_EventArg1 = m_StartTick;
}

WAITING_STATE_PROCNODE(SWET_ENTERENCE_CLOSED, SWET_FINAL_ENTERENCE_TIMEOUT)
{
    const State_Data& rData = m_StateData[m_EventType];

    //  처리 기간 [4'5", 5']
    const DWORD NOT_PROCESSED = 0;
    const DWORD WAIT_FOR_CLIENT_GRACE_PERIOD = !NOT_PROCESSED;

    if(WAIT_FOR_CLIENT_GRACE_PERIOD == m_EventArg0)
    {
        const BOOLEAN inSection =
            State_Data::IsAcceptSection(tmInfo, m_StartTick, rData.m_Begin, rData.m_End);
        if(!inSection)
        {    // goto next_state
            Clear_WaitingState_Vars();
            m_EventType = rData.m_Next;
            return;
        }
        return;
    }

    WAVERIX_DEBUG_LOG_0S1N("SWET_ENTERENCE_CLOSED", m_CodeOfSSQ);
    ASSERT(NOT_PROCESSED == m_EventArg0);
    // first process -or- long term skip
    m_EventArg0 = WAIT_FOR_CLIENT_GRACE_PERIOD;
}

WAITING_STATE_PROCNODE(SWET_FINAL_ENTERENCE_TIMEOUT, SWET_GAMBLE_PROCESS)
{
    const State_Data& rData = m_StateData[m_EventType];

    //  처리 기간 [5', 6']
    const DWORD NOT_PROCESSED = 0;
    const DWORD WAIT_FOR_CLIENT_GRACE_PERIOD = !NOT_PROCESSED;

    if(WAIT_FOR_CLIENT_GRACE_PERIOD == m_EventArg0)
    {
        const BOOLEAN inSection =
            State_Data::IsAcceptSection(tmInfo, m_StartTick, rData.m_Begin, rData.m_End);
        if(!inSection)
        {    // goto next_state
            Clear_WaitingState_Vars();
            m_EventType = rData.m_Next;
            return;
        }
        return;
    }

    WAVERIX_DEBUG_LOG_0S1N("SWET_FINAL_ENTERENCE_TIMEOUT", m_CodeOfSSQ);
    ASSERT(NOT_PROCESSED == m_EventArg0);
    // first process -or- long term skip
    m_EventArg0 = WAIT_FOR_CLIENT_GRACE_PERIOD;
}

WAITING_STATE_PROCNODE(SWET_GAMBLE_PROCESS, SWET_BATTLE_GRACE_PERIOD)
{
    const State_Data& rData = m_StateData[m_EventType];

    //  최초 진입 상태 <- 대기 시간 시작한 지 6분 경과
    //  처리 기간 [6'00", 6'15"]
    const DWORD NOT_PROCESSED = 0;
    const DWORD GAMBLE_PERIOD = 1;
    const DWORD GAMBLE_RESULT = 2;

    SSQInfoParser* const pSSQInfoParser = SSQInfoParser::Instance();
    const SSQInfo* pSSQInfo = pSSQInfoParser->FindSSQInfo(m_CodeOfSSQ);
    if (pSSQInfo == NULL)
    {
        SUNLOG(
            eCRITICAL_LOG,
            __FUNCTION__" : Not found SSQ Info : SSQCode : %d",
            m_CodeOfSSQ
            );
        return;
    }

    if (pSSQInfo->m_Cost == 0)
    {
        //배틀존 리뉴얼 이후 타락한 사원 입장 지불 조건이 사라졌고 스크립트 상
        //지불 금액을 0으로 세팅하게 되면서 겜블 이벤트자체의 유효성이 사라졌다.
        //스크립트 상에서 지불 금액을 0이상 값으로 설정하면 겜블 이벤트는 정상 동작.
        Clear_WaitingState_Vars();
        m_EventType = rData.m_Next;

        return;
    }

    if(NOT_PROCESSED != m_EventArg0)
    {
        const BOOLEAN inSection =
            State_Data::IsAcceptSection(tmInfo, m_StartTick, rData.m_Begin, rData.m_End);
        if(!inSection)
        {    // goto next_state
            Clear_WaitingState_Vars();
            m_EventType = rData.m_Next;
            return;
        }

        if(tmInfo.CurTick >= m_EventArg1)
        {
            BROADCASTER_MSG_CG_ZONE_EX_SSQ_ROOM_CTRL_CMD& rMsgCMD = m_pMsgRouter->GetCMD_CG();
            WAR_MSG_HDR* const pHeader = &rMsgCMD.MSG_HEADER;

            switch(const DWORD current_mode = m_EventArg0)
            {
            case GAMBLE_PERIOD:
                {
                    m_EventArg0 = GAMBLE_RESULT;
                    m_EventArg1 = m_StartTick + 6 * minute + 14 * second;
                    WAVERIX_DEBUG_LOG_0S1N("A->G Gameble StartCmd & Notice G->C", m_CodeOfSSQ);
                    {
                        BROADCASTER_MSG_AG_ZONE_SSQ_ROOM_CTRL_CMD& rMsgCMD = m_pMsgRouter->GetCMD_AG();
                        WAR_MSG_HDR* const pHeader = &rMsgCMD.MSG_HEADER;
                        MSGSUB_SSQ_S2S_GAMBLE_START msg;
                        msg.CodeOfSSQ = m_CodeOfSSQ;
                        msg.SerializeTo(pHeader);
                    };
                    {
                        MSGSUB_SSQ_PROTOCOL_NOTICE_GAMBLE_START msg;
                        msg.SerializeTo(pHeader);
                    };
                }
                break;
            case GAMBLE_RESULT:
                {    //  Gamble결과 확인 여부가 중요하다.
                    __WAVERIX_LOG_FORMAT_0S1N("Notice Gamble Result", m_CodeOfSSQ);
                    Clear_WaitingState_Vars();
                    m_EventType = rData.m_Next;

                    //  (THINKING)
                    //  - Agnet에서? GameServer에서? 지연시켜서 보낼까?, 클라 내장할까?
                    if(FlowControl::FCAssert(SSQ_S2S_GAMBLE_RESULT == m_GambleResult._TAG))
                    {
                        MSGSUB_SSQ_PROTOCOL_NOTICE_GAMBLE_RESULT msg;
                        msg.RewardHeim = m_GambleResult.RewardHeim;
                        msg.CharName = m_GambleResult.CharName;
                        msg.SerializeTo(pHeader);
                        __WAVERIX_LOG_FORMAT_0S((TCHAR*)m_GambleResult.CharName);
                    }

                    return;
                }
                break;
            }
        }
        return;
    }

    WAVERIX_DEBUG_LOG_0S1N("SWET_GAMBLE_PROCESS", m_CodeOfSSQ);
    ASSERT(NOT_PROCESSED == m_EventArg0);

    //  first process -or- long term skip
    //      A->G : start gamble process
    //      A->C : gamble notice
    //      A->C : gamble result notice
    m_EventArg0 = GAMBLE_PERIOD;
    if(m_IsFastTrack == 0)
        m_EventArg1 = m_StartTick + 6 * minute;    // 바로 시작 명령으로 
    else
        m_EventArg1 = m_StartTick;
}

WAITING_STATE_PROCNODE(SWET_BATTLE_GRACE_PERIOD, SWET_BATTLE_START_JUST_BEFORE)
{
    const State_Data& rData = m_StateData[m_EventType];

    //  최초 진입 상태 <- 갬블 종료후 6'15" -> 6'50" 까지...
    //  처리 기간 [6'15", 6'50"]
    const DWORD NOT_PROCESSED = 0;
    const DWORD PREPARE_BATTLE = !NOT_PROCESSED;

    if(PREPARE_BATTLE == m_EventArg0)
    {
        const BOOLEAN inSection =
            State_Data::IsAcceptSection(tmInfo, m_StartTick, rData.m_Begin, rData.m_End);
        if(!inSection)
        {    // goto next_state
            Clear_WaitingState_Vars();
            m_EventType = rData.m_Next;
            return;
        }
        return;
    }

    WAVERIX_DEBUG_LOG_0S1N("SWET_BATTLE_GRACE_PERIOD", m_CodeOfSSQ);
    ASSERT(NOT_PROCESSED == m_EventArg0);
    // first process -or- long term skip
    //WAVERIX_DEBUGGING_OUTPUTn("A<->G 전투를 위한 준비 : NPC 생성 등등");
    m_EventArg0 = PREPARE_BATTLE;
}

WAITING_STATE_PROCNODE(SWET_BATTLE_START_JUST_BEFORE, SWET_BATTLE_COUNTDOWN)
{
    const State_Data& rData = m_StateData[m_EventType];

    //  최초 진입 상태 <- 대기 시간 시작한 지 6'50" 경과
    //  처리 기간 [6'55", 7'00"]
    const DWORD NOT_PROCESSED = 0;
    const DWORD BATTLE_COUNTDOWN = !NOT_PROCESSED;

    if(BATTLE_COUNTDOWN == m_EventArg0)
    {
        const BOOLEAN inSection =
            State_Data::IsAcceptSection(tmInfo, m_StartTick, rData.m_Begin, rData.m_End);

        if(!inSection)
        {    // goto next_state
            Clear_WaitingState_Vars();
            m_EventType = rData.m_Next;
            return;
        }
        return;
    }

    WAVERIX_DEBUG_LOG_0S1N("SWET_BATTLE_START_JUST_BEFORE", m_CodeOfSSQ);
    ASSERT(NOT_PROCESSED == m_EventArg0);
    // first process -or- long term skip
    WAVERIX_DEBUG_LOG_0S("A<->G Battle Preparation : Create NPCs, etc...");
    m_EventArg0 = BATTLE_COUNTDOWN;

    {
        BROADCASTER_MSG_AG_ZONE_SSQ_ROOM_CTRL_CMD& rMsgCMD = m_pMsgRouter->GetCMD_AG();
        WAR_MSG_HDR* const pHeader = &rMsgCMD.MSG_HEADER;
        {
            MSGSUB_SSQ_S2S_BATTLE_COUNTDOWN msg;
            msg.CodeOfSSQ = m_CodeOfSSQ;
            msg.SerializeTo(pHeader);
        }
        {
            MSGSUB_SSQ_S2S_BATTLE_PREPARATION msg;
            msg.CodeOfSSQ = m_CodeOfSSQ;
            msg.SerializeTo(pHeader);
        }
    };
    {
        BROADCASTER_MSG_AW_OPERATION_ROOM_CTRL_CMD& rMsgCMD = m_pMsgRouter->GetCMD_AW();
        WAR_MSG_HDR* const pHeader = &rMsgCMD.MSG_HEADER;
        MSGSUB_SSQ_S2S_SET_CHAT_BLOCK_ZONE msg;
        msg.ZoneKey = m_pSSQField->GetKey();
        msg.Enabled = FALSE;
        msg.SerializeTo(pHeader);
    };
    {
        BROADCASTER_MSG_CG_ZONE_EX_SSQ_ROOM_CTRL_CMD& rMsgCMD = m_pMsgRouter->GetCMD_CG();
        WAR_MSG_HDR* const pHeader = &rMsgCMD.MSG_HEADER;
        MSGSUB_SSQ_PROTOCOL_NOTICE_BATTLE_COUNTDOWN msg;
        msg.SerializeTo(pHeader);
    };
}


BOOL
SSQUpdateCtrl_Waiting::Update(const curtime_info& tmInfo)
{
    if(m_Timer.IsExpired())
    {
        switch(m_EventType)
        {
        case SWET_START_WAITING_NODE        :
            WAITING_STATE_PROCNODE_CALL(SWET_START_WAITING_NODE, SWET_NOTICE_COUNTDOWN, tmInfo);
            break;
        case SWET_NOTICE_COUNTDOWN            :
            WAITING_STATE_PROCNODE_CALL(SWET_NOTICE_COUNTDOWN, SWET_ENTERENCE_CLOSED, tmInfo);
            break;
        case SWET_ENTERENCE_CLOSED            :
            WAITING_STATE_PROCNODE_CALL(SWET_ENTERENCE_CLOSED, SWET_FINAL_ENTERENCE_TIMEOUT, tmInfo);
            break;
        case SWET_FINAL_ENTERENCE_TIMEOUT    :
            {
                //  1) 전투 시작 조건 판단
                const BOOLEAN isCancelStart =
                    (m_pSSQField->GetUserNum() <
                     (DWORD)m_pSSQInfo->m_PlayerCountInfo[SSQInfo::PCL_MIN]);

                WAVERIX_DEBUG_CODE(
                    const CHAR* const pResultMsg
                        = isCancelStart
                        ?    "게임시작 최소 인원 제한에 걸려 SSQ 처리 실패"
                        :    "게임시작 최소 인원 제한 만족";
                    WAVERIX_DEBUG_LOG_0S1N(pResultMsg, m_CodeOfSSQ);
                );

                if(isCancelStart && m_IsFastTrack == 0)
                {
                    Process_Change_ClearState(tmInfo);
                    return TRUE;
                }
                WAITING_STATE_PROCNODE_CALL(SWET_FINAL_ENTERENCE_TIMEOUT, SWET_GAMBLE_PROCESS, tmInfo);
            }
            break;
        case SWET_GAMBLE_PROCESS        :
            {
                WAITING_STATE_PROCNODE_CALL(SWET_GAMBLE_PROCESS, SWET_BATTLE_GRACE_PERIOD, tmInfo);
            }
            break;
        case SWET_BATTLE_GRACE_PERIOD        :
                WAITING_STATE_PROCNODE_CALL(SWET_BATTLE_GRACE_PERIOD, SWET_BATTLE_START_JUST_BEFORE, tmInfo);
            break;
        case SWET_BATTLE_START_JUST_BEFORE    :
                WAITING_STATE_PROCNODE_CALL(SWET_BATTLE_START_JUST_BEFORE, SWET_BATTLE_COUNTDOWN, tmInfo);
                break;
        case SWET_BATTLE_COUNTDOWN    :
            {
                WAVERIX_DEBUG_LOG_0S1N("SWET_BATTLE_COUNTDOWN", m_CodeOfSSQ);
                // change updater - battle process
                Process_Change_BattleState(tmInfo);
            }
            break;
        default:
            ASSERT(!"Impossible Control Logic");
            break;
        };
    }

    m_CCUReport.Update(tmInfo, this);
    return TRUE;
}

VOID
SSQUpdateCtrl_Waiting::Process_Change_BattleState(const curtime_info& tmInfo)
{
    __WAVERIX_LOG_FORMAT_0S1N("Change to BattleState", m_CodeOfSSQ);

    SSQUpdateCtrl_Battle* const pCtrl = TAllocNew(SSQUpdateCtrl_Battle_Info::value_type);
    pCtrl->Migration(pCtrl, this);
    pCtrl->Init(tmInfo);
    pCtrl->RegisterUpdateCtrl(pCtrl);
    TAllocDelete(updater_info::value_type, this);
}

VOID
SSQUpdateCtrl_Waiting::Process_Change_ClearState(const curtime_info& tmInfo)
{
    __WAVERIX_LOG_FORMAT_0S1N("Change to ClearState", m_CodeOfSSQ);

    m_ControlField &= ~RCS_BATTLE_STARTED;

    {
        BROADCASTER_MSG_AG_ZONE_SSQ_ROOM_CTRL_CMD& rMsgCMD = m_pMsgRouter->GetCMD_AG();
        WAR_MSG_HDR* const pHeader = &rMsgCMD.MSG_HEADER;
        MSGSUB_SSQ_S2S_BATTLE_CANCEL msg;
        msg.CodeOfSSQ = m_CodeOfSSQ;
        msg.SerializeTo(pHeader);
    };
    {
        BROADCASTER_MSG_AW_OPERATION_ROOM_CTRL_CMD& rMsgCMD = m_pMsgRouter->GetCMD_AW();
        WAR_MSG_HDR* const pHeader = &rMsgCMD.MSG_HEADER;
        MSGSUB_SSQ_S2S_SET_CHAT_BLOCK_ZONE msg;
        msg.ZoneKey = m_pSSQField->GetKey();
        msg.Enabled = TRUE;
        msg.SerializeTo(pHeader);
    };
    {
        BROADCASTER_MSG_CG_ZONE_EX_SSQ_ROOM_CTRL_CMD& rMsgCMD = m_pMsgRouter->GetCMD_CG();
        WAR_MSG_HDR* const pHeader = &rMsgCMD.MSG_HEADER;
        MSGSUB_SSQ_PROTOCOL_NOTICE_BATTLE_CANCLED msg;
        msg.SerializeTo(pHeader);
    };

    SSQUpdateCtrl_Clear* const pCtrl = TAllocNew(SSQUpdateCtrl_Clear_Info::value_type);
    pCtrl->Migration(pCtrl, this);
    pCtrl->Init(tmInfo);
    pCtrl->RegisterUpdateCtrl(pCtrl);
    TAllocDelete(updater_info::value_type, this);
}


//==================================================================================================
//

VOID
SSQCCUReporter::Init(const curtime_info& tmInfo, SSQUpdateCtrl* const pUpdateCtrl)
{
    const eTIME_TYPE time_type = pUpdateCtrl->GetTimeType();
    ASSERT(time_type == TT_BATTLE || time_type == TT_WAITING);

    const BOOLEAN isBattleCountdown = time_type == TT_BATTLE;
    const SSQInfo* const pSSQInfo = pUpdateCtrl->GetSSQInfo();
    const DWORD total_second = isBattleCountdown ? pSSQInfo->m_QuestPlayTime * 60
                                                 : pSSQInfo->m_WatingTime * 60;

    m_EndTick = tmInfo.CurTick + (total_second*second);
    m_CCUReportTimer.Instance();
}


bool
SSQCCUReporter::Update(const curtime_info& tmInfo, SSQUpdateCtrl* const pUpdateCtrl)
{
    if(!m_CCUReportTimer.IsExpired())
        return true;

    m_CCUReportTimer.SetNextTimeoutByCurTickBased(5*second);

    const BOOLEAN bOverThan = !(tmInfo.CurTick < m_EndTick);

    BROADCASTER_MSG_CG_ZONE_EX_SSQ_ROOM_CTRL_CMD& rMsgCMD = pUpdateCtrl->GetMsgRouter()->GetCMD_CG();
    WAR_MSG_HDR* const pHeader = &rMsgCMD.MSG_HEADER;
    MSGSUB_SSQ_PROTOCOL_INFORMATION_REMAINED_TIME msg;

    if(bOverThan)
    {
        msg.Minute = 0;
        msg.Second = 0;
        msg.SerializeTo(pHeader);
        return false;
    }
    else
    {
        const DWORD diff = (DWORD)((m_EndTick - tmInfo.CurTick) / 1000);
        const SSQInfo::MINUTE_T remain_min = static_cast<SSQInfo::MINUTE_T>(diff / 60);
        const SSQInfo::SECOND_T remain_sec = static_cast<SSQInfo::SECOND_T>(diff % 60);
        msg.Minute = remain_min;
        msg.Second = remain_sec;
        msg.SerializeTo(pHeader);
    }
    return true;
}


//==================================================================================================
//  <SSQUpdateCtrl><Battle State>
//

VOID
SSQUpdateCtrl_Battle::Init(const curtime_info& tmInfo)
{
    WAVERIX_DEBUG_LOG_0S1N("Init", m_CodeOfSSQ);
    m_CurType = value;
    m_Timer.SetTimer(interval);

    SSQInfo::TimeRange range = m_pSSQInfo->m_CTimeBattle[m_ProcedIndex];
    _ConvertWzTmFmt2AcmTime(range);
    m_BattleCloseTimeFlag = range.End;
    m_BattleCloseCountdown = m_pSSQInfo->m_QuestPlayTime + 1; // (+1) 최초 Broadcasting처리 목적

    m_CCUReport.Init(tmInfo, this);

    m_ControlField = RCS_BATTLE_STARTED;

    {
        BROADCASTER_MSG_AG_ZONE_SSQ_ROOM_CTRL_CMD& rMsgCMD = m_pMsgRouter->GetCMD_AG();
        WAR_MSG_HDR* const pHeader = &rMsgCMD.MSG_HEADER;
        MSGSUB_SSQ_S2S_BATTLE_START msg;
        msg.CodeOfSSQ = m_CodeOfSSQ;
        msg.SerializeTo(pHeader);
    };
    {
        BROADCASTER_MSG_CG_ZONE_EX_SSQ_ROOM_CTRL_CMD& rMsgCMD = m_pMsgRouter->GetCMD_CG();
        WAR_MSG_HDR* const pHeader = &rMsgCMD.MSG_HEADER;
        MSGSUB_SSQ_PROTOCOL_NOTICE_BATTLE_START msg;
        msg.SerializeTo(pHeader);
    };
}

BOOL
SSQUpdateCtrl_Battle::Update(const curtime_info& tmInfo)
{
    //  send collected data to players
    //  - 전투가 빨리 끝날 수도 있다.

    // (WAVERIX) (090720) (CHANGES) void->bool (true: continued, false: overthan endtick)
    //const BOOLEAN isTimeout = tmInfo.AcmTime > m_BattleCloseTimeFlag;
    const BOOLEAN isTimeout = !m_CCUReport.Update(tmInfo, this);
    const BOOLEAN isNeedClear = !!(m_ControlField & RCS_BATTLE_END_REQUESTED);

    if(isTimeout || isNeedClear)
    {
        __WAVERIX_LOG_FORMAT_0S1N("On Battle End Condition", m_CodeOfSSQ);

        {
            BROADCASTER_MSG_AG_ZONE_SSQ_ROOM_CTRL_CMD& rMsgCMD = m_pMsgRouter->GetCMD_AG();
            WAR_MSG_HDR* const pHeader = &rMsgCMD.MSG_HEADER;
            MSGSUB_SSQ_S2S_BATTLE_END msg;
            msg.CodeOfSSQ = m_CodeOfSSQ;
            msg.SerializeTo(pHeader);
        };
        {
            BROADCASTER_MSG_CG_ZONE_EX_SSQ_ROOM_CTRL_CMD& rMsgCMD = m_pMsgRouter->GetCMD_CG();
            WAR_MSG_HDR* const pHeader = &rMsgCMD.MSG_HEADER;
            MSGSUB_SSQ_PROTOCOL_NOTICE_BATTLE_END msg;
            msg.SerializeTo(pHeader);
        };

        SSQUpdateCtrl_Clear* const pCtrl = TAllocNew(SSQUpdateCtrl_Clear_Info::value_type);
        pCtrl->Migration(pCtrl, this);
        pCtrl->Init(tmInfo);
        pCtrl->RegisterUpdateCtrl(pCtrl);
        TAllocDelete(updater_info::value_type, this);
        return TRUE;
    }

    //
    const WzTmFmt countdown = m_BattleCloseTimeFlag - tmInfo.AcmTime;
    if(countdown != m_BattleCloseCountdown)
    {
        --m_BattleCloseCountdown;
        //{
        //  BROADCASTER_MSG_CG_ZONE_EX_SSQ_ROOM_CTRL_CMD& rMsgCMD = m_pMsgRouter->GetCMD_CG();
        //  WAR_MSG_HDR* const pHeader = &rMsgCMD.MSG_HEADER;
        //  MSGSUB_SSQ_PROTOCOL_INFORMATION_REMAINED_TIME msg;
        //  msg.Remains = (SSQInfo::MINUTE_T)countdown;
        //  msg.SerializeTo(pHeader);
        //};
    }

    // (WAVERIX) (090720) (CHANGES) void->bool (true: continued, false: overthan endtick)
    //m_CCUReport.Update(tmInfo, this);

    return TRUE;
}


//==================================================================================================
//  <SSQUpdateCtrl><Clear State>
//  -    전투 후 정리 단계...
VOID
SSQUpdateCtrl_Clear::Init(const curtime_info& tmInfo)
{
    __UNUSED(tmInfo);
    m_CurType = value;
    m_Timer.SetTimer(interval);
    m_ClearStep = eStep_Init;

    //  m_ControlField    <- 이것은 Waiting & Battle에서 설정된 값으로 입력되므로, 이것에 따른 처리를 수행해야 한다.
    //  Waiting에서 전투 시작 취소가 발생했을 경우, 복귀시키고, 복귀 이후의 (입장료) 복구 처리를 수행한다.
    //  Battle에서의 전투 시작 취소가 발생했을 경우, 관련 정보 처리, 복귀 처리
    //  (공통)
    //      - 적절한 타임아웃 이후 강제 방 파괴... 안 나가고 버팅기는 상황도 충분히 존재할 수 있다.
    WAVERIX_DEBUG_LOG_0S("");
    if(m_ControlField & RCS_BATTLE_STARTED)
    {
        __WAVERIX_LOG_FORMAT_0S1N("전투 종료후 보상 처리", m_CodeOfSSQ);
    }
    else
    {
        __WAVERIX_LOG_FORMAT_0S1N("전투 취소후 복구 처리", m_CodeOfSSQ);
    }
}

BOOL
SSQUpdateCtrl_Clear::Update(const curtime_info& tmInfo)
{
    //  restore all changed condition
    //  winner reward process
    //  expel players from SSQField
    //  close SSQField

    switch(m_ClearStep)
    {
    case eStep_Init:
        m_ClearStep = (eStep)(m_ClearStep+1);
        break;
    case eStep_Wait_Cleared:
        _Step_Wait_Cleared(tmInfo);
        break;
    case eStep_Next:
        _Step_Next(tmInfo);
        break;
    };

    return TRUE;
}


VOID
SSQUpdateCtrl_Clear::_Step_Wait_Cleared(const curtime_info& tmInfo)
{
    __UNUSED(tmInfo);
    if(m_ControlField & RCS_CLEARED)
    {
        __WAVERIX_LOG_FORMAT_0S1N("Received Zone Clear Msg from GameServer", m_CodeOfSSQ);
        m_ClearStep = (eStep)(m_ClearStep+1);
    }
    else if(!(m_ControlField & RCS_CLEARING))
    {
        BROADCASTER_MSG_AG_ZONE_SSQ_ROOM_CTRL_CMD& rMsgCMD = m_pMsgRouter->GetCMD_AG();
        WAR_MSG_HDR* const pHeader = &rMsgCMD.MSG_HEADER;
        {
            MSGSUB_SSQ_S2S_FIELD_BLOCK msg;
            msg.CodeOfSSQ = m_CodeOfSSQ;
            msg.SerializeTo(pHeader);
        };
        {
            MSGSUB_SSQ_S2S_BATTLE_CLEAR msg;
            msg.CodeOfSSQ = m_CodeOfSSQ;
            msg.SerializeTo(pHeader);
        };

        m_ControlField |= RCS_CLEARING;
    }
}


VOID
SSQUpdateCtrl_Clear::_Step_Next(const curtime_info& tmInfo)
{
    m_pSSQField->SetClearProcess();

    //////////////////////////////////////////////////////////////////////////
    SSQUpdateCtrl_None* const pCtrl = TAllocNew(SSQUpdateCtrl_None_Info::value_type);
    pCtrl->Migration(pCtrl, this);
    pCtrl->Init(tmInfo);
    pCtrl->RegisterUpdateCtrl(pCtrl);
    TAllocDelete(updater_info::value_type, this);
}


