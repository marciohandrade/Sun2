#pragma once

struct MSG_BASE;
class MasterServerSession;

//-------------------------------------------------------------------------------------------------
// __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__

#define HandlerFromMasterServer_DECL( fnName )		\
	static VOID On##fnName( MasterServerSession* pServerSession, MSG_BASE* pMsg, WORD wSize )

#define HandlerFromMasterServer_IMPL( fnName )		\
	VOID Handler_MW::On##fnName( MasterServerSession* pServerSession, MSG_BASE* pMsg, WORD wSize )

//-------------------------------------------------------------------------------------------------


class Handler_MW
{
public:
    HandlerFromMasterServer_DECL(MW_NOTICE_CMD);
    HandlerFromMasterServer_DECL(MW_SERVER_INFO_CMD);
    HandlerFromMasterServer_DECL(MW_BATTLEZONEINFO_REQ);
    HandlerFromMasterServer_DECL(SERVERCOMMON_SERVERSHUTDOWN_REQ);
    //-------------------------------------------------------------------------------------------------
#ifdef __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
    HandlerFromMasterServer_DECL( MW_CONNECTION_DISCONNECT_CMD );
    HandlerFromMasterServer_DECL( MW_CONNECTION_PHASE_SHIFT_CMD );
    HandlerFromMasterServer_DECL( MW_CONNECTION_BLOCK_CMD );
    //
    //HandlerFromMasterServer_DECL( MW_DATA_TRANSFER_PLZ_GIVEME_DATA_SYN );
    HandlerFromMasterServer_DECL( SERVERCOMMON_DATA_TRANSFER_PLZ_GIVEME_DATA_ACK );
    HandlerFromMasterServer_DECL( SERVERCOMMON_DATA_TRANSFER_PLZ_GIVEME_DATA_NAK );
    HandlerFromMasterServer_DECL( SERVERCOMMON_DATA_TRANSFER_SENDDATA_CMD );
    //HandlerFromMasterServer_DECL( MW_DATA_TRANSFER_SENDDATA_ACK );
    //HandlerFromMasterServer_DECL( MW_DATA_TRANSFER_SENDDATA_NAK );
#endif //__WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
    //__NA001254_20090109_RTTG_FUNC_ADD
    HandlerFromMasterServer_DECL( MW_MULTIMEMO_SEND_SYN );
//-------------------------------------------------------------------------------------------------
};
