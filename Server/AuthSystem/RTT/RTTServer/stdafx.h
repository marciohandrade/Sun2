#pragma once

#if _MSC_VER < 1500 // vs2008
    #ifndef _WIN32_WINNT
        #define _WIN32_WINNT 0x0500
    #endif
#endif

#pragma  warning(disable:4996)
#pragma  warning(disable:4127)

#ifdef _DEBUG
	#pragma  comment(lib, "eXtremed.lib")
	#pragma  comment(lib, "CrashHandler_d.lib")
#else
	#pragma  comment(lib, "eXtreme.lib")
	#pragma  comment(lib, "CrashHandler.lib")
#endif


#include <Winsock2.h>
#include <Windows.h>
#include <Assert.h>
#include <process.h>
#include <tchar.h>
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

#define  SERVICE_NAME			"RTTServer"
#define  SERVICE_VERSION		"RTTServer v2006.1.18"
#define  SERVERINFO_FILENAME	"./ServerInfo.ini"


//#define  TESTCOUNTER

#define  COUNTER_RTTA_SENDPACKET		0x00
#define  COUNTER_RTTA_SENDTRANSBYTES	0x01
#define  COUNTER_RTTA_RECVPACKET		0x02
#define  COUNTER_RTTA_RECVTRANSBYTES	0x03

#define  COUNTER_WOPS_SENDPACKET		0x10
#define  COUNTER_WOPS_SENDTRANSBYTES	0x11
#define  COUNTER_WOPS_RECVPACKET		0x12
#define  COUNTER_WOPS_RECVTRANSBYTES	0x13

void	OutputDebug( char* format, ... );

#define CHANNEL_ALL 10000

#define  UID(w, c, t, i)	( w << 24 | c << 16 | t << 8 | i )