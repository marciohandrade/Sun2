#ifndef __ISQLLIB_H__
#define __ISQLLIB_H__

//=======================================================================================================================
/// MS SQL Server DB-Library�� ����ϱ� ���� Wrapping�� SQL LIB ���� Class
/**
	@author
			Kim Min Wook < taiyo@webzen.co.kr >	
	@since
			2004. 10. 20
	@remarks
			- MS SQL DB-Library�� ������ Ŭ����
	@note
			- Init(), Connect(), Execute(), Release()������ ����Ѵ�.
			- MS SQL 6.5 �������ķ� ������ Features�� ����� �� ����.
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