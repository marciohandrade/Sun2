#include "StdAfx.h"
#include "SunLog.h"
#include "SolarAuth.Common.h"
#include <SolarLog.h>
#include <io.h>

DWORD					CSunLog::g_dwSUNLOG_INIT = 0;
CSunLog::CSunLogInfo*	CSunLog::g_pSUNLOG = NULL;
static CSunLog::CSunLogInfo		g_sSUNLOG;

CSunLog::CSunLogInfo::CSunLogInfo()
{
	m_hConsole					= 0;
	//m_Handler.fn_FILE_LOGGER	= 0;
	//m_Handler.fn_CONSOLE_LOGGER	= 0;
	m_fnCONSOLE_LOGGER			= NULL;
	//
	LOGGING_INFO.Option			= CSunLog::cOUTPUT_NONE;
	LOGGING_INFO.Level			= CSunLog::cFULL_LOG;
	LOGGING_INFO.Mask			= CSunLog::cCATE_MASK;

	memset( &CONSOLE_LOG, 0, sizeof(CONSOLE_LOG) );

	InitializeCriticalSection( &m_csConsoleLog );
	m_dwConsoleTimeout = 0;

	ASSERT( g_pSUNLOG == NULL );
	g_pSUNLOG = this;
}

CSunLog::CSunLogInfo::~CSunLogInfo()
{
	EnterCriticalSection( &m_csConsoleLog );
	ASSERT( g_pSUNLOG != NULL );

	if( CONSOLE_LOG.QueuedStream )
	{
		SAFE_DELETE( CONSOLE_LOG.QueuedStream );
	}

	LeaveCriticalSection( &m_csConsoleLog );
}

CSunLog::CSunLog( VOID )
{
//	static CSunLog::CSunLogInfo		g_sSUNLOG;
	//
	m_pMainLogger		= NULL;
	memset( &FILE_LOG, 0, sizeof(FILE_LOG) );
	//InitializeCriticalSection( &m_csFileLog );
	m_dwFileTimeout		= 0;
	m_bFileOutDelay = FALSE;
	//
#ifdef __SUNLOG_UNUSED_SECT
	m_strFilePathLen	= 0;
	m_strFilePath		= NULL;
	memset( m_strFileTime, 0, sizeof(m_strFileTime) );
	memset( m_strFileName, 0, sizeof(m_strFileName) );
#endif


}

CSunLog::~CSunLog( VOID )
{
	if( g_dwSUNLOG_INIT )
		Flush();
	g_dwSUNLOG_INIT--;

	//EnterCriticalSection( &m_csFileLog );
	if( FILE_LOG.QueuedStream )
	{
		SAFE_DELETE( m_pMainLogger );
		SAFE_DELETE( FILE_LOG.QueuedStream );
	}

	//LeaveCriticalSection( &m_csFileLog );
}

// Reload
VOID	CSunLog::SetLogLevel( BYTE lv )
{
	eLOGLEVEL eLogLEVEL = (eLOGLEVEL)lv;
	if( CSunLog::g_pSUNLOG != NULL )
	{
		if( (cOUTPUT_MASK&eLogLEVEL) == eLogLEVEL )
		{
			CSunLogInfo::sLOGGING_INFO* pINFO = &(CSunLog::g_pSUNLOG->LOGGING_INFO);
			pINFO->Level = eLogLEVEL;
		}
	}
}

