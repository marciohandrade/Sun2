#include "stdafx.h"
#include "Entry.h"
#include "RTTServer.h"
#include "PatchRTTA.h"

#include <eXtreme/util/WinService.h>


#define  LIMITE_CONNECTTICK		3000

//  Connection state
#define  CS_CLOSED				0x0001
#define  CS_TRYCONNECT			0x0002
#define  CS_CONNECTED			0x0003


RTTServer::RTTServer()
	: eXtreme::ServerSession( 2048, 2048 )
{
	m_disconnTick	= 0;
	m_connectState	= CS_CLOSED;

	m_dwTicHeartBeat = 0;
}

RTTServer::~RTTServer()
{
}

void RTTServer::OnConnect( bool success, DWORD /*error*/ )
{
	if( success == true )
	{
		GetHandle()->SetBufferSize( 65535, 65535 );
		m_connectState = CS_CONNECTED;
		
		m_dwTicHeartBeat = ::GetTickCount();

		OutputDebug( "Connected to RTTServer.\n" );
	}
	else
	{
		m_connectState	= CS_CLOSED;
		m_disconnTick	= ::GetTickCount();
	}
}

void RTTServer::OnClose( DWORD error )
{
	m_connectState	= CS_CLOSED;
	m_disconnTick	= ::GetTickCount();

	OutputDebug( "Connection closed with RTTServer(%d)\n", error );
}

void RTTServer::CheckDisconnected()
{
	if( m_connectState != CS_CLOSED )
		return;

	//  연결을 재시도 한다.
	if( ::GetTickCount() - m_disconnTick > LIMITE_CONNECTTICK )
	{
		Connect( Entry::Instance()->m_serverIPAddr, ( unsigned short )Entry::Instance()->m_serverPortNo );

		m_connectState	= CS_TRYCONNECT;
		m_disconnTick	= ::GetTickCount();
	}
}

void RTTServer::Update()
{
	CheckDisconnected();
	if( m_connectState != CS_CONNECTED )
	{
		return;
	}
	
	if( ::GetTickCount() - m_dwTicHeartBeat > TIME_HEARTBEAT )
	{
		SendHeartBeat();
		m_dwTicHeartBeat = ::GetTickCount();
	}

	eXtreme::ServerSession::Update();
}

void RTTServer::SendHeartBeat()
{
	MSG_RTTR_HEARTBEAT_NTF sendPacket;
	SendMessage( sendPacket );
}

void RTTServer::SendMessage( MSG_HEADER8& packet, int size )
{
	if( size == 0 )
		ServerSession::SendPacket( &packet, packet.dwSize + sizeof( MSG_HEADER8 ) );
	else
		ServerSession::SendPacket( &packet, size );
}

void RTTServer::OnReceive( char* buffer, int size )
{
	MSG_HEADER8*		header = ( MSG_HEADER8* )buffer;

	switch( header->dwID )
	{
	case  RTTR_RTTASTART_NTF:		OnRTTAStartNTF( buffer, size );			break;
	case  RTTR_RTTASTOP_NTF:		OnRTTAStopNTF( buffer, size );			break;
	case  RTTR_RTTAPATCH_NTF:		OnRTTAPatchNTF( buffer, size );			break;
	}
}

void RTTServer::OnRTTAStartNTF( char* /*buffer*/, int /*size*/ )
{
	DWORD dwResult = eXtreme::WinService::Instance()->StartService( 0, SERVICENAME_RTTA );
	OutputDebug( "RTTA Service starting result = %d\n", dwResult );
}

void RTTServer::OnRTTAStopNTF( char* /*buffer*/, int /*size*/ )
{
	DWORD dwResult = eXtreme::WinService::Instance()->StopService( 0, SERVICENAME_RTTA );
	OutputDebug( "RTTA Service stopping result = %d\n", dwResult );
}

void RTTServer::OnRTTAPatchNTF( char* buffer, int /*size*/ )
{
	MSG_RTTR_RTTAPATCH_NTF*		recvPacket = ( MSG_RTTR_RTTAPATCH_NTF* )buffer;

	char	szFtpAddress[20]		= {0, };
	char	szFtpUser[20]			= {0, };
	char	szFtpPassword[20]		= {0, };
	char	szFtpGetFilePath[40]	= {0, };
	char	szFtpGetFileName[40]	= {0, };	
	
	DWORD dwFtpPort = recvPacket->dwFtpPort;
	::WideCharToMultiByte( CP_ACP, 0, recvPacket->szFtpAddress		, -1, szFtpAddress		, 20, 0, 0 );
	::WideCharToMultiByte( CP_ACP, 0, recvPacket->szFtpUser			, -1, szFtpUser			, 20, 0, 0 );
	::WideCharToMultiByte( CP_ACP, 0, recvPacket->szFtpPassword		, -1, szFtpPassword		, 20, 0, 0 );
	::WideCharToMultiByte( CP_ACP, 0, recvPacket->szFtpGetFilePath	, -1, szFtpGetFilePath	, 40, 0, 0 );
	::WideCharToMultiByte( CP_ACP, 0, recvPacket->szFtpGetFileName	, -1, szFtpGetFileName	, 40, 0, 0 );

	PatchRTTA::Instance()->Start( szFtpAddress, dwFtpPort, szFtpUser, szFtpPassword, szFtpGetFilePath, szFtpGetFileName );
}