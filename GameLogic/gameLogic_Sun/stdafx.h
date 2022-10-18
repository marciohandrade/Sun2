// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently
#pragma once

//==================================================================================================

#ifndef STRICT
#define STRICT
#endif

// Modify the following defines if you have to target a platform prior to the ones specified below.
// Refer to MSDN for the latest info on corresponding values for different platforms.
#ifndef WINVER				// Allow use of features specific to Windows 95 and Windows NT 4 or later.
#define WINVER 0x0400		// Change this to the appropriate value to target Windows 98 and Windows 2000 or later.
#endif

#ifndef _WIN32_WINNT		// Allow use of features specific to Windows NT 4 or later.
#define _WIN32_WINNT 0x0400	// Change this to the appropriate value to target Windows 2000 or later.
#endif						

#ifndef _WIN32_WINDOWS		// Allow use of features specific to Windows 98 or later.
#define _WIN32_WINDOWS 0x0410 // Change this to the appropriate value to target Windows Me or later.
#endif

#ifndef _WIN32_IE			// Allow use of features specific to IE 4.0 or later.
#define _WIN32_IE 0x0400	// Change this to the appropriate value to target IE 5.0 or later.
#endif

#define _ATL_APARTMENT_THREADED
#define _ATL_NO_AUTOMATIC_NAMESPACE

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// some CString constructors will be explicit

// turns off ATL's hiding of some common and often safely ignored warning messages
#define _ATL_ALL_WARNINGS

#include <atlbase.h>
#include <atlcom.h>
#include <atlwin.h>
#include <atltypes.h>
#include <atlctl.h>
#include <atlhost.h>

//#include "util/StdString.h"
//#define CString CStdString

#pragma warning (disable: 4786)
#pragma warning (disable:4267)
#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

#define TRACE

//==================================================================================================

#define BASE_PLAYERINFO_SIZE	250
#define INTERFACE_STRING_LENGTH 256

//#define __NA_000252_20070208_VERSION_GMCMD
//#define _OLD_EXTRA_STONE_PROCESS_


#include <comutil.h>
#pragma comment(lib, "comsupp.lib")
using namespace ATL;

// NOTE: in Game Logic module versioning tags
/*($Delete_[S/x]$)*/#define RANKTAG_CHANGE_20091111
/*($Delete_[S/x]$)*/#define SOCKETTAG_CHANGE_20091111
/*($Delete_[S/x]$)*/#define BUILD_KOREA_061211	// v1.0.9.16(���� 1:25 2007-02-09) �ֽ� �ѱ� ��������� �ݿ�

//
#define GM_SUCCEEDED        0x000
#define GM_ERROR_MAX_QUEST  0x001

/*
v1.0.9.16(���� 1:25 2007-02-09)
- encodeSkill���� Skill�� �Ϻ� �⺻��Ÿ�� �� ��ų������ ���̳ʸ��� ��ϵ��� �ʴ� ���� ����
- ���Ͽɼǿ��� ��޿ɼ��� ��µ��� �ʴ� ���� ����
*/

//#define BUILD_TAIWAN		// ������ Ŀ���͸���¡
//#define BUILD_TAIWAN		// ������ Ŀ���͸���¡
/*
v1.0.8.1(���� 12:00 2007-03-15) �߱� ���� ������Ʈ


v1.0.8.0(���� 2:19 2007-02-13)
- BUILD_KOREA_061211  �ѱ� �ֽ� ������� �ݿ�
	(���ȭ ������Ʈ)

v1.0.7.6(���� 10:34 2007-02-12)
- �븸 ���ȭ ������ ��ũ��Ʈ �߰�
(���ȭ ��ġ�� �ƴ� ���ȭ�����۸� ��ġ)



v1.0.7.5(���� 5:06 2007-01-29)
- �߱��� ���� cn ������ �Է� ���� ����
( �������� cn���� �����ϸ� �߱���� �ν���.)
*/

