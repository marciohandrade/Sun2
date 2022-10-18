#pragma once

class ServerSession;
struct MSG_BASE;

#define Handler_AgentServer_DECL( p )			static VOID On##p( ServerSession * pServerSession, MSG_BASE * pRecvMsg, WORD wSize )
#define Handler_AgentServer_IMPL( p )			VOID Handler_AgentServer::On##p( ServerSession * pServerSession, MSG_BASE * pRecvMsg, WORD wSize )

class Handler_AgentServer
{
public:
	Handler_AgentServer(void){}
	~Handler_AgentServer(void){}
};
