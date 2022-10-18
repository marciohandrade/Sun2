#include "stdafx.h"
#include "RTTService.h"
#include "ManagerService.h"
#include "Database.h"

#include "entry.h"

RTTService::RTTService()
	: eXtreme::ServerSession( 2048, 2048 ),
	m_worldID( 0 ), m_channelID( 0 ), m_serverType( 0 ), m_serverID( 0 ), m_tickAlive(0), m_bConnect(false)
{
}

RTTService::~RTTService()
{
}


void RTTService::OnAccept()
{
	m_tickAlive = ::GetTickCount();
	ManagerService::Instance()->AddClient( this );
	OutputDebug( "Accepted RTTService component from %s\n", GetIPAddress() );

	{
		wchar_t	wcs[20] = {0, };

		::MultiByteToWideChar( CP_ACP, 0, GetIPAddress(), -1, wcs, 20 );
		Database::Instance()->FindServerID( wcs, &m_worldID, &m_channelID, &m_serverType, &m_serverID );
	}

	m_bConnect = true;
}

void RTTService::OnClose( DWORD error )
{
	ManagerService::Instance()->RemoveClient( this );
	OutputDebug( "Closed connection with RTTService component(%s, err=%d)\n", GetIPAddress(), error );
	
	m_tickAlive = 0;
	m_bConnect = false;
}

void RTTService::Update()
{
	//  주기적인 응답이 없는 RTTAgent는 강제로 종료시킨다.
	if( (0 != m_tickAlive) && (::GetTickCount() - m_tickAlive > ALIVETICK_LIMITE) )
	{
		OutputDebug( "No more response from RTTService(%s)\n", GetIPAddress() );
		SetKill();
	}

	eXtreme::ServerSession::Update();
}
void RTTService::SendMessage( MSG_HEADER8& packet, int size )
{
	if(m_bConnect) 
	{
		if( size == 0 )
		{
			ServerSession::SendPacket( &packet, packet.dwSize + sizeof( MSG_HEADER8 ) );
		}
		else
		{
			ServerSession::SendPacket( &packet, size );
		}
	}
	else	// 연결이 안되어 있는 경우
	{
		OutputDebug( "disconnected RTTService(%s)\n", GetIPAddress() );
	}
}

void RTTService::SendRTTAStart()
{
	/*
	//  패치폴더 내의 파일목록을 만든다.
	{
		HANDLE			handleFileFind;
		WIN32_FIND_DATA	findData;
		__int64			fileSize;

		handleFileFind = FindFirstFile( "*.*", &findData );
		while( handleFileFind != INVALID_HANDLE_VALUE )
		{
			fileSize = findData.nFileSizeHigh << 32 | findData.nFileSizeLow;

			//  현재 하위 디렉토리 패치는 지원하지 않는다.
			if( findData.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY )
				continue;

			if( FindNextFile( handleFileFind, &findData ) == FALSE )
				break;
		}

		FindClose( handleFileFind );
	}
	*/


	MSG_RTTR_RTTASTART_NTF	sendPacket;
	SendMessage( sendPacket );
	OutputDebug( "Send RTTAStart to %s\n", GetIPAddress() );
}

void RTTService::SendRTTAStop()
{
	MSG_RTTR_RTTASTOP_NTF	sendPacket;

	SendMessage( sendPacket );

	OutputDebug( "Send RTTASTOP to %s\n", GetIPAddress() );
}

void RTTService::SendRTTAPatch()
{
	MSG_RTTR_RTTAPATCH_NTF	sendPacket;

	char szIP[16]		= {0, };
	char szUser[24]		= {0, };
	char szPassword[24]	= {0, };
	char szFilePath[40]	= {0, };
	char szFileName[40]	= {0, };
	
	::GetPrivateProfileString( "RTTAGENT_PATCH_FTP", "IP"		, "127.0.0.1"	, szIP		, 16, SERVERINFO_FILENAME );
	::GetPrivateProfileString( "RTTAGENT_PATCH_FTP", "USER"		, "WopsAdmin"	, szUser	, 24, SERVERINFO_FILENAME );
	::GetPrivateProfileString( "RTTAGENT_PATCH_FTP", "PASSWORD"	, "WopsAdmin"	, szPassword, 24, SERVERINFO_FILENAME );
	::GetPrivateProfileString( "RTTAGENT_PATCH_FTP", "FilePath"	, "."			, szFilePath, 40, SERVERINFO_FILENAME );
	
	sendPacket.dwFtpPort = ::GetPrivateProfileInt( "RTTAGENT_PATCH_FTP", "PORT", 0, SERVERINFO_FILENAME );
	
	sprintf(szFileName, "RTTAgent_0x%04x.zip", Entry::Instance()->GetRTTAgentVersion());

	::MultiByteToWideChar( CP_ACP, 0, szIP		, -1, sendPacket.szFtpAddress		, 16 );
	::MultiByteToWideChar( CP_ACP, 0, szUser	, -1, sendPacket.szFtpUser			, 24 );
	::MultiByteToWideChar( CP_ACP, 0, szPassword, -1, sendPacket.szFtpPassword		, 24 );
	::MultiByteToWideChar( CP_ACP, 0, szFilePath, -1, sendPacket.szFtpGetFilePath	, 40 );
    ::MultiByteToWideChar( CP_ACP, 0, szFileName, -1, sendPacket.szFtpGetFileName	, 40 );
	
	SendMessage( sendPacket );

	OutputDebug( "Send RTTAPatch to %s\n", GetIPAddress() );
}

void RTTService::OnReceive( char* buffer, int size )
{
	MSG_HEADER8*		header = ( MSG_HEADER8* )buffer;

	switch( header->dwID )
	{
	case  RTTR_HEARTBEAT_NTF:			OnRTTRHeartBeatNTF( buffer, size );			break;	
	default:
		OutputDebug( "[RTTService Session] Not defined MessageID(=0x%x)\n", header->dwID );
		break;
	}
}

void RTTService::OnRTTRHeartBeatNTF( char* /*buffer*/, int /*size*/ )
{
	m_tickAlive = ::GetTickCount();
#ifdef _DEBUG
	//OutputDebug( "[RTTService::OnRTTRHeartBeatNTF] (%s)\n", GetIPAddress() );
#endif
}