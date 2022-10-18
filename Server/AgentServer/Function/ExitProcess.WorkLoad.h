#pragma once

class cEXITINFO_OF_AGENTSERVER
{
	friend class PROCESS_EXIT_PROCESS;
public:
	enum eSTATUS
	{
		DISABLED	= FALSE,
		STARTED,
		PROCESSING,
		EXITED,
	};

	static const DWORD MIN_TOTAL_DELAY_TIME = 3000;
	static const DWORD MIN_NETWORK_BLOCK_TIME = 3000;

	cEXITINFO_OF_AGENTSERVER() : m_ProcessStatus(DISABLED), m_wkExitProcess(WorkLoadManager::c_NotExistKey) {}
	~cEXITINFO_OF_AGENTSERVER() {}

	//////////////////////////////////////////////////////////////////////////
	// <SERVER EXIT CONTROL>
	inline eSTATUS		GetExitStatus() { return m_ProcessStatus; }
	inline DWORD		GetWorkKey() { return m_wkExitProcess; }

	BOOL				StartServerExitProcess();

	BOOL				IsNetworkBlock() { return (BOOL)(GetExitStatus() == cEXITINFO_OF_AGENTSERVER::PROCESSING); }
	BOOL				IsShutDown() { return (BOOL)(GetExitStatus() == cEXITINFO_OF_AGENTSERVER::EXITED); }

	//////////////////////////////////////////////////////////////////////////
	// <SERVER EXIT TIMEOUT INFO>
	DWORD&				TotalDelayTime() { return m_TotalDelay; }
	DWORD&				BlockUserOnRecvDelay() { return m_BlockUserOnRecvDelay; }

private:
	DWORD&				NetworkBlockTime() { return m_NetworkBlockDelay; }
private:
	eSTATUS				m_ProcessStatus;
	DWORD				m_wkExitProcess;

	struct
	{ // 서버 종료시 참조하는 딜레이 정보
		// 종료 대기 시간
		DWORD			m_TotalDelay;
		DWORD			m_NetworkBlockDelay;
		// 종료 대기 이후 유저들의 패킷 수신을 더이상 처리하지 않고 대기하는 시간
		// 이 시간은 서버간 패킷들이 정리되는 시간을 감안한 것이다.
		// 너무 길면, 몬스터에게 맞아 죽는다. 5초면 넉넉할 듯
		DWORD			m_BlockUserOnRecvDelay;
	};
};

class PROCESS_EXIT_PROCESS : public IWorkNode
{
	enum eSTATUS
	{
		PROCSTART_SENDBRD_TO_USERS			= 0,
		PROC_POST_WAIT_DELAY_TIME			= PROCSTART_SENDBRD_TO_USERS+1,
		PROC_POST_NETWORK_BLOCK_TIME		= PROC_POST_WAIT_DELAY_TIME+1,
		PROC_COMPLETE						= PROC_POST_NETWORK_BLOCK_TIME+1,
		PROCESSED							= PROC_COMPLETE+1,
	};

	typedef VOID (PROCESS_EXIT_PROCESS::*ProcessFunc)( );

public:
	PROCESS_EXIT_PROCESS() {}
	~PROCESS_EXIT_PROCESS() {}

	VOID	SetInformation( cEXITINFO_OF_AGENTSERVER& rExitREF )
	{
		m_Status = PROCESSED;
		m_pExitREF = &rExitREF;
	}

//////////////////////////////////////////////////////////////////////////
// <IWORKNODE INTERFACEs>
public:
	virtual BOOL	DoInit();
	virtual BOOL	DoProcess();
	virtual BOOL	IsComplete();
	virtual BOOL	DoRelease();

	virtual VOID	OnMsg( sSIG_MSG* pSIG_MSG );

private:
	inline VOID		ChangeNextStatus() { m_Status = (eSTATUS)(m_Status + 1); }
	VOID			PROCESS_SendBRDToUser();
	VOID			PROCESS_PostWaitDelayTime();
	VOID			PROCESS_PostNetworkBlockTime();
	VOID			PROCESS_Complete();
private:
	eSTATUS				m_Status;
	ITimeout			m_Timeout;
	struct
	{
		ProcessFunc			m_Func;
	} m_FUNC[PROCESSED];

	cEXITINFO_OF_AGENTSERVER*	m_pExitREF;
};

