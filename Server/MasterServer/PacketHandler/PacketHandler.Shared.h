#ifndef _PACKETHANDLER_SHARED_H_
#define _PACKETHANDLER_SHARED_H_

#pragma once

#include <PacketStruct_ServerCommon.h>
#include "PacketHandler.h"

// <PROTOTYPEs>
class ServerSessionEx;

//-------------------------------------------------------------------------------------------------

#define HANDLER_COMMON_DECL( fnName )		\
	static VOID On##fnName( ServerSessionEx* pServerSessionEx, MSG_BASE* pMsg, WORD wSize )

#define HANDLER_COMMON_IMPL( fnName )		\
	VOID PacketHandlerShared::On##fnName( ServerSessionEx* pServerSessionEx, MSG_BASE* pMsg, WORD wSize )

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
	static sPACKET_HANDLER_INFO*	GetDataHandlerInfo();

private:
	HANDLER_COMMON_DECL(SERVERCOMMON_HEARTBEAT);
	HANDLER_COMMON_DECL(SERVERCOMMON_SERVERSHUTDOWN_ANS);
	HANDLER_COMMON_DECL(SERVERCOMMON_DATA_TRANSFER_PLZ_GIVEME_DATA_SYN);
	HANDLER_COMMON_DECL(SERVERCOMMON_DATA_TRANSFER_SENDDATA_ACK);
	HANDLER_COMMON_DECL(SERVERCOMMON_DATA_TRANSFER_SENDDATA_NAK);
	HANDLER_COMMON_DECL(SERVERCOMMON_DATA_TRANSFER_SENDDATA_NTF);

	HANDLER_COMMON_DECL(SERVERCOMMON_SECURE_LOG_NTF);
public:
	//-------------------------------------------------------------------------------------------------
	// <WorkLoad Interfaces>
	//
	// <OnAllDataLoaded>
	//   모든 데이터를 받았을 때 호출되는 콜백 함수 :
	//   ScriptDataManager::RegisterAllDataLoadedCallback() 으로 등록한다.
	static VOID		OnAllDataLoaded();
	//------------------------------------------------------------------------------

	static VOID		SEND_SERVERCOMMON_PHASESHIFT_CMD( ServerSessionEx* pServerSession, ePHASE_SHIFT shiftCMD );
};


#endif // _PACKETHANDLER_SHARED_H_