// <METHODs>
BOOL	CSunLog::Init(
					  const BYTE byLogOption,
					  const BYTE byLogLevel,
					  const TCHAR* pszFilePath,
					  const TCHAR* pszFileName,
					  BOOL bUseSolarLog
					  )
{
	eLOGOPTION eLogOption = (eLOGOPTION)byLogOption;
	eLOGLEVEL eLogLevel = (eLOGLEVEL)byLogLevel;

	if( (cOUTPUT_MASK&eLogOption) != eLogOption
		|| (cLEVEL_MASK&eLogLevel) != eLogLevel )
	{
		_putts( _T("CSunLog Initialization Failed! Check Log Option -or- Log Level") );
		return FALSE;
	}

	if( !pszFilePath || !pszFileName )
	{
		_putts( _T("CSunLog Initialization Failed! Check Log Folder -or- Log FileName") );
		return FALSE;
	}

	// � ������ �˻��ؾ� �ϴ��� Ȯ���� �� ��.
	if( _access( pszFilePath, 0) == -1 )	//������ �������� ������..
	{
		if( !CreateDirectory( pszFilePath, NULL ) )
			printf( "CreateDirectory Fail, path = %s \n", pszFilePath  );
	}

	if( bUseSolarLog )
	{
		m_pMainLogger = new SolarLog();
		// <NOTE> _T�� �̷��� ����ص� �Ǵ� ���ΰ�?

		// SolarLog���� �˾Ƽ� Ÿ���� �˻����ش�.
		//TCHAR* buffer = GetCurrentFileNameWithTime( pszFileName );
		if( !m_pMainLogger->Create( (TCHAR*)pszFilePath, (TCHAR*)pszFileName ) )
		{
			ASSERT( !"Can't Create LogFile" );
			SAFE_DELETE( m_pMainLogger );
			return FALSE;
		}
	}
	else
	{
		m_pMainLogger = NULL;
	}

	m_fnFILE_LOGGER = NULL;
	FILE_LOG.Written = 0;
	//FILE_LOG.QueuedStream = (TCHAR*)malloc( c_FileLogDataSize );
	FILE_LOG.QueuedStream = new TCHAR[c_FileLogDataSize];

	memset( FILE_LOG.QueuedStream, 0, c_FileLogDataSize*sizeof(TCHAR) );

	if( !CSunLog::g_dwSUNLOG_INIT )
	{
		g_pSUNLOG->m_hConsole = (HANDLE)GetStdHandle(STD_OUTPUT_HANDLE);
		g_pSUNLOG->LOGGING_INFO.Option = eLogOption;
		g_pSUNLOG->LOGGING_INFO.Level = eLogLevel;
		// <NOTE>
		//   ���� �Է��� �޴� ���·� ��ȯ�� ��. ����ü ���� ��������
		g_pSUNLOG->LOGGING_INFO.Mask = cCATE_MASK;

		//CheckHandler( g_pSUNLOG->m_Handler );
		g_pSUNLOG->CONSOLE_LOG.Written = 0;
		//g_pSUNLOG->CONSOLE_LOG.QueuedStream = (TCHAR*)malloc( c_FileLogDataSize );
		g_pSUNLOG->CONSOLE_LOG.QueuedStream = new TCHAR[c_ConsoleLogDataSize];
		memset( g_pSUNLOG->CONSOLE_LOG.QueuedStream, 0, c_ConsoleLogDataSize*sizeof(TCHAR) );
	}
	CheckHandler( g_pSUNLOG->m_fnCONSOLE_LOGGER, m_fnFILE_LOGGER );
	g_dwSUNLOG_INIT++;
	m_bFileOutDelay = TRUE;

	return TRUE;
}

#ifdef __SUNLOG_UNUSED_SECT
TCHAR*	CSunLog::GetCurrentFileNameWithTime( const TCHAR* defFileName )
{
	// �� �Լ��� ���������� ��� ����� �Լ��̴�.
	// ��κ��� üũ�� NULL �˻�� �������� �ʴ´�.
	const SIZE_T buflen = sizeof(m_strFileNameWithTime);

	ASSERT( !(_tcslen( defFileName ) < sizeof(m_strFileName)) );	// 100����

	TCHAR* buffer = m_strFileNameWithTime;

	INT iWritten = _sntprintf( buffer, buflen, "%s", defFileName );

	if( iWritten < 0 )
		return NULL;

	// no bound check
	buffer[iWritten++] = _T('-');

	// CLogTimeFormat�� �������̽����� ��Ÿ����. �� TRUE, FALSE�ۿ� ���� ���ΰ�?
	CLogTimeFormat::GetTime( 2, &buffer[iWritten], MAX_TIME_SIZE );
	// ���� �߰��� ������ ���� ��� �������.
	//iWritten += (INT)_tcslen( &buffer[iWritten] );	// �����ϴ� ��~

	return buffer;
}
#endif

