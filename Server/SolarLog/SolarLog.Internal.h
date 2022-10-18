#pragma once

#include "SolarLog.h"
#include <TIME.H>
#include <tchar.h>
#include <Define.h>

#define MEGA_BYTE (1024*1000)
#define GIGA_BYTE (MEGA_BYTE*1000)

#ifdef _NA_0_20110803_DIV_GAMELOG_FILE
    #ifdef _CHINA
        #define FILE_DIV_SIZE (MEGA_BYTE*512) // 512 MByte 
    #else
        #define FILE_DIV_SIZE (GIGA_BYTE*4) // 4 GByte 
    #endif
#else
    #define FILE_DIV_SIZE (GIGA_BYTE*1000)
#endif //_NA_0_20110803_DIV_GAMELOG_FILE

class SolarLog::Internal
{
public:
    uint max_file_size_;
	Internal()
		:	m_nYear ( 0 )
		,	m_nMonth ( 0 )
		,	m_nDay ( 0 )
#ifdef _NA_007039_20140121_TIME_UNIT_TEXTLOG
		,	m_nHour( 0 )
#endif //_NA_007039_20140121_TIME_UNIT_TEXTLOG
		,	m_hFile ( INVALID_HANDLE_VALUE )
		,	m_hLWWakeupEvent ( NULL )
		,	m_pLogBufferCS ( NULL )
		,	m_pWorkerThread ( NULL )
		,	m_pLogBufferPush ( NULL )
		,	m_pLogBufferPop ( NULL )
		,	m_fnCallBackErrorHandler ( NULL )
        ,  max_file_size_(FILE_DIV_SIZE)
	{

	}

	VOID _switchBuffer() 
	{ 
		util::RingBufferEx * tmpBuffer;
		tmpBuffer =  m_pLogBufferPush;
		m_pLogBufferPush = m_pLogBufferPop;
		m_pLogBufferPop = tmpBuffer;
	}
	util::RingBufferEx * _getPushBuffer() 
	{
		return m_pLogBufferPush;	
	}
	util::RingBufferEx * _getPopBuffer() 
	{ 
		return m_pLogBufferPop;	
	}

