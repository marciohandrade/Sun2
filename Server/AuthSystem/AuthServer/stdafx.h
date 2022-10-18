#pragma once

// 모든 프로젝트 및 공유 폴더 위치가 고정이라는 전제하에 설정되는 상대 위치를 통한 Include
#include "../../../include/Global.Define.h"

#pragma		warning(disable:4996)

//#include <SunAuth/AuthDefine.Build.h>

#include <string>
#include <queue>
#include <list>
#include <map>


#include <Winsock2.h>
#include <Windows.h>
#include <Assert.h>
#include <process.h>
#include <tchar.h>
#include <wininet.h>
#include <stdio.h>
#include <conio.h>
#include <functional_custom.h>

#include <eXtreme/eXtreme.h>
#include <eXtreme/threading/Mutex.h>

#include <Define.h>
#include <UtilityCommon.h>
#include <Const.h>
#include <ServerConst.h>
#include <SunLog.h>
#include <MemBand.h>
#include <MemoryPoolFactoryV.h>


#include <SunAuth/AuthDefine.h>
#include <SunAuth/AuthStruct.h>
#include <AuthStructInternal.h>
#include <SunAuth/AuthProtocol.h>
#include <AuthProtocol_FS.h>
#include <AuthInternal.hxx>

#ifdef _SECURE_AUTH

#ifdef _DEBUG
#pragma comment(lib, "../lib/"VC_DIR_REDIRECT"/MatrixLib_d.lib")
#else
#pragma comment(lib, "../lib/"VC_DIR_REDIRECT"/MatrixLib.lib")
#endif

#endif

#include <AuthLinkLibrayList.h>


#define  SERVICE_NAME					_T( "AuthServer" )
#define  SERVICE_HIGHLEVEL_NAME			_T( "AuthServerHighLevel" )


#define  CONFIG_FILENAME		_T( "./ServerInfo.ini" )
#define	 STRING_FILENAME		_T( "./StringTable.ini" )


typedef struct __tagServerInfo
{
	char			ipAddress[20];
	unsigned long	portNo;
	unsigned long	sessionCount;
} ServerInfo;


