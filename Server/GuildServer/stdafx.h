#pragma once

#define WIN32_LEAN_AND_MEAN

// SYSTEM Headers
#include <windows.h>
#include <tchar.h>

// SUN GLOBAL Headers
#include <Global.Define.h>
#include <Define.h>
#include <ServerCommonDef.h>

#include <UtilityCommon.h>
#include <Const.h>
#include <ServerConst.h>
#include <Global.h>
#include <ServerGlobal.h>
#include <ResultCode.h>

// UTILITY Headers
#include <chunk_stl.h>
#include <SolarHashTable.h>
#include <MemoryPoolFactoryV.h>

// SERVER SHARED Headers
#include <NetworkObject.h>
#include <IOCPServer.h>

#include <ScriptCode.IParser.h>
#include <ScriptDataManager.h>
#include <WorkLoad/WorkLoadPacketStream.h>
#include <WorkLoad/WorkLoadContainer.h>
#include <WorkLoad/WorkLoadManager.h>

#include <SunLog.h>
//////////////////////////////////////////////////////////////////////////
// LIBRARY LINK LIST
#include <LinkLibraryList.h>
//////////////////////////////////////////////////////////////////////////

#include "GuildLog.h"
extern GuildLog* g_pLOG;
#define GUILDLOG if(g_pLOG) g_pLOG

VOID DebugOut( TCHAR * tszMsg, ... );
#define _PRINT	printf



