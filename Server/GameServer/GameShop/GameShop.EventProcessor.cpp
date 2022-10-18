#include "stdafx.h"

#ifdef _NA_0_20110613_GAME_SHOP_RENEWAL
#include <atlconv.h>
#include <wchar.h>

#include "SCItemSlotContainer.h"

#include "GameShop.EventProcessor.h"
#include "GameShop.EventHandler.h"
#include "GameShop.System.h"
#include "GameShop.Manager.h"
#include "ItemManager.h"

namespace GameShop
{

EventProcessor::EventProcessor(USERGUID user_guid, GameShop::System* shop_system)
: user_guid_(user_guid)
, shop_system_(shop_system)
, is_link_(false)
{
}

EventProcessor::~EventProcessor()
{
}

RC::eSHOP_RESULT EventProcessor::TransBuyItemResult(int result_code)
{
#ifdef _GLOBAL
    switch (result_code)
    {
    case -3: 
        return RC::RC_SHOP_OUTBOUND_MODULE_ERROR;
    case -2:
    case -1: 
        return RC::RC_SHOP_SHOPSERVER_DB_FAILED;
    case 0: 
        return RC::RC_SHOP_SUCCESS;
    case 1: 
        return RC::RC_SHOP_NOT_ENOUGH_CASH;
    case 2: 
        return RC::RC_SHOP_BUY_IMPOSSIBLE;
    case 3:
        return RC::RC_SHOP_LIMIT_CASH_TO_GIFT;
    case 4: 
        return RC::RC_SHOP_ITEM_EMPTY;
    case 5: 
        return RC::RC_SHOP_SELL_DATE_END;
    case 6: 
        return RC::RC_SHOP_SELL_END;
    case 7: 
        return RC::RC_SHOP_BUY_IMPOSSIBLE;
    case 8: 
        return RC::RC_SHOP_EVENT_ITEM_BUY_IMPOSSIBLE;
    case 9: 
        return RC::RC_SHOP_EVENT_ITEM_BUY_COUNT_OVER;
    case 10:
        return RC::RC_SHOP_DIFFERENT_CASH_TYPE;
    case 11:
        return RC::RC_SHOP_BUY_COOL_TIME_FAIL;
    }
#else
    switch (result_code)
    {
    case -3: 
        return RC::RC_SHOP_OUTBOUND_MODULE_ERROR;
    case -2:
    case -1: 
        return RC::RC_SHOP_SHOPSERVER_DB_FAILED;
    case 0: 
        return RC::RC_SHOP_SUCCESS;
    case 1: 
        return RC::RC_SHOP_NOT_ENOUGH_CASH;
    case 2: 
        return RC::RC_SHOP_BUY_IMPOSSIBLE;
    case 3: 
        return RC::RC_SHOP_ITEM_EMPTY;
    case 4: 
        return RC::RC_SHOP_SELL_DATE_END;
    case 5: 
        return RC::RC_SHOP_SELL_END;
    case 6: 
        return RC::RC_SHOP_BUY_IMPOSSIBLE;
    case 7: 
        return RC::RC_SHOP_EVENT_ITEM_BUY_IMPOSSIBLE;
    case 8: 
        return RC::RC_SHOP_EVENT_ITEM_BUY_COUNT_OVER;
    case 9: 
        return RC::RC_SHOP_BUY_COOL_TIME_FAIL;
    }
#endif
    return RC::RC_SHOP_FAILED;
}

RC::eSHOP_RESULT EventProcessor::TransSendGiftResult(int result_code)
{
    switch (result_code)
    {
    case -3: return RC::RC_SHOP_OUTBOUND_MODULE_ERROR;
    case -2:
    case -1: return RC::RC_SHOP_SHOPSERVER_DB_FAILED;
    case 0: return RC::RC_SHOP_SUCCESS;
    case 1: return RC::RC_SHOP_NOT_ENOUGH_CASH;
    case 2: return RC::RC_SHOP_GIFT_IMPOSSIBLE;
    case 3: return RC::RC_SHOP_LIMIT_CASH_TO_GIFT;
    case 4: return RC::RC_SHOP_ITEM_EMPTY;
    case 5: return RC::RC_SHOP_SELL_DATE_END;
    case 6: return RC::RC_SHOP_SELL_END;
    case 7: return RC::RC_SHOP_GIFT_IMPOSSIBLE;
    case 8: return RC::RC_SHOP_EVENT_ITEM_BUY_IMPOSSIBLE;
    case 9: return RC::RC_SHOP_EVENT_ITEM_BUY_COUNT_OVER;
    case 10:
        {
            #ifdef _GLOBAL
                return RC::RC_SHOP_DIFFERENT_CASH_TYPE;
            #else
                return RC::RC_SHOP_CANNOT_GIFT_ONESELF;
            #endif
        }
        break;
    case 11: return RC::RC_SHOP_BUY_COOL_TIME_FAIL;
    }
    return RC::RC_SHOP_FAILED;
}

bool EventProcessor::ClearGarbage(uint delta_tick)
{
    uint dead_lock_transaction = 0;
    transaction_.Update(delta_tick, dead_lock_transaction);
    if (dead_lock_transaction)
    {
        shop_system_->Log("[GameShop]Dead Lock Transaction : %d", dead_lock_transaction);
    }
    return (transaction_.Get() || is_link_) ? false : true;
}

//==================================================================================
//
//
//
//==================================================================================
void EventProcessor::SendFail_Open(Player* player, RC::eSHOP_RESULT result)
{
    MSG_CG_SHOP_OPEN_NAK nak_message;
    nak_message.error_code_ = result;
    player->SendPacket(&nak_message, sizeof(nak_message));
}

void EventProcessor::OpenShop()
{
    Player* player = PlayerManager::Instance()->FindPlayerByUserKey(user_guid_);
    if (player == NULL)
    {
        shop_system_->Log("[GameShop]OpenShop -> Not Found Player : %d", user_guid_);
        return;
    }

    // �������� ���� �ִ°�?
    GameShop::EventHandler& event_handler = shop_system_->get_event_handler();
    if (!event_handler.IsConnect())
    {
        SendFail_Open(player, RC::RC_SHOP_SERVER_CLOSED);
        return;
    }

    MSG_CG_SHOP_OPEN_ACK ack_message;
    player->SendPacket(&ack_message, sizeof(ack_message));
}

//==================================================================================
//
//
//
//==================================================================================
void EventProcessor::CloseShop(RC::eSHOP_RESULT error_code)
{
    Player* player = PlayerManager::Instance()->FindPlayerByUserKey(user_guid_);
    if (player == NULL)
    {
        shop_system_->Log("[GameShop]CloseShop -> Not Found Player : %d", user_guid_);
        return;
    }

    MSG_CG_SHOP_CLOSE_NTF ntf_message;
    ntf_message.error_code_ = error_code;
    player->SendPacket(&ntf_message, sizeof(ntf_message));
}


//==================================================================================
//
//
//
//==================================================================================
void EventProcessor::SendFail_CashInfo(Player* player, RC::eSHOP_RESULT result)
{
    GameShop::EventProcessor* proc = GameShop::Manager::Instance()->FindEventProcessor(player->GetUserGuid());
    if (proc)
    {
        proc->transaction_.Sub(GameShop::Transaction::transaction_cash);
    }

    MSG_CG_SHOP_CASH_NAK nak_message;
    nak_message.error_code_ = result;
    player->SendPacket(&nak_message, sizeof(nak_message));
}

void EventProcessor::Request_CashInfo()
{
    Player* player = PlayerManager::Instance()->FindPlayerByUserKey(user_guid_);
    if (player == NULL)
    {
        shop_system_->Log("[GameShop]Request_CashInfo -> Not Found Player : %d", user_guid_);
        return;
    }


    // �������� ���� �ִ°�?
    GameShop::EventHandler& event_handler = shop_system_->get_event_handler();
    if (!event_handler.IsConnect())
    {
        SendFail_CashInfo(player, RC::RC_SHOP_SERVER_CLOSED);
        return;
    }

    if (!transaction_.CanUse(GameShop::Transaction::transaction_cash))
    {
        SendFail_CashInfo(player, RC::RC_SHOP_PROCESS_TRANSACTION);
        return;
    }

    // ���ϸ��� ����(MileageSection)
    // code : 533, ���
    // code : 534, PCr
    GameShop::ServerInfo& server_info = shop_system_->get_server_info();
#ifdef _JAPAN
    GAMECHUUSN usn = player->GetGameChuUsn();
    if (event_handler.InquireCash(server_info.game_code_, ShopProtocol::Outbound_SUN_JP, user_guid_, usn, 0, 0, true))
#elif defined(_GLOBAL)
    if (event_handler.InquireCash(1, user_guid_, server_info.game_code_, false, 0))
#elif defined(_RUSSIA)
    if (event_handler.InquireCash(server_info.game_code_, ShopProtocol::Outbound_SUN_RU, user_guid_, 0, 0, 0, true))
#elif defined(_CHINA)
    if (event_handler.InquireCash(server_info.game_code_, ShopProtocol::Outbound_SUN_CN, user_guid_, user_guid_, 0, 0, true))
#else // _KOREA
    if (event_handler.InquireCash(server_info.game_code_, ShopProtocol::Inbound, user_guid_, true, 0))
#endif
    {
        transaction_.Add(GameShop::Transaction::transaction_cash);
    }
    else
    {
        SendFail_CashInfo(player, RC::RC_SHOP_REQUEST_SHOPSERVER_FAIL);
    }
}

void EventProcessor::Receive_CashInfo(int result_code, const CashInfo& cash_info)
{
    // ��û�� Ʈ����� ����.
    transaction_.Sub(GameShop::Transaction::transaction_cash);

    Player* player = PlayerManager::Instance()->FindPlayerByUserKey(user_guid_);
    if (player == NULL)
    {
        shop_system_->Log("[GameShop]Receive_CashInfo -> Not Found Player : %d", user_guid_);
        return;
    }

    if (result_code != 0)
    {
        shop_system_->Log("[GameShop]Receive_CashInfo -> Result Fail : %d", result_code);
        SendFail_CashInfo(player, RC::RC_SHOP_SHOPSERVER_FAILED);
        return;
    }

    MSG_CG_SHOP_CASH_ACK ack_message;
    ack_message.cash_ = cash_info;
    player->SendPacket(&ack_message, sizeof(ack_message));
}





//==================================================================================
//
//
//
//==================================================================================
void EventProcessor::SendFail_BuyStorageList(Player* player, RC::eSHOP_RESULT result)
{
    GameShop::EventProcessor* proc = GameShop::Manager::Instance()->FindEventProcessor(player->GetUserGuid());
    if (proc)
    {
        proc->transaction_.Sub(GameShop::Transaction::transaction_buy_storage_list);
    }

    MSG_CG_SHOP_BUY_STORAGE_LIST_NAK nak_message;
    nak_message.error_code_ = result;
    player->SendPacket(&nak_message, sizeof(nak_message));
}


void EventProcessor::Request_BuyStorageList(int page, int item_count_per_page)
{
    Player* player = PlayerManager::Instance()->FindPlayerByUserKey(user_guid_);
    if (player == NULL)
    {
        shop_system_->Log("[GameShop]Request_BuyStorageList -> Not Found Player : %d", user_guid_);
        return;
    }

    // �������� ���� �ִ°�?
    GameShop::EventHandler& event_handler = shop_system_->get_event_handler();
    if (!event_handler.IsConnect())
    {
        SendFail_BuyStorageList(player, RC::RC_SHOP_SERVER_CLOSED);
        return;
    }

    if (!transaction_.CanUse(GameShop::Transaction::transaction_buy_storage_list))
    {
        SendFail_BuyStorageList(player, RC::RC_SHOP_PROCESS_TRANSACTION);
        return;
    }

    if (page <= 0 || item_count_per_page > BUY_STORAGE_ITEM_PER_PAGE)
    {
        SendFail_BuyStorageList(player, RC::RC_SHOP_STORAGE_PAGE_SIZE_FAIL);
        return;
    }

    // StorageType
    // A : ��ü���
    // S : ������ ���(���� ����)
    // G : ������ ���(���� ����)

    // NowPage
    // ��ȸ�� ������ ��ȣ(1�� ���� ~ N��)
    GameShop::ServerInfo& server_info = shop_system_->get_server_info();
    ShopScriptVersion& script_version = shop_system_->get_script_version();
    if (event_handler.InquireStorageListPageNoGiftMessageEx(user_guid_, 
                                                            server_info.game_code_,
                                                            script_version.sales_zone_.sales_zone_,
                                                            'S',
                                                            page,
                                                            item_count_per_page))
    {
        transaction_.Add(GameShop::Transaction::transaction_buy_storage_list);
    }
    else
    {
        SendFail_BuyStorageList(player, RC::RC_SHOP_REQUEST_SHOPSERVER_FAIL);
    }
}


void EventProcessor::Receive_BuyStorageList(int result_code, 
                                            char storage_type, 
                                            int page, 
                                            int total_page, 
                                            int total_count, 
                                            int list_count,
                                            ShopProtocol::STStorageNoGiftMessageEx* storage_list)
{
    transaction_.Sub(GameShop::Transaction::transaction_buy_storage_list);

    Player* player = PlayerManager::Instance()->FindPlayerByUserKey(user_guid_);
    if (player == NULL)
    {
        shop_system_->Log("[GameShop]Receive_BuyStorageList -> Not Found Player : %d", user_guid_);
        return;
    }

    if (result_code != 0)
    {
        shop_system_->Log("[GameShop]Receive_BuyStorageList -> Result Fail : %d", result_code);
        SendFail_BuyStorageList(player, RC::RC_SHOP_SHOPSERVER_FAILED);
        return;
    }

    if (page <= 0 || list_count > BUY_STORAGE_ITEM_PER_PAGE)
    {
        SendFail_BuyStorageList(player, RC::RC_SHOP_STORAGE_PAGE_SIZE_FAIL);
        return;
    }

    MSG_CG_SHOP_BUY_STORAGE_LIST_ACK ack_message;
    ack_message.page_ = page;
    ack_message.total_page_ = total_page;
    ack_message.total_item_count_ = total_count;
    ack_message.item_count_ = list_count;

    USES_CONVERSION;
    for (uint idx = 0 ; idx < list_count ; ++idx)
    {
        StorageItemInfoBuy& info = ack_message.item_info_[idx];
        info.storage_sequence_ = storage_list[idx].Seq;             // ������ ����
        info.storage_item_sequence_ = storage_list[idx].ItemSeq;    // ������ ��ǰ ���� or ĳ�� �׸� ����
        info.product_sequence_ = storage_list[idx].ProductSeq;      // ���� ��ǰ �ڵ�
        info.price_sequence_ = storage_list[idx].PriceSeq;          // ���� ��ȣ
        info.storage_item_type_ = storage_list[idx].ItemType;	    // ��ǰ ĳ�� ���� (P:��ǰ, C:ĳ��)
        info.delete_date_ = storage_list[idx].DeleteDate;           // ��ǰ ���� ��¥.
        _tcsncpy(info.buy_character_name_, W2A(storage_list[idx].SendAccountID),
                                            _countof(info.buy_character_name_));
    }
    player->SendPacket(&ack_message, ack_message.GetSize());
}


//==================================================================================
//
//
//
//==================================================================================
void EventProcessor::SendFail_GiftStorageList(Player* player, RC::eSHOP_RESULT result)
{
    GameShop::EventProcessor* proc = GameShop::Manager::Instance()->FindEventProcessor(player->GetUserGuid());
    if (proc)
    {
        proc->transaction_.Sub(GameShop::Transaction::transaction_gift_storage_list);
    }

    MSG_CG_SHOP_GIFT_STORAGE_LIST_NAK nak_message;
    nak_message.error_code_ = result;
    player->SendPacket(&nak_message, sizeof(nak_message));
}

void EventProcessor::Request_GiftStorageList(int page, int item_count_per_page)
{
    Player* player = PlayerManager::Instance()->FindPlayerByUserKey(user_guid_);
    if (player == NULL)
    {
        shop_system_->Log("[GameShop]Request_GiftStorageList -> Not Found Player : %d", user_guid_);
        return;
    }

    // �������� ���� �ִ°�?
    GameShop::EventHandler& event_handler = shop_system_->get_event_handler();
    if (!event_handler.IsConnect())
    {
        SendFail_GiftStorageList(player, RC::RC_SHOP_SERVER_CLOSED);
        return;
    }

    if (!transaction_.CanUse(GameShop::Transaction::transaction_gift_storage_list))
    {
        SendFail_GiftStorageList(player, RC::RC_SHOP_PROCESS_TRANSACTION);
        return;
    }

    if (page <= 0 || item_count_per_page > GIFT_STORAGE_ITEM_PER_PAGE)
    {
        SendFail_GiftStorageList(player, RC::RC_SHOP_STORAGE_PAGE_SIZE_FAIL);
        return;
    }

    // StorageType
    // A : ��ü���
    // S : ������ ���(���� ����)
    // G : ������ ���(���� ����)

    // NowPage
    // ��ȸ�� ������ ��ȣ(1�� ���� ~ N��)
    GameShop::ServerInfo& server_info = shop_system_->get_server_info();
    ShopScriptVersion& script_version = shop_system_->get_script_version();
    if (event_handler.InquireStorageListPageEx(user_guid_, 
                                               server_info.game_code_,
                                               script_version.sales_zone_.sales_zone_,
                                               'G',
                                               page,
                                               item_count_per_page))
    {
        transaction_.Add(GameShop::Transaction::transaction_gift_storage_list);
    }
    else
    {
        SendFail_GiftStorageList(player, RC::RC_SHOP_REQUEST_SHOPSERVER_FAIL);
    }
}

void EventProcessor::Receive_GiftStorageList(int result_code, 
                                             char storage_type, 
                                             int page, 
                                             int total_page, 
                                             int total_count, 
                                             int list_count,
                                             ShopProtocol::STStorageEx* storage_list)
{
    //--------------------------------------------------------------------------------------------------
    // ������ ���� ������ ������ ���� ���� ����Ʈ�� ��ȸ�� ���.
    // �������� ��Ȳ�� ���� ���⿡ ������.
    if (transaction_.Get() & GameShop::Transaction::transaction_exist_gift_notify)
    {
        bool exist = (total_count > 0) ? true : false;
        Receive_ExistGiftList(exist);
        return;
    }

    transaction_.Sub(GameShop::Transaction::transaction_gift_storage_list);

    Player* player = PlayerManager::Instance()->FindPlayerByUserKey(user_guid_);
    if (player == NULL)
    {
        shop_system_->Log("[GameShop]Receive_GiftStorageList -> Not Found Player : %d", user_guid_);
        return;
    }

    if (result_code != 0)
    {
        shop_system_->Log("[GameShop]Receive_GiftStorageList -> Result Fail : %d", result_code);
        SendFail_GiftStorageList(player, RC::RC_SHOP_SHOPSERVER_FAILED);
        return;
    }

    if (page <= 0 || list_count > GIFT_STORAGE_ITEM_PER_PAGE)
    {
        SendFail_GiftStorageList(player, RC::RC_SHOP_STORAGE_PAGE_SIZE_FAIL);
        return;
    }

    USES_CONVERSION;
    MSG_CG_SHOP_GIFT_STORAGE_LIST_ACK ack_message;
    ack_message.page_ = page;
    ack_message.total_page_ = total_page;
    ack_message.total_item_count_ = total_count;
    ack_message.item_count_ = list_count;
    for (uint idx = 0 ; idx < list_count ; ++idx)
    {
        StorageItemInfoGift& info = ack_message.item_info_[idx];
        info.storage_sequence_ = storage_list[idx].Seq;             // ������ ����
        info.storage_item_sequence_ = storage_list[idx].ItemSeq;    // ������ ��ǰ ���� or ĳ�� �׸� ����
        info.product_sequence_ = storage_list[idx].ProductSeq;      // ���� ��ǰ �ڵ�
        info.price_sequence_ = storage_list[idx].PriceSeq;          // ���� ��ȣ
        info.storage_item_type_ = storage_list[idx].ItemType;	    // ��ǰ ĳ�� ���� (P:��ǰ, C:ĳ��)
        info.delete_date_ = storage_list[idx].DeleteDate;           // ��ǰ ���� ��¥.
        _tcsncpy(info.send_character_name_, W2A(storage_list[idx].SendAccountID), _countof(info.send_character_name_));
        _tcsncpy(info.gift_message_, W2A(storage_list[idx].SendMessage), _countof(info.gift_message_));
        info.recv_character_name_[0] = '\0'; 
    }
    player->SendPacket(&ack_message, ack_message.GetSize());
}





//==================================================================================
//
//
//
//==================================================================================
void EventProcessor::SendFail_BuyItem(Player* player, RC::eSHOP_RESULT result)
{
    GameShop::EventProcessor* proc = GameShop::Manager::Instance()->FindEventProcessor(player->GetUserGuid());
    if (proc)
    {
        proc->transaction_.Sub(GameShop::Transaction::transaction_buy_item);
    }

    MSG_CG_SHOP_BUY_ITEM_NAK nak_message;
    nak_message.error_code_ = result;
    player->SendPacket(&nak_message, sizeof(nak_message));
}

void EventProcessor::Request_BuyItem(ShopItemInfo* shop_item, uint item_count)
{
    Player* player = PlayerManager::Instance()->FindPlayerByUserKey(user_guid_);
    if (player == NULL)
    {
        shop_system_->Log("[GameShop]Request_BuyItem -> Not Found Player : %d", user_guid_);
        return;
    }

    // �������� ���� �ִ°�?
    GameShop::EventHandler& event_handler = shop_system_->get_event_handler();
    if (!event_handler.IsConnect())
    {
        SendFail_BuyItem(player, RC::RC_SHOP_SERVER_CLOSED);
        return;
    }

    if (!transaction_.CanUse(GameShop::Transaction::transaction_buy_item))
    {
        SendFail_BuyItem(player, RC::RC_SHOP_PROCESS_TRANSACTION);
        return;
    }

    if (item_count <= 0 || item_count > SHOP_MAX_ITEM_BUY_COUNT)
    {
        SendFail_BuyItem(player, RC::RC_SHOP_BUY_ITEM_SIZE_FAIL);
        return;
    }

    // ������ �����Ϸ��� �������� ��� ������ �����Ѵ�.
    if (!item_relay_.Push(shop_item, item_count))
    {
        SendFail_BuyItem(player, RC::RC_SHOP_BUY_ITEM_RELAY_FAIL);
        return;
    }


    // ������ ����, ���� ù ��° ������ ���� ��û.
    // ���� ���� ����(����/����)�� ���� �������� ���� ��û�Ѵ�.
    // ��� �������� ���Ű� �Ϸ�� �� ���� ������ Ŭ���̾�Ʈ�� ������.
    ShopItemInfo* item = item_relay_.Get();
    BASE_PLAYERINFO* player_info = player->GetCharInfo();
    GameShop::ServerInfo& server_info = shop_system_->get_server_info();
    ShopScriptVersion& script_version = shop_system_->get_script_version();

    USES_CONVERSION;
#ifdef _JAPAN
    GAMECHUUSN usn = player->GetGameChuUsn();
    if (event_handler.BuyProduct(server_info.game_code_,
                                 ShopProtocol::Outbound_SUN_JP,
                                 user_guid_,
                                 usn,
                                 item->package_product_sequence_,
                                 item->price_sequence_,
                                 script_version.sales_zone_.sales_zone_,
                                 item->product_display_sequence_,
                                 player_info->m_byClassCode,
                                 player_info->m_LV,
                                 A2W(player_info->m_tszCharName),
                                 A2W(""),
                                 server_info.server_index_,
                                 inet_addr(player->GetUserIP()), 
                                 false))
#elif defined(_GLOBAL)
    int cash_type = 508; // ĳ��.
    if (item->cash_type_ == 1)
        cash_type = 509; // pp_cash
    if (event_handler.BuyProduct(user_guid_,
                                 server_info.game_code_,
                                 item->package_product_sequence_,
                                 item->price_sequence_,
                                 script_version.sales_zone_.sales_zone_,
                                 item->product_display_sequence_,
                                 player_info->m_byClassCode,
                                 player_info->m_LV,
                                 A2W(player_info->m_tszCharName),
                                 A2W(""),
                                 server_info.server_index_,
                                 cash_type,
                                 false,
                                 inet_addr(player->GetUserIP())))
#elif defined(_RUSSIA)
    if (event_handler.BuyProduct(server_info.game_code_,
                                ShopProtocol::Outbound_SUN_RU,
                                user_guid_,
                                0,
                                item->package_product_sequence_,
                                item->price_sequence_,
                                script_version.sales_zone_.sales_zone_,
                                item->product_display_sequence_,
                                player_info->m_byClassCode,
                                player_info->m_LV,
                                A2W(player_info->m_tszCharName),
                                A2W(""),
                                server_info.server_index_,
                                inet_addr(player->GetUserIP()), 
                                false))
#elif defined(_CHINA)
    if (event_handler.BuyProduct(server_info.game_code_,
                                ShopProtocol::Outbound_SUN_CN,
                                user_guid_,
                                user_guid_,
                                item->package_product_sequence_,
                                item->price_sequence_,
                                script_version.sales_zone_.sales_zone_,
                                item->product_display_sequence_,
                                player_info->m_byClassCode,
                                player_info->m_LV,
                                A2W(player_info->m_tszCharName),
                                A2W(""),
                                server_info.server_index_,
                                inet_addr(player->GetUserIP()), 
                                false))
#else // _KOREA
    if (event_handler.BuyProduct(server_info.game_code_, 
                                 user_guid_,
                                 item->package_product_sequence_,
                                 item->product_display_sequence_,
                                 script_version.sales_zone_.sales_zone_,
                                 item->price_sequence_,
                                 player_info->m_byClassCode,
                                 player_info->m_LV,
                                 inet_addr(player->GetUserIP()),
                                 A2W(player_info->m_tszCharName),
                                 A2W(""),
                                 server_info.server_index_))
#endif
    {
        transaction_.Add(GameShop::Transaction::transaction_buy_item);
    }
    else
    {
        item_relay_.Destroy(NULL, NULL);
        SendFail_BuyItem(player, RC::RC_SHOP_REQUEST_SHOPSERVER_FAIL);
    }
}

void EventProcessor::Receive_BuyItem(int result_code, long product_left_count)
{
    transaction_.Sub(GameShop::Transaction::transaction_buy_item);

    Player* player = PlayerManager::Instance()->FindPlayerByUserKey(user_guid_);
    if (player == NULL)
    {
        shop_system_->Log("[GameShop]Receive_BuyItem -> Not Found Player : %d", user_guid_);
        return;
    }

    // ������ ���´� ������ ��� ����.
    item_relay_.SaveResult(TransBuyItemResult(result_code), product_left_count);

    // ������ �����̿��� ���� ���õ� �������� ����.
    item_relay_.Pop();



    // ������ �����̿� ������ �������� �����ϴ� ����
    // ����ؼ� ���� ��û�� �Ѵ�.
    if (item_relay_.IsEmpty())
    {
        MSG_CG_SHOP_BUY_ITEM_ACK ack_message;
        item_relay_.Destroy(ack_message.item_info_, &(ack_message.item_count_));
        player->SendPacket(&ack_message, ack_message.GetSize());
    }
    else
    {
        // �������� ���� �ִ°�?
        GameShop::EventHandler& event_handler = shop_system_->get_event_handler();
        if (!event_handler.IsConnect())
        {
            SendFail_BuyItem(player, RC::RC_SHOP_SERVER_CLOSED);
            return;
        }

        ShopItemInfo* item = item_relay_.Get();
        BASE_PLAYERINFO* player_info = player->GetCharInfo();
        GameShop::ServerInfo& server_info = shop_system_->get_server_info();
        ShopScriptVersion& script_version = shop_system_->get_script_version();

        USES_CONVERSION;
    #ifdef _JAPAN
        GAMECHUUSN usn = player->GetGameChuUsn();
        if (event_handler.BuyProduct(server_info.game_code_,
                                    ShopProtocol::Outbound_SUN_JP,
                                    user_guid_,
                                    usn,
                                    item->package_product_sequence_,
                                    item->price_sequence_,
                                    script_version.sales_zone_.sales_zone_,
                                    item->product_display_sequence_,
                                    player_info->m_byClassCode,
                                    player_info->m_LV,
                                    A2W(player_info->m_tszCharName),
                                    A2W(""),
                                    server_info.server_index_,
                                    inet_addr(player->GetUserIP()), 
                                    false))
    #elif defined(_GLOBAL)
        int cash_type = 508; // ĳ��.
        if (item->cash_type_ == 1)
            cash_type = 509; // pp_cash
        if (event_handler.BuyProduct(user_guid_,
                                    server_info.game_code_,
                                    item->package_product_sequence_,
                                    item->price_sequence_,
                                    script_version.sales_zone_.sales_zone_,
                                    item->product_display_sequence_,
                                    player_info->m_byClassCode,
                                    player_info->m_LV,
                                    A2W(player_info->m_tszCharName),
                                    A2W(""),
                                    server_info.server_index_,
                                    cash_type,
                                    false,
									inet_addr(player->GetUserIP())))
    #elif defined(_RUSSIA)
        if (event_handler.BuyProduct(server_info.game_code_,
                                    ShopProtocol::Outbound_SUN_RU,
                                    user_guid_,
                                    0,
                                    item->package_product_sequence_,
                                    item->price_sequence_,
                                    script_version.sales_zone_.sales_zone_,
                                    item->product_display_sequence_,
                                    player_info->m_byClassCode,
                                    player_info->m_LV,
                                    A2W(player_info->m_tszCharName),
                                    A2W(""),
                                    server_info.server_index_,
                                    inet_addr(player->GetUserIP()), 
                                    false))
    #elif defined(_CHINA)
        if (event_handler.BuyProduct(server_info.game_code_,
                                    ShopProtocol::Outbound_SUN_CN,
                                    user_guid_,
                                    user_guid_,
                                    item->package_product_sequence_,
                                    item->price_sequence_,
                                    script_version.sales_zone_.sales_zone_,
                                    item->product_display_sequence_,
                                    player_info->m_byClassCode,
                                    player_info->m_LV,
                                    A2W(player_info->m_tszCharName),
                                    A2W(""),
                                    server_info.server_index_,
                                    inet_addr(player->GetUserIP()), 
                                    false))
    #else // _KOREA
        if (event_handler.BuyProduct(server_info.game_code_, 
                                    user_guid_,
                                    item->package_product_sequence_,
                                    item->product_display_sequence_,
                                    script_version.sales_zone_.sales_zone_,
                                    item->price_sequence_,
                                    player_info->m_byClassCode,
                                    player_info->m_LV,
                                    inet_addr(player->GetUserIP()),
                                    A2W(player_info->m_tszCharName),
                                    A2W(""),
                                    server_info.server_index_))
    #endif
        {
            transaction_.Add(GameShop::Transaction::transaction_buy_item);
#ifdef _NA_007288_20140611_GAMELOG_GAMESHOP
            /*SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"] : GameShop : BuyProduct Success! [PackSeq:%d/CatSeq:%d/PriceSeq:%d/", 
                item->package_product_sequence_, item->product_display_sequence_, item->price_sequence_);*/
#endif //_NA_007288_20140611_GAMELOG_GAMESHOP
        }
        else
        {
            MSG_CG_SHOP_BUY_ITEM_ACK ack_message;
            item_relay_.Destroy(ack_message.item_info_, &(ack_message.item_count_));
            player->SendPacket(&ack_message, ack_message.GetSize());
        }
    }
}



//==================================================================================
//
//
//
//==================================================================================
void EventProcessor::SendFail_SendGift(Player* player, RC::eSHOP_RESULT result)
{
    GameShop::EventProcessor* proc = GameShop::Manager::Instance()->FindEventProcessor(player->GetUserGuid());
    if (proc)
    {
        proc->transaction_.Sub(GameShop::Transaction::transaction_send_gift);
    }

    MSG_CG_SHOP_SEND_GIFT_NAK nak_message;
    nak_message.error_code_ = result;
    player->SendPacket(&nak_message, sizeof(nak_message));
#ifdef _NA_007288_20140611_GAMELOG_GAMESHOP
    SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"] : GameShop : Gift SendFail!");

