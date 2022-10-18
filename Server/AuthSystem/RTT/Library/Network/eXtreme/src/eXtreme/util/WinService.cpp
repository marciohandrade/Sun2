#include <eXtreme/Common.h>
#include <eXtreme/util/WinService.h>

using namespace eXtreme;

WinService::WinService()
	: m_handleServiceEvent( 0 ), m_bGetOSVersion(false)
{
	GetOSVersion();		// OS 의 버전을 가져온다
}

WinService::~WinService()
{
	if( m_handleServiceEvent != 0 )
		::CloseHandle( m_handleServiceEvent );
}

void WinService::AddEventLog( char* sourceName, EventLogType type, char* format, ... )
{
	char		buffer[1024] = {0, };
	va_list		args;

	va_start( args, format );
	wvsprintf( buffer, format, args );
	va_end( args );

	void* handleEventSource = ::RegisterEventSource( 0, sourceName );
    if( handleEventSource != 0 )
	{
		LPCTSTR		temp = buffer;

		::ReportEvent( handleEventSource, (WORD)type, 0, 0, 0, 1, 0, &temp, 0 );
		::DeregisterEventSource( handleEventSource );
    }
}

bool WinService::UploadService(char* serviceName, 
							   char* displayName, 
							   char* binaryPath,
							   char* serviceLogin, 
							   char* password, 
							   bool autoStart)
{
	return UploadService(serviceName, displayName, binaryPath, serviceLogin, password, NULL, autoStart);
}

/**
 2006.08.10
 add by guiun
 서비스 종속성을 가질 수 있는 서비스 등록 추가
*/
bool WinService::UploadService(char* serviceName, 
							   char* displayName, 
							   char* binaryPath,
							   char* serviceLogin, 
							   char* password, 
							   char* dependency,
							   bool /*autoStart*/ )
{
	SC_HANDLE handleManager = ::OpenSCManager( 0, 0, SC_MANAGER_ALL_ACCESS );
	if( handleManager == 0 )
	{
		return false;
	}

	//DWORD dwStartType = (autoStart==true) ? SERVICE_AUTO_START : SERVICE_DEMAND_START;

	SC_HANDLE handleService = ::CreateService(handleManager, 
									serviceName, 
									displayName, 
									SERVICE_ALL_ACCESS,
//									SERVICE_INTERACTIVE_PROCESS | SERVICE_WIN32_OWN_PROCESS, 
									SERVICE_WIN32_OWN_PROCESS,
									SERVICE_AUTO_START, 
									SERVICE_ERROR_NORMAL, 
									binaryPath, 
									NULL, 
									NULL, 
									dependency,				
									serviceLogin, 
									password);

	if( handleService == 0 )
	{
		::CloseServiceHandle( handleManager );

		return false;
	}

	::CloseServiceHandle( handleService );
	::CloseServiceHandle( handleManager );

	return true;
}

bool WinService::RemoveService( char* serviceName )
{
	SC_HANDLE handleManager = ::OpenSCManager( 0, 0, SC_MANAGER_CREATE_SERVICE );
	if( handleManager == 0 )
	{
		return false;
	}

	SC_HANDLE handleService = ::OpenService( handleManager, serviceName, DELETE );
	if( handleService == 0 )
	{
		::CloseServiceHandle( handleManager );
		return false;
	}

	::DeleteService( handleService );

	::CloseServiceHandle( handleService );
	::CloseServiceHandle( handleManager );

	return true;
}

bool WinService::StartDispatcher( char* serviceName, FN_StartCallback fnStart, FN_StopCallback fnStop, FN_UpdateCallback fnUpdate )
{
	SERVICE_TABLE_ENTRY dispatch[] =
	{
		{ m_serviceName, ServiceLoop },
		{ 0, 0 }
	};

	::strcpy( m_serviceName, serviceName );

	m_serviceStartFunc	= fnStart;
	m_serviceStopFunc	= fnStop;
	m_serviceUpdateFunc = fnUpdate;
	if( ::StartServiceCtrlDispatcher( dispatch ) == 0 )
	{
		return false;
	}

	return true;
}

void WinService::BreakOutDispatcher()
{
	m_statusEvent = SERVICE_CONTROL_STOP;
	::SetEvent( m_handleServiceEvent );
}

