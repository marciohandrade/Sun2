#include "stdafx.h"
#include "ResourceMonitor.h"
#include "RTTServer.h"

#include "ProcessMon.h"

ResourceMonitor::ResourceMonitor()
{
}

ResourceMonitor::~ResourceMonitor()
{
}

void ResourceMonitor::Initialize()
{
	HANDLE fileHandle = CreateFile( ".\\SystemInfo.inf", GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, CREATE_ALWAYS, 0, 0 );
	::CloseHandle( fileHandle );

	m_eventKill		= ::CreateEvent( 0, 0, 0, 0 );
	//m_handleThread	= ::CreateThread( 0, 0, Runner, this, 0, &m_threadID );
	u_int uiThreadID = 0;
	m_handleThread = (HANDLE)_beginthreadex(0, 0, Runner, this, 0, &uiThreadID);
	m_threadID = uiThreadID;

	memset( &m_currentInfo, 0, sizeof( m_currentInfo ) );
}

void ResourceMonitor::Release()
{
	SetEvent( m_eventKill );							OutputDebug("[ResourceMonitor::Release]SetEvent \n");
	::WaitForSingleObject( m_handleThread, 1000 );		OutputDebug("[ResourceMonitor::Release]WaitForSingleObject \n");
}

u_int __stdcall ResourceMonitor::Runner( LPVOID parameter )
{
	ResourceMonitor*		instance = ( ResourceMonitor* )parameter;
	STARTUPINFO				startUpInfo = { 0, };
	PROCESS_INFORMATION		processInfo;

	startUpInfo.cb = sizeof( STARTUPINFO );
	
	bool bRun = true;
	while( bRun )
	{
		if( ::WaitForSingleObject( instance->m_eventKill, 5000 ) == WAIT_OBJECT_0 )
		{
			::CloseHandle(instance->m_eventKill);
			instance->m_eventKill = INVALID_HANDLE_VALUE;

			OutputDebug("[ResourceMonitor::Runner]Thread Exit \n");
			break;
		}
		
		DWORD dwTickCount = ::GetTickCount();
		// wscript를 실행 시킨다.
		if(TRUE==::CreateProcess( 0, "wscript.exe .\\SystemInfo.vbs", 0, 0, FALSE, 0, 0, 0, &startUpInfo, &processInfo ))
		{
			// 10sec 동안 프로세스의 종료를 기다린다.
			DWORD dwResult = ::WaitForSingleObject( processInfo.hProcess, WAIT_RESOURCE_TIME );
			if( WAIT_OBJECT_0 == dwResult)
			{
				// 리소스 정보를 읽어 온다.
				instance->ReadInfo();
				// 리소스 정보를 전송한다.
				instance->SendReport();
			}
			else if( WAIT_TIMEOUT == dwResult)
			{
				// 대기 시간을 넘어가면 강제 종료를 시킨다.
				::TerminateProcess(processInfo.hProcess, 0);
				OutputDebug("[ResourceMonitor::Runner] Timeout(%d) \n", (::GetTickCount() - dwTickCount));
			}
			else	// 이 경우는 없어야 한다... 
			{
				// ERROR 강제 종료를 시킨다.
				TerminateProcess(processInfo.hProcess, 0);
				OutputDebug("[ResourceMonitor::Runner] TerminateProcess (%d) \n", dwResult);
			}

			::CloseHandle( processInfo.hThread  );
			::CloseHandle( processInfo.hProcess );
		}
		else
		{
			OutputDebug("[ResourceMonitor::Runner]CreateProcess Fail \n");
		}
	}
	
	_endthreadex(0);
	return 0;
}

void ResourceMonitor::ReadInfo()
{
	ResourceInfo	resInfo = { 0, 0, 0, 0, 0 };
	char			tmp[127] = {0, };
	int				i = 0;
	
	std::string strBuffer = FileRead();

	if(strBuffer.length()<=0) return;
	///////////////////////////////////////////////////////////////////////////////
	//  Memory 정보를 읽어온다.
	{
		__int64				totalSize = 0, usingSize = 0, readInt64 = 0;

		//  Total Size
		for( i = 0 ; i < 16 ; i++ )
		{
			sprintf( tmp, "Physical Total %d", i + 1 );

			readInt64 = FindInteger64Value(strBuffer, "[Memory]", tmp);//GetPrivateProfileInt( "Memory", tmp, -1, ".\\SystemInfo.inf" );
			if( readInt64 == -1 )
				break;

			totalSize += readInt64;
		}

		//  Available Size
		readInt64 = FindInteger64Value(strBuffer, "[Memory]", "AvailableBytes");//GetPrivateProfileInt( "Memory", "AvailableBytes", -1, ".\\SystemInfo.inf" );
		if( readInt64 != -1 )
			usingSize =  ( DWORD )( totalSize - readInt64 );

		resInfo.ramTotalSize = totalSize / ( 1024 * 1024 );
		resInfo.ramUsingSize = usingSize / ( 1024 * 1024 );
	}



	///////////////////////////////////////////////////////////////////////////////
	//  CPU 정보를 읽어온다.
	{
		DWORD		count = 0, total = 0;
		int				readInt = 0;

		for( i = 0 ; i < 32 ; i++ )
		{
			sprintf( tmp, "CPU%d", i + 1 );
			readInt = FindIntegerValue(strBuffer, "[CPU]", tmp);//GetPrivateProfileInt( "CPU", tmp, -1, ".\\SystemInfo.inf" );

			if( readInt == -1 )
				break;

			total += readInt;
			count++;
		}

		if( count > 0 )
			resInfo.cpuUsing = total / count;
	}


	///////////////////////////////////////////////////////////////////////////////
	//  HDD 정보를 읽어온다.
	{
		__int64		totalSize = 0, remainSize = 0;
//		DWORD		ret;
//		char		readBuffer[64];

		for( i = 'C' ; i < 'Z' ; i++ )
		{
			sprintf( tmp, "[HDD %c:]", i );

			totalSize += FindInteger64Value(strBuffer, tmp, "Size");//GetPrivateProfileString( tmp, "Size", "", readBuffer, 64, ".\\SystemInfo.inf" );
			//totalSize +=if( ret == 0 )
			//	break;
			//totalSize += _atoi64( readBuffer );
			
            			
			remainSize += FindInteger64Value(strBuffer, tmp, "FreeSpace");//GetPrivateProfileString( tmp, "FreeSpace", "", readBuffer, 64, ".\\SystemInfo.inf" );
			//if( ret == 0 )
			//	break;
			//remainSize += _atoi64( readBuffer );
		}

		resInfo.hddTotalSize	= ( DWORD )( totalSize / ( 1024 * 1024 ) );
		resInfo.hddUsingSize	= ( DWORD )( ( totalSize - remainSize ) / ( 1024 * 1024 ) );
	}


	memcpy( &m_currentInfo, &resInfo, sizeof( resInfo ) );
}

