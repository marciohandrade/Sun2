// embedded compile  "MasterServerInfo.h"

#include <PacketStruct_ServerCommon.h>

// 다른 모드에서 서비스로 전환되었을 때, 작업할 내역
class PROCESS_EXIT_PROCESS : public IWorkNode
{
	enum eSTATUS
	{
		PROCSTART_AGENT		= 0,
		PROCWAIT_AGENT		= PROCSTART_AGENT+1,
		PROCED_AGENT		= PROCWAIT_AGENT+1,
		PROCSTART_SERVERS	= PROCED_AGENT+1,
		PROCWAIT_SERVERS	= PROCSTART_SERVERS+1,
		PROCED_SERVERS		= PROCWAIT_SERVERS+1,
		PROCSTART_DBP		= PROCED_SERVERS+1,
		PROCWAIT_DBP		= PROCSTART_DBP+1,
		PROCED_DBP			= PROCWAIT_DBP+1,
		PROCESSED			= PROCED_DBP+1
	};

	typedef VOID (*ProcessFunc)( PROCESS_EXIT_PROCESS* THIS, EXIT_SERVER_MAP* pServers );

public:
	PROCESS_EXIT_PROCESS() {}
	~PROCESS_EXIT_PROCESS() {}

public:
	BOOL	SetInformation(
					EXIT_SERVER_MAP* pAgentServerList,
					EXIT_SERVER_MAP* pDBProxyServerList,
					EXIT_SERVER_MAP* pServerList )
	{
		if( !(pAgentServerList && pDBProxyServerList && pServerList) )
			return FALSE;

		m_pAgentServerList = pAgentServerList;
		m_pDBProxyServerList = pDBProxyServerList;
		m_pServerList = pServerList;

		return TRUE;
	}

public:
	// <INTERFACE><IMPLEMENTATION>
	virtual BOOL	DoInit()
	{
		m_Status = PROCSTART_AGENT;

		m_FUNC[PROCSTART_AGENT].m_Func = &ProcessStart;
		m_FUNC[PROCSTART_AGENT].m_Arg = m_pAgentServerList;

		m_FUNC[PROCWAIT_AGENT].m_Func = &ProcessWait;
		m_FUNC[PROCWAIT_AGENT].m_Arg = m_pAgentServerList;

		m_FUNC[PROCED_AGENT].m_Func = &Processed;
		m_FUNC[PROCED_AGENT].m_Arg = m_pAgentServerList;

		m_FUNC[PROCSTART_SERVERS].m_Func = &ProcessStart;
		m_FUNC[PROCSTART_SERVERS].m_Arg = m_pServerList;

		m_FUNC[PROCWAIT_SERVERS].m_Func = &ProcessWait;
		m_FUNC[PROCWAIT_SERVERS].m_Arg = m_pServerList;

		m_FUNC[PROCED_SERVERS].m_Func = &Processed;
		m_FUNC[PROCED_SERVERS].m_Arg = m_pServerList;

		m_FUNC[PROCSTART_DBP].m_Func = &ProcessStart;
		m_FUNC[PROCSTART_DBP].m_Arg = m_pDBProxyServerList;

		m_FUNC[PROCWAIT_DBP].m_Func = &ProcessWait;
		m_FUNC[PROCWAIT_DBP].m_Arg = m_pDBProxyServerList;

		m_FUNC[PROCED_DBP].m_Func = &Processed;
		m_FUNC[PROCED_DBP].m_Arg = m_pDBProxyServerList;

		MasterServerEx* pMasterServer = MasterServerEx::GetMainFrame();
		MasterServerInfo& rINFO = pMasterServer->GetMasterServerInfo();
		rINFO.ExitProcessTransaction(TRUE);

		NextActionStartTime() = GetTickCount();

		return TRUE;
	}

	virtual BOOL	DoProcess()
	{
		if( PROCESSED == m_Status ) //여기에 걸릴 리는 없겠지만... 걸리면 에러처리하자.
			return TRUE;

		ProcessFunc func = m_FUNC[m_Status].m_Func;
		EXIT_SERVER_MAP* pArg = m_FUNC[m_Status].m_Arg;
		func( this, pArg );

		return TRUE;
	}

