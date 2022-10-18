/*------------------------------------------------------------------------------
 * LogReader.cpp
 *------------------------------------------------------------------------------
 * 로그를 읽어오는데 필요한 기능을 수행하는 클래스이다.
 * 새로운 게임의 로그를 읽어와야 하는 경우 
 * 이 클래스를 상속받아 해당 게임의 전용 리더를 생성하면 된다.
 *------------------------------------------------------------------------------
 * All rights reserved by Jin Hye Jin (jiny7749@hotmail.com)
**----------------------------------------------------------------------------*/

#include "stdafx.h"
#include "DBHandler.h"
#include "IniControl.h"
#include "LogCollectorLog.h"
#include "LogMain.h"
#include "LogReader.h"
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <io.h>
#include <stdio.h>

extern LogMain g_SUNLogMain;

/**	----------------------------------------------------------------------------
	\brief	

	\param	
	\return	
------------------------------------------------------------------------------*/
LogReader::LogReader()
:	m_bReadThreadExitFlag(FALSE),
	m_iRunningThread(0),
	m_nNextDayCount(0),
	m_nTotalReadCount(0),
	m_bFirstPrint(TRUE),
	m_bLocalLog(FALSE)
{
	m_pConfigIni = g_SUNLogMain.GetConfigIni();
	m_pReadIni   = new IniControl(READINI_FILE_NAME);
	ZeroMemory(m_szLocalLogPath, sizeof(m_szLocalLogPath));

	memset(m_szLogFileName, 0 , MAX_PATH);
	memset(m_szStartDate  , 0, sizeof(m_szStartDate));
	memset(m_szEndDate    , 0, sizeof(m_szEndDate));
	memset(m_szServerIP   , 0, sizeof(m_szServerIP));
	memset(m_szPrefix	  , 0, sizeof(m_szPrefix));
	m_nWorld = 0;


#ifdef _RENAME
	m_pConfigIni->GetValue(_T("UPDATE"), _T("RENAME_FILE_FORMAT"), _T("%s_%s.wzl"), 
		m_szLogFileName, sizeof(m_szLogFileName));
#else
	m_pConfigIni->GetValue(_T("UPDATE"), _T("FILE_FORMAT"), _T("%s_%s.wzl"), 
		m_szLogFileName, sizeof(m_szLogFileName));
#endif //_RENAME

	InitializeCriticalSection(&m_ReadListCS);

	//MakeSemaphore(12);
}

/**	----------------------------------------------------------------------------
	\brief	

	\param	
	\return	
------------------------------------------------------------------------------*/
LogReader::~LogReader()
{
	ClearServerList();	// m_ServerList 삭제
	ClearLogDataList();

	if (m_pReadIni)
	{
		delete m_pReadIni, m_pReadIni = NULL;
	}

	DeleteSemaphore();
	m_hThreadSemaphore = NULL;

	DeleteCriticalSection(&m_ReadListCS);
}

void LogReader::MakeSemaphore(int iMax)
{
	TCHAR szCurrentPath[MAX_PATH] = _T("PrecessSE");
	TCHAR szName[MAX_PATH] = {0};

	srand((UINT)time(NULL));
	int nIdx = rand() % 10000;

	StringCchPrintf(szName, sizeof(szName), _T("%s%d"), szCurrentPath, nIdx);

	m_hThreadSemaphore = CreateSemaphore( 
		NULL,	// No security attrib.
		iMax,	// initial
		iMax,	// max
		szName); //MULTIREAD_RESTORE_LOG_VER1

	while(ERROR_ALREADY_EXISTS == ::GetLastError())
	{
		srand((UINT)time(NULL));
		nIdx = rand() % 20000;

		StringCchPrintf(szName, sizeof(szName), _T("%s%d"), szCurrentPath, nIdx);

		m_hThreadSemaphore = CreateSemaphore( 
			NULL,	// No security attrib.
			iMax,	// initial
			iMax,	// max
			szName); //MULTIREAD_RESTORE_LOG_VER1
	}

	if(m_hThreadSemaphore == NULL)
	{
		HANDLE hStdOut = ::GetStdHandle(STD_OUTPUT_HANDLE);
		PrintUserColorString(hStdOut, USER_RED | USER_SKYBLUE_BG, _T("Create Semaphore Fail.\n"));
	}

	TCHAR szMSG[MAX_PATH] = {0,};

	StringCchPrintf(szMSG, sizeof(szMSG), _T("CreateSemaphore Count : %d"), iMax);
	OutputDebugString(szMSG);
}

void LogReader::DeleteSemaphore(void)
{
	CloseHandle(m_hThreadSemaphore);
}

void LogReader::GetSemaphore()
{
	DWORD dwWaitResult = WaitForSingleObject(
		m_hThreadSemaphore, INFINITE);
}

void LogReader::ReturnSemaphore()
{
	long previous;
	int err = ReleaseSemaphore( 
		m_hThreadSemaphore,  // handle to semaphore
		1,           // increase count by one
		&previous);
}

/**	----------------------------------------------------------------------------
	\brief	

	\param	
	\return	
------------------------------------------------------------------------------*/
void LogReader::ClearServerList()
{
	int cnt = (int)m_ServerList.size();
	std::list<SERVERINFO *>::iterator iter = m_ServerList.begin();
	for (int i = 0; i < cnt; i++)
	{
		SERVERINFO * pData = *iter;
		delete pData;
		pData = NULL;
		iter++;
	}
	m_ServerList.empty();
}

/**	----------------------------------------------------------------------------
	\brief	

	\param	
	\return	
------------------------------------------------------------------------------*/
void LogReader::ClearLogDataList()
{
	size_t cnt = m_LogDataList.size();
	std::list<LPVOID>::iterator iter = m_LogDataList.begin();

	for(unsigned int i = 0; i< cnt; i++)
	{
		PVOID pData = *iter;
		delete pData, pData = NULL;
		iter++;
	}
	m_LogDataList.clear();

	size_t cnt2 = m_ReadList.size();
	std::list<TCHAR*>::iterator iter2 = m_ReadList.begin();

	for(unsigned int i = 0; i < cnt2; i++)
	{
		TCHAR* pszString = *iter2;
		delete [] pszString, pszString = NULL;
		iter2++;
	}

	m_ReadList.clear();
}

VOID LogReader::PrintfFirstReadFileName(SERVERINFO* pInfo)
{
	HANDLE hStdOut = ::GetStdHandle(STD_OUTPUT_HANDLE);
	TCHAR szMsg[MAX_PATH] = {0,};

	if(NULL == pInfo)
	{
		StringCchPrintf(szMsg, sizeof(szMsg), _T("\n"));
		PrintUserColorString(hStdOut, USER_GREEN | USER_WHITE_BG, szMsg);

		std::list<SERVERINFO*>::iterator iter = m_ServerList.begin();

		for(int nCount = 0; nCount < (int)m_ServerList.size(); nCount++)
		{
			SERVERINFO* pstInfo = *iter;

			StringCchPrintf(szMsg, sizeof(szMsg), _T("READ FILE : %s"), pstInfo->szFullPath);
			PrintUserColorString(hStdOut, USER_GREEN | USER_WHITE_BG, szMsg);	
		}
	}
	else
	{
		StringCchPrintf(szMsg, sizeof(szMsg), _T("READ FILE : %s"), pInfo->szFullPath);
		PrintUserColorString(hStdOut, USER_GREEN | USER_WHITE_BG, szMsg);
	}
	
}

/**	----------------------------------------------------------------------------
	\brief	S T A R T

	\param	
	\return	
------------------------------------------------------------------------------*/
int LogReader::StartReadLogFile()
{
	ClearServerList();

	if ( 0 != GetServerInfo() )
	{
		//OutputDebugString(_T("GetServerInfo Fail"));
		HANDLE hStdOut = ::GetStdHandle(STD_OUTPUT_HANDLE);
		PrintUserColorString(hStdOut, USER_RED | USER_SKYBLUE_BG, _T("GetServerInfo Fail\n"));
		return -1;
	}

	// 읽을 서버 갯수만큼 무조건 셋팅!
	TCHAR szMSG[100] = {0,};
	StringCchPrintf(szMSG, sizeof(szMSG), _T("Semaphore Count : %d"), m_ServerList.size());
	OutputDebugString(szMSG); 

#ifdef _KOREA
	MakeSemaphore(7);
#else
	MakeSemaphore((int)m_ServerList.size());
#endif
	//
	
    if(is_restore_)
    {
        g_SUNLogMain.SetTimeSpanLog(TRUE);
        StartRestore();
    }
    else
    {
        if ( 0 == m_szStartDate[0] )
        {
            StartRealTimeRead();
        }
        else
        {
            g_SUNLogMain.SetTimeSpanLog(TRUE);
            StartTimeSpanRead();
        }
    }
	
	return 0;
}