BOOL	CSunLog::CheckHandler( WriteMessage& pfnColsoleLogHandler, WriteMessage& pfnFileLogHandler )
{
	const CSunLogInfo::sLOGGING_INFO* pINFO = (const CSunLogInfo::sLOGGING_INFO*)&CSunLog::g_pSUNLOG->LOGGING_INFO;

	switch (int option = (pINFO->Option))
	{
	case CSunLog::cOUTPUT_FILE:
		pfnColsoleLogHandler = NULL;
		pfnFileLogHandler = CSunLog::WriteMessage_File; // file writer�� �Ҵ��Ѵ�.
		return TRUE;
	case CSunLog::cOUTPUT_CONSOLE:
		pfnColsoleLogHandler = CSunLog::WriteMessage_Console; // console writer�� �Ҵ��Ѵ�.
		pfnFileLogHandler = NULL;
		return TRUE;
	case CSunLog::cOUTPUT_BOTH:
		pfnColsoleLogHandler = CSunLog::WriteMessage_Console; // console writer�� �Ҵ��Ѵ�.
		pfnFileLogHandler = CSunLog::WriteMessage_File; // file writer�� �Ҵ��Ѵ�.
		return TRUE;
	}
	return FALSE;
}

// <DESCRIPTION>
//   �� �����ӽÿ� ���� �ð� ���� �˻縦 �����Ѵ�.
//   1) ���� Ÿ�Ӿƿ� üũ
//   2) ��¥ ���濡 ���� ����, �����ִ� ���� ���� ���� ó���� �����Ѵ�. 
//   2-1) �̰��, ������ ����Ǵ� ����� ������ �ذ��ϱ� ���� ������ ���� �̸� Flush�� �����ϵ��� �Ѵ�.
VOID	CSunLog::Update()
{
	CSunLogInfo* pLOGINFO = g_pSUNLOG;
	// (1) Ÿ�Ӿƿ��� ���� Flush ó��
	DWORD eLOGOPT = (eLOGOPTION)0;

	DWORD dwNow = GetTickCount();
	if( pLOGINFO->m_dwConsoleTimeout < dwNow )
	{
		pLOGINFO->m_dwConsoleTimeout = dwNow + c_ConsoleLogInterval;
		eLOGOPT |= cOUTPUT_CONSOLE;
	}

	// ���� ��� ����
	// File�� Console�� �����ؼ� Write����
	if( (eLOGOPTION)0 != eLOGOPT )
		Flush( (eLOGOPTION)eLOGOPT );
}

VOID	CSunLog::Flush( eLOGOPTION eLogOPT )
{
	const CSunLogInfo* pLOGINFO = (const CSunLogInfo*)g_pSUNLOG;

	// CONSOLE ó��
	if( eLogOPT & cOUTPUT_CONSOLE )
	{
		EnterCriticalSection( (LPCRITICAL_SECTION)&pLOGINFO->m_csConsoleLog );

		CSunLogInfo::sCONSOLE_LOG& wLogOff = (CSunLogInfo::sCONSOLE_LOG&)pLOGINFO->CONSOLE_LOG;
		if( wLogOff.Written )
		{
			WriteMessage_IO( pLOGINFO->m_hConsole, (BYTE*)wLogOff.QueuedStream, wLogOff.Written );
			wLogOff.Written = 0;
		}

		LeaveCriticalSection( (LPCRITICAL_SECTION)&pLOGINFO->m_csConsoleLog );
	}
}


