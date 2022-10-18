#include "stdafx.h"
#include <stdio.h>
#include <SolarSimpleConsole.h>
#include <SolarDump.h>
#include "PortalServer.h"
#include "ServerGlobal.h"

//==================================================================================================
// @desc 메모리 누수 리포터 사용 여부
#ifdef _DEBUG
#  define USE_MEMORY_LEAK_REPORT            1
#else
#  define USE_MEMORY_LEAK_REPORT            0
#endif

#define MEMORY_LEAK_REPORT_FILE             "memory_leak_Portal.txt"
// @desc 공유된 메모리 누수 출력 파일명
#define MEMORY_LEAK_REPORT_FILE_FOR_SHARED  "../_bin/memory_leak_Portal_shared.txt"
#include "./SharedMemoryLeakReporter.hxx"
//==================================================================================================

// Global
SolarSimpleConsole	g_Console;


extern "C"
{
	int WinMainCRTStartup();
}

int SunEntryPoint()
{
	SharedMemoryPool::CreateInstance();
	return WinMainCRTStartup();
}

int APIENTRY _tWinMain(HINSTANCE hInstance,
					 HINSTANCE	hPrevInstance,
					 LPTSTR		lpCmdLine,
					 int		nCmdShow)
{
	//SolarDump dp( ".", MiniDumpWithFullMemory );
	SolarDump dp;

	// 콘솔창 초기화
	CONSOLE_DESC cdesc;
	ZeroMemory( &cdesc, sizeof(cdesc) );
	cdesc.bUsingConsoleInput	= FALSE;
	cdesc.bUsingRedirect		= TRUE;
	g_Console.Init( &cdesc );

	// 명령 인수 파싱
	if( !ParseArguments( (LPTSTR)lpCmdLine ) )
	{
		DISPMSG( "Invalid argument!\n" );
		system( "pause" );
		return 0;
	}

	g_PortalServer.DoInit( "./PortalServer.ini",
							PortalServer::DoAllocServerSession,
							PortalServer::DoFreeServerSession,
							PortalServer::DoFreeConnectSession,
							PortalServer::DoAllocClient_NotUsed,
							PortalServer::DoFreeClient_NotUsed,
							PortalServer::DoFreeConnect_NotUsed );

	g_PortalServer.DoProcess();

	g_PortalServer.DoRelease();

	// 콘솔창 종료
	g_Console.Release();
	
	return 0;
}

















