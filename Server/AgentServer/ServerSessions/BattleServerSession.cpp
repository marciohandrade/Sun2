#include "stdafx.h"

#include <AgentServer.h>
#include <ServerSessions/BattleServerSession.h>
#include <PacketHandler/PacketHandler.h>


BattleServerSession::BattleServerSession()
{
}

BattleServerSession::~BattleServerSession()
{
}

VOID BattleServerSession::Init()
{
	ServerSessionEx::Init();
	m_dwNumberOfRooms = 0;
}

// CHANGES: f110617.1L
void BattleServerSession::OnRedirect(DWORD network_index, MSG_SERVER_TYPE* const first_contact_msg)
{
    ServerSessionEx::OnRedirect(network_index, first_contact_msg);
    // CHANGES: f110615.2L, added to solve high-load network bottleneck problem
    first_contact_msg->m_byCategory = AG_CONNECTION;
    first_contact_msg->m_byProtocol = AG_CONNECTION_GROUND_CMD;
    this->UseSendPendingBuffer(2 * 1024 * 1024);
    this->UseRecvDualBuffer(10 * 1024 * 1024);
}


