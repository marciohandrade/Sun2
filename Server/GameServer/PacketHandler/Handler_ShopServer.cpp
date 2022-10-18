#include "StdAfx.h"
#include ".\handler_shopserver.h"

#if !defined _NA_0_20110613_GAME_SHOP_RENEWAL


#include <PacketStruct_GS.h>
#include ".\GameShopManager.h"
#include ".\GameServerEx.h"
#include ".\Player.h"
#include ".\PlayerManager.h"

Handler_ShopServer::Handler_ShopServer(void)
{
}

Handler_ShopServer::~Handler_ShopServer(void)
{
}

//--------------------------------------------------------------------------------------------------
// changes, f100819.1L
const nsPacket::PacketHandlerInfo Handler_ShopServer::GetPacketHandlerNodes()
{
#define HANDLER_NODE_INFO(c, p, phase) { MAKEWORD(c, p), phase, Handler_ShopServer::On##p }
#define HANDLER_NODE_INFO_EOR() { 0, static_cast<ePHASE_SHIFT>(0), NULL }

    static const nsPacket::PacketHandlerNode handlers_static_[] =
    {
        HANDLER_NODE_INFO(GS_SHOP, GS_SHOP_USER_CASH_REQ_ACK, PHASE_SERVICE),
        HANDLER_NODE_INFO(GS_SHOP, GS_SHOP_ITEM_LIST_REQ_ACK, PHASE_SERVICE),
        HANDLER_NODE_INFO(GS_SHOP, GS_SHOP_EXTEND_OPTION_LIST_REQ_ACK, PHASE_SERVICE),
#ifdef __NA_00387_20070605_SHOP_MILEAGE2
        HANDLER_NODE_INFO(GS_SHOP, GS_SHOP_MILEAGE_LIST_REQ_ACK, PHASE_SERVICE),
#endif
        HANDLER_NODE_INFO(GS_SHOP, GS_SHOP_PACK_LIST_REQ_ACK, PHASE_SERVICE),
        HANDLER_NODE_INFO(GS_SHOP, GS_SHOP_BRANCH_LIST_REQ_ACK, PHASE_SERVICE),
        HANDLER_NODE_INFO(GS_SHOP, GS_SHOP_BUY_ITEM_ACK, PHASE_SERVICE),
        HANDLER_NODE_INFO(GS_SHOP, GS_SHOP_USER_BUY_HISTORY_ACK, PHASE_SERVICE),
        HANDLER_NODE_INFO(GS_SHOP, GS_SHOP_RELOAD_DATA_NTF, PHASE_SERVICE),

        HANDLER_NODE_INFO(GS_SHOP, GS_SHOP_SEND_GIFT_ACK, PHASE_SERVICE),
        HANDLER_NODE_INFO(GS_SHOP, GS_SHOP_ACTION_RECV_GIFT_ACK, PHASE_SERVICE),
        HANDLER_NODE_INFO(GS_SHOP, GS_SHOP_IS_RECV_GIFT_ACK, PHASE_SERVICE),
        HANDLER_NODE_INFO(GS_SHOP, GS_SHOP_RECV_GIFT_LIST_ACK, PHASE_SERVICE),
        HANDLER_NODE_INFO(GS_SHOP, GS_SHOP_SEND_GIFT_LIST_ACK, PHASE_SERVICE),

        HANDLER_NODE_INFO_EOR() // end of records
    };
    //
    BOOST_STATIC_ASSERT(_countof(handlers_static_) >= 1);
    nsPacket::PacketHandlerInfo return_value = { handlers_static_, _countof(handlers_static_) - 1 };
    //
    return return_value;
};
//--------------------------------------------------------------------------------------------------


