#include "stdafx.h"
#include ".\ioledb.h"
#include ".\QueryResult.h"

#define DBINITCONSTANTS
#define INITGUID

#include <SQLOLEDB.h>
#include <oledb.h>			// OLE DB Header
#include <oledberr.h>		// OLE DB Errors
#include <msdaguid.h>
#include <msdasql.h>
#include <oleauto.h>
#include <atldbcli.h>		// for cenumlator


#define OLEHR(hr)				\
	if(FAILED(hr))				\
	goto CLEANUP;
#define OLEOK(hr)				\
	if(hr)						\
	goto CLEANUP;

IOLEDB::IOLEDB(void)
	:	m_pIDataInitialize ( NULL ),
		m_pIDBInitialize ( NULL ),
		m_pIDBCreateSession ( NULL ),
		m_pIDBCreateCommand ( NULL ),
		m_pICommand ( NULL ),
		m_pICommandText ( NULL ),
		m_pIRowset ( NULL ),
		m_pIColumnsInfo ( NULL ),
		m_pIAccessor ( NULL ),
		m_hAccessor ( NULL ),
		m_hRow ( NULL ),
		m_phRow ( NULL ),
		m_pDBColumnInfo ( NULL )
		
{
	CoInitialize( NULL );
}

IOLEDB::~IOLEDB(void)
{
	CoUninitialize();
}

VOID IOLEDB::Release()
{
	if( m_pIAccessor )
		m_pIAccessor->ReleaseAccessor( m_hAccessor, NULL );
	SAFE_RELEASE( m_pIAccessor );
	SAFE_RELEASE( m_pIRowset );
	SAFE_RELEASE( m_pICommandText );
	SAFE_RELEASE(m_pICommand);
	SAFE_RELEASE(m_pIDBCreateCommand);
	SAFE_RELEASE(m_pIDBCreateSession);
	SAFE_RELEASE(m_pIDBInitialize);
	SAFE_RELEASE(m_pIDataInitialize);
}

BOOL IOLEDB::Init( char * pszIPAddress, char * pszDatabaseName, char * pszUserName, char * pszUserPassword, fnCallBackMessage fnErrorMessage )
{
	IDBCInterface::Init( pszIPAddress, pszDatabaseName, pszUserName, pszUserPassword, fnErrorMessage );

	HRESULT		hr;
	
#if (0)
	m_phRow = &m_hRow;
	CLSID		clsidOLEDB;
	OLECHAR		oledbCLSID[256];
	MultiByteToWideChar( CP_ACP, 0, "SQLOLEDB.1", -1, oledbCLSID, 256 );
	CLSIDFromProgID( oledbCLSID , &clsidOLEDB );
	OLEHR( hr = CoCreateInstance( clsidOLEDB, 0, CLSCTX_INPROC_SERVER, IID_IDBInitialize, (VOID **)&m_pIDBInitialize ) );
#else
#if (1)
	/// Create DBInitialize
	/// 
	OLEHR( hr = CoCreateInstance( CLSID_SQLOLEDB, 0, CLSCTX_INPROC_SERVER, IID_IDBInitialize, (VOID **)&m_pIDBInitialize ) );
#else
	/// OLEDB provider for ODBC
	OLEHR( hr = CoCreateInstance( CLSID_MSDAINITIALIZE, NULL, CLSCTX_INPROC_SERVER, IID_IDataInitialize, (VOID **)&m_pIDataInitialize ) );
	CLSID		clsidOLEDB;
	OLEHR( hr = CLSIDFromProgID(L"SQLOLEDB", &clsidOLEDB) );
	OLEHR( hr = m_pIDataInitialize->CreateDBInstance( clsidOLEDB, NULL, CLSCTX_INPROC_SERVER, NULL, IID_IDBInitialize, (IUnknown **)&m_pIDBInitialize ) );
#endif
#endif
	

	return TRUE;

CLEANUP:
	SQLError( hr );
	SAFE_RELEASE(m_pIDBInitialize);
	return FALSE;
}

