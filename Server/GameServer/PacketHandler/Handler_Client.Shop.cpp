#include "StdAfx.h"
#include ".\handler_client.h"



#if !defined _NA_0_20110613_GAME_SHOP_RENEWAL


#include <PacketStruct_CG.h>
#include <PacketStruct_GS.h>
#include <PacketStruct_DG.h>
#include <ArchiveOfStream.h>

#include "./Player.h"
#include "./PlayerManager.h"
#include <World/SSQ/SSQHandlerInGame.h>
#include "./GameShopManager.h"
#include "./GameServerEx.h"

// @history
// |10.02.08|waverix|remove '__NA001390_090915_RIDING_SYSTEM_ETC__',
//                   make connect status to shared logic|

// 샵 패킷
Handler_Client_IMPL( CG_SHOP_CASH_REQ_SYN )
{
    __TOUCHED((pServerSession, pRecvMsg, wSize));
	MSG_CG_SHOP_CASH_REQ_SYN * pMsg = (MSG_CG_SHOP_CASH_REQ_SYN *)pRecvMsg;
	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pMsg->m_dwKey );
	ASSERT( pPlayer );
	if( !pPlayer ) return ;
	if( !pPlayer->GetField() ) return;

    // (CHANGES) (091018) (WAVERIX) pre-check whether shop server connected
    if(g_pGameServer->IsConnected(SHOP_SERVER) == 0) {
        MSG_CG_SHOP_CASH_REQ_NAK msgNAK;
        msgNAK.m_byErrorCode = RC::RC_SHOP_SHOPSERVERDISCONNECTED;
        pPlayer->SendPacket(&msgNAK, sizeof(msgNAK));
        return;
    }

	RC::eSHOP_RESULT rt = g_GameShopManager.DoUserCash( pPlayer, pPlayer->GetUserGuid() );
	if( rt != RC::RC_SHOP_PENDING && rt != RC::RC_SHOP_SUCCESS )
	{
		MSG_CG_SHOP_CASH_REQ_NAK msg;
		msg.m_byErrorCode = rt;
		pPlayer->SendPacket( &msg, sizeof(msg) );
	}
}

Handler_Client_IMPL( CG_SHOP_ITEM_LIST_REQ_SYN )
{
    __TOUCHED((pServerSession, pRecvMsg, wSize));
	MSG_CG_SHOP_ITEM_LIST_REQ_SYN * pMsg = (MSG_CG_SHOP_ITEM_LIST_REQ_SYN *)pRecvMsg;
	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pMsg->m_dwKey );
	ASSERT( pPlayer );
	if( !pPlayer ) return ;
	if( !pPlayer->GetField() ) return;

    // (CHANGES) (091018) (WAVERIX) pre-check whether shop server connected
    if(g_pGameServer->IsConnected(SHOP_SERVER) == 0) {
        MSG_CG_SHOP_ITEM_LIST_REQ_NAK msgNAK;
        msgNAK.m_byErrorCode = RC::RC_SHOP_SHOPSERVERDISCONNECTED;
        pPlayer->SendPacket(&msgNAK, sizeof(msgNAK));
        return;
    }


    RC::eSHOP_RESULT result = RC::RC_SHOP_SUCCESS;