    GAMELOG->LogShopItem(ITEM_CHARGE_SEND_GIFT_FAILED, player, 0, 0, result);
#endif //_NA_007288_20140611_GAMELOG_GAMESHOP
}

void EventProcessor::Request_SendGift(ShopItemInfo* shop_item, uint item_count, char* receiver_name, char* gift_message)
{
    Player* player = PlayerManager::Instance()->FindPlayerByUserKey(user_guid_);
    if (player == NULL)
    {
        shop_system_->Log("[GameShop]Request_SendGift -> Not Found Player : %d", user_guid_);
        return;
    }

    // �������� ���� �ִ°�?
    GameShop::EventHandler& event_handler = shop_system_->get_event_handler();
    if (!event_handler.IsConnect())
    {
        SendFail_SendGift(player, RC::RC_SHOP_SERVER_CLOSED);
        return;
    }

    if (!transaction_.CanUse(GameShop::Transaction::transaction_send_gift))
    {
        SendFail_SendGift(player, RC::RC_SHOP_PROCESS_TRANSACTION);
        return;
    }

    if (item_count <= 0 || item_count > SHOP_MAX_ITEM_GIFT_COUNT)
    {
        SendFail_SendGift(player, RC::RC_SHOP_GIFT_ITEM_SIZE_FAIL);
        return;
    }


    // ���� ������ ��û�� ĳ������ ���� ���� Ȯ�� �������� �����Ѱ��� ��ġ�ؾ��Ѵ�.
    if (!gift_receiver_.MessageTo(receiver_name, gift_message))
    {
        SendFail_SendGift(player, RC::RC_SHOP_GIFT_RECEIVER_NOT_FOUND);
        return;
    }


    // ������ ���� �Ϸ��� �������� ��� ������ �����Ѵ�.
    if (!item_relay_.Push(shop_item, item_count))
    {
        SendFail_SendGift(player, RC::RC_SHOP_GIFT_ITEM_RELAY_FAIL);
        return;
    }


    // ������ ����, ���� ù ��° ������ ���� ��û.
    // ���� ���� ����(����/����)�� ���� �������� ���� ��û�Ѵ�.
    // ��� �������� ���Ű� �Ϸ�� �� ���� ������ Ŭ���̾�Ʈ�� ������.

    ShopItemInfo* item = item_relay_.Get();
    BASE_PLAYERINFO* player_info = player->GetCharInfo();
    GameShop::ServerInfo& server_info = shop_system_->get_server_info();
    ShopScriptVersion& script_version = shop_system_->get_script_version();

    USES_CONVERSION;
#ifdef _JAPAN
    GAMECHUUSN usn = player->GetGameChuUsn();
    if (event_handler.GiftProduct(server_info.game_code_,
                                  ShopProtocol::Outbound_SUN_JP,
                                  user_guid_,
                                  usn,
                                  A2W(player_info->m_tszCharName),
                                  server_info.server_index_,
                                  gift_receiver_.user_guid_,
                                  A2W(gift_receiver_.character_name_),
                                  server_info.server_index_,
                                  A2W(gift_receiver_.gift_message_),
                                  item->package_product_sequence_,
                                  item->price_sequence_,
                                  script_version.sales_zone_.sales_zone_,
                                  item->product_display_sequence_,
                                  inet_addr(player->GetUserIP()), 
                                  false))

#elif defined(_GLOBAL)
    int cash_type = 508; // ĳ��.
    if (item->cash_type_ == 1)
        cash_type = 509;

    if (event_handler.GiftProduct(user_guid_,
                                  A2W(player_info->m_tszCharName),
                                  server_info.server_index_,
                                  gift_receiver_.user_guid_,
                                  A2W(gift_receiver_.character_name_),
                                  server_info.server_index_,
                                  A2W(gift_receiver_.gift_message_),
                                  server_info.game_code_,
                                  item->package_product_sequence_,
                                  item->price_sequence_,
                                  script_version.sales_zone_.sales_zone_,
                                  item->product_display_sequence_,
                                  cash_type,
                                  false,
                                  inet_addr(player->GetUserIP())))
#elif defined(_RUSSIA)
    if (event_handler.GiftProduct(server_info.game_code_,
                                ShopProtocol::Outbound_SUN_RU,
                                user_guid_,
                                0,
                                A2W(player_info->m_tszCharName),
                                server_info.server_index_,
                                gift_receiver_.user_guid_,
                                A2W(gift_receiver_.character_name_),
                                server_info.server_index_,
                                A2W(gift_receiver_.gift_message_),
                                item->package_product_sequence_,
                                item->price_sequence_,
                                script_version.sales_zone_.sales_zone_,
                                item->product_display_sequence_,
                                inet_addr(player->GetUserIP()), 
                                false))
#elif defined(_CHINA)
    if (event_handler.GiftProduct(server_info.game_code_,
                                ShopProtocol::Outbound_SUN_CN,
                                user_guid_,
                                user_guid_,
                                A2W(player_info->m_tszCharName),
                                server_info.server_index_,
                                gift_receiver_.user_guid_,
                                A2W(gift_receiver_.character_name_),
                                server_info.server_index_,
                                A2W(gift_receiver_.gift_message_),
                                item->package_product_sequence_,
                                item->price_sequence_,
                                script_version.sales_zone_.sales_zone_,
                                item->product_display_sequence_,
                                inet_addr(player->GetUserIP()), 
                                false))
#else // KOREA
    if (event_handler.GiftProduct(server_info.game_code_, 
                                  user_guid_,
                                  server_info.server_index_,
                                  A2W(player_info->m_tszCharName),
                                  gift_receiver_.user_guid_,
                                  server_info.server_index_,
                                  A2W(gift_receiver_.character_name_),
                                  A2W(gift_receiver_.gift_message_),
                                  item->package_product_sequence_,
                                  item->price_sequence_,
                                  script_version.sales_zone_.sales_zone_,
                                  item->product_display_sequence_,
                                  inet_addr(player->GetUserIP())))
#endif
    {
        transaction_.Add(GameShop::Transaction::transaction_send_gift);
    }
    else
    {
        item_relay_.Destroy(NULL, NULL);
        SendFail_SendGift(player, RC::RC_SHOP_REQUEST_SHOPSERVER_FAIL);
    }
}