BOOL IOLEDB::Connect()
{
	HRESULT			hr = 0;
	const int		MAX_PROPERTY_NUM	= 6;
	DBPROP			properties[MAX_PROPERTY_NUM];
	DBPROPSET		propertySet;

	for( int i = 0 ; i < MAX_PROPERTY_NUM ; ++i )
	{
		VariantInit( &properties[i].vValue );
		properties[i].dwOptions	= DBPROPOPTIONS_REQUIRED;
		properties[i].colid		= DB_NULLID;
	}

	OLECHAR IPAddress[32], DataBase[32], UserID[32], Password[32];

	MultiByteToWideChar( CP_ACP, 0, m_pszIPAddress, -1, IPAddress, 32 );
	MultiByteToWideChar( CP_ACP, 0, m_pszDatabaseName, -1, DataBase, 32 );
	MultiByteToWideChar( CP_ACP, 0, m_pszUserName, -1, UserID, 32 );
	MultiByteToWideChar( CP_ACP, 0, m_pszUserPassword, -1, Password, 32 );

	properties[0].dwPropertyID		= DBPROP_INIT_PROMPT;
	properties[0].vValue.vt			= VT_I2;
	properties[0].vValue.iVal		= DBPROMPT_NOPROMPT;

	properties[1].dwPropertyID		= DBPROP_INIT_LOCATION;
	properties[1].vValue.vt			= VT_BSTR;
	properties[1].vValue.bstrVal	= SysAllocString( IPAddress );

	properties[2].dwPropertyID		= DBPROP_INIT_DATASOURCE;
	properties[2].vValue.vt			= VT_BSTR;
	properties[2].vValue.bstrVal	= SysAllocString( IPAddress );

	properties[3].dwPropertyID		= DBPROP_INIT_CATALOG;
	properties[3].vValue.vt			= VT_BSTR;
	properties[3].vValue.bstrVal	= SysAllocString( DataBase );

	properties[4].dwPropertyID		= DBPROP_AUTH_USERID;
	properties[4].vValue.vt			= VT_BSTR;
	properties[4].vValue.bstrVal	= SysAllocString( UserID );

	properties[5].dwPropertyID		= DBPROP_AUTH_PASSWORD;
	properties[5].vValue.vt			= VT_BSTR;
	properties[5].vValue.bstrVal	= SysAllocString( Password );
	
		
	// level of prompting that will be done to complete the connection process
	propertySet.guidPropertySet		= DBPROPSET_DBINIT;
	propertySet.cProperties			= MAX_PROPERTY_NUM;
	propertySet.rgProperties		= properties;



	/// Create DBProperties
	IDBProperties * pIDBProperties;
	OLEHR( m_pIDBInitialize->QueryInterface( IID_IDBProperties, (VOID **)&pIDBProperties) );	
	OLEHR( pIDBProperties->SetProperties( 1, &propertySet ) );

	pIDBProperties->Release();

	for( i = 1 ; i < MAX_PROPERTY_NUM ; ++i )
		SysFreeString( properties[i].vValue.bstrVal );

	OLEHR( hr = m_pIDBInitialize->Initialize() );


	/// Create DBCreateSession
	OLEHR( hr = m_pIDBInitialize->QueryInterface( IID_IDBCreateSession, (VOID **)&m_pIDBCreateSession ) );

	/// Create DBCreateCommand
	OLEHR( hr = m_pIDBCreateSession->CreateSession( NULL, IID_IDBCreateCommand, (IUnknown **)&m_pIDBCreateCommand ) );

	/// Create Command
	OLEHR( hr = m_pIDBCreateCommand->CreateCommand( NULL, IID_ICommand, (IUnknown **)&m_pICommand ) );
	//pIDBCreateCommand->CreateCommand( NULL, IID_ICommandText,  (IUnknown**) &pICommandText)
	
	/// Create CommandText
	OLEHR( hr = m_pICommand->QueryInterface( IID_ICommandText, (VOID **)&m_pICommandText ) );

	return TRUE;
CLEANUP:
	SQLError( hr );
	SAFE_RELEASE(m_pICommandText);
	SAFE_RELEASE(m_pICommand);
	SAFE_RELEASE(m_pIDBCreateCommand);
	SAFE_RELEASE(m_pIDBCreateSession);
	return FALSE;
}

VOID IOLEDB::Disconnect()
{
	SAFE_RELEASE(m_pICommandText);
	SAFE_RELEASE(m_pICommand);
	SAFE_RELEASE(m_pIDBCreateCommand);
	SAFE_RELEASE(m_pIDBCreateSession);
}

