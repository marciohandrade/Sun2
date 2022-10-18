#pragma once

class ServerSession;
struct MSG_BASE;

#define Handler_DBProxy_DECL( p )			static VOID On##p( ServerSession * pServerSession, MSG_BASE * pRecvMsg, WORD wSize )
#define Handler_DBProxy_IMPL( p )			VOID Handler_DBProxy::On##p( ServerSession * pServerSession, MSG_BASE * pRecvMsg, WORD wSize )

class Handler_DBProxy
{
public:
	Handler_DBProxy(void){}
	~Handler_DBProxy(void){}


	Handler_DBProxy_DECL(DP_RANKING_INFO_REQ_ACK);
	Handler_DBProxy_DECL(DP_RANKING_INFO_REQ_NAK);
	Handler_DBProxy_DECL(DP_RANKING_INFO_REQ_END_NTF);

	Handler_DBProxy_DECL(DP_RANKING_MISSION_CLEAR_ACK);
	Handler_DBProxy_DECL(DP_RANKING_MISSION_CLEAR_NAK);
	Handler_DBProxy_DECL(DP_RANKING_MISSION_REWARD_ACK);
	Handler_DBProxy_DECL(DP_RANKING_MISSION_REWARD_NAK);

	Handler_DBProxy_DECL(DP_RANKING_REGISTER_MISSION_POINT_ACK);
	Handler_DBProxy_DECL(DP_RANKING_REGISTER_MISSION_POINT_NAK);
};
