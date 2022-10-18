/*------------------------------------------------------------------------------
 * LogWriter.cpp
 *------------------------------------------------------------------------------
 * 읽어온 로그를 DB에 저장하는 기능을 수행하는 클래스이다.
 * 새로운 게임의 로그를 저장해야 하는 경우 
 * 이 클래스를 상속받아 해당 게임의 전용 라이터를 생성하면 된다.
 *------------------------------------------------------------------------------
 * All rights reserved by Jin Hye Jin (jiny7749@hotmail.com)
**----------------------------------------------------------------------------*/

#include "stdafx.h"
#include <io.h>
#include "DBHandler.h"
#include "LogCriticalSection.h"
#include "LogCollectorLog.h"
#include "LogMain.h"
#include "LogReader.h"
#include "LogWriter.h"

#define MAX_WRITE_COUNT	50
extern LogMain				  g_SUNLogMain;
extern LogCriticalSection	* g_pGlobalCS;
extern DBHandler			* g_pDBHandler;

/**	----------------------------------------------------------------------------
	\brief	

	\param	
	\return	
------------------------------------------------------------------------------*/
LogWriter::LogWriter()
:	m_bWriteThreadExitFlag(FALSE),
	m_iRunningThread(0),
	m_nWorldID(0),
	m_nChannelID(0)
{
	//m_pDBHandler = g_SUNLogMain.GetDBHandler();
	m_pDBHandler = g_pDBHandler;
}

/**	----------------------------------------------------------------------------
	\brief	

	\param	
	\return	
------------------------------------------------------------------------------*/
LogWriter::~LogWriter()
{
}

/**	----------------------------------------------------------------------------
	\brief	

	\param	
	\return	
------------------------------------------------------------------------------*/
void LogWriter::StartWriteLog(int iMaxThreadNum)
{
	m_nChannelID = g_SUNLogMain.m_nChannelID;
	m_nWorldID = g_SUNLogMain.m_nWorldID;

	for ( int i = 0; i < iMaxThreadNum; i++ )
	{
		DWORD dwThreadID = 0;
		HANDLE hWriteThread = (HANDLE)_beginthreadex(NULL, 
													0, 
													WriteLogThread, 
													this, 
													0, 
													(unsigned*)&dwThreadID);
	}
}

/**	----------------------------------------------------------------------------
	\brief	

	\param	
	\return	
------------------------------------------------------------------------------*/
void LogWriter::StartWriteFailLog()
{
	DWORD dwThreadID = 0;
	HANDLE hWriteThread = (HANDLE)_beginthreadex(NULL, 
												0, 
												WriteFailLogThread, 
												this, 
												0, 
												(unsigned*)&dwThreadID);
}

/**	----------------------------------------------------------------------------
	\brief	

	\param	
	\return	
------------------------------------------------------------------------------*/
void LogWriter::Stop()
{
	m_bWriteThreadExitFlag = TRUE;
}

/**	----------------------------------------------------------------------------
	\brief	

	\param	
	\return	
------------------------------------------------------------------------------*/
unsigned int __stdcall LogWriter::WriteLogThread(LPVOID lParam)
{
	if (NULL == lParam)
	{
		_ASSERTE(!"WriteLogThread");
		return -1;
	}

	LogWriter * pLogWriter = (LogWriter*)lParam;
	InterlockedIncrement((LONG *)&(pLogWriter->m_iRunningThread));

	LogReader * pLogReader = (LogReader*)g_SUNLogMain.GetLogReader();

	//pLogWriter->m_iRunningThread++;

	while(TRUE)
	{
		// LogReader의 m_LogDataList에서 LogData를 가져온 후
		// 리스트에서 그 엘레먼트를 제거
		// 한번에 MAX_WRITE_COUNT 만큼 꺼내온다.

		LPVOID pLogData[MAX_WRITE_COUNT];
	
		ZeroMemory(pLogData, sizeof(pLogData));
		/*
		for (int i = 0; i < MAX_WRITE_COUNT; i++)
		{
			pLogData[i] = NULL;
		}
		*/
		int iFound = 0;

		g_pGlobalCS->Enter();

		int cnt = (int)pLogReader->m_LogDataList.size();

		std::list<LPVOID>::iterator iter = pLogReader->m_LogDataList.begin();

		for (int i = 0; i < cnt ; i++)
		{
			pLogData[i] = (*iter);

			iter = pLogReader->m_LogDataList.erase(iter);
			
			iFound++;

			if( iFound >= MAX_WRITE_COUNT || 
				pLogWriter->m_bWriteThreadExitFlag )
			{
				break;
			}

		}

		g_pGlobalCS->Leave();

		if ( pLogWriter->m_bWriteThreadExitFlag && 0 == iFound)
		{
			break;
		}
		
		if ( 0 == iFound )
		{
			continue;
		}

		for (int i = 0; i < iFound; i++)
		{
			pLogWriter->DoWriteLogDB(pLogData[i]);
			delete pLogData[i], pLogData[i] = NULL;
		}

	}

	InterlockedDecrement((LONG *)&(pLogWriter->m_iRunningThread));
	//pLogWriter->m_iRunningThread--;
	//_endthreadex(1);

	return 1;
}