void EventProcessor::Receive_SendGift(int result_code,
                                      USERGUID receiver_guid,
                                      double limited_cash,
                                      long product_left_count)
{
    transaction_.Sub(GameShop::Transaction::transaction_send_gift);

    Player* player = PlayerManager::Instance()->FindPlayerByUserKey(user_guid_);
    if (player == NULL)
    {
        shop_system_->Log("[GameShop]Receive_SendGift -> Not Found Player : %d", user_guid_);
        return;
    }

    if (result_code != 0)
    {
        shop_system_->Log("[GameShop]Receive_SendGift -> Shop Result [%d]", result_code);
    }

    // ������ ���´� ������ ��� ����.
    item_relay_.SaveResult(TransSendGiftResult(result_code), product_left_count);

#ifdef _NA_007288_20140611_GAMELOG_GAMESHOP
    // ���������� �Ϸ� �α�
    ShopItemInfoEx* gift_item = item_relay_.Get();
    /*SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"] : GameShop : Request GiftProduct Success! [PackSeq:%d/CatSeq:%d/PriceSeq:%d/", 
        gift_item->package_product_sequence_, gift_item->product_display_sequence_, gift_item->price_sequence_);*/

    GAMELOG->LogShopItem(ITEM_CHARGE_SEND_GIFT_SUCCESS, player, gift_item->price_sequence_, 0, 0);

    // ���� �ޱ� �Ϸ� �α�
    Player* receiver = PlayerManager::Instance()->FindPlayerByUserKey(gift_receiver_.user_guid_);

    /*SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"] : GameShop : Receive GiftProduct Success! [PackSeq:%d/CatSeq:%d/PriceSeq:%d/", 
        gift_item->package_product_sequence_, gift_item->product_display_sequence_, gift_item->price_sequence_);*/

    GAMELOG->LogShop_RecvItem(ITEM_CHARGE_ACTION_RECV_GIFT_SUCCESS, receiver, gift_item->package_product_sequence_, 0);
#endif //_NA_007288_20140611_GAMELOG_GAMESHOP

    // ������ �����̿��� ���� ���õ� �������� ����.
    item_relay_.Pop();

    // ������ �����̿� ������ �������� �����ϴ� ����
    // ����ؼ� ���� ��û�� �Ѵ�.
    if (item_relay_.IsEmpty())
    {
        MSG_CG_SHOP_SEND_GIFT_ACK ack_message;
        item_relay_.Destroy(ack_message.item_info_, &(ack_message.item_count_));
        strcpy(ack_message.recv_character_name_, gift_receiver_.character_name_);
        player->SendPacket(&ack_message, ack_message.GetSize());

        // DBProxy�� ���� ���� �޴� ������ �����ϴ� ���Ӽ����� �뺸.
        MSG_DG_OTHER_SHOP_RECV_GIFT_NTF ntf_message;
        ntf_message.m_UserGuid = gift_receiver_.user_guid_;
        ntf_message.m_CharGuid = gift_receiver_.character_guid_;
        g_pGameServer->SendToServer(GAME_DBPROXY, &ntf_message, sizeof(ntf_message));
    }
    else
    {
        // �������� ���� �ִ°�?
        GameShop::EventHandler& event_handler = shop_system_->get_event_handler();
        if (!event_handler.IsConnect())
        {
            SendFail_SendGift(player, RC::RC_SHOP_SERVER_CLOSED);
            return;
        }

        ShopItemInfo* item = item_relay_.Get();
        BASE_PLAYERINFO* player_info = player->GetCharInfo();
        GameShop::ServerInfo& server_info = shop_system_->get_server_info();
        ShopScriptVersion& script_version = shop_system_->get_script_version();

        USES_CONVERSION;
    #ifdef _JAPAN
        GAMECHUUSN usn = player->GetGameChuUsn();
        if (event_handler.GiftProduct(server_info.game_code_,
                                    ShopProtocol::Outbound_SUN_JP,
                                    user_guid_,
                                    usn,
                                    A2W(player_info->m_tszCharName),
                                    server_info.server_index_,
                                    gift_receiver_.user_guid_,
                                    A2W(gift_receiver_.character_name_),
                                    server_info.server_index_,
                                    A2W(gift_receiver_.gift_message_),
                                    item->package_product_sequence_,
                                    item->price_sequence_,
                                    script_version.sales_zone_.sales_zone_,
                                    item->product_display_sequence_,
                                    inet_addr(player->GetUserIP()), 
                                    false))

    #elif defined(_GLOBAL)
        int cash_type = 508; // ĳ��.
        if (item->cash_type_ == 1)
            cash_type = 509;

        if (event_handler.GiftProduct(user_guid_,
                                    A2W(player_info->m_tszCharName),
                                    server_info.server_index_,
                                    gift_receiver_.user_guid_,
                                    A2W(gift_receiver_.character_name_),
                                    server_info.server_index_,
                                    A2W(gift_receiver_.gift_message_),
                                    server_info.game_code_,
                                    item->package_product_sequence_,
                                    item->price_sequence_,
                                    script_version.sales_zone_.sales_zone_,
                                    item->product_display_sequence_,
                                    cash_type,
                                    false, 
									inet_addr(player->GetUserIP())))
    #elif defined(_RUSSIA)
        if (event_handler.GiftProduct(server_info.game_code_,
                                    ShopProtocol::Outbound_SUN_RU,
                                    user_guid_,
                                    0,
                                    A2W(player_info->m_tszCharName),
                                    server_info.server_index_,
                                    gift_receiver_.user_guid_,
                                    A2W(gift_receiver_.character_name_),
                                    server_info.server_index_,
                                    A2W(gift_receiver_.gift_message_),
                                    item->package_product_sequence_,
                                    item->price_sequence_,
                                    script_version.sales_zone_.sales_zone_,
                                    item->product_display_sequence_,
                                    inet_addr(player->GetUserIP()), 
                                    false))
    #elif defined(_CHINA)
        if (event_handler.GiftProduct(server_info.game_code_,
                                    ShopProtocol::Outbound_SUN_CN,
                                    user_guid_,
                                    user_guid_,
                                    A2W(player_info->m_tszCharName),
                                    server_info.server_index_,
                                    gift_receiver_.user_guid_,
                                    A2W(gift_receiver_.character_name_),
                                    server_info.server_index_,
                                    A2W(gift_receiver_.gift_message_),
                                    item->package_product_sequence_,
                                    item->price_sequence_,
                                    script_version.sales_zone_.sales_zone_,
                                    item->product_display_sequence_,
                                    inet_addr(player->GetUserIP()), 
                                    false))
    #else // KOREA
        if (event_handler.GiftProduct(server_info.game_code_, 
                                    user_guid_,
                                    server_info.server_index_,
                                    A2W(player_info->m_tszCharName),
                                    gift_receiver_.user_guid_,
                                    server_info.server_index_,
                                    A2W(gift_receiver_.character_name_),
                                    A2W(gift_receiver_.gift_message_),
                                    item->package_product_sequence_,
                                    item->price_sequence_,
                                    script_version.sales_zone_.sales_zone_,
                                    item->product_display_sequence_,
                                    inet_addr(player->GetUserIP())))
    #endif
        {
            transaction_.Add(GameShop::Transaction::transaction_send_gift);
            //���� �������� ������(item_relay_)�� ���� ���������Ƿ�, ���Կ�û�� �ٽ� �����Ų��
        }
        else
        {
            MSG_CG_SHOP_SEND_GIFT_ACK ack_message;
            item_relay_.Destroy(ack_message.item_info_, &(ack_message.item_count_));
            strcpy(ack_message.recv_character_name_, gift_receiver_.character_name_);
            player->SendPacket(&ack_message, ack_message.GetSize());
        }
    }
}




