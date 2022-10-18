/*------------------------------------------------------------------------------
 * DBHandler.cpp
 *------------------------------------------------------------------------------
 * DB를 사용하기 위해 DB에 연결하고,
 * 라이트 쓰레드 수 만큼의 핸들을 준비해 놓는다.
 *
 * 실제 DB에 쓰는 함수는
 * 각 게임의 data type에 따라 이 클래스를 상속받아 작성해야 한다.
 *------------------------------------------------------------------------------
 * All rights reserved by Jin Hye Jin (jiny7749@hotmail.com)
**----------------------------------------------------------------------------*/

#include "stdafx.h"
#include "DB.h"
#include "DBHandler.h"
#include "LogCriticalSection.h"
#include "LogCollectorLog.h"

/**	----------------------------------------------------------------------------
	\brief	

	\param	
	\return	
------------------------------------------------------------------------------*/
DBHandler::DBHandler()
{
	m_pDBPool = NULL;
}

/**	----------------------------------------------------------------------------
	\brief	

	\param	
	\return	
------------------------------------------------------------------------------*/
DBHandler::~DBHandler()
{

}

/**	----------------------------------------------------------------------------
	\brief	

	\param	
	\return	
------------------------------------------------------------------------------*/
int DBHandler::DBConnect()
{
	return m_pDBPool->DBConnect();
}

/**	----------------------------------------------------------------------------
	\brief	

	\param	
	\return	
------------------------------------------------------------------------------*/
BOOL DBHandler::DoSQL(TCHAR * pszSQL)
{
	SQLHSTMT    hstmt = SQL_NULL_HSTMT;
	SQLHDBC     hdbc = SQL_NULL_HDBC;
	SQLRETURN   retcode;
	pDBInfo     pdbConnection = NULL;	
	LogCollectorLog cSQLLog(_T("SQL_FAIL_LOG"), _T("SQL_FAIL_LOG"));

#ifdef _DEBUG
	LogCollectorLog CTestLog(_T("SQL_TEST_LOG"), _T("SQL_TEST_LOG"));
	CTestLog.AddLog(pszSQL);
#endif //_DEBUG

	pdbConnection = m_pDBPool->GetDBHandle();
	if ( pdbConnection == NULL ) 
	{
		cSQLLog.AddLog(_T("Error DBConnection is null\n"));
		return FALSE;
	}

	hdbc  = pdbConnection->hSQLConnection;		
	hstmt = AllocStatement(hdbc);

	if (hstmt == SQL_NULL_HSTMT)
	{
		cSQLLog.AddLog(_T("Error SQL_NULL_HSTMT\n"));
		m_pDBPool->ReleaseDBHandle(pdbConnection);
		return FALSE;
	}
/*
	if (CheckContext(pszSQL))
	{
		FreeStatement(hstmt);
		m_pDBPool->ReleaseDBHandle(pdbConnection);
		return FALSE;
	}
*/
	retcode = SQLExecDirect(hstmt, (SQLTCHAR*)pszSQL, SQL_NTS);

	if ((retcode != SQL_SUCCESS) && 
		(retcode != SQL_SUCCESS_WITH_INFO) &&
		(retcode != SQL_NO_DATA) &&
		(retcode != SQL_STILL_EXECUTING) &&
		(retcode != SQL_NEED_DATA))
	{
		SQLTCHAR       SqlState[6] = {0};
		SQLTCHAR       Msg[SQL_MAX_MESSAGE_LENGTH] = {0};
		SQLINTEGER    NativeError;
		SQLSMALLINT   i = 1;
		SQLSMALLINT   MsgLen = 0;

		SQLGetDiagRec(SQL_HANDLE_STMT, hstmt, i, SqlState, &NativeError,
						Msg, sizeof(Msg), &MsgLen);

		cSQLLog.AddLog(_T("DoSQL() SQL Error, SQLSTATE ; %d, Msg  : %s"), SqlState, Msg);

		FreeStatement(hstmt);
		m_pDBPool->ReleaseDBHandle(pdbConnection);

		return FALSE;
	}

	FreeStatement(hstmt);
	m_pDBPool->ReleaseDBHandle(pdbConnection);
	
	return TRUE;
}

