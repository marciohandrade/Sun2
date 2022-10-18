
#pragma once

#include "PacketHandler.h"

//#include <PacketStruct_SE.h>

class TempServerSessionEx;

#define HandlerFromTempServer_DECL( fnName )		\
	static VOID On##fnName( TempServerSessionEx* pServerSession, MSG_BASE* pMsg, WORD wSize )

#define HandlerFromTempServer_IMPL( fnName )		\
	VOID HandlerFromTempServer::On##fnName( TempServerSessionEx* pServerSession, MSG_BASE* pMsg, WORD wSize )

#define TEMPSERVER_POSTACTION_DECL( fnName )	\
	static BOOL PostAction_##fnName

#define TEMPSERVER_POSTACTION_IMPL( fnName )	\
	BOOL HandlerFromTempServer::PostAction_##fnName


class HandlerFromTempServer
{
public:
	static VOID		RegisterHandler_ALL( PACKET_HANDLER_DELEGATOR pInstance );
private:
	static sPACKET_HANDLER_INFO*	GetHandlerInfo();
private:
	//------------------------------------------------------------------------------
	// <PACKET HANDLERs>
	// Declaration & Implementation
	//
	HandlerFromTempServer_DECL( SM_CONNECTION_HEARTBEAT );
	HandlerFromTempServer_DECL( SM_CONNECTION_CONNECTION_SYN );
	//HandlerFromTempServer_DECL( SM_CONNECTION_CONNECTION_ACK );
	//HandlerFromTempServer_DECL( SM_CONNECTION_CONNECTION_NAK );
	//HandlerFromTempServer_DECL( SM_CONNECTION_DISCONNECT_CMD );
	//HandlerFromTempServer_DECL( SM_CONNECTION_PHASE_SHIFT_CMD );
	//HandlerFromTempServer_DECL( SM_CONNECTION_BLOCK_CMD );
	////
	//HandlerFromTempServer_DECL(SM_SERVER_INFO_AUTHENTICATION_SYN),
	//HandlerFromTempServer_DECL(SM_SERVER_INFO_AUTHENTICATION_ACK),
	//HandlerFromTempServer_DECL(SM_SERVER_INFO_AUTHENTICATION_NAK),	
	////
	HandlerFromTempServer_DECL( SM_CRYPTOGRAPHY_KEY );
	HandlerFromTempServer_DECL( SM_CRYPTOGRAPHY_ACK );
	HandlerFromTempServer_DECL( SM_CRYPTOGRAPHY_NAK );
	//HandlerFromTempServer_DECL( SM_CRYPTOGRAPHY_CMD );
	//HandlerFromTempServer_DECL( SM_CRYPTOGRAPHY_PACKET_ENCODED );
	//HandlerFromTempServer_DECL( SM_CRYPTOGRAPHY_PACKET_ENCODING );
	////
	//HandlerFromTempServer_DECL( SM_DATA_TRANSFER_TEMPORARY );

	//------------------------------------------------------------------------------
	// <METHODs for HANDLER> <Send Packet 관련>
	//   SolarAuth와 Master간에 패킷 교환에 별도로 주어진 암호화된 파일로부터 해당 패킷을 En/Decrypt한 패킷 송수신한다.
	// <TYPE>
	//   BOOL	PostAction_XX_XXX( xxx );
	TEMPSERVER_POSTACTION_DECL(SM_CONNECTION_CONNECTION_SYN)( TempServerSessionEx* pServerSession, DWORD dwAccessCount );
	TEMPSERVER_POSTACTION_DECL(SM_CONNECTION_COMPLETE)( TempServerSessionEx* pServerSession );
};