//==================================================================================
//
//
//
//==================================================================================

class ItemPropery
{
public:
    ItemPropery(SLOTCODE item_code, uchar property_count, ShopProtocol::STItemProperty* PropertyList, GameShop::System* shop_system, int cash_type)
    {
        item_code_ = item_code;
        item_type_ = KindOfItemType::Default;
        item_count_ = 1;
        enchant_count_ = 0;
        socket_count_ = 0;
        delete_date_ = 0;

        if (cash_type == 509)
            item_tag_.value = KindOfItemTag::Tag_PCoinPurchase;
        else
            item_tag_.value = KindOfItemTag::Tag_None;

        for (uint idx = 0 ; idx < property_count ; ++idx)
        {
            switch (PropertyList[idx].PropertySeq)
            {
            case 10: // �Ⱓ
                // �Ⱓ�� �������� �ʴ��� �ð� ��(���������� ���)
                //delete_date_ = PropertyList[idx].Value; // �ʴ���
                break;
            case 7: //����
                // �Ҹ� �������� ����(���������� ���)
                break;

            case 109: // �Ⱓ�� ������(��� �Ⱓ - 1�� ����)
                delete_date_ = PropertyList[idx].Value * 86400; //(60*60*24)
                break;
            case 113: // �Ⱓ�� ������(������ Ÿ��)
                item_type_ = PropertyList[idx].Value;
                break;
            case 116: // �Ⱓ�� ������(��æƮ ����)
                enchant_count_ = PropertyList[idx].Value;
                break;
            case 119: // �Ⱓ�� ������(���� ����)
                socket_count_ = PropertyList[idx].Value;
                break;

            case 110: // �Ҹ� ������(������ ����)
                item_count_ = PropertyList[idx].Value;
                break;
            case 111: // �Ҹ� ������(������ Ÿ��)
                item_type_ = PropertyList[idx].Value;
                break;
            case 114: // �Ҹ� ������(��æƮ ����)
                enchant_count_ = PropertyList[idx].Value;
                break;
            case 117: // �Ҹ� ������(���� ����)
                socket_count_ = PropertyList[idx].Value;
                break;

            case 112: // ���� ������ (������ Ÿ��)
                item_type_ = PropertyList[idx].Value;
                break;
            case 115: // ���� ������ (��æƮ ����)
                enchant_count_ = PropertyList[idx].Value;
                break;
            case 118: // ���� ������ (���� ����)
                socket_count_ = PropertyList[idx].Value;
                break;
            default:
                shop_system->Log("[GameShop::ItemProperty - Not Defined Property]property seq : %d, property value : %d", 
                    PropertyList[idx].PropertySeq, PropertyList[idx].Value);
                break;
            }
        }
    }

