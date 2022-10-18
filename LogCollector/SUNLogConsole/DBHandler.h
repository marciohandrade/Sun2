/*------------------------------------------------------------------------------
 * DBHandler.h
 *------------------------------------------------------------------------------
 * 
 *------------------------------------------------------------------------------
 * All rights reserved by Jin Hye Jin (jiny7749@hotmail.com)
**----------------------------------------------------------------------------*/

#ifndef _DB_HANDLER_H_
#define _DB_HANDLER_H_

#include "DB.h"

class LogCriticalSection;
class DatabasePool;

class DBHandler
{
public:
	DBHandler();
	~DBHandler();
	int  DBConnect();
	BOOL DoSQL(TCHAR * pszSQL);
	void InsertError(TCHAR * szError);

protected:
	DatabasePool       * m_pDBPool;
	LogCriticalSection * m_pDBCS;

	BOOL GetSQLControlor(pDBInfo    dbConnection, 
						 SQLHDBC  * hdbc, 
						 SQLHSTMT * hstmt, 
						 LPCTSTR    szSQL);
	BOOL CheckContext(TCHAR * szContext);
};

#endif //_DB_HANDLER_H_