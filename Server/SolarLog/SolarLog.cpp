
#include "stdafx.h"
#include "SolarLog.h"
#include <TIME.H>
#include "LogWorkerThread.h"
#include <stdio.h>
#include <RingBufferEx.h>
#include <tchar.h>
#include ".\SolarLog.Internal.h"

SolarLog::SolarLog():_IN(new Internal)
{

}

SolarLog::~SolarLog()
{
	if( _IN->m_pWorkerThread )
	{
		_IN->m_pWorkerThread->EndThread();
		delete _IN->m_pWorkerThread;
		_IN->m_pWorkerThread = NULL;
	}
	SAFE_RELEASENDELETE( _IN->m_pLogBufferPush );
	SAFE_RELEASENDELETE( _IN->m_pLogBufferPop );

	if( _IN->m_pLogBufferCS )
	{
		::DeleteCriticalSection( _IN->m_pLogBufferCS );
		delete _IN->m_pLogBufferCS;
		_IN->m_pLogBufferCS = NULL;
	}

	CloseHandle( _IN->m_hLWWakeupEvent );
	CloseHandle( _IN->m_hFile );

	delete _IN;
	_IN = NULL;
}

BOOL SolarLog::Create( TCHAR * pszDirName, TCHAR * pszFileNamePrefix )
{
	ASSERT( !IsBadReadPtr( pszDirName, sizeof(pszDirName) ) );
	ASSERT( !IsBadReadPtr( pszFileNamePrefix, sizeof(pszFileNamePrefix) ) );

	struct tm * today;
	time_t		ltime;

	time( &ltime );
	today = localtime( &ltime );

	_IN->m_nYear  = today->tm_year + 1900;
	_IN->m_nMonth = today->tm_mon + 1;
	_IN->m_nDay   = today->tm_mday;
#ifdef _NA_007039_20140121_TIME_UNIT_TEXTLOG
    _IN->m_nHour  = today->tm_hour;
#endif //_NA_007039_20140121_TIME_UNIT_TEXTLOG

	_tcsncpy( _IN->m_pszDirName, pszDirName, MAX_PATH );
	_tcsncpy( _IN->m_pszFileNamePrefix, pszFileNamePrefix, MAX_PATH );

	if( !_IN->_createFile() ) return FALSE;

	/// 자료구조 초기화
	_IN->m_pLogBufferCS	= new CRITICAL_SECTION;
	::InitializeCriticalSection( _IN->m_pLogBufferCS );

	_IN->m_pLogBufferPush	= new util::RingBufferEx;
	_IN->m_pLogBufferPop	= new util::RingBufferEx;
	
    // (JAYKANG) (090812) (IMPROVEMENT)
    // 버퍼 크기를 102400(100K)에서 1048576(1M)로 증가
    // 0903 퀘스트 제한 확장 후 Snapshot 로그 단일 건 크기가 10K가 넘으므로
    // 버퍼 오버플로우 예방를 위해
    _IN->m_pLogBufferPush->Init(1048576);
	_IN->m_pLogBufferPop->Init(1048576);
	
    _IN->m_hLWWakeupEvent	= CreateEvent( NULL, FALSE, FALSE, NULL );

	/// 워커 스레드 생성
	_IN->m_pWorkerThread	= new LogWorkerThread( *this );
	_IN->m_pWorkerThread->StartThread();

	// 시작 틱저장
	_IN->m_dwTick = GetTickCount();


	return TRUE;
}
BOOL SolarLog::LOG( BYTE * pBuffer, DWORD dwSize )
{
	::EnterCriticalSection( _IN->m_pLogBufferCS );

	if( -1 == _IN->_getPushBuffer()->Write( pBuffer, dwSize ) )
	{
		///printf( "%u바이트넣기실패(%u:%u)\n", dwSize, _getPushBuffer()->GetDataSize(), _getPushBuffer()->GetFreeSize() );
		::LeaveCriticalSection( _IN->m_pLogBufferCS );
		_IN->_setLastError( "Buffer Put() in LOG() Error : 버퍼가 꽉 찼습니다." );
		return FALSE;
	}
	//printf( "%u바이트PUT(%u:%u)\n", dwSize, _getPushBuffer()->GetDataSize(), _getPushBuffer()->GetFreeSize() );


	::LeaveCriticalSection( _IN->m_pLogBufferCS );

	SetEvent( _IN->m_hLWWakeupEvent );

	return TRUE;
}
BOOL SolarLog::LOG( TCHAR * pszMessage )
{
	DWORD dwLength = (DWORD)_tcslen( pszMessage );

	::EnterCriticalSection( _IN->m_pLogBufferCS );

	if( !_IN->_getPushBuffer()->Write( (BYTE *)pszMessage, dwLength ) )
	{
		::LeaveCriticalSection( _IN->m_pLogBufferCS );
		_IN->_setLastError( "Buffer Put() in LOG() Error : 버퍼가 꽉 찼습니다." );
		return FALSE;
	}

	::LeaveCriticalSection( _IN->m_pLogBufferCS );

	SetEvent( _IN->m_hLWWakeupEvent );
	return TRUE;
}