/**	----------------------------------------------------------------------------
	\brief	실시간 로그 수집
			서버의 수만큼의 THREAD 실행
	\param	
	\return	
------------------------------------------------------------------------------*/
void LogReader::StartRealTimeRead()
{
    int cnt = (int)m_ServerList.size();

	std::list<SERVERINFO *>::iterator iter = m_ServerList.begin();


	HANDLE hStdOut = ::GetStdHandle(STD_OUTPUT_HANDLE);

	for (int i = 0; i < cnt; i++)
	{
		SERVERINFO * pParam = new SERVERINFO;
		memcpy(pParam, *iter, sizeof(SERVERINFO));
		// pParam은 thread 실행 후 복사본을 만들고 바로 삭제된다.

		DWORD dwThreadID = 0;
		HANDLE hReadThread = (HANDLE)_beginthreadex(NULL, 
													0, 
													RealTimeReadLogThread, 
													pParam, 
													0, 
													(unsigned*)&dwThreadID);

		PrintfFirstReadFileName(pParam);
	
		iter++;
	}

	TCHAR szMSG[100] = {0,};
	StringCchPrintf(szMSG, sizeof(szMSG), _T("Read ThreadCount : %d"), cnt);

	PrintUserColorString(hStdOut, USER_BLUE | USER_SKYBLUE_BG, szMSG);

	StringCchPrintf(szMSG, sizeof(szMSG), _T("Total Read ThreadCount : %d"), cnt);
	PrintUserColorString(hStdOut, USER_BLUE | USER_SKYBLUE_BG, szMSG);
}

void LogReader::StartRestore()
{
    int cnt = (int)m_ServerList.size();
    std::list<SERVERINFO *>::iterator iter = m_ServerList.begin();

    for (int i = 0; i < cnt; i++)
    {

        SERVERINFO * pParam = new SERVERINFO;
        memcpy(pParam, *iter, sizeof(SERVERINFO));        

        DWORD dwThreadID = 0;

        HANDLE hReadThread = (HANDLE)_beginthreadex(NULL, 
            0, 
            RestoreReadLogThread, 
            pParam, 
            0, 
            (unsigned*)&dwThreadID);

        iter++;
    }
}
/**	----------------------------------------------------------------------------
	\brief	기간 지정, IP 지정 로그 수집
			지정 IP가 있는경우 한개의 THREAD 실행
			지정 IP가 없는경우 서버의 수만큼의 THREAD 실행
	\param	
	\return	
------------------------------------------------------------------------------*/
void LogReader::StartTimeSpanRead()
{
	TCHAR szCurrentDate[16] = {0};
	BOOL  bNextDay          = TRUE;

	if (FAILED(StringCchCopy(szCurrentDate, sizeof(szCurrentDate), m_szStartDate)))
	{
		_ASSERTE(!"StringCchCopy");
	}

	while(bNextDay)
	{
		int cnt = (int)m_ServerList.size();
		std::list<SERVERINFO *>::iterator iter = m_ServerList.begin();

		for (int i = 0; i < cnt; i++)
		{
			
			SERVERINFO * pParam = new SERVERINFO;
			memcpy(pParam, *iter, sizeof(SERVERINFO));
			// pParam은 thread 실행 후 복사본을 만들고 바로 삭제된다.

			if(!m_bLocalLog)
			{
				GetReadFilePath( pParam, szCurrentDate );
				// szCurrentDate 날짜로 파일 명 찾기...
			}
			else
			{
				GetReadLocalFilePath(pParam, szCurrentDate);
			}

			DWORD dwThreadID = 0;
			HANDLE hReadThread = (HANDLE)_beginthreadex(NULL, 
														0, 
														TimeSpanReadLogThread, 
														pParam, 
														0, 
														(unsigned*)&dwThreadID);
			iter++;

			PrintfFirstReadFileName(pParam);
		} // for

		

		bNextDay = IsNextDay(szCurrentDate);
	} //while
}

/**	----------------------------------------------------------------------------
	\brief	

	\param	
	\return	
------------------------------------------------------------------------------*/
void LogReader::Stop()
{
	m_bReadThreadExitFlag = TRUE;
}

