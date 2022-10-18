// SunClientPrecompiledHeader.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �� ������Ʈ ���� ���� ������ 
// ��� �ִ� ���� �����Դϴ�.
//

#pragma once


#ifdef LAST_RELEASE
    // no operation
#else
    #ifndef WINVER				// Windows 95 �� Windows NT 4 ���� ���������� ����� ����� �� �ֽ��ϴ�.
    #define WINVER 0x0500		// Windows 98�� Windows 2000 ���� ������ �µ��� ������ ������ ������ �ֽʽÿ�.
    #endif

    #ifndef _WIN32_WINNT		// Windows NT 4 ���� ���������� ����� ����� �� �ֽ��ϴ�.
    #define _WIN32_WINNT 0x0500		// Windows 98�� Windows 2000 ���� ������ �µ��� ������ ������ ������ �ֽʽÿ�.
    #endif						

    #ifndef _WIN32_WINDOWS		// Windows 98 ���� ���������� ����� ����� �� �ֽ��ϴ�.
    #define _WIN32_WINDOWS 0x0410 // Windows Me ���� ������ �µ��� ������ ������ ������ �ֽʽÿ�.
    #endif

    //#ifndef _WIN32_IE			// IE 4.0 ���� ���������� ����� ����� �� �ֽ��ϴ�.
    //#define _WIN32_IE 0x0400	// IE 5.0 ���� ������ �µ��� ������ ������ ������ �ֽʽÿ�.
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

// TODO: ���α׷��� �ʿ��� �߰� ����� ���⿡�� �����մϴ�.
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
//        // ����� ��忡���� ���� �⺻���� LEAK Check .USING_SUN_SHARED_POOL �� ������ �� ���� �������̴�.
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
	// �������� �����Ǵ� ���������� OutputDebugString �� ��Ȱ��ȭ ��Ű��.
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