#include "stdafx.h"
#include "Entry.h"
#include "Database.h"
#include "CrashHandler.h"
#include "TestCounter.h"
#include "MonitorProcess.h"

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
	char		buffer[4096] = {0, };
	va_list		args;
	//try{
	va_start( args, format );
	vsprintf( buffer, format, args );
	va_end( args );

	GetLocalTime( &systemTime );

	if( prevDay != systemTime.wDay )
	{
		SYSTEMTIME	systemTime;
		char		tmp[256] = {0, };

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

	fprintf( g_fpLog, "[%02d/%02d %02d:%02d:%02d] %s", systemTime.wMonth, systemTime.wDay, systemTime.wHour, systemTime.wMinute, systemTime.wSecond, buffer );
	fflush( g_fpLog );

#ifdef  _DEBUG
	g_console.WriteLine( "[%02d/%02d %02d:%02d:%02d] %s", systemTime.wMonth, systemTime.wDay, systemTime.wHour, systemTime.wMinute, systemTime.wSecond, buffer );
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
				{
					eXtreme::WinService::Instance()->UploadService(
											SERVICE_NAME, SERVICE_NAME, moduleName, args[1], args[2], true );
				}
				else if( argc == 1 )
				{
					eXtreme::WinService::Instance()->UploadService(
											SERVICE_NAME, SERVICE_NAME, moduleName, 0, 0, true );
				}

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
	if( eXtreme::Initialize( SERVICE_NAME, ErrorHandler ) == false )
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
	return Entry::Instance()->Initialize();
}

void Entry::OnServiceStop()
{
	Entry::Instance()->Destroy();
}


Entry::Entry()
	: m_serviceRun( false )
{
	CrashHandler::Install( true, GenerateDmpName, ProcCrashHandling );

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
	if( g_fpLog != 0 )
		fclose( g_fpLog );
}

bool Entry::Initialize()
{
	bool	dbInitialized = false;

	{
		//  RTTServer 정보
		::GetPrivateProfileString( "RTTServer", "IpAddress", "err", m_ipAddress, 20, SERVERINFO_FILENAME );
		m_portNo = ::GetPrivateProfileInt( "RTTServer", "PortNo", 0, SERVERINFO_FILENAME );

		//  Wops Component 정보
		::GetPrivateProfileString( "Wops Component", "IpAddress", "err", m_ipAddressWops, 20, SERVERINFO_FILENAME );
		m_portNoWops = ::GetPrivateProfileInt( "Wops Component", "PortNo", 0, SERVERINFO_FILENAME );

		//  RTTService 정보
		::GetPrivateProfileString( "RTTService", "IpAddress", "err", m_ipAddressService, 20, SERVERINFO_FILENAME );
		m_portNoService = ::GetPrivateProfileInt( "RTTService", "PortNo", 0, SERVERINFO_FILENAME );

#ifdef	_090427_HTTP_WEB_DROP_USER
		//  Web Component 정보
		::GetPrivateProfileString( "Web Component", "IpAddress", "0.0.0.0", m_ipAddressWeb, 20, SERVERINFO_FILENAME );
		m_portNoWeb = ::GetPrivateProfileInt( "Web Component", "PortNo", 6274, SERVERINFO_FILENAME );
#endif

		// RTTAgent Version 정보
		m_dwRTTAgentVersion = ::GetPrivateProfileInt( "RTTAgent", "Version", 0, SERVERINFO_FILENAME );
	}

#ifdef  TESTCOUNTER
	//  TestCounter 초기화
	{
		TestCounter::Counter*	counter;

		counter = TestCounter::Instance()->Add( COUNTER_RTTA_SENDPACKET );			counter->Start();
		counter = TestCounter::Instance()->Add( COUNTER_RTTA_SENDTRANSBYTES );		counter->Start();
		counter = TestCounter::Instance()->Add( COUNTER_RTTA_RECVPACKET );			counter->Start();
		counter = TestCounter::Instance()->Add( COUNTER_RTTA_RECVTRANSBYTES );		counter->Start();

		counter = TestCounter::Instance()->Add( COUNTER_WOPS_SENDPACKET );			counter->Start();
		counter = TestCounter::Instance()->Add( COUNTER_WOPS_SENDTRANSBYTES );		counter->Start();
		counter = TestCounter::Instance()->Add( COUNTER_WOPS_RECVPACKET );			counter->Start();
		counter = TestCounter::Instance()->Add( COUNTER_WOPS_RECVTRANSBYTES );		counter->Start();
	}
#endif

	OutputDebug( "%s\n\n", SERVICE_VERSION );
    OutputDebug( "RTTAgent ver. v%d.00\n\n", m_dwRTTAgentVersion );
	OutputDebug( "[Acceptor info for RTTClient] %s/%d\n", m_ipAddress, m_portNo );
	OutputDebug( "[Acceptor info for WOPS Component] %s/%d\n", m_ipAddressWops, m_portNoWops );
	OutputDebug( "[Acceptor info for RTTService] %s/%d\n", m_ipAddressService, m_portNoService );

#ifdef	_090427_HTTP_WEB_DROP_USER
	OutputDebug( "[Acceptor info for Web Component] %s/%d\n", m_ipAddressWeb, m_portNoWeb );
#endif


	//  Wops Component측의 Network은 미리 오픈한다.
	int nThreadCount = ::GetPrivateProfileInt( "RTTServer", "ThreadCount" ,   6, SERVERINFO_FILENAME );
	m_networkWops.Create(20, ManagerWops::Instance(), nThreadCount);	// 500이었삼... 20으로 줄여주... (KJH : "어짜피 웝스는 원세션이기 때문에.." )
	
	/*
		WOPS에 WEB Service 를 호출 할 Network
		HTTP는 send/recv 반복 형태로 프로토콜이 되어 있으나 여기서는 send를 보내고 error는 로그로 확인을 한다.
	*/
	//m_networkWS.Create(1, ManagerWSSession::Instance());

	if( m_networkWops.OpenAcceptor( m_ipAddressWops, ( u_short )m_portNoWops ) == false )
	{
		OutputDebug( "[%s/%d] Binding failed.\n", m_ipAddressWops, m_portNoWops );
		return false;
	}


	//  Database로부터 필요정보를 로드한다.
	//  Main thread의 가동 이전에 작업되어야 한다.
	dbInitialized = Database::Instance()->Create();

	//  Run thread 가동
	{
		m_eventKill = ::CreateEvent( 0, 0, 0, 0 );
		Start();
	}

	return true;
}

