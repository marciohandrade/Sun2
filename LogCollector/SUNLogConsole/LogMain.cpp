/*------------------------------------------------------------------------------
 * LogMain.cpp
 *------------------------------------------------------------------------------
 * 프로그램에 필요한 기초 작업들을 수행하고, 
 * 로그를 읽고 쓰는 작업을 컨트롤한다.
 * 새로운 게임을 지원하게 될 경우 수정 필요..
 *------------------------------------------------------------------------------
 * All rights reserved by Jin Hye Jin (jiny7749@hotmail.com)
**----------------------------------------------------------------------------*/

#include "stdafx.h"
#include "IniControl.h"
#include "LogCriticalSection.h"
#include "LogMain.h"
#include "CrashHandler.h"
#include "SUN_LogReader.h"
#include "SUN_LogWriter.h"
#include "SUN_DBHandler.h"

#include "WriteFileLog.h"

LogCriticalSection * g_pGlobalCS;

extern DBHandler* g_pDBHandler;

/**	----------------------------------------------------------------------------
	\brief	

	\param	
	\return	
------------------------------------------------------------------------------*/
LogMain::LogMain()
:	m_nWorldID(0),
	m_nChannelID(0),
    is_restore_(FALSE)
{
	//CrashHandler::Install( true, GenerateDmpName, ProcCrashHandling );

	GetCurrentPath();

	g_pGlobalCS          = new LogCriticalSection;
	m_pConfigIni         = new IniControl(CONFIG_FILE_NAME);

	m_pDBHandler         = NULL;
	m_pLogReader         = NULL;
	m_pLogWriter         = NULL;
	m_iMaxThreadNum      = DEFAULT_WRITE_THREAD_NUM;
	m_bTimeSpan			 = FALSE;
//	m_iFileOrientation   = FILE_START_BEGIN;


}

/**	----------------------------------------------------------------------------
	\brief	

	\param	
	\return	
------------------------------------------------------------------------------*/
LogMain::~LogMain()
{
	if (NULL != m_pLogReader)
	{
		delete m_pLogReader, m_pLogReader = NULL;
	}

	if (NULL != m_pLogWriter)
	{
		delete m_pLogWriter, m_pLogWriter = NULL;
	}

	if (m_pDBHandler)
	{
		delete m_pDBHandler, m_pDBHandler = NULL;
	}

	if (m_pConfigIni)
	{
		delete m_pConfigIni, m_pConfigIni = NULL;
	}

	if (g_pGlobalCS)
	{
		delete g_pGlobalCS, g_pGlobalCS = NULL;
	}
}

/**	----------------------------------------------------------------------------
	\brief	

	\param	
	\return	
------------------------------------------------------------------------------*/
void LogMain::GetCurrentPath()
{
	TCHAR * ptr = NULL;

	GetModuleFileName(NULL, g_szCurrentPath, MAX_PATH);
	ptr = _tcsrchr(g_szCurrentPath, _T('\\'));

	if ( NULL != ptr )
	{
        *ptr = 0;
	}
	else
	{
		if (FAILED(StringCchCopy(g_szCurrentPath, sizeof(g_szCurrentPath), _T("."))))
		{
			_ASSERTE(!"StringCchCopy fail");
		}
	}
}

/**	----------------------------------------------------------------------------
	\brief	

	\param	
	\return	
------------------------------------------------------------------------------*/
int LogMain::SetDBHandler(INT nWorldNo)
{
#ifdef _KOREA
	if(nWorldNo <= 0)
	{
		HANDLE hStdOut = ::GetStdHandle(STD_OUTPUT_HANDLE);
		TCHAR szPrint[MAX_PATH];

		StringCchPrintf(szPrint, sizeof(szPrint), _T("WORLD NUMBER IS 0. PLEASE INSERT WORLD NUMBER THROUGH EXECUTE PARAMETER"));
		PrintUserColorString(hStdOut, USER_RED, szPrint);

		return -1;
	}
#endif

	if (FAILED(StringCchPrintf(szIniFile, sizeof(szIniFile),
								_T("%s\\%s"),
								g_szCurrentPath,
								CONFIG_FILE_NAME)))
	{
		_ASSERTE(!"StringCchPrintf");
		return -1;
	}

	ZeroMemory(szWorldDBName, sizeof(szWorldDBName));

	StringCchPrintf(szWorldDBName, sizeof(szWorldDBName), _T("DB"));

	m_pConfigIni->GetValue(szWorldDBName, _T("DSN")   , _T("SUN_LOG"), szDSN   , MAX_DB_PW);
	m_pConfigIni->GetValue(szWorldDBName, _T("DBNAME"), _T("")       , szDBName, MAX_DB_PW);
	m_pConfigIni->GetValue(szWorldDBName, _T("ID")    , _T("")       , szID    , MAX_DB_PW);
	m_pConfigIni->GetValue(szWorldDBName, _T("PWD")   , _T("")       , szPwd   , MAX_DB_PW);

	if(_tcslen(szDSN) < 1 || _tcslen(szDBName) < 1 || _tcslen(szID) < 1 || _tcslen(szPwd) < 1)
	{
		// 변경된 버전의 INI 형식을 가지고 있음.
		StringCchPrintf(szWorldDBName, sizeof(szWorldDBName), _T("%dWORLD-DB"), nWorldNo);

		m_pConfigIni->GetValue(szWorldDBName, _T("DSN")   , _T("SUN_LOG"), szDSN   , MAX_DB_PW);
		m_pConfigIni->GetValue(szWorldDBName, _T("DBNAME"), _T("")       , szDBName, MAX_DB_PW);
		m_pConfigIni->GetValue(szWorldDBName, _T("ID")    , _T("")       , szID    , MAX_DB_PW);
		m_pConfigIni->GetValue(szWorldDBName, _T("PWD")   , _T("")       , szPwd   , MAX_DB_PW);
	}

    return 0;
}

