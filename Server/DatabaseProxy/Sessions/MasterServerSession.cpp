#include "stdafx.h"
#include ".\masterserversession.h"
#include "PacketHandler/PacketHandler.h"
#include "Protocol_DM.h"
#include "PacketStruct_DM.h"
#include "QueryObjects/DBHandler.h"

__IMPL_CUSTOMPOOL_PTR( MasterServerSession )
MasterServerSession::MasterServerSession(void)
{
}

MasterServerSession::~MasterServerSession(void)
{
}

// CHANGES: f110617.1L
void MasterServerSession::OnRedirect(DWORD network_index, MSG_SERVER_TYPE* const first_contact_msg)
{
    ChildServerSession::OnRedirect(network_index, first_contact_msg);
}

VOID MasterServerSession::OnRecv( BYTE * pMsg, WORD wSize )
{
	PacketHandler::Instance()->ParsePacket( PACKET_HANDLER_MASTER, this, (MSG_BASE *)pMsg, wSize );
}

VOID MasterServerSession::OnConnect( BOOL bSuccess, DWORD dwNetworkIndex )
{
	ChildServerSession::OnConnect( bSuccess, dwNetworkIndex );

	if( bSuccess )
	{
		db_unit_id_ = g_DBHandler.AllocateDBUnit( GetServerType(), GetSessionIndex() );
		ASSERT( 0 != db_unit_id_ );
		SetSyncKey( MASTER_SERVER );
	}
}

VOID MasterServerSession::OnDisconnect()
{
	ChildServerSession::OnDisconnect();

#if DBPROXY_TYPE_SELECTOR == DBPROXY_TYPE_DEFAULT
	#ifdef __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
		ScriptDataManager::ReRegisterReloadDataList();
	#endif //__WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
#endif
	g_DBProxyServer.SetPhaseStatus( PHASE_UNKNOWN );
	MessageOut(eCRITICAL_LOG,  "Disconnected from master server." );
}