	virtual BOOL	IsComplete()
	{
		if( PROCESSED == m_Status )
		{
			MasterServerEx* pMasterServer = MasterServerEx::GetMainFrame();
			MasterServerInfo& rINFO = pMasterServer->GetMasterServerInfo();
			rINFO.ExitProcessTransaction(FALSE);
			return TRUE;
		}

		return FALSE;
	}

	virtual BOOL	DoRelease()
	{
		SAFE_DELETE( m_pAgentServerList );
		SAFE_DELETE( m_pDBProxyServerList );
		SAFE_DELETE( m_pServerList );

		return TRUE;
	}

	virtual VOID	OnMsg( sSIG_MSG* pSIG_MSG );

	inline static void ChangeNextStatus( PROCESS_EXIT_PROCESS* THIS )
	{
		THIS->m_Status = (eSTATUS)(THIS->m_Status + 1);
	}

	static VOID ProcessStart( PROCESS_EXIT_PROCESS* THIS, EXIT_SERVER_MAP* pServers );
	static VOID ProcessWait( PROCESS_EXIT_PROCESS* THIS, EXIT_SERVER_MAP* pServers );
	static VOID Processed( PROCESS_EXIT_PROCESS* THIS, EXIT_SERVER_MAP* pServers );

	inline DWORD&		NextActionStartTime() { return m_Timeout; }
private:
	eSTATUS				m_Status;
	DWORD				m_Timeout;
	EXIT_SERVER_MAP*	m_pAgentServerList;
	EXIT_SERVER_MAP*	m_pDBProxyServerList;
	EXIT_SERVER_MAP*	m_pServerList;
	struct
	{
		ProcessFunc			m_Func;
		EXIT_SERVER_MAP*	m_Arg;
	} m_FUNC[PROCESSED];
};


VOID	PROCESS_EXIT_PROCESS::ProcessStart( PROCESS_EXIT_PROCESS* THIS, EXIT_SERVER_MAP* pServers )
{
	EXIT_SERVER_MAP::iterator it=pServers->begin(), end=pServers->end();

	MSG_SERVERCOMMON_SERVERSHUTDOWN_REQ msgREQ;
	msgREQ.m_byCategory		= SERVERCOMMON;
	msgREQ.m_byProtocol		= SERVERCOMMON_SERVERSHUTDOWN_REQ;

	for( ; it != end ; ++it )
	{
		ServerSessionEx* pServerSession = it->first;
		pServerSession->SendPacket( &msgREQ, sizeof(msgREQ) );
	}

	ChangeNextStatus(THIS);
}

VOID	PROCESS_EXIT_PROCESS::ProcessWait( PROCESS_EXIT_PROCESS* THIS, EXIT_SERVER_MAP* pServers )
{
	if( !(pServers->empty()) )
		return;

	if( GetTickCount() > THIS->NextActionStartTime() )
		ChangeNextStatus(THIS);
}

VOID	PROCESS_EXIT_PROCESS::Processed( PROCESS_EXIT_PROCESS* THIS, EXIT_SERVER_MAP* pServers )
{
	ASSERT( pServers->empty() );
	// 일단 0이 아니더라도 처리해버리자.

	THIS->NextActionStartTime() = GetTickCount() + 5000;
	ChangeNextStatus(THIS);
}

VOID	PROCESS_EXIT_PROCESS::OnMsg( sSIG_MSG* pSIG_MSG )
{
	if( eSIGMSG::EXITPROCESS != pSIG_MSG->_TYPE )
		return;

	sSIG_EXITED_SESSION* pSIGNAL = (sSIG_EXITED_SESSION*)pSIG_MSG;

	ServerSessionEx* pSession = pSIGNAL->Session();

	if( !pSession )
	{
		ASSERT( !"이런 게 와선 안되징~" );
		return;
	}

	EXIT_SERVER_MAP* pServerList = m_FUNC[m_Status].m_Arg;
	if( !pServerList->erase( pSession ) )
	{
		ASSERT( !"PROCESS_EXIT_PROCESS::OnMsg 흐음..." );
	}
}

//
//
//
//
//
//












