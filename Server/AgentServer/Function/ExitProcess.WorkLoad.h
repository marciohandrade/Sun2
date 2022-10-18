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
	{ // ���� ����� �����ϴ� ������ ����
		// ���� ��� �ð�
		DWORD			m_TotalDelay;
		DWORD			m_NetworkBlockDelay;
		// ���� ��� ���� �������� ��Ŷ ������ ���̻� ó������ �ʰ� ����ϴ� �ð�
		// �� �ð��� ������ ��Ŷ���� �����Ǵ� �ð��� ������ ���̴�.
		// �ʹ� ���, ���Ϳ��� �¾� �״´�. 5�ʸ� �˳��� ��
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