/**	----------------------------------------------------------------------------
	\brief	DB에서 읽은 서버 정보와,
			초기값을 셋팅하여 m_ServerList에 추가
	\param	
	\return	
------------------------------------------------------------------------------*/
int LogReader::GetServerInfo()
{
#ifdef YOUNGMOON_TEST
    SERVERINFO * pstInfo = new SERVERINFO;
    memset(pstInfo, 0, sizeof(SERVERINFO));

    pstInfo->ulOffset     = 0;
    pstInfo->eLogState    = LOGSTATE_LOADING;    
    pstInfo->nServerCode  = 1;
    pstInfo->nChannelID   = 1;
    pstInfo->nWorldNo	  = 9;
    pstInfo->nServerType  = FIELD_SERVER;
    StringCchCopy(pstInfo->szPrefix, 24, _T("FIELD"));
    StringCchCopy(pstInfo->szServerIP, sizeof(pstInfo->szServerIP), "10.101.12.27");

//    GetReadFilePath(pstInfo);

    m_ServerList.push_back(pstInfo);
    m_nTotalReadCount++;

    SERVERINFO * pstInfo1 = new SERVERINFO;
    memset(pstInfo1, 0, sizeof(SERVERINFO));

    pstInfo1->ulOffset     = 0;
    pstInfo1->eLogState    = LOGSTATE_LOADING;    
    pstInfo1->nServerCode  = 1;
    pstInfo1->nChannelID   = 1;
    pstInfo1->nWorldNo	  = 9;
    pstInfo1->nServerType  = FIELD_SERVER;
    StringCchCopy(pstInfo1->szPrefix, 24, _T("BATTLE"));
    StringCchCopy(pstInfo1->szServerIP, sizeof(pstInfo->szServerIP), "10.101.12.27");

    //    GetReadFilePath(pstInfo);

    m_ServerList.push_back(pstInfo1);
    m_nTotalReadCount++;
    return 0;
#endif

    
	// Config.ini 파일에서 DB 정보를 읽기
	TCHAR szDSN[MAX_DB_PW];
	TCHAR szDBID[MAX_DB_PW];
	TCHAR szDBPWD[MAX_DB_PW];
	TCHAR szLocalDir[MAX_PATH];
	int   iWorldNum          = 0;
	ULONGLONG ulWorldNum     = 0;
	INT nLocalChCount = 0;

	ZeroMemory(szDSN, sizeof(szDSN));
	ZeroMemory(szDBID, sizeof(szDBID));
	ZeroMemory(szDBPWD, sizeof(szDBPWD));
	ZeroMemory(szLocalDir, sizeof(szLocalDir));

	HANDLE hStdOut = ::GetStdHandle(STD_OUTPUT_HANDLE);

	m_pConfigIni->GetValue(_T("LOCAL"), _T("PATH") , _T(""), szLocalDir, MAX_PATH);
	nLocalChCount = (INT)m_pConfigIni->GetValue(_T("LOCAL"), _T("CH_COUNT"), 1);

	if(m_nWorld <= 0)
	{
		ulWorldNum = m_pConfigIni->GetValue(_T("COMMON"), _T("WORLDNO"), 1);
	}
	else
	{
		ulWorldNum = m_nWorld;
	}

	iWorldNum = (int)ulWorldNum;

	
	if(_tcslen(szLocalDir) > 1 && nLocalChCount >= 1 && _tcslen(m_szStartDate) > 2)
	{
		m_bLocalLog = TRUE;

		TCHAR szLocalPrefixList[5][24] = {_T("agent"), _T("field"), _T("world"), _T("battle1"), _T("battle2")};
		TCHAR szPrefixList[5][24] = {_T("AGENT"), _T("FIELD"), _T("WORLD"), _T("BATTLE"), _T("GUILD")};

		for(int nCount = 1; nCount <= nLocalChCount; nCount++)
		{
			StringCchCopy(m_szLocalLogPath, sizeof(m_szLocalLogPath), szLocalDir);

			for(int nPrefixCount = 0; nPrefixCount < 5; nPrefixCount++)
			{
				SERVERINFO * pInfo = new SERVERINFO;

				StringCchCopy(pInfo->szPrefix, sizeof(pInfo->szPrefix), szPrefixList[nPrefixCount]);
				pInfo->nServerCode = iWorldNum;
				pInfo->nWorldNo = iWorldNum;
				pInfo->nChannelID = nCount;
	
				GetReadLocalFilePath(pInfo, m_szStartDate, szLocalPrefixList[nPrefixCount]);

				m_ServerList.push_back(pInfo);

				m_nTotalReadCount++;
			}
			
		}
	}
	else
	{
		m_bLocalLog = FALSE;

		m_pConfigIni->GetValue(_T("COMMON"), _T("AUTHDBNAME"), _T(""), szDSN  , MAX_DB_PW);
		m_pConfigIni->GetValue(_T("COMMON"), _T("AUTHDBID")  , _T(""), szDBID , MAX_DB_PW);
		m_pConfigIni->GetValue(_T("COMMON"), _T("AUTHDBPWD") , _T(""), szDBPWD, MAX_DB_PW);

#ifdef USE_SEPERATED_CHANNEL
        int target_channel = m_pConfigIni->GetValue(_T("COMMON"), _T("TARGET_CHANNEL"), 1);
#endif //USE_SEPERATED_CHANNEL

		// DB 접속
		DatabasePool dbPool(1, szDSN, szDBID, szDBPWD, _T("TmpDB"));
		if ( 0 != dbPool.DBConnect() )
		{
			PrintUserColorString(hStdOut, USER_RED | USER_SKYBLUE_BG, _T("DBConnect FAIL\n"));
			//OutputDebugString("DBConnect FAIL");
			return -1;
		}

		SQLTCHAR    sqlQuery[SQL_MAX_MESSAGE_LENGTH] = {0};
		SQLHSTMT   hstmt = SQL_NULL_HSTMT;
		SQLHDBC    hdbc  = SQL_NULL_HDBC;
		SQLRETURN  retcode;
		SQLINTEGER pIndicators[2];

		DBInfo * dbConnection = dbPool.GetDBHandle();
		if ( NULL == dbConnection )
		{
			PrintUserColorString(hStdOut, USER_RED | USER_SKYBLUE_BG, _T("GetDBHandle FAIL\n"));
			//OutputDebugString("GetDBHandle FAIL");
			return -1;
		}

		hdbc  = dbConnection->hSQLConnection;
		hstmt = AllocStatement(hdbc);


		/*
		#ifdef _DEBUG
		SERVERINFO * pstInfo2 = new SERVERINFO;
		memset(pstInfo2, 0, sizeof(SERVERINFO));

		pstInfo2->ulOffset     = 0;
		pstInfo2->eLogState    = LOGSTATE_LOADING;
		pstInfo2->nServerType  = 4;
		pstInfo2->nServerCode  = 1;


		StringCchPrintf(pstInfo2->szPrefix, sizeof(pstInfo2->szPrefix), _T("FIELD"));
		StringCchPrintf(pstInfo2->szFileName, sizeof(pstInfo2->szFileName), _T("%s"), _T("814_SUN_FIELD__Date_20080825.wzl"));
		StringCchPrintf(pstInfo2->szFullPath, sizeof(pstInfo2->szFullPath), _T("%s"), _T("\\\\10.101.203.23\\Log\\FIELD\\814_SUN_FIELD__Date_20080825.wzl"));
		StringCchPrintf(pstInfo2->szPath, sizeof(pstInfo2->szPath), _T("%s"), _T("Log"));
		StringCchPrintf(pstInfo2->szServerIP, sizeof(pstInfo2->szServerIP), _T("%s"), _T("10.101.203.23"));
		GetReadFilePath(pstInfo2);


		m_ServerList.push_back(pstInfo2);
		#else //_DEBUG
		*/
#ifdef _TEST_SERVER
		if (FAILED(StringCchPrintf((TCHAR *)sqlQuery, sizeof(sqlQuery),
			_T("exec USP_GetServerInfo %d"),
			iWorldNum)))
#else
		if (FAILED(StringCchPrintf((TCHAR *)sqlQuery, sizeof(sqlQuery),
			_T("exec USP_GetServerInfo_LOG %d"),
			iWorldNum)))
#endif //_TEST_SERVER
		{
			FreeStatement(hstmt);
			dbPool.ReleaseDBHandle(dbConnection);
			_ASSERTE(!"StringCchPrintf");
			return -1;
		}

		TCHAR szIneerIP[32+1] = {0};
		BYTE btServerType; 
		BYTE btWorldID;
		BYTE btChannelID;

		SQLBindCol(hstmt, 1, SQL_C_CHAR   ,  szIneerIP   , 32 , &pIndicators[0]);
		SQLBindCol(hstmt, 2, SQL_C_TINYINT, &btServerType, sizeof(BYTE), &pIndicators[0]);
		SQLBindCol(hstmt, 3, SQL_C_TINYINT, &btWorldID   , sizeof(BYTE), &pIndicators[0]);
		SQLBindCol(hstmt, 4, SQL_C_TINYINT, &btChannelID , sizeof(BYTE), &pIndicators[0]);

		retcode = SQLExecDirect(hstmt, sqlQuery, SQL_NTS);

		if (!(retcode == SQL_SUCCESS || 
			retcode == SQL_SUCCESS_WITH_INFO) )
		{
			SQLTCHAR       SqlState[6] = {0};
			SQLTCHAR       Msg[SQL_MAX_MESSAGE_LENGTH] = {0};
			SQLINTEGER    NativeError;
			SQLSMALLINT   i = 1;
			SQLSMALLINT   MsgLen;
			SQLGetDiagRec(SQL_HANDLE_STMT, hstmt, i, SqlState, &NativeError,
				Msg, sizeof(Msg), &MsgLen);
			FreeStatement(hstmt);
			dbPool.ReleaseDBHandle(dbConnection);			
			return -1;
		}

		// DB에서 읽어온 서버 정보를 m_ServerList에 저장, 초기화

		SQLRETURN rcsub;

		while ((rcsub = SQLFetch(hstmt)) != SQL_NO_DATA) 
		{
			if (rcsub == 0 && pIndicators[0] == SQL_NULL_DATA)
			{
				PrintUserColorString(hStdOut, USER_RED | USER_SKYBLUE_BG, _T("Return NULL DATA Server IP And Prifix\n"));
				//OutputDebugString("Return NULL DATA Server IP And Prifix");
				break;
			}

#ifdef USE_SEPERATED_CHANNEL
            if (target_channel <= 1 && btChannelID > target_channel) {
                continue;
            }

            if (target_channel > 1 && btChannelID != target_channel) {
                continue;
            }
#endif //USE_SEPERATED_CHANNEL

			SERVERINFO * pstInfo = new SERVERINFO;
			memset(pstInfo, 0, sizeof(SERVERINFO));

			pstInfo->ulOffset     = 0;
			pstInfo->eLogState    = LOGSTATE_LOADING;
			pstInfo->nServerType  = btServerType;
			pstInfo->nServerCode  = btWorldID;
			pstInfo->nChannelID   = btChannelID;
			pstInfo->nWorldNo	  = (int)ulWorldNum;

			if(0 == g_SUNLogMain.m_nChannelID || 0 == g_SUNLogMain.m_nWorldID)
			{
				g_SUNLogMain.m_nChannelID = btChannelID;
				g_SUNLogMain.m_nWorldID = btWorldID;
			}

			GetServerTypeString(pstInfo->nServerType, pstInfo->szPrefix);


			if ( FAILED(StringCchCopy(pstInfo->szServerIP,
				sizeof(pstInfo->szServerIP), 
				szIneerIP)))
			{
				FreeStatement(hstmt);
				dbPool.ReleaseDBHandle(dbConnection);
				_ASSERTE(!"StringCchCopy");
				delete pstInfo;
				return -1;
			}

			if (!_tcscmp(pstInfo->szPrefix, _T("UNKNOWN")))
			{
				delete pstInfo;
				continue;
			}

			if(0 < _tcslen(m_szPrefix))
			{
				if (0 != _tcscmp(pstInfo->szPrefix, m_szPrefix))
				{
					delete pstInfo;
					continue;
				}
			}

			if ( 0 < _tcslen(m_szServerIP) )
			{
				// 지정된 IP가 있다면 해당 아이피의 서버가 아닐경우
				// 건너 뛴다.
				if ( 0 != _tcscmp(pstInfo->szServerIP, m_szServerIP) )
				{
					delete pstInfo;
					continue;
				}
			}
			/*
			if(0 == strcmp("10.13.10.27", pstInfo->szServerIP))
			{
			delete pstInfo;
			continue;
			}
			*/
			GetReadFilePath(pstInfo);
            ;{
                if (IsExistFilePath(pstInfo->szFullPath))
                {
                    IniControl* read_ini = GetReadIni();
                    // 오늘 날짜의 로그 파일이 중간에 멈춰진 상태인 경우 해당 오프셋을 가져온다.
                    pstInfo->ulOffset = read_ini->GetValue(pstInfo->szFullPath, _T("OFFSET"), 0);
                    pstInfo->eLogState = LOGSTATE_LOADING;
                }
            };

			m_ServerList.push_back(pstInfo);

			m_nTotalReadCount++;
		}



		//#endif //_DEBUG

		FreeStatement(hstmt);
		dbPool.ReleaseDBHandle(dbConnection);

	}
	

	if(1 > m_ServerList.size())
	{
		return -1;
	}

	return 0;
}

