#pragma once

#include "./PacketHandler.h"

#define Handler_FromMasterServer_DECL( p )	static VOID On##p( ChildServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )
#define Handler_FromMasterServer_IMPL( p )	VOID Handler_FromMasterServer::On##p( ChildServerSession * pServerSession, MSG_BASE * pMsg, WORD wSize )

class ChildServerSession;
class Handler_FromMasterServer
{
public:
	Handler_FromMasterServer(void){}
	~Handler_FromMasterServer(void){}
    // NOTE: f110518.2L
    static const nsPacket::PacketHandlerInfo GetPacketHandlerNodes();

    Handler_FromMasterServer_DECL(DM_CONNECTION_GROUND_CMD);
	Handler_FromMasterServer_DECL(DM_CONNECTION_SERVER_INFO_CMD);
	Handler_FromMasterServer_DECL(MX_CONNECTION_REQ_SERVER_INFO_SYN);
	Handler_FromMasterServer_DECL(MX_CONNECTION_REQ_SERVER_INFO_DBR);
	Handler_FromMasterServer_DECL(SERVERCOMMON_SERVERSHUTDOWN_REQ);
	Handler_FromMasterServer_DECL(SERVERCOMMON_HEARTBEAT);

	Handler_FromMasterServer_DECL(MX_FUNCRESTRICT_RELOAD_REQ);
	Handler_FromMasterServer_DECL(MX_FUNCRESTRICT_RELOAD_DBR);
#ifdef _NA002635_GZ_DZ_PROTOCOL_UINIFICATION_IN_DBP
    //__NA001254_20090109_RTTG_FUNC_ADD
    Handler_FromMasterServer_DECL(MZ_GUILD_NAME_CHANGE_SYN);
#endif
//-------------------------------------------------------------------------------------------------
#ifdef __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
	Handler_FromMasterServer_DECL( DM_CONNECTION_DISCONNECT_CMD );
	Handler_FromMasterServer_DECL( DM_CONNECTION_PHASE_SHIFT_CMD );
	Handler_FromMasterServer_DECL( DM_CONNECTION_BLOCK_CMD );
	//
	//Handler_FromMasterServer_DECL( AM_DATA_TRANSFER_PLZ_GIVEME_DATA_SYN );
	Handler_FromMasterServer_DECL( SERVERCOMMON_DATA_TRANSFER_PLZ_GIVEME_DATA_ACK );
	Handler_FromMasterServer_DECL( SERVERCOMMON_DATA_TRANSFER_PLZ_GIVEME_DATA_NAK );
	Handler_FromMasterServer_DECL( SERVERCOMMON_DATA_TRANSFER_SENDDATA_CMD );
	//Handler_FromMasterServer_DECL( AM_DATA_TRANSFER_SENDDATA_ACK );
	//Handler_FromMasterServer_DECL( AM_DATA_TRANSFER_SENDDATA_NAK );
#endif //__WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
//-------------------------------------------------------------------------------------------------


	static VOID ExecuteServerInfoQuery( ChildServerSession * pServer, BYTE Page );
	static VOID ExecuteRestrictInfoQuery( ChildServerSession * pServer );

};
