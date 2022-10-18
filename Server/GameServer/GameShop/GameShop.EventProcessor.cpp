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

    // 샵서버가 열려 있는가?
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


    // 샵서버가 열려 있는가?
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

    // 마일리지 유형(MileageSection)
    // code : 533, 고블린
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
    // 요청한 트랜잭션 해제.
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

    // 샵서버가 열려 있는가?
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
    // A : 전체목록
    // S : 보관함 목록(본인 구매)
    // G : 선물함 목록(받은 선물)

    // NowPage
    // 조회할 페이지 번호(1번 부터 ~ N번)
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
        info.storage_sequence_ = storage_list[idx].Seq;             // 보관함 순번
        info.storage_item_sequence_ = storage_list[idx].ItemSeq;    // 보관함 상품 순번 or 캐시 항목 순번
        info.product_sequence_ = storage_list[idx].ProductSeq;      // 단위 상품 코드
        info.price_sequence_ = storage_list[idx].PriceSeq;          // 가격 번호
        info.storage_item_type_ = storage_list[idx].ItemType;	    // 상품 캐시 구분 (P:상품, C:캐시)
        info.delete_date_ = storage_list[idx].DeleteDate;           // 상품 삭제 날짜.
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

    // 샵서버가 열려 있는가?
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
    // A : 전체목록
    // S : 보관함 목록(본인 구매)
    // G : 선물함 목록(받은 선물)

    // NowPage
    // 조회할 페이지 번호(1번 부터 ~ N번)
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
    // 선물의 존재 여부의 측정을 위해 선물 리스트를 조회할 경우.
    // 예외적인 상황에 의해 여기에 덧붙임.
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
        info.storage_sequence_ = storage_list[idx].Seq;             // 보관함 순번
        info.storage_item_sequence_ = storage_list[idx].ItemSeq;    // 보관함 상품 순번 or 캐시 항목 순번
        info.product_sequence_ = storage_list[idx].ProductSeq;      // 단위 상품 코드
        info.price_sequence_ = storage_list[idx].PriceSeq;          // 가격 번호
        info.storage_item_type_ = storage_list[idx].ItemType;	    // 상품 캐시 구분 (P:상품, C:캐시)
        info.delete_date_ = storage_list[idx].DeleteDate;           // 상품 삭제 날짜.
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

    // 샵서버가 열려 있는가?
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

    // 유저가 구입하려는 아이템의 모든 정보를 저장한다.
    if (!item_relay_.Push(shop_item, item_count))
    {
        SendFail_BuyItem(player, RC::RC_SHOP_BUY_ITEM_RELAY_FAIL);
        return;
    }


    // 아이템 구매, 제일 첫 번째 아이템 구매 요청.
    // 이후 구매 응답(성공/실패)시 다음 아이템을 구매 요청한다.
    // 모든 아이템의 구매가 완료된 후 구매 응답을 클라이언트로 보낸다.
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
    int cash_type = 508; // 캐시.
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

    // 이전에 보냈던 정보의 결과 저장.
    item_relay_.SaveResult(TransBuyItemResult(result_code), product_left_count);

    // 아이템 릴레이에서 현재 선택된 아이템을 제거.
    item_relay_.Pop();



    // 아이템 릴레이에 구입할 아이템이 존재하는 동안
    // 계속해서 구입 요청을 한다.
    if (item_relay_.IsEmpty())
    {
        MSG_CG_SHOP_BUY_ITEM_ACK ack_message;
        item_relay_.Destroy(ack_message.item_info_, &(ack_message.item_count_));
        player->SendPacket(&ack_message, ack_message.GetSize());
    }
    else
    {
        // 샵서버가 열려 있는가?
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
        int cash_type = 508; // 캐시.
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

    // 샵서버가 열려 있는가?
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


    // 선물 보내기 요청전 캐릭터의 존재 여부 확인 절차에서 저장한값과 일치해야한다.
    if (!gift_receiver_.MessageTo(receiver_name, gift_message))
    {
        SendFail_SendGift(player, RC::RC_SHOP_GIFT_RECEIVER_NOT_FOUND);
        return;
    }


    // 유저가 선물 하려는 아이템의 모든 정보를 저장한다.
    if (!item_relay_.Push(shop_item, item_count))
    {
        SendFail_SendGift(player, RC::RC_SHOP_GIFT_ITEM_RELAY_FAIL);
        return;
    }


    // 아이템 구매, 제일 첫 번째 아이템 구매 요청.
    // 이후 구매 응답(성공/실패)시 다음 아이템을 구매 요청한다.
    // 모든 아이템의 구매가 완료된 후 구매 응답을 클라이언트로 보낸다.

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
    int cash_type = 508; // 캐시.
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

    // 이전에 보냈던 정보의 결과 저장.
    item_relay_.SaveResult(TransSendGiftResult(result_code), product_left_count);

#ifdef _NA_007288_20140611_GAMELOG_GAMESHOP
    // 선물보내기 완료 로그
    ShopItemInfoEx* gift_item = item_relay_.Get();
    /*SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"] : GameShop : Request GiftProduct Success! [PackSeq:%d/CatSeq:%d/PriceSeq:%d/", 
        gift_item->package_product_sequence_, gift_item->product_display_sequence_, gift_item->price_sequence_);*/

    GAMELOG->LogShopItem(ITEM_CHARGE_SEND_GIFT_SUCCESS, player, gift_item->price_sequence_, 0, 0);

    // 선물 받기 완료 로그
    Player* receiver = PlayerManager::Instance()->FindPlayerByUserKey(gift_receiver_.user_guid_);

    /*SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"] : GameShop : Receive GiftProduct Success! [PackSeq:%d/CatSeq:%d/PriceSeq:%d/", 
        gift_item->package_product_sequence_, gift_item->product_display_sequence_, gift_item->price_sequence_);*/

    GAMELOG->LogShop_RecvItem(ITEM_CHARGE_ACTION_RECV_GIFT_SUCCESS, receiver, gift_item->package_product_sequence_, 0);
#endif //_NA_007288_20140611_GAMELOG_GAMESHOP

    // 아이템 릴레이에서 현재 선택된 아이템을 제거.
    item_relay_.Pop();

    // 아이템 릴레이에 구입할 아이템이 존재하는 동안
    // 계속해서 구입 요청을 한다.
    if (item_relay_.IsEmpty())
    {
        MSG_CG_SHOP_SEND_GIFT_ACK ack_message;
        item_relay_.Destroy(ack_message.item_info_, &(ack_message.item_count_));
        strcpy(ack_message.recv_character_name_, gift_receiver_.character_name_);
        player->SendPacket(&ack_message, ack_message.GetSize());

        // DBProxy를 통해 선물 받는 유저가 존재하는 게임서버로 통보.
        MSG_DG_OTHER_SHOP_RECV_GIFT_NTF ntf_message;
        ntf_message.m_UserGuid = gift_receiver_.user_guid_;
        ntf_message.m_CharGuid = gift_receiver_.character_guid_;
        g_pGameServer->SendToServer(GAME_DBPROXY, &ntf_message, sizeof(ntf_message));
    }
    else
    {
        // 샵서버가 열려 있는가?
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
        int cash_type = 508; // 캐시.
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
            //선물 보내려는 아이템(item_relay_)이 아직 남아있으므로, 구입요청을 다시 진행시킨다
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
            case 10: // 기간
                // 기간제 아이템의 초단위 시간 값(빌링에서만 사용)
                //delete_date_ = PropertyList[idx].Value; // 초단위
                break;
            case 7: //갯수
                // 소모성 아이템의 갯수(빌링에서만 사용)
                break;

            case 109: // 기간제 아이템(사용 기간 - 1일 단위)
                delete_date_ = PropertyList[idx].Value * 86400; //(60*60*24)
                break;
            case 113: // 기간제 아이템(아이템 타입)
                item_type_ = PropertyList[idx].Value;
                break;
            case 116: // 기간제 아이템(인챈트 갯수)
                enchant_count_ = PropertyList[idx].Value;
                break;
            case 119: // 기간제 아이템(소켓 갯수)
                socket_count_ = PropertyList[idx].Value;
                break;

            case 110: // 소모성 아이템(아이템 갯수)
                item_count_ = PropertyList[idx].Value;
                break;
            case 111: // 소모성 아이템(아이템 타입)
                item_type_ = PropertyList[idx].Value;
                break;
            case 114: // 소모성 아이템(인챈트 갯수)
                enchant_count_ = PropertyList[idx].Value;
                break;
            case 117: // 소모성 아이템(소켓 갯수)
                socket_count_ = PropertyList[idx].Value;
                break;

            case 112: // 영구 아이템 (아이템 타입)
                item_type_ = PropertyList[idx].Value;
                break;
            case 115: // 영구 아이템 (인챈트 갯수)
                enchant_count_ = PropertyList[idx].Value;
                break;
            case 118: // 영구 아이템 (소켓 갯수)
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

    // 샵서버가 열려 있는가?
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

    // 인벤토리에 빈 공간이 있는가?
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
        // [아이템 획득 성공 로그]
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

        // [아이템 획득 실패 로그]
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

    // 샵서버가 열려 있는가?
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

    // 샵서버가 열려 있는가?
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
    // A : 전체목록
    // S : 보관함 목록(본인 구매)
    // G : 선물함 목록(받은 선물)

    // NowPage
    // 조회할 페이지 번호(1번 부터 ~ N번)
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