#ifdef _NA_0_20101015_SOLAR_SHOP_TEMP_REFACTORING
    GameShopUser* shopuser = (GameShopUser*)g_GameShopManager.FindShopUser(pPlayer->GetUserGuid());
    if (shopuser == NULL)
    {
        MSG_CG_SHOP_ITEM_LIST_REQ_NAK msgNAK;
        msgNAK.m_byErrorCode = RC::RC_SHOP_USERISNOTFOUND;
        pPlayer->SendPacket(&msgNAK, sizeof(msgNAK));
        return;
    }

    static GameShopManager::SearchInfo search_info;
    ZeroMemory(&search_info, sizeof(search_info));
    search_info.input.partition_index = (eSHOP_PARTITION)pMsg->partition_index;
    search_info.input.category_index = (eSHOP_CATEGORY)pMsg->category_index;
    search_info.input.page = pMsg->request_page;
    search_info.input.filter = pMsg->item_filter;

    if (g_GameShopManager.Search(shopuser, search_info))
    {
        static MSG_CG_SHOP_ITEM_LIST_REQ_ACK message;
        uint16_t buffer_size = (uint16_t)(MSG_CG_SHOP_ITEM_LIST_REQ_ACK::MAX_BUFFER_SIZE);
        result = g_GameShopManager.Serialize(search_info, message.buffer, buffer_size);
        if (result == RC::RC_SHOP_SUCCESS || result == RC::RC_SHOP_PENDING)
        {
            message.partition_index = pMsg->partition_index;
            message.category_index  = pMsg->category_index;
            message.item_filter = pMsg->item_filter;
            message.buffer_size = buffer_size;
            message.answer_page = search_info.output.page;
            message.page_count = search_info.output.total_page;
            message.guid_count = search_info.output.exist_guid_count;
            memcpy(message.shopitem_guid, search_info.output.exist_guid, sizeof(uint) * MSG_CG_SHOP_ITEM_LIST_REQ_ACK::MAX_GUID);
            pPlayer->SendPacket(&message, message.GetSize());
            return;
        }
    }

    MSG_CG_SHOP_ITEM_LIST_REQ_NAK nak_message;
    nak_message.m_byErrorCode = RC::RC_SHOP_FAILED;
    pPlayer->SendPacket(&nak_message, sizeof(nak_message));

#else
	static MSG_CG_SHOP_ITEM_LIST_REQ_ACK msg;
	msg.m_TotalPageNum = 0;
	msg.m_Size = MSG_CG_SHOP_ITEM_LIST_REQ_ACK::MAX_BUFFER_SIZE;
	result = g_GameShopManager.Serialize( pPlayer, pMsg->m_byCategorySeperate, pMsg->m_byCategoryIndex, pMsg->m_byPage, msg.m_pBUFFER, msg.m_Size, eSHOP_SERIALIZE_ITEM_LIST_LOAD );

	if( result != RC::RC_SHOP_PENDING && result != RC::RC_SHOP_SUCCESS )
	{
		MSG_CG_SHOP_ITEM_LIST_REQ_NAK nmsg;
		nmsg.m_byErrorCode = result;
		pPlayer->SendPacket( &nmsg, sizeof(nmsg) );
		return;
	}

	ShopCategory * pCategory = g_GameShopManager.FindShopCategory( (const eSHOP_CATEGORY_SEPERATE)pMsg->m_byCategorySeperate, (const eSHOP_CATEGORY)pMsg->m_byCategoryIndex );
	if( pCategory )
	{
		msg.m_TotalPageNum = pCategory->GetTotalPageNum();
	}

    SUNLOG( eDEV_LOG, "[Handler_Client_IMPL( CG_SHOP_ITEM_LIST_REQ_SYN )] Category = %d, TotalPage = %d", pMsg->m_byCategoryIndex, msg.m_TotalPageNum );
    pPlayer->SendPacket( &msg, msg.GetSize() );

#endif // _NA_0_20101015_SOLAR_SHOP_TEMP_REFACTORING

}

Handler_Client_IMPL( CG_SHOP_ITEM_REQ_SYN )
{
    __TOUCHED((pServerSession, pRecvMsg, wSize));
	MSG_CG_SHOP_ITEM_REQ_SYN * pMsg = (MSG_CG_SHOP_ITEM_REQ_SYN *)pRecvMsg;
	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pMsg->m_dwKey );
	ASSERT( pPlayer );
	if( !pPlayer || !pPlayer->GetField() ) return;

    // (CHANGES) (091018) (WAVERIX) pre-check whether shop server connected
    if(g_pGameServer->IsConnected(SHOP_SERVER) == 0) {
        MSG_CG_SHOP_ITEM_REQ_NAK msgNAK;
        msgNAK.m_byErrorCode = RC::RC_SHOP_SHOPSERVERDISCONNECTED;
        pPlayer->SendPacket(&msgNAK, sizeof(msgNAK));
        return;
    }

	static MSG_CG_SHOP_ITEM_REQ_ACK msg;
	msg.m_Size = MSG_CG_SHOP_ITEM_LIST_REQ_ACK::MAX_BUFFER_SIZE;
	RC::eSHOP_RESULT rt = g_GameShopManager.SerializeItemDetail( pPlayer, pMsg->m_ItemGuid, msg.m_pBUFFER, msg.m_Size, eSHOP_SERIALIZE_ITEM_LOAD );
	if( rt != RC::RC_SHOP_PENDING && rt != RC::RC_SHOP_SUCCESS )
	{
		MSG_CG_SHOP_ITEM_REQ_NAK nmsg;
		nmsg.m_byErrorCode = rt;
		pPlayer->SendPacket( &nmsg, sizeof(nmsg) );
		return;
	}
	pPlayer->SendPacket( &msg, msg.GetSize() );
}