// 1) �⺻ ������� �־��� ���ڰ������� �˻�
//   T) ������ ������ ������� ó����ƾ ����
//   F) return FALSE
// 2) 
BOOL	CSunLog::LOGGINGwithARG(
						 CSunLog* pSunLog,
						 const eLOGOPTION logOpt,
						 const eLOGLEVEL logLEVEL,
						 const eLOGCATEGORY category,
						 const TCHAR* pszMsg, va_list vlist )
{
	if( pSunLog )
	{
		const CSunLogInfo::sLOGGING_INFO* pINFO = (const CSunLogInfo::sLOGGING_INFO*)&(CSunLog::g_pSUNLOG->LOGGING_INFO);

		if( !pSunLog->CheckControl( pINFO, logLEVEL, logOpt, category ) )
			return FALSE;

		if( CSunLog::cOUTPUT_FILE&logOpt )
		{
			if( pSunLog->m_fnFILE_LOGGER )
			{
				//EnterCriticalSection( &pSunLog->m_csFileLog );
				// need Make format
				CSunLog::sFILE_LOG& wLogOff = (CSunLog::sFILE_LOG&)pSunLog->FILE_LOG;

				INT iWritten = MakePrefixFormat(
										(ePREFIX)(CSunLog::PREFIX_WRITE_TIME|CSunLog::PREFIX_CATEGORY),
										wLogOff.QueuedStream + wLogOff.Written,
										c_FileLogDataSize-wLogOff.Written, category );
				wLogOff.Written += iWritten;
				pSunLog->m_fnFILE_LOGGER( pSunLog, pszMsg, vlist );
				//LeaveCriticalSection( &pSunLog->m_csFileLog );
			}
		}

		if( CSunLog::cOUTPUT_CONSOLE&logOpt)
		{
			WriteMessage wmLogger = CSunLog::g_pSUNLOG->m_fnCONSOLE_LOGGER;
			if( wmLogger )
			{
				EnterCriticalSection( &CSunLog::g_pSUNLOG->m_csConsoleLog );
				wmLogger( pSunLog, pszMsg, vlist );
				LeaveCriticalSection( &CSunLog::g_pSUNLOG->m_csConsoleLog );
			}
		}

		return TRUE;
	}
	return FALSE;
}

VOID		CSunLog::RegisterHandler( WriteMessage fnFileLogger )
{
	// NULL�̸�, �ش� �α��� ���Ѵ�?
	m_fnFILE_LOGGER = fnFileLogger;
}

BOOL		CSunLog::WriteMessage_IO( HANDLE hWriter, const BYTE* stream, const DWORD dwLength )
{
	DWORD dwWrittenInHandle; // �� ������ ���� �ٽ� ������ ������ ��, �ƴϸ� ������ ��?
	if( WriteFile( hWriter, (LPCVOID)stream, dwLength, (LPDWORD)&dwWrittenInHandle, NULL ) )
	{
		return TRUE;
	}

	else
	{
		TCHAR pMSG[256];
		_sntprintf( pMSG, sizeof(pMSG), _T("FAILED LOGGING IO\tHandle [%u]:PTR(%p)LENGTH(%d)\r\n"), hWriter, stream, dwLength );
		perror( pMSG );
		return FALSE;
	}
}

BOOL	CSunLog::WriteMessage_Console( const CSunLog* pSunLog, const TCHAR* pMsg, va_list vlist )
{
    __UNUSED(pSunLog);// used g_pSUNLOG
	// timeout�����ؼ��� no lock�� �����Ѵ�. �ݵ�� �ش� �ð����� ����ߴٰ� �� �ʿ�� ����.
	CSunLogInfo* pLOGINFO = (CSunLogInfo*)g_pSUNLOG;

	CSunLogInfo::sCONSOLE_LOG& wLogOff = pLOGINFO->CONSOLE_LOG;

	int iWritten = _vsntprintf(
						wLogOff.QueuedStream+wLogOff.Written,
						CSunLog::c_ConsoleLogDataSize-wLogOff.Written,
						pMsg, vlist );

	if( iWritten < 0 )
	{
		return FALSE;
	}

	wLogOff.Written += (DWORD)iWritten;
	if( wLogOff.QueuedStream[wLogOff.Written-1] != _T('\n') )
		wLogOff.QueuedStream[wLogOff.Written++] = _T('\n');

	if( c_ConsoleLogDataSizeLimit < wLogOff.Written )
	{
		WriteMessage_IO( pLOGINFO->m_hConsole, (BYTE*)wLogOff.QueuedStream, wLogOff.Written );
		wLogOff.Written = 0;
	}

	return TRUE;
}

