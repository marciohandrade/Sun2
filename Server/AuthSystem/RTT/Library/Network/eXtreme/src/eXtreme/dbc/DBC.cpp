#define  _WIN32_DCOM

#include <eXtreme/Common.h>
#include <eXtreme/eXtreme.h>
#include <eXtreme/dbc/DBC.h>

#include <objbase.h>

using namespace eXtreme;

DBC::DBC()
	: m_connection( 0 ), m_callbackError( 0 )
{
	::CoInitializeEx( 0, COINIT_MULTITHREADED );
}

DBC::~DBC()
{
	::CoUninitialize();
}

bool DBC::Open( _bstr_t connStr, long timeout )
{
	/*
	#   Connect via an IP address:
	"Provider=sqloledb;Data Source=190.190.200.100,1433;Network Library=DBMSSOCN;Initial Catalog=pubs;User ID=sa;Password=asdasd;"

		(DBMSSOCN=TCP/IP instead of Named Pipes, at the end of the Data Source is the port to use (1433 is the default))
	*/
	try
	{
		m_connection.CreateInstance( __uuidof( Connection ) );

		m_connection->ConnectionString	= connStr;
		m_connection->ConnectionTimeout	= timeout;
		m_connection->Open( "", "", "", adConnectUnspecified );
	}
	catch( _com_error& e )
	{
		if( m_callbackError != 0 )
		{
			( *m_callbackError )( this, e.Error(), e.ErrorMessage(), e.Source(), e.Description() );
		}
		
		return false;
	}

	return true;
}

bool DBC::Open( _bstr_t dbName, _bstr_t initDB, _bstr_t account, _bstr_t pw, long timeout )
{
	try
	{
		m_connection.CreateInstance( __uuidof( Connection ) );

		m_connection->ConnectionString = "DSN=" + dbName + ";UID=" + account + ";PWD=" + pw
											+ ";Initial Catalog=" + initDB + ";";
		m_connection->ConnectionTimeout = timeout;
		m_connection->Open( "", "", "", adConnectUnspecified );
	}
	catch( _com_error& e )
	{
		if( m_callbackError != 0 )
		{
			( *m_callbackError )( this, e.Error(), e.ErrorMessage(), e.Source(), e.Description() );
		}
		
		return false;
	}

	return true;
}

void DBC::Close()
{
	try
	{
		if( m_connection != 0 )
		{
			if( m_connection->State != adStateClosed )
				m_connection->Close();
		}
	}
	catch( _com_error& e )
	{
		if( m_callbackError != 0 )
			( *m_callbackError )( this, e.Error(), e.ErrorMessage(), e.Source(), e.Description() );
	}

	m_connection = 0;
}

bool DBC::Query( RecordSet* bind, _bstr_t queryStr )
{
	try
	{
		m_connection->Errors->Clear();

		if( bind == 0 )
		{
			m_lastQuery = queryStr;

			m_connection->Execute( queryStr, 0, adExecuteNoRecords );
		}
		else
		{
			m_lastQuery			= queryStr;
			bind->m_lastQuery	= queryStr;

			if( bind->Fetch( queryStr, m_connection ) == false )
			{
				return false;
			}
		}
	}
	catch( _com_error& e )
	{
		if( m_callbackError != 0 )
			( *m_callbackError )( this, e.Error(), e.ErrorMessage(), e.Source(), e.Description() );

		return false;
	}

	return true;
}

bool DBC::CallStoredProcedure( RecordSet* bind, _bstr_t spName )
{
	if( bind == 0 )
	{
		return false;
	}

	try
	{
		bind->PrepareSP( spName );

		if( bind->BindingParameter() == false )
		{
			return false;
		}

		bind->m_command->ActiveConnection = m_connection;

		bind->m_command->Execute( 0, 0, adCmdStoredProc );
		bind->FetchParameter();
	}
	catch( _com_error& e )
	{
		if( m_callbackError != 0 )
			( *m_callbackError )( this, e.Error(), e.ErrorMessage(), e.Source(), e.Description() );

		return false;
	}

	return true;
}

DWORD DBC::GetState()
{
	try
	{
		return m_connection->State;
	}
	catch( ... )
	{
	}

	return adStateClosed;
}