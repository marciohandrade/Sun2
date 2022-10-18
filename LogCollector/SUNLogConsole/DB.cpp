/*------------------------------------------------------------------------------
 * DB.cpp
 *------------------------------------------------------------------------------
 * 
 *------------------------------------------------------------------------------
 * All rights reserved by Jin Hye Jin (jiny7749@hotmail.com)
**----------------------------------------------------------------------------*/

#include "stdafx.h"
#include "DB.h"
#include "LogCriticalSection.h"

/**	----------------------------------------------------------------------------
	\brief	

	\param	
	\return	
------------------------------------------------------------------------------*/
DatabasePool::DatabasePool(int max, TCHAR * dsn, TCHAR * id, TCHAR * passwd, TCHAR * szName)
:m_bConnected(FALSE),
 m_nMaxConnection(max),
 m_henv(SQL_NULL_HENV),
 m_hSemaphore(NULL)
{
	if (NULL == dsn ||
		NULL == id ||
		NULL == passwd)
	{
		_ASSERTE(!"DatabasePool");
	}

	m_pDBCS   = new LogCriticalSection;
	m_pDBInfo = new DBInfo[max]; // DB 정보를 담을 구조체(스레드 개수와 같게)
	memset(m_pDBInfo, 0, sizeof(DBInfo) * max);

	if (FAILED(StringCchCopy(m_szDsn, sizeof(m_szDsn), dsn)))
	{
		_ASSERTE(!"StringCchCopy");
	}

	if (FAILED(StringCchCopy(m_szID, sizeof(m_szID), id)))
	{
		_ASSERTE(!"StringCchCopy");
	}

	if (FAILED(StringCchCopy(m_szPasswd, sizeof(m_szPasswd), passwd)))
	{
		_ASSERTE(!"StringCchCopy");
	}

	if(szName)
	{
		/*
		TCHAR szCurrentPath[MAX_PATH];
		TCHAR szRealName[MAX_PATH];
		GetModuleFileName( GetModuleHandle( NULL ), szCurrentPath, MAX_PATH );
		*(_tcsrchr( szCurrentPath, _T('\\') ) ) = 0;

		StringCchPrintf(szRealName, sizeof(szRealName), _T("%s\\%s"), szCurrentPath, szName);
*/
		// create semaphore for resource locking
		m_hSemaphore = CreateSemaphore( NULL,	// No security attrib.
			max,	// initial
			max,	// max
			szName);
	}
	else
	{
		TCHAR szTmpName[MAX_PATH] = _T("DBHandle");
		TCHAR szSemaphoreName[MAX_PATH] = {0};

		srand((UINT)time(NULL));
		
		INT nIdx = rand() % 10000;

		StringCchPrintf(szSemaphoreName, MAX_PATH, _T("%s%d"), szTmpName, nIdx);

		// create semaphore for resource locking
		m_hSemaphore = CreateSemaphore( NULL,	// No security attrib.
			max,	// initial
			max,	// max
			szSemaphoreName);

		while(ERROR_ALREADY_EXISTS == ::GetLastError())
		{
			srand((UINT)time(NULL));

			nIdx = rand() % 20000;

			StringCchPrintf(szSemaphoreName, MAX_PATH, _T("%s%d"), szTmpName, nIdx);

			// create semaphore for resource locking
			m_hSemaphore = CreateSemaphore( NULL,	// No security attrib.
				max,	// initial
				max,	// max
				szSemaphoreName);
		}
	}


	TRACE("Create Semaphore");
}

/**	----------------------------------------------------------------------------
	\brief	

	\param	
	\return	
------------------------------------------------------------------------------*/
DatabasePool::~DatabasePool()
{
	for (int i = 0; i < m_nMaxConnection; i++)
	{
		if (m_pDBInfo[i].hSQLConnection)
		{
			SQLDisconnect(m_pDBInfo[i].hSQLConnection);
			SQLFreeHandle(SQL_HANDLE_DBC, m_pDBInfo[i].hSQLConnection);
		}
	}

		if (NULL != m_pDBCS)
	{
		delete m_pDBCS, m_pDBCS = NULL;
	}
	if (m_pDBInfo)
	{
		delete [] m_pDBInfo;
		m_pDBInfo = NULL;
	}

	SQLFreeHandle(SQL_HANDLE_ENV, m_henv);
	CloseHandle(m_hSemaphore);
	m_hSemaphore = NULL;
}

/**	----------------------------------------------------------------------------
	\brief	

	\param	
	\return	
------------------------------------------------------------------------------*/
int DatabasePool::DBConnect()
{
	// make connection to database
	if ( !AllocEnv(m_henv) )
	{
		return -1;
	}

	for (int i = 0; i < m_nMaxConnection; i++) 
	{
		// 할당한 DB정보 구조체에 순서(id)와 DB핸들을 저장한다.
		m_pDBInfo[i].id = i;
		m_pDBInfo[i].fUse = FALSE;				// set flag to "NOT USE"
		m_pDBInfo[i].hSQLConnection = ConnectDSN(m_henv, 
												(TCHAR*)m_szDsn, 
												(TCHAR*)m_szID, 
												(TCHAR*)m_szPasswd);

		if (m_pDBInfo[i].hSQLConnection == INVALID_HANDLE_VALUE)
		{
			m_bConnected = FALSE;
		}
		else
		{
			m_bConnected = TRUE;
		}
	}
	return 0;
}