Handler_Client_IMPL( CG_SHOP_ITEM_BUY_SYN )
{
    __TOUCHED((pServerSession, pRecvMsg, wSize));
#if !defined(__NA000851_080122_APPLY_EFFECT_ITEM_ON_INVEN_FOR_CACHE__)
    MSG_CG_SHOP_ITEM_BUY_SYN* pMsg = static_cast<MSG_CG_SHOP_ITEM_BUY_SYN*>(pRecvMsg);
    Player* player = PlayerManager::Instance()->FindPlayerByUserKey(pMsg->m_dwKey);
    if (FlowControl::FCAssert(player != NULL) == false) {
        return;
    }
    if (player->GetField() == NULL) {
        return;
    }

    // (CHANGES) (091018) (WAVERIX) pre-check whether shop server connected
    if (g_pGameServer->IsConnected(SHOP_SERVER) == false)
    {
        MSG_CG_SHOP_ITEM_BUY_NAK msgNAK;
        msgNAK.m_byErrorCode = RC::RC_SHOP_SHOPSERVERDISCONNECTED;
        player->SendPacket(&msgNAK, sizeof(msgNAK));
        return;
    }

    BOOLEAN purchase_pp_cash = false;
  #ifdef _NA00XXXX_090111_CASH_TYPE_SUBDIVIDE_
    ;       purchase_pp_cash = pMsg->m_PurchaseType == pMsg->PURCHASE_PPCASH;
  #endif

    RC::eSHOP_RESULT rt = g_GameShopManager.CanBuyNDoRequest(player, \
        pMsg->m_Count, pMsg->m_PriceGuid, pMsg->m_SlotIndex, pMsg->m_SlotPos, purchase_pp_cash);
    if (rt != RC::RC_SHOP_PENDING && rt != RC::RC_SHOP_SUCCESS)
    {
        MSG_CG_SHOP_ITEM_BUY_NAK nmsg;
        nmsg.m_byErrorCode = rt;
        player->SendPacket(&nmsg, sizeof(nmsg));
    }

#else //__NA000851_080122_APPLY_EFFECT_ITEM_ON_INVEN_FOR_CACHE__
/*
	const DWORD		ERRCODE_DEFAULT = 100000;
	MSG_CG_SHOP_ITEM_BUY_SYN* pMsg = (MSG_CG_SHOP_ITEM_BUY_SYN *)pRecvMsg;
	Player* const player = PlayerManager::Instance()->FindPlayerByUserKey(pMsg->m_dwKey);

	FlowControl flow;
	FLOWCONTROL_START()
    {
		// start LOGIC_BLOCK
		IfFailThenBreak(player && player->GetField(), ERRCODE_DEFAULT);

		const DWORD arrcount = pMsg->m_Count;
		IfFailThenBreak(0 < arrcount &&
                            arrcount <= MSG_CG_SHOP_ITEM_BUY_SYN::_MAX_BUY_LIST_NUM &&
                        pMsg->GetSize() == wSize,
                        RC::RC_SHOP_CANNOT_USE_SHOP);

		RC::eSHOP_RESULT rt = g_GameShopManager.CanBuyNDoRequest(player, pMsg->m_Count, pMsg->m_PriceGuid, pMsg->m_SlotIndex, pMsg->m_SlotPos);
		IfFailThenBreak(RC::RC_SHOP_SUCCESS == rt || RC::RC_SHOP_PENDING == rt, rt);

		return;
    }
    FLOWCONTROL_END;

	if (flow.GetErrorCode() != ERRCODE_DEFAULT)
	{
		MSG_CG_SHOP_ITEM_BUY_NAK nmsg;
		nmsg.m_byErrorCode = (BYTE)flow.GetErrorCode();
		player->SendPacket(&nmsg, sizeof(nmsg));
	}
	return;
*/
#endif
}

