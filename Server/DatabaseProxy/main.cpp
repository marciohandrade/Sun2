#include "stdafx.h"
#include <stdio.h>
#include <SolarSimpleConsole.h>
#include <SolarDump.h>
#include "DBProxyServerEx.h"
#include "ServerGlobal.h"

//==================================================================================================
// @desc 메모리 누수 리포터 사용 여부
#define MEMORY_LEAK_REPORT_FILE             "memory_leak_DBProxy.txt"
// @desc 공유된 메모리 누수 출력 파일명
#define MEMORY_LEAK_REPORT_FILE_FOR_SHARED  "./memory_leak_DBProxy_shared.txt"
#include "./SharedMemoryLeakReporter.hxx"
//==================================================================================================


// Global
SolarSimpleConsole	g_Console;

extern "C"
{
	int WinMainCRTStartup();
}

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

int SunEntryPoint()
{
	SharedMemoryPool::CreateInstance();
	return WinMainCRTStartup();
}

static SolarDump solarDUMP(".");

int APIENTRY _tWinMain(HINSTANCE hInstance,
					   HINSTANCE	hPrevInstance,
					   LPTSTR		lpCmdLine,
					   int		nCmdShow)
{
    __UNUSED((hInstance, hPrevInstance, nCmdShow));
	//SolarDump dp( ".", MiniDumpWithFullMemory );

	// 콘솔창 초기화
	CONSOLE_DESC cdesc;
	ZeroMemory( &cdesc, sizeof(cdesc) );
	cdesc.bUsingConsoleInput	= FALSE;
	cdesc.bUsingRedirect		= TRUE;
	g_Console.Init( &cdesc );

	std::set_new_handler( ErrorHandler_new );

	// 명령 인수 파싱
	if( !ParseArguments( (LPTSTR)lpCmdLine ) )
	{
		DISPMSG( "Invalid argument!\n" );
		system( "pause" );
		return 0;
	}

	const CHAR* cstr_ConfigFile = 
#if DBPROXY_TYPE_SELECTOR == DBPROXY_TYPE_ACCOUNT
		"./ADBPServer.ini";
#elif DBPROXY_TYPE_SELECTOR == DBPROXY_TYPE_DEFAULT
		"./DBPServer.ini";
#elif DBPROXY_TYPE_SELECTOR == DBPROXY_TYPE_UNIT
        "./DBPUnit.ini";
#endif //

	g_DBProxyServer.DoInit( (CHAR*)cstr_ConfigFile,
		DBProxyServerEx::DoAllocServerSession,
		DBProxyServerEx::DoFreeServerSession,
		DBProxyServerEx::DoFreeConnectSession,
		DBProxyServerEx::DoAllocClient_NotUsed,
		DBProxyServerEx::DoFreeClient_NotUsed,
		DBProxyServerEx::DoFreeConnect_NotUsed );

	g_DBProxyServer.DoProcess();

	g_DBProxyServer.DoRelease();

	// 콘솔창 종료
	g_Console.Release();

	return 0;
}










#if (0)








#include <stdio.h>
#include <Stackwalker.h>
#include "DBProxyServerEx.h"
#include <SUNConsole.h>
#include <SolarDump.h>
#include "ServiceApp.h"
#include "ServerGlobal.h"

SUNConsole g_SUNConsole;
ISolarConsole * g_pIConsole = NULL;


class DBProxyServiceApp : public ServiceApp
{
public :
	DBProxyServiceApp(const char* pServiceName)
	: ServiceApp(pServiceName)
	{
	}

	virtual VOID		Update()
	{
		g_DBProxyServer.Run();
		g_DBProxyServer.Release();
		// 서버 종료
		DISPMSG( "Server is terminated...\n" );
		g_SUNConsole.Release();	
	}

	virtual VOID		OnStop()
	{
		g_DBProxyServer.Shutdown();
	}
};

#ifdef	_DEBUG
__DECLARE_SERVICE( DBProxyServiceApp, "DBProxyServer_d" );
#else
__DECLARE_SERVICE( DBProxyServiceApp, "DBProxyServer" );
#endif


extern "C"
{
	int WinMainCRTStartup();
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

	if (lRequest >= 31475)	// 389862
	{
		//if (nSize == 118)
		{
			__asm int 3;
		}
	}
	

	return TRUE;
}

int SunEntryPoint()
{
	//_CrtSetAllocHook(SunAllocHook);
	return WinMainCRTStartup();
}

int APIENTRY WinMain(HINSTANCE hInstance,
					 HINSTANCE hPrevInstance,
					 LPSTR     lpCmdLine,
					 int       nCmdShow)
{
	StartMemoryLeakCheck();
	//BreakPointOnMemoryLeak(148);	

	SolarDump dp;

	BOOL bConsoleMode = (strstr( lpCmdLine, "service" ) == NULL);

	/*
	
		SolarDatabaseDesc desc_;
		//desc.DataBaseModuleType			= DBCInterfaceType_OLEDB;
		desc_.DataBaseModuleType		= DBCInterfaceType_ODBC;
		desc_.dwQueryPoolSize			= 2000;
		desc_.wQueryProcessorNum		= 1;
		desc_.pszDatabaseIP				= "10.1.48.38";
		desc_.pszDatabaseName			= "SUNOnline";
		desc_.pszUserName				= "sa";
		desc_.pszUserPassword			= "xodidtjqjd";
		desc_.fnErrorMessage			= NULL;
		desc_.fnQueryResult				= NULL;
	
		
		SolarDatabaseInDBP*	db = new SolarDatabaseInDBP;
		db->Initialize( desc_ );
		db->Release();
		delete db;*/

#ifdef _USING_SIMPLE_CONSOLE
	if( !g_SUNConsole.Init( eSIMPLE_CONSOLE ) )
#else
	if( !g_SUNConsole.Init( eCOMPLEX_CONSOLE ) )
#endif
	{
		ASSERT( !"Solar console Initialization failed!" );
		return 0;
	}

	//서비스 모드시 현재 경로를 설정해주기 위해 Init전에 반드시 사용..
//	ServiceApp::SetApplicationDirectory();

	// 명령 인수 파싱
	if( !ParseArguments( (LPTSTR)lpCmdLine ) )
	{
		DISPMSG( "Invalid argument!\n" );
		system( "pause" );
		return 0;
	}

	// 서버 초기화
	if( !g_DBProxyServer.Init() )
	{
		DISPMSG( "Server initialization failed!\n" );
		system( "pause" );
		return 0;
	}

	SERVER_ENV* pServerEnv = g_DBProxyServer.GetFileParser()->GetServerEnv();

	DISPMSG(  "====================================================\n" );
	DISPMSG(  "  SUN Online DB Proxy Server Initialization Complete.\n" );
	DISPMSG(  "====================================================\n" );

	// 서버 시작
	if( eCONSOLE_MODE == pServerEnv->byServiceMode )
	{
		g_DBProxyServer.Run();

		g_DBProxyServer.Release();
		// 서버 종료
		DISPMSG( "Server is terminated...\n" );

		g_SUNConsole.Release();
	}
	else if( eSERVICE_MODE == pServerEnv->byServiceMode )
	{
		DISPMSG( "<<<<< Service Mode >>>>>\n" );
		__START_SERVICE( DBProxyServiceApp );
	}


#ifdef _DEBUG
	//DeInitAllocCheck();//Memory Leak Check End
#endif
	

	return 0;
}









#endif





