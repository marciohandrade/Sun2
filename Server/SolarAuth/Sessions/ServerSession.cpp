
#include "stdafx.h"

#include <ServerGlobal.h>
#include <Protocol_ServerCommon.h>
#include <PacketStruct_ServerCommon.h>

#include <ServerFrame.h>
#include "ServerSession.h"
//#include "ServerSessionExManager.h"

#include "PacketHandler/HandlerFromMasterServer.h"
#include "PacketHandler/HandlerFromTempServer.h"

ServerSessionEx::ServerSessionEx()
{
	//_Initialize();
}

ServerSessionEx::~ServerSessionEx()
{
}

VOID	ServerSessionEx::_Initialize()
{
	Services().RegisterPacketHandler();
	Services().Init();
}

//
VOID	ServerSessionEx::_Release()
{
	// _Initialize와 유사하다 정확한 설정사항을 확인할 것.
	Services().Init();
}

VOID	ServerSessionEx::Clear()
{
	ASSERT( !"여기로 들어오는 것은 뭐니?" );
}

//VOID	ServerSessionEx::OnRedirect()
//{
//	GetFrame()->AddSession( GetSessionIndex(),  this );
//	MessageOut( eCRITICAL_LOG, _T("Redirect %s\n"), GetServerType2String( GetServerType() ) );
//}

VOID	ServerSessionEx::OnDisconnect()
{
	// 이것은 모든 Work들을 제거하는 것이므로 올바른 위치에 사용되지 않은 것이다.
	//WorkLoadManager::ReleaseTaskAll();
	Services().GetWorkLoadPacketStream().Initialize(
												WorkLoadPacketStream::c_DefaultUpdateTick,
												WorkLoadPacketStream::c_DefaultLimitTransferSize
												);
	Services().GetWorkLoadContainer().Release( TRUE );

	ServerSession::OnDisconnect();
}

VOID ServerSessionEx::OnLogString( char* pszLog )
{
	SUNLOG( eCRITICAL_LOG, "[ServerSessionEx] %s", pszLog );
}

VOID ServerSessionEx::OnRecv( BYTE *pMsg, WORD wSize )
{
	Services().GetPacketHandler()->ParsePacket( GetServerType(), this, (MSG_BASE*)pMsg, wSize );
}

//
//-------------------------------------------------------------------------------------------------

// <MOVE to PacketHandler>
//VOID	ServerSessionEx::RegisterPacketHandler()

//-------------------------------------------------------------------------------------------------
// SeverSession Interfaces
//BOOL ServerSessionEx::Send( BYTE* pMsg, WORD wSize )
//{
//	m_WorkLoadPacketStream.AddTransferSize( (DWORD)wSize );
//	return NetworkObject::Send( pMsg, wSize );
//}






