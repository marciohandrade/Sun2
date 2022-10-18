#include <eXtreme/Common.h>
#include <eXtreme/eXtreme.h>
#include <eXtreme/dbc/RecordSet.h>

using namespace eXtreme;

RecordSet::RecordSet()
	: m_callbackError( 0 )
{
}

RecordSet::~RecordSet()
{
	while( m_listRecordInfo.size() > 0 )
	{
		RecordInfo*	info = m_listRecordInfo.front();

		m_listRecordInfo.pop_front();
		delete info;
	}
}

void RecordSet::BindVariable( void* ptr, _bstr_t rowName )
{
	RecordInfo*		info = new RecordInfo();

	info->ptr		= ptr;
	info->rowName	= rowName;

	try
	{
		m_listRecordInfo.push_back( info );
	}
	catch( std::bad_alloc& )
	{
		OutputDebugString( "[eXtreme::RecordSet::BindVariable] std::bad_alloc exception.\n" );
	}
}

void RecordSet::BindParameter( void* ptr, _bstr_t rowName,
										DataTypeEnum type, ParameterDirectionEnum dir, long size, _bstr_t var )
{
	{
		for(ListRecordInfo::iterator iter = m_listRecordInfo.begin() ; iter != m_listRecordInfo.end() ; iter++ )
		{
			RecordInfo*	info = *iter;

			if( info->rowName == rowName )
			{
				info->ptr	= ptr;
				info->type	= type;
				info->dir	= dir;
				info->size	= size;
				info->var	= var;
								
				return;
			}
		}
	}

	RecordInfo*		info = new RecordInfo();

	info->ptr		= ptr;
	info->rowName	= rowName;
	info->type		= type;
	info->dir		= dir;
	info->size		= size;
	info->var		= var;

	try
	{
		m_listRecordInfo.push_back( info );
	}
	catch( std::bad_alloc& )
	{
		OutputDebugString( "[eXtreme::RecordSet::BindVariable] std::bad_alloc exception.\n" );
	}
}

bool RecordSet::Fetch( _bstr_t& queryStr, _ConnectionPtr conn )
{
	try
	{
		EndFetch();

		m_Recordset = conn->Execute( queryStr, 0, 0 );
		if( IsEndRecord() == false )
		{
			m_Recordset->MoveFirst();
		}
		else
		{
			return true;
		}
	}
	catch( _com_error& e )
	{
		if( m_callbackError != 0 )
		{
			( *m_callbackError )( this, e.Error(), e.ErrorMessage(), e.Source(), e.Description() );
		}

		EndFetch();

		return false;
	}

	return BindingVariable();
}

void RecordSet::EndFetch()
{
	if( m_Recordset == 0 || m_Recordset->GetState() == adStateClosed )
	{
		return;
	}

	try
	{
		m_Recordset->Close();
	}	
	catch( _com_error& e )
	{
		if( m_callbackError != 0 )
			( *m_callbackError )( this, e.Error(), e.ErrorMessage(), e.Source(), e.Description() );
	}

	m_Recordset		= 0;
	m_opItemCount	= 0;
}

bool RecordSet::BindingVariable()
{
	try
	{
		if( IsEndRecord() == true )
		{
			EndFetch();
			return false;
		}

		RecordInfo*					info = NULL;
		ListRecordInfo::iterator iter = m_listRecordInfo.begin();

		while( iter != m_listRecordInfo.end() )
		{
			info = *iter;

			_variant_t vAttachValue = m_Recordset->Fields->GetItem( info->rowName )->Value;

			AttachVariable( vAttachValue, info->ptr );

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

bool RecordSet::BindingParameter()
{
	try
	{
		RecordInfo*					info = NULL;
		_ParameterPtr				param;
		ListRecordInfo::iterator iter = m_listRecordInfo.begin();

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

void RecordSet::PrepareSP( _bstr_t& spName )
{
	m_command.CreateInstance( __uuidof( Command ) );
	m_command->CommandText	= spName;
	m_command->CommandType	= adCmdStoredProc;
}

bool RecordSet::IsEndRecord()
{
	try
	{
		return ( m_Recordset == 0 || m_Recordset->GetState() == adStateClosed || m_Recordset->adoEOF != FALSE );
	}
	catch( _com_error& e )
	{
		if( m_callbackError != 0 )
			( *m_callbackError )( this, e.Error(), e.ErrorMessage(), e.Source(), e.Description() );

		return true;
	}
}

bool RecordSet::Next()
{
	m_opItemCount = 0;

	try
	{
		HRESULT hr = m_Recordset->MoveNext();
		if( FAILED( hr ) )
		{
			return false;
		}
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

bool RecordSet::Prev()
{
	m_opItemCount = 0;

	try
	{
		HRESULT hr = m_Recordset->MovePrevious();
		if( FAILED( hr ) )
		{
			return false;
		}
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

void RecordSet::AttachVariable( _variant_t& value, void* ptr )
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
	case  VT_I1:		*( INT* )ptr		= ( INT )value.iVal;				break;
	case  VT_R8:		*( float* )ptr		= ( float )value.dblVal;			break;
	case  VT_NULL:		break;

		//  Binary
	case  VT_ARRAY | VT_UI1:	memcpy( ptr, value.parray->pvData, value.parray->rgsabound->cElements );	break;
	}
}

void RecordSet::FetchParameter()
{
	try
	{
		RecordInfo*					info = NULL;
		ListRecordInfo::iterator iter = m_listRecordInfo.begin();
		while( iter != m_listRecordInfo.end() )
		{
			info = *iter;

			if( info->ptr != 0 && ( info->dir == adParamOutput || info->dir == adParamInputOutput || info->dir == adParamReturnValue ) )
			{
				_variant_t vAttachValue = m_command->Parameters->Item[info->rowName]->Value;				
				AttachVariable( vAttachValue, info->ptr );
			}

			iter++;
		}
	}
	catch( _com_error& e )
	{
		if( m_callbackError != 0 )
		{
			( *m_callbackError )( this, e.Error(), e.ErrorMessage(), e.Source(), e.Description() );
		}
	}
}

long RecordSet::GetRecordCount()
{
	if( m_Recordset == 0 || m_Recordset->GetState() == adStateClosed )
		return 0;

	long	count = 0;

	try
	{
		count = m_Recordset->RecordCount;
	}
	catch( _com_error& e )
	{
		if( m_callbackError != 0 )
			( *m_callbackError )( this, e.Error(), e.ErrorMessage(), e.Source(), e.Description() );

		count = 0;
	}

	return count;
}