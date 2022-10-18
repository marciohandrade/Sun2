#include "StdAfx.h"
#include ".\MasterServerSession.h"
#include <PacketHandler/PacketHandler.h>
#include <GuildServer.h>

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

VOID	MasterServerSession::OnConnect( BOOL bSuccess, DWORD dwSessionIndex )
{
	ServerSession::OnConnect( bSuccess, dwSessionIndex );

	if( bSuccess )
	{
		SetSyncKey( MASTER_SERVER );
	}
}

VOID	MasterServerSession::OnDisconnect()
{
	ServerSessionEx::OnDisconnect();

#ifdef __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
	ScriptDataManager::ReRegisterReloadDataList();
#endif
	GuildServer::GetMainFrame()->SetPhaseStatus( PHASE_UNKNOWN );
}