Handler_ShopServer_IMPL(GS_SHOP_USER_CASH_REQ_ACK)
{
    __TOUCHED((pServerSession, pRecvMsg, wSize));
	MSG_GS_SHOP_USER_CASH_REQ_ACK * pMsg = (MSG_GS_SHOP_USER_CASH_REQ_ACK*)pRecvMsg;

    CashData cash_data = { 0, };
    cash_data.cash_ = pMsg->dwUserCash;
  #ifdef __CN_0_20070507_SHOP_MILEAGE
    cash_data.mileage_ = pMsg->dwHonors;
  #endif
  #if defined(_NA00XXXX_090111_CASH_TYPE_SUBDIVIDE_)
    cash_data.pp_cash_ = pMsg->pp_cash;
  #endif
    g_GameShopManager.OnResultUserCash(pMsg->dwUserGuid, cash_data);
}

Handler_ShopServer_IMPL(GS_SHOP_ITEM_LIST_REQ_ACK)
{
    __TOUCHED((pServerSession, pRecvMsg, wSize));
	MSG_GS_SHOP_ITEM_LIST_REQ_ACK * pMsg = (MSG_GS_SHOP_ITEM_LIST_REQ_ACK*)pRecvMsg;
	if( pMsg->dwItemCount != E_EXCEPTION )
	{
		g_GameShopManager.ReceiveSignal( eSHOP_SIGNAL_RECEIVE_ITEM_LIST, pMsg );
	}
	else
	{		
		SUNLOG( eCRITICAL_LOG, "[MSG_GS_SHOP_ITEM_LIST_REQ_ACK]Shop Server Error Request Again" );
		/*
		MSG_GS_SHOP_ITEM_LIST_REQ_SYN msg;
		if( !g_pGameServer->SendToServer( SHOP_SERVER, (MSG_BASE*)&msg, sizeof(msg) ) )
		{
			SUNLOG( eCRITICAL_LOG, "[GS_SHOP_ITEM_LIST_REQ_ACK]SHOP_SERVER is Disconnected2" );
		}
		*/
	}	
}

Handler_ShopServer_IMPL(GS_SHOP_EXTEND_OPTION_LIST_REQ_ACK)
{
    __TOUCHED((pServerSession, pRecvMsg, wSize));
	MSG_GS_SHOP_EXTEND_OPTION_LIST_REQ_ACK * pMsg = (MSG_GS_SHOP_EXTEND_OPTION_LIST_REQ_ACK*)pRecvMsg;

	if( pMsg->dwItemOptionCount != E_EXCEPTION )
	{
		g_GameShopManager.ReceiveSignal( eSHOP_SIGNAL_RECEIVE_EXTEND_OPTION_LIST, pMsg );
	}
	else
	{
		SUNLOG( eCRITICAL_LOG, "[GS_SHOP_EXTEND_OPTION_LIST_REQ_ACK]Shop Server Error Request Again" );
		/*
		MSG_GS_SHOP_EXTEND_OPTION_LIST_REQ_SYN msg;
		if( !g_pGameServer->SendToServer( SHOP_SERVER, (MSG_BASE*)&msg, sizeof(msg) ) )
		{
			SUNLOG( eCRITICAL_LOG, "[GS_SHOP_EXTEND_OPTION_LIST_REQ_ACK]SHOP_SERVER is Disconnected2" );
		}
		*/
	}
	
}

