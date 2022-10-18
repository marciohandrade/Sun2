#pragma once

#include "./PacketHandler.h"

#define Handler_FromSolarAuthServer_DECL( p )	static VOID On##p( ChildServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
#define Handler_FromSolarAuthServer_IMPL( p )	VOID Handler_FromSolarAuthServer::On##p( ChildServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )

class ChildServerSession;
class Handler_FromSolarAuthServer
{
public:
	Handler_FromSolarAuthServer(void){}
	~Handler_FromSolarAuthServer(void){}
    // NOTE: f110518.2L
    static const nsPacket::PacketHandlerInfo GetPacketHandlerNodes();


	Handler_FromSolarAuthServer_DECL(SX_CONNECTION_REQ_SERVER_INFO_SYN);
	Handler_FromSolarAuthServer_DECL(SX_CONNECTION_REQ_SERVER_INFO_DBR);
//	Handler_FromSolarAuthServer_DECL(SERVERCOMMON_SERVERSHUTDOWN_REQ);
	Handler_FromSolarAuthServer_DECL(SERVERCOMMON_HEARTBEAT);

////-------------------------------------------------------------------------------------------------
//#ifdef __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
//	Handler_FromSolarAuthServer_DECL( DM_CONNECTION_DISCONNECT_CMD );
//	Handler_FromSolarAuthServer_DECL( DM_CONNECTION_PHASE_SHIFT_CMD );
	Handler_FromSolarAuthServer_DECL( SX_CONNECTION_BLOCK_CMD );
//	//
//	//Handler_FromSolarAuthServer_DECL( AM_DATA_TRANSFER_PLZ_GIVEME_DATA_SYN );
//	Handler_FromSolarAuthServer_DECL( SERVERCOMMON_DATA_TRANSFER_PLZ_GIVEME_DATA_ACK );
//	Handler_FromSolarAuthServer_DECL( SERVERCOMMON_DATA_TRANSFER_PLZ_GIVEME_DATA_NAK );
//	Handler_FromSolarAuthServer_DECL( SERVERCOMMON_DATA_TRANSFER_SENDDATA_CMD );
//	//Handler_FromSolarAuthServer_DECL( AM_DATA_TRANSFER_SENDDATA_ACK );
//	//Handler_FromSolarAuthServer_DECL( AM_DATA_TRANSFER_SENDDATA_NAK );
//#endif //__WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
////-------------------------------------------------------------------------------------------------


	static VOID ExecuteServerInfoQuery( ChildServerSession * pServer, BYTE Page );
};
