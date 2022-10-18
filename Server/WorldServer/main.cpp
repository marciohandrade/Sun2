#include "stdafx.h"
#include "WorldServer.h"
#include <stdio.h>
#include <SolarSimpleConsole.h>
#include <SolarDump.h>
#include "ServerGlobal.h"
#include "ServiceApp.h"
#include "WorldServerInfoParser.h"

//==================================================================================================
// @desc 메모리 누수 리포터 사용 여부
#define MEMORY_LEAK_REPORT_FILE             "memory_leak_World.txt"
// @desc 공유된 메모리 누수 출력 파일명
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

		// 콘솔창 종료
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

	// 콘솔창 초기화
	CONSOLE_DESC cdesc;
	ZeroMemory( &cdesc, sizeof(cdesc) );
	cdesc.bUsingConsoleInput	= FALSE;
	cdesc.bUsingRedirect		= TRUE;
	g_Console.Init( &cdesc );

	// 서버 시작
//	ServiceApp::SetApplicationDirectory();

	// 명령 인수 파싱
	if( !ParseArguments( (LPTSTR)lpCmdLine ) )
	{
		DISPMSG( "Invalid argument!\n" );
		system( "pause" );
		return 0;
	}

	// 서버 초기화
	if( !WorldServer::Instance()->Init() )
	{
		MessageOut(eCRITICAL_LOG,  "World server initialization failure!" );
	}
	else
	{
		// INI 파일 파싱
		if( !WorldServerInfoParser::Instance()->ParseINI( "WorldServer.ini" ) )
		{
			MessageOut(eCRITICAL_LOG,   "WorldServer.ini parsing failed." );
			return FALSE;
		}

		if( eCONSOLE_MODE == WorldServerInfoParser::Instance()->IsServiceMode() )		//콘솔 모드이면...
		{
			// 서버 시작
			WorldServer::Instance()->Run();

			// 서버 종료
			MessageOut(eCRITICAL_LOG,  "World server is terminated..." );

			WorldServer::Instance()->Release();
			WorldServer::Instance()->DestroyInstance();

			// 콘솔창 종료
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