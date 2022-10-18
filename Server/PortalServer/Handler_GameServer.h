#pragma once

class ServerSession;
struct MSG_BASE;

#define Handler_GameServer_DECL( p )			static VOID On##p( ServerSession * pServerSession, MSG_BASE * pRecvMsg, WORD wSize )
#define Handler_GameServer_IMPL( p )			VOID Handler_GameServer::On##p( ServerSession * pServerSession, MSG_BASE * pRecvMsg, WORD wSize )

class Handler_GameServer
{
public:
	Handler_GameServer(void){}
	~Handler_GameServer(void){}


	Handler_GameServer_DECL(GP_RANKING_INFO_REQ_SYN);
	Handler_GameServer_DECL(GP_RANKING_MISSION_CLEAR_SYN);
	Handler_GameServer_DECL(GP_RANKING_MISSION_REWARD_SYN);

	Handler_GameServer_DECL(GP_RANKING_SETTLE_CMD);

	Handler_GameServer_DECL(GP_RANKING_REGISTER_MISSION_POINT_SYN);

	Handler_GameServer_DECL(GP_MISSION_RANKING_SETTLE_DATE_SYN);
};
