
#pragma once

#define WIN32_LEAN_AND_MEAN

#include <Global.Define.h>

#include <windows.h>

#include <iostream>
#include <tchar.h>
#include <stdio.h>
#include <conio.h>
#include <string.h>

#include <vector>
#include <list>
#include <map>
#include <hash_map>
#include <algorithm>
#include <functional>

#define __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION_CRYPT__
#define __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION_4SERVERFRAME__
#define __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION_TEST__

#define __WAVERIX_FUNCTION_RESTRICT_EXTENDED__

//====================================================
//  SunServer Include Files

#include <Define.h>
#include <Const.h>
#include <ServerConst.h>
#include <Version.h>

#include <UtilityCommon.h>
#include <ServerCommonDef.h>
#include <chunk_stl.h>

#include <ResultCode.h>
#include <SolarAssert.h>

#include <Struct.h>
#include <NetworkObject.h>

#include <ProgramCommon/WzDataType.h>
#include <ServerGlobal.h>
#include <ServerStruct.h>

#include <IOCPServer.h>

#include <SolarAuth.Common.h>
#include <SolarAuth.Interfaces.h>

#include <WorkLoad/WorkLoadContainer.h>

#include <SolarAuth.CryptFlowPolicy.h>

//
//====================================================
//

#ifdef __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
#include <ScriptCode.IParser.h>
#include <ScriptDataManager.h>
#include <WorkLoad/WorkLoadPacketStream.h>
#include <WorkLoad/WorkLoadContainer.h>
#include <WorkLoad/WorkLoadManager.h>

#include <SolarAuth.CryptFlowPolicy.h>
#endif //__WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__

//
//
#include <Function/MasterServerInfoStruct.h>

#include <Include/MasterServer.eSIGMSG.h>


//#define __WAVERIX_SOLAR_AUTH_DEBUG_CODE__
#ifdef __WAVERIX_SOLAR_AUTH_DEBUG_CODE__

#define __WAVERIX_SOLAR_AUTH_DEBUG_CODE(code)	\
	do {	\
	code	\
	} while( 0 );

#else //__WAVERIX_SOLAR_AUTH_DEBUG_CODE__

#define __WAVERIX_SOLAR_AUTH_DEBUG_CODE(code)

#endif //__WAVERIX_SOLAR_AUTH_DEBUG_CODE__

// 전역변수		2005/10/27 조재식 추가
#include <SunLog.h>
extern CSunLog* g_pSecureCollectLog;


//////////////////////////////////////////////////////////////////////////
// LIBRARY LINK LIST
#include <LinkLibraryList.h>

//////////////////////////////////////////////////////////////////////////
#ifdef _DEBUG

// <LIB><MASTER SERVER>
#pragma comment(lib, "../lib/"VC_DIR_REDIRECT"/MOLA_DDA_d.lib")

#else //

#pragma comment(lib, "../lib/"VC_DIR_REDIRECT"/MOLA_DDA.lib")
// <LIB><MASTER SERVER>

#endif //

#pragma comment(lib, "../../lib/libvisual2003_visual2008.lib")
//////////////////////////////////////////////////////////////////////////


#define DISP_OK		DISPMSG( " [OK]\n" );
#define DISP_FAIL	DISPMSG( " Failed!\n" );
