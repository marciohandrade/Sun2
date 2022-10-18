#pragma once

#include "./PacketHandler.h"

#if !defined(_NA002635_GP_DP_PROTOCOL_UINIFICATION)

#define Handler_FromPortalServer_DECL( p )	static VOID On##p( ChildServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
#define Handler_FromPortalServer_IMPL( p )	VOID Handler_FromPortalServer::On##p( ChildServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )

class ChildServerSession;
class Handler_FromPortalServer
{
public:
	Handler_FromPortalServer(void){}
	~Handler_FromPortalServer(void){}
    // NOTE: f110518.2L
    static const nsPacket::PacketHandlerInfo GetPacketHandlerNodes();

	Handler_FromPortalServer_DECL( DP_RANKING_INFO_REQ_SYN );
	Handler_FromPortalServer_DECL( DP_RANKING_INFO_REQ_DBR );

	Handler_FromPortalServer_DECL( DP_RANKING_MISSION_CLEAR_SYN );
	Handler_FromPortalServer_DECL( DP_RANKING_MISSION_CLEAR_DBR );

	Handler_FromPortalServer_DECL( DP_RANKING_MISSION_REWARD_SYN );
	Handler_FromPortalServer_DECL( DP_RANKING_MISSION_REWARD_DBR );

	Handler_FromPortalServer_DECL( DP_RANKING_REGISTER_MISSION_POINT_SYN );
	Handler_FromPortalServer_DECL( DP_RANKING_REGISTER_MISSION_POINT_DBR );
};

#endif //if !defined(_NA002635_GP_DP_PROTOCOL_UINIFICATION)