#include "StdAfx.h"
#include ".\handler_cg_shop.h"

#ifdef _NA_0_20110613_GAME_SHOP_RENEWAL

#include ".\Player.h"
#include ".\PlayerManager.h"
#include <PacketStruct_CG.h>
#include <PacketStruct_DG.h>
#include "./GameShop/GameShop.Manager.h"
#include "./GameShop/GameShop.EventProcessor.h"


const nsPacket::PacketHandlerInfo Handler_CG_SHOP::GetPacketHandlerNodes()
{
#define HANDLER_NODE_INFO(c, p, phase) { MAKEWORD(c, p), phase, Handler_CG_SHOP::On##p }
#define HANDLER_NODE_INFO_EOR() { 0, static_cast<ePHASE_SHIFT>(0), NULL }

    static const nsPacket::PacketHandlerNode handlers_static_[] =
    {
        HANDLER_NODE_INFO(CG_SHOP,  CG_SHOP_VERSION_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_SHOP,  CG_SHOP_OPEN_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_SHOP,  CG_SHOP_CLOSE_NTF, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_SHOP,  CG_SHOP_CASH_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_SHOP,  CG_SHOP_BUY_STORAGE_LIST_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_SHOP,  CG_SHOP_GIFT_STORAGE_LIST_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_SHOP,  CG_SHOP_BUY_ITEM_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_SHOP,  CG_SHOP_SEND_GIFT_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_SHOP,  CG_SHOP_USE_STORAGE_ITEM_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_SHOP,  CG_SHOP_CHAR_NAME_CHECK_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO(CG_SHOP,  CG_SHOP_EXIST_GIFT_SYN, PHASE_SERVICE),
        HANDLER_NODE_INFO_EOR() // end of records
    };
    //
    BOOST_STATIC_ASSERT(_countof(handlers_static_) >= 1);
    nsPacket::PacketHandlerInfo return_value = { handlers_static_, _countof(handlers_static_) - 1 };
    //
    return return_value;
};
//--------------------------------------------------------------------------------------------------

Handler_CG_SHOP_IMPL(CG_SHOP_VERSION_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));
    MSG_CG_SHOP_VERSION_SYN* recv_message = (MSG_CG_SHOP_VERSION_SYN*)pMsg;
    Player* player = PlayerManager::Instance()->FindPlayerByUserKey(recv_message->m_dwKey);
    ASSERT(player);
    if(!player) 
        return;

    GameShop::Manager::Instance()->Request_ScriptVersion(player);
}

Handler_CG_SHOP_IMPL(CG_SHOP_OPEN_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));
    MSG_CG_SHOP_OPEN_SYN* recv_message = (MSG_CG_SHOP_OPEN_SYN*)pMsg;
    Player* player = PlayerManager::Instance()->FindPlayerByUserKey(recv_message->m_dwKey);
    ASSERT(player);
    if(!player) 
        return;

    ShopScriptVersion script_version;
    script_version.sales_zone_ = recv_message->sales_version_;
    GameShop::Manager::Instance()->OpenShop(player, script_version);
}

Handler_CG_SHOP_IMPL(CG_SHOP_CLOSE_NTF)
{
    __TOUCHED((pServerSession, pMsg, wSize));
    MSG_CG_SHOP_CLOSE_NTF* recv_message = (MSG_CG_SHOP_CLOSE_NTF*)pMsg;
    Player* player = PlayerManager::Instance()->FindPlayerByUserKey(recv_message->m_dwKey);
    ASSERT(player);
    if(!player) 
        return;

    GameShop::Manager::Instance()->CloseShop(player);
}


Handler_CG_SHOP_IMPL(CG_SHOP_CASH_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));
    MSG_CG_SHOP_CASH_SYN* recv_message = (MSG_CG_SHOP_CASH_SYN*)pMsg;
    Player* player = PlayerManager::Instance()->FindPlayerByUserKey(recv_message->m_dwKey);
    ASSERT(player);
    if(!player) 
        return;

    USERGUID user_guid = player->GetUserGuid();
    GameShop::EventProcessor* proc = GameShop::Manager::Instance()->FindEventProcessor(user_guid);
    if (proc)
    {
        proc->Request_CashInfo();
    }
    else
    {
        GameShop::EventProcessor::SendFail_CashInfo(player, RC::RC_SHOP_CANNOT_USE_SHOP);
    }
}

Handler_CG_SHOP_IMPL(CG_SHOP_BUY_STORAGE_LIST_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));
    MSG_CG_SHOP_BUY_STORAGE_LIST_SYN* recv_message = (MSG_CG_SHOP_BUY_STORAGE_LIST_SYN*)pMsg;
    Player* player = PlayerManager::Instance()->FindPlayerByUserKey(recv_message->m_dwKey);
    ASSERT(player);
    if(!player) 
        return;

    USERGUID user_guid = player->GetUserGuid();
    GameShop::EventProcessor* proc = GameShop::Manager::Instance()->FindEventProcessor(user_guid);
    if (proc)
    {
        proc->Request_BuyStorageList(recv_message->page_, recv_message->item_count_per_page_);
    }
    else
    {
        GameShop::EventProcessor::SendFail_BuyStorageList(player, RC::RC_SHOP_CANNOT_USE_SHOP);
    }
}

