#pragma once

#include <PacketStruct_AM.h>
#include "PacketHandler.h"

// <PROTOTYPEs>
class MasterServerSession;

//-------------------------------------------------------------------------------------------------

#define HANDLER_AM_DECL( fnName )		\
	static VOID On##fnName( MasterServerSession* pServerSession, MSG_BASE* pMsg, WORD wSize )

#define HANDLER_AM_IMPL( fnName )		\
	VOID Handler_AM::On##fnName( MasterServerSession* pServerSession, MSG_BASE* pMsg, WORD wSize )

//-------------------------------------------------------------------------------------------------

class Handler_AM
{
public:
	static VOID		RegisterHandler_ALL( PACKET_HANDLER_DELEGATOR pInstance );
private:
	static sPACKET_HANDLER_INFO*	GetHandlerInfo();

public:
	//------------------------------------------------------------------------------
	// <PACKET HANDLERs>
	// Declaration & Implementation
	//
    HANDLER_AM_DECL(AM_CONNECTION_GROUND_CMD);
	HANDLER_AM_DECL(AM_OPERATION_CHAR_DISCONNECT_SYN);
	HANDLER_AM_DECL(AM_OPERATION_CHAR_DISCONNECT_ACK);
	HANDLER_AM_DECL(AM_OPERATION_LOGIC_VIOLATION_DISCONNECT_CMD);

	HANDLER_AM_DECL(AM_CONNECTION_SERVER_INFO_CMD);
	HANDLER_AM_DECL(SERVERCOMMON_SERVERSHUTDOWN_REQ);

	HANDLER_AM_DECL( AM_CONNECTION_PHASE_SHIFT_CMD );


#ifdef __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
	//
	//HANDLER_AM_DECL( AM_DATA_TRANSFER_PLZ_GIVEME_DATA_SYN );
	HANDLER_AM_DECL( SERVERCOMMON_DATA_TRANSFER_PLZ_GIVEME_DATA_ACK );
	HANDLER_AM_DECL( SERVERCOMMON_DATA_TRANSFER_PLZ_GIVEME_DATA_NAK );
	HANDLER_AM_DECL( SERVERCOMMON_DATA_TRANSFER_SENDDATA_CMD );
	//HANDLER_AM_DECL( AM_DATA_TRANSFER_SENDDATA_ACK );
	//HANDLER_AM_DECL( AM_DATA_TRANSFER_SENDDATA_NAK );
#endif //__WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
};



