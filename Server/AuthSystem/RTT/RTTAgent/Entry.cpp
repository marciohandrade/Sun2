#include "stdafx.h"
#include "Entry.h"
#include "CrashHandler.h"
#include "ManagerSkel.h"
#include "ResourceMonitor.h"
#include "DropStatePoll.h"
#include "ProcessMon.h"
#include "TestCounter.h"

#include <direct.h>

#ifdef  _DEBUG
eXtreme::Console	g_console( SERVICE_VERSION );
#endif
FILE*				g_fpLog;

void ErrorHandler( DWORD lastError, char* desc )
{
	OutputDebug( "Error = %d\n  %s\n", lastError, desc );
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

	fprintf( g_fpLog, "[%02d/%02d %02d:%02d:%02d] %s", systemTime.wMonth, systemTime.wDay, systemTime.wHour, systemTime.wMinute, systemTime.wSecond, buffer );
	fflush( g_fpLog );

#ifdef  _DEBUG
	g_console.WriteLine( "[%02d/%02d %02d:%02d:%02d] %s", systemTime.wMonth, systemTime.wDay, systemTime.wHour, systemTime.wMinute, systemTime.wSecond, buffer );
#endif
}

void ParseCommand( char* cmdLine, int* argc, char args[10][32] )
{
	char	delimit[] = " \t,\"";
	char*	token = NULL;

	token	= strtok( cmdLine, delimit );
	while( token != 0 && *argc < 10 )
	{
		//  인자 하나의 길이는 32자를 넘지 못한다.
		strncpy( args[*argc], token, 32 );

		( *argc )++;
		token = strtok( 0, delimit );
	}
}

#pragma  comment(lib, "Version.lib")

DWORD GetFileVersion(LPTSTR aszFileName)
{
	DWORD dwRetValue = 0;
	char buffer[4096] = {0, };
	if( TRUE == ::GetFileVersionInfo(aszFileName, NULL, 4096, buffer) )
	{
        LPVOID  lpBuffer;
		UINT    uiBytes;
		::VerQueryValue(buffer, TEXT("\\StringFileInfo\\040904B0\\ProductVersion"), &lpBuffer, &uiBytes);
		dwRetValue = atoi((char*)lpBuffer);       
    }

	return dwRetValue;
}