Handler_CG_SHOP_IMPL(CG_SHOP_GIFT_STORAGE_LIST_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));
    MSG_CG_SHOP_GIFT_STORAGE_LIST_SYN* recv_message = (MSG_CG_SHOP_GIFT_STORAGE_LIST_SYN*)pMsg;
    Player* player = PlayerManager::Instance()->FindPlayerByUserKey(recv_message->m_dwKey);
    ASSERT(player);
    if(!player) 
        return;

    USERGUID user_guid = player->GetUserGuid();
    GameShop::EventProcessor* proc = GameShop::Manager::Instance()->FindEventProcessor(user_guid);
    if (proc)
    {
        proc->Request_GiftStorageList(recv_message->page_, recv_message->item_count_per_page_);
    }
    else
    {
        GameShop::EventProcessor::SendFail_GiftStorageList(player, RC::RC_SHOP_CANNOT_USE_SHOP);
    }
}

Handler_CG_SHOP_IMPL(CG_SHOP_BUY_ITEM_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));
    MSG_CG_SHOP_BUY_ITEM_SYN* recv_message = (MSG_CG_SHOP_BUY_ITEM_SYN*)pMsg;
    Player* player = PlayerManager::Instance()->FindPlayerByUserKey(recv_message->m_dwKey);
    ASSERT(player);
    if(!player) 
        return;
    
    USERGUID user_guid = player->GetUserGuid();
    GameShop::EventProcessor* proc = GameShop::Manager::Instance()->FindEventProcessor(user_guid);
    if (proc)
    {
        proc->Request_BuyItem(recv_message->item_info_, recv_message->item_count_);
    }
    else
    {
        GameShop::EventProcessor::SendFail_BuyItem(player, RC::RC_SHOP_CANNOT_USE_SHOP);
    }
}

Handler_CG_SHOP_IMPL(CG_SHOP_SEND_GIFT_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));
    MSG_CG_SHOP_SEND_GIFT_SYN* recv_message = (MSG_CG_SHOP_SEND_GIFT_SYN*)pMsg;
    Player* player = PlayerManager::Instance()->FindPlayerByUserKey(recv_message->m_dwKey);
    ASSERT(player);
    if(!player) 
        return;

    USERGUID user_guid = player->GetUserGuid();
    GameShop::EventProcessor* proc = GameShop::Manager::Instance()->FindEventProcessor(user_guid);
    if (proc)
    {
        proc->Request_SendGift(recv_message->item_info_,
                               recv_message->item_count_,
                               recv_message->recv_character_name_,
                               recv_message->gift_message_);
    }
    else
    {
        GameShop::EventProcessor::SendFail_SendGift(player, RC::RC_SHOP_CANNOT_USE_SHOP);
    }
}

Handler_CG_SHOP_IMPL(CG_SHOP_USE_STORAGE_ITEM_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));
    MSG_CG_SHOP_USE_STORAGE_ITEM_SYN* recv_message = (MSG_CG_SHOP_USE_STORAGE_ITEM_SYN*)pMsg;
    Player* player = PlayerManager::Instance()->FindPlayerByUserKey(recv_message->m_dwKey);
    ASSERT(player);
    if(!player) 
        return;

    USERGUID user_guid = player->GetUserGuid();
    GameShop::EventProcessor* proc = GameShop::Manager::Instance()->FindEventProcessor(user_guid);
    if (proc)
    {
        proc->Request_UseStorageItem(recv_message->use_item_);                                     
    }
    else
    {
        GameShop::EventProcessor::SendFail_UseStorageItem(player, RC::RC_SHOP_CANNOT_USE_SHOP);
    }
}

Handler_CG_SHOP_IMPL(CG_SHOP_CHAR_NAME_CHECK_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));
    MSG_CG_SHOP_CHAR_NAME_CHECK_SYN* recv_message = (MSG_CG_SHOP_CHAR_NAME_CHECK_SYN*)pMsg;
    Player* player = PlayerManager::Instance()->FindPlayerByUserKey(recv_message->m_dwKey);
    ASSERT(player);
    if(!player) 
        return;

    USERGUID user_guid = player->GetUserGuid();
    GameShop::EventProcessor* proc = GameShop::Manager::Instance()->FindEventProcessor(user_guid);
    if (proc)
    {
        proc->Request_CharacterNameCheck(recv_message->character_name_);
    }
    else
    {
        GameShop::EventProcessor::SendFail_CharacterNameCheck(player, RC::RC_SHOP_CANNOT_USE_SHOP);
    }
}

Handler_CG_SHOP_IMPL(CG_SHOP_EXIST_GIFT_SYN)
{
    __TOUCHED((pServerSession, pMsg, wSize));
    MSG_CG_SHOP_EXIST_GIFT_SYN* recv_message = (MSG_CG_SHOP_EXIST_GIFT_SYN*)pMsg;
    Player* player = PlayerManager::Instance()->FindPlayerByUserKey(recv_message->m_dwKey);
    ASSERT(player);
    if(!player) 
        return;

    GameShop::Manager::Instance()->Request_ExistGiftNotify(player);
}

#endif //_NA_0_20110613_GAME_SHOP_RENEWAL
