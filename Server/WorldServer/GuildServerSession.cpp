#include "stdafx.h"
#if !defined(_NA002635_GZ_DZ_PROTOCOL_UINIFICATION)
#include "GuildServerSession.h"
#include "PacketHandler.h"
#include <PacketStruct.h>
#include <PacketStruct_MW.h>
#include <Protocol_MW.h>


GuildServerSession::GuildServerSession()
{
}

GuildServerSession::~GuildServerSession()
{
}


VOID GuildServerSession::Init()
{	
	ServerSession::Init();
}

VOID GuildServerSession::Release()
{
	ServerSession::Release();
}

VOID GuildServerSession::Update()
{
	ServerSession::Update();
}

VOID GuildServerSession::OnRedirect()
{
	MessageOut( eCRITICAL_LOG, "Guild Server Connected." );
}

VOID GuildServerSession::OnConnect( BOOL bSuccess, DWORD dwNetworkIndex  )
{
	ServerSession::OnConnect( bSuccess, dwNetworkIndex );
}

VOID GuildServerSession::OnDisconnect()
{
	ServerSession::OnDisconnect();
	MessageOut( eCRITICAL_LOG, "Guild Server Disconnected." );
}

VOID GuildServerSession::OnRecv( BYTE *pMsg, WORD wSize )
{
	if( !PacketHandler::Instance()->ParsePacket_WZ( this, (MSG_BASE*)pMsg, wSize ) )
	{
		MessageOut(eCRITICAL_LOG,   "Invalid packet from Guild server! ServerType(%d)", GetServerType() );
	}
}
#endif //_NA002635_GZ_DZ_PROTOCOL_UINIFICATION