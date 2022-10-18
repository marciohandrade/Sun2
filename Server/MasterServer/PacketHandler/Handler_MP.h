#ifndef _HANDLER_MP_H_
#define _HANDLER_MP_H_

#pragma once
#if !defined(_NA002635_GP_DP_PROTOCOL_UINIFICATION)
#include <PacketStruct_MP.h>
#include "PacketHandler.h"

// <PROTOTYPEs>
class PortalServerSession;

//-------------------------------------------------------------------------------------------------

#define HANDLER_MP_DECL( fnName )		\
	static VOID On##fnName( PortalServerSession* pServerSession, MSG_BASE* pMsg, WORD wSize )

#define HANDLER_MP_IMPL( fnName )		\
	VOID Handler_MP::On##fnName( PortalServerSession* pServerSession, MSG_BASE* pMsg, WORD wSize )

#define HANDLER_MP_POSTACTION_DECL( fnName )	\
	static BOOL PostAction_##fnName

#define HANDLER_MP_POSTACTION_IMPL( fnName )	\
	BOOL Handler_MP::PostAction_##fnName

//-------------------------------------------------------------------------------------------------

class Handler_MP
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
	//HANDLER_MP_DECL(SERVERCOMMON_SERVERSHUTDOWN_ANS);
};
#endif //if !defined(_NA002635_GP_DP_PROTOCOL_UINIFICATION)
#endif //_HANDLER_MP_H_
