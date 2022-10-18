#include "stdafx.h"

//////////////////////////////////////////////////////////////////////////
#include "AgentServer.Timer.WorkLoad.h"
#include <PacketStruct_AM.h>

#include <AgentServer.h>
#include <UserSessions/UserManager.h>
#include <Zone/ZoneManager.h>
#include <Zone/PortalManager.h>
#include <AgentParty/AgentPartyManager.h>
#include <SunAuth/AuthProtocol_GA.h>
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#include <ChaosZoneSystem/BattleGroundManager.h>
#endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#ifdef _NA_006088_20120910_CHAOS_ZONE_FREE_PVP
#include <ChaosZoneSystem/FreePVPManager.h>
#endif // _NA_006088_20120910_CHAOS_ZONE_FREE_PVP
#ifdef _NA_006620_20130402_GM_BATTLE_GROUND_EVENT_GAME
#include <ChaosZoneSystem/TeamBattleGroundManager.h>
#endif // _NA_006620_20130402_GM_BATTLE_GROUND_EVENT_GAME
#ifdef _NA_008543_20151027_ONE_DAYS_GUILD_MISSION
#include <ChaosZoneSystem/OneDaysGuildMissionManager.h>
#endif //(_NA_008543_20151027_ONE_DAYS_GUILD_MISSION)

#ifdef _NA_20100802_SPEED_HACK_DETECTION
namespace ns_functor {
;

struct CheckTickSpeed : public ns_functor::IUserForeachFunctor
{
    virtual bool operator()(User* const user)
	{
		DWORD tick_count = GetTickCount();
		TickSpeedChecker& checker = user->GetTickSpeedChecker();
		if (checker.IsEnabled() && checker.TryActivate(tick_count)) {
			checker.Update();
		}
        return true;
    }
};

}; //end of namespace
#endif

BOOL	PROCESS_AGENTSERVER_PERIODIC_WORKLOAD::DoInit()
{
	Complete() = FALSE;

	m_Interval4Update.SetTimer(UPDATE_INTERVAL);
	m_IntervalUserManager.SetTimer(UPDATE_INTERVAL_USER_MANAGER);
	m_IntervalRemoveNoActionTempUser.SetTimer(UPDATE_INTERVAL_REMOVE_NOACTION_TEMPUSER);
	m_IntervalCCUReport.SetTimer(UPDATE_INTERVAL_CCU_REPORT);
#ifdef __NA001334_090420_SERVER_SELECT_FACILITY__
    m_IntervalServerSelect.SetTimer(UPDATE_INTERVAL_SERVER_SELECT_PROC);
#endif

	m_IntervalS2SUserCount.SetTimer(UPDATE_INTERVAL_S2S_USERCOUNT_NTF);
	m_IntervalUnExpectedClosingUser.SetTimer(UPDATE_INTERVAL_UNEXPECTED_CLOSING_USER);
    m_PortalUpdateTimer.SetTimer(20 * SECOND);
#ifdef _NA_20100802_SPEED_HACK_DETECTION
    m_IntervalTickSpeedCheck.SetTimer(UPDATE_INTERVAL_TICK_SPEED_CHECKER);
#endif
#ifdef _NA_007750_20141113_WZDAT_MODIFY_LOG
    m_IntervalWzdatLog.SetTimer(UPDATE_INTERVAL_WZDAT_LOG);
#endif // _NA_007750_20141113_WZDAT_MODIFY_LOG

#ifdef _NA_000000_20131129_ADD_KISS_API
    //KISS Agent와의 IPC를 개설
    UINT result_error=0;
    result_error = kiss_wrapper.Open();
    if (result_error != ERROR_SUCCESS )
    {
        SUNLOG(eCRITICAL_LOG, _T("|[%s]|KISS Agent Connect Fail![error_code: %d]|"),
            __FUNCTION__, result_error);
    }
#endif //_NA_000000_20131129_ADD_KISS_API
	return TRUE;
}

