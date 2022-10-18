#include "stdafx.h"

#include <SolarSimpleConsole.h>
#include <SolarDump.h>

#include "GuildServer.h"

//==================================================================================================
// @desc �޸� ���� ������ ��� ����
#define MEMORY_LEAK_REPORT_FILE             "memory_leak_Guild.txt"
// @desc ������ �޸� ���� ��� ���ϸ�
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

	// �ܼ�â �ʱ�ȭ
	CONSOLE_DESC cdesc;
	ZeroMemory( &cdesc, sizeof(cdesc) );
	cdesc.bUsingConsoleInput	= FALSE;
	cdesc.bUsingRedirect		= TRUE;
	g_Console.Init( &cdesc );

	// ��� �μ� �Ľ�
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

	// �ܼ�â ����
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









