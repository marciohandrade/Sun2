#include "stdafx.h"

#include <SolarSimpleConsole.h>
#include <SolarDump.h>

#include "GuildServer.h"

//==================================================================================================
// @desc 메모리 누수 리포터 사용 여부
#define MEMORY_LEAK_REPORT_FILE             "memory_leak_Guild.txt"
// @desc 공유된 메모리 누수 출력 파일명
#define MEMORY_LEAK_REPORT_FILE_FOR_SHARED  "../_bin/memory_leak_Guild_shared.txt"
#include "./SharedMemoryLeakReporter.hxx"
//==================================================================================================

//
//
//============================================================
//
extern "C"
{
	int WinMainCRTStartup();
}

int SunEntryPoint()
{
	SharedMemoryPool::CreateInstance();
	return WinMainCRTStartup();
}

//============================================================
// Module Variables
static GuildServer*		s_pGuildServer	= NULL;

// Global
SolarSimpleConsole	g_Console;

//
//
//
GuildServer*		GuildServer::GetMainFrame()
{
	return s_pGuildServer;
}

int APIENTRY WinMain(HINSTANCE hInstance,
					 HINSTANCE hPrevInstance,
					 LPSTR     lpCmdLine,
					 int       nCmdShow)
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

	GuildServer* pGuildServer = new GuildServer();
	s_pGuildServer = pGuildServer;

	pGuildServer->DoInit(
						"./GuildServer.ini",
						GuildServer::CreateServerSideAcceptedObject,
						GuildServer::DestroyServerSideAcceptedObject,
						GuildServer::DestroyServerSideConnectedObject,
						NULL, NULL, NULL
						);

	pGuildServer->DoProcess();

	pGuildServer->DoRelease();

	// 콘솔창 종료
	g_Console.Release();

	SAFE_DELETE( pGuildServer );
	
	return 0;
}










static int SunAllocHook(int nAllocType, void *pvData,
						size_t nSize, int nBlockUse, long lRequest,
						const unsigned char * szFileName, int nLine )
{
	__UNUSED(nAllocType);
	__UNUSED(pvData);
	__UNUSED(nBlockUse);
	__UNUSED(szFileName);
	__UNUSED(nLine);

	if (lRequest >= 492604)	// 389862
	{
		if (nSize == 32)
		{
			//		__asm int 3;
		}
	}


	return TRUE;
}









