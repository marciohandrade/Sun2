#include "stdafx.h"
#include ".\isqllib.h"
#include ".\QueryResult.h"

ISQLLIB::ISQLLIB(void)
	:	m_pLoginRec ( NULL ),
		m_pDBProcess ( NULL )
{
}

ISQLLIB::~ISQLLIB(void)
{
}

VOID ISQLLIB::Release()
{
	Disconnect();
	dbexit();
}

BOOL ISQLLIB::Init( char * pszIPAddress, char * pszDatabaseName, char * pszUserName, char * pszUserPassword, fnCallBackMessage fnErrorMessage )
{
	IDBCInterface::Init( pszIPAddress, pszDatabaseName, pszUserName, pszUserPassword, fnErrorMessage );


	if( NULL == dbinit()  )						return FALSE;
	if( FAIL == ( m_pLoginRec = dblogin() ) )	return FALSE;

	DBSETLUSER( m_pLoginRec, m_pszUserName );
	DBSETLPWD( m_pLoginRec, m_pszUserPassword );
	DBSETLVERSION( m_pLoginRec, DBVER60 );

	return TRUE;
}

BOOL ISQLLIB::Connect()
{
	if( NULL == ( m_pDBProcess = dbopen( m_pLoginRec, m_pszIPAddress ) ) )			return FALSE;
	if( NULL == dbuse( m_pDBProcess, m_pszDatabaseName ) )							return FALSE;

	return TRUE;
}

VOID ISQLLIB::Disconnect()
{
	dbfreelogin( m_pLoginRec );
	dbclose( m_pDBProcess );
}

BOOL ISQLLIB::ExecuteLargeData( Query * pQuery )
{
	__UNUSED(pQuery);
	return FALSE;
}

BOOL ISQLLIB::Execute( Query * pQuery )
{
	RETCODE ret = 0;

	// use dbcmd to build up a multiple-line SQL statement
	if( FAIL == ( ret = dbcmd( m_pDBProcess, pQuery->GetQuery() ) ) )
		goto CLEANUP;

	if( FAIL == ( ret = dbsqlexec( m_pDBProcess ) ) )
		goto CLEANUP;


	if( FAIL == ( ret = dbresults( m_pDBProcess ) ) )
		goto CLEANUP;

	if( 0 == pQuery->GetMaxRowNum() )
	{
		///< insert, update query : force setting
		pQuery->SetAffectNum(1);
	}

	ULONG * pPtr;
	LONG csize;
	SHORT sType; 
	ULONG ulRow = 0;
	do
	{
		if( ulRow == pQuery->GetMaxRowNum() ) break;

		for( ULONG i = 0 ; i < pQuery->GetMaxColNum() ; ++i )
		{
			pQuery->GetColumnInfo( ulRow, i, pPtr, csize, sType);	
			if( FAIL == ( ret = dbbind( m_pDBProcess, i + 1, sType, (DBINT)0, (BYTE *)pPtr ) ) )
			{
				ASSERT( !"Failed dbbind()" );
				goto CLEANUP;
			}
		}

	}
	while( ( NO_MORE_ROWS != dbnextrow( m_pDBProcess ) ) && ++ulRow );

	pQuery->SetResultColNum( pQuery->GetMaxColNum() );
	pQuery->SetResultRowNum( ulRow );

CLEANUP:
	// 초기값 셋팅
	pQuery->SetAffectNum( 0 );
	pQuery->SetResultRowNum( 0 );
	pQuery->SetResultColNum( 0 );

	dbfreebuf( m_pDBProcess );
	while( NO_MORE_ROWS != dbnextrow( m_pDBProcess ) );

	return (ret == SUCCEED ? TRUE : FALSE );
}


