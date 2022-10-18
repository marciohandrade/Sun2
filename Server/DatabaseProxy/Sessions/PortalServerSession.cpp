#include "stdafx.h"
#include "./PortalServerSession.h"
#if !defined(_NA002635_GP_DP_PROTOCOL_UINIFICATION)
#include ".\PacketHandler.h"

__IMPL_CUSTOMPOOL_PTR( PortalServerSession )
PortalServerSession::PortalServerSession(void)
{
}

PortalServerSession::~PortalServerSession(void)
{
}

VOID PortalServerSession::OnRecv( BYTE * pMsg, WORD wSize )
{
	PacketHandler::Instance()->ParsePacket( PACKET_HANDLER_PORTAL, this, (MSG_BASE *)pMsg, wSize );
}

#endif //if !defined(_NA002635_GP_DP_PROTOCOL_UINIFICATION)