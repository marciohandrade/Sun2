#include "stdafx.h"
#include "BattleServerSession.h"
#include "PacketHandler.h"
#include "GameServerSession.h"


BattleServerSession::BattleServerSession()
{
}

BattleServerSession::~BattleServerSession()
{
}

VOID BattleServerSession::Init()
{
	GameServerSession::Init();
}

VOID BattleServerSession::Release()
{
	GameServerSession::Release();
}

VOID BattleServerSession::Update()
{
	GameServerSession::Update();
}

VOID BattleServerSession::OnRedirect()
{	
	GameServerSession::OnRedirect();

	MessageOut(eCRITICAL_LOG,  "Battle server connected.(%s)", GetIP() );
}

VOID BattleServerSession::OnConnect( BOOL bSuccess, DWORD dwNetworkIndex )
{
	GameServerSession::OnConnect( bSuccess, dwNetworkIndex );
}

VOID BattleServerSession::OnDisconnect()
{
	ServerSession::OnDisconnect();
	MessageOut(eCRITICAL_LOG,  "Field server disconnected." );
}

VOID BattleServerSession::OnRecv( BYTE *pMsg, WORD wSize )
{
	if( PacketHandler::Instance()->ParsePacket_BW( this, (MSG_BASE*)pMsg, wSize ) )
	{
		return;
	}

	GameServerSession::OnRecv( pMsg, wSize );
}
