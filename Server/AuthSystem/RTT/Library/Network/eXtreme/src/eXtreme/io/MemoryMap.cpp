#include <eXtreme/Common.h>
#include <eXtreme/eXtreme.h>
#include <eXtreme/io/MemoryMap.h>

using namespace eXtreme;


MemoryMap::MemoryMap() :
	m_handleFile( INVALID_HANDLE_VALUE ),m_handleMemMap( INVALID_HANDLE_VALUE ),
	m_baseAddr( 0 ), m_size( 0 )
{
}

MemoryMap::~MemoryMap()
{
	Close();
}

bool MemoryMap::Open( char* fileName, char * /*memMapName*/ )
{
	if( m_handleFile != INVALID_HANDLE_VALUE )
	{
		return false;
	}


	m_handleFile = ::CreateFile( fileName, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0 );
	if( m_handleFile == INVALID_HANDLE_VALUE )
	{
		return false;
	}

	m_handleMemMap = ::CreateFileMapping( m_handleFile, 0, PAGE_WRITECOPY, 0, 0, 0 );
	if( m_handleMemMap == 0 )
	{
		return false;
	}

	m_baseAddr = ::MapViewOfFile( m_handleMemMap, FILE_MAP_COPY, 0, 0, 0 );
	if( m_baseAddr == 0 )
	{
		return false;
	}

	m_size = ::GetFileSize( m_handleFile, 0 );

	return true;
}

void MemoryMap::Close()
{
	if( m_handleFile != INVALID_HANDLE_VALUE )
	{
		::CloseHandle( m_handleFile );
		m_handleFile = INVALID_HANDLE_VALUE;
	}

	if( m_handleMemMap != INVALID_HANDLE_VALUE )
	{
		::CloseHandle( m_handleMemMap );
		m_handleMemMap = INVALID_HANDLE_VALUE;
	}

	if( m_baseAddr != 0 )
	{
		::UnmapViewOfFile( m_baseAddr );
		m_baseAddr = 0;
	}

	m_size = 0;
}

bool MemoryMap::Flush()
{
	if( ::FlushViewOfFile( m_baseAddr, 0 ) == FALSE )
	{
		return false;
	}

	return true;
}

void* MemoryMap::GetBaseAddress()
{
	return m_baseAddr;
}

int MemoryMap::GetSize()
{
	return m_size;
}