#ifdef __NA_00387_20070605_SHOP_MILEAGE2
Handler_ShopServer_IMPL(GS_SHOP_MILEAGE_LIST_REQ_ACK)
{
    __TOUCHED((pServerSession, pRecvMsg, wSize));
	MSG_GS_SHOP_MILEAGE_LIST_REQ_ACK * pMsg = (MSG_GS_SHOP_MILEAGE_LIST_REQ_ACK*)pRecvMsg;
	if( pMsg->dwItemCount != E_EXCEPTION )
	{
		g_GameShopManager.ReceiveSignal( eSHOP_SIGNAL_RECEIVE_MILEAGE_LIST, pMsg );
	}
	else
	{
		SUNLOG( eCRITICAL_LOG, "[MSG_GS_SHOP_ITEM_LIST_REQ_ACK]Shop Server Error Request Again" );
		/*
		MSG_GS_SHOP_MILEAGE_LIST_REQ_SYN msg;
		if( !g_pGameServer->SendToServer( SHOP_SERVER, (MSG_BASE*)&msg, sizeof(msg) ) )
		{
			SUNLOG( eCRITICAL_LOG, "[MSG_GS_SHOP_ITEM_LIST_REQ_ACK]SHOP_SERVER is Disconnected2" );
		}
		*/
	}	
}
#endif
Handler_ShopServer_IMPL(GS_SHOP_PACK_LIST_REQ_ACK)
{
    __TOUCHED((pServerSession, pRecvMsg, wSize));
	MSG_GS_SHOP_PACK_LIST_REQ_ACK * pMsg = (MSG_GS_SHOP_PACK_LIST_REQ_ACK*)pRecvMsg;

	g_GameShopManager.ReceiveSignal( eSHOP_SIGNAL_RECEIVE_PACKAGE_LIST, pMsg );
}


Handler_ShopServer_IMPL(GS_SHOP_BRANCH_LIST_REQ_ACK)
{
    __TOUCHED((pServerSession, pRecvMsg, wSize));
	MSG_GS_SHOP_BRANCH_LIST_REQ_ACK * pMsg = (MSG_GS_SHOP_BRANCH_LIST_REQ_ACK*)pRecvMsg;

	g_GameShopManager.ReceiveSignal( eSHOP_SIGNAL_RECEIVE_BRANCH_LIST, pMsg );

	//// 테스트 코드
	//MSG_GS_SHOP_USER_CASH_REQ_SYN msg;
	//msg.dwUserGuid = 321970;
	//g_pGameServer->SendToServer( SHOP_SERVER, (MSG_BASE*)&msg, sizeof(msg) );
	//g_GameShopManager.CanBuyAndThenReq( NULL, 2, NULL );
}
Handler_ShopServer_IMPL(GS_SHOP_BUY_ITEM_ACK)
{
    __TOUCHED((pServerSession, pRecvMsg, wSize));
	MSG_GS_SHOP_BUY_ITEM_ACK * pMsg = (MSG_GS_SHOP_BUY_ITEM_ACK*)pRecvMsg;

	// 임시 코드!!!
	// pMsg->dwUserGuid  == UserKey와 동일하다는 가정!!!!!
	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pMsg->dwUserGuid );
    CashData cash_data = { 0, };
    cash_data.cash_ = pMsg->dwUserCash;
  #ifdef __CN_0_20070507_SHOP_MILEAGE
    cash_data.mileage_ = pMsg->dwHonors;
  #endif
  #if defined(_NA00XXXX_090111_CASH_TYPE_SUBDIVIDE_)
    cash_data.pp_cash_ = pMsg->pp_cash;
  #endif
    g_GameShopManager.OnBuyResult(pPlayer, pMsg->dwUserGuid, pMsg->dwCharGuid,
                                  cash_data, pMsg->dwResult,
                                  static_cast<BYTE>(pMsg->dwCount), pMsg->transactions);
}


Handler_ShopServer_IMPL(GS_SHOP_USER_BUY_HISTORY_ACK)
{
    __TOUCHED((pServerSession, pRecvMsg, wSize));
	MSG_GS_SHOP_USER_BUY_HISTORY_ACK * pMsg = (MSG_GS_SHOP_USER_BUY_HISTORY_ACK*)pRecvMsg;
}
Handler_ShopServer_IMPL(GS_SHOP_RELOAD_DATA_NTF)
{
    __TOUCHED((pServerSession, pRecvMsg, wSize));
	MSG_GS_SHOP_RELOAD_DATA_NTF * pMsg = (MSG_GS_SHOP_RELOAD_DATA_NTF*)pRecvMsg;
	g_GameShopManager.OnStart();
	SUNLOG( eCRITICAL_LOG, "[GS_SHOP_RELOAD_DATA_NTF]" );
}



