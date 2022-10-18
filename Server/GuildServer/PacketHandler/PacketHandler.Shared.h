#ifndef _PACKETHANDLER_SHARED_H_
#define _PACKETHANDLER_SHARED_H_

#pragma once

#include <PacketStruct_ServerCommon.h>
#include "PacketHandler.h"

// <PROTOTYPEs>
class ServerSession;

//-------------------------------------------------------------------------------------------------

#define HANDLER_COMMON_DECL( fnName )		\
	static VOID On##fnName( ServerSession* pServerSession, MSG_BASE* pMsg, WORD wSize )

#define HANDLER_COMMON_IMPL( fnName )		\
	VOID PacketHandlerShared::On##fnName( ServerSession* pServerSession, MSG_BASE* pMsg, WORD wSize )

#define HANDLER_COMMON_POSTACTION_DECL( fnName )	\
	static BOOL PostAction_##fnName

#define HANDLER_COMMON_POSTACTION_IMPL( fnName )	\
	BOOL PacketHandlerShared::PostAction_##fnName


class PacketHandlerShared
{
public:
	PacketHandlerShared();
	~PacketHandlerShared();

public:
	static sPACKET_HANDLER_INFO*	GetHandlerInfo();

private:
	HANDLER_COMMON_DECL(SERVERCOMMON_HEARTBEAT);
public:
};


#endif // _PACKETHANDLER_SHARED_H_