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

// 마스터에 접속하는 서버들은 자신의 OnAccept는 호출되지 않고 TempServerSession::OnAccept만 호출된다.
VOID TempServerSession::OnAccept( DWORD dwNetworkIndex )
{
	ServerSession::OnAccept( dwNetworkIndex );
}

VOID TempServerSession::OnRecv( BYTE *pMsg, WORD wSize )
{
    __UNUSED(wSize);
	assert( m_bFirstPacketReceived == FALSE );
	m_bFirstPacketReceived = TRUE;

	// 풀로부터 NetworkObject를 받아 온다.

	MSG_SERVER_TYPE *pRecvMsg = (MSG_SERVER_TYPE *)pMsg;
	ServerSession *pNewSession = ServerSessionFactory::Instance()->AllocServerSession( (eSERVER_TYPE)pRecvMsg->m_byServerType );

	// 새로운 NetworkObject로 교체
	Redirect( pNewSession );

	/*
	// 서버 정보 테이블에서 IP와 서버 타입으로 서버를 찾아서 그 키를 세팅한다.
	DWORD dwServerKey = ServerInfoManager::Instance()->GetServerKey( (eSERVER_TYPE)pRecvMsg->m_byServerType, GetIP() );
	pNewSession->SetServerKey( dwServerKey );
	*/

	// 새로운 세션에 정보 복사
	pNewSession->SetSessionIndex( GetSessionIndex() );
	pNewSession->OnRedirect();

	// TempServer 정리하고
	this->Release();
	ServerSessionManager::Instance()->RemoveServer( this->GetSessionIndex() );
	ServerSessionFactory::Instance()->FreeServerSession( this );

	// 실제 사용할 세션을 매니저에 추가
	ServerSessionManager::Instance()->AddServer( pNewSession );
}