BOOL LogReader::GetTodayDate(TCHAR * szDate)
{
    SYSTEMTIME stTimeTmp;
    GetLocalTime(&stTimeTmp);

    if (FAILED(StringCchPrintf(szDate, 16,
        _T("%04d%02d%02d"),
        stTimeTmp.wYear, 
        stTimeTmp.wMonth, 
        stTimeTmp.wDay)))
    {
        return FALSE;
    }
    return TRUE;
}

/**	----------------------------------------------------------------------------
	\brief	오늘 날짜를 이용하여 읽어들일 로그 파일 이름을 생성한다.
			szDate를 지정하면 해당 날짜로 파일이름을 생성
	\param	
	\return	
------------------------------------------------------------------------------*/
void LogReader::GetReadFilePath(SERVERINFO * pInfo, TCHAR * szDate)
{
	TCHAR szTmpTime[100] = {0};

	if (NULL == pInfo)
	{
		_ASSERTE(!"GetReadFilePath");
	}

	if ( NULL == szDate )
	{
		// 지정된 날짜가 없다면 오늘 날짜로 설정
		SYSTEMTIME stTimeTmp;
		GetLocalTime(&stTimeTmp);

		if (FAILED(StringCchPrintf(szTmpTime, sizeof(szTmpTime),
									_T("%04d%02d%02d"),
									stTimeTmp.wYear, 
									stTimeTmp.wMonth, 
									stTimeTmp.wDay)))
		{
			_ASSERTE(!"StringCchPrintf");
		}

		// szFileName : OO_szPrefix_szDate.xxx 
#ifdef _RENAME
		TCHAR szVersion[7] = {0,};
		int nVersion = 0;

		m_pConfigIni->GetValue(_T("UPDATE"), _T("VERSION"), _T("815"), 
			szVersion, sizeof(szVersion));

		nVersion = atoi(szVersion);

		if (FAILED(StringCchPrintf(pInfo->szFileName, MAX_PATH,
			m_szLogFileName,
			nVersion,
			pInfo->szPrefix,
			szTmpTime)))
#else
		if (FAILED(StringCchPrintf(pInfo->szFileName, MAX_PATH,
			m_szLogFileName,
			pInfo->szPrefix,
			szTmpTime)))
#endif //_RENAME
		{
			_ASSERTE(!"StringCchPrintf");
		}
/*
		if(0 == _tcscmp(pInfo->szPrefix, _T("GUILD")))
		{
			TCHAR szLogFileName[100];
			INT nGuildVersion = 0;
			memset(szLogFileName, 0, sizeof(szLogFileName));

			m_pConfigIni->GetValue(_T("UPDATE"), _T("RENAME_FILE_FORMAT"), _T("%d_SUN_%s__Date_%s.wzl"), 
				szLogFileName, sizeof(szLogFileName));

			nGuildVersion = (INT)m_pConfigIni->GetValue(_T("UPDATE"), _T("GUILD_VERSION"), 819);

			if (FAILED(StringCchPrintf(pInfo->szFileName, MAX_PATH,
				szLogFileName,
				nGuildVersion,
				pInfo->szPrefix,
				szTmpTime)))
			{
				_ASSERTE(!"StringCchPrintf");
			}
		}
		*/
	}
	else
	{
#ifdef _RENAME
		TCHAR szVersion[7] = {0,};
		int nVersion = 0;

		m_pConfigIni->GetValue(_T("UPDATE"), _T("VERSION"), _T("815"), 
			szVersion, sizeof(szVersion));

		nVersion = atoi(szVersion);

		// szFileName : OO_szPrefix_szDate.xxx 
		if (FAILED(StringCchPrintf(pInfo->szFileName, MAX_PATH,
			m_szLogFileName,
			nVersion,
			pInfo->szPrefix,
			szDate)))
#else
		// szFileName : OO_szPrefix_szDate.xxx 
		if (FAILED(StringCchPrintf(pInfo->szFileName, MAX_PATH,
			m_szLogFileName,
			pInfo->szPrefix,
			szDate)))
#endif //_RENAME
		{
			_ASSERTE(!"StringCchPrintf");
		}
/*
		if(0 == _tcscmp(pInfo->szPrefix, _T("GUILD")))
		{
			TCHAR szLogFileName[100];
			INT nGuildVersion = 0;
			memset(szLogFileName, 0, sizeof(szLogFileName));

			m_pConfigIni->GetValue(_T("UPDATE"), _T("RENAME_FILE_FORMAT"), _T("%d_SUN_%s__Date_%s.wzl"), 
				szLogFileName, sizeof(szLogFileName));

			nGuildVersion = (INT)m_pConfigIni->GetValue(_T("UPDATE"), _T("GUILD_VERSION"), 819);

			if (FAILED(StringCchPrintf(pInfo->szFileName, MAX_PATH,
				szLogFileName,
				nGuildVersion,
				pInfo->szPrefix,
				szDate)))
			{
				_ASSERTE(!"StringCchPrintf");
			}
		}
		*/
	}

	// szPath
	if (FAILED(StringCchCopy(pInfo->szPath, MAX_PATH, READ_FILE_PATH)))
	{
		_ASSERTE(!"StringCcCopy");
	}

	// szFullPath : \\\\10.1.1.1\\szPath\\szPrefix\\szFileName
	if (FAILED(StringCchPrintf(pInfo->szFullPath, MAX_PATH,
								_T("\\\\%s\\%s\\%s\\%s"),
								pInfo->szServerIP,
								pInfo->szPath,
								pInfo->szPrefix, 
								pInfo->szFileName)))
	{
		_ASSERTE(!"StringCchPrintf");
	}
}


/**	----------------------------------------------------------------------------
\brief	특정 폴더 & 날짜를 이용하여 읽어들일 로그 파일 이름을 생성한다.
szDate를 지정하면 해당 날짜로 파일이름을 생성
\param	
\return	
------------------------------------------------------------------------------*/
void LogReader::GetReadLocalFilePath(SERVERINFO * pInfo, TCHAR * szDate, TCHAR* szLocalPrefixList)
{
	TCHAR szTmpTime[100] = {0};

	if (NULL == pInfo || NULL == szDate)
	{
		_ASSERTE(!"GetReadFilePath");
	}

#ifdef _RENAME
	TCHAR szVersion[7] = {0,};
	int nVersion = 0;

	m_pConfigIni->GetValue(_T("UPDATE"), _T("VERSION"), _T("815"), 
		szVersion, sizeof(szVersion));

	nVersion = atoi(szVersion);

	// szFileName : OO_szPrefix_szDate.xxx 
	if (FAILED(StringCchPrintf(pInfo->szFileName, MAX_PATH,
		m_szLogFileName,
		nVersion,
		pInfo->szPrefix,
		szDate)))
#else
		// szFileName : OO_szPrefix_szDate.xxx 
	if (FAILED(StringCchPrintf(pInfo->szFileName, MAX_PATH,
		m_szLogFileName,
		pInfo->szPrefix,
		szDate)))
#endif //_RENAME
	{
		_ASSERTE(!"StringCchPrintf");
	}


/*
	// szPath
	if (FAILED(StringCchCopy(pInfo->szPath, MAX_PATH, READ_FILE_PATH)))
	{
		_ASSERTE(!"StringCcCopy");
	}

	// szFullPath : \\\\10.1.1.1\\szPath\\szPrefix\\szFileName
	if (FAILED(StringCchPrintf(pInfo->szFullPath, MAX_PATH,
		_T("\\\\%s\\%s\\%s\\%s"),
		pInfo->szServerIP,
		pInfo->szPath,
		pInfo->szPrefix, 
		pInfo->szFileName)))
	{
		_ASSERTE(!"StringCchPrintf");
	}
	*/


	if(NULL == szLocalPrefixList)
	{
		if(FAILED(StringCchPrintf(pInfo->szFullPath, sizeof(pInfo->szFullPath), _T("%s\\%dch\\%s\\%s"), 
			m_szLocalLogPath,
			pInfo->nChannelID, 
			pInfo->szLocalPrefixPath,
			pInfo->szFileName)))
		{
			_ASSERTE(!"StringCchPrintf");
		}
	}
	else
	{
		StringCchCopy(pInfo->szLocalPrefixPath, sizeof(pInfo->szLocalPrefixPath), szLocalPrefixList);

		if(FAILED(StringCchPrintf(pInfo->szFullPath, sizeof(pInfo->szFullPath), _T("%s\\%dch\\%s\\%s"), 
			m_szLocalLogPath,
			pInfo->nChannelID, 
			szLocalPrefixList,
			pInfo->szFileName)))
		{
			_ASSERTE(!"StringCchPrintf");
		}
	}

	
}

/**	----------------------------------------------------------------------------
	\brief	파일의 날짜를 비교하여 오늘 날짜의 파일인지 확인한다.

	\param	
	\return	
------------------------------------------------------------------------------*/

#ifdef _NA_0_20110803_DIV_GAMELOG_FILE

