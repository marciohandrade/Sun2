/*------------------------------------------------------------------------------
 * DB.h
 *------------------------------------------------------------------------------
 * 
 *------------------------------------------------------------------------------
 * All rights reserved by Jin Hye Jin (jiny7749@hotmail.com)
**----------------------------------------------------------------------------*/

#ifndef _DB_POOL_H_
#define _DB_POOL_H_

#include "stdafx.h"

class LogCriticalSection;

typedef struct _DBInfo
{
	int			id;
	BOOL		fUse;
	SQLHDBC		hSQLConnection;

} DBInfo, *pDBInfo;

SQLHSTMT AllocStatement(SQLHDBC & hdbc);
void     FreeStatement (SQLHSTMT hstmt);
BOOL     AllocEnv(SQLHENV & henv);
SQLHDBC  ConnectDSN(SQLHENV & henv, TCHAR * szDSN, TCHAR * szUID, TCHAR * szAuthStr);

class DatabasePool
{
public:
	DatabasePool(int max, TCHAR * dsn, TCHAR * id, TCHAR * passwd,  TCHAR * szName = NULL);
	~DatabasePool();
	int     DBConnect();
	pDBInfo GetDBHandle();
	void    ReleaseDBHandle(pDBInfo pinfo);
	BOOL    IsConnected() { return m_bConnected; };

private:
	BOOL                 m_bConnected;
	int                  m_nMaxConnection;
	TCHAR                 m_szDsn[MAX_DB_PW];
	TCHAR                 m_szID[MAX_DB_PW];
	TCHAR                 m_szPasswd[MAX_DB_PW];
	SQLHENV              m_henv;
	pDBInfo              m_pDBInfo;
	HANDLE               m_hSemaphore;
	LogCriticalSection * m_pDBCS;
};

#endif //_DB_POOL_H_