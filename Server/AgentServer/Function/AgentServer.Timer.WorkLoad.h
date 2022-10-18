#pragma once

//  @history:
//      - 090427, waverix : change ITimerBase -> ITimerBase
#ifdef _NA_000000_20131129_ADD_KISS_API
#include "KISSApi.h"
#include "KISSApiWrapper.h"

#define AGENTSERVER_PORT_NUMBER 55901
#endif //_NA_000000_20131129_ADD_KISS_API

class PROCESS_AGENTSERVER_PERIODIC_WORKLOAD : public IWorkNode
{
public:
#define DECL_CONST_FIELD	static const DWORD

	DECL_CONST_FIELD	SECOND										= 1000;
	DECL_CONST_FIELD	MINUTE										= 60*SECOND;
	DECL_CONST_FIELD	UPDATE_INTERVAL								=  1*SECOND;
	DECL_CONST_FIELD	UPDATE_INTERVAL_USER_MANAGER				=  5*MINUTE;
	DECL_CONST_FIELD	UPDATE_INTERVAL_REMOVE_NOACTION_TEMPUSER	=  5*SECOND;
	DECL_CONST_FIELD	UPDATE_INTERVAL_CCU_REPORT					= 10*SECOND;
    DECL_CONST_FIELD	UPDATE_INTERVAL_SERVER_SELECT_PROC          =  2*SECOND;
	DECL_CONST_FIELD	UPDATE_INTERVAL_S2S_USERCOUNT_NTF			=  5*SECOND;
	DECL_CONST_FIELD	UPDATE_INTERVAL_UNEXPECTED_CLOSING_USER		= 10*SECOND;
#ifdef _NA_20100802_SPEED_HACK_DETECTION
    DECL_CONST_FIELD    UPDATE_INTERVAL_TICK_SPEED_CHECKER          =  1*SECOND;
#endif
#ifdef _NA_007750_20141113_WZDAT_MODIFY_LOG
    DECL_CONST_FIELD    UPDATE_INTERVAL_WZDAT_LOG                   =  1*MINUTE;
#endif // _NA_007750_20141113_WZDAT_MODIFY_LOG

#undef DECL_CONST_FIELD

public:
    PROCESS_AGENTSERVER_PERIODIC_WORKLOAD() {}
    ~PROCESS_AGENTSERVER_PERIODIC_WORKLOAD() {}

//////////////////////////////////////////////////////////////////////////
// <IWORKNODE INTERFACEs>
public:
    virtual BOOL    DoInit();
    virtual BOOL    DoProcess();
    virtual BOOL    IsComplete();
    virtual BOOL    DoRelease();

    virtual VOID    OnMsg( sSIG_MSG* pSIG_MSG );

private:
    inline BOOL&    Complete() { return m_bComplete; }

private:
    VOID _Process_CCU_Reporting();
	FLOAT CalculateTotalCCU(const MSG_AM_CONNECTION_CCU_REPORT_NTF* ccu_msg);
#ifdef _NA_000000_20131129_ADD_KISS_API
    //KISS에 동접자정보를 보낸다
    VOID SendCCU_To_KISS(const float server_ccu, const DWORD agentserver_listenport);
#endif //_NA_000000_20131129_ADD_KISS_API
//////////////////////////////////////////////////////////////////////////
// <FIELDs>
private:
#ifdef _NA_000000_20131129_ADD_KISS_API
    CKISSApiWrapper kiss_wrapper;
#endif //_NA_000000_20131129_ADD_KISS_API
    BOOL       m_bComplete;
    ITimerBase m_Interval4Update;
    ITimerBase m_IntervalUserManager;
    ITimerBase m_IntervalRemoveNoActionTempUser;
    ITimerBase m_IntervalCCUReport;
#ifdef __NA001334_090420_SERVER_SELECT_FACILITY__
    ITimerBase m_IntervalServerSelect;
#endif
    ITimerBase m_IntervalS2SUserCount;
    ITimerBase m_IntervalUnExpectedClosingUser;
    ITimerBase m_PortalUpdateTimer;
#ifdef _NA_20100802_SPEED_HACK_DETECTION
    ITimerBase m_IntervalTickSpeedCheck;
#endif
#ifdef _NA_007750_20141113_WZDAT_MODIFY_LOG
    ITimerBase m_IntervalWzdatLog;
#endif // _NA_007750_20141113_WZDAT_MODIFY_LOG
};

