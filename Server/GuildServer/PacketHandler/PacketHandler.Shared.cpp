#include "stdafx.h"

#include "PacketHandler.Shared.h"
#include <PacketStruct_ServerCommon.h>

#include "Handler_FromGameServer.h"
#include "Handler_FromAgentServer.h"
#include "Handler_FromDBProxyServer.h"
#include "Handler_FromMasterServer.h"
#include "Handler_FromWorldServer.h"

#include <SolarDateTime.h>
#include <GuildServer.h>


PacketHandlerShared::PacketHandlerShared()
{
}

PacketHandlerShared::~PacketHandlerShared()
{
}

#define ADD_HANDLER_INFO_EOR()	{ MAKEWORD(0, 0), PHASE_NOACTION, (PH_fnHandler)NULL }
#define ADD_HANDLER_INFO( c, p, accptablePHASE )		\
{ MAKEWORD(c, p), (ePHASE_SHIFT)(accptablePHASE), (PH_fnHandler)&PacketHandlerShared::On##p }

sPACKET_HANDLER_INFO*	PacketHandlerShared::GetHandlerInfo()
{
	static sPACKET_HANDLER_INFO s_pHandlerInfo[] =
	{
		//ADD_HANDLER_INFO(SERVERCOMMON, SERVERCOMMON_HEARTBEAT, PHASE_UNLIMITED),
		ADD_HANDLER_INFO_EOR(),
	};

	return s_pHandlerInfo;
}

//
//
//

HANDLER_COMMON_IMPL(SERVERCOMMON_HEARTBEAT)
{	// 0:1
	//SERVERCOMMON_HEARTBEAT
//	printf( "heartbeat\n" );
}

