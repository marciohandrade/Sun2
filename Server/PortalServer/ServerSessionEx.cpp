#include "stdafx.h"
#include <ServerGlobal.h>
#include "ServerSessionEx.h"
#include <PacketStruct.h>
#include <PacketStruct_ServerCommon.h>
#include <Protocol_ServerCommon.h>

#include "PortalServer.h"

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

VOID ServerSessionEx::Init()
{
	ServerSession::Init();
	SetSyncKey( TEMP_SERVER );
}

VOID ServerSessionEx::Release()
{
	ServerSession::Release();
	SetSyncKey( TEMP_SERVER );
}



VOID ServerSessionEx::OnDisconnect()
{
	ServerSession::OnDisconnect();

	SetSyncKey( TEMP_SERVER );
	m_WorkLoadPacketStream.Initialize( WorkLoadPacketStream::c_DefaultUpdateTick, WorkLoadPacketStream::c_DefaultLimitTransferSize );
	// 이것은 모든 Work들을 제거하는 것이므로 올바른 위치에 사용되지 않은 것이다.
	//WorkLoadManager::ReleaseTaskAll();
	m_WorkLoad.Release( TRUE );
}



BOOL	ServerSessionEx::IsValidObject( DWORD dwCheckValue )
{
	return (BOOL)(m_dwSyncKey == dwCheckValue);
}

BOOL	ServerSessionEx::IsValidState( DWORD dwStatusValue )
{
	DWORD status = (DWORD)g_PortalServer.GetPhaseStatus();
	return (BOOL)(status == dwStatusValue);
}

BOOL	ServerSessionEx::SendPacket4Interface( MSG_BASE* pMsg, WORD wSize )
{
	return SendPacket( pMsg, wSize );
}
