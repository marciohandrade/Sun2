#ifndef _HANDLER_DM_H_
#define _HANDLER_DM_H_

#pragma once

#include <PacketStruct_DM.h>
#include "PacketHandler.h"

// <PROTOTYPEs>
class GameDBProxySession;

//-------------------------------------------------------------------------------------------------

#define HANDLER_DM_DECL( fnName )		\
	static VOID On##fnName( GameDBProxySession* pServerSession, MSG_BASE* pMsg, WORD wSize )

#define HANDLER_DM_IMPL( fnName )		\
	VOID Handler_DM::On##fnName( GameDBProxySession* pServerSession, MSG_BASE* pMsg, WORD wSize )

#define HANDLER_DM_POSTACTION_DECL( fnName )	\
	static BOOL PostAction_##fnName

#define HANDLER_DM_POSTACTION_IMPL( fnName )	\
	BOOL Handler_DM::PostAction_##fnName

//-------------------------------------------------------------------------------------------------


class Handler_DM
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
    HANDLER_DM_DECL(DM_CONNECTION_GROUND_CMD);
	//HANDLER_DM_DECL( SERVERCOMMON_HEARTBEAT );
	//HANDLER_DM_DECL( SERVERCOMMON_SERVERSHUTDOWN_ANS );
};

#endif	// _HANDLER_DM_H_
