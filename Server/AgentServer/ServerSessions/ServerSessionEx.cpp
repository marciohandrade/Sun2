#include "stdafx.h"

#include <PacketStruct.h>
#include <PacketStruct_ServerCommon.h>
#include <Protocol_ServerCommon.h>

#include "ServerSessionEx.h"
#include <PacketHandler/PacketHandler.h>
#include <AgentServer.h>

ServerSessionEx::ServerSessionEx(void)
{
	// 세션 풀을 설정할 때만 수행된다.
	m_WorkLoadPacketStream.Initialize( WorkLoadPacketStream::c_DefaultUpdateTick, WorkLoadPacketStream::c_DefaultLimitTransferSize );
	m_WorkLoad.Initialize();
}

ServerSessionEx::~ServerSessionEx(void)
{
	// 아마 세션 풀을 해제할 경우만 수행될 듯.
	m_WorkLoadPacketStream.Initialize( WorkLoadPacketStream::c_DefaultUpdateTick, WorkLoadPacketStream::c_DefaultLimitTransferSize );
	m_WorkLoad.Release( TRUE );
}

VOID	ServerSessionEx::Init()
{
	SetSyncKey( TEMP_SERVER );
	ServerSession::Init();
}

VOID	ServerSessionEx::Release()
{
	SetSyncKey( TEMP_SERVER );
	m_WorkLoadPacketStream.Initialize( WorkLoadPacketStream::c_DefaultUpdateTick, WorkLoadPacketStream::c_DefaultLimitTransferSize );
	// 이것은 모든 Work들을 제거하는 것이므로 올바른 위치에 사용되지 않은 것이다.
	//WorkLoadManager::ReleaseTaskAll();
	m_WorkLoad.Release( TRUE );

	ServerSession::Release();
}

VOID	ServerSessionEx::Update()
{
	ServerSession::Update();
}

// CHANGES: f110617.1L
void ServerSessionEx::OnRedirect(DWORD network_index, MSG_SERVER_TYPE* const first_contact_msg)
{
    ServerSession::OnRedirect(network_index, first_contact_msg);
}

VOID	ServerSessionEx::OnRecv( BYTE* pMsg, WORD wSize )
{
	MSG_BASE* pMsgBASE = (MSG_BASE*)pMsg;

	PacketHandler::GetHandler()->ParsePacket( GetServerType(), this, pMsgBASE, wSize );
}

VOID	ServerSessionEx::OnDisconnect()
{
	ServerSession::OnDisconnect();
	GetWorkLoadContainer().Release( TRUE );
}

VOID	ServerSessionEx::OnLogString( CHAR* pszLog )
{
	const TCHAR* pServerString = GetServerType2String( GetServerType() );
	SUNLOG( eCRITICAL_LOG, _T("NetworkLog [%s] %s"), pServerString, pszLog );
}

//VOID ServerSessionEx::SendServerType()
//{
//	MSG_SERVER_TYPE msg;
//	msg.m_byCategory = SERVERCOMMON;
//	msg.m_byProtocol = SERVERCOMMON_SERVERTYPE;
//	msg.m_byServerType = GUILD_SERVER;
//	Send( (BYTE*)&msg, sizeof(MSG_SERVER_TYPE) );
//}


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
	DWORD status = (DWORD)AgentServer::GetInstance()->GetPhaseStatus();
	return (BOOL)(status == dwStatusValue);
}

BOOL	ServerSessionEx::SendPacket4Interface( MSG_BASE* pMsg, WORD wSize )
{
	return SendPacket( pMsg, wSize );
}
