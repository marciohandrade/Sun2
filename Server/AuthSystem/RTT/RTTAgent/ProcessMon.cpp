#include "stdafx.h"
#include "ProcessMon.h"
#include "RTTServer.h"

#include <psapi.h>

#pragma comment(lib, "psapi.lib")


ProcessMon::ProcessMon()
	: m_eventKill( INVALID_HANDLE_VALUE ), m_patchKey( 0 )
{
}

ProcessMon::~ProcessMon()
{
}

void ProcessMon::StartMonitor()
{
	m_eventKill = ::CreateEvent( 0, 0, 0, 0 );

	bool bRet = Start();
	if( false == bRet )
	{
		OutputDebug( "[ProcessMon::StartMonitor] start thread error(%d) \n", errno );
	}
	else
	{
		OutputDebug( "[ProcessMon::StartMonitor] start thread \n" );
	}
//	eXtreme::Thread::Start();
}

void ProcessMon::Destroy()
{
	Clear();
	SetEvent( m_eventKill );				OutputDebug("[ProcessMon::Destroy]SetEvent \n");
	WaitForTerminate(3000);
}

void ProcessMon::Clear()
{
	MONITOR_PROCESS_INFO*	info = NULL;

	while( m_listMonProcess.size() > 0 )
	{
		info = m_listMonProcess.front();
		m_listMonProcess.pop_front();

		delete info;
	}
}

void ProcessMon::AddMonitorProcess( char* abPath, char* fileName, char* serviceName, bool isService )
{
	MONITOR_PROCESS_INFO*	info = new MONITOR_PROCESS_INFO;
	memset(info, 0, sizeof(MONITOR_PROCESS_INFO) );

	::strncpy( info->abPath, abPath, MAX_PATH );
	::strncpy( info->fileName, fileName, 100 );
	::strncpy( info->serviceName, serviceName, 20 );

	info->isService	= isService;
	info->isActive	= false;

	m_listMonProcess.push_back( info );
}

bool ProcessMon::CreateAllMonitorProcess()
{
	MONITOR_PROCESS_INFO*		info = NULL;
	bool						ret = true;

	for(ListMonProcess::iterator iter = m_listMonProcess.begin() ; iter != m_listMonProcess.end() ; iter++ )
	{
		info = *iter;

		//  하나 이상 실패하게 되면 false를 반환한다.
		if( CreateMonitorProcess( info->abPath, info->fileName ) == false )
			ret = false;
	}

	return ret;
}

bool ProcessMon::CreateMonitorProcess( char* abPath, char* fileName )
{
	STARTUPINFO				startUpInfo = { 0 };
	PROCESS_INFORMATION		processInfo;
	char					tmp[MAX_PATH] = {0, };
/*
	startUpInfo.lpDesktop = "WinSta0\\default";
*/
	sprintf( tmp, "%s\\%s", abPath, fileName );

	if(IsRunningProcess(abPath, fileName)==false){
		ZeroMemory( &startUpInfo, sizeof( STARTUPINFO ) );
		startUpInfo.cb			= sizeof( STARTUPINFO );
		startUpInfo.lpDesktop	= 0;

		if( ::CreateProcess( 0, tmp, 0, 0, FALSE, CREATE_NEW_CONSOLE, 0, abPath, &startUpInfo, &processInfo ) == FALSE )
		{
			OutputDebug( "CreateProcess failed(=%d). '%s'\n", ::GetLastError(), tmp );
			return false;
		}

		::CloseHandle( processInfo.hThread );
		::CloseHandle( processInfo.hProcess );

		OutputDebug( "'%s' process created.\n", tmp );
	}
	else{
		OutputDebug( "'%s' running process.\n", tmp );
	}

	return true;
}

bool ProcessMon::KillMonitorProcess( char* abPath, char* fileName )
{
	eXtreme::Guard	< eXtreme::Mutex >	guard( m_mutexProcess );
	HANDLE				handleProcess;
	PRUNNING_PROC_INFO	rpi = 0;

	for(DWORD i = 0 ; i < m_vecProcess.size() ; i++ )
	{
		rpi = &m_vecProcess.at( i );

		handleProcess = OpenProcess( PROCESS_ALL_ACCESS, 0, rpi->ProcessID );

		if( handleProcess != 0 )
		{
			HMODULE		hMod;
			DWORD		cbNeeded = 0;

			if( EnumProcessModules( handleProcess, &hMod, sizeof( hMod ), &cbNeeded ) )
			{
				char	path[MAX_PATH] = "<unknown>";
				char	drive[4] = {0, };
				char	dir[MAX_PATH] = {0, };
				char	fname[64] = {0, };
				char	ext[64] = {0, };
			    char	processName[MAX_PATH] = "<unknown>";
				char	processPath[MAX_PATH] = {0, };;

				GetModuleFileNameEx( handleProcess, hMod, path, MAX_PATH );

				_splitpath( path, drive, dir, fname, ext );
				sprintf( processName, "%s%s", fname, ext );
				sprintf( processPath, "%s%s", drive, dir );

				int len = ( int )strlen( processPath );
				if( processPath[len - 1] == '\\' )
					processPath[len - 1] = 0;


				if( strcmp(_strupr(processPath), _strupr(abPath)) == 0 && strcmp(_strupr(processName),_strupr(fileName)) == 0 )
				{
					BOOL ret = TerminateProcess( handleProcess, 0 );

					if( ret == false )
						OutputDebug( "Failed to killProcess(=%d). '%s\\%s'\n", ::GetLastError(), processPath, processName );
					else
						OutputDebug( "'%s\\%s' process killed.\n", processPath, processName );

					return true;
				}
			}
		}

		CloseHandle( handleProcess );
	}

	return false;
}

