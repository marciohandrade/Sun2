#pragma once

#include <PacketStruct_AM.h>
#include "PacketHandler.h"

// <PROTOTYPEs>
class AgentServerSession;

//-------------------------------------------------------------------------------------------------
#define HANDLER_AM_DECL( fnName )		\
	static VOID On##fnName( AgentServerSession* pServerSession, MSG_BASE* pMsg, WORD wSize )

#define HANDLER_AM_IMPL( fnName )		\
	VOID Handler_AM::On##fnName( AgentServerSession* pServerSession, MSG_BASE* pMsg, WORD wSize )

#define HANDLER_AM_POSTACTION_DECL( fnName )	\
	static BOOL PostAction_##fnName

#define HANDLER_AM_POSTACTION_IMPL( fnName )	\
	BOOL Handler_AM::PostAction_##fnName

//-------------------------------------------------------------------------------------------------


class Handler_AM
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
    HANDLER_AM_DECL(AM_CONNECTION_GROUND_CMD);
	//HANDLER_AM_DECL(AM_OPERATION_USERCOUNT_SYN);
	//HANDLER_AM_DECL(SERVERCOMMON_SERVERSHUTDOWN_ANS);
	HANDLER_AM_DECL(AM_ARMORKIT_FINDHACK_SYN);
	HANDLER_AM_DECL(AM_OPERATION_CHAR_DISCONNECT_ACK);
	HANDLER_AM_DECL(AM_CONNECTION_CCU_REPORT_NTF);
	HANDLER_AM_DECL(AM_CONNECTION_ENTERSERVER_CMD);
};