Handler_Client_IMPL( CG_SHOP_BUY_HISTORY_SYN )
{
    __TOUCHED((pServerSession, pRecvMsg, wSize));
	MSG_CG_SHOP_BUY_HISTORY_SYN * pMsg = (MSG_CG_SHOP_BUY_HISTORY_SYN *)pRecvMsg;
	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pMsg->m_dwKey );
	ASSERT( pPlayer );
	if( !pPlayer ) return ;
	if( !pPlayer->GetField() ) return;

	DISPMSG( "[CG_SHOP_BUY_HISTORY_SYN]Not Implemented\n" );
}
//
//Handler_Client_IMPL( CG_SHOP_CHECK_CHAR_NAME_SYN )
//{
//	MSG_CG_SHOP_CHECK_CHAR_NAME_SYN * pMsg = (MSG_CG_SHOP_CHECK_CHAR_NAME_SYN *)pRecvMsg;
//	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pMsg->m_dwKey );
//	ASSERT( pPlayer );
//	if( !pPlayer ) return ;
//	if( !pPlayer->GetField() ) return;
//
//	MSG_DG_OTHER_EXIST_CHARNAME_SYN msg;
//	msg.m_Type = eCHARNAME_ACTION_SHOP_SEND_GIFT;
//	_tcsncpy(msg.m_tszCharName, pMsg->m_tszCharName, MAX_CHARNAME_LENGTH);
//	msg.m_tszCharName[MAX_CHARNAME_LENGTH] = '\0';
//	if( !g_pGameServer->SendToServer( GAME_DBPROXY, &msg, sizeof(msg) ) )
//	{
//		MSG_CG_SHOP_CHECK_CHAR_NAME_NAK nmsg;
//		nmsg.m_byErrorCode = RC::RC_SHOP_DB_ERROR;
//		pPlayer->SendPacket( &nmsg, sizeof(nmsg) );
//	}
//}

#ifdef __NA000000_SHOP_USERID_50_PASSING_PROC__

Handler_Client_IMPL( CG_SHOP_SEND_GIFT_RELAY_SYN )
{
    __TOUCHED((pServerSession, pRecvMsg, wSize));
    MSG_CG_SHOP_SEND_GIFT_RELAY_SYN* pMsg = static_cast<MSG_CG_SHOP_SEND_GIFT_RELAY_SYN*>(pRecvMsg);
    Player* player = PlayerManager::Instance()->FindPlayerByUserKey(pMsg->m_dwKey);
    if (FlowControl::FCAssert(player != NULL) == false) {
        return;
    }
    if (player->GetField() == 0) {
        return;
    }
    // (CHANGES) (091018) (WAVERIX) pre-check whether shop server connected
    if(g_pGameServer->IsConnected(SHOP_SERVER) == 0) {
        MSG_CG_SHOP_SEND_GIFT_NAK msgNAK;
        msgNAK.m_byErrorCode = RC::RC_SHOP_SHOPSERVERDISCONNECTED;
        player->SendPacket(&msgNAK, sizeof(msgNAK));
        return;
    }
    BOOST_STATIC_ASSERT(sizeof(pMsg->gift_message_) == \
                        sizeof(((MSG_GS_SHOP_SEND_GIFT_SYN*)0)->szGiftMessage) &&
                        _countof(pMsg->gift_message_) == \
                        _countof(((MSG_GS_SHOP_SEND_GIFT_SYN*)0)->szGiftMessage));

    BOOLEAN puchase_pp_cash = false;
    ;       puchase_pp_cash = pMsg->purchase_type_ == pMsg->PURCHASE_PPCASH;
    RC::eSHOP_RESULT rt = g_GameShopManager.OnSendGiftNDoRequest(player,
                                                                 pMsg->item_type_,
                                                                 pMsg->price_guid_,
                                                                 puchase_pp_cash,
                                                                 pMsg->user_info_,
                                                                 pMsg->gift_message_);
    if(rt != RC::RC_SHOP_PENDING && rt != RC::RC_SHOP_SUCCESS)
    {
        MSG_CG_SHOP_SEND_GIFT_NAK msgNAK;
        msgNAK.m_byErrorCode = rt;
        player->SendPacket(&msgNAK, sizeof(msgNAK));
    }
}

