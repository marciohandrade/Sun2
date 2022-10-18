#include "stdafx.h"

#include <AgentServer.h>
#include <ServerSessions/MasterServerSession.h>
#include <PacketHandler/PacketHandler.h>


MasterServerSession::MasterServerSession(void)
{
}

MasterServerSession::~MasterServerSession(void)
{
}

// CHANGES: f110617.1L
void MasterServerSession::OnRedirect(DWORD network_index, MSG_SERVER_TYPE* const first_contact_msg)
{
    ServerSessionEx::OnRedirect(network_index, first_contact_msg);
}

VOID MasterServerSession::OnConnect( BOOL bSuccess, DWORD dwSessionIndex )
{
	ServerSessionEx::OnConnect( bSuccess, dwSessionIndex );
	if( bSuccess )
	{
		SetSyncKey( MASTER_SERVER );
		if( g_pSecureLog )
			g_pSecureLog->RegisterSession( this );
	}
}

VOID MasterServerSession::OnDisconnect()
{
	ServerSessionEx::OnDisconnect();

	SetSyncKey( TEMP_SERVER );
	if( g_pSecureLog )
		g_pSecureLog->RegisterSession( this );

#ifdef __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
	ScriptDataManager::ReRegisterReloadDataList();
#endif //__WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__

	AgentServer::GetInstance()->SetPhaseStatus( PHASE_UNKNOWN );
}


