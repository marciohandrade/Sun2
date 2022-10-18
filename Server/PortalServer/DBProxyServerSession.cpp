#include "StdAfx.h"
#include ".\dbproxyserversession.h"
#include "PacketHandler.h"
#include "PortalRankingManager.h"

__IMPL_CUSTOMPOOL_PTR( DBProxyServerSession )
DBProxyServerSession::DBProxyServerSession(void)
{
}

DBProxyServerSession::~DBProxyServerSession(void)
{
}

VOID DBProxyServerSession::OnRecv( BYTE *pMsg, WORD wSize )
{
	PacketHandler::Instance()->ParsePacket( PACKET_HANDLER_DBPROXY, this, (MSG_BASE *)pMsg, wSize );
}

void DBProxyServerSession::OnConnect( BOOL bSuccess, DWORD dwNetworkIndex )
{
	ServerSession::OnConnect( bSuccess, dwNetworkIndex );

	if( bSuccess )
	{
		g_RankingMgr.Initialize();
	}
}