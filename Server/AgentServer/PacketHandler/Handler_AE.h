#pragma once

#include <PacketStruct_ServerCommon.h>
#include "PacketHandler.h"

// <PROTOTYPEs>
class TempServerSessionEx;

//-------------------------------------------------------------------------------------------------

#define HANDLER_AE_DECL( fnName )		\
	static VOID On##fnName( TempServerSessionEx* pServerSession, MSG_BASE* pMsg, WORD wSize )

#define HANDLER_AE_IMPL( fnName )		\
	VOID Handler_AE::On##fnName( TempServerSessionEx* pServerSession, MSG_BASE* pMsg, WORD wSize )

//-------------------------------------------------------------------------------------------------

class Handler_AE
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
	HANDLER_AE_DECL(SERVERCOMMON_SERVERTYPE);
};
