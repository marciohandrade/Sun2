#include "stdafx.h"
#include "MasterServerSession.h"
#include "PacketHandler/PacketHandler.h"
#include "GameServerEx.h"
#include <PacketStruct_GM.h>
#include <Protocol_GM.h>

__IMPL_CUSTOMPOOL_PTR( MasterServerSession )
MasterServerSession::MasterServerSession()
{
}

MasterServerSession::~MasterServerSession()
{
}

// CHANGES: f110617.1L
void MasterServerSession::OnRedirect(DWORD network_index, MSG_SERVER_TYPE* const first_contact_msg)
{
    // the RemoteServerSession that is base class has not OnRedirect
    ServerSession::OnRedirect(network_index, first_contact_msg);
}

VOID MasterServerSession::OnConnect( BOOL bSuccess, DWORD dwSessionIndex )
{
	RemoteServerSession::OnConnect( bSuccess, dwSessionIndex );

	if( bSuccess )
	{
		SUNLOG( eFULL_LOG,  "[MasterServerSession::OnConnect] Connected to master server." );
		g_pSecureLog->RegisterSession( this );
		SetSyncKey( MASTER_SERVER );
	}
	else
	{
		//SUNLOG( eFULL_LOG,  "Can't connect to master server." );
	}
}

VOID MasterServerSession::OnDisconnect()
{
	if( g_pSecureLog )
		g_pSecureLog->RegisterSession( NULL );

	DISPMSG( "Disconnected from master server.\n" );

	RemoteServerSession::OnDisconnect();

#ifdef __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
	ScriptDataManager::ReRegisterReloadDataList();
#endif //__WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__

	g_pGameServer->SetPhaseStatus( PHASE_UNKNOWN );
}

VOID MasterServerSession::OnRecv( BYTE *pMsg, WORD wSize )
{
	PacketHandler::Instance()->ParsePacket( PACKET_HANDLER_MASTER, this, (MSG_BASE *)pMsg, wSize );
}