void ResourceMonitor::SendReport()
{
	MSG_RTTR_RESOURCE_NTF	sendPacket;

	sendPacket.dwCPU		= m_currentInfo.cpuUsing;
	sendPacket.dwMemoryUse	= m_currentInfo.ramUsingSize;
	sendPacket.dwMemoryMax	= m_currentInfo.ramTotalSize;
	sendPacket.dwHDDUse		= m_currentInfo.hddUsingSize;
	sendPacket.dwHDDMax		= m_currentInfo.hddTotalSize;
	
	if( m_currentInfo.ramTotalSize == 0 || m_currentInfo.hddTotalSize == 0 )
	{
		OutputDebug( "[ResourceMonitor::SendReport]CPU(%d/100) MEM(%d/%d) HDD(%d/%d)\n", 
			sendPacket.dwCPU,
			sendPacket.dwMemoryUse,
			sendPacket.dwMemoryMax,
			sendPacket.dwHDDUse,
			sendPacket.dwHDDMax);
		return;
	}

	RTTServer::instance->SendMessage( sendPacket, sizeof( sendPacket ) );	
}
std::string ResourceMonitor::FileRead(void)
{
	char* lpFileName = ".\\SystemInfo.inf";

	HANDLE hFile = CreateFile(TEXT(lpFileName),      // file to create
						   GENERIC_READ,          // open for writing
						   FILE_SHARE_READ,       // do not share
						   NULL,                  // default security
						   OPEN_ALWAYS,           // overwrite existing
						   FILE_ATTRIBUTE_NORMAL, // normal file
						   NULL);                 // no attr. template

#define MAX_READ_BUFFER 4096
	char inBuffer[MAX_READ_BUFFER] = {0, };
	DWORD dwBytesRead = 0;
	if(hFile!=INVALID_HANDLE_VALUE) 
	{ 

		BOOL bResult = ReadFile(hFile, &inBuffer, MAX_READ_BUFFER, &dwBytesRead, NULL) ; 
		// Check for end of file. 
		if (bResult &&  dwBytesRead == 0) 
		{ 
			// we're at the end of the file 
			CloseHandle(hFile);
			return "";
		} 
	}	
	CloseHandle(hFile);

	if(dwBytesRead>0){
		std::string strBuffer(inBuffer, dwBytesRead);
		return strBuffer;
	}
	else{
		return "";
	}
}

int ResourceMonitor::FindIntegerValue(std::string strBuffer, char* lpHeader, char* lpValueName)
{
	int retValue = -1;
    
	int nHeader = (int)strBuffer.find(lpHeader, 0);
	int nStart = (int)strBuffer.find(lpValueName, nHeader);
	if(nStart>=0){
		nStart = nStart + (int)strlen(lpValueName) + 1;
		
		int nEnd = (int)strBuffer.find("\r\n", nStart) - nStart;
		
		if(nEnd>0)
		{
			std::string strValue(strBuffer, nStart, nEnd);
			retValue = atoi(strValue.c_str());
		}
	}

	return retValue;
}

__int64 ResourceMonitor::FindInteger64Value(std::string strBuffer, char* lpHeader, char* lpValueName)
{
	__int64 retValue = -1;
    
	int nHeader = (int)strBuffer.find(lpHeader, 0);
	int nStart = (int)strBuffer.find(lpValueName, nHeader);

	if(nStart>=0){
		nStart = nStart + (int)strlen(lpValueName) + 1;
		
		int nEnd = (int)strBuffer.find("\r\n", nStart) - nStart;
		
		if(nEnd>0)
		{
			std::string strValue(strBuffer, nStart, nEnd);
			retValue = _atoi64(strValue.c_str());
		}
	}
	return retValue;
}
