#ifdef _NA_0_20110613_GAME_SHOP_RENEWAL


#ifndef GAMESHOP_EVENTPROCESSOR_H_
#define GAMESHOP_EVENTPROCESSOR_H_


#if _MSC_VER > 1000
#pragma once
#endif

//#include "GameShopHeader.h"
#include "GameShop.h"
//#include <public/protocol_shop.h>

namespace GameShop
{

class System;

class EventProcessor
{
public:
    EventProcessor(USERGUID user_guid, GameShop::System* shop_system);
    ~EventProcessor(void);


    static void SendFail_Open(Player* player, RC::eSHOP_RESULT result);
    static void SendFail_CashInfo(Player* player, RC::eSHOP_RESULT result);
    static void SendFail_BuyStorageList(Player* player, RC::eSHOP_RESULT result);
    static void SendFail_GiftStorageList(Player* player, RC::eSHOP_RESULT result);
    static void SendFail_BuyItem(Player* player, RC::eSHOP_RESULT result);
    static void SendFail_SendGift(Player* player, RC::eSHOP_RESULT result);
    static void SendFail_UseStorageItem(Player* player, RC::eSHOP_RESULT result);
    static void SendFail_CharacterNameCheck(Player* player, RC::eSHOP_RESULT result);


    void Link(bool link);
    bool ClearGarbage(uint delta_tick);

    void OpenShop();
    void CloseShop(RC::eSHOP_RESULT error_code);

    void Request_CashInfo();
    void Receive_CashInfo(int result_code, const CashInfo& cash_info);

    void Request_BuyStorageList(int page, int item_count_per_page);
    void Receive_BuyStorageList(int result_code, 
                                char storage_type, 
                                int page, 
                                int total_page, 
                                int total_count, 
                                int list_count,
                                ShopProtocol::STStorageNoGiftMessageEx* storage_list);


    void Request_GiftStorageList(int page, int item_count_per_page);
    void Receive_GiftStorageList(int result_code, 
                                 char storage_type, 
                                 int page, 
                                 int total_page, 
                                 int total_count, 
                                 int list_count,
                                 ShopProtocol::STStorageEx* storage_list);

    void Request_BuyItem(ShopItemInfo* shop_item, uint item_count);
    void Receive_BuyItem(int result_code, long product_left_count);

    void Request_SendGift(ShopItemInfo* shop_item, uint item_count, char* receiver_name, char* gift_message);
    void Receive_SendGift(int result_code,
                          USERGUID receiver_guid,
                          double limited_cash,
                          long product_left_count);


    void Request_UseStorageItem(UseStorageItem& IN use_storage_item);
    void Receive_UseStorageItem(int result_code, 
                                uint game_item_code,
                                int product_sequence, 
                                int storage_sequence, 
                                int storage_itemseguence, 
                                uchar property_count, 
                                ShopProtocol::STItemProperty* PropertyList,
                                int cash_type);

    void Request_CharacterNameCheck(const char* character_name);
    void Receive_CharacterNameCheck(USERGUID receive_user_guid,
                                    USERGUID receive_character_guid,
                                    int character_class,
                                    int character_level,
                                    const char* character_name, 
                                    const char* guild_name);
                                    

    void Request_ExistGiftList();
    void Receive_ExistGiftList(bool exist);

    void Item_Serial_Update(const SCItemSlot& item_slot);
    void Receive_Item_RollBack(long result_code);

private:
    RC::eSHOP_RESULT TransBuyItemResult(int result_code);
    RC::eSHOP_RESULT TransSendGiftResult(int result_code);

private:
    USERGUID                user_guid_;
    bool                    is_link_;
    GameShop::System*       shop_system_;
    GameShop::Transaction   transaction_;
    GameShop::ItemRelay     item_relay_;
    GameShop::GiftReceiver  gift_receiver_;
    UseStorageItem          use_storage_item_;
    ItemSerial              item_serial_;
};

//------------------------------------------------------------------------
inline void EventProcessor::Link(bool link)
{
    is_link_ = link;
}

}; //namespace GameShop

#endif //GAMESHOP_EVENTPROCESSOR_H_

#endif //_NA_0_20110613_GAME_SHOP_RENEWAL
