#include "stdafx.h"
#include <PacketStruct.h>
#include <PacketStruct_DM.h>
#include <Protocol_DM.h>
#include <const.h>
#include "GameDBProxySession.h"
#include "WorldServer.h"
#include "PacketHandler.h"

GameDBProxySession::GameDBProxySession()
{
}

GameDBProxySession::~GameDBProxySession()
{
}

VOID GameDBProxySession::Init()
{
	ServerSession::Init();
}

VOID GameDBProxySession::Release()
{
	ServerSession::Release();
}

VOID GameDBProxySession::Update()
{
	ServerSession::Update();
}

VOID GameDBProxySession::OnRedirect()
{
	MessageOut(eCRITICAL_LOG,  "Game DB Proxy Connected." );
}

VOID GameDBProxySession::OnConnect( BOOL bSuccess, DWORD dwNetworkIndex )
{
	ServerSession::OnConnect( bSuccess, dwNetworkIndex );

	if( bSuccess )
	{
		MessageOut(eCRITICAL_LOG,  "Connected to Game DB Proxy." );
	}
}

VOID GameDBProxySession::OnDisconnect()
{
	ServerSession::OnDisconnect();

	MessageOut(eCRITICAL_LOG,   "Game DB proxy disconnected." );
}

VOID GameDBProxySession::OnRecv( BYTE *pMsg, WORD wSize )
{
	MSG_BASE *pBaseMsg = (MSG_BASE*)pMsg;

	PacketHandler::Instance()->ParsePacket_DW( this, (MSG_BASE*)pMsg, wSize );
}
