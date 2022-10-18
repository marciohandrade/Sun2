#include "stdafx.h"
#include "DBWorker.h"
#include "QueryManager.h"
#include "AccountManager.h"
#include "AuthFrame.h"
#include "DBDecryptFileReader.h"
#include "Query_Auth.h"
#include "Query_Log.h"
#include "Query_RTT.h"
#include "Query_Netcafe.h"


DBWorker::DBWorker()
{
	m_EndQueryCompleted = FALSE;
	ZeroMemory( m_szConnDBString, sizeof(TCHAR)*256*DB_TYPE_MAX );
}

DBWorker::~DBWorker()
{
}

VOID DBWorker::CallbackErrorDBC( eXtreme::DBC* dbc, HRESULT result, _bstr_t msg, _bstr_t src, _bstr_t desc )
{
	SUNLOG( eCRITICAL_LOG, "[DBC Error]\n" );
	SUNLOG( eCRITICAL_LOG, "Code = %08lx\n", result );
	SUNLOG( eCRITICAL_LOG, "Means = %s\n", ( char* )msg );
	SUNLOG( eCRITICAL_LOG, "Source = %s\n", ( char* )src );
	SUNLOG( eCRITICAL_LOG, "Query = %s\n", ( char* )dbc->m_lastQuery );
	SUNLOG( eCRITICAL_LOG, "Description = %s\n", ( char* )desc );
}

VOID DBWorker::CallbackErrorRecordSet( eXtreme::RecordSet* recordset, HRESULT result, _bstr_t msg, _bstr_t src, _bstr_t desc )
{
	SUNLOG( eCRITICAL_LOG, "[RecordSet Error]\n" );
	SUNLOG( eCRITICAL_LOG, "Code = %08lx\n", result );
	SUNLOG( eCRITICAL_LOG, "Means = %s\n", ( char* )msg );
	SUNLOG( eCRITICAL_LOG, "Source = %s\n", ( char* )src );
	SUNLOG( eCRITICAL_LOG, "Query = %s\n", ( char* )recordset->m_lastQuery );
	SUNLOG( eCRITICAL_LOG, "Description = %s\n", ( char* )desc );
}

VOID DBWorker::Initialize()
{
	eXtreme::Thread::Start();

	m_EndQueryCompleted = TRUE;
	ConnectDB();
}

VOID DBWorker::Destroy()
{
	// 마지막을 알리는 쿼리를 보낸다.
	QueryManager::Instance()->EndQuery();

	Terminate( 0 );

	for( int i = 0; i < DB_TYPE_MAX; i++ )
	{
		m_DBConnection[i].Close();
	}
}

BOOL DBWorker::Run()
{
	if( !m_EndQueryCompleted )
		return TRUE;

	// Query 객체 하나를 가져온다.
	QueryObject *pQuery = QueryManager::Instance()->WaitForQueryObj();
	if( !pQuery )
		return FALSE;

	// 마지막을 알리는 쿼리가 오면 더이상 처리를 하지 않는다.
	if( pQuery->m_queryType == QUERYTYPE_END )
	{
		m_EndQueryCompleted = FALSE;
		return TRUE;
	}

	eXtreme::DBC * pDBConnection = &m_DBConnection[pQuery->GetDBType()];

	if( pDBConnection->GetState() == adStateClosed ) 
		pDBConnection->Open( m_szConnDBString[pQuery->GetDBType()] );

	pQuery->m_recordSet.m_callbackError = CallbackErrorRecordSet;

	pQuery->m_Success = pDBConnection->CallStoredProcedure( &pQuery->m_recordSet, pQuery->m_spName.data(), pQuery->m_query.data() );

	QueryManager::Instance()->AddQueryResult( pQuery );

	return FALSE;
}


BOOL DBWorker::ConnectDB()
{
	DBDecryptFileReader* pReader = new DBDecryptFileReader();
	if( !pReader->ReadStartFromBuffer( CryptInfoStream::GetCryptStream(), "AUTH" ) )
	{
		SUNLOG( eCRITICAL_LOG, "[DB Error]Can't open encrypted file\n" );
		pReader->ReadEnd();
		delete pReader;
		return FALSE;
	}

	// DB의 ID, PW 설정(Netcafe는 ini에서 이미 설정했다)
	for( int DBType = 0; DBType < DB_TYPE_NETCAFE; DBType++ )
	{
		DB_INFO * pDBInfo = AuthFrame::Instance()->GetDBInfo( (DB_TYPE)DBType );
        // (CHANGES) (f100531.3L) change an authentication information loader \
        // to support an authentication information enable loaded from a configuration file 
        if (pDBInfo->m_szID[0] != _T('\0') && pDBInfo->m_szPW[0] != _T('\0')) {
            continue;
        }
		ZeroMemory( pDBInfo->m_szID, sizeof(pDBInfo->m_szID) );
		ZeroMemory( pDBInfo->m_szPW, sizeof(pDBInfo->m_szPW) );

		int DBCryptType = DBType + 1;

		::strncpy( pDBInfo->m_szID, pReader->GetIdentification( DBCryptType ), MAX_DBCRYPT_LENGTH );
		::strncpy( pDBInfo->m_szPW, pReader->GetPassword( DBCryptType ), MAX_DBCRYPT_LENGTH );
	}

	// DB 오픈
	for(int DBType = 0; DBType < DB_TYPE_MAX; DBType++ )
	{
#ifndef _NET_CAFE
		if( DBType == DB_TYPE_NETCAFE )
			continue;
#endif

		DB_INFO * pDBInfo = AuthFrame::Instance()->GetDBInfo( (DB_TYPE)DBType );

		::_stprintf( m_szConnDBString[DBType], _T( "Provider=sqloledb;Data Source=%s;Network Library=DBMSSOCN;"
			"Initial Catalog=%s;User ID=%s;Password=%s;Max Pool Size=30;Min Pool Size=20;Pooling=yes" ), 
			pDBInfo->m_szIpPort, pDBInfo->m_szInitDB, pDBInfo->m_szID, pDBInfo->m_szPW );

		m_DBConnection[DBType].m_callbackError = CallbackErrorDBC;

		m_DBConnection[DBType].Open( m_szConnDBString[DBType] );
	}

	delete pReader;

	return TRUE;
}