BOOL LogReader::InCreaseFileNumber(const TCHAR* src_file, TCHAR* des_file, size_t des_size)
{
    TCHAR* find_pos = _tcsstr(src_file, "_Date_");
    if (find_pos)
    {
        int date_pos = find_pos - src_file;

        // "_Date_yyyymmdd_xxxx.wzl"
        // "          1         2  "
        // "01234567890123456789012"
        int file_number_pos = date_pos + 14;

        if (src_file[file_number_pos] == '_')
        {
            TCHAR file_number[10];
            int temp = file_number_pos + 1;
            for (int idx = 0 ; src_file[temp] != '.' ; ++idx)
            {
                file_number[idx] = src_file[temp];
                file_number[idx + 1] = '\0';
                ++temp;
            }

            int number = atoi(file_number);
            ++number;

            TCHAR first_string[MAX_PATH];
            _tcscpy(first_string, src_file);
            first_string[file_number_pos] = '\0';
            _sntprintf(des_file, des_size, "%s_%d.wzl", first_string, number);
        }
        else
        {
            TCHAR first_string[MAX_PATH];
            _tcscpy(first_string, src_file);
            first_string[file_number_pos] = '\0';
            _sntprintf(des_file, des_size, "%s_1.wzl", first_string);
        }
        return TRUE;
    }
    return FALSE;
}

BOOL LogReader::IsReadComplete(SERVERINFO* sInfo)
{
    CWriteLogMng manager;
    fpos_t end_offset = 0;
    BOOL result = FALSE;
    if (manager.Open(sInfo->szFullPath, end_offset))
    {
        if (sInfo->ulOffset >= end_offset)
            result = TRUE;
        manager.Close();
    }
    return result;
}

BOOL LogReader::IsExistFilePath(const TCHAR* file_path)
{
    DWORD error = ::GetLastError();
    HANDLE temp_handle = INVALID_HANDLE_VALUE;
    int file_handle = _open(file_path, _O_BINARY|_O_RDONLY, _S_IREAD);
    if(file_handle == -1)
    {
        ::SetLastError(error);
        return FALSE;
    }

    _close(file_handle);
    ::SetLastError(error);
    return TRUE;
}

void LogReader::ChangeNextFilePath(SERVERINFO* sInfo, CHANGE_FILE& OUT result)
{
    result.change_ = false;
    result.change_day_ = false;

    IniControl* read_ini = GetReadIni();

    // 전체 경로가 "" 일 경우 오늘 날짜로 시작.
    if (_tcslen(sInfo->szFullPath) == 0)
    {
        GetReadFilePath(sInfo);

        if (IsExistFilePath(sInfo->szFullPath))
        {
            // 오늘 날짜의 로그 파일이 중간에 멈춰진 상태인 경우 해당 오프셋을 가져온다.
            sInfo->ulOffset = read_ini->GetValue(sInfo->szFullPath, _T("OFFSET"), 0);
            sInfo->eLogState = LOGSTATE_LOADING;

            TCHAR message[MAX_PATH] = { 0, };
            StringCchPrintf(message, sizeof(message), \
				_T("[%s:%d] start at path : %s, offset : %I64d") , \
				__FUNCTION__, __LINE__,
                sInfo->szFullPath, sInfo->ulOffset);
            LogCollectorLog Log(_T("EXPLOG"), _T("EXPLOG"));
            Log.AddLog(message);
        }
        return;
    }
    

    SERVERINFO out_info;
    int find_next = FindNextFilePath(sInfo, &out_info);
    if (find_next == 0)
        return;

    // 다음 파일을 찾은 경우.
    if (sInfo->nRepeatCount >= 10)
    {
        // 현재 파일 로그 읽기 종료.
        sInfo->eLogState = LOGSTATE_END;
        sInfo->nRepeatCount = 0;
        SaveLastReadInfo(sInfo);

        // 다음 파일 로그 시작.
        memcpy(sInfo, &out_info, sizeof(SERVERINFO));
        sInfo->ulOffset = read_ini->GetValue(sInfo->szFullPath, _T("OFFSET"), 0);
        sInfo->eLogState = LOGSTATE_LOADING;
        sInfo->nRepeatCount = 0;
        SaveLastReadInfo(sInfo);

        if (find_next == 2)
            result.change_day_ = true;
        else
            result.change_day_ = false;

        result.change_ = true;
    }
}

int LogReader::FindNextFilePath(SERVERINFO* sInfo, SERVERINFO* OUT out_put)
{
    memcpy(out_put, sInfo, sizeof(SERVERINFO));

    DWORD error = ::GetLastError();
    TCHAR next_file_path[MAX_PATH];
    if (InCreaseFileNumber(out_put->szFullPath, next_file_path, MAX_PATH))
    {
        if (IsExistFilePath(next_file_path))
        {
            _tcscpy(out_put->szFullPath, next_file_path);
            ::SetLastError(error);
            return 1;
        }
    }

    if (!IsTodayFile(sInfo))
    {
        // 날짜가 바뀌었다. 파일 이름, offset 다시 설정
        GetReadFilePath(out_put);

        if (IsExistFilePath(out_put->szFullPath))
        {
            ::SetLastError(error);
            return 2;
        }
    }

    ::SetLastError(error);

    return 0;
}

BOOL LogReader::GetNextFilePath(SERVERINFO* sInfo)
{
    TCHAR next_file_path[MAX_PATH];
    if (InCreaseFileNumber(sInfo->szFullPath, next_file_path, MAX_PATH))
    {
        if (IsExistFilePath(next_file_path))
        {
            _tcscpy(sInfo->szFullPath, next_file_path);
            return TRUE;
        }
    }
    return FALSE;
}

#endif //_NA_0_20110803_DIV_GAMELOG_FILE





BOOL LogReader::IsTodayFile(SERVERINFO* sInfo)
{
	if (NULL == sInfo->szFileName)
	{
		_ASSERTE(!"IsTodayFile");
		return -1;
	}

	if(1 > _tcslen(sInfo->szFileName))
	{
		return FALSE;
	}

	TCHAR*	   szFileName = sInfo->szFileName;
	TCHAR      szDate[20] = {0};
	SYSTEMTIME stTimeTmp;
	GetLocalTime(&stTimeTmp);

	if (FAILED(StringCchPrintf(szDate, sizeof(szDate),
								_T("%04d%02d%02d"),
								stTimeTmp.wYear, 
								stTimeTmp.wMonth, 
								stTimeTmp.wDay)))
	{
		_ASSERTE(!"StringCchPrintf");
	}	

	int len = 0;

	if ( FAILED(StringCchLength( szFileName, MAX_PATH, (size_t *)&len )) )
	{
		_ASSERTE(!"StringCchLength");
	}

#ifdef _NA_0_20110803_DIV_GAMELOG_FILE
    TCHAR* find_pos = _tcsstr(szFileName, szDate);
    if (find_pos)
        return TRUE;
#else
	if (0 == _tcsnicmp( (TCHAR *)(szFileName + len - 12), szDate, 8) ||
		!IsLogComplete(sInfo))
	{
		return TRUE;
	}
	else
	{
		sInfo->eLogState = LOGSTATE_END;
	}
#endif
	
	return FALSE;
}


BOOL LogReader::IsLogComplete(SERVERINFO* sInfo, BOOL bFindOffset)
{
	WIN32_FIND_DATA fd32;
	HANDLE handle = NULL;
	LARGE_INTEGER liSize;
	BOOL bResult = TRUE;
	TCHAR szDay[9] = {0};

	::Sleep(200);

	handle = ::FindFirstFile(sInfo->szFullPath, &fd32);

	if(handle != INVALID_HANDLE_VALUE)
	{
		HANDLE hOutput = ::GetStdHandle(STD_OUTPUT_HANDLE);

		liSize.LowPart = fd32.nFileSizeLow;
		liSize.HighPart = fd32.nFileSizeHigh;

		sInfo->ulFileSize = liSize.QuadPart;

		if((ULONGLONG)liSize.QuadPart > sInfo->ulOffset)
		{
			TCHAR szMSG[100] = {0,};

			if(bFindOffset)
			{
				bResult = FALSE;
			}
			else
			{
				TCHAR szFileName[MAX_PATH] = {0,};
				TCHAR* szObj = NULL;
				INT nCount = 0;

				//memcpy(szFileName, sInfo->szFileName, MAX_PATH);
				if(0 < _tcslen(sInfo->szFileName))
				{
					StringCchPrintf(szFileName, sizeof(szFileName), _T("%s"), sInfo->szFileName);

					szObj = _tcstok(szFileName, _T("_"));

					while(szObj != NULL)
					{
#ifndef _RENAME
						if(nCount == 3)
#else
						if(nCount == 4)
#endif //!_RENAME
						{
							break;
						}

						szObj = _tcstok(NULL, _T("_"));
						nCount++;
					}

					if(szObj != NULL)
					{
						StringCchCopy(szDay, sizeof(szDay), szObj);

						StringCchPrintf(szMSG,sizeof(szMSG), _T("\nNOT COMPELETE %s DAY FILE : %s"), szDay, sInfo->szFullPath);
						PrintUserColorString(hOutput, USER_WHITE, szMSG);
						StringCchPrintf(szMSG,sizeof(szMSG), _T("\nRETRY READ %s FILE.\n"), sInfo->szFullPath);
						PrintUserColorString(hOutput, USER_WHITE, szMSG);
					}

				}

				bResult = FALSE;			
			}
		}
		else if((ULONGLONG)liSize.QuadPart <= sInfo->ulOffset)
		{
			bResult = TRUE;
			//sInfo->ulOffset = (ULONGLONG)liSize.QuadPart;
		}
			
		::FindClose(handle);
	}
	else
	{
		if(sInfo->szFullPath != NULL)
		{
			TCHAR szMSG[100] = {0,};
			StringCchPrintf(szMSG, sizeof(szMSG), _T("%s, FAIL FIND FILE"), sInfo->szFullPath);
			OutputDebugString(szMSG);
			bResult = TRUE;
		}
	}

	return bResult;
}

