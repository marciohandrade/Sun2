#include "stdafx.h"
#include ".\guildserversession.h"
#if !defined(_NA002635_GZ_DZ_PROTOCOL_UINIFICATION)
#include "PacketHandler/PacketHandler.h"

__IMPL_CUSTOMPOOL_PTR( GuildServerSession )
GuildServerSession::GuildServerSession(void)
{
}

GuildServerSession::~GuildServerSession(void)
{
}

// CHANGES: f110617.1L
void GuildServerSession::OnRedirect(DWORD network_index, MSG_SERVER_TYPE* const first_contact_msg)
{
    ServerSession::OnRedirect(network_index, first_contact_msg);
}

VOID GuildServerSession::OnConnect( BOOL bSuccess, DWORD dwSessionIndex )
{
	ServerSession::OnConnect( bSuccess, dwSessionIndex );

	if( bSuccess )
	{
		SUNLOG( eFULL_LOG,  "[GuildServerSession::OnConnect] Connected to Guild Server." );
	}
	else
	{
	}
}

VOID GuildServerSession::OnDisconnect()
{
	printf( "Disconnected from Guild Server." );

	ServerSession::OnDisconnect();
}

VOID GuildServerSession::OnRecv( BYTE *pMsg, WORD wSize )
{
	PacketHandler::Instance()->ParsePacket( PACKET_HANDLER_GUILDDB, this, (MSG_BASE *)pMsg, wSize );
}
#endif //if !defined(_NA002635_GZ_DZ_PROTOCOL_UINIFICATION)