/**	----------------------------------------------------------------------------
	\brief	

	\param	
	\return	
------------------------------------------------------------------------------*/
BOOL DBHandler::CheckContext(TCHAR * szContext)
{
	if (NULL == szContext)
	{
		_ASSERTE(!"CheckContext");
		return FALSE;
	}

	TCHAR szBuffer[SQL_MAX_MESSAGE_LENGTH] = {0};
	size_t len = 0;

	if (FAILED(StringCchCopy(szBuffer, sizeof(szBuffer), szContext)))
	{
		_ASSERTE(!"StringCchCopy");
		return FALSE;
	}

	if ( FAILED(StringCchLength(szBuffer, sizeof(szBuffer), &len )) )
	{
		_ASSERTE(!"StringCchLength failed");
		return FALSE;
	}

	for (int i = 0; i < (int)len; i++)
	{
		TCHAR cTmp = szBuffer[i];
		if (cTmp == _T('\''))
		{
			cTmp = _T('\"');
		}
		szBuffer[i] = cTmp;
	}

	if (FAILED(StringCchCopy(szContext, sizeof(szContext), szBuffer)))
	{
		_ASSERTE(!"StringCchCopy");
		return FALSE;
	}

	return TRUE;
}

/**	----------------------------------------------------------------------------
	\brief	

	\param	
	\return	
------------------------------------------------------------------------------*/
void DBHandler::InsertError(TCHAR * szError)
{
	if (NULL == szError)
	{
		_ASSERTE(!"InsertError");
		return;
	}

	SYSTEMTIME stTime;
	GetLocalTime(&stTime);

	TCHAR szSQL[SQL_MAX_MESSAGE_LENGTH] = {0};
	TCHAR szTime[100] = {0};

	if (FAILED(StringCchPrintf(szTime, sizeof(szTime),
								_T("%02d-%02d-%02d %02d:%02d:%02d.%03d"),
								stTime.wYear, 
								stTime.wMonth, 
								stTime.wDay, 
								stTime.wHour,
								stTime.wMinute,
								stTime.wSecond,
								stTime.wMilliseconds)))
	{
		_ASSERTE(!"StringCchPrintf");
	}

	if (FAILED(StringCchPrintf(szSQL, sizeof(szSQL),
								_T("exec S_ErrorInfoInsert N'%s', N'%s'"),
								szTime,
								szError)))
	{
		_ASSERTE(!"StringCchPrintf");
	}

	DoSQL(szSQL);
}

/**	----------------------------------------------------------------------------
	\brief	

	\param	
	\return	
------------------------------------------------------------------------------*/
BOOL DBHandler::GetSQLControlor(pDBInfo    dbConnection, 
								SQLHDBC  * hdbc, 
								SQLHSTMT * hstmt, 
								LPCTSTR    szSQL)
{
	SQLHDBC   tmphdbc = SQL_NULL_HDBC;
	SQLHSTMT  tmphstmt = SQL_NULL_HSTMT;
	SQLRETURN retcode;

	dbConnection = m_pDBPool->GetDBHandle();
	if ( dbConnection == NULL )
	{
		return FALSE;
	}

	tmphdbc  = dbConnection->hSQLConnection;
	tmphstmt = AllocStatement(tmphdbc);	

	if (tmphstmt == SQL_NULL_HSTMT)
	{
		m_pDBPool->ReleaseDBHandle(dbConnection);
		return FALSE;
	}

	retcode = SQLPrepare(tmphstmt, (SQLTCHAR*)szSQL, SQL_NTS);

	if (retcode != SQL_SUCCESS && 
		retcode != SQL_SUCCESS_WITH_INFO &&
		retcode != SQL_NO_DATA &&
		retcode != SQL_STILL_EXECUTING &&
		retcode != SQL_NEED_DATA)
	{
		FreeStatement(tmphstmt);
		m_pDBPool->ReleaseDBHandle(dbConnection);

		SQLTCHAR       SqlState[6] = {0};
		SQLTCHAR       Msg[SQL_MAX_MESSAGE_LENGTH] = {0};
		SQLINTEGER    NativeError;
		SQLSMALLINT   i = 1;
		SQLSMALLINT   MsgLen = 0;

		SQLGetDiagRec(SQL_HANDLE_STMT, tmphstmt, i, SqlState, &NativeError,
						Msg, sizeof(Msg), &MsgLen);
		return FALSE;
	}

	*hdbc  = tmphdbc;
	*hstmt = tmphstmt;

	return FALSE;
}