	BOOL _changeFile()
	{
		DWORD dwCurTick = GetTickCount();
		if( dwCurTick < m_dwTick + 10*1000/*10초*/ )
			return FALSE;

		m_dwTick = dwCurTick;

		struct tm *	today = NULL;
		time_t		ltime = 0;

		time( &ltime );
		today = localtime( &ltime );

		today->tm_year += 1900;
		today->tm_mon  += 1;

#ifdef _NA_007039_20140121_TIME_UNIT_TEXTLOG
		if( (today->tm_year <= m_nYear) && 
			(today->tm_mon  <= m_nMonth) &&
			(today->tm_mday <= m_nDay) &&
			(today->tm_hour <= m_nHour) )
#else
		if( (today->tm_year <= m_nYear) && 
			(today->tm_mon  <= m_nMonth) &&
			(today->tm_mday <= m_nDay) )
#endif //_NA_007039_20140121_TIME_UNIT_TEXTLOG
		{
        #ifdef _NA_0_20110803_DIV_GAMELOG_FILE
            if( m_hFile !=  INVALID_HANDLE_VALUE)
            {
                LARGE_INTEGER file_size = {0,};
                GetFileSizeEx(m_hFile, &file_size);
                if (file_size.QuadPart > max_file_size_)
                {
                    return _createFile();
                }
            }
        #endif
			return FALSE;
		}
		m_nYear  = today->tm_year;
		m_nMonth = today->tm_mon;
		m_nDay   = today->tm_mday;//+(++varable);
#ifdef _NA_007039_20140121_TIME_UNIT_TEXTLOG
		m_nHour = today->tm_hour;
#endif //_NA_007039_20140121_TIME_UNIT_TEXTLOG

		return _createFile();
	}

#ifdef _NA_0_20110803_DIV_GAMELOG_FILE
    BOOL _createFile()
    {
#ifdef _NA_007039_20140121_TIME_UNIT_TEXTLOG
        //만드려는 로그가 텍스트로그인지 바이너리로그인지 확인한다.
        BOOL is_text_log=false;
        TCHAR textlog_file_dir[MAX_PATH];
#endif //_NA_007039_20140121_TIME_UNIT_TEXTLOG
        TCHAR szfilepath[MAX_PATH];

        /// 유효한 경로인가?
        /// 디렉토리 존재 여부
        /// 파일 존재 여부
        if (m_hFile != INVALID_HANDLE_VALUE)
            CloseHandle( m_hFile );

#ifdef _NA_007039_20140121_TIME_UNIT_TEXTLOG
        if (_tcsncmp(m_pszFileNamePrefix, "SUN_", 4) != 0) 
        {
            is_text_log = true;
            _sntprintf(textlog_file_dir, MAX_PATH, "%s\\%02d%02d%02d", m_pszDirName, m_nYear, m_nMonth, m_nDay);
            CreateDirectory( textlog_file_dir, NULL );
        }
        else {
            CreateDirectory( m_pszDirName, NULL );
        }
#else
        CreateDirectory( m_pszDirName, NULL );
#endif //_NA_007039_20140121_TIME_UNIT_TEXTLOG

        // for 루프를 돌면서 생성(오픈)할 파일의 이름을 찾는다.
        int tail_index = 0;
        for ( ; ; )
        {
#ifdef _NA_007039_20140121_TIME_UNIT_TEXTLOG
            //TextLog는 뒤에 시간을 붙여 시간단위로 끊지만 BinaryLog는 이전처럼 시간단위로 끊지 않는다.
            if (is_text_log)
            {
                if (tail_index == 0)
                    _sntprintf( szfilepath, MAX_PATH, "%s\\%s__Date_%02d%02d%02d%02d.wzl", 
                    textlog_file_dir, m_pszFileNamePrefix, m_nYear, m_nMonth, m_nDay, m_nHour );
                else
                    _sntprintf( szfilepath, MAX_PATH, "%s\\%s__Date_%02d%02d%02d%02d_%d.wzl", 
                    textlog_file_dir, m_pszFileNamePrefix, m_nYear, m_nMonth, m_nDay, m_nHour, tail_index );
            }
            else
            {
                if (tail_index == 0)
                    _sntprintf( szfilepath, MAX_PATH, "%s\\%s__Date_%02d%02d%02d.wzl", 
                    m_pszDirName, m_pszFileNamePrefix, m_nYear, m_nMonth, m_nDay );
                else
                    _sntprintf( szfilepath, MAX_PATH, "%s\\%s__Date_%02d%02d%02d_%d.wzl", 
                    m_pszDirName, m_pszFileNamePrefix, m_nYear, m_nMonth, m_nDay, tail_index );
            }
			
#else
			if (tail_index == 0)
				_sntprintf( szfilepath, MAX_PATH, "%s\\%s__Date_%02d%02d%02d.wzl", m_pszDirName, m_pszFileNamePrefix, m_nYear, m_nMonth, m_nDay );
			else
				_sntprintf( szfilepath, MAX_PATH, "%s\\%s__Date_%02d%02d%02d_%d.wzl", m_pszDirName, m_pszFileNamePrefix, m_nYear, m_nMonth, m_nDay, tail_index );
#endif //_NA_007039_20140121_TIME_UNIT_TEXTLOG

            HANDLE temp_handle = INVALID_HANDLE_VALUE;
            temp_handle = CreateFile( szfilepath, GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL );
            
            // 해당 경로의 파일이 존재하지 않음으로 새로 생성된다.
            if (temp_handle == INVALID_HANDLE_VALUE)
                break;

            LARGE_INTEGER file_size = {0,};
            GetFileSizeEx(temp_handle, &file_size);
            if (file_size.QuadPart > max_file_size_)
            {
                ++tail_index;
                CloseHandle(temp_handle);
            }
            else
            {
                // 파일을 오픈 후 최대 사이즈(256MByte) 보다 작은 경우
                // 그 파일안에 연속해서 로그를 작성한다.
                CloseHandle(temp_handle);
                break;
            }
        }

        m_hFile = CreateFile( szfilepath, GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL );
        if( INVALID_HANDLE_VALUE == m_hFile )
        {
            _setLastError( "CreateFile() in _changeFile() Error :(%d)", GetLastError() );
            return FALSE;
        }
        return TRUE;
    }
#else
	BOOL _createFile()
	{
		TCHAR szfilepath[MAX_PATH];
		_sntprintf( szfilepath, MAX_PATH, "%s\\%s__Date_%02d%02d%02d.wzl", m_pszDirName, m_pszFileNamePrefix, m_nYear, m_nMonth, m_nDay );

		/// 유효한 경로인가?
		/// 디렉토리 존재 여부
		/// 파일 존재 여부
		if( m_hFile ) CloseHandle( m_hFile );
		CreateDirectory( m_pszDirName, NULL );
		m_hFile = CreateFile( szfilepath, GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL );
		if( INVALID_HANDLE_VALUE == m_hFile )
		{
			_setLastError( "CreateFile() in _changeFile() Error :(%d)", GetLastError() );
			return FALSE;
		}
		return TRUE;
	}
#endif

	VOID _setLastError( TCHAR * pszMsg, ... )
	{
		va_list		pArguments;
		va_start( pArguments, pszMsg );
		int iwritten = _vsntprintf( m_szTempBuffer, MaxTempBufferSize, pszMsg, pArguments );
		va_end( pArguments );

		if( iwritten > 0 && m_fnCallBackErrorHandler )
			m_fnCallBackErrorHandler( m_szTempBuffer );

	}

public:
	static const int					MaxTempBufferSize = 512;
	TCHAR								m_szTempBuffer[MaxTempBufferSize];
	int									m_nYear;
	int									m_nMonth;
	int									m_nDay;
#ifdef _NA_007039_20140121_TIME_UNIT_TEXTLOG
	int									m_nHour;
#endif //_NA_007039_20140121_TIME_UNIT_TEXTLOG
	DWORD								m_dwTick;
	char								m_pszDirName[MAX_PATH];
	char								m_pszFileNamePrefix[MAX_PATH];

	HANDLE								m_hFile;
	HANDLE								m_hLWWakeupEvent;
	CRITICAL_SECTION *					m_pLogBufferCS;
	util::IThreadInterface *					m_pWorkerThread;
	util::RingBufferEx *				m_pLogBufferPush;
	util::RingBufferEx *				m_pLogBufferPop;
	fnCallBackError						m_fnCallBackErrorHandler;
};
