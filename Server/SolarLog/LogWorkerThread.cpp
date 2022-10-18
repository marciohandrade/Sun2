#include "StdAfx.h"
#include ".\logworkerthread.h"
#include ".\SolarLog.h"
#include <RingBuffer.h>
#include <stdio.h>
#include ".\SolarLog.Internal.h"

LogWorkerThread::LogWorkerThread( SolarLog & sl )
	:	m_solarlog ( sl )
{
	m_hEndThread = CreateEvent( NULL, FALSE, FALSE, NULL );
}

LogWorkerThread::~LogWorkerThread()
{
	CloseHandle( m_hEndThread );
}

VOID LogWorkerThread::EndThread()
{
	SetEvent( m_hEndThread );
	WaitForSingleObject( GetHandle(), INFINITE );
}

BOOL LogWorkerThread::_SetEndOfFile()
{
	m_readPointer.LowPart = GetFileSize( m_solarlog._IN->m_hFile, &m_readPointer.HighPart );

	if( m_readPointer.LowPart == INVALID_FILE_SIZE && GetLastError() != NO_ERROR )
	{ 
		m_solarlog._IN->_setLastError("GetFileSize() Error (%d) ", GetLastError() );
		return FALSE;
	}
/*
	m_readPointer.QuadPart	= 0;
	m_readPointer.LowPart	= SetFilePointer(m_solarlog.m_hFile, m_readPointer.LowPart, &m_readPointer.HighPart, FILE_END);
	if (m_readPointer.LowPart == INVALID_SET_FILE_POINTER && GetLastError() != NO_ERROR)
	{
		m_readPointer.QuadPart = -1;
		return FALSE;
	}
*/

	return TRUE;
}

DWORD LogWorkerThread::run() 
{ 
	HANDLE hHandle[2];
	hHandle[0]					= m_solarlog._IN->m_hLWWakeupEvent;
	hHandle[1]					= m_hEndThread;
//	DWORD timeout				= 1*1000;							//< 타임아웃의 체크 시간 
	
	if( !_SetEndOfFile() )
		return 1;

	BOOL bEventInfiniteLoop = TRUE;
	while( bEventInfiniteLoop )
	{
		switch( WaitForMultipleObjects( 2, hHandle, FALSE, INFINITE ) )
		{
		case WAIT_OBJECT_0:
			{
				if( m_solarlog._IN->_changeFile() )
					_SetEndOfFile();

				_OPERATE();
			}
			break;
		//case WAIT_TIMEOUT:
		//	{
		//		// 날짜가 바뀌면 파일 이름 바꾸기
		//		m_solarlog._changeFile();
		//	}
		//	break;
		case WAIT_OBJECT_0+1:
			{
				if( m_solarlog._IN->_changeFile() )
					_SetEndOfFile();

				_OPERATE();
				return 0;
			}
			
		}
	}

	return 0; 
}

VOID LogWorkerThread::_OPERATE()
{
	BOOL ret					= FALSE;
	DWORD dwTran				= 0;
	DWORD dwWritten				= 0;
	DWORD dwSize				= 0;
	BOOL loop					= TRUE;

	while( loop )
	{
		if( m_solarlog._IN->_getPushBuffer()->IsEmpty() == TRUE ) return;

		::EnterCriticalSection( m_solarlog._IN->m_pLogBufferCS );
		m_solarlog._IN->_switchBuffer();
		::LeaveCriticalSection( m_solarlog._IN->m_pLogBufferCS );

		BOOL loop2 = TRUE;
		while( loop2 )
		{
			m_ovl.Offset		= m_readPointer.LowPart;
			m_ovl.OffsetHigh	= m_readPointer.HighPart;
			m_ovl.hEvent		= NULL;

			//SetEndOfFile( m_solarlog.m_hFile );
			//SetFilePointer( m_solarlog.m_hFile, 0, &high, FILE_END );
			BYTE * pFrontPtr = m_solarlog._IN->_getPopBuffer()->GetContigousPtr((INT&)dwSize);
			if( dwSize == 0 ) 
			{
				break;
			}
			ret = WriteFile( m_solarlog._IN->m_hFile, pFrontPtr, dwSize, &dwWritten, &m_ovl );
			if( FALSE == ret && GetLastError() != ERROR_IO_PENDING )
			{
				m_solarlog._IN->_setLastError("WriteFile() Error (%d) ", GetLastError() );
				continue;
			}

			ret = GetOverlappedResult( m_solarlog._IN->m_hFile, &m_ovl, &dwTran, TRUE );
			if( ret && dwTran == dwSize )
			{
				m_readPointer.QuadPart += dwSize;
				m_solarlog._IN->_getPopBuffer()->Skip( dwSize );
				continue;
			}
			else
			{
				m_solarlog._IN->_setLastError("GetOverlappedResult() Error (%d) ", GetLastError() );
				continue;
			}
		}
	}
}