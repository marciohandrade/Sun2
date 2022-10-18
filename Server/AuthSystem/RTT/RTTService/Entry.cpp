#include "stdafx.h"
#include "Entry.h"
#include "PatchRTTA.h"

#include <direct.h>

#ifdef  _DEBUG
eXtreme::Console	g_console( SERVICE_VERSION );
#endif
FILE*				g_fpLog = NULL;

void ErrorHandler( DWORD lastError, char* desc )
{
	OutputDebug( "Error = %d,  %s\n", lastError, desc );
}

void OutputDebug( char* format, ... )
{
	static		WORD	prevDay = 0;

	SYSTEMTIME	systemTime;
	char		buffer[4092] = {0, };
	va_list		args;

	va_start( args, format );
	vsprintf( buffer, format, args );
	va_end( args );

	GetLocalTime( &systemTime );

	if( prevDay != systemTime.wDay )
	{
		SYSTEMTIME	systemTime;
		char		tmp[127] = {0, };

		GetLocalTime( &systemTime );
		#ifdef _DEBUG
			sprintf( tmp, "%s_%04d%02d%02d.txt", SERVICE_NAME, systemTime.wYear, systemTime.wMonth, systemTime.wDay);
		#else
			sprintf( tmp, "%s_%04d%02d%02d_%02d.txt", SERVICE_NAME, systemTime.wYear, systemTime.wMonth, systemTime.wDay, systemTime.wHour );
		#endif		

		if( g_fpLog != 0 )
			fclose( g_fpLog );

		g_fpLog = fopen( tmp, "w+" );
		prevDay	= systemTime.wDay;
	}

	fprintf( g_fpLog, "[%2d/%2d %2d:%2d:%d] %s",
		systemTime.wMonth, systemTime.wDay, systemTime.wHour, systemTime.wMinute, systemTime.wSecond, buffer );
	fflush( g_fpLog );


#ifdef  _DEBUG
	try{
		g_console.WriteLine( "[%2d/%2d %2d:%2d:%d] %s", systemTime.wMonth, systemTime.wDay, systemTime.wHour, systemTime.wMinute, systemTime.wSecond, buffer );
	}
	catch(...){}
#endif
}

void ParseCommand( char* cmdLine, int* argc, char args[10][32] )
{
	char	delimit[] = " \t,\"";

	char* token	= strtok( cmdLine, delimit );
	while( token != 0 && *argc < 10 )
	{
		//  인자 하나의 길이는 32자를 넘지 못한다.
		strncpy( args[*argc], token, 32 );

		( *argc )++;
		token = strtok( 0, delimit );
	}
}

int WINAPI WinMain( HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/, LPSTR lpCmdLine, int /*nCmdShow*/ )
{
	///////////////////////////////////////////////////////////////////////////////////////////////////
	//  For Windows service
	IfNotDebug()
	{
		char	moduleName[1024] = {0, };
		char	args[10][32] = { {0, }, };
		int		argc = 0;

		//  인자를 얻어온다.	<type> <account> <password>
		ParseCommand( lpCmdLine, &argc, args );

		::GetModuleFileName( 0, moduleName, 1024 );
		strupr( args[0] );

		if( argc > 0 )
		{
			if( strncmp( args[0], "INSTALL", 7 ) == 0 )
			{
				if( argc == 3 )
					eXtreme::WinService::Instance()->UploadService(
											SERVICE_NAME, SERVICE_NAME, moduleName, args[1], args[2], true );
				else if( argc == 1 )
					eXtreme::WinService::Instance()->UploadService(
											SERVICE_NAME, SERVICE_NAME, moduleName, 0, 0, true );

				return 0;
			}
			else if( strncmp( args[0], "REMOVE", 6 ) == 0 )
			{
				eXtreme::WinService::Instance()->RemoveService( SERVICE_NAME );

				return 0;
			}
		}
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////
	//  Running Process
	if( eXtreme::Initialize( SERVICE_NAME ) == false )
	{
		OutputDebug( "Already same server running.\n" );

		return 0;
	}
	
	IfDebug()
	{
		//  Debug일 경우 Console Window로 동작한다.
		Entry::Instance()->Initialize();
		Entry::Instance()->WaitForTerminate();
		Entry::Instance()->Destroy();

		OutputDebug( "Press any key to abort program..." );
		getch();
	}
	else
	{
		//  Release의 경우 Background Service로 동작해야한다.
		//  프로그램이 종료되기 전 까지 StartDispatcher에서 Block된다.
		eXtreme::WinService::Instance()->StartDispatcher(
						SERVICE_NAME, Entry::OnServiceStart, Entry::OnServiceStop );
	}

	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
bool Entry::OnServiceStart()
{
	bool ret = Entry::Instance()->Initialize();
	if( ret == false )
		return false;

	OutputDebug( "[[ RTTService started. ]]\n" );

	return true;
}

void Entry::OnServiceStop()
{
	Entry::Instance()->Destroy();

	OutputDebug( "[[ RTTService stopped. ]]\n" );
}


Entry::Entry()
{
	//  Change director to execution file folder( just for Release )
	IfNotDebug()
	{
		TCHAR	fileName[1024] = {0, };

		::GetModuleFileName( 0, fileName, 1024 );
		int len = ( int )::strlen( fileName );
        int i = 0;
		for(i = len - 1 ; i >= 0 ; i-- )
		{
			if( fileName[i] == '\\' )
				break;
		}

		fileName[i] = 0;
		::chdir( fileName );
	}
}

Entry::~Entry()
{
	fclose( g_fpLog );
}

bool Entry::Initialize()
{
	{
		//  RTTServer 정보
		::GetPrivateProfileString( "RTTServer", "IpAddress", "err", m_serverIPAddr, 20, SERVERINFO_FILENAME );
		m_serverPortNo = ::GetPrivateProfileInt( "RTTServer", "PortNo", 0, SERVERINFO_FILENAME );
	}

	//  Run thread 가동
	m_eventKill = ::CreateEvent( 0, 0, 0, 0 );
	Start();

	return true;
}

void Entry::Destroy()
{
	//  Run thread를 종료시킨다.
	{
		::SetEvent( m_eventKill );

		if( WaitForTerminate( 1000 ) == false )
			Terminate( 0 );

		::CloseHandle( m_eventKill );
		m_eventKill = INVALID_HANDLE_VALUE;
	}
}

void Entry::Run()
{
	PatchRTTA::Instance();
	m_networkRTTS.Create( 1, &m_managerRTTS );

	while( true )
	{
		if( ::WaitForSingleObject( m_eventKill, 10 ) == WAIT_OBJECT_0 )
			break;

		m_networkRTTS.Update();
	}

	m_networkRTTS.Destroy();
}