#include <eXtreme/Common.h>
#include <eXtreme/eXtreme.h>
#include <eXtreme/util/Stream.h>

using namespace eXtreme;

Stream::Stream( int size )
	: m_dataSize( 0 ), m_bufferSize( size )
{
	if( size == STREAM_DEFAULT_SIZE )
	{
		m_buffer = &m_defaultBuffer[0];
	}
	else
	{
		m_buffer = _new_dbg_ char [size];
	}

	memset(m_buffer, 0, size);
	m_readPos	= &m_buffer[0];
	m_endPos	= &m_buffer[m_bufferSize - 1];
}

Stream::Stream( Stream& stream )
{
	if( stream.m_bufferSize == STREAM_DEFAULT_SIZE )
		m_buffer = &m_defaultBuffer[0];
	else
		m_buffer = _new_dbg_ char [stream.m_bufferSize];

	m_dataSize		= stream.m_dataSize;
	m_bufferSize	= stream.m_bufferSize;
	m_readPos		= &m_buffer[0];
	m_endPos		= &m_buffer[m_bufferSize - 1];

	Write( stream.m_buffer, m_dataSize );
}

Stream::Stream( char* buffer, int len )
	: m_dataSize( 0 )
{
	if( len <= STREAM_DEFAULT_SIZE )
	{
		m_buffer		= &m_defaultBuffer[0];
		m_bufferSize	= STREAM_DEFAULT_SIZE;
	}
	else
	{
		m_buffer		= _new_dbg_ char [len];
		m_bufferSize	= len;
	}

	m_readPos	= &m_buffer[0];
	m_endPos	= &m_buffer[m_bufferSize - 1];

	Write( buffer, len );
}

Stream::~Stream()
{
	if( m_buffer != &m_defaultBuffer[0] )
		delete [] m_buffer;
}

void Stream::Clear( int resize )
{
	m_dataSize	= 0;
	m_readPos	= &m_buffer[0];
	m_endPos	= &m_buffer[m_bufferSize - 1];

	if( resize > 0 && m_bufferSize != (u_long)resize )
	{
		if( m_buffer != &m_defaultBuffer[0] )
			delete [] m_buffer;

		if( resize < STREAM_DEFAULT_SIZE )
			m_buffer = &m_defaultBuffer[0];
		else
			m_buffer = _new_dbg_ char [resize];

		m_bufferSize	= resize;
		m_readPos		= &m_buffer[0];
		m_endPos		= &m_buffer[m_bufferSize - 1];
	}

	memset(m_buffer, 0, m_bufferSize);
}

bool Stream::Write( void* source, int size )
{
	if( m_dataSize + size > m_bufferSize )
	{
		return false;
	}

	::memcpy( ( m_buffer + m_dataSize ), source, size );
	m_dataSize += size;

	return true;
}

bool Stream::Read( void* dest, int size )
{
	if( m_readPos + size > m_endPos )
	{
		return false;
	}

	::memcpy( dest, m_readPos, size );
	m_readPos += size;

	return true;
}

Stream&	Stream::operator =  ( Stream& stream )
{
	Write( stream.m_buffer, stream.m_dataSize );

	return *this;
}

Stream&	Stream::operator << ( Stream& stream )
{
	Write( ( void* )stream.m_buffer, stream.m_dataSize );

	return *this;
}

Stream&	Stream::operator << ( char* arg )
{
	Write( ( void* )arg, ( int )::strlen( ( char* )arg ) + 1 );

	return *this;
}

Stream&	Stream::operator << ( const char* arg )
{
	Write( ( void* )arg, ( int )::strlen( ( char* )arg ) + 1 );

	return *this;
}

Stream&	Stream::operator << ( unsigned char* arg )
{
	Write( ( void* )arg, ( int )::strlen( ( char* )arg ) + 1 );

	return *this;
}

Stream&	Stream::operator << ( const unsigned char* arg )
{
	Write( ( void* )arg, ( int )::strlen( ( char* )arg ) + 1 );

	return *this;
}

Stream&	Stream::operator >> ( char* arg )
{
	Read( ( void* )arg, ( int )::strlen( m_readPos ) + 1 );

	return *this;
}

Stream&	Stream::operator >> ( const char* arg )
{
	Read( ( void* )arg, ( int )::strlen( m_readPos ) + 1 );

	return *this;
}

Stream&	Stream::operator >> ( unsigned char* arg )
{
	Read( ( void* )arg, ( int )::strlen( m_readPos ) + 1 );

	return *this;
}

Stream&	Stream::operator >> ( const unsigned char* arg )
{
	Read( ( void* )arg, ( int )::strlen( m_readPos ) + 1 );

	return *this;
}