bool WinService::ChangeServiceInfo( char* netMachine,  char* serviceName,
											char* account, char* password, DWORD startType )
{
	SC_HANDLE handleService = ::OpenSCManager( netMachine, 0, GENERIC_ALL );
	if( handleService == 0 )
	{
		return false;
	}

	SC_HANDLE handleServerConnect = ::OpenService( handleService, serviceName, SERVICE_CHANGE_CONFIG );
	if( handleServerConnect == 0 )
	{
		::CloseServiceHandle( handleService );

		return false;
	}

	BOOL ret = ::ChangeServiceConfig( handleServerConnect, SERVICE_WIN32_OWN_PROCESS, startType,
							SERVICE_ALL_ACCESS, 0, 0, 0, 0, account, password, 0 );

	return ( ret == TRUE );
}

DWORD WinService::QueryService( char* netMachine, char* serviceName, QUERY_SERVICE_CONFIG* config, DWORD bufSize )
{
	SC_HANDLE handleService = ::OpenSCManager( netMachine, 0, GENERIC_READ );
	if( handleService == 0 )
		return ::GetLastError();


	//  해당 서비스의 정보를 얻어온다.
	if( config != 0 )
	{
		DWORD		configBytes = 0;

		SC_HANDLE handleConfig = ::OpenService( handleService, serviceName, SERVICE_QUERY_CONFIG );

		::QueryServiceConfig( handleConfig, config, bufSize, &configBytes );
		::CloseServiceHandle( handleConfig );
	}


	//  해당 서비스의 상태를 얻어온다.
	SC_HANDLE handleServerConnect = ::OpenService( handleService, serviceName, SERVICE_INTERROGATE );
	if( handleServerConnect == 0 )
	{
		::CloseServiceHandle( handleService );

		return ::GetLastError();
	}

	do
	{
		::ControlService( handleServerConnect, SERVICE_CONTROL_INTERROGATE, &m_ssQueryControl );
	} while( m_ssQueryControl.dwCurrentState != SERVICE_STOPPED &&
			m_ssQueryControl.dwCurrentState != SERVICE_RUNNING &&
			m_ssQueryControl.dwCurrentState != SERVICE_PAUSED &&
			m_ssQueryControl.dwCurrentState != 1 && handleServerConnect != 0 );

	switch( m_ssQueryControl.dwCurrentState )
	{
	case SERVICE_STOPPED:
	case SERVICE_RUNNING:
	case SERVICE_PAUSED:
		break;
	default: 
		m_ssQueryControl.dwCurrentState = 0;
		break;
	}

	::CloseServiceHandle( handleServerConnect );
	::CloseServiceHandle( handleService );

	return m_ssQueryControl.dwCurrentState;
}

DWORD WinService::StartService( char* netMachine, char* serviceName )
{
	DWORD dwErrorCode = ERROR_SUCCESS;

	SC_HANDLE handleService = ::OpenSCManager( netMachine, 0, SC_MANAGER_CONNECT );

	if( handleService == 0 ){
		dwErrorCode = ::GetLastError();
		return dwErrorCode;		
	}

	SC_HANDLE handleServerConnect = ::OpenService( handleService, serviceName, SERVICE_START | SERVICE_QUERY_STATUS );
	if( handleServerConnect == 0 )
	{
		::CloseServiceHandle( handleService );

		dwErrorCode = ::GetLastError();
		return dwErrorCode;
	}

	if(::StartService( handleServerConnect, 0, 0 ) == TRUE )
	{
		::Sleep( m_ssQueryControl.dwWaitHint );
		::QueryServiceStatus( handleServerConnect, &m_ssQueryControl );
	}

	::CloseServiceHandle( handleServerConnect );
	::CloseServiceHandle( handleService );

	
	return m_ssQueryControl.dwCurrentState;
}

DWORD WinService::StopService( char* netMachine, char* serviceName )
{
	SC_HANDLE handleService = ::OpenSCManager( netMachine, 0, SC_MANAGER_ALL_ACCESS );

	if( handleService == 0 )
		return ::GetLastError();

//	SC_HANDLE handleServerConnect = ::OpenService( handleService, serviceName, GENERIC_EXECUTE );

	SC_HANDLE handleServerConnect = ::OpenService( handleService, serviceName, SC_MANAGER_ALL_ACCESS );
	
	if( handleServerConnect == 0 )
	{
		::CloseServiceHandle( handleService );

		return ::GetLastError();
	}

	::ControlService( handleServerConnect, SERVICE_CONTROL_STOP, &m_ssQueryControl );

	::Sleep(m_ssQueryControl.dwWaitHint);
	::ControlService( handleServerConnect, SERVICE_CONTROL_INTERROGATE, &m_ssQueryControl );

	::CloseServiceHandle( handleServerConnect );
	::CloseServiceHandle( handleService );
	
	return m_ssQueryControl.dwCurrentState;
}

