#ifndef _HANDLER_GM_H_
#define _HANDLER_GM_H_

#pragma once

#include <PacketStruct_GM.h>
#include "PacketHandler.h"

// <PROTOTYPEs>
class GameServerSession;

//-------------------------------------------------------------------------------------------------

#define HANDLER_GM_DECL( fnName )		\
	static VOID On##fnName( GameServerSession* pServerSession, MSG_BASE* pMsg, WORD wSize )

#define HANDLER_GM_IMPL( fnName )		\
	VOID Handler_GM::On##fnName( GameServerSession* pServerSession, MSG_BASE* pMsg, WORD wSize )

#define HANDLER_GM_POSTACTION_DECL( fnName )	\
	static BOOL PostAction_##fnName

#define HANDLER_GM_POSTACTION_IMPL( fnName )	\
	BOOL Handler_GM::PostAction_##fnName

//-------------------------------------------------------------------------------------------------


class Handler_GM
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
    HANDLER_GM_DECL(GM_CONNECTION_GROUND_CMD);
	//HANDLER_GM_DECL(SERVERCOMMON_HEARTBEAT);
	HANDLER_GM_DECL(GM_USERMONEY_NTF);
	//HANDLER_GM_DECL(SERVERCOMMON_SERVERSHUTDOWN_ANS);
	HANDLER_GM_DECL(GM_OPERATION_USERMONEY_ANS);
	HANDLER_GM_DECL(GM_OPERATION_STATISTICS_ANS);
#ifdef __NA000000_070523_REPORT_TO_SOLARAUTH
	HANDLER_GM_DECL(GM_OPERATION_REPORT_CACHEITEM_PURCHASE_NTF);
#endif //
};

#endif	// _HANDLER_GM_H_
