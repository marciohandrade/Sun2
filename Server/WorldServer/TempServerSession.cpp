#include "stdafx.h"
#include "TempServerSession.h"
#include <Macro.h>
#include "PacketStruct.h"
#include "ServerSessionFactory.h"
#include "ServerSessionManager.h"
//#include "PacketHandler.h"

TempServerSession::TempServerSession()
{
	m_bFirstPacketReceived = FALSE;
}

TempServerSession::~TempServerSession()
{
}
VOID TempServerSession::Release()
{
	m_bFirstPacketReceived = FALSE;
}

// �����Ϳ� �����ϴ� �������� �ڽ��� OnAccept�� ȣ����� �ʰ� TempServerSession::OnAccept�� ȣ��ȴ�.
VOID TempServerSession::OnAccept( DWORD dwNetworkIndex )
{
	ServerSession::OnAccept( dwNetworkIndex );
}

VOID TempServerSession::OnRecv( BYTE *pMsg, WORD wSize )
{
    __UNUSED(wSize);
	assert( m_bFirstPacketReceived == FALSE );
	m_bFirstPacketReceived = TRUE;

	// Ǯ�κ��� NetworkObject�� �޾� �´�.

	MSG_SERVER_TYPE *pRecvMsg = (MSG_SERVER_TYPE *)pMsg;
	ServerSession *pNewSession = ServerSessionFactory::Instance()->AllocServerSession( (eSERVER_TYPE)pRecvMsg->m_byServerType );

	// ���ο� NetworkObject�� ��ü
	Redirect( pNewSession );

	/*
	// ���� ���� ���̺��� IP�� ���� Ÿ������ ������ ã�Ƽ� �� Ű�� �����Ѵ�.
	DWORD dwServerKey = ServerInfoManager::Instance()->GetServerKey( (eSERVER_TYPE)pRecvMsg->m_byServerType, GetIP() );
	pNewSession->SetServerKey( dwServerKey );
	*/

	// ���ο� ���ǿ� ���� ����
	pNewSession->SetSessionIndex( GetSessionIndex() );
	pNewSession->OnRedirect();

	// TempServer �����ϰ�
	this->Release();
	ServerSessionManager::Instance()->RemoveServer( this->GetSessionIndex() );
	ServerSessionFactory::Instance()->FreeServerSession( this );

	// ���� ����� ������ �Ŵ����� �߰�
	ServerSessionManager::Instance()->AddServer( pNewSession );
}