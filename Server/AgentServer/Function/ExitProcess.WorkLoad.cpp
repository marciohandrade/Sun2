#include "stdafx.h"

//////////////////////////////////////////////////////////////////////////
#include <AgentServer.h>
#include "ExitProcess.WorkLoad.h"

//////////////////////////////////////////////////////////////////////////
#include <UserSessions/UserManager.h>
#include <ServerSessions/ServerSessionManager.h>
#include <ServerSessions/ServerSessionEx.h>


BOOL	cEXITINFO_OF_AGENTSERVER::StartServerExitProcess()
{
	if( DISABLED != m_ProcessStatus )
		return FALSE;

	PROCESS_EXIT_PROCESS* pWorkNode = new PROCESS_EXIT_PROCESS;
	pWorkNode->SetInformation( *this );

	WORK_KEY wkWorkKey = WorkLoadManager::RegisterTask( pWorkNode );
	if( WorkLoadManager::c_NotExistKey != wkWorkKey )
	{
		m_wkExitProcess = wkWorkKey;
		m_ProcessStatus = cEXITINFO_OF_AGENTSERVER::STARTED;
		return TRUE;
	}

	SAFE_DELETE( pWorkNode );
	return FALSE;
}

BOOL	PROCESS_EXIT_PROCESS::DoInit()
{
	m_Status = PROCSTART_SENDBRD_TO_USERS;
	m_Timeout.SetNextTimeout(0);

	m_FUNC[PROCSTART_SENDBRD_TO_USERS].m_Func = &PROCESS_EXIT_PROCESS::PROCESS_SendBRDToUser;
	m_FUNC[PROC_POST_WAIT_DELAY_TIME].m_Func = &PROCESS_EXIT_PROCESS::PROCESS_PostWaitDelayTime;
	m_FUNC[PROC_POST_NETWORK_BLOCK_TIME].m_Func = &PROCESS_EXIT_PROCESS::PROCESS_PostNetworkBlockTime;
	m_FUNC[PROC_COMPLETE].m_Func = &PROCESS_EXIT_PROCESS::PROCESS_Complete;

	m_pExitREF->m_ProcessStatus = cEXITINFO_OF_AGENTSERVER::STARTED;

	DWORD dwWaitTime = m_pExitREF->TotalDelayTime();
	dwWaitTime >>= 3;	// time/8
	if( dwWaitTime < cEXITINFO_OF_AGENTSERVER::MIN_TOTAL_DELAY_TIME )
		dwWaitTime = cEXITINFO_OF_AGENTSERVER::MIN_TOTAL_DELAY_TIME;
	if( m_pExitREF->TotalDelayTime() > dwWaitTime )
		m_pExitREF->TotalDelayTime() -= dwWaitTime;
	m_pExitREF->NetworkBlockTime() = dwWaitTime;

	return TRUE;
}

BOOL	PROCESS_EXIT_PROCESS::DoProcess()
{
	if( PROCESSED == m_Status ) //���⿡ �ɸ� ���� ��������... �ɸ��� ����ó������.
		return TRUE;

	if( m_Timeout.IsExpired() )
	{
		ProcessFunc func = m_FUNC[m_Status].m_Func;
		(this->*func)();
	}

	return TRUE;
}

BOOL	PROCESS_EXIT_PROCESS::IsComplete()
{
	if( PROCESSED == m_Status )
	{
		m_pExitREF->m_ProcessStatus = cEXITINFO_OF_AGENTSERVER::EXITED;
		return TRUE;
	}

	return FALSE;
}

BOOL	PROCESS_EXIT_PROCESS::DoRelease()
{
	//m_pExitREF->m_ProcessStatus = cEXITINFO_OF_AGENTSERVER::DISABLED;
	return TRUE;
}

VOID	PROCESS_EXIT_PROCESS::OnMsg( sSIG_MSG* pSIG_MSG )
{
    __UNUSED(pSIG_MSG);
}

//////////////////////////////////////////////////////////////////////////

VOID	PROCESS_EXIT_PROCESS::PROCESS_SendBRDToUser()
{
	DWORD dwTotalDelay = m_pExitREF->m_TotalDelay;
	// �������� ����!
	MSG_CG_CONNECTION_SERVER_SHUTDOWN_BRD msgBRD;
	msgBRD.m_dwStringCode		= 0;
	msgBRD.m_byShutdownTime		= (BYTE)(dwTotalDelay/1000);
    UserManager::Instance()->SendMessageToAllUsers(&msgBRD, sizeof(msgBRD));

	m_Timeout.SetNextTimeoutByCurTickBased( dwTotalDelay );
	ChangeNextStatus();
}

//==================================================================================================
namespace ns_functor {
;
class UserDisconnectControl : public ns_functor::IUserForeachFunctor
{
public:
    UserDisconnectControl() {}
    ~UserDisconnectControl() {}
    virtual bool operator()(User* const user)
    {
        user->SetDisconnectCode(RC::RC_DISCONNECT_USER_RELEASE);
        user->DisconnectUser();
        return true;
    }
};

}; //end of namespace
//==================================================================================================

VOID	PROCESS_EXIT_PROCESS::PROCESS_PostWaitDelayTime()
{
	// �� ��ƾ�� ���� ���� ���� ���� ��� �ð��� �Ϸ�� ���´�.
	m_pExitREF->m_ProcessStatus = cEXITINFO_OF_AGENTSERVER::PROCESSING;

	AgentServer::GetInstance()->StartNetworkBlock4Closing();

	DWORD dwDelay = m_pExitREF->NetworkBlockTime();
	m_Timeout.SetNextTimeoutByCurTickBased( dwDelay );
	ChangeNextStatus();
}

//class ServerDisconnectControl
//{
//public:
//	ServerDisconnectControl() {}
//	~ServerDisconnectControl() {}
//	VOID operator() ( ServerSessionEx* pSession )
//	{
//		pSession->Disconnect();
//	}
//};


VOID	PROCESS_EXIT_PROCESS::PROCESS_PostNetworkBlockTime()
{
    ns_functor::UserDisconnectControl disconnector;
    UserManager::Instance()->Foreach(&disconnector);

	// Network Update�ð� ���
	m_Timeout.SetNextTimeoutByCurTickBased( 1000 );
	ChangeNextStatus();
}

VOID	PROCESS_EXIT_PROCESS::PROCESS_Complete()
{
	// Network block time ���� ó��
	ChangeNextStatus();
}

//VOID	PROCESS_EXIT_PROCESS::PROCESS_DisconnectGameServers()
//{
//	ServerDisconnectControl op;
//	ServerSessionManager::Instance()->Foreach( op );
//	ChangeNextStatus();
//}




