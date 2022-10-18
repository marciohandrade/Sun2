
#pragma once

#include "PacketHandler.h"


class AccountDBProxySession;

#define HandlerFromAccountDBPServer_DECL( fnName )		\
	static VOID On##fnName( AccountDBProxySession* pServerSession, MSG_BASE* pMsg, WORD wSize )

#define HandlerFromAccountDBPServer_IMPL( fnName )		\
	VOID HandlerFromAccountDBPServer::On##fnName( AccountDBProxySession* pServerSession, MSG_BASE* pMsg, WORD wSize )

#define ADBPSERVER_POSTACTION_DECL( fnName )	\
	static BOOL PostAction_##fnName

#define ADBPSERVER_POSTACTION_IMPL( fnName )	\
	BOOL HandlerFromAccountDBPServer::PostAction_##fnName


class HandlerFromAccountDBPServer
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
	HandlerFromAccountDBPServer_DECL(SX_CONNECTION_REQ_SERVER_INFO_ACK);
	HandlerFromAccountDBPServer_DECL(SX_CONNECTION_REQ_SERVER_INFO_CMD);
};


