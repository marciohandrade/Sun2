#pragma once

// <LIB><SHARED>
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "../../lib/"VC_DIR_REDIRECT"/zlib.lib")
#pragma comment(linker, "/NODEFAULTLIB:WzFilePacker_Static")

//#define ENGINE_LIB_PATH  "../../../SunEngine/trunk/lib"
#define ENGINE_LIB_PATH  "../../SUN3DEngine/lib"

//////////////////////////////////////////////////////////////////////////
#ifdef _DEBUG

    // <LIB><DEBUG>
    #pragma comment(lib, ENGINE_LIB_PATH"/"VC_DIR_REDIRECT"/ProgramCommonLibForEngine_d.lib")
    #pragma comment(lib, ENGINE_LIB_PATH"/"VC_DIR_REDIRECT"/Common_d.lib")

    #pragma comment(lib, "../lib/"VC_DIR_REDIRECT"/Network_d.lib")
    #pragma comment(lib, "../lib/"VC_DIR_REDIRECT"/SolarLog_d.lib")

  #ifdef _JP_0_20101123_PMS    
    #pragma comment(lib, "../lib/PMSConnD.lib")
  #endif

#else //

    // <LIB><RELEASE>
    #pragma comment(lib, ENGINE_LIB_PATH"/"VC_DIR_REDIRECT"/ProgramCommonLibForEngine.lib")
    #pragma comment(lib, ENGINE_LIB_PATH"/"VC_DIR_REDIRECT"/Common.lib")

    #pragma comment(lib, "../lib/"VC_DIR_REDIRECT"/Network.lib")
    #pragma comment(lib, "../lib/"VC_DIR_REDIRECT"/SolarLog.lib")

  #ifdef _JP_0_20101123_PMS
    #pragma comment(lib, "../lib/PMSConn.lib")
  #endif

#endif //

