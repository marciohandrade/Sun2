#include "stdafx.h"
#include "Web.h"
#include "ManagerWeb.h"
#include "Database.h"

#include "entry.h"

Web::Web()
	: eXtreme::ServerSession( 2048, 2048 ), m_tickAlive(0), m_bConnect(false)
{
}

Web::~Web()
{
}


void Web::OnAccept()
{
	m_tickAlive = ::GetTickCount();
	ManagerWeb::Instance()->AddClient( this );
	OutputDebug( "Accepted Web component from %s\n", GetIPAddress() );

	m_bConnect = true;
}

void Web::OnClose( DWORD error )
{
	ManagerWeb::Instance()->RemoveClient( this );
	OutputDebug( "Closed connection with Web component(%s, err=%d)\n", GetIPAddress(), error );
	
	m_tickAlive = 0;
	m_bConnect = false;
}

void Web::Update()
{
	//  �ֱ����� ������ ���� RTTAgent�� ������ �����Ų��.
	if( (0 != m_tickAlive) && (::GetTickCount() - m_tickAlive > ALIVETICK_LIMITE) )
	{
		OutputDebug( "No more response from Web(%s)\n", GetIPAddress() );
		SetKill();
	}

	eXtreme::ServerSession::Update();
}
void Web::SendMessage( MSG_HEADER8& packet, int size )
{
	if( m_bConnect )
	{
		//  ������ ������ Overflow ���θ� �˻��Ѵ�.
		{
			DWORD	remainSize = 0, maxSize = 0;

			GetSendBufferSize( &remainSize, &maxSize );
			if( remainSize + size >= maxSize )
			{
				//  ������ ���ۿ� ���� �� ���ٸ�, ������ ������ ���� �۽��Ͽ�
				//  ���۸� ��쵵�� �Ѵ�.
				Flush();
			}
		}

		if( size == 0 )
		{
			ServerSession::SendPacket( &packet, packet.dwSize + sizeof( MSG_HEADER8 ) );
		}
		else
		{
			ServerSession::SendPacket( &packet, size );
		}
	}
	else	// ������ �ȵǾ� �ִ� ���
	{
		OutputDebug( "Already disconnected Web(%s)\n", GetIPAddress() );
	}
}

void Web::OnReceive( char* buffer, int size )
{
	MSG_HEADER8*		header = ( MSG_HEADER8* )buffer;

	m_tickAlive = ::GetTickCount();

	switch( header->dwID )
	{
	case  RTTH_DROPUSER_REQ:			OnRTTHDropUserReq( buffer, size );			break;	
	default:
		OutputDebug( "[Web Session] Not defined MessageID(=0x%x)\n", header->dwID );
		break;
	}
}

void Web::OnRTTHDropUserReq( char* buffer, int /*size*/ )
{
	MSG_RTTH_DROPUSER_REQ*	recvPacket = ( MSG_RTTH_DROPUSER_REQ* )buffer;
	eXtreme::Stream			stream( 4096 );

	RTTAgent* rttAgent = ManagerClient::Instance()->FindClient( SERVERTYPE_AUTH );
	if( rttAgent == 0 )		//  ���� ������ ���ӵ��� �ʾҴ�.
	{
		MSG_RTTH_DROPUSER_ANS	sendPacket;
		sendPacket.dwError = 2;	//  ������ ��� ����
		SendMessage( sendPacket );

		OutputDebug("[Wops::OnRTTHDropUserReq] RTTAgent not found\n");
		return;
	}

	stream << ( DWORD )( 4 + sizeof( *recvPacket ) ) << ( DWORD )RTTH_DROPUSER_REQ << ( DWORD )GetUID();
	stream.Write( recvPacket, sizeof( *recvPacket ) );

	rttAgent->SendPacket( stream );

	OutputDebug("[Web::OnRTTHDropUserReq] Session UID : %d\n", ( DWORD )GetUID() );
}