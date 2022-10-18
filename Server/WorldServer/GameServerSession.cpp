#include "stdafx.h"
#include "GameServerSession.h"
#include "PacketHandler.h"
#include <PacketStruct.h>
#include <PacketStruct_GM.h>
#include <Protocol_GM.h>


GameServerSession::GameServerSession()
{
}

GameServerSession::~GameServerSession()
{
}


VOID GameServerSession::Init()
{	
	ServerSession::Init();
}

VOID GameServerSession::Release()
{
	ServerSession::Release();
}

VOID GameServerSession::Update()
{
	ServerSession::Update();
}

VOID GameServerSession::OnRedirect()
{
}

VOID GameServerSession::OnConnect( BOOL bSuccess, DWORD dwNetworkIndex  )
{
	ServerSession::OnConnect( bSuccess, dwNetworkIndex );
}

VOID GameServerSession::OnDisconnect()
{
	ServerSession::OnDisconnect();
}

VOID GameServerSession::OnRecv( BYTE *pMsg, WORD wSize )
{
	if( !PacketHandler::Instance()->ParsePacket_GW( this, (MSG_BASE*)pMsg, wSize ) )
	{
		MessageOut(eCRITICAL_LOG,   "Invalid packet from game server! ServerType(%d)", GetServerType() );
	}
}

// 게임 서버가 connect 해야 할 DB 프락시 서버의 IP와 포트를 알려준다.
VOID GameServerSession::SendDBProxyServerInfo()
{
	/*
	SERVER_INFO dbProxyServerInfo;
	ServerInfoManager::Instance()->GetDBProxyServerInfo( &dbProxyServerInfo );

	MSG_GM_CONNECTION_REQ_DBPROXY_SERVER_INFO_ACK sendMsg;

	sendMsg.m_byCategory		= GM_OTHER;
	sendMsg.m_byProtocol		= GM_CONNECTION_REQ_DBPROXY_SERVER_INFO_ACK;
	sendMsg.m_wPort				= dbProxyServerInfo.wInnerPort;
	strncpy( sendMsg.m_szIP, dbProxyServerInfo.szInnerIP, MAX_IP_STRING_LEN );

	Send( (BYTE*)&sendMsg, sizeof(sendMsg) );
	*/
}

