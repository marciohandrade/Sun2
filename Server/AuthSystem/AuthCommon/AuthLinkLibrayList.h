#pragma once

#pragma comment(lib, "Wininet.lib")
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "../../../lib/"VC_DIR_REDIRECT"/zlib.lib")
#pragma comment(linker, "/NODEFAULTLIB:WzFilePacker_Static")
#pragma comment(linker, "/NODEFAULTLIB:HanAuthForSvrD.lib")
#pragma comment(linker, "/NODEFAULTLIB:HanAuthForSvr.lib")
//
#pragma comment(lib, "../lib/CrashHandler.lib")
//
//////////////////////////////////////////////////////////////////////////
#ifdef _DEBUG

    // <LIB><DEBUG>
    #pragma comment(lib, "../../../SUN3DEngine/lib/"VC_DIR_REDIRECT"/ProgramCommonLibForEngine_d.lib")
    #pragma comment(lib, "../../lib/"VC_DIR_REDIRECT"/Network_d.lib")
    #pragma comment(lib, "../../lib/"VC_DIR_REDIRECT"/SolarLog_d.lib")
    #pragma comment(lib, "../../lib/"VC_DIR_REDIRECT"/TheMOLA_d.lib")
    //
    #pragma comment(lib, "../../lib/"VC_DIR_REDIRECT"/SunWriteLogMng_d.lib")
    #pragma comment(lib, "../../../lib/"VC_DIR_REDIRECT"/SolarSlot_sd.lib")
  #ifdef __NA_20100201_INTERGRATEDBILLING_
	#if _MSC_VER >= 1500 // vs2008
		#pragma comment(lib, "../lib/"VC_DIR_REDIRECT"/ClientStub_VS2008_d.lib")
	#elif _MSC_VER >= 1310 // vs2003
		#pragma comment(lib, "../lib/"VC_DIR_REDIRECT"/ClientStub_VS2003_d.lib")
	#endif
  #endif
  #ifdef __CN_20100909_SECURITY_TOKEN__
    #pragma comment(lib, "../lib/"VC_DIR_REDIRECT"/DCSHCoreApi.lib")
  #endif
  #ifdef _JP_0_20101123_PMS
    #pragma comment(lib, "../../lib/PMSConnD.lib")
  #endif

#else //

    // <LIB><RELEASE>
    #pragma comment(lib, "../../../SUN3DEngine/lib/"VC_DIR_REDIRECT"/ProgramCommonLibForEngine.lib")
    #pragma comment(lib, "../../lib/"VC_DIR_REDIRECT"/Network.lib")
    #pragma comment(lib, "../../lib/"VC_DIR_REDIRECT"/SolarLog.lib")
    #pragma comment(lib, "../../lib/"VC_DIR_REDIRECT"/TheMOLA.lib")

    #pragma comment(lib, "../../lib/"VC_DIR_REDIRECT"/SunWriteLogMng.lib")
    #pragma comment(lib, "../../../lib/"VC_DIR_REDIRECT"/SolarSlot_s.lib")
  #ifdef __NA_20100201_INTERGRATEDBILLING_
	#if _MSC_VER >= 1500 // vs2008
		#pragma comment(lib, "../lib/"VC_DIR_REDIRECT"/ClientStub_VS2008.lib")
	#elif _MSC_VER >= 1310 // vs2003
		#pragma comment(lib, "../lib/"VC_DIR_REDIRECT"/ClientStub_VS2003.lib")
	#endif
  #endif
  #ifdef __CN_20100909_SECURITY_TOKEN__
    #pragma comment(lib, "../lib/"VC_DIR_REDIRECT"/DCSHCoreApi.lib")
  #endif
  #ifdef _JP_0_20101123_PMS
    #pragma comment(lib, "../../lib/PMSConn.lib")
  #endif

#endif //

