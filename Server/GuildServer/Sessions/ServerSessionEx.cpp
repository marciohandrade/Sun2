#include "stdafx.h"

#include <PacketStruct.h>
#include <PacketStruct_ServerCommon.h>
#include <Protocol_ServerCommon.h>

#include "ServerSessionEx.h"
#include <PacketHandler/PacketHandler.h>
#include <GuildServer.h>

ServerSessionEx::ServerSessionEx(void)
{
	Clear();

	// ���� Ǯ�� ������ ���� ����ȴ�.
	m_WorkLoadPacketStream.Initialize( WorkLoadPacketStream::c_DefaultUpdateTick, WorkLoadPacketStream::c_DefaultLimitTransferSize );
	m_WorkLoad.Initialize();
}

ServerSessionEx::~ServerSessionEx(void)
{
	// �Ƹ� ���� Ǯ�� ������ ��츸 ����� ��.
	m_WorkLoadPacketStream.Initialize( WorkLoadPacketStream::c_DefaultUpdateTick, WorkLoadPacketStream::c_DefaultLimitTransferSize );
	m_WorkLoad.Release( TRUE );
}

VOID ServerSessionEx::Clear()
{
	//m_dwSessionIndex	= 0;
	//m_wConnectPort		= 0;		//< ������ PORT
	//m_bIsConnected		= FALSE;	//< ����Ǿ����� ����
	//m_bIsConnector		= FALSE;	//< ������ Ư���� �������ΰ�?
	//m_HeartBeatTimer.SetTimer(KEEP_ALIVE_DEFAULT_TIME);
	////m_ReconnectTimer.SetTimer(RECONNECT_DEFAULT_TIME);
	//m_HeartBeatTimer.DisableCheckTime();
	////m_ReconnectTimer.DisableCheckTime();

	SetSyncKey( TEMP_SERVER );
}

VOID ServerSessionEx::Init()
{
}

VOID ServerSessionEx::Release()
{

}

VOID ServerSessionEx::Update()
{
	ServerSession::Update();
}

// CHANGES: f110617.1L
void ServerSessionEx::OnRedirect(DWORD network_index, MSG_SERVER_TYPE* const first_contact_msg)
{
    ServerSession::OnRedirect(network_index, first_contact_msg);
}

VOID	ServerSessionEx::OnRecv( BYTE *pMsg, WORD wSize )
{
	MSG_BASE* pMsgBASE = (MSG_BASE*)pMsg;

	PacketHandler::GetHandler()->ParsePacket( GetServerType(), this, (MSG_BASE*)pMsg, wSize );
}

VOID	ServerSessionEx::OnDisconnect()
{
	GetWorkLoadContainer().Release( TRUE );
	ServerSession::OnDisconnect();
}

VOID ServerSessionEx::SendHeartbeatPacket()
{
	//MSG_HEARTBEAT msg;
	//msg.m_byCategory		= 0;
	//msg.m_byProtocol		= SERVERCOMMON_HEARTBEAT;
	//Send( (BYTE*)&msg, sizeof(MSG_HEARTBEAT) );
}

BOOL ServerSessionEx::SendPacket( MSG_BASE * pMsg, WORD wSize )
{
	return Send( (BYTE*)pMsg, wSize );
}

BOOL	ServerSessionEx::IsValidObject( DWORD dwCheckValue )
{
	return (BOOL)(m_dwSyncKey == dwCheckValue);
}

BOOL	ServerSessionEx::IsValidState( DWORD dwStatusValue )
{
	DWORD status = (DWORD)GuildServer::GetMainFrame()->GetPhaseStatus();
	return (BOOL)(status == dwStatusValue);
}

BOOL	ServerSessionEx::SendPacket4Interface( MSG_BASE* pMsg, WORD wSize )
{
	return SendPacket( pMsg, wSize );
}
