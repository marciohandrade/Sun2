#ifndef _HANDLER_SM_H_
#define _HANDLER_SM_H_

#pragma once

#include <PacketStruct_SM.h>
#include "PacketHandler.h"

// <PROTOTYPEs>
class SolarAuthServerSession;

//-------------------------------------------------------------------------------------------------

#define HANDLER_SM_DECL( fnName )		\
	static VOID On##fnName( SolarAuthServerSession* pServerSession, MSG_BASE* pMsg, WORD wSize )

#define HANDLER_SM_IMPL( fnName )		\
	VOID Handler_SM::On##fnName( SolarAuthServerSession* pServerSession, MSG_BASE* pMsg, WORD wSize )

#define HANDLER_SM_POSTACTION_DECL( fnName )	\
	static BOOL PostAction_##fnName

#define HANDLER_SM_POSTACTION_IMPL( fnName )	\
	BOOL Handler_SM::PostAction_##fnName

//-------------------------------------------------------------------------------------------------

// <WARNING!>
// MASTER SERVER �� SolarAuth�� ������ ��Ŷ �ڵ鷯���� �ٸ� ������ ���� ������ �ִ�.
// �������̽��� �ڵ���� ����� �ÿ� �ٸ� �κе� xx_(SSSSSSSSSSSSSSSSs) ���·� ã�Ƽ� �����ؾ� �Ѵ�.
// �ڵ� �ߺ��� ������ �� ���� �ٵ�...

class Handler_SM
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
	HANDLER_SM_DECL(SM_CONNECTION_HEARTBEAT);
	//
	//HANDLER_SM_DECL(SM_CONNECTION_CONNECTION_SYN);
	HANDLER_SM_DECL(SM_CONNECTION_CONNECTION_ACK);
	HANDLER_SM_DECL(SM_CONNECTION_CONNECTION_NAK);
	HANDLER_SM_DECL(SM_CONNECTION_DISCONNECT_CMD);
	HANDLER_SM_DECL(SM_CONNECTION_PHASE_SHIFT_CMD);
	HANDLER_SM_DECL(SM_CONNECTION_BLOCK_CMD);
	//
	HANDLER_SM_DECL(SM_SERVER_INFO_DESTINATION_KEY_SYN);
	HANDLER_SM_DECL(SM_SERVER_INFO_DESTINATION_KEY_ACK);
	HANDLER_SM_DECL(SM_SERVER_INFO_DESTINATION_KEY_NAK);
	HANDLER_SM_DECL(SM_SERVER_INFO_QUERY_SYN);
	HANDLER_SM_DECL(SM_SERVER_INFO_QUERY_ACK);
	HANDLER_SM_DECL(SM_SERVER_INFO_QUERY_CMD);
	HANDLER_SM_DECL(SM_SERVER_INFO_QUERY_NAK);

	HANDLER_SM_DECL(SM_SERVER_INFO_AUTHENTICATION_SYN);
	HANDLER_SM_DECL(SM_SERVER_INFO_AUTHENTICATION_ACK);
	HANDLER_SM_DECL(SM_SERVER_INFO_AUTHENTICATION_NAK);
	//
	HANDLER_SM_DECL(SM_CRYPTOGRAPHY_KEY);
	//HANDLER_SM_DECL(SM_CRYPTOGRAPHY_ACK);
	//HANDLER_SM_DECL(SM_CRYPTOGRAPHY_NAK);
	HANDLER_SM_DECL(SM_CRYPTOGRAPHY_CMD);
	HANDLER_SM_DECL(SM_CRYPTOGRAPHY_PACKET_ENCODED);
	HANDLER_SM_DECL(SM_CRYPTOGRAPHY_PACKET_ENCODING);
	HANDLER_SM_DECL(SM_CRYPTOGRAPHY_SNK_KEY);
	HANDLER_SM_DECL(SM_CRYPTOGRAPHY_SNK_CMD);
	HANDLER_SM_DECL(SM_CRYPTOGRAPHY_SNK_ACK);
	HANDLER_SM_DECL(SM_CRYPTOGRAPHY_SNK_NAK);
	//
	//HANDLER_SM_DECL(SM_DATA_TRANSFER_PLZ_GIVEME_DATA_SYN);
	HANDLER_SM_DECL(SM_DATA_TRANSFER_PLZ_GIVEME_DATA_ACK);
	HANDLER_SM_DECL(SM_DATA_TRANSFER_PLZ_GIVEME_DATA_NAK);
	HANDLER_SM_DECL(SM_DATA_TRANSFER_SENDDATA_CMD);
	//HANDLER_SM_DECL(SM_DATA_TRANSFER_SENDDATA_ACK);
	//HANDLER_SM_DECL(SM_DATA_TRANSFER_SENDDATA_NAK);

	//------------------------------------------------------------------------------
	// <METHODs for HANDLER>
	//

};

#endif //_HANDLER_SM_H_
