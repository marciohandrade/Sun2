#include "stdafx.h"
#include <stdio.h>
#include "ServerSession.h"
#include "WorldServer.h"
#include "ServerSessionManager.h"
#include <Protocol_ServerCommon.h>
#include <PacketStruct_ServerCommon.h>

#include <ServerGlobal.h>
#include <WorldServerInfoParser.h>

ServerSession::ServerSession()
{
#ifdef __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
	// 세션 풀을 설정할 때만 수행된다.
	m_WorkLoadPacketStream.Initialize( WorkLoadPacketStream::c_DefaultUpdateTick, WorkLoadPacketStream::c_DefaultLimitTransferSize );
	m_WorkLoad.Initialize();
#endif //__WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
}

ServerSession::~ServerSession()
{
}

VOID ServerSession::Init()
{
	m_dwSessionIndex			= 0;
	m_dwServerKey				= 0;
	m_wConnectPort				= 0;
	m_bForConnect				= FALSE;
	m_dwLastHeartbeatTick		= GetTickCount();
	m_bConnection				= FALSE;
	m_strConnectIP.clear();

#ifdef __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
	SetSyncKey( TEMP_SERVER );
#endif
}

VOID ServerSession::Release()
{
#ifdef __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
	// 아마 세션 풀을 해제할 경우만 수행될 듯.
	m_WorkLoadPacketStream.Initialize( WorkLoadPacketStream::c_DefaultUpdateTick, WorkLoadPacketStream::c_DefaultLimitTransferSize );
	m_WorkLoad.Release( TRUE );
#endif
}

VOID ServerSession::Update()
{
	if( IsForConnect() )
	{
		// heartbeat 보내기
		DWORD dwCurTick = GetTickCount();
		if( dwCurTick - m_dwLastHeartbeatTick > 10000 )
		{
			m_dwLastHeartbeatTick = dwCurTick;

			/*MSG_HEARTBEAT msg;
			msg.m_byCategory		= 0;
			msg.m_byProtocol		= SERVERCOMMON_HEARTBEAT;
			Send( (BYTE*)&msg, sizeof(MSG_HEARTBEAT) );*/
		}
	}
}

VOID ServerSession::OnConnect( BOOL bSuccess, DWORD dwNetworkIndex )
{
	if( bSuccess )
	{
		SetForConnect( TRUE );
		SetSessionIndex( dwNetworkIndex );
		ServerSessionManager::Instance()->AddServer( this );
		m_bConnection = TRUE;
		SendServerType();
	}
}

VOID ServerSession::OnAccept( DWORD dwNetworkIndex )
{
	SetForConnect( FALSE );
	SetSessionIndex( dwNetworkIndex );
	m_bConnection = TRUE;
#ifndef __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__

	ServerSessionManager::Instance()->AddServer( this );
	MessageOut( eCRITICAL_LOG, "[OnAccept][%u] Accept Server Success(%s) \n", dwNetworkIndex, GetServerType2String(GetServerType()) );

#else //__WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__

	ServerSessionManager::Instance()->AddServer( this );
	if( ! ( WorldServerInfoParser::Instance()->IsServerInfoFromFile() ) )
	{
		if( WorldServer::Instance()->GetPhaseStatus() == PHASE_SERVICE )
			MessageOut( eCRITICAL_LOG, "[OnAccept][%u] Accept Server Success(%s) \n", dwNetworkIndex, GetServerType2String(GetServerType()) );
		else
			this->Disconnect();
	}
	else
	{
		MessageOut( eCRITICAL_LOG, "[OnAccept][%u] Accept Server Success(%s) \n", dwNetworkIndex, GetServerType2String(GetServerType()) );
	}

#endif //__WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
}

VOID ServerSession::OnDisconnect()
{
	ServerSessionManager::Instance()->RemoveServer( GetSessionIndex() );
	m_bConnection = FALSE;

#ifdef __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
	SetSyncKey( TEMP_SERVER );
	m_WorkLoadPacketStream.Initialize( WorkLoadPacketStream::c_DefaultUpdateTick, WorkLoadPacketStream::c_DefaultLimitTransferSize );
	// 이것은 모든 Work들을 제거하는 것이므로 올바른 위치에 사용되지 않은 것이다.
	//WorkLoadManager::ReleaseTaskAll();
	m_WorkLoad.Release( TRUE );
#endif
}

VOID ServerSession::SetAddr( char *pszIP, WORD wPort )
{
	m_strConnectIP = pszIP;
	m_wConnectPort = wPort;
}

VOID ServerSession::SendServerType()
{
	MSG_SERVER_TYPE msg;
	msg.m_byCategory	= SERVERCOMMON;
	msg.m_byProtocol	= SERVERCOMMON_SERVERTYPE;
	msg.m_byServerType	= WORLD_SERVER;

	Send( (BYTE*)&msg, sizeof(MSG_SERVER_TYPE) );
}

#ifdef __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
BOOL	ServerSession::IsValidObject( DWORD dwCheckValue )
{
	return (BOOL)(m_dwSyncKey == dwCheckValue);
}

BOOL	ServerSession::IsValidState( DWORD dwStatusValue )
{
	DWORD status = (DWORD)WorldServer::Instance()->GetPhaseStatus();
	return (BOOL)(status == dwStatusValue);
}

BOOL	ServerSession::SendPacket4Interface( MSG_BASE* pMsg, WORD wSize )
{
	return Send( (BYTE*)pMsg, wSize );
}
#endif

VOID ServerSession::OnLogString( char *pszLog )
{
	SUNLOG( eCRITICAL_LOG, "[SessionLog]Type:%d,%s", GetServerType(), pszLog );
}