/**	----------------------------------------------------------------------------
	\brief	

	\param	
	\return	
------------------------------------------------------------------------------*/
unsigned int __stdcall LogWriter::WriteFailLogThread(LPVOID lParam)
{
	if (NULL == lParam)
	{
		_ASSERTE(!"WriteFailLogThread");
		return -1;
	}

	LogWriter * pLogWriter = (LogWriter*)lParam;
	
	InterlockedIncrement((LONG *)&(pLogWriter->m_iRunningThread));
	//pLogWriter->m_iRunningThread++;

	TCHAR szDir[MAX_PATH] = {0};
	if (FAILED(StringCchPrintf(szDir, sizeof(szDir), _T("%s\\FAILLOG"),
								g_szCurrentPath)))
	{
		_ASSERTE(!"StringCchPrintf");
	}

	while(TRUE)
	{
		if ( pLogWriter->m_bWriteThreadExitFlag )
		{
			break;
		}

		if ( -1 == _taccess(szDir, 0))
		{
			Sleep(4000);
			continue;
		}

		pLogWriter->DoWriteFailLogDB();
#ifdef _KOREA
		Sleep(1000);
#endif
	}

	InterlockedDecrement((LONG *)&(pLogWriter->m_iRunningThread));
	//pLogWriter->m_iRunningThread--;

	//_endthreadex(0);

	return 0;
}

/**	----------------------------------------------------------------------------
	\brief	

	\param	
	\return	
------------------------------------------------------------------------------*/
void LogWriter::DoWriteFailLogDB()
{
#ifdef _UNICODE
	struct _wfinddata_t c_file;
#else
	struct _finddata_t c_file;
#endif //_UNICODE
	intptr_t hFile;
	TCHAR szTxt[MAX_PATH] = {0};
	TCHAR szBuf[MAX_PATH] = {0};

	if (FAILED(StringCchPrintf(szTxt, sizeof(szTxt), _T("%s\\FAIL_LOG\\*.txt"),
								g_szCurrentPath)))
	{
		_ASSERTE(!"StringCchPrintf");
	}

	hFile = _tfindfirst(szTxt, &c_file);

	while ( INVALID_HANDLE_VALUE != (HANDLE)hFile )
	{
		if ( m_bWriteThreadExitFlag )
		{
			break;
		}

		if (FAILED(StringCchPrintf(szTxt, sizeof(szTxt), _T("%s\\FAIL_LOG\\%s"),
									g_szCurrentPath, c_file.name)))
		{
			_ASSERTE(!"StringCchPrintf");
		}

		FILE * fp = _tfopen(szTxt, _T("r"));

		if ( NULL != fp )
		{
			while( _fgetts(szBuf, SQL_MAX_MESSAGE_LENGTH, fp) )
			{
				if(!m_pDBHandler->DoSQL(szBuf))
				{
					LogCollectorLog cLog1(_T("FAIL_LOG_SE"), _T("FAIL_LOG"));
					cLog1.AddSQL(szBuf);

					LogCollectorLog cLog2(_T("FAIL_LOG"));
					cLog2.AddLog(_T("FileName : %s, SQL COMMAND : %s\n"), 
									c_file.name, szBuf);
				}
			}

			fclose(fp);
			DeleteFile(szTxt);

			TCHAR * ptr = _tcsrchr(szTxt, _T('.'));
			if ( NULL != ptr )
			{
				*ptr = 0;
				if (FAILED(StringCchCat(szTxt, sizeof(szTxt), _T(".wzl"))))
				{
					_ASSERTE(!"StringCchCat fail");
				}
				DeleteFile(szTxt);
			}
		}
		else
		{
			DWORD nErr = ::GetLastError();
			LogCollectorLog cLog2(_T("FAIL_LOG"));
			cLog2.AddLog(_T("Found FIle. But File Read Fail(ERROR). ErrCode = %d"),
							nErr);
		}

		 _tfindnext(hFile, &c_file);
	}
}

/**	----------------------------------------------------------------------------
	\brief	

	\param	
	\return	
------------------------------------------------------------------------------*/
void LogWriter::AddMark(TCHAR * pString)
{
	TCHAR szTemp[SQL_MAX_MESSAGE_LENGTH] = {0};

	if (FAILED(StringCchPrintf(szTemp, sizeof(szTemp),
								_T("'%s '"),
								pString)) )
	{
		_ASSERTE(!"StringCchPrintf");
	}

	int len = 0;

	if ( FAILED(StringCchLength(szTemp, SQL_MAX_MESSAGE_LENGTH, (size_t *)&len )) )
	{
		_ASSERTE(!"StringCchLength");
	}

	szTemp[len - 1] = _T('\'');

	if (FAILED(StringCchCopy(pString, SQL_MAX_MESSAGE_LENGTH, szTemp)) )
	{
		_ASSERTE(!"StringCchCopy");
	}
}
