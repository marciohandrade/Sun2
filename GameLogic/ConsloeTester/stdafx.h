// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once


#include <iostream>
#include <tchar.h>
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// some CString constructors will be explicit

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers
#endif

#include <afx.h>
#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <conio.h>
#include <map>
#include <string>
#include <boost/static_assert.hpp>
#include <boost/static_warning.hpp>


#include "Define.h"
#include "Const.h"
#include "UtilityCommon.h"
#include "Struct.h"

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
#else
    #pragma comment(lib, "../lib/"VC_DIR_REDIRECT"/XMLParser.lib")
    #pragma comment(lib, "../../SUN3DEngine/lib/"VC_DIR_REDIRECT"/WzFilePacker_Static.lib")

    #pragma comment(lib, "../../lib/"VC_DIR_REDIRECT"/ProgramCommon.lib")
    #pragma comment(lib, "../../lib/"VC_DIR_REDIRECT"/SolarSlot.lib")
    #pragma comment(lib, "../../lib/"VC_DIR_REDIRECT"/SolarQuest.lib")
    //#pragma comment(lib, "../../lib/"VC_DIR_REDIRECT"/AreaConquest.lib")
    #pragma comment(lib, "../../lib/"VC_DIR_REDIRECT"/Utility.lib")
#endif

// CHANGES: f101206.2L, ... ¤Ð¤Ð
#ifdef _DEBUG
    #if _MSC_VER >= 1600 // vs2010
        #ifdef LANG_KO
        #import "../_binv100/kr/gameLogic_Sun_d.dll" no_namespace
        #elif LANG_JP
        #import "../_binv100/jp/gameLogic_Sun_d.dll" no_namespace
        #elif LANG_TW
        #import "../_binv100/tw/gameLogic_Sun_d.dll" no_namespace
        #elif LANG_CN
        #import "../_binv100/cn/gameLogic_Sun_d.dll" no_namespace
        #elif LANG_EN
        #import "../_binv100/us/gameLogic_Sun_d.dll" no_namespace
        #elif LANG_GLOBAL
        #import "../_binv100/gp/gameLogic_Sun_d.dll" no_namespace
        #elif LANG_RU
        #import "../_binv100/ru/gameLogic_Sun_d.dll" no_namespace
        #else
        #import "../_binv100/gameLogic_Sun_d.dll" no_namespace
        #endif
    #elif _MSC_VER >= 1500 // vs2008
        #ifdef LANG_KO
        #import "../_binv90/kr/gameLogic_Sun_d.dll" no_namespace
        #elif LANG_JP
        #import "../_binv90/jp/gameLogic_Sun_d.dll" no_namespace
        #elif LANG_TW
        #import "../_binv90/tw/gameLogic_Sun_d.dll" no_namespace
        #elif LANG_CN
        #import "../_binv90/cn/gameLogic_Sun_d.dll" no_namespace
        #elif LANG_EN
        #import "../_binv90/us/gameLogic_Sun_d.dll" no_namespace
        #elif LANG_GLOBAL
        #import "../_binv90/gp/gameLogic_Sun_d.dll" no_namespace
        #elif LANG_RU
        #import "../_binv90/ru/gameLogic_Sun_d.dll" no_namespace
        #else
        #import "../_binv90/gameLogic_Sun_d.dll" no_namespace
        #endif
    #elif _MSC_VER >= 1310 // vs2003
        #ifdef LANG_KO
        #import "../_bin/kr/gameLogic_Sun_d.dll" no_namespace
        #elif LANG_JP
        #import "../_bin/jp/gameLogic_Sun_d.dll" no_namespace
        #elif LANG_TW
        #import "../_bin/tw/gameLogic_Sun_d.dll" no_namespace
        #elif LANG_CN
        #import "../_bin/cn/gameLogic_Sun_d.dll" no_namespace
        #elif LANG_EN
        #import "../_bin/us/gameLogic_Sun_d.dll" no_namespace
        #elif LANG_GLOBAL
        #import "../_bin/gp/gameLogic_Sun_d.dll" no_namespace
        #elif LANG_RU
        #import "../_bin/ru/gameLogic_Sun_d.dll" no_namespace
        #else
        #import "../_bin/gameLogic_Sun_d.dll" no_namespace
        #endif
    #endif
#else
    #if _MSC_VER >= 1600 // vs2010
        #ifdef LANG_KO
        #import "../_binv100/kr/gameLogic_Sun.dll" no_namespace
        #elif LANG_JP
        #import "../_binv100/jp/gameLogic_Sun.dll" no_namespace
        #elif LANG_TW
        #import "../_binv100/tw/gameLogic_Sun.dll" no_namespace
        #elif LANG_CN
        #import "../_binv100/cn/gameLogic_Sun.dll" no_namespace
        #elif LANG_EN
        #import "../_binv100/us/gameLogic_Sun.dll" no_namespace
        #elif LANG_GLOBAL
        #import "../_binv100/gp/gameLogic_Sun.dll" no_namespace
        #elif LANG_RU
        #import "../_binv100/ru/gameLogic_Sun.dll" no_namespace
        #else
        #import "../_binv100/gameLogic_Sun.dll" no_namespace
        #endif
    #elif _MSC_VER >= 1500 // vs2008
        #ifdef LANG_KO
        #import "../_binv90/kr/gameLogic_Sun.dll" no_namespace
        #elif LANG_JP
        #import "../_binv90/jp/gameLogic_Sun.dll" no_namespace
        #elif LANG_TW
        #import "../_binv90/tw/gameLogic_Sun.dll" no_namespace
        #elif LANG_CN
        #import "../_binv90/cn/gameLogic_Sun.dll" no_namespace
        #elif LANG_EN
        #import "../_binv90/us/gameLogic_Sun.dll" no_namespace
        #elif LANG_GLOBAL
        #import "../_binv90/gp/gameLogic_Sun.dll" no_namespace
        #elif LANG_RU
        #import "../_binv90/ru/gameLogic_Sun.dll" no_namespace
        #else
        #import "../_binv90/gameLogic_Sun.dll" no_namespace
        #endif
    #elif _MSC_VER >= 1310 // vs2003
        #ifdef LANG_KO
        #import "../_bin/kr/gameLogic_Sun.dll" no_namespace
        #elif LANG_JP
        #import "../_bin/jp/gameLogic_Sun.dll" no_namespace
        #elif LANG_TW
        #import "../_bin/tw/gameLogic_Sun.dll" no_namespace
        #elif LANG_CN
        #import "../_bin/cn/gameLogic_Sun.dll" no_namespace
        #elif LANG_EN
        #import "../_bin/us/gameLogic_Sun.dll" no_namespace
        #elif LANG_GLOBAL
        #import "../_bin/gp/gameLogic_Sun.dll" no_namespace
        #elif LANG_RU
        #import "../_bin/ru/gameLogic_Sun.dll" no_namespace
        #else
        #import "../_bin/gameLogic_Sun.dll" no_namespace
        #endif
    #endif
#endif

#include "resource.h"       // main symbols
#include <SCItemSlot.h>
#include <SCItemSlotContainer.h>
#include <SCSkillSlot.h>
#include <SCSkillSlotContainer.h>

#include <ItemInfoParser.h>
#include <SkillInfoParser.h>
#include <MissionManager.h>
#include <MapInfoParser.h>
#include <RankOptionParser.h>

//#include "Quest.h"
#include <QuestScriptLoader.h>
#include <QuestInfoManager.h>
#include <QuestManager.h>

//#include <AreaConquestInfoParser.h>
//#include <AreaConquest.h>


// TODO: reference additional headers your program requires here