BOOL	PROCESS_AGENTSERVER_PERIODIC_WORKLOAD::DoProcess()
{
	if(!m_Interval4Update.IsExpired())
		return TRUE;

    const DWORD current_tick = GetTickCount();
    __TOUCHED(current_tick);

    AgentServer* const pAgentServer = AgentServer::GetInstance();
	UserManager& rUserManager = *UserManager::Instance();
	if(m_IntervalUserManager.IsExpired())
		rUserManager.GarbageCollector();

	if(m_IntervalRemoveNoActionTempUser.IsExpired())
		rUserManager.TempUserClosureTimeout();

#ifdef _NA_20100802_SPEED_HACK_DETECTION
    if (m_IntervalTickSpeedCheck.IsExpired()) {
        if (TickSpeedChecker::IsGloballyEnabled()) { // 전역 활성화 여부 먼저 검사
            ns_functor::CheckTickSpeed checker;
            rUserManager.Foreach(&checker);
        }
    }
#endif

    //////////////////////////////////////////////////////////////////////////
    // 다음 구간은 UserManager에서 서비스 중인 유저들을 일괄 처리하는 부분
    // UserManager::Process
    BOOLEAN bCCUReport = !!m_IntervalCCUReport.IsExpired();
#ifdef __NA001334_090420_SERVER_SELECT_FACILITY__
    BOOLEAN bServerSelectProcess = !!m_IntervalServerSelect.IsExpired();
#else
    BOOLEAN bServerSelectProcess = FALSE;
#endif
	if(bCCUReport || bServerSelectProcess)
    {
		MSG_AM_CONNECTION_CCU_REPORT_NTF msg;   // for Reporting

		rUserManager.Process(bCCUReport, bServerSelectProcess, &msg);

        // (WAVERIX) (090521) (BUG-FIX) 동접보고상 문제 수정
        if(bCCUReport) {
		    pAgentServer->SendToMasterServer(&msg, sizeof(msg));
#ifdef _NA_000000_20131129_ADD_KISS_API
            //KISS Agent에 동접정보를 전송
            if (kiss_wrapper.IsOpened() && 
                (pAgentServer->GetMode() == eSERVER_LIVE || pAgentServer->GetMode() == eSERVER_TESTLIVE)) {
                SendCCU_To_KISS(CalculateTotalCCU(&msg), static_cast<DWORD>(pAgentServer->GetListenPort_s()));
            }
#endif //_NA_000000_20131129_ADD_KISS_API
#ifdef _NA_007750_20141113_WZDAT_MODIFY_LOG
            if (m_IntervalWzdatLog.IsExpired())
            {
                SERVER_KEY rKey(pAgentServer->GetKey());
                TCHAR tszAddInfo[MAX_ADDINFO_SIZE + 1] = { 0, };
                _snprintf(tszAddInfo, MAX_ADDINFO_SIZE, "ChannelID:%u,NumberOfUsers:%f",
                    rKey.GetChannelID(), CalculateTotalCCU(&msg));
                GAMELOG->LogWzDat(SESSION_CHANNEL_INFO, tszAddInfo);
            }
#endif // _NA_007750_20141113_WZDAT_MODIFY_LOG
        }
	}
    //
    //////////////////////////////////////////////////////////////////////////

	if(m_IntervalS2SUserCount.IsExpired())
    {
		MSG_GA_AUTH_USER_COUNT_NTF msg;
		msg.m_wUserCount = (WORD)rUserManager.GetNumberOfUsers();
		pAgentServer->SendToAuthAgentServer((BYTE*)&msg, sizeof(msg));
	}

	if(m_IntervalUnExpectedClosingUser.IsExpired())
    {
		AgentPartyManager::Instance()->Update();
	}

    if(m_PortalUpdateTimer.IsExpired())
        PortalManager::Update();

    //_NA_20110630_BATTLE_ZONE_RENEWAL
    AgentPartyManager::Instance()->DoStartToWaitingRoom();

#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
    BattleGroundManager::Instance()->Update();
#endif // _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
#ifdef _NA_006088_20120910_CHAOS_ZONE_FREE_PVP 
    FreePVPManager::Instance()->Update();
#endif // _NA_006088_20120910_CHAOS_ZONE_FREE_PVP 
#ifdef _NA_006620_20130402_GM_BATTLE_GROUND_EVENT_GAME
    TeamBattleGroundManager::Instance()->Update();
#endif // _NA_006620_20130402_GM_BATTLE_GROUND_EVENT_GAME

#ifdef _NA_008543_20151027_ONE_DAYS_GUILD_MISSION
	OneDaysGuildMissionManager::Instance()->Update();
#endif //(_NA_008543_20151027_ONE_DAYS_GUILD_MISSION)

	return TRUE;
}

