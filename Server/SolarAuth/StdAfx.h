// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 및 프로젝트 관련 포함 파일이 
// 들어 있는 포함 파일입니다.
//

#pragma once

#define WIN32_LEAN_AND_MEAN
#define _USING_SIMPLE_CONSOLE
#define	_KOREA					//국가별 언어 설정.

#include <Global.Define.h>

// TODO: 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.
#include <windows.h>

#include <iostream>
#include <tchar.h>
#include <stdio.h>
#include <conio.h>
#include <string.h>

#include <algorithm>
#include <functional>

#include <winsock2.h>


#define WAVERIX_CODE_TEST		(0)
#define WAVERIX_CODE_BACKUP		(0)
#define WAVERIX_CODE_RENEWAL	(1)

//====================================================
//  SunServer Include Files
// @history
//  |10.02.03|waverix|delete macro '__NA000000_070523_REPORT_TO_SOLARAUTH'

#include <Define.h>
#include <ServerCommonDef.h>

#include <Const.h>
#include <ServerConst.h>
#include <Version.h>
#include <UtilityCommon.h>

#include <chunk_stl.h>

#include <ResultCode.h>
#include <SolarAssert.h>

#include <Struct.h>
#include <NetworkObject.h>

#include <ProgramCommon/WzDataType.h>
#include <ServerGlobal.h>

#include <IOCPServer.h>

#include <SolarAuth.Common.h>
#include <SolarAuth.Interfaces.h>

#include <WorkLoad/WorkLoadContainer.h>

#include <SolarAuth.CryptFlowPolicy.h>

#include <Include/SolarAuthServer.eSIGMSG.h>

#include <SunLog.h>


//////////////////////////////////////////////////////////////////////////
// LIBRARY LINK LIST
#include <LinkLibraryList.h>

//////////////////////////////////////////////////////////////////////////
#ifdef _DEBUG

// <LIB><SOLARAUTH SERVER>
#pragma comment( lib, "../lib/"VC_DIR_REDIRECT"/TheMola_d.lib" )
#pragma comment( lib, "../lib/"VC_DIR_REDIRECT"/TheMola4File_d.lib" )
#pragma comment( lib, "../lib/"VC_DIR_REDIRECT"/MOLA_DDA_d.lib" )

#else //

// <LIB><SOLARAUTH SERVER>
#pragma comment( lib, "../lib/"VC_DIR_REDIRECT"/TheMola.lib" )
#pragma comment( lib, "../lib/"VC_DIR_REDIRECT"/TheMola4File.lib" )
#pragma comment( lib, "../lib/"VC_DIR_REDIRECT"/MOLA_DDA.lib" )

#endif //
//////////////////////////////////////////////////////////////////////////

//
//====================================================


//====================================================
//  Console

#	include <SolarSimpleConsole.h>

extern SolarSimpleConsole		g_SimpleConsole;

//
//====================================================


//
//====================================================

#define ERROR_PARSER(a)			if( !(a) )			\
								{					\
									DISP_FAIL;		\
									return FALSE;	\
								}

#define DISP_OK		DISPMSG( " [OK]\n" );
#define DISP_FAIL	DISPMSG( " Failed!\n" );

