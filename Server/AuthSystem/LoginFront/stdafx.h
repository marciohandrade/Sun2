#pragma once

// ��� ������Ʈ �� ���� ���� ��ġ�� �����̶�� �����Ͽ� �����Ǵ� ��� ��ġ�� ���� Include
#include "../../../include/Global.Define.h"

#pragma		warning(disable:4996)

//#include <SunAuth/AuthDefine.Build.h>
// LoginFront Control
#define __CN000000_090604_LOGIN_FRONT_SERVICE_CONTROL__ //�������� �ٿ�� �α� ����Ʈ�� ����ó��
#define __NA000000_090701_AUTH_QUERY_TRANSACTION__      //���� ��û ���� Ʈ�����

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