BOOL IOLEDB::Execute( Query * pQuery )
{
	HRESULT hr;
	
	OLECHAR ocQuery[MAX_QUERY_LENGTH];

	::MultiByteToWideChar( CP_ACP, 0, pQuery->GetQuery(), -1, ocQuery, MAX_QUERY_LENGTH );

	OLEHR( hr = m_pICommandText->SetCommandText( DBGUID_DBSQL, ocQuery ) );

	OLEHR( hr = m_pICommandText->Execute( NULL, IID_IRowset, NULL, pQuery->GetAffectNumPtr(), (IUnknown **)&m_pIRowset ) );

	OLEOK( 0 == pQuery->GetAffectNum() || NULL == m_pIRowset );
	
	/*
	// column정보를 읽을 필요가 없다.
	OLEHR( hr = m_pIRowset->QueryInterface( IID_IColumnsInfo, (VOID **)&m_pIColumnsInfo ) );
	OLECHAR * wcStringBuffer;
	OLEHR( hr = m_pIColumnsInfo->GetColumnInfo( pQuery->GetResultColNumPtr(), &m_pDBColumnInfo, &wcStringBuffer ) );
	CoTaskMemFree(wcStringBuffer);

	m_pIColumnsInfo->Release();
	*/

	ULONG offset;
	ULONG * pOffset = &offset;
	LONG csize;
	SHORT sType;
	for( ULONG i = 0 ; i < pQuery->GetMaxColNum() ; ++i )
	{
		pQuery->GetColumnInfo( 0, i, pOffset, csize, sType);
		m_pDBBindings[i].iOrdinal	= i+1;									///< index
		m_pDBBindings[i].wType		= sType;								///< GetOleType()
		m_pDBBindings[i].cbMaxLen	= csize;								///< sizeofType
		m_pDBBindings[i].obValue	= offset;								///< offsetbuffer()
		m_pDBBindings[i].obStatus	= NULL;
		m_pDBBindings[i].obLength	= NULL;
		m_pDBBindings[i].pTypeInfo	= NULL;
		m_pDBBindings[i].pObject	= NULL;
		m_pDBBindings[i].pBindExt	= NULL;
		m_pDBBindings[i].dwPart		= DBPART_VALUE;
		m_pDBBindings[i].dwMemOwner	= DBMEMOWNER_CLIENTOWNED;
		m_pDBBindings[i].eParamIO	= DBPARAMIO_NOTPARAM;
		m_pDBBindings[i].dwFlags	= NULL;
		m_pDBBindings[i].bPrecision	= 0;
		m_pDBBindings[i].bScale		= 0;
	}

	OLEHR( hr = m_pIRowset->QueryInterface( IID_IAccessor, (VOID **)&m_pIAccessor ) );
	OLEHR( hr = m_pIAccessor->CreateAccessor( DBACCESSOR_ROWDATA, pQuery->GetMaxColNum(), m_pDBBindings, 0, &m_hAccessor, NULL ) );
	

	
	ULONG ulRow = 0;
	do
	{
		m_pIRowset->GetNextRows( DB_NULL_HCHAPTER, 0, 1, pQuery->GetResultRowNumPtr(), &m_phRow );

		if( 0 == pQuery->GetResultRowNum() ) break;

		OLEHR( hr = m_pIRowset->GetData( m_phRow[0], m_hAccessor, pQuery->GetBuffer(ulRow) ) );

		OLEHR( hr = m_pIRowset->ReleaseRows( pQuery->GetResultRowNum(), m_phRow, NULL, NULL, NULL ) );

		++ulRow;
	}
	while(TRUE);
	pQuery->SetResultRowNum( ulRow );
	pQuery->SetResultColNum( pQuery->GetMaxColNum() );

	if( m_pIAccessor )
	m_pIAccessor->ReleaseAccessor( m_hAccessor, NULL );
	SAFE_RELEASE( m_pIAccessor );
	SAFE_RELEASE( m_pIRowset );

	return TRUE;

CLEANUP:
	// 초기값 셋팅
	pQuery->SetAffectNum( 0 );
	pQuery->SetResultRowNum( 0 );
	pQuery->SetResultColNum( 0 );

	SQLError( hr );
	if( m_pIAccessor )
	m_pIAccessor->ReleaseAccessor( m_hAccessor, NULL );
	SAFE_RELEASE( m_pIAccessor );
	SAFE_RELEASE( m_pIRowset );

	return FALSE;
}

