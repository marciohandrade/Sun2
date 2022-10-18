#pragma once

// 모든 프로젝트 및 공유 폴더 위치가 고정이라는 전제하에 설정되는 상대 위치를 통한 Include
#include "../../../include/Global.Define.h"

#pragma		warning(disable:4996)

//#include <SunAuth/AuthDefine.Build.h>
// LoginFront Control
#define __CN000000_090604_LOGIN_FRONT_SERVICE_CONTROL__ //인증서버 다운시 로긴 프론트도 종료처리
#define __NA000000_090701_AUTH_QUERY_TRANSACTION__      //인증 요청 쿼리 트랜잭션

#include <string>
#include <queue>
#include <list>
#include <map>

#include <Winsock2.h>
#include <Windows.h>
#include <TChar.h>
#include <Assert.h>

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

#include <SunAuth/AuthDefine.h>
#include <SunAuth/AuthStruct.h>
#include <AuthStructInternal.h>
#include <SunAuth/AuthProtocol.h>
#include <AuthInternal.hxx>


#include <AuthLinkLibrayList.h>


#define  SERVICE_NAME						_T( "LoginFront" )
#define  SERVICE_HIGHLEVEL_NAME				_T( "LoginFrontHighLevel" )


#define  CONFIG_FILENAME		_T( "./ServerInfo.ini" )


typedef struct __tagServerInfo
{
	char			ipAddress[20];
	unsigned long	portNo;
	unsigned long	sessionCount;
} ServerInfo;




