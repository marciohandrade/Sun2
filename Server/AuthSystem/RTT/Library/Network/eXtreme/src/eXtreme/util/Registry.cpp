#include <eXtreme/Common.h>
#include <eXtreme/eXtreme.h>
#include <eXtreme/util/Registry.h>

using namespace eXtreme;

Registry::Registry( HKEY key )
	: m_key( 0 ), m_defaultKey( key )
{
}

Registry::~Registry()
{
	Close();
}

bool Registry::Open( char* subKey )
{
	Close();

	::strcpy( m_subKey, subKey );

	return ( ::RegOpenKey( m_defaultKey, subKey, &m_key ) == ERROR_SUCCESS );
}

bool Registry::Create( char* key )
{
	::strcpy( m_subKey, key );

	return ( RegCreateKey( m_defaultKey, key, &m_key ) == ERROR_SUCCESS );
}

void Registry::Close()
{
	if( m_key == 0 )
	{
		return;
	}

	::RegCloseKey( m_key );
	m_key = 0;
}

bool Registry::ReadDWORD( char* valueName, unsigned long* buffer )
{
	if( m_key == 0 )
	{
		return false;
	}

	unsigned long	type = 0;
	unsigned long	len = sizeof( unsigned long );

	return ( ::RegQueryValueEx( m_key, valueName, 0, &type, ( LPBYTE )buffer, &len ) == ERROR_SUCCESS &&
			type == REG_DWORD );
}

bool Registry::ReadString( char* valueName, char* buffer )
{
	if( m_key == 0 )
	{
		return false;
	}

	unsigned long	type = 0;
	char			value[256] = {0, };	

	unsigned long len = sizeof( value );
	bool ret = ( ::RegQueryValueEx( m_key, valueName, 0, &type, ( LPBYTE )&value[0], &len ) == ERROR_SUCCESS &&
			type == REG_SZ );

	::strncpy( buffer, value, len );

	return ret;
}

bool Registry::WriteDWORD( char* valueName, unsigned long& buffer )
{
	if( m_key == 0 )
	{
		return false;
	}

	return ( ::RegSetValueEx( m_key, valueName, 0, REG_DWORD, ( LPBYTE )&buffer,
				sizeof( unsigned long ) ) == ERROR_SUCCESS );
}

bool Registry::WriteString( char* valueName, char* buffer )
{
	if( m_key == 0 )
	{
		return false;
	}

	return ( ::RegSetValueEx( m_key, valueName, 0, REG_SZ, ( LPBYTE )buffer,
				( DWORD )::strlen( buffer ) ) == ERROR_SUCCESS );
}