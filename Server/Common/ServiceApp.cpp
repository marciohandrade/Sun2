#include "stdafx.h"
#include <stdio.h>
#include "ServiceApp.h"

ServiceApp::ServiceApp(const char* pServiceName)
{
	if( pServiceName )
		strcpy(m_ServiceName, pServiceName);
}

ServiceApp::~ServiceApp()
{
}

VOID	ServiceApp::SetServiceName( const char* pszServiceName )
{
	if( pszServiceName )
		strncpy( m_ServiceName, pszServiceName, 256 );
}


void ServiceApp::HandleStart (DWORD argc, LPTSTR *argv) 
{ 
	DWORD status; 
	DWORD specificError; 

	m_ServiceStatus.dwServiceType        = SERVICE_WIN32 | SERVICE_INTERACTIVE_PROCESS; 
	m_ServiceStatus.dwCurrentState       = SERVICE_START_PENDING; 
	m_ServiceStatus.dwControlsAccepted   = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_PAUSE_CONTINUE; 
	m_ServiceStatus.dwWin32ExitCode      = 0; 
	m_ServiceStatus.dwServiceSpecificExitCode = 0; 
	m_ServiceStatus.dwCheckPoint         = 0; 
	m_ServiceStatus.dwWaitHint           = 0; 

	m_ServiceStatusHandle = RegisterServiceCtrlHandler( m_ServiceName, m_pServiceCtrlHandler ); 

	if (m_ServiceStatusHandle == (SERVICE_STATUS_HANDLE)0) 
	{ 
		DebugOut( "RegisterServiceCtrlHandler failed %d\n", GetLastError()); 
		return; 
	} 

	// Initialization code goes here. 
	status = OnInit(argc,argv, &specificError); 

	// Handle error condition 
	if (status != NO_ERROR) 
	{ 
		m_ServiceStatus.dwCurrentState       = SERVICE_STOPPED; 
		m_ServiceStatus.dwCheckPoint         = 0; 
		m_ServiceStatus.dwWaitHint           = 0; 
		m_ServiceStatus.dwWin32ExitCode      = status; 
		m_ServiceStatus.dwServiceSpecificExitCode = specificError; 

		SetServiceStatus (m_ServiceStatusHandle, &m_ServiceStatus); 
		return; 
	} 

	// Initialization complete - report running status. 
	m_ServiceStatus.dwCurrentState       = SERVICE_RUNNING; 
	m_ServiceStatus.dwCheckPoint         = 0; 
	m_ServiceStatus.dwWaitHint           = 0; 

	if (!SetServiceStatus (m_ServiceStatusHandle, &m_ServiceStatus)) 
	{ 
		status = GetLastError(); 
		DebugOut("SetServiceStatus error %ld\n", status); 
	} 

	// This is where the service does its work. 
	DebugOut("Returning the Main Thread \n", 0); 

	Update();

	return; 
} 

VOID ServiceApp::HandleControl (DWORD Opcode) 
{ 
	DWORD status; 

	switch(Opcode) 
	{ 
		case SERVICE_CONTROL_PAUSE: 
			// Do whatever it takes to pause here. 
			m_ServiceStatus.dwCurrentState = SERVICE_PAUSED;
			OnPause();
		break; 

		case SERVICE_CONTROL_CONTINUE: 
			// Do whatever it takes to continue here. 
			m_ServiceStatus.dwCurrentState = SERVICE_RUNNING; 
			OnContinue();
		break; 

		case SERVICE_CONTROL_STOP: 
			// Do whatever it takes to stop here. 
			m_ServiceStatus.dwWin32ExitCode = 0; 
			m_ServiceStatus.dwCurrentState  = SERVICE_STOPPED; 
			m_ServiceStatus.dwCheckPoint    = 0; 
			m_ServiceStatus.dwWaitHint      = 0; 

			if (SetServiceStatus (m_ServiceStatusHandle, &m_ServiceStatus))
				OnStop();			
			else
			{ 
				status = GetLastError(); 
			//	DebugOut("SetServiceStatus error %ld\n", status); 
			}	

			//DebugOut("Leaving MyService \n", 0); 
		return;

		case SERVICE_CONTROL_INTERROGATE: 
			// Fall through to send current status. 
			OnInterrogate();
		break; 

		case SERVICE_CONTROL_SHUTDOWN:
			OnShutDown();
			break;

		default: 
			DebugOut("Unrecognized opcode %ld\n", Opcode); 
	} 

	// Send current status. 
	if (!SetServiceStatus (m_ServiceStatusHandle,  &m_ServiceStatus)) 
	{ 
		status = GetLastError(); 
		DebugOut("SetServiceStatus error %ld\n", status); 
	} 
}

VOID ServiceApp::StartService(LPSERVICE_MAIN_FUNCTION func, LPHANDLER_FUNCTION pCtrlHandler)
{
	//서비스 모드로 시작하면, 현재 디렉토리가 C:\windows\system 으로 된다.
	//따라서 현재 어플리케이션의 디렉토리로 설정해줘야 한다.
	SetApplicationDirectory();

	SERVICE_TABLE_ENTRY   DispatchTable[] = 
	{ 
		{ m_ServiceName, func      }, 
		{ NULL,          NULL      } 
	}; 

	m_pServiceCtrlHandler = pCtrlHandler;

	if( !StartServiceCtrlDispatcher( DispatchTable) )  
	{
		DWORD dwError = GetLastError();
		DISPMSG( "StartServiceCtrlDispatcher error = %d\n", GetLastError() );
//		DebugOut("StartServiceCtrlDispatcher error = %d\n", GetLastError()); 
	} 
}

VOID ServiceApp::DebugOut(LPSTR String, DWORD Status) 
{ 
	CHAR  Buffer[1024]; 
	if (strlen(String) < 1000) 
	{ 
		sprintf(Buffer, String, Status); 
		OutputDebugStringA(Buffer); 
	} 
}


VOID ServiceApp::SetApplicationDirectory()
{
	TCHAR szPath[MAX_PATH], szDir[MAX_PATH];
	GetModuleFileName( NULL, szPath, MAX_PATH );
	char* pBackSlash = strrchr( szPath, '\\');
	int nBackSlash = pBackSlash - szPath;
	strncpy( szDir, szPath, MAX_PATH );
	szDir[nBackSlash] = '\0';
	SetCurrentDirectory( szDir );
	DISPMSG( "Current Work Directory = %s", szDir );
}