//#ifndef BUILD_TAIWAN
/*($Delete_[S/x]$)*/#define _MODULE_COMMERCIAL_KOREA
/*($Delete_[S/x]$)*/#define _FIXED_DATETIIME_METHOD_
                                    //  ���ȭ ������ �� �Ⱓ�� eCHARGE_SUB_FIXED_AMOUNT, eCHARGE_SUB_FIXED_AMOUNT_EX
									// �������� ���� �� ���� �����ð��� ǥ�õȴ�.
									//  �� Ÿ���� �����۵��� ����ð� ǥ��, ���� �ð� ǥ�� �� �Ѱ����� ǥ�õǴµ�
									// ���� �ð��� ǥ�õǴ� ������ ���� �� �����ͺ��̽��� �ٽ� �Է��Ҷ��� SCItemSlot::SetFixedDateTime
									// �Լ��� �����. �ֱ� �ѱ� ���忡���� �����ϴ� �Լ���.
//#endif

//==================================================================================================
\
#include "Deployment.h"
#include "Define.h"
#include "Const.h"
#include "UtilityCommon.h"
#include "Struct.h"
/*
#ifdef LANG_KO
  #if !defined(_KOREA)
    #error "This build configuration is not matched by the nation code of the SUN game configuration"
  #endif
#elif LANG_JP
  #if !defined(_JAPAN)
    #error "This build configuration is not matched by the nation code of the SUN game configuration"
  #endif
#elif LANG_TW	
    #error "This build configuration is not supported anymore"
#elif LANG_CN
  #if !defined(_CHINA)
    #error "This build configuration is not matched by the nation code of the SUN game configuration"
  #endif
#elif LANG_EN
    #error "This build configuration is not supported anymore"
#elif LANG_GLOBAL
  #if !defined(_GLOBAL)
    #error "This build configuration is not matched by the nation code of the SUN game configuration"
  #endif
#else
#endif
*/
//
#pragma comment(lib, "../../lib/"VC_DIR_REDIRECT"/zlib.lib")

#ifdef _DEBUG
    #pragma comment(lib, "../lib/"VC_DIR_REDIRECT"/XMLParserd.lib")
    #pragma comment(lib, "../../SUN3DEngine/lib/"VC_DIR_REDIRECT"/WzFilePacker_Static_d.lib")


    #pragma comment(lib, "../../lib/"VC_DIR_REDIRECT"/ProgramCommon_d.lib")
    #pragma comment(lib, "../../lib/"VC_DIR_REDIRECT"/SolarSlot_d.lib")
    #pragma comment(lib, "../../lib/"VC_DIR_REDIRECT"/SolarQuest_d.lib")
    //#pragma comment(lib, "../../lib/"VC_DIR_REDIRECT"/AreaConquest_d.lib")
    #pragma comment(lib, "../../lib/"VC_DIR_REDIRECT"/Utility_d.lib")
#ifdef C_NA_0_20100520_ACSYSTEM_CHANGED
    #pragma comment(lib, "../../lib/"VC_DIR_REDIRECT"/Achievement_d.lib")
#endif
#else
    #pragma comment(lib, "../lib/"VC_DIR_REDIRECT"/XMLParser.lib")
    #pragma comment(lib, "../../SUN3DEngine/lib/"VC_DIR_REDIRECT"/WzFilePacker_Static.lib")

    #pragma comment(lib, "../../lib/"VC_DIR_REDIRECT"/ProgramCommon.lib")
    #pragma comment(lib, "../../lib/"VC_DIR_REDIRECT"/SolarSlot.lib")
    #pragma comment(lib, "../../lib/"VC_DIR_REDIRECT"/SolarQuest.lib")
    //#pragma comment(lib, "../../lib/"VC_DIR_REDIRECT"/AreaConquest.lib")
    #pragma comment(lib, "../../lib/"VC_DIR_REDIRECT"/Utility.lib")
#ifdef C_NA_0_20100520_ACSYSTEM_CHANGED
#pragma comment(lib, "../../lib/"VC_DIR_REDIRECT"/Achievement.lib")
#endif
#endif

void ConvertBSTRToString(BSTR pSrc, std::string& strSrc);