#else //!__NA000000_SHOP_USERID_50_PASSING_PROC__

Handler_Client_IMPL( CG_SHOP_SEND_GIFT_SYN )
{
    __TOUCHED((pServerSession, pRecvMsg, wSize));
	MSG_CG_SHOP_SEND_GIFT_SYN * pMsg = (MSG_CG_SHOP_SEND_GIFT_SYN *)pRecvMsg;
	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pMsg->m_dwKey );
	ASSERT( pPlayer );
	if( !pPlayer ) return ;
	if( !pPlayer->GetField() ) return;

    // (CHANGES) (091018) (WAVERIX) pre-check whether shop server connected
    if(g_pGameServer->IsConnected(SHOP_SERVER) == 0) {
        MSG_CG_SHOP_SEND_GIFT_NAK msgNAK;
        msgNAK.m_byErrorCode = RC::RC_SHOP_SHOPSERVERDISCONNECTED;
        pPlayer->SendPacket(&msgNAK, sizeof(msgNAK));
        return;
    }

	RC::eSHOP_RESULT rt = g_GameShopManager.CanSendGiftNDoRequest( pPlayer, 
		pMsg->m_ItemType, pMsg->m_PriceGuid, pMsg->m_tszCharName, pMsg->m_tszGiftMessage );
	if( rt != RC::RC_SHOP_PENDING && rt != RC::RC_SHOP_SUCCESS )
	{
		MSG_CG_SHOP_SEND_GIFT_NAK nmsg;
		nmsg.m_byErrorCode = rt;
		pPlayer->SendPacket( &nmsg, sizeof(nmsg) );
	}
}
#endif //!__NA000000_SHOP_USERID_50_PASSING_PROC__

Handler_Client_IMPL( CG_SHOP_ACTION_RECV_GIFT_SYN )
{
    __TOUCHED((pServerSession, pRecvMsg, wSize));
	MSG_CG_SHOP_ACTION_RECV_GIFT_SYN * pMsg = (MSG_CG_SHOP_ACTION_RECV_GIFT_SYN *)pRecvMsg;
	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pMsg->m_dwKey );
	ASSERT( pPlayer );
	if( !pPlayer ) return ;
	if( !pPlayer->GetField() ) return;

    // (CHANGES) (091018) (WAVERIX) pre-check whether shop server connected
    if(g_pGameServer->IsConnected(SHOP_SERVER) == 0) {
        MSG_CG_SHOP_ACTION_RECV_GIFT_NAK msgNAK;
        msgNAK.m_byErrorCode = RC::RC_SHOP_SHOPSERVERDISCONNECTED;
        pPlayer->SendPacket(&msgNAK, sizeof(msgNAK));
        return;
    }

	RC::eSHOP_RESULT rt = g_GameShopManager.CanActionRecvGiftNDoRequest( pPlayer, pMsg->m_ActionType, pMsg->m_GiftGuid, "" );
	if( rt != RC::RC_SHOP_PENDING && rt != RC::RC_SHOP_SUCCESS )
	{
		MSG_CG_SHOP_ACTION_RECV_GIFT_NAK nmsg;
		nmsg.m_byErrorCode = rt;
		pPlayer->SendPacket( &nmsg, sizeof(nmsg) );
	}
}

