#include "StdAfx.h"
#include ".\shopserversession.h"

#if !defined _NA_0_20110613_GAME_SHOP_RENEWAL

#include "PacketHandler/PacketHandler.h"
#include ".\GameShopManager.h"

__IMPL_CUSTOMPOOL_PTR( ShopServerSession )
ShopServerSession::ShopServerSession(void)
{
}

ShopServerSession::~ShopServerSession(void)
{
}

// CHANGES: f110617.1L
void ShopServerSession::OnRedirect(DWORD network_index, MSG_SERVER_TYPE* const first_contact_msg)
{
    ServerSession::OnRedirect(network_index, first_contact_msg);
}

VOID ShopServerSession::OnConnect( BOOL bSuccess, DWORD dwSessionIndex )
{
	SetIsManualConnect(false);
	SetConnecting(false);

	if( bSuccess )
	{
        // CHANGES: f110615.2L, added to solve high-load network bottleneck problem
        this->UseRecvDualBuffer(10 * 1024 * 1024);
        //
		OnConnectSuccess( dwSessionIndex );

		SUNLOG( eFULL_LOG,  "[ShopServerSession::OnConnect] Connected to Shop server." );
		g_GameShopManager.OnStart();
	}
}

VOID ShopServerSession::OnDisconnect()
{
	SUNLOG( eCRITICAL_LOG, "[ShopServerSession::OnDisconnect] Disconnected from Shop Server" );

	g_GameShopManager.OnEnd();

	ServerSession::OnDisconnect();
}

VOID ShopServerSession::OnRecv( BYTE *pMsg, WORD wSize )
{
	PacketHandler::Instance()->ParsePacket( PACKET_HANDLER_SHOP, this, (MSG_BASE_INTERNAL*)pMsg, wSize );
}

BOOL ShopServerSession::Send( BYTE *pMsg, WORD wSize )
{
	BOOL bSuccess = NetworkObject::Send( pMsg, wSize );

	return bSuccess;
}


#endif //#if !defined _NA_0_20110613_GAME_SHOP_RENEWAL

