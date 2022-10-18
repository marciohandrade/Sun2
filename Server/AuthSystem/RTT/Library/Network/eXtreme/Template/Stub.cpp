#include "stdafx.h"
#include "Stub.h"

#define  LIMITE_CONNECTTICK		3000

//  Connection state
#define  CS_CLOSED				0x0001
#define  CS_TRYCONNECT			0x0002
#define  CS_CONNECTED			0x0003

Stub::Stub()
	: eXtreme::ServerSession( 8192, 8192 )
{
	m_disconnTick	= 0;
	m_connectState	= CS_CLOSED;
}

Stub::~Stub()
{
}

void Stub::OnConnect( bool success, DWORD error )
{
	if( success == true )
	{
		m_connectState = CS_CONNECTED;
		printf( "Connected.\n" );
	}
	else
	{
		m_connectState	= CS_CLOSED;
		printf( "Connection failed.\n" );
	}
}

void Stub::OnSend( int size )
{
}

void Stub::OnReceive( char* buffer, int size )
{
}

void Stub::OnClose( DWORD error )
{
	m_connectState	= CS_CLOSED;
	printf( "Closed.\n" );
}

void Stub::Update( bool activate )
{
	eXtreme::ServerSession::Update( activate );

	if( m_connectState != CS_CLOSED )
		return;

	//  연결을 재시도 한다.
	if( ::GetTickCount() - m_disconnTick > LIMITE_CONNECTTICK )
	{
		m_connectState	= CS_TRYCONNECT;
		m_disconnTick	= ::GetTickCount();

		Connect( "127.0.0.1", 10100 );
	}
}