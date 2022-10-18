#ifndef GAMESERVER_ITEMMANAGER_UTIL_H
#define GAMESERVER_ITEMMANAGER_UTIL_H
#pragma once

//==================================================================================================
// @history
//  - |~10/03/24|
//  - |10/03/24|waverix|fit in coding style
//  - |10/03/25|waverix|move to code file related ItemStatSwitch implementation parts

#include "ItemInfoParser.h"

namespace nsSlot {
;
class ItemFunctionItemLimitation;
}; //end of namespace
//==================================================================================================

class ItemStatSwitch
{
    typedef std::vector<SLOTCODE> ItemCodeList;
    struct ItemCodeListInfo
    {
        CODETYPE n_code_;
        CODETYPE wi_code_;
        CODETYPE vi_code_;
        ItemCodeList item_code_list_;

        ItemCodeListInfo(CODETYPE n_code, CODETYPE wi_code, CODETYPE vi_code)
            : n_code_(n_code), wi_code_(wi_code), vi_code_(vi_code)
        {
        }
    };
    typedef std::vector<ItemCodeListInfo> CodeListCache;
    //typedef CodeListCache::iterator  CodeListCacheIter;

public:
    ItemStatSwitch(SCItemSlot* target_item_slot, SCItemSlot* use_item_slot,
                   SCItemSlotContainer* slot_container);
    ~ItemStatSwitch();
    bool CreateSameItem(SLOTCODE item_code);
    bool GetItemCodeList(MSG_CG_ITEM_STAT_CHANGE_USE_ACK* recv_msg);
    static void* operator new(std::size_t size);
    static void operator delete(void* pos, std::size_t size);
private:
    void Search_ItemCode_();
    ItemCodeList* FindItemCodeList(CODETYPE n_code, CODETYPE wi_code, CODETYPE vi_code);
private:
    union
    {
        SCItemSlot* use_item_slot_;
        ItemStatSwitch* item_stat_switch_next_link_;
    };

    SCItemSlot* target_item_slot_;
    SCItemSlotContainer* slot_container_;
    SCItemSlot making_item_slot_;

    static CodeListCache matching_table_static_;
    static ItemStatSwitch* pool_free_root_;
    static const std::size_t pool_size_static_;
    //
    //friend class ItemManager;
    friend class nsSlot::ItemFunctionItemLimitation;
};

//==================================================================================================

#endif //GAMESERVER_ITEMMANAGER_UTIL_H