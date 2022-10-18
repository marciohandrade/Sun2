#include "stdafx.h"

#if !defined(_NA002635_GZ_DZ_PROTOCOL_UINIFICATION)

#include ".\guildserversession.h"
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
    ChildServerSession::OnRedirect(network_index, first_contact_msg);
}


VOID GuildServerSession::OnRecv( BYTE * pMsg, WORD wSize )
{
	PacketHandler::Instance()->ParsePacket( PACKET_HANDLER_GUILD, this, (MSG_BASE *)pMsg, wSize );
}

#endif //_NA002635_GZ_DZ_PROTOCOL_UINIFICATION