#ifndef __STDAFX_H__
#define __STDAFX_H__
#pragma once

#define DBPROXY_TYPE_DEFAULT    (1)
#define DBPROXY_TYPE_ACCOUNT    (2)
#define DBPROXY_TYPE_UNIT       (3)

#if 1 // if you want the db batch unit, set 0
  #if !defined(_USING_ACCOUNT_DBPROXY_SERVER)
    #define DBPROXY_TYPE_SELECTOR   DBPROXY_TYPE_DEFAULT
  #else
    #define DBPROXY_TYPE_SELECTOR   DBPROXY_TYPE_ACCOUNT
  #endif
#else
  #define DBPROXY_TYPE_SELECTOR   DBPROXY_TYPE_UNIT
#endif

//==================================================================================================
//	<PREPROCESSOR>
#include <Global.Define.h>

#define _USING_ODBC				///< ODBC사용
//#define _USING_OLEDB			///< OLEDB사용
//#define _USING_SQLLIB			///< SQLLIB사용
#define _USING_SIMPLE_CONSOLE	//< solarSimpleConsole사용
#define WIN32_LEAN_AND_MEAN

//==================================================================================================
//	<REFERENCEs | SYSTEM>
#include <windows.h>
#include <Winsock2.h>
#include <oledb.h>
#include <tchar.h>
#include <stdio.h>
#include <time.h>

//==================================================================================================
//	<REFERENCEs | SUN COMMON>
#include <Define.h>
#include <ServerCommonDef.h>
#include <Const.h>
#include <ServerConst.h>

#include <UtilityCommon.h>
#include <chunk_stl.h>
#include <functional_custom.h>

#include <PacketStruct.h>
#include <PacketStruct_ServerCommon.h>
#include <ServerStruct.h> // (f100729.2L) added

#include <ScriptCode.IParser.h>
#include <ScriptDataManager.h>
#include <WorkLoad/WorkLoadPacketStream.h>
#include <WorkLoad/WorkLoadContainer.h>
#include <WorkLoad/WorkLoadManager.h>

//==================================================================================================
namespace util {
class CCriticalSection;
};

//==================================================================================================
// LIBRARY LINK LIST
#include <LinkLibraryList.h>

#pragma comment(lib, "odbc32.lib")
#pragma comment(lib, "odbccp32.lib")

//==================================================================================================
#ifdef _DEBUG

// <LIB><DBPROXY SERVER>
#pragma comment(lib, "../lib/"VC_DIR_REDIRECT"/SolarDatabase_d.lib")
#pragma comment(lib, "../lib/"VC_DIR_REDIRECT"/TheMOLA_d.lib") // Load "DBProxy Encrypted Data"

#else //

// <LIB><DBPROXY SERVER>
#pragma comment(lib, "../lib/"VC_DIR_REDIRECT"/SolarDatabase.lib")
#pragma comment(lib, "../lib/"VC_DIR_REDIRECT"/TheMOLA.lib") // Load "DBProxy Encrypted Data"

#endif //
#pragma comment(lib, "../../lib/libvisual2003_visual2008.lib")
//==================================================================================================

// 전역변수		2005/11/23 조재식 추가
#include <SunLog.h>
#include <SecureLog.h>

#define SUNCONNLOG		MessageDBConnOut
extern CSunLog*			g_pSunConnLog;
extern BOOL				MessageDBConnOut( BYTE byLogLevel, TCHAR* pszMsg, ... );

#ifdef _DEBUG

	#include <SUNConsole.h>
	extern SUNConsole g_SUNConsole;
	#ifndef _USING_SIMPLE_CONSOLE
		#define DISPMSG				g_pIConsole->OutputDisplayArg
	#else
		#define DISPMSG				printf
	#endif

#else

#endif //_DEBUG


//==================================================================================================
//	<REFERENCEs | DATABASE PROXY EXTRERNAL>
#include "QueryObjects/SolarDatabaseInDBP.h"
#include <SolarAssert.h>

#include <DBDecryptFileReader.h>

//==================================================================================================
// Version Control
#if defined(_NA002635_GP_DP_PROTOCOL_UINIFICATION) && \
    (DBPROXY_TYPE_SELECTOR == DBPROXY_TYPE_DEFAULT)
  #define _NA002635_GP_DP_PROTOCOL_UINIFICATION_IN_DBP
#else
  //
#endif

#if defined(_NA002635_GZ_DZ_PROTOCOL_UINIFICATION) && \
    (DBPROXY_TYPE_SELECTOR == DBPROXY_TYPE_DEFAULT)
  #define _NA002635_GZ_DZ_PROTOCOL_UINIFICATION_IN_DBP
#else
  //
#endif

//==================================================================================================
// <REFERENCEs | DATABASE PROXY LOCAL>
#include "./DBProxyCommonStruct.h"
#include "TempPasswordPool.h"
#include <ITimeout.h>
#include <DBProxyServerEx.h>

//==================================================================================================
//_NA002635_GZ_DZ_PROTOCOL_UINIFICATION_IN_DBP
#include "Services/GuildLog.h"
#define GUILDLOG if (g_guild_log) g_guild_log
//==================================================================================================

#endif // __STDAFX_H__