#pragma once

#if !defined _NA_0_20110613_GAME_SHOP_RENEWAL



#ifndef GAMESERVER_GAMESHOPUSER_H
#define GAMESERVER_GAMESHOPUSER_H

#include <ShopUser.h>
#include "./ItemManager_Types.h"

class GameShopManager;
//==================================================================================================
// @history
//  - ~100908
//  - 100908, waverix, change a data structure

class GameShopUser : public ShopUser
{
public:
    static const int MAX_LOCKEDINFO_NUM = MAX_INVENTORY_SLOT_NUM;
    // NOTE: use to extend an expiration date of an item containing the expiration date.
    struct ExtendSlotInfo {
        SLOTIDX slot_index;
        POSTYPE slot_pos;
    };
    //
    GameShopUser();
    virtual ~GameShopUser();
    //
    virtual void Release();
    //
    POSTYPE GetUsedLockedCount() const;
    void ClearTransactionInfo();
private:
    ExtendSlotInfo extend_slot_info_;
    KindOfItemTag option_tag_; // _NA000424_GLOBAL_CASH_ITEM_RENEWAL_
    POSTYPE number_of_used_locked_infos_;
    LOCKED_INFO locked_info_array_[MAX_LOCKEDINFO_NUM];
    //
    friend class GameShopManager;
};

//==================================================================================================

inline GameShopUser::GameShopUser()
{
    const size_t clear_size = static_cast<size_t>(
        reinterpret_cast<char*>(&locked_info_array_[_countof(locked_info_array_)]) -
        reinterpret_cast<char*>(&extend_slot_info_));
    ZeroMemory(&extend_slot_info_, clear_size);
}

//virtual
inline GameShopUser::~GameShopUser()
{
}

inline void GameShopUser::ClearTransactionInfo()
{
    ZeroMemory(&extend_slot_info_, sizeof(extend_slot_info_));
    option_tag_.value = option_tag_.Tag_None;
    number_of_used_locked_infos_ = 0;
}

inline POSTYPE GameShopUser::GetUsedLockedCount() const
{
    return number_of_used_locked_infos_;
}

//virtual
inline void GameShopUser::Release()
{
    ZeroMemory(&extend_slot_info_, sizeof(extend_slot_info_));
    option_tag_.value = option_tag_.Tag_None;
    number_of_used_locked_infos_ = 0;
    //ZeroMemory(locked_info_array_, sizeof(locked_info_array_));
    ShopUser::Release();
};


#endif //GAMESERVER_GAMESHOPUSER_H


#endif //#if !defined _NA_0_20110613_GAME_SHOP_RENEWAL