bool WinService::ConnectNet( char* account, char* password, char* netMachine )
{
	char		temp[128] = {0, };
	NETRESOURCE	netInfo;

	::memset( &netInfo, 0, sizeof( netInfo ) );
	::sprintf( temp, "\\\\%s", netMachine );

	netInfo.dwType			= RESOURCETYPE_ANY;
	netInfo.lpLocalName		= 0;
	netInfo.lpRemoteName	= temp;
	netInfo.lpProvider		= 0;

	return ( ::WNetAddConnection2( &netInfo, password, account, CONNECT_UPDATE_PROFILE | CONNECT_INTERACTIVE) == NO_ERROR );
}

bool WinService::CancelNet( char* netMachine )
{
	char		temp[128] = {0, };

	::sprintf( temp, "\\\\%s", netMachine );

	return ( ::WNetCancelConnection2( temp, 0, 0 ) == NO_ERROR );
}

void __stdcall WinService::ServiceHandler( DWORD status )
{
	Instance()->m_statusEvent = status;
	::SetEvent( Instance()->m_handleServiceEvent );
}

void __stdcall WinService::ServiceLoop( DWORD /*argc*/, char** /*argv*/ )
{
	bool	loop			= true;
	DWORD	serviceState	= SERVICE_RUNNING;

	if( Instance()->OnServiceStart() == false )
	{
		Instance()->OnServiceStop();
		
		return;
	}


	while( loop )
	{
		if( Instance()->IsServiceUpdate() )
		{
			// sun network library
			if( !Instance()->OnServiceUpdate() )
			{
				serviceState	= SERVICE_STOP_PENDING;
				loop			= false;
				continue;
			}
		}
		else
		{
			// rnd network library
			if ( WAIT_TIMEOUT == ::WaitForSingleObject( Instance()->m_handleServiceEvent, 60000 ) )
			{
				continue;
			}
		}

		switch( Instance()->m_statusEvent )
		{
		case  SERVICE_CONTROL_STOP :
			serviceState	= SERVICE_STOP_PENDING;
			loop			= false;
			break;

		case  SERVICE_CONTROL_CONTINUE :
			serviceState	= SERVICE_RUNNING;
			break;

		case  SERVICE_CONTROL_PAUSE :
			serviceState	= SERVICE_PAUSED;
			break;

		case  SERVICE_CONTROL_SHUTDOWN :
			serviceState	= SERVICE_STOP_PENDING;
			loop			= false;
			break;

		case  SERVICE_CONTROL_INTERROGATE :
			break;

		default :
			break;
		}
	}
	
	Instance()->OnServiceStop();
}

bool WinService::OnServiceStart()
{
	SERVICE_STATUS ss;

	m_handleServiceEvent			= ::CreateEvent( 0, 0, 0, 0 );
	m_handleServiceStatus			= ::RegisterServiceCtrlHandler( m_serviceName, ServiceHandler );

//	ss.dwServiceType				= SERVICE_WIN32_OWN_PROCESS | SERVICE_INTERACTIVE_PROCESS;
	ss.dwServiceType				= SERVICE_WIN32_OWN_PROCESS;
	ss.dwCurrentState				= SERVICE_START_PENDING;
	ss.dwWin32ExitCode				= NO_ERROR;
	ss.dwServiceSpecificExitCode	= 0;
	ss.dwCheckPoint					= 1;
	ss.dwWaitHint					= 1000;
	ss.dwControlsAccepted			= GetControlAccepted();		// 컨트롤 옵션을 가져온다
	::SetServiceStatus( m_handleServiceStatus, &ss );

	if( m_serviceStartFunc != 0 && ( *m_serviceStartFunc )() == false )
	{
		return false;
	}

	ss.dwCurrentState = SERVICE_RUNNING;
	ss.dwCheckPoint	  = 0;
	ss.dwWaitHint	  = 0;
	::SetServiceStatus( m_handleServiceStatus, &ss );

	return true;
}

