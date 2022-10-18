#include <eXtreme/Common.h>
#include <eXtreme/eXtreme.h>
#include <eXtreme/io/Pipe.h>

#define DEFAULT_READBUFFER_SIZE		4096
#define DEFAULT_WRITEBUFFER_SIZE	4096

using namespace eXtreme;

Pipe::Pipe() :
	m_handlePipe( INVALID_HANDLE_VALUE )
{
	m_pipeName[0] = 0;
}

Pipe::~Pipe()
{
	Close();
}

bool Pipe::Create( char* name )
{
	if( m_pipeName[0] != 0 )
	{
		return false;
	}

	m_handlePipe = ::CreateNamedPipe(	name,
										PIPE_ACCESS_DUPLEX,
										PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT,
										PIPE_UNLIMITED_INSTANCES,
										DEFAULT_WRITEBUFFER_SIZE,
										DEFAULT_READBUFFER_SIZE,
										INFINITE,
										0 );
	if( m_handlePipe == INVALID_HANDLE_VALUE )
	{
		return false;
	}

	::strcpy( m_pipeName, name );

	return true;
}

bool Pipe::Open( char* name )
{
	if( m_pipeName[0] != 0 )
	{
		return false;
	}

	m_handlePipe = ::CreateFile(	name,
									GENERIC_READ | GENERIC_WRITE,
									FILE_SHARE_READ | FILE_SHARE_WRITE,
									0,
									OPEN_EXISTING,
									FILE_ATTRIBUTE_NORMAL,
									0 );
	if( m_handlePipe == INVALID_HANDLE_VALUE )
	{
		return false;
	}

	::WaitNamedPipe( m_pipeName, NMPWAIT_WAIT_FOREVER );
	::strcpy( m_pipeName, name );

	return true;
}

void Pipe::Close()
{
	if( m_handlePipe == INVALID_HANDLE_VALUE )
	{
		return;
	}

	::FlushFileBuffers( m_handlePipe );
	::DisconnectNamedPipe( m_handlePipe );
	::CloseHandle( m_handlePipe );

	m_handlePipe = INVALID_HANDLE_VALUE;
}

int Pipe::Write( char* buffer, DWORD size )
{
	DWORD	transBytes = 0;

	if( ::WriteFile( m_handlePipe, buffer, size, &transBytes, 0 ) == FALSE )
	{
		DWORD error = ::GetLastError();

		
		if( error == ERROR_BROKEN_PIPE || error == ERROR_NO_DATA )
			return -1;
		else
			return 0;
	}

	return transBytes;
}

int Pipe::Read( char* buffer, DWORD size )
{
	DWORD	transBytes = 0;

	if( ::ReadFile( m_handlePipe, buffer, size, &transBytes, 0 ) == FALSE )
	{
		DWORD error = ::GetLastError();

		if( error == ERROR_BROKEN_PIPE || error == ERROR_NO_DATA )
			return -1;
		else
			return 0;
	}

	return transBytes;
}