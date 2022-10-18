#ifndef _HANDLER_MO_H_
#define _HANDLER_MO_H_

#pragma once

#include <PacketStruct_MO.h>
#include "PacketHandler.h"

// <PROTOTYPEs>
class OpServerSession;

//-------------------------------------------------------------------------------------------------

#define HANDLER_MO_DECL( fnName )		\
	static VOID On##fnName( OpServerSession* pServerSession, MSG_BASE_INTERNAL* pMsg, WORD wSize )

#define HANDLER_MO_IMPL( fnName )		\
	VOID Handler_MO::On##fnName( OpServerSession* pServerSession, MSG_BASE_INTERNAL* pMsg, WORD wSize )

#define HANDLER_MO_POSTACTION_DECL( fnName )	\
	static BOOL PostAction_##fnName

#define HANDLER_MO_POSTACTION_IMPL( fnName )	\
	BOOL Handler_MO::PostAction_##fnName

//-------------------------------------------------------------------------------------------------

class Handler_MO
{
public:
	static VOID		RegisterHandler_ALL( PACKET_HANDLER_DELEGATOR pInstance );
    //{__NA001254_20090109_RTTG_FUNC_ADD
	static void	SendTest();
	static void SendGuildNameChange();
	static void SendMMemo();
    //}
private:
	static sPACKET_HANDLER_INFO*	GetHandlerInfo();
private:
	//------------------------------------------------------------------------------
	// <PACKET HANDLERs>
	// Declaration & Implementation
	//
	HANDLER_MO_DECL(MO_RTTG_CERTIFY_ANS);
	HANDLER_MO_DECL(MO_RTTG_GAMENOTICE_REQ);
	HANDLER_MO_DECL(MO_RTTG_BATTLEZONEINFO_REQ);
	HANDLER_MO_DECL(MO_RTTG_USERMONEY_REQ);
	HANDLER_MO_DECL(MO_RTTG_SERVERSHUTDOWN_REQ);
	HANDLER_MO_DECL(MO_RTTG_USERDISCONNECT_REQ);
	HANDLER_MO_DECL(MO_RTTG_FUNCRESTRICT_RELOAD_CMD);
	HANDLER_MO_DECL(MO_RTTG_WAREHOUSEMONEY_REQ);
	HANDLER_MO_DECL(MO_RTTG_GAMESTATISTICS_REQ);
	//HANDLER_MO_DECL(MO_RELOAD_DATA_SYN);
    //{__NA001254_20090109_RTTG_FUNC_ADD
	HANDLER_MO_DECL(MO_RTTG_CHANGE_GUILDNAME_REQ);
	HANDLER_MO_DECL(MO_RTTG_SEND_MESSAGE_REQ);
    //}
};

#endif //_HANDLER_MO_H_
