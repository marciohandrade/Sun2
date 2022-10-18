// SunClientPrecompiledHeader.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 및 프로젝트 관련 포함 파일이 
// 들어 있는 포함 파일입니다.
//

#pragma once


#ifdef LAST_RELEASE
    // no operation
#else
    #ifndef WINVER				// Windows 95 및 Windows NT 4 이후 버전에서만 기능을 사용할 수 있습니다.
    #define WINVER 0x0500		// Windows 98과 Windows 2000 이후 버전에 맞도록 적합한 값으로 변경해 주십시오.
    #endif

    #ifndef _WIN32_WINNT		// Windows NT 4 이후 버전에서만 기능을 사용할 수 있습니다.
    #define _WIN32_WINNT 0x0500		// Windows 98과 Windows 2000 이후 버전에 맞도록 적합한 값으로 변경해 주십시오.
    #endif						

    #ifndef _WIN32_WINDOWS		// Windows 98 이후 버전에서만 기능을 사용할 수 있습니다.
    #define _WIN32_WINDOWS 0x0410 // Windows Me 이후 버전에 맞도록 적합한 값으로 변경해 주십시오.
    #endif

    //#ifndef _WIN32_IE			// IE 4.0 이후 버전에서만 기능을 사용할 수 있습니다.
    //#define _WIN32_IE 0x0400	// IE 5.0 이후 버전에 맞도록 적합한 값으로 변경해 주십시오.
    //#endif
#endif


#pragma warning( push )
#pragma warning( disable : 4312 )
#pragma warning( disable : 4311 )
#pragma warning( disable : 4201 )
#pragma warning( disable : 4100 )
#pragma warning( disable : 4511 )
#pragma warning( disable : 4512 )

#ifdef _DEBUG
#pragma warning( disable : 4189 )
#endif

#pragma warning(disable : 4996)
#pragma warning(disable : 4995)

#include "../../include/Global.Define.h"


//==================================================================================================  

#include "../SolarClientNetwork/SunNetworkApi.h"

#include <windows.h>
#include <stdio.h>
#include <d3dx9math.h>
#include <direct.h>

// TODO: 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.
#include <assert.h>
#include <UtilityCommon.h>
#include <Define.h>
#include <Const.h>
#include <tchar.h>
#include <list>
#include <time.h>

#include "GameDefine.h"
#include "GameConst.h"
#include <Struct.h>
#include <BaseParser.h>
#include <FilePackerManager.h>

#include "SunEngine.h"

//==================================================================================================  
//#if USING_SUN_SHARED_POOL
//    void* /*__cdecl*/ operator new(size_t size, const char* name, size_t line_no);
//    void* /*__cdecl*/ operator new[] (size_t size, const char* name, size_t line_no);
//    void  /*__cdecl*/ operator delete(void*, const char* name, size_t line_no);
//    void  /*__cdecl*/ operator delete[] (void*, const char* name, size_t line_no);
//  #if USING_SUN_SHARED_POOL_REPORT
//    #undef TAllocNew
//    #define TAllocNew(_type_)   \
//        new (##_type_)
//    #define new new(__FUNCTION__, __LINE__)
//  #endif
////==================================================================================================  
//#else
//
//    #ifdef _DEBUG
//        // 디버깅 모드에서의 가장 기본적인 LEAK Check .USING_SUN_SHARED_POOL 과 공존할 수 없는 디파인이다.
//        #define _CLIENT_LEAK_CHECK_DEFAULT_VER  
//    #endif
//
//#endif
//
//
//#ifdef _CLIENT_LEAK_CHECK_DEFAULT_VER
//    #define _CRTDBG_MAP_ALLOC
//    #define _INC_MALLOC
//    #include <stdlib.h>
//    #include <crtdbg.h>
//    #define new new(_CLIENT_BLOCK, __FILE__, __LINE__)
//#endif
//==================================================================================================  

//#include "..\\wzsound\\wzsound.h"

#include "../Utility/nmath.h"
#include "../Utility/vector.h"
#include "../Utility/matrix.h"

#include <macro.h>
#include <tstring.h>
#include <mbctype.h>

#include <PacketStruct.h>
#include <Protocol_CG.h>
#include <structinpacket.h>
#include <packetstruct_cg.h>
#include <Protocol_CW.h>
#include <PacketStruct_CW.h>
#include <ScriptCode.IParser.h>
#include <string>
#include <xstring>
#include <map>
#include <hash_map>
#include <vector>
#include <deque>
#include <queue>
#include <algorithm>

#include "GlobalData.h"
#include "GlobalMacro.h"
#include "GameStruct.h"
#include "GameParameter.h"
#include "GameOption.h"
#include "LoginGameParam.h"

#include "safe_string.h"
#include "InterfaceStruct.h"
#include "UI_Sound_Define.h"

#ifdef MAKE_TEST_MODULE_MACRO
#include "TestModule.h"
#endif

#include "CustomAssertion.h"

using namespace std;

#ifdef _DEV_VER	
	// no operation
#else
	// 유저에게 배포되는 버전에서는 OutputDebugString 을 비활성화 시키자.
	#ifdef OutputDebugString
	#undef OutputDebugString
	#endif

	#define		OutputDebugString	__noop
#endif

//
//#if _MSC_VER >= 1500 // vs2008
//    #include "SunClientPreCompiledHeader.hpp"
//#endif //#if _MSC_VER >= 1500 // vs2008

#pragma warning( pop )