/*
VOID IOLEDB::GetString()
{
	switch( m_pDBBindings[dwColPos].wType )
	{
	case DBTYPE_R4:		//  4 Byte Float
	case DBTYPE_R8:		//  8 Byte Float
	case DBTYPE_I8:		//  8 Byte int
	case DBTYPE_UI8:	//  8 Byte unsinged  int
	case DBTYPE_I1:		//  1 Byte int
	case DBTYPE_I2:		//  2 Byte int
	case DBTYPE_I4:		//  4 Byte int
	case DBTYPE_UI1:	//  1 Byte unsinged  int
	case DBTYPE_UI2:	//  2 Byte unsinged  int
	case DBTYPE_UI4:	//  4 Byte unsinged  int
	case DBTYPE_STR :	//  String
	case DBTYPE_BSTR :	//  UniCode String
	case DBTYPE_WSTR :	//  UniCode End-Null String
	case DBTYPE_DBDATE :	//  DB Date
	case DBTYPE_DBTIME :	//  DB Time
	case DBTYPE_DBTIMESTAMP : //  DB TimeStamp
	}
}
*/

VOID IOLEDB::zDisplayProvider()
{
	HRESULT		hr;

	CEnumerator enumDataSource;
	hr = enumDataSource.Open();
	hr = enumDataSource.MoveFirst();
	while(hr == S_OK)
	{
		hr = enumDataSource.GetData();  // DataSource 얻기
		CLSID       clsid;
		LPOLESTR    lpwstrProgID;
		char strDesc[256], strProgID[256], strCLSID[256];

		// CLSID로 ProgID 얻기
		CLSIDFromString(enumDataSource.m_pAccessor->m_szParseName, &clsid);
		ProgIDFromCLSID(clsid, &lpwstrProgID);

		// Char 변환
		WideCharToMultiByte(CP_ACP, 0, enumDataSource.m_pAccessor->m_szDescription, -1, strDesc, 256, NULL, NULL);
		WideCharToMultiByte(CP_ACP, 0, lpwstrProgID, -1, strProgID, 256, NULL, NULL);
		WideCharToMultiByte(CP_ACP, 0, enumDataSource.m_pAccessor->m_szParseName, -1, strCLSID, 256, NULL, NULL);

		char aaa[256];
		sprintf(aaa, "%s, %s, %s\n", strDesc, strProgID, strCLSID);
		m_fnErrorMessage(aaa );

		::GlobalFree(lpwstrProgID);

		hr = enumDataSource.MoveNext();
	}

}

