#pragma once
#include <Protocol_ServerCommon.h>

class ServerSession;
struct MSG_BASE;

#define Handler_MasterServer_DECL( p )			static VOID On##p( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
#define Handler_MasterServer_IMPL( p )			VOID Handler_MasterServer::On##p( ServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )


class Handler_MasterServer
{
public:
	Handler_MasterServer(void){}
	~Handler_MasterServer(void){}

	Handler_MasterServer_DECL(MP_CONNECTION_SERVER_INFO_CMD);
	Handler_MasterServer_DECL(SERVERCOMMON_SERVERSHUTDOWN_REQ);

#ifdef __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
	Handler_MasterServer_DECL( MP_CONNECTION_DISCONNECT_CMD );
	Handler_MasterServer_DECL( MP_CONNECTION_PHASE_SHIFT_CMD );
	Handler_MasterServer_DECL( MP_CONNECTION_BLOCK_CMD );

	Handler_MasterServer_DECL( SERVERCOMMON_DATA_TRANSFER_PLZ_GIVEME_DATA_ACK );
	Handler_MasterServer_DECL( SERVERCOMMON_DATA_TRANSFER_PLZ_GIVEME_DATA_NAK );
	Handler_MasterServer_DECL( SERVERCOMMON_DATA_TRANSFER_SENDDATA_CMD );
#endif //__WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
};
