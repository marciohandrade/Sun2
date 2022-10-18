#include "StdAfx.h"
#include ".\AgentServerSession.h"
#include <PacketHandler/PacketHandler.h>

AgentServerSession::AgentServerSession(void)
{
}

AgentServerSession::~AgentServerSession(void)
{
}

// CHANGES: f110617.1L
void AgentServerSession::OnRedirect(DWORD network_index, MSG_SERVER_TYPE* const first_contact_msg)
{
    ServerSessionEx::OnRedirect(network_index, first_contact_msg);
}