BOOL IOLEDB::SQLError( HRESULT hr )
{
	if( 0 == hr )	return TRUE;

	typedef struct 
	{
		DWORD dwFlag;
		char *szText;
	} Note;
	
	static Note ResultCodes[] = {
			// oledberr.h error codes
			NOTE(DB_E_BADACCESSORHANDLE),
			NOTE(DB_E_BADACCESSORHANDLE),
			NOTE(DB_E_ROWLIMITEXCEEDED),
			NOTE(DB_E_READONLYACCESSOR),
			NOTE(DB_E_SCHEMAVIOLATION),
			NOTE(DB_E_BADROWHANDLE),
			NOTE(DB_E_OBJECTOPEN),
			NOTE(DB_E_BADBINDINFO),
			NOTE(DB_SEC_E_PERMISSIONDENIED),
			NOTE(DB_E_NOTAREFERENCECOLUMN),
			NOTE(DB_E_NOCOMMAND),
			NOTE(DB_E_BADBOOKMARK),
			NOTE(DB_E_BADLOCKMODE),
			NOTE(DB_E_PARAMNOTOPTIONAL),
			NOTE(DB_E_BADRATIO),
			NOTE(DB_E_ERRORSINCOMMAND),
			NOTE(DB_E_BADSTARTPOSITION),
			NOTE(DB_E_NOTREENTRANT),
			NOTE(DB_E_NOAGGREGATION),
			NOTE(DB_E_DELETEDROW),
			NOTE(DB_E_CANTFETCHBACKWARDS),
			NOTE(DB_E_ROWSNOTRELEASED),
			NOTE(DB_E_BADSTORAGEFLAG),
			NOTE(DB_E_BADSTATUSVALUE),
			NOTE(DB_E_CANTSCROLLBACKWARDS),
			NOTE(DB_E_INTEGRITYVIOLATION),
			NOTE(DB_E_ABORTLIMITREACHED),
			NOTE(DB_E_DUPLICATEINDEXID),
			NOTE(DB_E_NOINDEX),
			NOTE(DB_E_INDEXINUSE),
			NOTE(DB_E_NOTABLE),
			NOTE(DB_E_CONCURRENCYVIOLATION),
			NOTE(DB_E_BADCOPY),
			NOTE(DB_E_BADPRECISION),
			NOTE(DB_E_BADSCALE),
			NOTE(DB_E_BADID),
			NOTE(DB_E_BADTYPE),
			NOTE(DB_E_DUPLICATECOLUMNID),
			NOTE(DB_E_DUPLICATETABLEID),
			NOTE(DB_E_TABLEINUSE),
			NOTE(DB_E_NOLOCALE),
			NOTE(DB_E_BADRECORDNUM),
			NOTE(DB_E_BOOKMARKSKIPPED),
			NOTE(DB_E_BADPROPERTYVALUE),
			NOTE(DB_E_INVALID),
			NOTE(DB_E_BADACCESSORFLAGS),
			NOTE(DB_E_BADSTORAGEFLAGS),
			NOTE(DB_E_BYREFACCESSORNOTSUPPORTED),
			NOTE(DB_E_NULLACCESSORNOTSUPPORTED),
			NOTE(DB_E_NOTPREPARED),
			NOTE(DB_E_BADACCESSORTYPE),
			NOTE(DB_E_WRITEONLYACCESSOR),
			NOTE(DB_SEC_E_AUTH_FAILED),
			NOTE(DB_E_CANCELED),
			NOTE(DB_E_BADSOURCEHANDLE),
			NOTE(DB_S_ROWLIMITEXCEEDED),
			NOTE(DB_S_COLUMNTYPEMISMATCH),
			NOTE(DB_S_TYPEINFOOVERRIDDEN),
			NOTE(DB_S_BOOKMARKSKIPPED),
			NOTE(DB_S_ENDOFROWSET),
			NOTE(DB_S_BUFFERFULL),
			NOTE(DB_S_CANTRELEASE),
			NOTE(DB_S_DIALECTIGNORED),
			NOTE(DB_S_UNWANTEDPHASE),
			NOTE(DB_S_COLUMNSCHANGED),
			NOTE(DB_S_ERRORSRETURNED),
			NOTE(DB_S_BADROWHANDLE),
			NOTE(DB_S_DELETEDROW),
			NOTE(DB_S_STOPLIMITREACHED),
			NOTE(DB_S_LOCKUPGRADED),
			NOTE(DB_S_PROPERTIESCHANGED),
			NOTE(DB_S_ERRORSOCCURRED),
			NOTE(DB_S_PARAMUNAVAILABLE),
			NOTE(DB_S_MULTIPLECHANGES),

			// winerr.h
			NOTE(E_UNEXPECTED),
			NOTE(E_NOTIMPL),
			NOTE(E_OUTOFMEMORY),
			NOTE(E_INVALIDARG),
			NOTE(E_NOINTERFACE),
			NOTE(E_POINTER),
			NOTE(E_HANDLE),
			NOTE(E_ABORT),
			NOTE(E_FAIL),
			NOTE(E_ACCESSDENIED),
			NOTE(S_OK),
			NOTE(S_FALSE),
			NOTE(E_UNEXPECTED),
			NOTE(E_NOTIMPL),
			NOTE(E_OUTOFMEMORY),
			NOTE(E_INVALIDARG),
			NOTE(E_NOINTERFACE),
			NOTE(E_POINTER),
			NOTE(E_HANDLE),
			NOTE(E_ABORT),
			NOTE(E_FAIL),
			NOTE(E_ACCESSDENIED),
			// BindMoniker Errors
			NOTE(MK_E_NOOBJECT),
			NOTE(MK_E_EXCEEDEDDEADLINE),
			NOTE(MK_E_CONNECTMANUALLY),
			NOTE(MK_E_INTERMEDIATEINTERFACENOTSUPPORTED),
			NOTE(STG_E_ACCESSDENIED),
			NOTE(MK_E_SYNTAX),
			NOTE(MK_E_CANTOPENFILE),
	};

	char buff[255];

	int err_num = sizeof(ResultCodes)/sizeof(*ResultCodes);
	for( int j = 0; j < err_num ; ++j ) 
	{
		if (ResultCodes[j].dwFlag == (DWORD)hr)
			break;
	}
	
	if( j == err_num )
		sprintf( buff, "HREULT : <Unknown>" );
	else
		sprintf( buff, "HREULT : %d, %s",  ResultCodes[j].dwFlag, ResultCodes[j].szText );
	m_fnErrorMessage( buff );


	return FALSE;
}