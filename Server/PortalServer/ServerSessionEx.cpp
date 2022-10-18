#include "stdafx.h"
#include <ServerGlobal.h>
#include "ServerSessionEx.h"
#include <PacketStruct.h>
#include <PacketStruct_ServerCommon.h>
#include <Protocol_ServerCommon.h>

#include "PortalServer.h"

ServerSessionEx::ServerSessionEx(void)
{
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
	// �̰��� ��� Work���� �����ϴ� ���̹Ƿ� �ùٸ� ��ġ�� ������ ���� ���̴�.
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
