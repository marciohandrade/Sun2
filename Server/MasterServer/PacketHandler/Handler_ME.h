#ifndef _HANDLER_ME_H_
#define _HANDLER_ME_H_

#pragma once

#include <PacketStruct_ServerCommon.h>
#include "PacketHandler.h"

// <PROTOTYPEs>
class TempServerSession;

//-------------------------------------------------------------------------------------------------

#define HANDLER_ME_DECL( fnName )		\
	static VOID On##fnName( TempServerSession* pServerSession, MSG_BASE* pMsg, WORD wSize )

#define HANDLER_ME_IMPL( fnName )		\
	VOID Handler_ME::On##fnName( TempServerSession* pServerSession, MSG_BASE* pMsg, WORD wSize )

#define HANDLER_ME_POSTACTION_DECL( fnName )	\
	static BOOL PostAction_##fnName

#define HANDLER_ME_POSTACTION_IMPL( fnName )	\
	BOOL Handler_ME::PostAction_##fnName

//-------------------------------------------------------------------------------------------------


class Handler_ME
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
	HANDLER_ME_DECL(SERVERCOMMON_SERVERTYPE);
};

#endif //_HANDLER_ME_H_
