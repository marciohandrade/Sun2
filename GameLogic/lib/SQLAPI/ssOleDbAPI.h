// ssOleDbAPI.h
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SSOLEDBAPI_H__11EAA72A_EFF5_4944_8B1F_44973D30EF73__INCLUDED_)
#define AFX_SSOLEDBAPI_H__11EAA72A_EFF5_4944_8B1F_44973D30EF73__INCLUDED_

#include "SQLAPI.h"

// API header(s)
#ifdef LINUX
	#include "./ss_linux/sqloledb.h"
	#include <oledberr.h>
#endif
#ifdef WIN32
	#include "./ss_win32/sqloledb.h"
	#include <oledberr.h>
#endif

extern const GUID SA_CLSID_SQLOLEDB;
extern const GUID SA_DBPROPSET_DATASOURCEINFO;
extern const DBID SA_DB_NULLID;
extern const GUID SA_DBPROPSET_DBINIT;
extern const GUID SA_DBPROPSET_SQLSERVERDBINIT;
extern const GUID SA_DBPROPSET_ROWSET;
extern const IID SA_IID_IAccessor;
extern const GUID SA_DBSCHEMA_PROCEDURE_PARAMETERS;
extern const GUID SA_DBGUID_MSSQLXML;

extern void AddSSOleDbSupport();
extern void ReleaseSSOleDbSupport();

// API declarations
class ssOleDbAPI : public saAPI
{
public:
	ssOleDbAPI();
};

class SQLAPI_API ssOleDbConnectionHandles : public saConnectionHandles
{
public:
	ssOleDbConnectionHandles();

	IDBInitialize *pIDBInitialize;
	IDBCreateCommand *pIDBCreateCommand;
	ITransactionLocal *pITransactionLocal;
};

class SQLAPI_API ssOleDbCommandHandles : public saCommandHandles
{
public:
	ssOleDbCommandHandles();

	ICommandText *pICommandText;
	IMultipleResults *pIMultipleResults;
	IRowset *pIRowset;
};

extern ssOleDbAPI g_ssOleDbAPI;

#endif // !defined(AFX_SSOLEDBAPI_H__11EAA72A_EFF5_4944_8B1F_44973D30EF73__INCLUDED_)
