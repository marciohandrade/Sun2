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
    //  Ư�� ���� ������ ��... enter enabled state
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
    // UserLeave�� Zone::users->Slot(insert) ó���� �̷������� ����.??
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
            m_ProcedIndex = (BYTE)findedIndex;    // ���� Ȱ��ȭ�� ��
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

    // �ʱ�ȭ �Ǵ� ���� broadcast ó�� �ð�
    m_StartTick = m_NextTimeout = GetTickCount();
    SSQInfo::TimeRange range = m_pSSQInfo->m_CTimeNotice[m_ProcedIndex];
    _ConvertWzTmFmt2AcmTime(range);

    // Init��Ű�� ��ü�� SSQUpdateCtrl_None�̰�, �����ð����� ���ϹǷ� �׻� 0<=x
    ASSERT(tmInfo.AcmTime >= range.Start);
    m_NextAcmTime = tmInfo.AcmTime; // ó������ �ٷ� enter
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

        // 5ȸ ���� ���� ���ð����� ��ȯ - (WARNING) SSQ Field���� ���� Ȯ���� ��
        if(broadcast_changeto_waiting_countdown_value /*0*/ == m_Countdown)
        {
            Process_CreateSSQField();    // ���� �̺�Ʈ�� ���� ������ m_pSSQField�� �����ȴ�.

            SSQUpdateCtrl* pCtrl = NULL;
            if(!m_pSSQField)
            {    // �� �̻� ������ �� ���� ��Ȳ�̴�. �̹� SSQ Dungeon open�� ��ŵ�ϱ�� �Ѵ�.
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

        //  5,4,3,2,1 �� ���� ��� ���⿡ �����ϰ� �� ���̴�.
        SSQInfo::TimeRange range = m_pSSQInfo->m_CTimeNotice[m_ProcedIndex];
        _ConvertWzTmFmt2AcmTime(range);
        const BYTE remain_countdown = range.End >= tmInfo.AcmTime
                                    ?   BYTE(range.End - tmInfo.AcmTime)
                                    :   0;
        if(FlowControl::FCAssert((remain_countdown <= broadcast_countdown_init) &&
                                 "��� ó���ؾ��� ���� ����϶�..."))
        {
            // ���ǿ� �����ϴ� ���� next timeout�� ���϶�.
            BYTE offset = m_StartCountdown - remain_countdown;__TOUCHED(offset);
            //m_NextTimeout = m_StartTick + interval_broadcast;// * (offset));
            m_Countdown = remain_countdown;
            m_NextAcmTime += 1; // 1'
        }
        else
        {
            __WAVERIX_LOG_FORMAT_0S1N("Proc Error", remain_countdown);
            // ���� ó���� �ؾ��� ���̴�.
            m_NextTimeout = 0;
            m_Countdown = broadcast_changeto_waiting_countdown_value;
        }

        switch(m_Countdown)
        {
        case broadcast_countdown_init - 0: // (5-0)�� ��
        case broadcast_countdown_init - 1: // (5-1)�� ��
        case broadcast_countdown_init - 2: // (5-2)�� ��
        case broadcast_countdown_init - 3: // (5-3)�� ��
        case broadcast_countdown_init - 4: // (5-4)�� ��
            {
                // ���߿� BattleServer�� �׾��ų� ���߿� �ߴ� ��츦 �����Ѵ�.
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

        //  ���� ã���� ����.... CodeOfSSQ��...
        //  -    Key�� ����, Zone IO ����� ���������� ���ؼ�
        InstanceDungeon* pInstanceDungeon = NULL;
        const RC::eROOM_RESULT rt = g_ZoneManager.CreateStaticDungeon(m_pSSQInfo->m_MapCode,
                                                                      pInstanceDungeon,
                                                                      INDUN_KIND_SS_QUEST,
                                                                      FALSE);
        //  �̰��� ������ �ڿ� this->GetSSQField�� �ڵ� �����ȴ�.
        IfFailThenBreak(RC::RC_ROOM_SUCCESS == rt, RESULT_FAIL);

        // �ش� ������ ���� object������ �Ҵ�Ǿ�� �Ѵ�.
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
    // ��� ������ ������ ���� ����

    // < ���� ��ũ��Ʈ���� ���ѻ����� ����Ǿ��� ��� ���⿡ �����ϴ� ����� ����ؾ� �� ���̴�.

    {    // event: ���� ���� ����(instance)
        State_Data data = {
            SWET_START_WAITING_NODE,
            0, 2 * minute, //, 0, enterable_limit_enter_notice
            SWET_NOTICE_COUNTDOWN
        };
        m_StateData[SWET_START_WAITING_NODE            ] = data;
    };
    {    // event: ���� ���� countdown ����(2x)
        State_Data data = {
            SWET_NOTICE_COUNTDOWN,
            2 * minute, 5 * minute, //, enterable_limit_enter_notice, enterable_limit_countdown_notice
            SWET_ENTERENCE_CLOSED
        };
        m_StateData[SWET_NOTICE_COUNTDOWN            ] = data;
    };
    {    // event: ���� ���� ����(instance), ���� ��û ��� ó�� <- skip process <-
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

    //����������������������������������������������������������������������������������
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
    const DWORD NOTICE_2_3 = 2;    // ���� ���� 2�� ��
    const DWORD NOTICE_3_4 = 3;    // ���� ���� 1�� ��
    const DWORD NOTICE_4__ = 4;    // ���� ����
    const DWORD GOTO_NEXT  = 5;

    if(NOT_PROCESSED != m_EventArg0)
    {
        const BOOLEAN inSection =
            State_Data::IsAcceptSection(tmInfo, m_StartTick, rData.m_Begin, rData.m_End);
        if(!inSection)
        {    // goto next_state - ������ ������ ���� ���μ����� ���� �� ��츦 ��������.
            //  �׷� ����� �ٷ� �Ʒ��� ���·� �Ѿ�� ���� ���� ���� �𸣰ڴ�.
            WAVERIX_DEBUG_LOG_0S1N("Change to NextState - ���� ���� �Ұ�", m_CodeOfSSQ);
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
                {    //  2���� ���� �߼�
                    WAVERIX_DEBUG_LOG_0S1N("(���� ���� ����) 2�� ��", m_CodeOfSSQ);
                    m_EventArg0 = NOTICE_2_3;
                    m_EventArg1 = m_StartTick + 3 * minute;
                    if(m_IsFastTrack)
                        m_EventArg1 = m_StartTick + 10*second;
                    m_pManager->SSQSingletonNoticeCtrl.Broadcast_Notice(
                        this, SSQSingletonNotice::eNotice_CloseCountdown, 2);
                }
                break;
            case NOTICE_2_3:
                {    //  1���� ���� �߼�
                    WAVERIX_DEBUG_LOG_0S1N("(���� ���� ����) 1�� ��", m_CodeOfSSQ);
                    m_EventArg0 = NOTICE_3_4;
                    m_EventArg1 = m_StartTick + 4 * minute;
                    if(m_IsFastTrack)
                        m_EventArg1 = m_StartTick + 15*second;
                    m_pManager->SSQSingletonNoticeCtrl.Broadcast_Notice(
                        this, SSQSingletonNotice::eNotice_CloseCountdown, 1);
                }
                break;
            case NOTICE_3_4:
                {    //  ���� ��������� 1�� ó�� (50')
                    __WAVERIX_LOG_FORMAT_0S1N("(���� ���� ����) ��Ż ���� ���ɽð� ��", m_CodeOfSSQ);
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
                    WAVERIX_DEBUG_LOG_0S1N("(���� ����) 10�� ��", m_CodeOfSSQ);
                    m_EventArg0 = GOTO_NEXT;
                    m_EventArg1 = m_StartTick + 4 * minute + 59 * second;
                    if(m_IsFastTrack)
                        m_EventArg1 = m_StartTick + 25*second;
                    return;
                }
                break;
            case GOTO_NEXT:
                {
                    WAVERIX_DEBUG_LOG_0S1N("(���� ���� ����) ���� ����", m_CodeOfSSQ);
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
    m_EventArg0 = NOTICE_0_2;    // 0~2�� ����
    m_EventArg1 = m_StartTick + 2 * minute;
    if(m_IsFastTrack)
        m_EventArg1 = m_StartTick;
}

WAITING_STATE_PROCNODE(SWET_ENTERENCE_CLOSED, SWET_FINAL_ENTERENCE_TIMEOUT)
{
    const State_Data& rData = m_StateData[m_EventType];

    //  ó�� �Ⱓ [4'5", 5']
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

    //  ó�� �Ⱓ [5', 6']
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

    //  ���� ���� ���� <- ��� �ð� ������ �� 6�� ���
    //  ó�� �Ⱓ [6'00", 6'15"]
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
        //��Ʋ�� ������ ���� Ÿ���� ��� ���� ���� ������ ������� ��ũ��Ʈ ��
        //���� �ݾ��� 0���� �����ϰ� �Ǹ鼭 �׺� �̺�Ʈ��ü�� ��ȿ���� �������.
        //��ũ��Ʈ �󿡼� ���� �ݾ��� 0�̻� ������ �����ϸ� �׺� �̺�Ʈ�� ���� ����.
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
                {    //  Gamble��� Ȯ�� ���ΰ� �߿��ϴ�.
                    __WAVERIX_LOG_FORMAT_0S1N("Notice Gamble Result", m_CodeOfSSQ);
                    Clear_WaitingState_Vars();
                    m_EventType = rData.m_Next;

                    //  (THINKING)
                    //  - Agnet����? GameServer����? �������Ѽ� ������?, Ŭ�� �����ұ�?
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
        m_EventArg1 = m_StartTick + 6 * minute;    // �ٷ� ���� ������� 
    else
        m_EventArg1 = m_StartTick;
}

WAITING_STATE_PROCNODE(SWET_BATTLE_GRACE_PERIOD, SWET_BATTLE_START_JUST_BEFORE)
{
    const State_Data& rData = m_StateData[m_EventType];

    //  ���� ���� ���� <- ���� ������ 6'15" -> 6'50" ����...
    //  ó�� �Ⱓ [6'15", 6'50"]
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
    //WAVERIX_DEBUGGING_OUTPUTn("A<->G ������ ���� �غ� : NPC ���� ���");
    m_EventArg0 = PREPARE_BATTLE;
}

WAITING_STATE_PROCNODE(SWET_BATTLE_START_JUST_BEFORE, SWET_BATTLE_COUNTDOWN)
{
    const State_Data& rData = m_StateData[m_EventType];

    //  ���� ���� ���� <- ��� �ð� ������ �� 6'50" ���
    //  ó�� �Ⱓ [6'55", 7'00"]
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
                //  1) ���� ���� ���� �Ǵ�
                const BOOLEAN isCancelStart =
                    (m_pSSQField->GetUserNum() <
                     (DWORD)m_pSSQInfo->m_PlayerCountInfo[SSQInfo::PCL_MIN]);

                WAVERIX_DEBUG_CODE(
                    const CHAR* const pResultMsg
                        = isCancelStart
                        ?    "���ӽ��� �ּ� �ο� ���ѿ� �ɷ� SSQ ó�� ����"
                        :    "���ӽ��� �ּ� �ο� ���� ����";
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
    m_BattleCloseCountdown = m_pSSQInfo->m_QuestPlayTime + 1; // (+1) ���� Broadcastingó�� ����

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
    //  - ������ ���� ���� ���� �ִ�.

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
//  -    ���� �� ���� �ܰ�...
VOID
SSQUpdateCtrl_Clear::Init(const curtime_info& tmInfo)
{
    __UNUSED(tmInfo);
    m_CurType = value;
    m_Timer.SetTimer(interval);
    m_ClearStep = eStep_Init;

    //  m_ControlField    <- �̰��� Waiting & Battle���� ������ ������ �ԷµǹǷ�, �̰Ϳ� ���� ó���� �����ؾ� �Ѵ�.
    //  Waiting���� ���� ���� ��Ұ� �߻����� ���, ���ͽ�Ű��, ���� ������ (�����) ���� ó���� �����Ѵ�.
    //  Battle������ ���� ���� ��Ұ� �߻����� ���, ���� ���� ó��, ���� ó��
    //  (����)
    //      - ������ Ÿ�Ӿƿ� ���� ���� �� �ı�... �� ������ ���ñ�� ��Ȳ�� ����� ������ �� �ִ�.
    WAVERIX_DEBUG_LOG_0S("");
    if(m_ControlField & RCS_BATTLE_STARTED)
    {
        __WAVERIX_LOG_FORMAT_0S1N("���� ������ ���� ó��", m_CodeOfSSQ);
    }
    else
    {
        __WAVERIX_LOG_FORMAT_0S1N("���� ����� ���� ó��", m_CodeOfSSQ);
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


