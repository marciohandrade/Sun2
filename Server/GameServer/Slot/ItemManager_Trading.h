#ifndef GAMESERVER_ITEMMANAGER_TRADING_H
#define GAMESERVER_ITEMMANAGER_TRADING_H

class SCItemSlotContainer;
class ItemManager;

//==================================================================================================
namespace nsSlot {
;
//==================================================================================================
// NOTE: f110611.1L, Sub-Manager of the ItemManager
// about trading in npc shop and between player trading
class ItemTradingImpl
{
public:
    ItemTradingImpl();
    // called by ItemManager::Init
    ItemTradingImpl(ItemManager* item_manager);
    //
    //  마을에서만 사용할 수 있는 것들! NPC를 통해 가능
    RC::eITEM_RESULT Sell(
        const SLOTIDX slot_type, const POSTYPE slot_position, const DURATYPE sell_quantity);
    RC::eITEM_RESULT Buy(
        const DWORD shop_list_id, const BYTE shop_tab_index, const BYTE shop_item_index, 
        MSG_CG_ITEM_BUY_ACK* OUT const buy_msg,
        const DURATYPE buy_quantity);
    RC::eITEM_RESULT Exchange(POSTYPE item_pos, INVENTORY_TOTAL_INFO* OUT total_info);
#ifdef __NA_001027_20080423_RANDOM_REWARD_EVENT_
    RC::eITEM_RESULT ExchangeRewardItem(SLOTCODE ItemCode, DURATYPE num,
                                        INVENTORY_TOTAL_INFO* OUT pTotalInfo);
#endif
    void RegisterSaleItem(SCItemSlot&);
    RC::eITEM_RESULT Repurchase(POSTYPE, INVENTORY_TOTAL_INFO*);
#ifdef _NA_20100106_REPURCHAS_BUGFIX_2496
    VOID SerializeRepurchaseItems(REPURCHASITEM_TOTAL_INFO & RepurchasItems, eSERIALIZE eType);
#endif
    VOID ClearRepurchasItemList();

private:
#ifdef _NA_20110303_ADD_TYPE_OF_NPCSHOP
    //상점 타입별 지급 처리 핸들링
    RC::eITEM_RESULT _PayProcessing(const BYTE shop_tab_index,
                                    const DURATYPE number_of_buy_wanted_item,
                                    const SCItemSlot buy_wanted_item_slot,
                                    MSG_CG_ITEM_BUY_ACK* OUT const buy_msg,
                                    SHOPINFO* shop_info,
                                    SHOPINFO::SHOP_ITEM* const shop_item);

    //일반 상점
    RC::eITEM_RESULT _PayNormalShop(SHOPINFO* shop_info,
                                    const BYTE shop_tab_index,
                                    const SCItemSlot buy_wanted_item_slot, 
                                    const DURATYPE number_of_buy_wanted_item,
                                    MSG_CG_ITEM_BUY_ACK* OUT const buy_msg);
    //아이템 상점
    RC::eITEM_RESULT _PayItemShop(SHOPINFO* shop_info,
                                  SHOPINFO::SHOP_ITEM* const shop_item, 
                                  const SCItemSlot buy_wanted_item_slot,
                                  const DURATYPE number_of_buy_wanted_item,
                                  MSG_CG_ITEM_BUY_ACK* OUT const buy_msg);
    //적립 포인트 상점
    RC::eITEM_RESULT _PayAccumulatePointShop(SHOPINFO* shop_info,
                                             SHOPINFO::SHOP_ITEM* const shop_item, 
                                             const SCItemSlot buy_wanted_item_slot,
                                             const DURATYPE number_of_buy_wanted_item,
                                             MSG_CG_ITEM_BUY_ACK* OUT const buy_msg);
#else//_NA_20110303_ADD_TYPE_OF_NPCSHOP
    RC::eITEM_RESULT _PayProcessing(const BYTE shop_tab_index,
                                    const DURATYPE number_of_buy_wanted_item,
                                    const SCItemSlot buy_wanted_item_slot,
                                    MSG_CG_ITEM_BUY_ACK* OUT const buy_msg,
                                    SHOPINFO* shop_info);
#endif//_NA_20110303_ADD_TYPE_OF_NPCSHOP

#ifdef _NA004034_20120102_POINT_WALLET_SYSTEM
    // 포인트 상점
    RC::eITEM_RESULT _PayPointWalletShop( SHOPINFO* shop_info,
                                          SHOPINFO::SHOP_ITEM* const shop_item,
                                          const SCItemSlot buy_wanted_item_slot, 
                                          const DURATYPE number_of_buy_wanted_item,
                                          MSG_CG_ITEM_BUY_ACK* OUT const buy_msg);
#endif //_NA004034_20120102_POINT_WALLET_SYSTEM

    //-----------------------------------------------
    // fields...
    ItemManager* const root_;
    //
    friend class ItemManager;
    //
    __DISABLE_COPY(ItemTradingImpl);
};

//==================================================================================================
inline ItemTradingImpl::ItemTradingImpl()
    : root_(NULL)
{
}

inline ItemTradingImpl::ItemTradingImpl(ItemManager* item_manager)
    : root_(item_manager)
{
}

//==================================================================================================
}; //end of namespace
//==================================================================================================

#endif //GAMESERVER_ITEMMANAGER_TRADING_H