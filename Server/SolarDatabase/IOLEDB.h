#ifndef __IOLEDB_H__
#define __IOLEDB_H__

#pragma once
#include "idbcinterface.h"
#include <msdasc.h>


class QueryResult;
//=======================================================================================================================
/// OLE DB�� ����ϱ� ���� Wrapping�� OLE DB ���� Class
/**
	@author
			Kim Min Wook < taiyo@webzen.co.kr >	
	@since
			2004. 10. 20
	@remarks
			- OLE DB�� ������ Ŭ���� ( Pure COM OLE API ��� )
			- ������ : OLEDB provider for SQL Server���� ���õǾ� �ִ�.
			- �ٸ� Database���� ����ϱ� ���ؼ��� Database�� �´� �����ڸ� �ٿ�ε�޾� ��ġ�ϰ� Ŭ����ID�� �����Ѵ�.
	@note
			- Init(), Connect(), Execute(), Release()������ ����Ѵ�.
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