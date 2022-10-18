#pragma once

#pragma  warning(disable:4996)

#ifdef _DEBUG
	#pragma  comment(lib, "eXtremed.lib")
	#pragma  comment(lib, "CrashHandler_d.lib")
#else
	#pragma  comment(lib, "eXtreme.lib")
	#pragma  comment(lib, "CrashHandler.lib")
#endif


#define _WIN32_WINNT	0x0501

#include <Winsock2.h>
#include <Windows.h>
#include <Assert.h>
#include <wininet.h>

#include <stdio.h>
#include <conio.h>

#include <iostream>

#include "CountryCode.h"

#include <eXtreme/eXtreme.h>
#include <eXtreme/util/Stream.h>
#include <eXtreme/util/Console.h>
#include <eXtreme/util/WinService.h>

#include "Protocol_RTT.h"

#define  SERVICE_NAME			"RTTService"
#define  SERVICE_VERSION		"RTTService v2005.12.28"
#define  SERVERINFO_FILENAME	"./ServerInfo.ini"


#define  SERVICENAME_RTTA		"RTTAgent"


void	OutputDebug( char* format, ... );
void	GetFileVersion(LPTSTR aszFileName);