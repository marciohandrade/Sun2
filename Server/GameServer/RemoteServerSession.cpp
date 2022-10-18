
#include "StdAfx.h"
#include "GameServerEx.h"
#include "RemoteServerSession.h"




RemoteServerSession::RemoteServerSession()
{
	// ���� Ǯ�� ������ ���� ����ȴ�.
	m_WorkLoadPacketStream.Initialize( WorkLoadPacketStream::c_DefaultUpdateTick, WorkLoadPacketStream::c_DefaultLimitTransferSize );
	m_WorkLoad.Initialize();
}

RemoteServerSession::~RemoteServerSession()
{
	// �Ƹ� ���� Ǯ�� ������ ��츸 ����� ��.
	m_WorkLoadPacketStream.Initialize( WorkLoadPacketStream::c_DefaultUpdateTick, WorkLoadPacketStream::c_DefaultLimitTransferSize );
	m_WorkLoad.Release( TRUE );
}

VOID	RemoteServerSession::Init()
{
	ServerSession::Init();

	SetSyncKey( TEMP_SERVER );
}

VOID	RemoteServerSession::Release()
{
	ServerSession::Release();

	SetSyncKey( TEMP_SERVER );
}


//
VOID	RemoteServerSession::OnDisconnect()
{
	ServerSession::OnDisconnect();

	SetSyncKey( TEMP_SERVER );
	m_WorkLoadPacketStream.Initialize( WorkLoadPacketStream::c_DefaultUpdateTick, WorkLoadPacketStream::c_DefaultLimitTransferSize );
	// �̰��� ��� Work���� �����ϴ� ���̹Ƿ� �ùٸ� ��ġ�� ������ ���� ���̴�.
	//WorkLoadManager::ReleaseTaskAll();
	m_WorkLoad.Release( TRUE );
}

VOID	RemoteServerSession::OnLogString( CHAR* pszLog )
{
	switch( g_pGameServer->GetServerType() )
	{
	case FIELD_SERVER:
		SUNLOG( eCRITICAL_LOG, "[ServerSession:Field] %s", pszLog );
		break;
	case BATTLE_SERVER:
		SUNLOG( eCRITICAL_LOG, "[ServerSession:Battle] %s", pszLog );
		break;
	default:
		ASSERT(0);
	}	
}


#ifdef __WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__
BOOL	RemoteServerSession::IsValidObject( DWORD dwCheckValue )
{
	return (BOOL)(m_dwSyncKey == dwCheckValue);
}

BOOL	RemoteServerSession::IsValidState( DWORD dwStatusValue )
{
	DWORD status = (DWORD)g_pGameServer->GetPhaseStatus();
	return (BOOL)(status == dwStatusValue);
}

BOOL	RemoteServerSession::SendPacket4Interface( MSG_BASE* pMsg, WORD wSize )
{
	return Send( (BYTE*)pMsg, wSize );
}
#endif //__WAVERIX_SOLAR_AUTH_SERVER_IMPLEMENTATION__