/**	----------------------------------------------------------------------------
	\brief	

	\param	
	\return	
------------------------------------------------------------------------------*/
BOOL AllocEnv(SQLHENV & henv)
{
	// vars of ODBC connectivity
	SQLRETURN retcode;

    // Allocate the ODBC Environment and save handle.
    retcode = SQLAllocHandle (SQL_HANDLE_ENV, NULL, &henv);
	if ( !(retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) )
	{
	   // Connect failed, call SQLGetDiagRec for errors.
		return FALSE;
    }
    // Let ODBC know this is an ODBC 3.0 application.
    retcode = SQLSetEnvAttr(henv, SQL_ATTR_ODBC_VERSION,
                            (SQLPOINTER)SQL_OV_ODBC3,
                            SQL_IS_INTEGER);

	if ( !(retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) )
	{
	   // Connect failed, call SQLGetDiagRec for errors.
		return FALSE;
    }
	return TRUE;
}

/**	----------------------------------------------------------------------------
	\brief	

	\param	
	\return	
------------------------------------------------------------------------------*/
SQLHDBC ConnectDSN(SQLHENV & henv, TCHAR * szDSN, TCHAR * szUID, TCHAR * szAuthStr)
{
	SQLHDBC connection = SQL_NULL_HDBC;
	
	int retcode;

    // Allocate an ODBC connection handle and connect.
    retcode = SQLAllocHandle(SQL_HANDLE_DBC, henv, &connection);
	if ( !(retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) )
	{
	   // Connect failed, call SQLGetDiagRec for errors.
		return connection;
    }
 
	// connect to ODBC using supplied parameters
    retcode = SQLConnect(connection, 
							(SQLTCHAR*)szDSN, 
							(SWORD)_tcslen((TCHAR*)&szDSN[0]),
							(SQLTCHAR*)szUID, 
							(SWORD)_tcslen((TCHAR*)&szUID[0]),
							(SQLTCHAR*)szAuthStr, 
							(SWORD)_tcslen((TCHAR*)&szAuthStr[0]));

    if ( !(retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) )
	{
	   // Connect failed, call SQLGetDiagRec for errors.
		return connection;
    }
	return connection;
}

/**	----------------------------------------------------------------------------
	\brief	

	\param	
	\return	
------------------------------------------------------------------------------*/
pDBInfo DatabasePool::GetDBHandle()
{
	DWORD    dwWaitResult = 0;
	pDBInfo pinfo = NULL;

	// wait while connections are available
	dwWaitResult = WaitForSingleObject(m_hSemaphore, INFINITE);

	//DWORD dwErr = ::GetLastError();

	switch ( dwWaitResult ) 
	{
	case WAIT_OBJECT_0:
		// OK. there's available connection.
		__try 
		{
			m_pDBCS->Enter();
			// find unused connection
			for (int i = 0; i < m_nMaxConnection; i++)
			{
				if (!m_pDBInfo[i].fUse)
				{
					m_pDBInfo[i].fUse = TRUE;
					pinfo = &m_pDBInfo[i];
					break;
				}
			}

		}
		__finally
		{
			m_pDBCS->Leave();
		}
		break;

	case WAIT_TIMEOUT:
		// Cannot obtain connection
		return NULL;
	}


	return pinfo;
}

/**	----------------------------------------------------------------------------
	\brief	

	\param	
	\return	
------------------------------------------------------------------------------*/
void DatabasePool::ReleaseDBHandle(pDBInfo pinfo)
{
	__try 
	{
		m_pDBCS->Enter();

		pinfo->fUse = FALSE;
		long previous;
		int err = ReleaseSemaphore(m_hSemaphore,  // handle to semaphore
									1,            // increase count by one
									&previous);

		if (err == FALSE)
		{
			OutputDebugString(_T("ReleaseDBHandle Function FAIL!!"));
			return;
		}
	}
	__finally
	{
		m_pDBCS->Leave();
	}
}

/**	----------------------------------------------------------------------------
	\brief	

	\param	
	\return	
------------------------------------------------------------------------------*/
SQLHSTMT AllocStatement(SQLHDBC & hdbc)
{
	// memory allocation for DB handle
	int retcode;
	SQLHSTMT hstmt;
	
	hstmt = SQL_NULL_HSTMT;

	retcode = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt); 
	if (!(retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) )
	{
		return hstmt;
    }

	return hstmt;
}

/**	----------------------------------------------------------------------------
	\brief	

	\param	
	\return	
------------------------------------------------------------------------------*/
void FreeStatement (SQLHSTMT hstmt) 
{
	SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
}
