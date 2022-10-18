#include "stdafx.h"

#include <AgentServer.h>
#include <ServerSessions/DBProxyServerSession.h>
#include <ServerSessions/ServerSessionManager.h>
#include <PacketHandler/PacketHandler.h>


DBProxyServerSession::DBProxyServerSession()
{
}

DBProxyServerSession::~DBProxyServerSession()
{
}

// CHANGES: f110617.1L
void DBProxyServerSession::OnRedirect(DWORD network_index, MSG_SERVER_TYPE* const first_contact_msg)
{
    ServerSessionEx::OnRedirect(network_index, first_contact_msg);
}

VOID DBProxyServerSession::OnConnect( BOOL bSuccess, DWORD dwNetworkIndex )
{
	ServerSessionEx::OnConnect( bSuccess, dwNetworkIndex );
	if( bSuccess )
	{
		AgentServer* pAgentServer = AgentServer::GetInstance();
		if( SERVER_ABNORMAL_DISCONNECTED == pAgentServer->GetDBProxyServerState() )
		{
			//sendAllUsersInfo();		
		}
		pAgentServer->StartServerSideListen();
	}
}

VOID DBProxyServerSession::OnDisconnect()
{
	// DBproxy�� ������ ��������, Agent�� Field�� BattleServer�� ������ ��� �ش� �۾��� ó���Ѵ�.
	ServerSessionManager::Instance()->DisconnectAllServer();
	// �������� DB Proxy ����
	AgentServer::GetInstance()->SetDBProxyServerState( SERVER_ABNORMAL_DISCONNECTED );

	ServerSessionEx::OnDisconnect();
    // NOTE: f110427.5L, do shutdown process if a handler disconnected with odbc.
    AgentServer::GetInstance()->SERVER_EXIT.StartServerExitProcess();
    SUNLOG(eCRITICAL_LOG,
           _T("|["__FUNCTION__"]|Msg=db proxy server disconnected, close agent server|"));
}

VOID DBProxyServerSession::SendAllUsersInfo()
{
}
