#include "ClientSocket.h"

ClientSocket::ClientSocket()
	: m_socketValue( INVALID_SOCKET )
{
	WSADATA		wsaData;

	::WSAStartup( 0x202, &wsaData );
}

ClientSocket::~ClientSocket()
{
}

bool ClientSocket::Connect( LPCTSTR ipAddress, u_short portNo )
{
	if( m_socketValue != INVALID_SOCKET )
		return false;

	//  Socket 생성
	m_socketValue = ::socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
	if( m_socketValue == INVALID_SOCKET )
		return false;


	//  연결 시도
	SOCKADDR_IN		sockAddr;

	sockAddr.sin_family			= AF_INET;
	sockAddr.sin_port			= ::htons( portNo );
	sockAddr.sin_addr.s_addr	= ::inet_addr( ipAddress );

	if( sockAddr.sin_addr.s_addr == INADDR_NONE )
	{
		//  문자로 구성된 주소를 ip 번호로 변환한다.
		LPHOSTENT	host;

		host = ::gethostbyname( ipAddress );
		if( host != 0 )
			sockAddr.sin_addr.s_addr = ( ( LPIN_ADDR )host->h_addr )->s_addr;
		else
			return false;
	}

	if( ::connect( m_socketValue, ( struct sockaddr* )&sockAddr, sizeof( sockAddr ) ) == SOCKET_ERROR )
	{
		DWORD	error = ::GetLastError();

		if( error != WSA_IO_PENDING && error != WSAEWOULDBLOCK )
			return false;
	}

	return true;
}

void ClientSocket::Close()
{
	if( m_socketValue == INVALID_SOCKET )
		return;

	::closesocket( m_socketValue );
	m_socketValue = INVALID_SOCKET;
}

int ClientSocket::Send( LPVOID buffer, int size )
{
	return ::send( m_socketValue, ( const char* )buffer, size, 0 );
}

int ClientSocket::Recv( LPVOID buffer, int size )
{
	return ::recv( m_socketValue, ( char* )buffer, size, 0 );
}