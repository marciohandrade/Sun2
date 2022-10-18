#pragma once

//─────────────────────────────────────────
//  <PREPROCESSOR><DEFINEs>
//  blah~

#include <Global.Define.h>

//─────────────────────────────────────────
//  <INCLUDE><WINDOWS>
#include <tchar.h>
#include <winsock2.h>
#include <windows.h>
#include <oledb.h>

//─────────────────────────────────────────
//  <INCLUDE><STLs>

//─────────────────────────────────────────
//
#define WAVERIX_DEBUGGING_OUTPUT()			printf( "[%s][%u]\n", __FUNCTION__, __LINE__ )
#define WAVERIX_DEBUGGING_OUTPUTn( msg )	printf( "[%s][%u] %s\n", __FUNCTION__, __LINE__, msg )

//─────────────────────────────────────────
//  <INCLUDE><SUN><DEFINE>
#include <Define.h>
#include <ServerCommonDef.h>

#include <Version.h>
#include <ServerBuildVersion.h>

//─────────────────────────────────────────
//  <INCLUDE><SUN><COMMON>
#include <UtilityCommon.h>
#include <functional_custom.h>

#include <Const.h>
#include <ServerConst.h>
#include <ResultCode.h>
#include <PacketStruct.h>
#include <ServerGlobal.h>
#include <ServerStruct.h>

//─────────────────────────────────────────
//  <INCLUDE><SUN><NETWORK>
#include <IOCPServer.h>
#include <NetworkObject.h>

//─────────────────────────────────────────
//  <INCLUDE><SUN><UTILITY>
#include <LinkedlistCS.h>
#include <CriticalSection.h>
#include <MemoryPoolFactory.h>
#include <MemoryPoolFactoryV.h>
#include <SolarAssert.h>
#include <Misc.h>
#include <ITimeout.h>
#include <Timer.h>
#include <Singleton.h>

//─────────────────────────────────────────
//  <INCLUDE><SUN><PARTYLIB>
#include <PartyStruct.h>
#include <PartyDefine.h>

//─────────────────────────────────────────
//  <INCLUDE><SUN><SHARED>
#include <PacketStruct_CG.h>
#include <ScriptCode.IParser.h>
#include <ScriptDataManager.h>
#include <WorkLoad/WorkLoadPacketStream.h>
#include <WorkLoad/WorkLoadContainer.h>
#include <WorkLoad/WorkLoadManager.h>

//─────────────────────────────────────────
//  <INCLUDE><SUN><LOG>
#include <SolarLog.h>
#include <SunLog.h>
#include <SecureLog.h>

//─────────────────────────────────────────
//	<LOG><DECLAREs><APPENDIX>
//	-	SUNCONNLOG로 g_pSunConnLog의 instance를 사용하는 로그출력 Interface
#define SUNCONNLOG		MessageConnOut
extern CSunLog*			g_pSunConnLog;
extern BOOL				MessageConnOut( BYTE byLogLevel, TCHAR* pszMsg, ... );
// { USING_XIGNCODE ...
extern CSunLog* g_pSecureModuleLog;
extern bool MessageSecureModuleOut(eSUN_LOG_LEVEL log_level, const TCHAR* meta_format, ...);
// }
//─────────────────────────────────────────
//  <INCLUDE><SUN><AGENTSERVER>

//==================================================================================================
#define USING_DUMMY_CLIENT  (0) // << test unit (0 or 1)

#if USING_DUMMY_CLIENT
//-----------------------------------------------------------------------------
namespace test_unit {
;
class DummyUser;
void StartDummyClient();
void UpdateDummyClient();
void SendToDummyClient(MSG_BASE* pMsg, WORD wSize);
};

#define USING_DUMMY_CLIENT_FRIEND_ACCESS    friend class test_unit::DummyUser;
//-----------------------------------------------------------------------------
#else //!USING_DUMMY_CLIENT
//-----------------------------------------------------------------------------
#define USING_DUMMY_CLIENT_FRIEND_ACCESS    ;
//-----------------------------------------------------------------------------
#endif

#if !defined(_DEBUG) && USING_DUMMY_CLIENT
    #error "Can't use a dummy client in a release version"
#endif

//─────────────────────────────────────────

//─────────────────────────────────────────
//  <DECLARATIONs><IMPORT LIBRARIES>

//─────────────────────────────────────────
//	LIBRARY LINK LIST
#include <LinkLibraryList.h>

#pragma comment(lib, "../../lib/"VC_DIR_REDIRECT"/WZ_MD5.lib" )
#pragma comment(lib, "../../LIB/HShield/" MAKESTR(HACKSHIELD_VERSION) "/AntiCpXSvr.lib")
#pragma comment(lib, "../../lib/libvisual2003_visual2008.lib")
//─────────────────────────────────────────
#if defined(_KOREA)
	#ifdef _DEBUG
		#pragma comment(lib, "../lib/"VC_DIR_REDIRECT"/MOLA_sd.lib")
	#else
		#pragma comment(lib, "../lib/"VC_DIR_REDIRECT"/MOLA_s.lib")
	#endif
	#pragma comment(lib, "../../lib/"VC_DIR_REDIRECT"/CPShowLIB_d.lib")
#else
	#ifdef _DEBUG
		#pragma comment(lib, "../lib/"VC_DIR_REDIRECT"/"NATION_BUILD_NAME_OF_COUNTRY "/" MAKESTR(NATION_BUILD_VERSION) "/MOLA_sd.lib")
	#else
		#pragma comment(lib, "../lib/"VC_DIR_REDIRECT"/"NATION_BUILD_NAME_OF_COUNTRY "/" MAKESTR(NATION_BUILD_VERSION) "/MOLA_s.lib")
	#endif
	#pragma comment( lib, "../../lib/"VC_DIR_REDIRECT"/CPShowLIB_d.lib") //이것도 국가별 추가할 것.
#endif

//─────────────────────────────────────────

//─────────────────────────────────────────
//  <ETCs>
//	-	DEBUG | RELEASE시 달리 구현되는 부분
#ifdef _DEBUG
#	define DEBUG_INTERRUPT()							__debugbreak();
#	define SHOW_PACKET_STREAM( ptr, size, comment )		PACKET_CONTROL::ShowPacketStream( ptr, size, comment )
#else //
#	define DEBUG_INTERRUPT()
#	define SHOW_PACKET_STREAM( ptr, size, comment )
#endif //

//==================================================================================================

#include <Function/CommonTypesInAgent.h>
#include <Function/ObjectPoolRouter.h>
#include <Function/AgentServer.eSIGMSG.h>
#include "AgentServer.h"

#include <Function/AgentLog.h>
extern AgentLog* g_pLOG;
#define GAMELOG  if(g_pLOG) g_pLOG

//==================================================================================================

#include "UserSessions/UserManager.h"
#include "UserSessions/User.h"