void Entry::Destroy()
{
#ifdef  TESTCOUNTER
	//  TestCounter 종료
	{
		TestCounter::Counter*	counter[4];

		counter[0] = TestCounter::Instance()->Get( COUNTER_RTTA_SENDPACKET );			counter[0]->Stop();
		counter[1] = TestCounter::Instance()->Get( COUNTER_RTTA_SENDTRANSBYTES );		counter[1]->Stop();
		counter[2] = TestCounter::Instance()->Get( COUNTER_RTTA_RECVPACKET );			counter[2]->Stop();
		counter[3] = TestCounter::Instance()->Get( COUNTER_RTTA_RECVTRANSBYTES );		counter[3]->Stop();

		OutputDebug( "** TestCounter Report **\n" );
		OutputDebug( "[RTTAgent]\n" );
		OutputDebug( "Test time : %d ms\n", counter[0]->GetTimeGap() );
		OutputDebug( "Sent packet count = %d(%d)\n", counter[0]->GetCount(), counter[0]->GetCount() / 1000 );
		OutputDebug( "Sent transbytes = %d(%d)\n",   counter[1]->GetCount(), counter[1]->GetCount() / 1000 );
		OutputDebug( "Recv packet count = %d(%d)\n", counter[2]->GetCount(), counter[2]->GetCount() / 1000 );
		OutputDebug( "Recv transbytes = %d(%d)\n",   counter[3]->GetCount(), counter[3]->GetCount() / 1000 );



		counter[0] = TestCounter::Instance()->Get( COUNTER_WOPS_SENDPACKET );			counter[0]->Stop();
		counter[1] = TestCounter::Instance()->Get( COUNTER_WOPS_SENDTRANSBYTES );		counter[1]->Stop();
		counter[2] = TestCounter::Instance()->Get( COUNTER_WOPS_RECVPACKET );			counter[2]->Stop();
		counter[3] = TestCounter::Instance()->Get( COUNTER_WOPS_RECVTRANSBYTES );		counter[3]->Stop();

		OutputDebug( "[WOPS Component]\n" );
		OutputDebug( "Test time : %d ms\n", counter[0]->GetTimeGap() );
		OutputDebug( "Sent packet count = %d(%d)\n", counter[0]->GetCount(), counter[0]->GetCount() / 1000 );
		OutputDebug( "Sent transbytes = %d(%d)\n",   counter[1]->GetCount(), counter[1]->GetCount() / 1000 );
		OutputDebug( "Recv packet count = %d(%d)\n", counter[2]->GetCount(), counter[2]->GetCount() / 1000 );
		OutputDebug( "Recv transbytes = %d(%d)\n",   counter[3]->GetCount(), counter[3]->GetCount() / 1000 );


		TestCounter::Instance()->RemoveAll();
	}
#endif


	//  Run thread를 종료시킨다.
	{
		::SetEvent( m_eventKill );

		if( WaitForTerminate( 1000 ) == false )
			Terminate( 0 );

		::CloseHandle( m_eventKill );
		m_eventKill = INVALID_HANDLE_VALUE;
	}
	
	m_networkWops.CloseAcceptor();
	m_networkWops.Destroy();
	
	//m_networkWS.Destroy();

	ServiceTerminate();

	Database::Instance()->Destroy();
}