/**	----------------------------------------------------------------------------
	\brief	

	\param	
	\return	
------------------------------------------------------------------------------*/
int LogMain::Start(INT nWorld, TCHAR * szStartDate, TCHAR * szEndDate, TCHAR * szServerIP, TCHAR * szPrefix, TCHAR * logfilepath, TCHAR * txtfilepath, BOOL is_restore)
{
    is_restore_ = is_restore;
    if(strlen(logfilepath) > 0 && strlen(txtfilepath) > 0)
    {
        WriteFileLog log;

        log.WriteLogFile(logfilepath, txtfilepath);

        return -1;
    }
    else
    {
        if (0 != SetDBHandler(nWorld))
        {
            HANDLE hStdOut = ::GetStdHandle(STD_OUTPUT_HANDLE);
            PrintUserColorString(hStdOut, USER_RED | USER_SKYBLUE_BG, _T("SetDBHandler is Fail\n"));
            //OutputDebugString("SetDBHandler is Fail");
            return -1;
        }

        m_pDBHandler = new SUN_DBHandler(m_iMaxThreadNum, szDSN, szID, szPwd);
        if (m_pDBHandler->DBConnect() != 0)
        {
            HANDLE hStdOut = ::GetStdHandle(STD_OUTPUT_HANDLE);
            PrintUserColorString(hStdOut, USER_RED | USER_SKYBLUE_BG, _T("DBConnect Fail\n"));
            return -1;
        }

        g_pDBHandler = m_pDBHandler;

        m_pLogWriter = new SUN_LogWriter;
        m_pLogReader = new SUN_LogReader(nWorld, szStartDate, szEndDate, szServerIP, szPrefix, is_restore);

        // reader	
        m_pLogReader->StartReadLogFile();
        /*
        // writer
        m_pLogWriter = new SUN_LogWriter;
        m_pLogWriter->StartWriteLog(m_iMaxThreadNum);
        */
        /*
        if ( 1 >= _tcslen(szStartDate))
        {
        // fail log writer
        m_pLogWriter->StartWriteFailLog();
        }
        */
    }
	
	return 0;
}

VOID LogMain::LookoutProcess()
{
	TCHAR szFromat[MAX_PATH] = {0};

	while( 0 != m_pLogReader->m_iRunningThread )
	{
		StringCchPrintf(szFromat, MAX_PATH, _T("Now Read Thread Count : %d"), m_pLogReader->m_iRunningThread);
		OutputDebugString(szFromat);
		Sleep(500);
	}

	size_t nDataCnt = 0;

	g_pGlobalCS->Enter();
	nDataCnt = m_pLogReader->m_LogDataList.size();
	g_pGlobalCS->Leave();

	while (	0 != nDataCnt )
	{
		g_pGlobalCS->Enter();
		nDataCnt = m_pLogReader->m_LogDataList.size();
		g_pGlobalCS->Leave();

		Sleep(500);
	}

	HANDLE hStdOut = ::GetStdHandle(STD_OUTPUT_HANDLE);

	PrintUserColorString(hStdOut, USER_RED | USER_SKYBLUE_BG, _T("\n===================================\n"));
	PrintUserColorString(hStdOut, USER_RED | USER_SKYBLUE_BG, _T("TimeSpan Process is Finished!!\n"));
	PrintUserColorString(hStdOut, USER_RED | USER_SKYBLUE_BG, _T("Please Input 'q' Key!!\n"));
	PrintUserColorString(hStdOut, USER_RED | USER_SKYBLUE_BG, _T("\n===================================\n"));

	PrintUserColorString(hStdOut, USER_WHITE, _T(""));  
    
}

