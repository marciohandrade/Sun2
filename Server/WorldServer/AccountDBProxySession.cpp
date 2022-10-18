#include "stdafx.h"
#include <PacketStruct_MX.h>
#include <Protocol_MX.h>
#include <const.h>
#include "AccountDBProxySession.h"
#include "WorldServer.h"
#include "PacketHandler.h"

AccountDBProxySession::AccountDBProxySession()
{
}

AccountDBProxySession::~AccountDBProxySession()
{
}

VOID AccountDBProxySession::Init()
{
	ServerSession::Init();
}

VOID AccountDBProxySession::Release()
{
	ServerSession::Release();
}

VOID AccountDBProxySession::Update()
{
	ServerSession::Update();
}

VOID AccountDBProxySession::OnAccept( DWORD dwNetworkIndex )
{
	ServerSession::OnAccept( dwNetworkIndex );
}

VOID AccountDBProxySession::OnConnect( BOOL bSuccess, DWORD dwNetworkIndex )
{
	ServerSession::OnConnect( bSuccess, dwNetworkIndex );

	if( bSuccess )
	{
		MessageOut(eCRITICAL_LOG,   "Connected to account DB proxy server." );
		SendReqServerInfo();
	}
}

VOID AccountDBProxySession::OnDisconnect()
{
	ServerSession::OnDisconnect();
	MessageOut(eCRITICAL_LOG,   "Disconnected from account DB proxy server." );
}

VOID AccountDBProxySession::OnRecv( BYTE *pMsg, WORD wSize )
{
	MSG_BASE *pBaseMsg = (MSG_BASE*)pMsg;

	PacketHandler::Instance()->ParsePacket_TW( this, (MSG_BASE*)pMsg, wSize );
}

VOID AccountDBProxySession::SendReqServerInfo()
{
	/*
	MSG_MX_CONNECTION_REQ_SERVER_INFO_SYN msg;
	msg.m_byCategory	= MX_CONNECTION;
	msg.m_byProtocol	= MX_CONNECTION_REQ_SERVER_INFO_SYN;
	
	Send( (BYTE*)&msg, sizeof(MSG_MX_CONNECTION_REQ_SERVER_INFO_SYN) );
	MessageOut(eCRITICAL_LOG,   "Request server information..." );
	*/
}