VOID LogReader::RefreshDayDateQueue()
{
	EnterCriticalSection(&m_ReadListCS);

	size_t cnt = m_ReadList.size();
	std::list<TCHAR*>::iterator iter = m_ReadList.begin();

	for(unsigned int i = 0; i < cnt; i++)
	{
		TCHAR* pszString = *iter;
		delete [] pszString, pszString = NULL;
		iter++;
	}

	m_ReadList.clear();
	m_nNextDayCount = 0;

	LeaveCriticalSection(&m_ReadListCS);
}

VOID LogReader::InsertDayDateQueue(LPCTSTR pstrDay)
{
	EnterCriticalSection(&m_ReadListCS);

	if(m_nTotalReadCount != m_nNextDayCount)
	{
		TCHAR* pszName = new TCHAR[200];
		StringCchCopy(pszName, 200, pstrDay);
		m_ReadList.push_back(pszName);

		InterlockedIncrement(&m_nNextDayCount);
	}

	LeaveCriticalSection(&m_ReadListCS);
}

VOID LogReader::PrintNowReadDay()
{
	if(!m_bFirstPrint)
	{
		if(m_nTotalReadCount == m_nNextDayCount)
		{
			TCHAR szMsg[MAX_PATH] = {0,};
			TCHAR* pszFileName = NULL;

			HANDLE Handle = ::GetStdHandle(STD_OUTPUT_HANDLE);

			StringCchPrintf(szMsg, sizeof(szMsg), _T("\n"));
			PrintUserColorString(Handle, USER_GREEN | USER_WHITE_BG, szMsg);

			EnterCriticalSection(&m_ReadListCS);

			size_t nSize = m_ReadList.size();
			std::list<TCHAR*>::iterator iter = m_ReadList.begin();

			for(unsigned int nCount = 0; nCount < nSize; nCount++)
			{
				pszFileName = *iter;

				StringCchPrintf(szMsg, sizeof(szMsg), _T("READ FILE : %s"), pszFileName);
				PrintUserColorString(Handle, USER_GREEN | USER_WHITE_BG, szMsg);

				iter++;

				InterlockedDecrement(&m_nNextDayCount);
			}

			LeaveCriticalSection(&m_ReadListCS);

			RefreshDayDateQueue();
		}

	}
	else
	{
		RefreshDayDateQueue();
		m_bFirstPrint = FALSE;
	}
	
}

/**	----------------------------------------------------------------------------
	\brief	서버로 연결

	\param	
	\return	
------------------------------------------------------------------------------*/
int LogReader::ConnectSession(SERVERINFO * pstInfo)
{
	if (NULL == pstInfo)
	{
		_ASSERTE(!"ConnectSession");
		return -1;
	}

	TCHAR szCmdLine[MAX_PATH] = {0};
	TCHAR szText[MAX_PATH]    = {0};
	TCHAR szValue[MAX_PATH]   = {0};
	TCHAR szUserName[30]      = {0};
	TCHAR szPassword[30]      = {0};
	int   nRet = 0;

	LogCollectorLog cLog(_T("Service"), _T("Log"));

#ifdef _TEST_SERVER
	if(stricmp(pstInfo->szPrefix, _T("AGENT"))==0 || stricmp(pstInfo->szPrefix, _T("FIELD"))==0 || stricmp(pstInfo->szPrefix, _T("WORLD"))==0)
	{
		m_pConfigIni->GetValue(_T("COMMON"), _T("LOGUSERNAME_1"), _T(""), szUserName, MAX_PATH);
		m_pConfigIni->GetValue(_T("COMMON"), _T("LOGPASSWORD_1"), _T(""), szPassword, MAX_PATH);
	}
	else
	{
		m_pConfigIni->GetValue(_T("COMMON"), _T("LOGUSERNAME_2"), _T(""), szUserName, MAX_PATH);
		m_pConfigIni->GetValue(_T("COMMON"), _T("LOGPASSWORD_2"), _T(""), szPassword, MAX_PATH);
	}
#else
	TCHAR szDBName[50];

	ZeroMemory(szDBName, sizeof(szDBName));

	StringCchPrintf(szDBName, sizeof(szDBName), _T("COMMON"));


	m_pConfigIni->GetValue(szDBName, _T("LOGUSERNAME"), _T(""), szUserName, MAX_PATH);
	m_pConfigIni->GetValue(szDBName, _T("LOGPASSWORD"), _T(""), szPassword, MAX_PATH);

	if(_tcslen(szUserName) < 1 || _tcslen(szPassword) < 1)
	{
		// 변경된 버전의 INI 형식을 가지고 있음.
		StringCchPrintf(szDBName, sizeof(szDBName), _T("%dWORLD-DB"), m_nWorld);

		m_pConfigIni->GetValue(szDBName, _T("LOGUSERNAME"), _T(""), szUserName, MAX_PATH);
		m_pConfigIni->GetValue(szDBName, _T("LOGPASSWORD"), _T(""), szPassword, MAX_PATH);
	}
#endif //_TEST_SERVER

	// 세션 연결을 종료한다.
	if (FAILED(StringCchPrintf(szCmdLine, sizeof(szCmdLine),
								_T("net use \\\\%s\\%s /delete"),
								pstInfo->szServerIP 
								,pstInfo->szPath
								)))
	{
		_ASSERTE(!"StringCchPrintf");
		return -1;
	}

#ifdef _UNICODE
	nRet = WinExec(CW2A(szCmdLine), SW_HIDE);
#else
	nRet = system(szCmdLine);
	//nRet = WinExec(szCmdLine, SW_HIDE);
#endif //_UNICODE

	::Sleep(3000);

	cLog.AddLog(_T("WinExec %s RetCode %d"), szCmdLine, nRet);

	// 공유 계정으로 세션 연결 시도
	if (FAILED(StringCchPrintf(szCmdLine, sizeof(szCmdLine),
								_T("net use \\\\%s\\%s /user:%s %s"),
								pstInfo->szServerIP, 
								pstInfo->szPath, 
								szUserName, 
								szPassword)))
	{
		_ASSERTE(!"StringCchPrintf");
		return -1;
	}
#ifdef _UNICODE
	nRet = WinExec(CW2A(szCmdLine), SW_HIDE);
#else
	nRet = system(szCmdLine);
	//nRet = WinExec(szCmdLine, SW_HIDE);
#endif //_UNICODE

	::Sleep(3000);

	cLog.AddLog(_T("WinExec %s RetCode %d"), szCmdLine, nRet);

	return 0;
}

