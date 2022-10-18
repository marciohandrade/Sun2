
#pragma once

#define WIN32_LEAN_AND_MEAN
//#define __CHECK_MEMORY_LEAK_TEST_

#include <windows.h>

#include <Global.Define.h>
#include <Define.h>
//
#include <UtilityCommon.h>
#include <Const.h>
#include <ServerConst.h>
#include <chunk_stl.h>

#include <PacketStruct_CW.h>
#include <PacketStruct_CG.h>

#include <ScriptCode.IParser.h>
#include <ScriptDataManager.h>
#include <WorkLoad/WorkLoadPacketStream.h>
#include <WorkLoad/WorkLoadContainer.h>
#include <WorkLoad/WorkLoadManager.h>

#include <Timer.h>
#include <ITimeout.h>

#include <SunLog.h>

// 전역변수		2006/01/06 성현창 추가
#include <WorldLog.h>
extern WorldLog*	g_pLOG;
#define GAMELOG		if(g_pLOG) g_pLOG


// SunLog 관련
extern CSunLog* g_pCountLog;
BOOL MessageCount( BYTE byLogLevel, TCHAR* pszMsg, ... );


//////////////////////////////////////////////////////////////////////////
// LIBRARY LINK LIST
#include <LinkLibraryList.h>

//////////////////////////////////////////////////////////////////////////
#if defined(_KOREA)
	#ifdef _DEBUG
		#pragma comment(lib, "../lib/"VC_DIR_REDIRECT"/MOLA_sd.lib")
	#else
		#pragma comment(lib, "../lib/"VC_DIR_REDIRECT"/MOLA_s.lib")
	#endif
#else
	#ifdef _DEBUG
		#pragma comment( lib, "../lib/"VC_DIR_REDIRECT"/"NATION_BUILD_NAME_OF_COUNTRY "/" MAKESTR(NATION_BUILD_VERSION) "/MOLA_sd.lib")
	#else
		#pragma comment( lib, "../lib/"VC_DIR_REDIRECT"/"NATION_BUILD_NAME_OF_COUNTRY "/" MAKESTR(NATION_BUILD_VERSION) "/MOLA_s.lib")
	#endif
#endif
#pragma comment(lib, "../../lib/libvisual2003_visual2008.lib")
//////////////////////////////////////////////////////////////////////////

#define WAVERIX_CODE_BACKUP				(0)
#define WAVERIX_CODE_RENEWAL			(1)
#define WAVERIX_CODE_TEST				(1)
#define WAVERIX_CODE_DELETE_RESERVED	(1)