BOOL	CSunLog::WriteMessage_File( const CSunLog* pSunLog, const TCHAR* pMsg, va_list vlist )
{
	CSunLog::sFILE_LOG& wLogOff = (CSunLog::sFILE_LOG&)pSunLog->FILE_LOG;

	INT iWritten = _vsntprintf(
						wLogOff.QueuedStream+wLogOff.Written,
						CSunLog::c_FileLogDataSize-wLogOff.Written,
						pMsg, vlist );

	if( iWritten < 0 )
	{
		return FALSE;
	}

	wLogOff.Written += (DWORD)iWritten;

	// '\r' '\n'�� ������ ���� ����� �⺻���� �����Ѵ�.
	// ��, '\r' ������ �ٸ� ���� ���� �� ������ ������ ����.
	// �̿� ���� �� �ִ� ����� ���� �����Ѵ�.
	if( wLogOff.Written > 1 )	//2
	{
		if( _T('\n') == wLogOff.QueuedStream[wLogOff.Written-1] )
			wLogOff.Written --;
		else if( _T('\r') == wLogOff.QueuedStream[wLogOff.Written-2] )
			wLogOff.Written -= 2;
	}

	wLogOff.QueuedStream[wLogOff.Written++] = _T('\r');
	wLogOff.QueuedStream[wLogOff.Written++] = _T('\n');

	// Write2File Direct without Double-Buffering
	{
		if( pSunLog && pSunLog->m_pMainLogger )
			pSunLog->m_pMainLogger->LOG( (BYTE*)wLogOff.QueuedStream, (DWORD)(wLogOff.Written*sizeof(TCHAR)) );
		wLogOff.Written = 0;
	}

	return TRUE;
}


BOOL	CSunLog::WriteMessage_Console2( const CSunLog* pSunLog, const TCHAR* pMsg, va_list vlist )
{
    __UNUSED((pSunLog, pMsg, vlist));
	return FALSE;
}

BOOL	CSunLog::WriteMessage_Console3( const CSunLog* pSunLog, const TCHAR* pMsg, va_list vlist )
{
    __UNUSED((pSunLog, pMsg, vlist));
	return FALSE;
}

//
//
//
//
//

typedef CHAR_WRAPPER	CATEGORY_STRING;

static inline DWORD __FFS( DWORD dwVal )
{
	DWORD dwIdx;

	dwIdx = 31;
	if (dwVal & 0x0000FFFF) { dwIdx -= 16; dwVal <<= 16; }
	if (dwVal & 0x00FF0000) { dwIdx -= 8;  dwVal <<= 8;  }
	if (dwVal & 0x0F000000) { dwIdx -= 4;  dwVal <<= 4;  }
	if (dwVal & 0x30000000) { dwIdx -= 2;  dwVal <<= 2;  }
	if (dwVal & 0x40000000) { dwIdx -= 1; }

	return dwIdx;
}

static inline DWORD __ArrayIndexByOffset( PVOID pBASE, PVOID pOffset )
{
	DWORD dwBase = (DWORD)(__int64)pBASE;
	DWORD dwOffset = (DWORD)(__int64)pOffset;

	DWORD dwDiff = dwOffset-dwBase;
	dwDiff>>=2;

	return dwDiff;
}

//
//
//

