#ifndef __IOLEDB_H__
#define __IOLEDB_H__

#pragma once
#include "idbcinterface.h"
#include <msdasc.h>


class QueryResult;
//=======================================================================================================================
/// OLE DB를 사용하기 위해 Wrapping한 OLE DB 구현 Class
/**
	@author
			Kim Min Wook < taiyo@webzen.co.kr >	
	@since
			2004. 10. 20
	@remarks
			- OLE DB를 구현한 클래스 ( Pure COM OLE API 사용 )
			- 제공자 : OLEDB provider for SQL Server으로 선택되어 있다.
			- 다른 Database에서 사용하기 위해서는 Database에 맞는 제공자를 다운로드받아 설치하고 클래스ID를 설정한다.
	@note
			- Init(), Connect(), Execute(), Release()순으로 사용한다.
*/
//=======================================================================================================================

class IOLEDB :	public IDBCInterface
{
public:
	IOLEDB(void);
	virtual ~IOLEDB(void);

	virtual VOID			Release();
	virtual BOOL			Init( char * pszIPAddress, char * pszDatabaseName, char * pszUserName, char * pszUserPassword, fnCallBackMessage fnErrorMessage );
	virtual BOOL			Connect();
	virtual VOID			Disconnect();
	virtual BOOL			Execute( Query * pQuery );

private:
	BOOL					SQLError( HRESULT hr );
	VOID					zDisplayProvider();			///< helper function

	IDataInitialize *		m_pIDataInitialize;
	IDBInitialize *			m_pIDBInitialize;
	IDBCreateSession *		m_pIDBCreateSession;
	IDBCreateCommand *		m_pIDBCreateCommand;
	ICommand *				m_pICommand;
	ICommandText *			m_pICommandText;
	IRowset *				m_pIRowset;
	IColumnsInfo *			m_pIColumnsInfo;
	IAccessor *				m_pIAccessor;
	HACCESSOR				m_hAccessor;

	HROW					m_hRow;
	HROW*					m_phRow;
	DBBINDING				m_pDBBindings[MAX_COL_NUM];
	DBCOLUMNINFO *			m_pDBColumnInfo;

};

#endif // __IOLEDB_H__