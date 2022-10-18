#include "stdafx.h"
#include "FieldServerSession.h"
#include "PacketHandler.h"
#include "GameServerSession.h"


FieldServerSession::FieldServerSession()
{
}

FieldServerSession::~FieldServerSession()
{
}

VOID FieldServerSession::Init()
{
	GameServerSession::Init();
}

VOID FieldServerSession::Release()
{
	GameServerSession::Release();
}

VOID FieldServerSession::Update()
{
	GameServerSession::Update();
}

VOID FieldServerSession::OnRedirect()
{
	GameServerSession::OnRedirect();
	MessageOut(eCRITICAL_LOG,  "Field server connected.(%s)", GetIP() );
}

VOID FieldServerSession::OnConnect( BOOL bSuccess, DWORD dwNetworkIndex )
{
	GameServerSession::OnConnect( bSuccess, dwNetworkIndex );
}

VOID FieldServerSession::OnDisconnect()
{
	ServerSession::OnDisconnect();
	MessageOut(eCRITICAL_LOG,  "Field server disconnected." );
}

VOID FieldServerSession::OnRecv( BYTE *pMsg, WORD wSize )
{
	if( PacketHandler::Instance()->ParsePacket_FW( this, (MSG_BASE*)pMsg, wSize ) )
	{
		return;
	}

	GameServerSession::OnRecv( pMsg, wSize );
}