static const CATEGORY_STRING sc_CategoryNameTable[32] =
{
	CATEGORY_STRING(_T("(null)")),		//[00]
	CATEGORY_STRING(_T("CONNECTION")),	//[01] 0x00000001
	CATEGORY_STRING(_T("WAREHOUSE")),	//[02] 0x00000002
	CATEGORY_STRING(_T("TRADE")),		//[03] 
	CATEGORY_STRING(_T("ITEM")),		//[04] 
	CATEGORY_STRING(_T("MISSION")),		//[05] 
	CATEGORY_STRING(_T("ABUSING")),		//[06] 1<< 6
	CATEGORY_STRING(_T("(null)")),		//[07]
	CATEGORY_STRING(_T("(null)")),		//[08]
	CATEGORY_STRING(_T("(null)")),		//[09]
	CATEGORY_STRING(_T("(null)")),		//[10]
	CATEGORY_STRING(_T("(null)")),		//[11]
	CATEGORY_STRING(_T("(null)")),		//[12]
	CATEGORY_STRING(_T("(null)")),		//[13]
	CATEGORY_STRING(_T("(null)")),		//[14]
	CATEGORY_STRING(_T("(null)")),		//[15]
	CATEGORY_STRING(_T("(null)")),		//[16]
	CATEGORY_STRING(_T("(null)")),		//[17]
	CATEGORY_STRING(_T("(null)")),		//[18]
	CATEGORY_STRING(_T("(null)")),		//[19]
	CATEGORY_STRING(_T("(null)")),		//[20]
	CATEGORY_STRING(_T("(null)")),		//[21]
	CATEGORY_STRING(_T("(null)")),		//[22]
	CATEGORY_STRING(_T("(null)")),		//[23]
	CATEGORY_STRING(_T("(null)")),		//[24]
	CATEGORY_STRING(_T("(null)")),		//[25]
	CATEGORY_STRING(_T("(null)")),		//[26]
	CATEGORY_STRING(_T("(null)")),		//[27]
	CATEGORY_STRING(_T("(null)")),		//[28]
	CATEGORY_STRING(_T("(null)")),		//[29]
	CATEGORY_STRING(_T("DATARELOAD")),	//[30]
	CATEGORY_STRING(_T("(null)")),		//[31]
};

CATEGORY_STRING GetCategoryNameByCode( CSunLog::eLOGCATEGORY dwCode )
{
	if( !dwCode )
		return NULL;

	DWORD dwIdx = __FFS( (DWORD)dwCode );

	if( !(dwIdx < 32) )
		return NULL;

	return sc_CategoryNameTable[dwIdx];//.m_pSTRING;
}


//  <LOG CATEGORY INTERFACE>
// <WARNING>
//   ���� �޼ҵ�� NULLüũ�� ���� �ʴ´�.
INT		CSunLog::MakePrefixFormat( const ePREFIX ePrefix, TCHAR* buffer, const DWORD dwMaxWritableLen, const eLOGCATEGORY cate )
{
	INT iWritten = 0, iOffset;
	DWORD dwRemainWritableLen = dwMaxWritableLen;
	if( PREFIX_WRITE_TIME & ePrefix )
	{
		if( MAX_TIME_SIZE > dwRemainWritableLen )
			return iWritten;

		// format [20060614_16:17:10]
		SYSTEMTIME sTIMESPEC;
		util::TimeSync::GetLocalTime( &sTIMESPEC );
		iOffset = _sntprintf( buffer+iWritten, dwRemainWritableLen, _T("[%04d%02d%02d_%02d:%02d:%02d]"),
			sTIMESPEC.wYear, sTIMESPEC.wMonth, sTIMESPEC.wDay,
			sTIMESPEC.wHour, sTIMESPEC.wMinute, sTIMESPEC.wSecond
			);

		if( iOffset < 0 )
			return iWritten;

		dwRemainWritableLen -= iOffset;
		iWritten += iOffset;
	}

	buffer[iWritten++] = _T('\t');

	if( PREFIX_CATEGORY & ePrefix )
	{
		TCHAR* strPrefix = NULL;
		if( 32 > dwRemainWritableLen )
			return 0;

		strPrefix = GetCategoryNameByCode(cate);

		iOffset = _sntprintf( buffer+iWritten, dwRemainWritableLen, _T("[%s]"), strPrefix );

		if( iOffset < 0 )
			return iWritten;

		dwRemainWritableLen -= iOffset;
		iWritten += iOffset;
	}

	buffer[iWritten++] = _T('\t');

	return iWritten;
}



