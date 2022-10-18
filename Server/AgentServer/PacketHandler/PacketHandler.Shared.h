#ifndef _PACKETHANDLER_SHARED_H_
#define _PACKETHANDLER_SHARED_H_

#pragma once

#include <PacketStruct_ServerCommon.h>
#include "PacketHandler.h"

// <PROTOTYPEs>
class ServerSessionEx;
class User;

//-------------------------------------------------------------------------------------------------

#define HANDLER_COMMON_DECL( fnName )		\
	static VOID On##fnName( ServerSessionEx* pServerSessionEx, MSG_BASE* pMsg, WORD wSize )

#define HANDLER_COMMON_IMPL( fnName )		\
	VOID PacketHandlerShared::On##fnName( ServerSessionEx* pServerSessionEx, MSG_BASE* pMsg, WORD wSize )

class PacketHandlerShared
{
public:
	static sPACKET_HANDLER_INFO*	GetHandlerInfo();

private:
	HANDLER_COMMON_DECL(SERVERCOMMON_HEARTBEAT);

public:
	static VOID OnUSER_EMPTY_HANDLER( User* pSession, MSG_BASE* pMsg, WORD wSize );
	static VOID OnGAME_FORWARD_HANDLER( User* pSession, MSG_BASE* pMsg, WORD wSize );
    //_NA000000_CW_PROTOCOL_UNIFICATION
    static void OnWORLD_FORWARD_HANDLER(User* session, MSG_BASE* msg, WORD msg_size);
public:
	//-------------------------------------------------------------------------------------------------
};


#endif // _PACKETHANDLER_SHARED_H_