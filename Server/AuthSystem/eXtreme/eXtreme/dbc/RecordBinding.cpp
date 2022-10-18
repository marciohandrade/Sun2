#include <eXtreme/dbc/RecordBinding.h>

using namespace eXtreme;

RecordBinding::RecordBinding()
	: m_callbackError( 0 )
{
}

RecordBinding::~RecordBinding()
{
	while( m_listRecordInfo.size() > 0 )
	{
		RecordInfo*	info = m_listRecordInfo.front();

		m_listRecordInfo.pop_front();
		delete info;
	}
}

void RecordBinding::BindVariable( void* ptr, _bstr_t rowName )
{
	RecordInfo*		info = new RecordInfo();

	info->ptr		= ptr;
	info->rowName	= rowName;

	m_listRecordInfo.push_back( info );
}

void RecordBinding::BindParameter( void* ptr, _bstr_t rowName,
										DataTypeEnum type, ParameterDirectionEnum dir, long size, _bstr_t var )
{
	RecordInfo*		info = new RecordInfo();

	info->ptr		= ptr;
	info->rowName	= rowName;
	info->type		= type;
	info->dir		= dir;
	info->size		= size;
	info->var		= var;

	m_listRecordInfo.push_back( info );
}

bool RecordBinding::Fetch( _bstr_t& queryStr, _ConnectionPtr conn )
{
	try
	{
		EndFetch();

		m_recordSet.CreateInstance( __uuidof( Recordset ) );
		m_recordSet->Open( queryStr, ( IDispatch* )conn, adOpenStatic, adLockReadOnly, adCmdText );

		if( m_recordSet == 0 || m_recordSet->GetState() == adStateClosed )
			return false;

		if( IsEndRecord() == true )
			return true;

		m_recordSet->MoveFirst();
	}
	catch( _com_error& e )
	{
		if( m_callbackError != 0 )
			( *m_callbackError )( this, e.Error(), e.ErrorMessage(), e.Source(), e.Description() );

		return false;
	}

	return BindingVariable();
}

void RecordBinding::EndFetch()
{
	if( m_recordSet == 0 )
		return;

	m_recordSet->Close();
	m_recordSet		= 0;
	m_opItemCount	= 0;
}

bool RecordBinding::BindingVariable()
{
	ListRecordInfo::iterator	iter;
	RecordInfo*					info;

	try
	{
		if( IsEndRecord() == true )
		{
			EndFetch();
			return false;
		}

		iter = m_listRecordInfo.begin();
		while( iter != m_listRecordInfo.end() )
		{
			info = *iter;

			AttachVariable( m_recordSet->Fields->GetItem( info->rowName )->Value, info->ptr );

			iter++;
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

bool RecordBinding::BindingParameter()
{
	ListRecordInfo::iterator	iter;
	RecordInfo*					info;
	_ParameterPtr				param;

	try
	{
		iter = m_listRecordInfo.begin();
		while( iter != m_listRecordInfo.end() )
		{
			info = *iter;

			if( info->dir == adParamInput || info->dir == adParamInputOutput )
				param = m_command->CreateParameter( info->rowName, info->type, info->dir, info->size, info->var );
			else if( info->dir == adParamOutput )
				param = m_command->CreateParameter( info->rowName, info->type, info->dir, info->size );
			else if( info->dir == adParamReturnValue )
				param = m_command->CreateParameter( info->rowName, info->type, info->dir, 0 );

			m_command->Parameters->Append( param );

			iter++;
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

void RecordBinding::PrepareSP( _bstr_t& spName )
{
	m_command.CreateInstance( __uuidof( Command ) );
	m_command->CommandText	= spName;
	m_command->CommandType	= adCmdStoredProc;
}

bool RecordBinding::IsEndRecord()
{
	try
	{
		return ( m_recordSet == 0 || m_recordSet->adoEOF != FALSE );
	}
	catch( _com_error& e )
	{
		if( m_callbackError != 0 )
			( *m_callbackError )( this, e.Error(), e.ErrorMessage(), e.Source(), e.Description() );

		return true;
	}

	return true;
}

bool RecordBinding::Next()
{
	m_opItemCount = 0;

	try
	{
		HRESULT		hr;

		hr = m_recordSet->MoveNext();
		if( FAILED( hr ) )
			return false;
	}
	catch( _com_error& e )
	{
		if( m_callbackError != 0 )
			( *m_callbackError )( this, e.Error(), e.ErrorMessage(), e.Source(), e.Description() );

		EndFetch();

		return false;
	}

	return BindingVariable();
}

bool RecordBinding::Prev()
{
	m_opItemCount = 0;

	try
	{
		HRESULT		hr;

		hr = m_recordSet->MovePrevious();
		if( FAILED( hr ) )
			return false;
	}
	catch( _com_error& e )
	{
		if( m_callbackError != 0 )
			( *m_callbackError )( this, e.Error(), e.ErrorMessage(), e.Source(), e.Description() );

		EndFetch();

		return false;
	}

	return BindingVariable();
}

void RecordBinding::AttachVariable( _variant_t& value, void* ptr )
{
	switch( value.vt )
	{
	case  VT_I2:		*( SHORT* )ptr		= ( SHORT )value.iVal;				break;
	case  VT_INT:
	case  VT_I4:		*( INT* )ptr		= ( INT )value.intVal;				break;
	case  VT_I8:		*( LONGLONG* )ptr	= ( LONGLONG )value.llVal;			break;
	case  VT_UI2:		*( USHORT* )ptr		= ( USHORT )value.uiVal;			break;
	case  VT_UINT:
	case  VT_UI4:		*( UINT* )ptr		= ( UINT )value.uintVal;			break;
	case  VT_UI8:		*( ULONGLONG* )ptr	= ( ULONGLONG )value.ullVal;		break;
	case  VT_DATE:		*( DATE* )ptr		= ( DATE )value.date;				break;
	case  VT_BOOL:		*( BOOL* )ptr		= ( BOOL )value.boolVal;			break;
	case  VT_DECIMAL:	*( LONGLONG* )ptr	= ( LONGLONG )value.llVal;			break;
	case  VT_BSTR:		wcscpy( ( wchar_t* )ptr, value.bstrVal );				break;
	case  VT_NULL:		/* Binary */	break;
	}
}

void RecordBinding::FetchParameter()
{
	ListRecordInfo::iterator	iter;
	RecordInfo*					info;

	try
	{
		iter = m_listRecordInfo.begin();
		while( iter != m_listRecordInfo.end() )
		{
			info = *iter;

			if( info->ptr != 0 )
				AttachVariable( m_command->Parameters->Item[info->rowName]->Value, info->ptr );

			iter++;
		}
	}
	catch( _com_error& e )
	{
		if( m_callbackError != 0 )
			( *m_callbackError )( this, e.Error(), e.ErrorMessage(), e.Source(), e.Description() );
	}
}

long RecordBinding::GetRecordCount()
{
	long	count;

	if( m_recordSet == 0 )
		return 0;

	try
	{
		count = m_recordSet->RecordCount;
	}
	catch( _com_error& e )
	{
		if( m_callbackError != 0 )
			( *m_callbackError )( this, e.Error(), e.ErrorMessage(), e.Source(), e.Description() );

		count = 0;
	}

	return count;
}