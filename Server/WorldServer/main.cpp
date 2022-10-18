#include "stdafx.h"
#include "WorldServer.h"
#include <stdio.h>
#include <SolarSimpleConsole.h>
#include <SolarDump.h>
#include "ServerGlobal.h"
#include "ServiceApp.h"
#include "WorldServerInfoParser.h"

//==================================================================================================
// @desc �޸� ���� ������ ��� ����
#define MEMORY_LEAK_REPORT_FILE             "memory_leak_World.txt"
// @desc ������ �޸� ���� ��� ���ϸ�
#define MEMORY_LEAK_REPORT_FILE_FOR_SHARED  "./memory_leak_World_shared.txt"
#include "./SharedMemoryLeakReporter.hxx"
//==================================================================================================

// Global
SolarSimpleConsole	g_Console;


class WorldServiceApp : public ServiceApp
{
public :
	WorldServiceApp(const char* pServiceName)
	: ServiceApp(pServiceName)
	{
	}

	virtual VOID		Update()
	{
		WorldServer::Instance()->Run();
		WorldServer::Instance()->Release();

		// �ܼ�â ����
		g_Console.Release();
	}

	virtual VOID		OnStop()
	{
		WorldServer::Instance()->Shutdown();
	}

	virtual VOID		OnShutDown()
	{
		printf("SHUT DOWN");
	}
};

#ifdef _DEBUG
__DECLARE_SERVICE( WorldServiceApp, "WorldServer_d" );
#else
__DECLARE_SERVICE( WorldServiceApp, "WorldServer" );
#endif

static VOID		ErrorHandler_new()
{
	printf( "Can't Alloc Memory\n" );
	if( g_pSunLog )
	{
		MessageOut( eCRITICAL_LOG, _T("Can't Alloc Memory\n") );
		g_pSunLog->Flush( CSunLog::cOUTPUT_BOTH );
	}

	__debugbreak();
}


extern "C"
{
	int WinMainCRTStartup();
}

//===============================================================================================
// <ENTRY POINT>
int SunEntryPoint()
{
	SharedMemoryPool::CreateInstance();
#ifdef __WAVERIX_TEST_LOGIC_TASK_MACRO__
	WaverixTestEntry();
#endif
	return WinMainCRTStartup();
}

static SolarDump solarDUMP(".");

int APIENTRY WinMain(HINSTANCE hInstance,
					 HINSTANCE hPrevInstance,
					 LPSTR     lpCmdLine,
					 int       nCmdShow)
{
	//SolarDump dp( ".", MiniDumpWithFullMemory );

	StartMemoryLeakCheck();
	//BreakPointOnMemoryLeak(332777);

	std::set_new_handler( ErrorHandler_new );

	// �ܼ�â �ʱ�ȭ
	CONSOLE_DESC cdesc;
	ZeroMemory( &cdesc, sizeof(cdesc) );
	cdesc.bUsingConsoleInput	= FALSE;
	cdesc.bUsingRedirect		= TRUE;
	g_Console.Init( &cdesc );

	// ���� ����
//	ServiceApp::SetApplicationDirectory();

	// ��� �μ� �Ľ�
	if( !ParseArguments( (LPTSTR)lpCmdLine ) )
	{
		DISPMSG( "Invalid argument!\n" );
		system( "pause" );
		return 0;
	}

	// ���� �ʱ�ȭ
	if( !WorldServer::Instance()->Init() )
	{
		MessageOut(eCRITICAL_LOG,  "World server initialization failure!" );
	}
	else
	{
		// INI ���� �Ľ�
		if( !WorldServerInfoParser::Instance()->ParseINI( "WorldServer.ini" ) )
		{
			MessageOut(eCRITICAL_LOG,   "WorldServer.ini parsing failed." );
			return FALSE;
		}

		if( eCONSOLE_MODE == WorldServerInfoParser::Instance()->IsServiceMode() )		//�ܼ� ����̸�...
		{
			// ���� ����
			WorldServer::Instance()->Run();

			// ���� ����
			MessageOut(eCRITICAL_LOG,  "World server is terminated..." );

			WorldServer::Instance()->Release();
			WorldServer::Instance()->DestroyInstance();

			// �ܼ�â ����
			g_Console.Release();
		}
		else
		{
			DISPMSG( "[WinMain] <<<<< Service Mode >>>>>\n" );
			__START_SERVICE( WorldServiceApp );
		}
	}

	return 0;
}