Handler_Client_IMPL( CG_SHOP_IS_RECV_GIFT_SYN )
{
    __TOUCHED((pServerSession, pRecvMsg, wSize));
	MSG_CG_SHOP_IS_RECV_GIFT_SYN* pMsg = (MSG_CG_SHOP_IS_RECV_GIFT_SYN *)pRecvMsg;
	Player* pPlayer = PlayerManager::Instance()->FindPlayerByUserKey(pMsg->m_dwKey);
	ASSERT(pPlayer);
	if(pPlayer == 0)
        return;
    GameField* pGameField = pPlayer->GetField();
	if(pGameField == 0)
        return;

    //---------------------------------------------------------------------------------------------
    // CHANGES: f100908.2L, change a process order
    // because a server that is disconnected with a shop can't send a related inforamtion to player

    //_NA000023_090323_ENHANCED_INVENTORY_SLOT_FUNCTIONALITY_DB_
    ;{  // (CHANGES) (f100714.2L) pre-fetch to support a delayed event query process
        // 응모한 이벤트 정보를 요청
        MSG_DG_EVENT_ITEM_STATUS_SYN msg_status;
        msg_status.m_dwKey = pPlayer->GetUserGuid();
        msg_status.m_CharGUID = pPlayer->GetCharGuid();
        pPlayer->SendToGameDBPServer(&msg_status, sizeof(msg_status));
    };
    ;{
        MSG_CG_ITEM_INVENTORY_CONFIG_CHANGE_ACK msg_ack;
        msg_ack.changed = pPlayer->inventory_config();
        msg_ack.result = RC::RC_ITEM_SUCCESS;
        pPlayer->SendPacket(&msg_ack, sizeof(msg_ack));
    };
    //---------------------------------------------------------------------------------------------

    // (CHANGES) (091018) (WAVERIX) pre-check whether shop server connected
    if(g_pGameServer->IsConnected(SHOP_SERVER) == 0) {
        MSG_CG_SHOP_IS_RECV_GIFT_NAK msgNAK;
        msgNAK.m_byErrorCode = RC::RC_SHOP_SHOPSERVERDISCONNECTED;
        pPlayer->SendPacket(&msgNAK, sizeof(msgNAK));
        return;
    }

	RC::eSHOP_RESULT rt = g_GameShopManager.RequestRecvGiftCount(pPlayer);
	if(rt != RC::RC_SHOP_PENDING && rt != RC::RC_SHOP_SUCCESS)
	{
		MSG_CG_SHOP_IS_RECV_GIFT_NAK nmsg;
		nmsg.m_byErrorCode = rt;
		pPlayer->SendPacket(&nmsg, sizeof(nmsg));
	}
}

Handler_Client_IMPL( CG_SHOP_RECV_GIFT_LIST_SYN	)
{
    __TOUCHED((pServerSession, pRecvMsg, wSize));
	MSG_CG_SHOP_RECV_GIFT_LIST_SYN * pMsg = (MSG_CG_SHOP_RECV_GIFT_LIST_SYN *)pRecvMsg;
	Player * pPlayer = PlayerManager::Instance()->FindPlayerByUserKey( pMsg->m_dwKey );
	ASSERT( pPlayer );
	if( !pPlayer ) return ;
	if( !pPlayer->GetField() ) return;

    // (CHANGES) (091018) (WAVERIX) pre-check whether shop server connected
    if(g_pGameServer->IsConnected(SHOP_SERVER) == 0) {
        MSG_CG_SHOP_RECV_GIFT_LIST_NAK msgNAK;
        msgNAK.m_byErrorCode = RC::RC_SHOP_SHOPSERVERDISCONNECTED;
        pPlayer->SendPacket(&msgNAK, sizeof(msgNAK));
        return;
    }

	RC::eSHOP_RESULT rt = g_GameShopManager.RequestRecvGiftList( pPlayer );
	if( rt != RC::RC_SHOP_PENDING && rt != RC::RC_SHOP_SUCCESS )
	{
		MSG_CG_SHOP_RECV_GIFT_LIST_NAK nmsg;
		nmsg.m_byErrorCode = rt;
		pPlayer->SendPacket( &nmsg, sizeof(nmsg) );
	}
}

#endif // !defined _NA_0_20110613_GAME_SHOP_RENEWAL
