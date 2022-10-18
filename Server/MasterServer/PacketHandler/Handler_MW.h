#ifndef _HANDLER_MW_H_
#define _HANDLER_MW_H_

#pragma once

#include <PacketStruct_MW.h>
#include "PacketHandler.h"

// <PROTOTYPEs>
class WorldServerSession;

//-------------------------------------------------------------------------------------------------

#define HANDLER_MW_DECL( fnName )		\
	static VOID On##fnName( WorldServerSession* pServerSession, MSG_BASE* pMsg, WORD wSize )

#define HANDLER_MW_IMPL( fnName )		\
	VOID Handler_MW::On##fnName( WorldServerSession* pServerSession, MSG_BASE* pMsg, WORD wSize )

#define HANDLER_MW_POSTACTION_DECL( fnName )	\
	static BOOL PostAction_##fnName

#define HANDLER_MW_POSTACTION_IMPL( fnName )	\
	BOOL Handler_MW::PostAction_##fnName

//-------------------------------------------------------------------------------------------------

class Handler_MW
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
    HANDLER_MW_DECL(MW_CONNECTION_GROUND_CMD);
	HANDLER_MW_DECL(MW_USERCOUNT_SYN);
	HANDLER_MW_DECL(MW_BATTLEZONEINFO_ANS);
	HANDLER_MW_DECL(MW_USERMONEY_SYN);
	HANDLER_MW_DECL(MW_ARMORKIT_FINDHACK_SYN);
	//HANDLER_MW_DECL(SERVERCOMMON_SERVERSHUTDOWN_ANS);
	HANDLER_MW_DECL(MW_MULTIMEMO_SEND_ACK); //__NA001254_20090109_RTTG_FUNC_ADD
};

#endif //_HANDLER_MW_H_