BOOL SolarLog::LOG_Arg( TCHAR * pszMessage, ... )
{
	TCHAR		szBuffer[512]="";
	va_list		pArguments;

	va_start( pArguments, pszMessage );
	int iwritten = _vsntprintf( szBuffer, 512, pszMessage, pArguments );
	va_end( pArguments );

	if( iwritten < 0 ) return FALSE;

	return LOG( szBuffer );
}

BOOL SolarLog::LOG_Date( TCHAR * pszMessage )
{
	TCHAR		szBuffer[512]="", tmpbuf[512] = "", ampm[] = "a";
	va_list		pArguments;

	struct tm * today;
	time_t		ltime;

	time( &ltime );
	today = localtime( &ltime );

	if( today->tm_hour >= 12 )
	{
		_tcsncpy( ampm, "p", 1 );
		today->tm_hour -= 12;
	}
	if( today->tm_hour == 0 )
		today->tm_hour = 12;

	//wsprintf( tmpbuf, "%.8s%s ", asctime( today ) + 11, ampm );
	_sntprintf( tmpbuf, 512, "%.8s%s ", asctime( today ) + 11, ampm );

	va_start( pArguments, pszMessage );
	int iwritten = _vsntprintf( szBuffer, 512, pszMessage, pArguments );
	va_end( pArguments );

	if( iwritten < 0 ) return FALSE;

	//strcat(tmpbuf, szBuffer);
	_tcsncat( tmpbuf, szBuffer, 512 );

	return LOG( tmpbuf );
}
////int varable = 0;
//BOOL SolarLog::_changeFile()
//{
//	DWORD dwCurTick = GetTickCount();
//	if( dwCurTick < _IN->m_dwTick + 10*1000/*10초*/ )
//		return FALSE;
//
//	_IN->m_dwTick = dwCurTick;
//
//	struct tm *	today = NULL;
//	time_t		ltime = 0;
//
//	time( &ltime );
//	today = localtime( &ltime );
//
//	today->tm_year += 1900;
//	today->tm_mon  += 1;
//
//	if( (today->tm_year <= _IN->m_nYear) && 
//		(today->tm_mon  <= _IN->m_nMonth) &&
//		(today->tm_mday <= _IN->m_nDay) )
//	{
//		//_setLastError( "_changeFile() Warning : 날짜가 잘못되어 있습니다." );
//		return FALSE;
//	}
//	_IN->m_nYear  = today->tm_year;
//	_IN->m_nMonth = today->tm_mon;
//	_IN->m_nDay   = today->tm_mday;//+(++varable);
//
//	return _createFile();
//}
//
//BOOL SolarLog::_createFile()
//{
//	TCHAR szfilepath[MAX_PATH];
//	//wsprintf( szfilepath, "%s\\%s__Date_%02d%02d%02d.wzl", m_pszDirName, m_pszFileNamePrefix, _IN->m_nYear, _IN->m_nMonth, _IN->m_nDay );
//	_sntprintf( szfilepath, MAX_PATH, "%s\\%s__Date_%02d%02d%02d.wzl", _IN->m_pszDirName, _IN->m_pszFileNamePrefix, _IN->m_nYear, _IN->m_nMonth, _IN->m_nDay );
//
//	/// 유효한 경로인가?
//	/// 디렉토리 존재 여부
//	/// 파일 존재 여부
//	if( _IN->m_hFile ) CloseHandle( _IN->m_hFile );
//	CreateDirectory( _IN->m_pszDirName, NULL );
//	_IN->m_hFile = CreateFile( szfilepath, GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL );
//	if( INVALID_HANDLE_VALUE == _IN->m_hFile )
//	{
//		_IN->_setLastError( "CreateFile() in _changeFile() Error :(%d)", GetLastError() );
//		return FALSE;
//	}
//	return TRUE;
//}
//
//VOID SolarLog::_switchBuffer() 
//{ 
//	util::RingBufferEx * tmpBuffer;
//	tmpBuffer =  _IN->m_pLogBufferPush;
//	_IN->m_pLogBufferPush = _IN->m_pLogBufferPop;
//	_IN->m_pLogBufferPop = tmpBuffer;
//}
//util::RingBufferEx * SolarLog::_getPushBuffer() 
//{
//	return _IN->m_pLogBufferPush;	
//}
//util::RingBufferEx * SolarLog::_getPopBuffer() 
//{ 
//	return _IN->m_pLogBufferPop;	
//}
VOID SolarLog::SetErrorHandler( fnCallBackError fnFunc ) 
{ 
	_IN->m_fnCallBackErrorHandler = fnFunc;	
}