    ~ItemPropery()
    {
    }

    bool IsValid()
    {
        return (item_code_) ? true : false;
    }

    SLOTCODE item_code_;
    int item_type_;
    int item_count_;
    int enchant_count_;
    int socket_count_;
    uint delete_date_;
    KindOfItemTag item_tag_;
};



void EventProcessor::SendFail_UseStorageItem(Player* player, RC::eSHOP_RESULT result)
{
    GameShop::EventProcessor* proc = GameShop::Manager::Instance()->FindEventProcessor(player->GetUserGuid());
    if (proc)
    {
        proc->transaction_.Sub(GameShop::Transaction::transaction_use_storage_item);
    }

    MSG_CG_SHOP_USE_STORAGE_ITEM_NAK nak_message;
    nak_message.error_code_ = result;
    player->SendPacket(&nak_message, sizeof(nak_message));
}

void EventProcessor::Request_UseStorageItem(UseStorageItem& IN use_storage_item)
{
    Player* player = PlayerManager::Instance()->FindPlayerByUserKey(user_guid_);
    if (player == NULL)
    {
        shop_system_->Log("[GameShop]Request_UseStorageItem -> Not Found Player : %d", user_guid_);
        return;
    }

    // �������� ���� �ִ°�?
    GameShop::EventHandler& event_handler = shop_system_->get_event_handler();
    if (!event_handler.IsConnect())
    {
        SendFail_UseStorageItem(player, RC::RC_SHOP_SERVER_CLOSED);
        return;
    }

    if (!transaction_.CanUse(GameShop::Transaction::transaction_use_storage_item))
    {
        SendFail_UseStorageItem(player, RC::RC_SHOP_PROCESS_TRANSACTION);
        return;
    }

    // �κ��丮�� �� ������ �ִ°�?
    SCSlotContainer* inventory = player->GetItemManager()->GetItemSlotContainer(SI_INVENTORY);
    if (inventory->GetEmptySlotNum() <= 0)
    {
        SendFail_UseStorageItem(player, RC::RC_SHOP_INVENTORY_FULL);
        return;
    }

    ShopItemInfo* item = item_relay_.Get();
    BASE_PLAYERINFO* player_info = player->GetCharInfo();
    GameShop::ServerInfo& server_info = shop_system_->get_server_info();
    ShopScriptVersion& script_version = shop_system_->get_script_version();

    USES_CONVERSION;
#ifdef _GLOBAL
    if (event_handler.UseStorageEx(user_guid_,
#elif defined(_CHINA)
    if (event_handler.UseStorageExDeductMileageFlag(user_guid_,
#else
    if (event_handler.UseStorage(user_guid_,
#endif
                                   server_info.game_code_,
                                   inet_addr(player->GetUserIP()),
                                   use_storage_item.storage_sequence_, 
                                   use_storage_item.storage_item_sequence_, 
                                   use_storage_item.item_type_, 
                                   player_info->m_byClassCode,
                                   player_info->m_LV, 
                                   A2W(player_info->m_tszCharName), 
                                   A2W(""),
                                   server_info.server_index_))
    {
        transaction_.Add(GameShop::Transaction::transaction_use_storage_item);
        use_storage_item_ = use_storage_item;
    }
    else
    {
        SendFail_UseStorageItem(player, RC::RC_SHOP_REQUEST_SHOPSERVER_FAIL);
    }
}

void EventProcessor::Receive_UseStorageItem(int result_code, 
                                            uint game_item_code,
                                            int product_sequence, 
                                            int storage_sequence, 
                                            int storage_itemseguence, 
                                            uchar property_count, 
                                            ShopProtocol::STItemProperty* PropertyList, 
                                            int cash_type)
{
    transaction_.Sub(GameShop::Transaction::transaction_use_storage_item);

    Player* player = PlayerManager::Instance()->FindPlayerByUserKey(user_guid_);
    if (player == NULL)
    {
        shop_system_->Log("[GameShop]Receive_UseStorageItem -> Not Found Player : %d", user_guid_);
        return;
    }

    ItemPropery item_property(game_item_code, property_count, PropertyList, shop_system_, cash_type);
    if (!item_property.IsValid())
    {
        shop_system_->Log("[GameShop]User : %d, Property Count : %d", user_guid_, property_count);
        for (uint idx = 0 ; idx < property_count ; ++idx)
        {
            shop_system_->Log("[GameShop]Property Sequence : %d, Property Value : %d", PropertyList[idx].PropertySeq, PropertyList[idx].Value);
        }

        SendFail_UseStorageItem(player, RC::RC_SHOP_FAILED);
        return;
    }

    MSG_CG_SHOP_USE_STORAGE_ITEM_ACK ack_message;
    RC::eITEM_RESULT result = player->GetItemManager()->ObtainAtEmptySlot(
                                                                        item_property.item_tag_, 
                                                                        item_property.item_code_, 
                                                                        item_property.item_count_,
                                                                        item_property.enchant_count_,
                                                                        item_property.item_type_,
                                                                        item_property.socket_count_,
                                                                        item_property.delete_date_,
                                                                        &ack_message.inventory_info_);


    if (result == RC::RC_ITEM_SUCCESS)
    {
        // [������ ȹ�� ���� �α�]
        GAMELOG->LogUseStorageItem(ITEM_CHARGE_BUY_SUCCESS, 
                                   player, 
                                   item_property.item_code_, 
                                   item_property.item_count_, 
                                   item_property.delete_date_);

        player->SerializeInfoToDBProxy();

        use_storage_item_.storage_sequence_ = storage_sequence;
        use_storage_item_.storage_item_sequence_ = storage_itemseguence;

        ack_message.use_item_ = use_storage_item_;
        player->SendPacket(&ack_message, ack_message.GetSize());


        item_serial_.item_code_ = game_item_code;
        item_serial_.item_pos_ = ack_message.inventory_info_.m_Slot[0].m_Pos;
        item_serial_.storage_sequence_ = storage_sequence;
        item_serial_.storage_item_sequence_ = storage_itemseguence;

        transaction_.Add(GameShop::Transaction::transaction_item_serial_update);

        shop_system_->Log("[GameShop]Receive_UseStorageItem Success [code : %d, storage : %d, sequence : %d ]", 
                            game_item_code, storage_sequence, storage_itemseguence);
    }
    else
    {

        // [������ ȹ�� ���� �α�]
        GAMELOG->LogUseStorageItem(ITEM_CHARGE_BUY_FAILED, 
                                   player, 
                                   item_property.item_code_, 
                                   item_property.item_count_, 
                                   item_property.delete_date_);

        item_serial_.Clear();

        GameShop::EventHandler& event_handler = shop_system_->get_event_handler();
        GameShop::ServerInfo& server_info = shop_system_->get_server_info();
        
        bool roll_back = event_handler.RollbackUseStorage(user_guid_, 
                                                          server_info.game_code_, 
                                                          storage_sequence, 
                                                          storage_itemseguence);

        if (roll_back)
        {
            transaction_.Add(GameShop::Transaction::transaction_item_roll_back);

            shop_system_->Log("[GameShop]Receive_UseStorageItem -> Roll Back %d, %d, %d, %d", 
                                    user_guid_, server_info.game_code_, storage_sequence, storage_itemseguence);
        }
        else
        {
            shop_system_->Log("[GameShop]Receive_UseStorageItem -> Roll Back Fail %d, %d, %d, %d", 
                                    user_guid_, server_info.game_code_, storage_sequence, storage_itemseguence);
        }

        shop_system_->Log("[GameShop]Receive_UseStorageItem -> Error Code %d", result);
        SendFail_UseStorageItem(player, RC::RC_SHOP_INVENTORY_FULL);
    }
}

void EventProcessor::Receive_Item_RollBack(long result_code)
{
    transaction_.Sub(GameShop::Transaction::transaction_item_roll_back);

    shop_system_->Log("[GameShop]Receive_Item_RollBack -> User : %d, Result : %d", user_guid_, result_code);

    CloseShop(RC::RC_SHOP_FAILED);
}

void EventProcessor::Item_Serial_Update(const SCItemSlot& item_slot)
{
    if (item_serial_.item_code_ == item_slot.GetCode() &&
        item_serial_.item_pos_ == item_slot.GetPos())
    {
        GameShop::EventHandler& event_handler = shop_system_->get_event_handler();
        GameShop::ServerInfo& server_info = shop_system_->get_server_info();

        if (!event_handler.ItemSerialUpdate(user_guid_, 
                                            server_info.game_code_,
                                            item_serial_.storage_sequence_,
                                            item_serial_.storage_item_sequence_,
                                            item_slot.GetWorldSerial()))
        {
            shop_system_->Log("[GameShop]Item_Serial_Update FAIL !! -> Code %d, Serial %I64d", 
                                    item_serial_.item_code_, item_slot.GetSerial());
        }

        item_serial_.Clear();

        transaction_.Sub(GameShop::Transaction::transaction_item_serial_update);
    }
}


//==================================================================================
//
//
//
//==================================================================================
void EventProcessor::SendFail_CharacterNameCheck(Player* player, RC::eSHOP_RESULT result)
{
    GameShop::EventProcessor* proc = GameShop::Manager::Instance()->FindEventProcessor(player->GetUserGuid());
    if (proc)
    {
        proc->transaction_.Sub(GameShop::Transaction::transaction_character_name_check);    
        proc->gift_receiver_.Clear();
    }

    MSG_CG_SHOP_CHAR_NAME_CHECK_NAK nak_message;
    nak_message.error_code_ = result;
    player->SendPacket(&nak_message, sizeof(nak_message));
}


void EventProcessor::Request_CharacterNameCheck(const char* character_name)
{
    Player* player = PlayerManager::Instance()->FindPlayerByUserKey(user_guid_);
    if (player == NULL)
    {
        shop_system_->Log("[GameShop]Request_UseStorageItem -> Not Found Player : %d", user_guid_);
        return;
    }

    // �������� ���� �ִ°�?
    GameShop::EventHandler& event_handler = shop_system_->get_event_handler();
    if (!event_handler.IsConnect())
    {
        SendFail_CharacterNameCheck(player, RC::RC_SHOP_SERVER_CLOSED);
        return;
    }

    if (!transaction_.CanUse(GameShop::Transaction::transaction_character_name_check))
    {
        SendFail_CharacterNameCheck(player, RC::RC_SHOP_PROCESS_TRANSACTION);
        return;
    }

    MSG_DG_OTHER_EXIST_CHARNAME_SYN syn_message;
    syn_message.m_dwKey = user_guid_;
    syn_message.m_Type = eCHARNAME_ACTION_SHOP_SEND_GIFT;
    _tcsncpy(syn_message.m_tszCharName, character_name, _countof(syn_message.m_tszCharName));
    syn_message.m_tszCharName[MAX_CHARNAME_LENGTH]='\0';
    if (g_pGameServer->SendToServer(GAME_DBPROXY, &syn_message, sizeof(syn_message)))
    {
        transaction_.Add(GameShop::Transaction::transaction_character_name_check);
    }
    else
    {
        SendFail_CharacterNameCheck(player, RC::RC_SHOP_FAILED);
    }
}

void EventProcessor::Receive_CharacterNameCheck(USERGUID receive_user_guid,
                                                USERGUID receive_character_guid,
                                                int character_class,
                                                int character_level,
                                                const char* character_name, 
                                                const char* guild_name)
{
    transaction_.Sub(GameShop::Transaction::transaction_character_name_check);

    Player* player = PlayerManager::Instance()->FindPlayerByUserKey(user_guid_);
    if (player == NULL)
    {
        shop_system_->Log("[GameShop]Receive_UseStorageItem -> Not Found Player : %d", user_guid_);
        return;
    }

    gift_receiver_.Clear();
    gift_receiver_.ApplyReceiver(receive_user_guid, receive_character_guid, character_name, guild_name);

    MSG_CG_SHOP_CHAR_NAME_CHECK_ACK ack_message;
    ack_message.character_class_ = character_class;
    ack_message.character_level_ = character_level;

    uint size = 0;
    size = _countof(ack_message.character_name_);
    _tcsncpy(ack_message.character_name_, character_name, size);
    ack_message.character_name_[size-1] = '\0';

    size = _countof(ack_message.guild_name_);
    _tcsncpy(ack_message.guild_name_, guild_name, size);
    ack_message.guild_name_[size-1] = '\0';

    player->SendPacket(&ack_message, sizeof(ack_message));
}


void EventProcessor::Request_ExistGiftList()
{
    Player* player = PlayerManager::Instance()->FindPlayerByUserKey(user_guid_);
    if (player == NULL)
    {
        shop_system_->Log("[GameShop]Request_ExistGiftList -> Not Found Player : %d", user_guid_);
        return;
    }

    // �������� ���� �ִ°�?
    GameShop::EventHandler& event_handler = shop_system_->get_event_handler();
    if (!event_handler.IsConnect())
    {
        shop_system_->Log("[GameShop]Request_ExistGiftList -> Shop Server is Not Connected");
        MSG_CG_SHOP_EXIST_GIFT_NAK nak_message;
        player->SendPacket(&nak_message, sizeof(nak_message));
        return;
    }

    if (!transaction_.CanUse(GameShop::Transaction::transaction_exist_gift_notify))
    {
        shop_system_->Log("[GameShop]Request_ExistGiftList -> Transaction");
        MSG_CG_SHOP_EXIST_GIFT_NAK nak_message;
        player->SendPacket(&nak_message, sizeof(nak_message));
        return;
    }


    // StorageType
    // A : ��ü���
    // S : ������ ���(���� ����)
    // G : ������ ���(���� ����)

    // NowPage
    // ��ȸ�� ������ ��ȣ(1�� ���� ~ N��)
    GameShop::ServerInfo& server_info = shop_system_->get_server_info();
    ShopScriptVersion& script_version = shop_system_->get_script_version();
    if (event_handler.InquireStorageListPageEx(user_guid_, 
                                               server_info.game_code_,
                                               script_version.sales_zone_.sales_zone_,
                                               'G',
                                               1,
                                               1))
    {
        transaction_.Add(GameShop::Transaction::transaction_exist_gift_notify);
    }
    else
    {
        MSG_CG_SHOP_EXIST_GIFT_NAK nak_message;
        player->SendPacket(&nak_message, sizeof(nak_message));
    }
}

void EventProcessor::Receive_ExistGiftList(bool exist)
{
    transaction_.Sub(GameShop::Transaction::transaction_exist_gift_notify);

    Player* player = PlayerManager::Instance()->FindPlayerByUserKey(user_guid_);
    if (player == NULL)
    {
        shop_system_->Log("[GameShop]Receive_ExistGiftList -> Not Found Player : %d", user_guid_);
        return;
    }

    if (exist)
    {
        MSG_CG_SHOP_EXIST_GIFT_ACK ack_message;
        player->SendPacket(&ack_message, sizeof(ack_message));
    }
    else
    {
        MSG_CG_SHOP_EXIST_GIFT_NAK nak_message;
        player->SendPacket(&nak_message, sizeof(nak_message));
    }
}

}; //namespace GameShop

#endif // _NA_0_20110613_GAME_SHOP_RENEWAL