BOOL	PROCESS_AGENTSERVER_PERIODIC_WORKLOAD::IsComplete()
{
	return m_bComplete;
}

BOOL	PROCESS_AGENTSERVER_PERIODIC_WORKLOAD::DoRelease()
{
#ifdef _NA_000000_20131129_ADD_KISS_API
    //KISS Agent와의 IPC개설을 종료
    if (kiss_wrapper.IsOpened() ) {
        kiss_wrapper.Close();
    }
#endif //_NA_000000_20131129_ADD_KISS_API
	return TRUE;
}

VOID	PROCESS_AGENTSERVER_PERIODIC_WORKLOAD::OnMsg(sSIG_MSG* pSIG_MSG)
{
    __UNUSED(pSIG_MSG);
}

VOID	PROCESS_AGENTSERVER_PERIODIC_WORKLOAD::_Process_CCU_Reporting()
{
}

FLOAT PROCESS_AGENTSERVER_PERIODIC_WORKLOAD::CalculateTotalCCU( const MSG_AM_CONNECTION_CCU_REPORT_NTF* ccu_msg )
{
	DWORD dwCCU = ccu_msg->dwVillageCount;
	dwCCU += ccu_msg->dwVillageCountPC;
	dwCCU += ccu_msg->dwBattleZoneCount;
	dwCCU += ccu_msg->dwBattleZoneCountPC;

#ifdef _CHANNELING_AUTH_FLOW_HANGAME_USER_
	dwCCU += ccu_msg->numberOfInVillageHangame;       // Village    | Default | Hangame
	dwCCU += ccu_msg->numberOfInVillageHangamePC;     // Village    | PC      | Hangame
	dwCCU += ccu_msg->numberOfInBattleZoneHangame;    // BattleZone | Default | Hangame
	dwCCU += ccu_msg->numberOfInBattleZoneHangamePC;  // BattleZone | PC      | Hangame
#endif //_CHANNELING_AUTH_FLOW_HANGAME_USER_
#ifdef __NA000000_090911_NEOWIZ_USER_CCU_REPORT__
	dwCCU += ccu_msg->numberOfInVillageNeowiz;       // Village    | Default | Neowiz
	dwCCU += ccu_msg->numberOfInVillageNeowizPC;     // Village    | PC      | Neowiz
	dwCCU += ccu_msg->numberOfInBattleZoneNeowiz;    // BattleZone | Default | Neowiz
	dwCCU += ccu_msg->numberOfInBattleZoneNeowizPC;  // BattleZone | PC      | Neowiz
#endif

	

	return static_cast<FLOAT>(dwCCU);
}

#ifdef _NA_000000_20131129_ADD_KISS_API
VOID PROCESS_AGENTSERVER_PERIODIC_WORKLOAD::SendCCU_To_KISS( const float server_ccu, 
                                                             const DWORD agentserver_listenport )
{
    NOTIFY_INFO_NUM infoNum;
    int activity_id = 1005;
    DWORD kiss_port = agentserver_listenport;
    
    InitNotifyInfoNumber(&infoNum, activity_id, server_ccu, 0, 0, kiss_port);

    if (ERROR_SUCCESS != kiss_wrapper.NotifyNumber(&infoNum)) {
        SUNLOG(eCRITICAL_LOG, _T("|[%s]|KISS Error! Notify Number|"),
            __FUNCTION__);
    }

}  
#endif //_NA_000000_20131129_ADD_KISS_API

