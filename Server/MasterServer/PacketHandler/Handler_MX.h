#ifndef _HANDLER_MX_H_
#define _HANDLER_MX_H_

#pragma once

#include <PacketStruct_MX.h>
#include "PacketHandler.h"

// <PROTOTYPEs>
class AccountDBProxySession;

//-------------------------------------------------------------------------------------------------

#define HANDLER_MX_DECL( fnName )		\
	static VOID On##fnName( AccountDBProxySession* pServerSession, MSG_BASE* pMsg, WORD wSize )

#define HANDLER_MX_IMPL( fnName )		\
	VOID Handler_MX::On##fnName( AccountDBProxySession* pServerSession, MSG_BASE* pMsg, WORD wSize )

#define HANDLER_MX_POSTACTION_DECL( fnName )	\
	static BOOL PostAction_##fnName

#define HANDLER_MX_POSTACTION_IMPL( fnName )	\
	BOOL Handler_MX::PostAction_##fnName

//-------------------------------------------------------------------------------------------------


class Handler_MX
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
    HANDLER_MX_DECL(MX_CONNECTION_GROUND_CMD);
	HANDLER_MX_DECL(MX_CONNECTION_REQ_SERVER_INFO_ACK);
	HANDLER_MX_DECL(MX_CONNECTION_REQ_SERVER_INFO_CMD);
	HANDLER_MX_DECL(MX_FUNCRESTRICT_RELOAD_ANS);
	HANDLER_MX_DECL(MX_FUNCRESTRICT_RELOAD_NAK);
};

#endif	// _HANDLER_MX_H_
