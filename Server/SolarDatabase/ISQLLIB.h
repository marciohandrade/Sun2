#ifndef __ISQLLIB_H__
#define __ISQLLIB_H__

//=======================================================================================================================
/// MS SQL Server DB-Library를 사용하기 위해 Wrapping한 SQL LIB 구현 Class
/**
	@author
			Kim Min Wook < taiyo@webzen.co.kr >	
	@since
			2004. 10. 20
	@remarks
			- MS SQL DB-Library를 구현한 클래스
	@note
			- Init(), Connect(), Execute(), Release()순으로 사용한다.
			- MS SQL 6.5 버전이후로 제공된 Features은 사용할 수 없다.
*/
//=======================================================================================================================

#pragma once

#include "idbcinterface.h"
#include <winscard.h>

#ifdef __cplusplus
extern "C" {
#endif
#define	DBNTWIN32
#include ".\sql_include\sqlfront.h"
#include ".\sql_include\sqldb.h"
#ifdef __cplusplus
}
#endif


class ISQLLIB :	public IDBCInterface
{
public:
	ISQLLIB(void);
	virtual ~ISQLLIB(void);

	virtual VOID			Release();
	virtual BOOL			Init( char * pszIPAddress, char * pszDatabaseName, char * pszUserName, char * pszUserPassword, fnCallBackMessage fnErrorMessage );
	virtual BOOL			Connect();
	virtual VOID			Disconnect();
	virtual BOOL			Execute( Query * pQuery );
	virtual BOOL			ExecuteLargeData( Query * pQuery );
private:
	LOGINREC *				m_pLoginRec;
	DBPROCESS *				m_pDBProcess;
};


#endif // __ISQLLIB_H__