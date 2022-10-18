#pragma once

#include <SunAuth/AuthProtocol_GA.h>
#include "PacketHandler.h"

// <PROTOTYPEs>
class AuthAgentServerSession;

//-------------------------------------------------------------------------------------------------

#define HANDLER_AU_DECL( fnName )		\
	static VOID On##fnName( AuthAgentServerSession* pServerSession, MSG_BASE_INTERNAL* pMsg, WORD wSize )

#define HANDLER_AU_IMPL( fnName )		\
	VOID Handler_AU_AUTH::On##fnName( AuthAgentServerSession* pServerSession, MSG_BASE_INTERNAL* pMsg, WORD wSize )

//-------------------------------------------------------------------------------------------------

class Handler_AU_AUTH
{
public:
	static VOID		RegisterHandler_ALL( PACKET_HANDLER_DELEGATOR pInstance );
private:
	static sPACKET_HANDLER_INFO*	GetHandlerInfo();

public:
	//------------------------------------------------------------------------------
	// <PACKET HANDLERs>
	// Declaration & Implementation
	//
	HANDLER_AU_DECL(AG_AUTH_NEWCLIENT_TO_GAMEAGENT_SYN);
	HANDLER_AU_DECL(AG_AUTH_ASK_PC_LOGOUT_SYN);
	HANDLER_AU_DECL(AG_AUTH_BILLING_ALARM_SYN);
#ifdef __NA001334_090420_SERVER_SELECT_FACILITY__
    HANDLER_AU_DECL(AU_AUTH_SERVER_SELECT_ACK);
#endif
    HANDLER_AU_DECL(AU_AUTH_COMMON_QUERY_USER_ID_ACK);   //__NA000000_SHOP_USERID_50_PASSING_PROC__
    HANDLER_AU_DECL(AU_AUTH_BILLING_CHANGE_CONTROL_SYN); //_PCBANG_POLICY_CHANGE_POST_EXPIRED
};