/**	----------------------------------------------------------------------------
	\brief	실시간 로그 수집 THREAD 
			종료 플래그가 셋 될때까지 무한 루프를 돌며 로그를 수집
	\param	
	\return	
------------------------------------------------------------------------------*/
unsigned int __stdcall LogReader::RealTimeReadLogThread(LPVOID lParam)
{
	if ( NULL == lParam )
	{
		_ASSERTE(!"RealTimeLogReadThread");
		return -1;
	}

	LogReader  * pLogReader = (LogReader*)g_SUNLogMain.GetLogReader();
	IniControl * pReadIni   = pLogReader->GetReadIni();

	InterlockedIncrement((LONG *)&(pLogReader->m_iRunningThread));

	SERVERINFO sInfo;
	memcpy( &sInfo, lParam, sizeof(SERVERINFO) );

	delete lParam,lParam = NULL;

	while (TRUE)
	{
		if ( pLogReader->m_bReadThreadExitFlag )
		{
			pLogReader->SaveLastReadInfo(&sInfo);

			pLogReader->RefreshDayDateQueue();

			break;
		}

    #ifdef _NA_0_20110803_DIV_GAMELOG_FILE
        CHANGE_FILE change_file;
        pLogReader->ChangeNextFilePath(&sInfo, change_file);
        if (change_file.change_ && change_file.change_day_)
        {
            pLogReader->InsertDayDateQueue(sInfo.szFullPath);
        }
        pLogReader->PrintNowReadDay();
    #else
        // 현재 게임서버에서 읽은 로그데이터의 최종 파일명과 오프셋, 
        // 경로를 Read ini에서 읽어온다.
        // sInfo.szFullPath : \\\\10.101.203.12\\XXXXX\\XXXXX\\XXX_XXX_20060905.wzl
        pReadIni->GetValue(sInfo.szFullPath, _T("FILENAME"), _T(""), sInfo.szFileName, MAX_PATH);

        if (!pLogReader->IsTodayFile(&sInfo))
		{
			pLogReader->SaveLastReadInfo(&sInfo);
			// 날짜가 바뀌었다. 파일 이름, offset 다시 설정
			sInfo.ulOffset = 0;
			pLogReader->GetReadFilePath(&sInfo);

			sInfo.ulOffset = pReadIni->GetValue(sInfo.szFullPath, _T("OFFSET"), 0);

			pLogReader->InsertDayDateQueue(sInfo.szFullPath);
		}
		else
		{
			TCHAR szValue[MAX_PATH] = {0};

			pReadIni->GetValue(sInfo.szFullPath, _T("FILENAME"), _T(""), szValue, MAX_PATH);

			if ( 0 == szValue[0] )
			{
				sInfo.ulOffset = 0;
			}
			else
			{
				sInfo.ulOffset = pReadIni->GetValue(sInfo.szFullPath, _T("OFFSET"), 0);
			}

			pLogReader->PrintNowReadDay();
		}
    #endif //_NA_0_20110803_DIV_GAMELOG_FILE


		pLogReader->GetSemaphore();

		pLogReader->DoReadLogFile(&sInfo);

		pLogReader->ReturnSemaphore();

		// read ini 갱신 : 로그 데이터의 최종 파일명과 오프셋 기록
		pLogReader->SaveLastReadInfo(&sInfo);

		Sleep(1);

	} // while

	InterlockedDecrement((LONG *)&(pLogReader->m_iRunningThread));

	//_endthreadex(0);

	return 0;
}


unsigned int __stdcall LogReader::RestoreReadLogThread(LPVOID lParam)
{
    LogReader  * pLogReader = (LogReader*)g_SUNLogMain.GetLogReader();
    IniControl * pReadIni   = pLogReader->GetReadIni();

    if (NULL == lParam)
    {
        _ASSERTE(!"RestoreReadLogThread");
        return -1;
    }

    InterlockedIncrement((LONG *)&(pLogReader->m_iRunningThread));

    pLogReader->GetSemaphore(); // 서버가 많아지면 추가..

    SERVERINFO sInfo;
    memcpy(&sInfo, lParam, sizeof(SERVERINFO));
    delete lParam, lParam = NULL;

    //////////////////////////////////////////////////////////////////////////
    TCHAR szCurrentDate[16] = {0};
        
    if(!pLogReader->GetTodayDate(szCurrentDate))
    {        
        return -1;
    }

    BYTE error_count = 0;

    while(pLogReader->IsPrevDay(szCurrentDate))
    {
        SERVERINFO server_info;
        memcpy(&server_info, &sInfo, sizeof(SERVERINFO));

        if(!pLogReader->m_bLocalLog)
        {
            pLogReader->GetReadFilePath( &server_info, szCurrentDate );
            // szCurrentDate 날짜로 파일 명 찾기...
        }
        else
        {
            pLogReader->GetReadLocalFilePath(&server_info, szCurrentDate);
        }

        TCHAR szValue[MAX_PATH] = {0};

        // 타임 구간이 설정 되었을 경우, 무한 루프가 아니고 해당 서버의 해당 
        // 날짜 파일만을 작업한다.
        //	pReadIni->LockIni();

        pReadIni->GetValue(server_info.szFullPath, _T("FILENAME"), _T(""), szValue, MAX_PATH);

        if ( 0 == szValue[0] )
        {
            server_info.ulOffset = 0;
            error_count++;
            if(error_count > 30)
            {
                //모든 로그 백업 완료 좀 쉬었다가 다시 체크 해보자 
                Sleep(1000*60*60*24); //하루 쉬자 
                if(!pLogReader->GetTodayDate(szCurrentDate))
                {        
                    return -1;
                }
            }

            continue;
        }
        else
        {
            server_info.ulOffset = pReadIni->GetValue(server_info.szFullPath, _T("OFFSET"), 0);
            error_count = 0;
        }

        //pLogReader->PrintfFirstReadFileName(&server_info);        
        
        pLogReader->DoReadLogFile(&server_info);

        // read ini 갱신 : 로그 데이터의 최종 파일명과 오프셋 기록
        pLogReader->SaveLastReadInfo(&server_info);

     
    } //while
    //////////////////////////////////////////////////////////////////////////

    pLogReader->ReturnSemaphore();

    InterlockedDecrement((LONG *)&(pLogReader->m_iRunningThread));

    return 1;
}

/**	----------------------------------------------------------------------------
	\brief	시간 지정 로그 수집 THREAD 

	\param	
	\return	
------------------------------------------------------------------------------*/
unsigned int __stdcall LogReader::TimeSpanReadLogThread(LPVOID lParam)
{
	LogReader  * pLogReader = (LogReader*)g_SUNLogMain.GetLogReader();
	IniControl * pReadIni   = pLogReader->GetReadIni();

	if (NULL == lParam)
	{
		_ASSERTE(!"TimeSpanReadLogThread");
		return -1;
	}

	InterlockedIncrement((LONG *)&(pLogReader->m_iRunningThread));


	pLogReader->GetSemaphore(); // 서버가 많아지면 추가..

	SERVERINFO sInfo;
	memcpy(&sInfo, lParam, sizeof(SERVERINFO));
	delete lParam, lParam = NULL;

	TCHAR szValue[MAX_PATH] = {0};

	// 타임 구간이 설정 되었을 경우, 무한 루프가 아니고 해당 서버의 해당 
	// 날짜 파일만을 작업한다.
//	pReadIni->LockIni();

#ifdef _NA_0_20110803_DIV_GAMELOG_FILE
    while (true)
    {
        pReadIni->GetValue(sInfo.szFullPath, _T("FILENAME"), _T(""), szValue, MAX_PATH);

        if ( 0 == szValue[0] )
        {
            sInfo.ulOffset = 0;
        }
        else
        {
            sInfo.ulOffset = pReadIni->GetValue(sInfo.szFullPath, _T("OFFSET"), 0);
        }

        pLogReader->DoReadLogFile(&sInfo);
        pLogReader->SaveLastReadInfo(&sInfo);
        
        if (!pLogReader->GetNextFilePath(&sInfo))
            break;
    }
#else
	pReadIni->GetValue(sInfo.szFullPath, _T("FILENAME"), _T(""), szValue, MAX_PATH);
	
	if ( 0 == szValue[0] )
	{
		sInfo.ulOffset = 0;
	}
	else
	{
		sInfo.ulOffset = pReadIni->GetValue(sInfo.szFullPath, _T("OFFSET"), 0);
	}

	//pLogReader->ConnectSession(&sInfo);
	pLogReader->DoReadLogFile(&sInfo);

//	pReadIni->UnLockIni();

	// read ini 갱신 : 로그 데이터의 최종 파일명과 오프셋 기록
	pLogReader->SaveLastReadInfo(&sInfo);
#endif

	pLogReader->ReturnSemaphore();


	InterlockedDecrement((LONG *)&(pLogReader->m_iRunningThread));

	//_endthreadex(1);

	return 1;
}