int WINAPI WinMain( HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/, LPSTR lpCmdLine, int /*nCmdShow*/ )
{

	///////////////////////////////////////////////////////////////////////////////////////////////////
	//  For Windows service
	IfNotDebug()
	{
		char	moduleName[1024] = {0, };
		char	args[10][32] = { {0,}, };
		int		argc = 0;

		//  인자를 얻어온다.	<type> <account> <password>
		ParseCommand( lpCmdLine, &argc, args );

		::GetModuleFileName( 0, moduleName, 1024 );
		strupr( args[0] );

		if( argc > 0 )
		{
			if( strncmp( args[0], "INSTALL", 7 ) == 0 )
			{
				/**
				 2008.08.10
				 modify by guiun
				 RTTAgent는 RTTService 실행후 실행이 되어야 한다.(패치 프로세스 진행시 문제가 될 수 있는 상황이 생길 수 있다)
				*/

				if( argc == 3 )
				{
					eXtreme::WinService::Instance()->UploadService(SERVICE_NAME, 
						                                           SERVICE_NAME, 
																   moduleName, 
																   args[1],		// login user id
																   args[2],		// login password
																   RTT_SERVICE_NAME,
																   true );
				}
				else if( argc == 1 )
				{
					eXtreme::WinService::Instance()->UploadService(SERVICE_NAME, 
						                                           SERVICE_NAME, 
																   moduleName, 
																   NULL, 
																   NULL, 
																   RTT_SERVICE_NAME,
																   true );
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
	{
		//  RTTServer 정보
		::GetPrivateProfileString( "RTTServer", "IpAddress", "err", m_serverIPAddr, 20, SERVERINFO_FILENAME );
		m_serverPortNo = ::GetPrivateProfileInt( "RTTServer", "PortNo", 0, SERVERINFO_FILENAME );

		//  RTTAgent 정보
		::GetPrivateProfileString( "RTTAgent", "IpAddress", "err", m_agentIPAddr, 20, SERVERINFO_FILENAME );
		m_agentPortNo = ::GetPrivateProfileInt( "RTTAgent", "PortNo", 0, SERVERINFO_FILENAME );

		//  Patch Launcher 정보
		::GetPrivateProfileString( "Patch Launcher", "Path", "none", RTTServer::m_patchLauncherPath, MAX_PATH, SERVERINFO_FILENAME );
		::GetPrivateProfileString( "Patch Launcher", "Binary", "none", RTTServer::m_patchLauncherBinary, 60, SERVERINFO_FILENAME );
	}


#ifdef  TESTCOUNTER
	//  TestCounter 초기화
	{
		TestCounter::Counter*	counter;

		counter = TestCounter::Instance()->Add( COUNTER_RTTS_SENDPACKET );			counter->Start();
		counter = TestCounter::Instance()->Add( COUNTER_RTTS_SENDTRANSBYTES );		counter->Start();
		counter = TestCounter::Instance()->Add( COUNTER_RTTS_RECVPACKET );			counter->Start();
		counter = TestCounter::Instance()->Add( COUNTER_RTTS_RECVTRANSBYTES );		counter->Start();

		counter = TestCounter::Instance()->Add( COUNTER_SKEL_SENDPACKET );			counter->Start();
		counter = TestCounter::Instance()->Add( COUNTER_SKEL_SENDTRANSBYTES );		counter->Start();
		counter = TestCounter::Instance()->Add( COUNTER_SKEL_RECVPACKET );			counter->Start();
		counter = TestCounter::Instance()->Add( COUNTER_SKEL_RECVTRANSBYTES );		counter->Start();
	}
#endif

	// Resource에서 rttagent의 제품 버젼 정보를 가져온다.
	Entry::Instance()->SetVersion(GetFileVersion("rttagent.exe"));

	OutputDebug( "[RTTAgent Version]%s(0x00%x)\n\n", SERVICE_VERSION, Entry::Instance()->GetVersion() );
	OutputDebug( "[RTTAgent Information] %s/%d\n", m_agentIPAddr, m_agentPortNo );
	OutputDebug( "[RTTServer Information] %s/%d\n\n", m_serverIPAddr, m_serverPortNo );

	//  Run thread 가동
	{
		m_eventKill = ::CreateEvent( 0, 0, 0, 0 );
		if( NULL == m_eventKill)
		{
			OutputDebug( "[Entry::Initialize] CreateEvent Error %d \n", ::GetLastError() );
		}

		bool bRet = Start();
		if( false == bRet )
		{
			OutputDebug( "[Entry::Initialize] Start Thread Error(%d)\n", errno );
		}
		else
		{
			OutputDebug( "[Entry::Initialize] Start Thread\n" );
		}
	}

	return true;
}

void Entry::Destroy()
{
#ifdef  TESTCOUNTER
	//  TestCounter 종료
	{
		TestCounter::Counter*	counter[4];

		counter[0] = TestCounter::Instance()->Get( COUNTER_RTTS_SENDPACKET );			counter[0]->Stop();
		counter[1] = TestCounter::Instance()->Get( COUNTER_RTTS_SENDTRANSBYTES );		counter[1]->Stop();
		counter[2] = TestCounter::Instance()->Get( COUNTER_RTTS_RECVPACKET );			counter[2]->Stop();
		counter[3] = TestCounter::Instance()->Get( COUNTER_RTTS_RECVTRANSBYTES );		counter[3]->Stop();

		OutputDebug( "** TestCounter Report **\n" );
		OutputDebug( "[RTTServer]\n" );
		OutputDebug( "Test time : %d ms\n", counter[0]->GetTimeGap() );
		OutputDebug( "Sent packet count = %d(%d)\n", counter[0]->GetCount(), counter[0]->GetCount() / 1000 );
		OutputDebug( "Sent transbytes = %d(%d)\n",   counter[1]->GetCount(), counter[1]->GetCount() / 1000 );
		OutputDebug( "Recv packet count = %d(%d)\n", counter[2]->GetCount(), counter[2]->GetCount() / 1000 );
		OutputDebug( "Recv transbytes = %d(%d)\n",   counter[3]->GetCount(), counter[3]->GetCount() / 1000 );



		counter[0] = TestCounter::Instance()->Get( COUNTER_SKEL_SENDPACKET );			counter[0]->Stop();
		counter[1] = TestCounter::Instance()->Get( COUNTER_SKEL_SENDTRANSBYTES );		counter[1]->Stop();
		counter[2] = TestCounter::Instance()->Get( COUNTER_SKEL_RECVPACKET );			counter[2]->Stop();
		counter[3] = TestCounter::Instance()->Get( COUNTER_SKEL_RECVTRANSBYTES );		counter[3]->Stop();

		OutputDebug( "[Skel]\n" );
		OutputDebug( "Test time : %d ms\n", counter[0]->GetTimeGap() );
		OutputDebug( "Sent packet count = %d(%d)\n", counter[0]->GetCount(), counter[0]->GetCount() / 1000 );
		OutputDebug( "Sent transbytes = %d(%d)\n",   counter[1]->GetCount(), counter[1]->GetCount() / 1000 );
		OutputDebug( "Recv packet count = %d(%d)\n", counter[2]->GetCount(), counter[2]->GetCount() / 1000 );
		OutputDebug( "Recv transbytes = %d(%d)\n",   counter[3]->GetCount(), counter[3]->GetCount() / 1000 );


		TestCounter::Instance()->RemoveAll();
	}
#endif

	ServiceTerminate();


	//  Run thread를 종료시킨다.
	{
		::SetEvent( m_eventKill );

		if( WaitForTerminate( 1000 ) == false ){
			Terminate( 0 );
		}

		::CloseHandle( m_eventKill );
		m_eventKill = INVALID_HANDLE_VALUE;
	}
}

void Entry::ServiceStartUp()
{
	m_networkSkel.Create( 10, ManagerSkel::Instance() );
	if( m_networkSkel.OpenAcceptor( m_agentIPAddr, ( u_short )m_agentPortNo ) == false )
	{
		OutputDebug( "[%s/%d] Binding failed.\n", m_agentIPAddr, m_agentPortNo );
		return;
	}

	ResourceMonitor::Instance()->Initialize();
	ProcessMon::Instance()->StartMonitor();

	m_serviceRun = true;
	OutputDebug( "!-*-*-* Service Started *-*-*-!\n\n" );
}

void Entry::ServiceTerminate()
{
	OutputDebug("[ServiceTerminate Start] \n");
	m_serviceRun = false;

	m_networkSkel.CloseAcceptor();
	
	ManagerSkel::Instance()->DisconnectAll();
	ManagerSkel::Instance()->Clear();			OutputDebug("[ServiceTerminate Start]ManagerSkel::Instance()->Clear() \n");	
	
	m_networkSkel.Destroy();					OutputDebug("[ServiceTerminate Start]m_networkSkel.Destroy() \n");

	ResourceMonitor::Instance()->Release();		OutputDebug("[ServiceTerminate Start]ResourceMonitor::Instance()->Release() \n");
	ProcessMon::Instance()->Destroy();			OutputDebug("[ServiceTerminate Start]ProcessMon::Instance()->Destroy() \n");
	DropStatePoll::Instance()->Destroy();		OutputDebug("[ServiceTerminate Start]DropStatePoll::Instance()->Destroy() \n");	
	
	OutputDebug( "!-*-*-* Service Closed *-*-*-!\n\n" );
}

void Entry::Run()
{
	m_networkRTTS.Create( 1, &m_managerRTTS );
	
	OutputDebug("[Entry::Run] start \n");
	while( true )
	{
		if( ::WaitForSingleObject( m_eventKill, 10 ) == WAIT_OBJECT_0 )
		{	
			OutputDebug("[Entry::Run]Thread Exit \n");
			break;
		}
		
		if( m_serviceRun == true )
		{
			m_networkSkel.Update();
		}

		m_networkRTTS.Update();
	}

	m_networkRTTS.Destroy();
	OutputDebug("[Entry::Run] end \n");
}

void Entry::ProcCrashHandling( LPCTSTR stackWalkLog, LPCTSTR dumpFileName )
{
	SYSTEMTIME	systemTime;
	TCHAR		temp[127] = {0, };

	GetLocalTime( &systemTime );
	sprintf( temp, "RTTAgent CrashLog_%d%02d%02d_%02d%02d%02d.txt",
		systemTime.wYear, systemTime.wMonth, systemTime.wDay,
		systemTime.wHour, systemTime.wMinute, systemTime.wSecond );

	FILE* fp = fopen( temp, "w" );
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