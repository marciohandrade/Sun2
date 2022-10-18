#include "StdAfx.h"
#include ".\WorldServerSession.h"
#include <PacketHandler/PacketHandler.h>
#include <GuildServer.h>

WorldServerSession::WorldServerSession(void)
{
}

WorldServerSession::~WorldServerSession(void)
{
}

// CHANGES: f110617.1L
void WorldServerSession::OnRedirect(DWORD network_index, MSG_SERVER_TYPE* const first_contact_msg)
{
    ServerSessionEx::OnRedirect(network_index, first_contact_msg);
}