bool WinService::OnServiceUpdate()
{
	if( m_serviceUpdateFunc != 0 )
		return ( *m_serviceUpdateFunc )();
	return true;
}

bool WinService::IsServiceUpdate()
{
	if( m_serviceUpdateFunc ) return true;
	return false;
}

void WinService::OnServiceStop()
{
	SERVICE_STATUS ss;

	ss.dwServiceType				= SERVICE_WIN32_OWN_PROCESS;
	ss.dwWin32ExitCode				= NO_ERROR;
	ss.dwServiceSpecificExitCode	= 0;
	ss.dwCurrentState				= SERVICE_STOP_PENDING;
	ss.dwCheckPoint					= 1;
	ss.dwWaitHint					= 5000;
	ss.dwControlsAccepted			= GetControlAccepted();		// 컨트롤 옵션을 가져온다
	::SetServiceStatus( m_handleServiceStatus, &ss );

	if( m_serviceStopFunc != 0 )
		( *m_serviceStopFunc )();

	ss.dwCurrentState				= SERVICE_STOPPED;
	ss.dwCheckPoint					= 0;
	ss.dwWaitHint					= 0;
	::SetServiceStatus( m_handleServiceStatus, &ss );

	if( m_handleServiceEvent == INVALID_HANDLE_VALUE )
	{
		::CloseHandle( m_handleServiceEvent );
		m_handleServiceEvent = INVALID_HANDLE_VALUE;
	}
}

bool WinService::LocalSystemShutdown( LPTSTR msg, DWORD timeout, BOOL bReboot )
{
	HANDLE				handleToken;

	if( ::OpenProcessToken( GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &handleToken ) == FALSE ) 
		return FALSE;

	//  Get the LUID for shutdown privilege.
	TOKEN_PRIVILEGES	tkp;
	LookupPrivilegeValue( 0, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid );
	tkp.PrivilegeCount				= 1;
	tkp.Privileges[0].Attributes	= SE_PRIVILEGE_ENABLED;


	//  Get shutdown privilege for this process. 
	::AdjustTokenPrivileges( handleToken, FALSE, &tkp, 0, ( PTOKEN_PRIVILEGES )0, 0 );


	//  Cannot test the return value of AdjustTokenPrivileges.
	if( ::GetLastError() != ERROR_SUCCESS )
	{
		return false;
	}

	//  Display the shutdown dialog box and start the countdown. 
	BOOL result = ::InitiateSystemShutdown( 0,	// shut down local computer 
									msg,		// message for user
									timeout,	// time-out period 
									FALSE,		// ask user to close apps
                                    bReboot);	// reboot after shutdown 

	if( result == FALSE )
	{
		return false; 
	}

	//  Disable shutdown privilege. 
	if( msg != 0 )
	{
		tkp.Privileges[0].Attributes = 0;

		AdjustTokenPrivileges( handleToken, FALSE, &tkp, 0, ( PTOKEN_PRIVILEGES ) 0, 0 );
	}

	return true;
}

void WinService::GetOSVersion()
{
	ZeroMemory(&m_OSVersion, sizeof(OSVERSIONINFOEX));
	m_OSVersion.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

	if( !GetVersionEx ((OSVERSIONINFO *) &m_OSVersion) )
	{
		m_OSVersion.dwOSVersionInfoSize = sizeof (OSVERSIONINFO);
		if( !GetVersionEx( (OSVERSIONINFO *) &m_OSVersion) )
		{
			return;
		}
	}
	m_bGetOSVersion = true;
}

DWORD WinService::GetControlAccepted()
{
    bool bWin2000_OR_NT = true;

	if( m_bGetOSVersion && VER_PLATFORM_WIN32_NT == m_OSVersion.dwPlatformId )
	{
		if ( m_OSVersion.dwMajorVersion == 5 && m_OSVersion.dwMinorVersion > 0 )
		{
			bWin2000_OR_NT = false;
		}
	}

	DWORD	dwControlAccepted = 0;

	if(bWin2000_OR_NT)
	{
		dwControlAccepted = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN;
	}
	else
	{
		dwControlAccepted = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN | SERVICE_ACCEPT_SESSIONCHANGE;
	}

	return dwControlAccepted;
}