void ProcessMon::CheckActiveProcess()
{
	MONITOR_PROCESS_INFO*		info = NULL;
	
	for(ListMonProcess::iterator iter = m_listMonProcess.begin() ; iter != m_listMonProcess.end() ; iter++ )
	{
		info = *iter;
		info->isActive = IsRunningProcess( info->abPath, info->fileName );
		if(info->isActive==false)
		{
			OutputDebug( "[ProcessMon::CheckActiveProcess] %s\\%s  is not running\n", info->abPath, info->fileName );
		}
	}
}

bool ProcessMon::IsRunningProcess( char* abPath, char* fileName )
{
	eXtreme::Guard	< eXtreme::Mutex >	guard( m_mutexProcess );

/*	PRUNNING_PROC_INFO	rpi = 0;
	char				fullName[MAX_PATH] = {0, };

	sprintf( fullName, "%s\\%s", abPath, fileName );
	strlwr( fullName );

	for(DWORD i = 0 ; i < m_vecProcess.size() ; i++ )
	{
		rpi = &m_vecProcess.at( i );

		strlwr( rpi->FullExeFile );
		
		OutputDebug( "[ProcessMon::IsRunningProcess] %s %s \n", rpi->FullExeFile, fullName );

		if( strncmp( fullName, rpi->FullExeFile, MAX_PATH ) == 0 )
		{
			return true;
		}
	}
*/

	// 07-05-01, 김종호
	// 경로가 바뀔수도 있으므로 절대경로로 비교하지 않고 파일이름만 검색한다
	PRUNNING_PROC_INFO	rpi = 0;
	char szFileName[MAX_PATH] = {0, };

	sprintf( szFileName, "%s", fileName );
	strlwr( szFileName );

	for(DWORD i = 0 ; i < m_vecProcess.size() ; i++ )
	{
		rpi = &m_vecProcess.at( i );

		strlwr( rpi->ExeFile );

		if( strncmp( szFileName, rpi->ExeFile, MAX_PATH ) == 0 )
		{
			return true;
		}
	}

	return false;
}

void ProcessMon::Run()
{
	bool bRun = true;
	while( bRun )
	{
		if( ::WaitForSingleObject( m_eventKill, 5000 ) == WAIT_OBJECT_0 )
		{
			::CloseHandle(m_eventKill);
			m_eventKill = INVALID_HANDLE_VALUE;

			OutputDebug("[ProcessMon::Run]Thread Exit \n");
			break;
		}

		{
			eXtreme::Guard	< eXtreme::Mutex >	guard( m_mutexProcess );
			m_vecProcess.clear();
			EnumProcs( &m_vecProcess );
		}

		CheckActiveProcess();
		SendProcessStatus();
	}
}

void ProcessMon::SendProcessStatus()
{
	MSG_RTTR_PROCESSMONITOR_NTF		sendPacket;
	MONITOR_PROCESS_INFO*			info = NULL;
	ListMonProcess::iterator iter;
	int i = 0;

	sendPacket.dwCount = ( DWORD )m_listMonProcess.size();
	for(iter = m_listMonProcess.begin(), i = 0 ; iter != m_listMonProcess.end() ; iter++, i++ )
	{
		info = *iter;

		::MultiByteToWideChar( CP_ACP, 0, info->fileName, -1, sendPacket.szBinaryName[i], 100 );
		::MultiByteToWideChar( CP_ACP, 0, info->abPath, -1, sendPacket.szAbsolutePath[i], 100 );

		sendPacket.dwStatus[i] = ( info->isActive == true ? 1 : 0 );

		//OutputDebug( "[ProcessMon::SendProcessStatus] %s\\%s(%d)\n", info->abPath, info->fileName, info->isActive );
	}

	RTTServer::instance->SendMessage( sendPacket, sendPacket.dwSize + sizeof( MSG_HEADER8 ) );
}