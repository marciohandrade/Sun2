// GameServer.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <stdio.h>
#include <SolarSimpleConsole.h>
#include <SolarDump.h>
#include "FieldServer.h"
#include "BattleServer.h"
#include "GameServerEx.h"
#include "main.h"
#include "ServerGlobal.h"
#include "Psapi.h"

//==================================================================================================
// @desc 메모리 누수 리포터 사용 여부
#define MEMORY_LEAK_REPORT_FILE             "memory_leak_Game.txt"
// @desc 공유된 메모리 누수 출력 파일명
#define MEMORY_LEAK_REPORT_FILE_FOR_SHARED  "./memory_leak_Game_shared.txt"
#include "./SharedMemoryLeakReporter.hxx"
//==================================================================================================


// Global
SolarSimpleConsole	g_Console;

extern "C"
{
	int WinMainCRTStartup();
}

INT SunEntryPoint()
{
	SharedMemoryPool::CreateInstance();
#ifdef __WAVERIX_TEST_LOGIC_TASK_MACRO__
	WaverixTestEntry();
#endif
	return WinMainCRTStartup();
}

#ifdef _NA_007743_20141027_USE_MINIDUMP_WITH_FULLMEMORY
static SolarDump	solarDUMP(".", MiniDumpWithFullMemory);
#else  // _NA_007743_20141027_USE_MINIDUMP_WITH_FULLMEMORY
static SolarDump	solarDUMP(".");
#endif // _NA_007743_20141027_USE_MINIDUMP_WITH_FULLMEMORY
static VOID			Init_Circumstance();

int APIENTRY _tWinMain(HINSTANCE	hInstance,
					   HINSTANCE	hPrevInstance,
					   LPTSTR		lpCmdLine,
					   int			nCmdShow )
{
    __UNUSED((hInstance,hPrevInstance,nCmdShow));
	StartMemoryLeakCheck();
    //BreakPointOnMemoryLeak(259894);
	Init_Circumstance();

	// 명령 인수 파싱
	if( ParsingGameServerType( (LPTSTR)lpCmdLine ) )
	{
		if( g_pGameServer->DoInit( "./GameServer.ini",
			GameServerEx::DoAllocServerSession,
			GameServerEx::DoFreeServerSession,
			GameServerEx::DoFreeConnectSession,
			GameServerEx::DoAllocClient_NotUsed,
			GameServerEx::DoFreeClient_NotUsed,
			GameServerEx::DoFreeConnect_NotUsed ) )
		{
			g_pGameServer->DoProcess();

			g_pGameServer->DoRelease();
			g_pGameServer->Finalize();
		}
		else
		{
			DISPMSG( "Frame Init() is Failed!\n" );
			system( "pause" );
		}
	}
	else
	{
		DISPMSG( "Invalid argument!\n" );
		system( "pause" );
	}

	// 콘솔창 종료
	g_Console.Release();

	delete g_pGameServer;
	g_pGameServer = NULL;

	

	return 0;
}

BOOL ParseArgumentsForGameServer( char *lpCmdLine )
{
	char szServerType[255], szPath[255];
	GetCommandLineArg( 0, lpCmdLine, szServerType, 255 );
	GetCommandLineArg( 1, lpCmdLine, szPath, 255 );

	// 서버타입 설정
	if( _stricmp( szServerType, "field" ) == 0 || _stricmp( szServerType, "" ) == 0 )
	{
		g_pGameServer = new FieldServer;
	}
	else if( _stricmp( szServerType, "battle" ) == 0 )
	{
		g_pGameServer = new BattleServer;
	}

	// 작업디렉토리 설정
	if( _stricmp( szPath, "" ) == 0 )
		SetApplicationDirectory();
	else
		SetCurrentDirectory( szPath );

	return TRUE;
}

BOOL ParsingGameServerType( LPSTR lpCmdLine )
{
#if defined(_DEBUG)
	char	szArg[255];
	GetCommandLineArg( 0, lpCmdLine, szArg, 255 );

	if( _stricmp( szArg, "" ) != 0 )
	{
		if( _stricmp( szArg, "field" ) == 0 )
		{
			g_pGameServer = new FieldServer;
			SetApplicationDirectory();
			return TRUE;
		}
		else if( _stricmp( szArg, "battle" ) == 0 )
		{
			g_pGameServer = new BattleServer;
			SetApplicationDirectory();
			return TRUE;
		}
		else //S_Drive
			SetCurrentDirectory( szArg );
	}
#endif

	HANDLE hCurPro = GetCurrentProcess();
	if( hCurPro )
	{
		TCHAR szFilePath[MAX_PATH], szProcessName[MAX_PATH], szProcessDir[MAX_PATH], szProcessFile[MAX_PATH];
		GetProcessImageFileName( hCurPro, szFilePath, MAX_PATH );
		_splitpath( szFilePath, NULL, szProcessDir, szProcessName, szProcessFile );
        // CHANGES: f110310.3L, changes partial searching
        szProcessName[_countof(szProcessName) - 1] = _T('\0');
        // CHANGES: f110526.2L, fixed to lowercase comparison
        // because the executing image name is undefined.
        _strlwr(szProcessName);
        BOOST_STATIC_ASSERT(sizeof(szProcessName[0]) == sizeof(char));
        if (strstr(szProcessName, "field")) {
            g_pGameServer = new FieldServer;
        }
        else if (strstr(szProcessName, "battle")) {
            g_pGameServer = new BattleServer;
        }
        else {
            return false;
        }
#if SUN_CODE_BACKUP
		if( _stricmp( szProcessName, "FieldServer_d" ) == 0 || _stricmp( szProcessName, "FieldServer" ) == 0 )
			g_pGameServer = new FieldServer;
		else if( _stricmp( szProcessName, "BattleServer_d" ) == 0 || _stricmp( szProcessName, "BattleServer" ) == 0 )
			g_pGameServer = new BattleServer;
		else
			return FALSE;
#endif
		char	szArg[255];
		GetCommandLineArg( 0, lpCmdLine, szArg, 255 );

		// 디버그도 아닌데, 인자가 있다면... S_Drive 경로다.
		if( _stricmp( szArg, "" ) != 0 )
			SetCurrentDirectory( szArg);
		else
			SetApplicationDirectory();

		return TRUE;
	}

	return FALSE;
}


static VOID		ErrorHandler_new()
{
	printf( "Can't Alloc Memory\n" );
	if( g_pSunLog )
	{
		MessageOut( eCRITICAL_LOG, _T("Can't Alloc Memory\n") );
		g_pSunLog->Flush( CSunLog::cOUTPUT_BOTH );
	}
    // 콜스택 덤프를 얻기 위해서 수정
    int* break_point = NULL;
    *break_point = 200;
	//__debugbreak();
}


//#ifdef __NA000000_080317_ABNORMAL_EXIT_REDIRECT__
#include <signal.h>
VOID		signal_redirect( INT signo )
{
    __TOUCHED(signo);
	__debugbreak();
}
//#endif

static VOID		Init_Circumstance()
{
	std::set_new_handler( ErrorHandler_new );

#ifdef __NA000000_080317_ABNORMAL_EXIT_REDIRECT__
	signal( SIGABRT, signal_redirect );
	_set_error_mode( _OUT_TO_STDERR );
#endif

	// 콘솔창 초기화
	CONSOLE_DESC cdesc;
	ZeroMemory( &cdesc, sizeof(cdesc) );
	cdesc.bUsingConsoleInput	= FALSE;
	cdesc.bUsingRedirect		= TRUE;
	g_Console.Init( &cdesc );
}


