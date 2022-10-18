#include "stdafx.h"
#include "./PortalServerSession.h"

#if !defined(_NA002635_GP_DP_PROTOCOL_UINIFICATION)

#include "PacketHandler/PacketHandler.h"
#include ".\GameRankingManager.h"

__IMPL_CUSTOMPOOL_PTR( PortalServerSession )
PortalServerSession::PortalServerSession(void)
{
}

PortalServerSession::~PortalServerSession(void)
{
}


VOID PortalServerSession::OnConnect( BOOL bSuccess, DWORD dwSessionIndex )
{
	ServerSession::OnConnect( bSuccess, dwSessionIndex );

	if( bSuccess )
	{
		SUNLOG( eFULL_LOG,  "[PortalServerSession::OnConnect] Connected to Portal Server." );
		g_GameRankingMgr.Initialize();
	}
	else
	{
	}
}

VOID PortalServerSession::OnDisconnect()
{
	printf( "Disconnected from Portal Server." );
    // CHANGES: f110511.1L, shared logic, _NA002635_GP_DP_PROTOCOL_UINIFICATION
    g_GameRankingMgr.OnReceivedStatusControl(eRANKING_STATUS_INIT);

	ServerSession::OnDisconnect();
}

VOID PortalServerSession::OnRecv( BYTE *pMsg, WORD wSize )
{
	PacketHandler::Instance()->ParsePacket( PACKET_HANDLER_RANKING, this, (MSG_BASE *)pMsg, wSize );
}

#endif //if !defined(_NA002635_GP_DP_PROTOCOL_UINIFICATION)