/**	----------------------------------------------------------------------------
	\brief	

	\param	
	\return	
------------------------------------------------------------------------------*/
void LogMain::Stop()
{
	HANDLE hStdOut = ::GetStdHandle(STD_OUTPUT_HANDLE);

	m_pLogReader->Stop();

	PrintUserColorString(hStdOut, USER_RED | USER_YELLOW_BG, _T("Now Stop Read Thread. Please wait process\n"));
	PrintUserColorString(hStdOut, USER_WHITE, _T(""));

	while(0 != m_pLogReader->m_iRunningThread)
	{
		Sleep(500);
	}

	PrintUserColorString(hStdOut, USER_RED | USER_YELLOW_BG, _T("Finished Read Thread process\n"));
	PrintUserColorString(hStdOut, USER_WHITE, _T(""));

	size_t nDataCnt = 0;

	PrintUserColorString(hStdOut, USER_RED | USER_SKYBLUE_BG, _T("Now DataList Clearing. Please wait process\n"));
	PrintUserColorString(hStdOut, USER_WHITE, _T(""));

	g_pGlobalCS->Enter();
	nDataCnt = m_pLogReader->m_LogDataList.size();
	g_pGlobalCS->Leave();

	while (	0 != nDataCnt )
	{
		Sleep(500);

		g_pGlobalCS->Enter();
		nDataCnt = m_pLogReader->m_LogDataList.size();
		g_pGlobalCS->Leave();
	}

	PrintUserColorString(hStdOut, USER_RED | USER_SKYBLUE_BG, _T("Finished DataList Clear process\n"));
	PrintUserColorString(hStdOut, USER_WHITE, _T(""));

	m_pLogWriter->Stop();

	PrintUserColorString(hStdOut, USER_BLUE | USER_PINK_BG, _T("Now Stop Write Thread. Please wait process\n"));
	PrintUserColorString(hStdOut, USER_WHITE, _T(""));

	while( 0 != m_pLogWriter->m_iRunningThread )
	{
		Sleep(500);
	}

	PrintUserColorString(hStdOut, USER_BLUE | USER_PINK_BG, _T("Finished Write Thread process\n"));
	PrintUserColorString(hStdOut, USER_WHITE, _T("LogCollector All Process Finish Successed\n"));

	BackUpReadIni();
}

void LogMain::BackUpReadIni()
{
	TCHAR szOriginalIni[MAX_PATH] = {0};
	TCHAR szBackUpPath[MAX_PATH] = {0};
	TCHAR szBackUpIni[MAX_PATH] = {0};

	if (FAILED(StringCchPrintf(szBackUpPath, sizeof(szBackUpPath),
								_T("%s\\BACKUP_INI"),
								g_szCurrentPath)))
	{
		_ASSERTE(!"StringCchPrintf");
		return;
	}

	if (0 == ::CreateDirectory(szBackUpPath, NULL) )
	{
		if(ERROR_ALREADY_EXISTS != GetLastError())
		{
			printf("\nBackUp ReadIni CreateDirectory FAIL!\n");
			return;
		}
	}

	if (FAILED(StringCchPrintf(szOriginalIni, sizeof(szOriginalIni),
								_T("%s\\%s"),
								g_szCurrentPath,
								READINI_FILE_NAME)))
	{
		_ASSERTE(!"StringCchPrintf");
		return;
	}

	if (FAILED(StringCchPrintf(szBackUpIni, sizeof(szBackUpIni),
								_T("%s\\%s"),
								szBackUpPath,
								READINI_FILE_NAME)))
	{
		_ASSERTE(!"StringCchPrintf");
		return;
	}

	if ( ::CopyFile(szOriginalIni, szBackUpIni, FALSE) )
	{
		printf("\nBackUp ReadIni SUCCESS.\n");
	}
	else
	{
		printf("\nBackUp ReadIni CopyFile FAIL.\n");
	}
}

void LogMain::ProcCrashHandling( LPCTSTR stackWalkLog, LPCTSTR dumpFileName )
{
	FILE*		fp = NULL;
	SYSTEMTIME	systemTime;
	TCHAR		temp[127] = {0, };

	GetLocalTime( &systemTime );
	StringCchPrintf( temp, sizeof(temp), "LOGCOLLECTOR_CrashLog_%d%02d%02d_%02d%02d%02d.txt",
		systemTime.wYear, systemTime.wMonth, systemTime.wDay,
		systemTime.wHour, systemTime.wMinute, systemTime.wSecond );

	fp = fopen( temp, "w" );
	fprintf( fp, "Dump file path : %s\r\n\r\n", dumpFileName );
	fprintf( fp, stackWalkLog );

	fclose( fp );

/*
	
		//  Report to console screen
		IfDebug()
		{
			OutputDebug( "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n" );
			OutputDebug( "This application occured an unknown error as follow...\n\n" );
			OutputDebug( "%s\n", stackWalkLog );
			OutputDebug( "This report and memory dump which is %s file\n sent to reinlainer@webzen.co.kr.\n", dumpFileName );
			OutputDebug( "\n\nPress any key to abort application...." );
	
			getch();
		}*/
	
}

LPCTSTR LogMain::GenerateDmpName()
{
	static	char		fileName[64] = {0, };
	SYSTEMTIME	time;

	::GetLocalTime( &time );
	StringCchPrintf( fileName, sizeof(fileName), "%s_%d%d%d_%d%d.dmp", _T("LOGCOLLECTOR"), time.wYear, time.wMonth, time.wDay,
		time.wHour, time.wMinute );

	return fileName;
}
