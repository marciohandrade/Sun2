#include "StdAfx.h"
#include ".\worldserversession.h"
#include "PacketHandler/PacketHandler.h"

__IMPL_CUSTOMPOOL_PTR( WorldServerSession )
WorldServerSession::WorldServerSession(void)
{
}

WorldServerSession::~WorldServerSession(void)
{
}

// CHANGES: f110617.1L
void WorldServerSession::OnRedirect(DWORD network_index, MSG_SERVER_TYPE* const first_contact_msg)
{
    ChildServerSession::OnRedirect(network_index, first_contact_msg);
}

VOID WorldServerSession::OnRecv( BYTE * pMsg, WORD wSize )
{
	PacketHandler::Instance()->ParsePacket( PACKET_HANDLER_WORLD, this, (MSG_BASE *)pMsg, wSize );
}


