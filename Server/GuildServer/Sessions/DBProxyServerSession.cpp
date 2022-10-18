#include "StdAfx.h"
#include ".\dbproxyserversession.h"
#include <PacketHandler/PacketHandler.h>
#include <GuildServer.h>

DBProxyServerSession::DBProxyServerSession(void)
{
}

DBProxyServerSession::~DBProxyServerSession(void)
{
}

// CHANGES: f110617.1L
void DBProxyServerSession::OnRedirect(DWORD network_index, MSG_SERVER_TYPE* const first_contact_msg)
{
    ServerSessionEx::OnRedirect(network_index, first_contact_msg);
}
