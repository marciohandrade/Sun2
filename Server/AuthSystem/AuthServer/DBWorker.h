#pragma once

#include <extreme/threading/thread.h>
#include <extreme/dbc/dbc.h>

class DBWorker : public eXtreme::Thread
{
public:
	DBWorker();
	~DBWorker();

	VOID			Initialize();
	VOID			Destroy();

protected:
	virtual	BOOL	Run();

private:
	static	VOID	CallbackErrorDBC( eXtreme::DBC* dbc,
		HRESULT result, _bstr_t msg, _bstr_t src, _bstr_t desc );
	static	VOID	CallbackErrorRecordSet( eXtreme::RecordSet* recordset,
		HRESULT result, _bstr_t msg, _bstr_t src, _bstr_t desc );

	BOOL			ConnectDB();

	eXtreme::DBC	m_DBConnection[DB_TYPE_MAX];
	TCHAR			m_szConnDBString[DB_TYPE_MAX][256];
	BOOL			m_EndQueryCompleted;
};