void Entry::ServiceStartUp()
{
	int maxRTTACount = ::GetPrivateProfileInt( "RTTServer", "MaxRTTACount", 100, SERVERINFO_FILENAME );
	int nThreadCount = ::GetPrivateProfileInt( "RTTServer", "ThreadCount" ,   6, SERVERINFO_FILENAME );

	//  RTTAgent측의 Network은 ServiceStartUp에서 오픈한다.
	m_networkClient.Create ( (WORD)maxRTTACount, ManagerClient::Instance() , nThreadCount );
	m_networkService.Create( (WORD)maxRTTACount, ManagerService::Instance(), nThreadCount );

#ifdef	_090427_HTTP_WEB_DROP_USER
	m_networkWeb.Create ( 50, ManagerWeb::Instance() , nThreadCount );
#endif


	if( m_networkClient.OpenAcceptor( m_ipAddress, ( u_short )m_portNo ) == false )
	{
		OutputDebug( "[%s/%d] Binding failed.\n", m_ipAddress, m_portNo );
		return;
	}

	if( m_networkService.OpenAcceptor( m_ipAddressService, ( u_short )m_portNoService ) == false )
	{
		OutputDebug( "[%s/%d] Binding failed.\n", m_ipAddressService, m_portNoService );
		return;
	}

#ifdef	_090427_HTTP_WEB_DROP_USER
	if( m_networkWeb.OpenAcceptor( m_ipAddressWeb, ( u_short )m_portNoWeb ) == false )
	{
		OutputDebug( "[%s/%d] Binding failed.\n", m_ipAddressWeb, m_portNoWeb );
		return;
	}
#endif

	m_serviceRun = true;
	OutputDebug( "!-*-*-* Service Started *-*-*-!\n\n" );
}

void Entry::ServiceTerminate()
{
	if(m_serviceRun)
	{
		m_networkClient.CloseAcceptor();
		m_networkService.CloseAcceptor();

		/*
			RTTAgent / RTTService 연결 소켓 정리
		*/
		ManagerClient::Instance()->DisconnectAll();
		ManagerService::Instance()->DisconnectAll();

		
		m_networkClient.Destroy();		
		m_networkService.Destroy();

#ifdef	_090427_HTTP_WEB_DROP_USER
		m_networkWeb.CloseAcceptor();
		ManagerWeb::Instance()->DisconnectAll();
		m_networkWeb.Destroy();	
#endif

		MonitorProcess::Instance()->Clear();
		m_serviceRun = false;
		OutputDebug( "!-*-*-* Service Closed *-*-*-!\n\n" );
	}
}

void Entry::Run()
{
	while( true )
	{
		if( ::WaitForSingleObject( m_eventKill, 10 ) == WAIT_OBJECT_0 )
			break;

		if( m_serviceRun == true )
		{
			m_networkClient.Update();
			m_networkService.Update();
			//m_networkWS.Update();
		}

		m_networkWops.Update();


		IfDebug()
		{
			if( kbhit() != 0 )
			{
				if( getch() == 27 )
					break;
			}
		}
	}
}

void Entry::ProcCrashHandling( LPCTSTR stackWalkLog, LPCTSTR dumpFileName )
{
	FILE*		fp = NULL;
	SYSTEMTIME	systemTime;
	TCHAR		temp[127] = {0, };

	GetLocalTime( &systemTime );
	sprintf( temp, "RTTServer CrashLog_%d%02d%02d_%02d%02d%02d.txt",
		systemTime.wYear, systemTime.wMonth, systemTime.wDay,
		systemTime.wHour, systemTime.wMinute, systemTime.wSecond );

	fp = fopen( temp, "w" );
	fprintf( fp, "Dump file path : %s\r\n\r\n", dumpFileName );
	fprintf( fp, stackWalkLog );

	fclose( fp );


	//  Report to console screen
	IfDebug()
	{
		OutputDebug( "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n" );
		OutputDebug( "This application occured an unknown error as follow...\n\n" );
		OutputDebug( "%s\n", stackWalkLog );
		OutputDebug( "This report and memory dump which is %s file\n sent to reinlainer@webzen.co.kr.\n", dumpFileName );
		OutputDebug( "\n\nPress any key to abort application...." );

		getch();
	}
}

LPCTSTR Entry::GenerateDmpName()
{
	static	char		fileName[64] = {0, };
	SYSTEMTIME	time;

	::GetLocalTime( &time );
	::sprintf( fileName, "%s_%d%d%d_%d%d.dmp", SERVICE_NAME, time.wYear, time.wMonth, time.wDay,
											time.wHour, time.wMinute );

	return fileName;
}