#include "stdafx.h"
#include <ServerGlobal.h>
#include "ServerSession.h"
#include "ServerFrame.h"
#include <PacketStruct.h>
#include <PacketStruct_ServerCommon.h>
#include <Protocol_ServerCommon.h>


ServerSession::ServerSession(void)
{
	Clear();
}

ServerSession::~ServerSession(void)
{
}

void ServerSession::Clear()
{
	m_dwSessionIndex	= 0;
	m_wConnectPort		= 0;		//< ������ PORT
    connected_ = false;    //< ����Ǿ����� ����
    is_connector_ = false; //< ������ Ư���� �������ΰ�?
	manual_connect_mode_ = true;
	m_HeartBeatTimer.SetTimer(KEEP_ALIVE_DEFAULT_TIME);
	m_HeartBeatTimer.DisableCheckTime();

	connecting_ = false;
}

void ServerSession::Init()
{
	Clear();
}

void ServerSession::Release()
{
	Clear();
}

void ServerSession::Update()
{
	if( IsConnected() && IsConnector() )
	{
		if( m_HeartBeatTimer.IsExpired() )
		{
			SendHeartbeatPacket();
		}
	}
	else
	{

	}
}

void ServerSession::OnConnect( BOOL bSuccess, DWORD index )
{
	NetworkObject::OnConnect( bSuccess, index );

	manual_connect_mode_ = false;		//< ���� �õ��� �̹� �ߴ�(Connect()�� ȣ���)�� �ǹ��̹Ƿ�, FALSE
	connecting_ = false;		//< ���� �õ��� �����ٴ� �ǹ�

	if( bSuccess )
	{
		OnConnectSuccess( index );
		GetFrame()->SendServerType( this );
	}
	else
	{
	}
}

void ServerSession::OnConnectSuccess( DWORD index )
{
	GetFrame()->RemoveConnectionlessConnector( GetSessionIndex() );

	SetConnected(true);				//< ����Ǿ���.
	SetIsConnector(true);				//< ������
	SetSessionIndex( index );

	GetFrame()->AddSession( GetSessionIndex(), this );

	m_HeartBeatTimer.Reset();

	SUNLOG( eCRITICAL_LOG, "[OnConnect][%u] %s Connect Success!", index, GetServerType2String(GetServerType()) );
}

// CHANGES: f110617.1L, the sub-sessions can control first_contact_msg as the needs
void ServerSession::OnRedirect(DWORD session_index, MSG_SERVER_TYPE* const first_contact_msg)
{
    __UNUSED((first_contact_msg)); // default do not control this argument.
    SetConnected(true);
    SetIsConnector(false);
    SetSessionIndex(session_index);
    assert(session_index == GetSessionIndex());
    m_pFrame->AddSession(session_index, this);

    SUNLOG(eCRITICAL_LOG, _T("[OnRedirect][%u] %s[%s] Redirect!"),
        session_index, GetServerType2String(GetServerType()), GetIP());
}

void ServerSession::OnAccept( DWORD index )
{
	NetworkObject::OnAccept( index );

	SetConnected(true);
	SetIsConnector(false);
	SetSessionIndex( index );

	GetFrame()->AddSession( GetSessionIndex(), this );
	
	if( GetServerType() != CLIENT_SERVER )
	{
		SUNLOG( eCRITICAL_LOG, "[OnAccept][%u] %s[%s] Accept Success!", index, GetServerType2String(GetServerType()), GetIP() );
	}
}

void ServerSession::OnDisconnect()
{
	if( GetServerType() != CLIENT_SERVER )
	{
		SUNLOG( eCRITICAL_LOG, "[OnDisconnect][%u] %s[%s] Disconnect!", GetSessionIndex(), 
			GetServerType2String(GetServerType()), IPOperation::ConvertDWORD2StrIP(GetTempIPAddress()) );
	}

	NetworkObject::OnDisconnect();

	m_HeartBeatTimer.DisableCheckTime();

	SetConnected(false);

	GetFrame()->RemoveSession( GetSessionIndex() );


	if( IsConnector() )
		GetFrame()->AddConnectionlessConnector(this);
}

void ServerSession::SendHeartbeatPacket()
{
	/*
	MSG_HEARTBEAT msg;
	msg.m_byCategory		= 0;
	msg.m_byProtocol		= SERVERCOMMON_HEARTBEAT;
	SendPacket( &msg, sizeof(msg) );
	*/
}

void ServerSession::OnLogString( char *pszLog )
{
	SUNLOG( eCRITICAL_LOG, "[SessionLog] Server[%s] %s", GetServerType2String(GetServerType()), pszLog );
}
