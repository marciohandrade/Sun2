#include "StdAfx.h"
#include ".\masterserversession.h"
#include "PacketHandler.h"
#include "PortalServer.h"

__IMPL_CUSTOMPOOL_PTR( MasterServerSession )
MasterServerSession::MasterServerSession(void)
{
}

MasterServerSession::~MasterServerSession(void)
{
}

VOID MasterServerSession::OnRecv( BYTE *pMsg, WORD wSize )
{
	PacketHandler::Instance()->ParsePacket( PACKET_HANDLER_MASTER, this, (MSG_BASE *)pMsg, wSize );
}

VOID MasterServerSession::Init()
{
	ServerSession::Init();
	SetSyncKey( TEMP_SERVER );
}

VOID MasterServerSession::Release()
{
}


VOID MasterServerSession::OnConnect( BOOL bSuccess, DWORD dwSessionIndex )
{
	ServerSession::OnConnect( bSuccess, dwSessionIndex );
	if( bSuccess )
	{
		SetSyncKey( MASTER_SERVER );
	}
}


VOID MasterServerSession::OnDisconnect()
{
	ServerSession::OnDisconnect();

#ifdef __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
	ScriptDataManager::ReRegisterReloadDataList();
#endif
	g_PortalServer.SetPhaseStatus( PHASE_UNKNOWN );
}


