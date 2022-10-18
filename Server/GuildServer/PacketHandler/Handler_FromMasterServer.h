#pragma once

#include <PacketStruct_WZ.h>
#include "PacketHandler.h"

// <PROTOTYPEs>
class MasterServerSession;

//-------------------------------------------------------------------------------------------------
#define Handler_FromMasterServer_DECL( p )	static VOID On##p( MasterServerSession* pServerSession, MSG_BASE* pMsg, WORD wSize )
#define Handler_FromMasterServer_IMPL( p )	VOID Handler_FromMasterServer::On##p( MasterServerSession* pServerSession, MSG_BASE* pMsg, WORD wSize )
//-------------------------------------------------------------------------------------------------

class Handler_FromMasterServer
{
public:
	static VOID		RegisterHandler_ALL( PACKET_HANDLER_DELEGATOR pInstance );
private:
	static sPACKET_HANDLER_INFO*	GetHandlerInfo();
private:

	Handler_FromMasterServer_DECL( MZ_SERVER_INFO_CMD );
	Handler_FromMasterServer_DECL( SERVERCOMMON_SERVERSHUTDOWN_REQ );

#ifdef __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
	Handler_FromMasterServer_DECL( MZ_CONNECTION_DISCONNECT_CMD );
	Handler_FromMasterServer_DECL( MZ_CONNECTION_PHASE_SHIFT_CMD );
	Handler_FromMasterServer_DECL( MZ_CONNECTION_BLOCK_CMD );

	Handler_FromMasterServer_DECL( SERVERCOMMON_DATA_TRANSFER_PLZ_GIVEME_DATA_ACK );
	Handler_FromMasterServer_DECL( SERVERCOMMON_DATA_TRANSFER_PLZ_GIVEME_DATA_NAK );
	Handler_FromMasterServer_DECL( SERVERCOMMON_DATA_TRANSFER_SENDDATA_CMD );
#endif //__WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
    //__NA001254_20090109_RTTG_FUNC_ADD
	Handler_FromMasterServer_DECL( MZ_GUILD_NAME_CHANGE_SYN );
};
