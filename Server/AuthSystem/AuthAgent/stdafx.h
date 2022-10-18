#pragma once

// 모든 프로젝트 및 공유 폴더 위치가 고정이라는 전제하에 설정되는 상대 위치를 통한 Include
#include "../../../include/Global.Define.h"

#pragma warning(disable:4996)

//#include <SunAuth/AuthDefine.Build.h>

#include <string>
#include <queue>
#include <list>
#include <map>

#include <Winsock2.h>
#include <Windows.h>
#include <Assert.h>
#include <TChar.h>

#include <stdio.h>
#include <conio.h>

#include <eXtreme/eXtreme.h>
#include <eXtreme/util/Stream.h>

#include <Define.h>
#include <UtilityCommon.h>
#include <Const.h>
#include <ServerConst.h>
#include <SunLog.h>
#include <MemBand.h>
#include <MemoryPoolFactoryV.h>
#include <ServerGlobal.h>

#include <SunAuth/AuthDefine.h>
#include <SunAuth/AuthStruct.h>
#include <AuthStructInternal.h>
#include <SunAuth/AuthProtocol.h>
#include <AuthInternal.hxx>

#include <AuthLinkLibrayList.h>


#define  SERVICE_NAME           "AuthAgent"
#define  SERVICE_HIGHLEVEL_NAME "AuthAgentHighLevel"


typedef struct __tagServerInfo
{
    char			ipAddress[20];
    unsigned long	portNo;
    unsigned long	sessionCount;
} ServerInfo;



