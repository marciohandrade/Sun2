#ifndef _HANDLER_MZ_H_
#define _HANDLER_MZ_H_

#pragma once

#include <PacketStruct_MZ.h>
#include "PacketHandler.h"

// <PROTOTYPEs>
class GuildServerSession;

//-------------------------------------------------------------------------------------------------

#define HANDLER_MZ_DECL( fnName )		\
	static VOID On##fnName( GuildServerSession* pServerSession, MSG_BASE* pMsg, WORD wSize )

#define HANDLER_MZ_IMPL( fnName )		\
	VOID Handler_MZ::On##fnName( GuildServerSession* pServerSession, MSG_BASE* pMsg, WORD wSize )

#define HANDLER_MZ_POSTACTION_DECL( fnName )	\
	static BOOL PostAction_##fnName

#define HANDLER_MZ_POSTACTION_IMPL( fnName )	\
	BOOL Handler_MZ::PostAction_##fnName

//-------------------------------------------------------------------------------------------------


class Handler_MZ
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
    HANDLER_MZ_DECL(DM_CONNECTION_GROUND_CMD);
	//HANDLER_MZ_DECL(SERVERCOMMON_SERVERSHUTDOWN_ANS);
    //{__NA001254_20090109_RTTG_FUNC_ADD
	HANDLER_MZ_DECL(MZ_GUILD_NAME_CHANGE_ACK);
	HANDLER_MZ_DECL(MZ_GUILD_NAME_CHANGE_NAK);
    //}
};

#endif //_HANDLER_MZ_H_
