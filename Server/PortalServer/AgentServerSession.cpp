#include "StdAfx.h"
#include ".\agentserversession.h"
#include "PacketHandler.h"

__IMPL_CUSTOMPOOL_PTR( AgentServerSession )
AgentServerSession::AgentServerSession(void)
{
}

AgentServerSession::~AgentServerSession(void)
{
}

VOID AgentServerSession::OnRecv( BYTE *pMsg, WORD wSize )
{
	PacketHandler::Instance()->ParsePacket( PACKET_HANDLER_AGENT, this, (MSG_BASE *)pMsg, wSize );
}