Handler_ShopServer_IMPL(GS_SHOP_SEND_GIFT_ACK)
{
    __TOUCHED((pServerSession, pRecvMsg, wSize));
	MSG_GS_SHOP_SEND_GIFT_ACK * pMsg = (MSG_GS_SHOP_SEND_GIFT_ACK*)pRecvMsg;
	// 임시 코드!!!
	// pMsg->dwUserGuid  == UserKey와 동일하다는 가정!!!!!
	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pMsg->dwUserGuid );
    CashData cash_data = { 0, };
    cash_data.cash_ = pMsg->dwUserCash;
  #ifdef __CN_0_20070507_SHOP_MILEAGE
    cash_data.mileage_ = pMsg->dwHonors;
  #endif
  #if defined(_NA00XXXX_090111_CASH_TYPE_SUBDIVIDE_)
    cash_data.pp_cash_ = pMsg->pp_cash;
  #endif
    g_GameShopManager.OnSendGift(pPlayer, pMsg->dwUserGuid, pMsg->dwCharGuid,
                                 cash_data, pMsg->dwPriceGuid, pMsg->dwResult);
}

Handler_ShopServer_IMPL(GS_SHOP_ACTION_RECV_GIFT_ACK)
{
    __TOUCHED((pServerSession, pRecvMsg, wSize));
	MSG_GS_SHOP_ACTION_RECV_GIFT_ACK * pMsg = (MSG_GS_SHOP_ACTION_RECV_GIFT_ACK*)pRecvMsg;
	// 임시 코드!!!
	// pMsg->dwUserGuid  == UserKey와 동일하다는 가정!!!!!
	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pMsg->dwUserGuid );
	g_GameShopManager.OnActionRecvGift( pPlayer, 
		pMsg->dwUserGuid, pMsg->dwCharGuid, 
		pMsg->dwGiftItemID,
		pMsg->dwAction,
		pMsg->dwResult );
}

Handler_ShopServer_IMPL(GS_SHOP_IS_RECV_GIFT_ACK)
{
    __TOUCHED((pServerSession, pRecvMsg, wSize));
	MSG_GS_SHOP_IS_RECV_GIFT_ACK * pMsg = (MSG_GS_SHOP_IS_RECV_GIFT_ACK*)pRecvMsg;
	// 임시 코드!!!
	// pMsg->dwUserGuid  == UserKey와 동일하다는 가정!!!!!
	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pMsg->dwUserGuid );
	g_GameShopManager.OnRecvGiftListCount( pPlayer, pMsg->dwGiftCount );
}

Handler_ShopServer_IMPL(GS_SHOP_RECV_GIFT_LIST_ACK)
{
    __TOUCHED((pServerSession, pRecvMsg, wSize));
	MSG_GS_SHOP_RECV_GIFT_LIST_ACK * pMsg = (MSG_GS_SHOP_RECV_GIFT_LIST_ACK*)pRecvMsg;
	// 임시 코드!!!
	// pMsg->dwUserGuid  == UserKey와 동일하다는 가정!!!!!
	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pMsg->dwUserGuid );

	g_GameShopManager.OnRecvGiftList( pPlayer, 
		pMsg->dwUserGuid, pMsg->dwCharGuid, 
		pMsg->dwCount, pMsg->recvGiftItems );
}

Handler_ShopServer_IMPL(GS_SHOP_SEND_GIFT_LIST_ACK)
{
    __TOUCHED((pServerSession, pRecvMsg, wSize));
	MSG_GS_SHOP_SEND_GIFT_LIST_ACK * pMsg = (MSG_GS_SHOP_SEND_GIFT_LIST_ACK*)pRecvMsg;

}

#endif //#if !defined _NA_0_20110613_GAME_SHOP_RENEWAL
