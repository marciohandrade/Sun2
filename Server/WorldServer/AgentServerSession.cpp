#include "stdafx.h"
#include <PacketStruct.h>
#include <PacketStruct_AM.h>
#include <Protocol_AM.h>
#include <const.h>
#include "AgentServerSession.h"
#include "WorldServer.h"
#include "PacketHandler.h"
#include "ChannelManager.h"

AgentServerSession::AgentServerSession()
{
}

AgentServerSession::~AgentServerSession()
{
}

VOID AgentServerSession::Init()
{
	ServerSession::Init();

	m_dwChannelID = 0;
}

VOID AgentServerSession::Release()
{
	ServerSession::Release();
}

VOID AgentServerSession::Update()
{
	ServerSession::Update();
}

VOID AgentServerSession::OnRedirect()
{
	MessageOut(eCRITICAL_LOG,   "Agent server connected.(%s)", GetIP() );	
}

VOID AgentServerSession::OnConnect( BOOL bSuccess, DWORD dwNetworkIndex )
{
	ServerSession::OnConnect( bSuccess, dwNetworkIndex );
}

VOID AgentServerSession::OnDisconnect()
{
	ServerSession::OnDisconnect();

	ChannelManager::Instance()->RemoveChannel( GetChannelID() );

	MessageOut(eCRITICAL_LOG,   "Agent server disconnected." );
}

VOID AgentServerSession::OnRecv( BYTE *pMsg, WORD wSize )
{
	MSG_BASE *pBaseMsg = (MSG_BASE*)pMsg;

	PacketHandler::Instance()->ParsePacket_AW( this, (MSG_BASE*)pMsg, wSize );
}
