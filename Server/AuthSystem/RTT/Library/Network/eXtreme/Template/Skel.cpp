#include "stdafx.h"
#include "Skel.h"

Skel::Skel()
	: eXtreme::ServerSession( 1024, 1024 )
{
}

Skel::~Skel()
{
}

void Skel::OnAccept()
{
	printf( "Accept\n" );
}

void Skel::OnClose( DWORD error )
{
	printf( "Close\n" );
}

void Skel::Update( bool activate )
{
	eXtreme::ServerSession::Update( activate );

	if( kbhit() != 0 )
		SetKill();
}

void Skel::OnReceive( char* buffer, int size )
{
}