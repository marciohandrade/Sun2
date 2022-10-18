#pragma once

#pragma  warning(disable:4996)
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

#define  SERVICE_NAME			"RTTAgent"
#define  SERVICE_VERSION		"RTTAgent v2005.12.28"
#define  SERVERINFO_FILENAME	"./ServerInfo.ini"

#define RTT_SERVICE_NAME		"RTTService"

#include <string>
//#define  TESTCOUNTER

#define  COUNTER_RTTS_SENDPACKET		0x00
#define  COUNTER_RTTS_SENDTRANSBYTES	0x01
#define  COUNTER_RTTS_RECVPACKET		0x02
#define  COUNTER_RTTS_RECVTRANSBYTES	0x03

#define  COUNTER_SKEL_SENDPACKET		0x10
#define  COUNTER_SKEL_SENDTRANSBYTES	0x11
#define  COUNTER_SKEL_RECVPACKET		0x12
#define  COUNTER_SKEL_RECVTRANSBYTES	0x13


void	OutputDebug( char* format, ... );