/**	----------------------------------------------------------------------------
	\brief	read ini에 최종 기록

	\param	
	\return	
------------------------------------------------------------------------------*/
void LogReader::SaveLastReadInfo(SERVERINFO * pstInfo)
{
	if (pstInfo == NULL)
	{
		_ASSERTE(!"SaveLastReadInfo");
		return;
	}

	TCHAR szValue[MAX_PATH]   = {0};
	TCHAR szDate[50]          = {0};

	m_pReadIni->SetValue(pstInfo->szFullPath, _T("WORLD_NUMBER")  , pstInfo->nWorldNo);
	m_pReadIni->SetValue(pstInfo->szFullPath, _T("PATH")    , pstInfo->szPath);
	m_pReadIni->SetValue(pstInfo->szFullPath, _T("FILENAME"), pstInfo->szFileName);
	m_pReadIni->SetValue(pstInfo->szFullPath, _T("OFFSET")  , pstInfo->ulOffset);

	switch( pstInfo->eLogState )
	{
	case LOGSTATE_LOADING :
		m_pReadIni->SetValue(pstInfo->szFullPath, _T("STATE"), _T("LOGSTATE_LOADING"));
		break;

	case LOGSTATE_END:
		m_pReadIni->SetValue(pstInfo->szFullPath, _T("STATE"), _T("LOGSTATE_END"));
		break;

	case LOGSTATE_REALTIME_SAME_OFFSET:
		m_pReadIni->SetValue(pstInfo->szFullPath, _T("STATE"), _T("LOGSTATE_REALTIME_SAME_OFFSET"));
		break;

	case LOGSTATE_INVALIED_OFFSET:
		m_pReadIni->SetValue(pstInfo->szFullPath, _T("STATE"), _T("LOGSTATE_INVALIED_OFFSET"));
		break;

	case LOGSTATE_INSERT_FAIL:
		m_pReadIni->SetValue(pstInfo->szFullPath, _T("STATE"), _T("LOGSTATE_INSERT_FAIL"));
		break;

	case LOGSTATE_NETWORK_LOGON_FAIL:
		m_pReadIni->SetValue(pstInfo->szFullPath, _T("STATE"), _T("LOGSTATE_NETWORK_LOGON_FAIL"));
#ifndef _DEBUG	
		ConnectSession(pstInfo);
#endif //_DEBUG
		Sleep(100);
		break;

	case LOGSTATE_INVALID_PATH:
		m_pReadIni->SetValue(pstInfo->szFullPath, _T("STATE"), _T("LOGSTATE_INVALID_PATH"));
		break;

	case LOGSTATE_ACCESS_DENY:
		m_pReadIni->SetValue(pstInfo->szFullPath, _T("STATE"), _T("LOGSTATE_ACCESS_DENY"));
		break;

	case LOGSTATE_NOT_EXIST_FILE:
		m_pReadIni->SetValue(pstInfo->szFullPath, _T("STATE"), _T("LOGSTATE_NOT_EXIST_FILE"));
		break;

	case LOGSTATE_FIND_OFFET:
		m_pReadIni->SetValue(pstInfo->szFullPath, _T("STATE"), _T("LOGSTATE_FIND_OFFET"));
		break;

    case LOGSTATE_IO_PENDING:
        m_pReadIni->SetValue(pstInfo->szFullPath, _T("STATE"), _T("LOGSTATE_IO_PENDING"));
        break;

	default: //LOGSTATE_UNKNOWN_OPEN_FAIL:
		m_pReadIni->SetValue(pstInfo->szFullPath, _T("STATE"), _T("LOGSTATE_UNKNOWN_FAIL"));
		break;
	}
}

void LogReader::GetLastReadInfo(SERVERINFO * pstInfo)
{
	if (pstInfo == NULL)
	{
		_ASSERTE(!"SaveLastReadInfo");
		return;
	}

	TCHAR szValue[MAX_PATH]   = {0};

	m_pReadIni->GetValue(pstInfo->szFullPath, _T("PATH"), _T(""), pstInfo->szPath, MAX_PATH);
	m_pReadIni->GetValue(pstInfo->szFullPath, _T("FILENAME"), _T(""), pstInfo->szFileName, MAX_PATH);
	m_pReadIni->GetValue(pstInfo->szFullPath, _T("OFFSET"), _T(""), szValue, MAX_PATH);

	pstInfo->ulOffset = _atoi64(szValue);
}

/**	----------------------------------------------------------------------------
	\brief	

	\param	
	\return	
------------------------------------------------------------------------------*/
void LogReader::GetServerTypeString(DWORD nType, TCHAR * szType)
{
	if (szType == NULL)
	{
		_ASSERTE(!"GetServerTypeString");
		return;
	}

	switch(nType)
	{
	case AGENT_SERVER:
		if (FAILED(StringCchCopy(szType, 24, _T("AGENT"))))
		{
			_ASSERTE(!"StringCchCopy");
			return;
		}
		break;
	case FIELD_SERVER:
		if (FAILED(StringCchCopy(szType, 24, _T("FIELD"))))
		{
			_ASSERTE(!"StringCchCopy");
			return;
		}
		break;
	case BATTLE_SERVER:
		if (FAILED(StringCchCopy(szType, 24, _T("BATTLE"))))
		{
			_ASSERTE(!"StringCchCopy");
			return;
		}
		break;
	case WORLD_SERVER:
		if (FAILED(StringCchCopy(szType, 24, _T("WORLD"))))
		{
			_ASSERTE(!"StringCchCopy");
			return;
		}
		break;
#ifdef _GUILD_LOG
	case GUILD_SERVER :
		if (FAILED(StringCchCopy(szType, 24, _T("GUILD"))))
		{
			_ASSERTE(!"StringCchCopy");
			return;
		}
		break;
#endif //_GUILD
	default :
		if (FAILED(StringCchCopy(szType, 24, _T("UNKNOWN"))))
		{
			_ASSERTE(!"StringCchCopy");
			return;
		}
	}
}

BOOL LogReader::IsPrevDay(TCHAR * szCurrentDate)
{
    if ( NULL == szCurrentDate )
    {
        _ASSERTE(!"IsPrevDay");
        return FALSE;
    }

    TCHAR szY[16] = {0};
    TCHAR szM[16] = {0};
    TCHAR szD[16] = {0};

    int nY = 0, nM = 0, nD = 0;
    BOOL bNextM = FALSE;
    BOOL bYY    = FALSE;

    szY[0] = szCurrentDate[0];
    szY[1] = szCurrentDate[1];
    szY[2] = szCurrentDate[2];
    szY[3] = szCurrentDate[3];
    szM[0] = szCurrentDate[4];
    szM[1] = szCurrentDate[5];
    szD[0] = szCurrentDate[6];
    szD[1] = szCurrentDate[7];

    nY = _ttoi(szY);
    nM = _ttoi(szM);
    nD = _ttoi(szD);

    time_t current;
    struct tm t;

    t.tm_year = nY - 1900;
    t.tm_mon = nM - 1;
    t.tm_mday = nD;
    t.tm_hour = 0;
    t.tm_sec = 0;
    t.tm_min = 0;

    current = mktime(&t);

    current -= (60*60*24);

    struct tm *yday = localtime(&current);

    if (FAILED(StringCchPrintf(szCurrentDate, 16, _T("%04d%02d%02d"), yday->tm_year + 1900, yday->tm_mon+1, yday->tm_mday)))
    {
        _ASSERTE(!"StringCchPrintf");
        return FALSE;
    }


    return TRUE;
}

/**	----------------------------------------------------------------------------
	\brief	

	\param	
	\return	
------------------------------------------------------------------------------*/
BOOL LogReader::IsNextDay(TCHAR * szCurrentDate)
{
	if ( NULL == szCurrentDate )
	{
		_ASSERTE(!"IsNextDay");
		return FALSE;
	}

	if ( 0 == m_szEndDate[0] )
		return FALSE;

	if ( 0 == _tcscmp(szCurrentDate, m_szEndDate) )
		return FALSE;

	// 0 1 2 3 4 5 6 7 - index
	// 2 0 0 7 1 2 3 1 - date

	TCHAR szY[16] = {0};
	TCHAR szM[16] = {0};
	TCHAR szD[16] = {0};

	int nY = 0, nM = 0, nD = 0;
	BOOL bNextM = FALSE;
	BOOL bYY    = FALSE;

	szY[0] = szCurrentDate[0];
	szY[1] = szCurrentDate[1];
	szY[2] = szCurrentDate[2];
	szY[3] = szCurrentDate[3];
	szM[0] = szCurrentDate[4];
	szM[1] = szCurrentDate[5];
	szD[0] = szCurrentDate[6];
	szD[1] = szCurrentDate[7];

	nY = _ttoi(szY);
	nM = _ttoi(szM);
	nD = _ttoi(szD);

	nD++;

	// find next day
	if ( 2 == nM )	// 2월
	{
		if ( 0 == nY % 400 )		// 윤년
			bYY = TRUE; 
		else if ( 0 == nY % 100 )	// 평년
			bYY = FALSE; 						
		else if ( 0 == nY % 4 )		// 윤년
			bYY = TRUE;

		if (bYY)
		{
			if ( 29 < nD )
			{
				nD = 1;
				bNextM = TRUE;
			}
		}
		else
		{
			if ( 28 < nD )
			{
				nD = 1;
				bNextM = TRUE;
			}
		}
	}
	else if ( 4 == nM ||  6 == nM ||  9 == nM ||  11 == nM )	// 30일인 달
	{
		if ( 30 < nD )
		{
			nD = 1;
			bNextM = TRUE;
		}		
	}
	else	// 31일인 달
	{
		if ( 31 < nD )
		{
			nD = 1;
			bNextM = TRUE;
		}		
	}

	if (bNextM)
	{
		nM++;

		if ( 12 < nM )
		{
			nM = 1;
			nY++;
		}
	}

	if (FAILED(StringCchPrintf(szCurrentDate, 16, _T("%04d%02d%02d"), nY, nM, nD)))
	{
		_ASSERTE(!"StringCchPrintf");
		return FALSE;
	}

	return TRUE;
}

