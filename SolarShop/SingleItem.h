
#ifndef _NA_0_20110613_GAME_SHOP_RENEWAL


#pragma once
#ifndef SOLAR_SHOP_SINGLE_ITEM_H
#define SOLAR_SHOP_SINGLE_ITEM_H

#include "ShopItem.h"

//==================================================================================================

class SingleItem : public ShopItem
{
    __DECL_CUSTOMPOOL_PTR(SingleItem)
public:
    SingleItem();
    virtual ~SingleItem();

    virtual eTYPE GetType() const { return ShopItem::SINGLE; }
    virtual void Release();
    virtual void Display();

    // for server side
#ifdef _NA_0_20101015_SOLAR_SHOP_TEMP_REFACTORING
    virtual void Set(const tblSellItemContainer& container, ShopManager& mgr);
#endif
    void Set(const tblSellItem& sell_item_by_shop, ShopManager& mgr);
    // Properties
    SLOTCODE GetItemCode() const { return single_item_data_.item_code_; }
    DWORD GetChargeSubType() const { return single_item_data_.charge_sub_type_; }
    uint8_t GetEnchant() const { return single_item_data_.enchant_; }
    // compatible interface
    uint8_t GetRank() const { return single_item_data_.item_type_; }
    //
    void SetEnchant(uint8_t enchant) { single_item_data_.enchant_ = enchant; }
    void SetRank(uint8_t item_type) { single_item_data_.item_type_ = item_type; }
protected:
    virtual void vSerialize(ShopManager& mgr IN, ArchiveOfStream& ar, eSHOP_SERIALIZE se);

private:
    struct SingleItemData {
        SLOTCODE item_code_;
        DWORD charge_sub_type_; //< 蜡丰拳 何啊 加己(1: 沥咀力 2: 沥樊力 3: 家葛己 4:公力茄)
        uint8_t enchant_;
        uint8_t item_type_; // the 'item_type_' replace 'Rank' since 1004
    } single_item_data_;
    //__REF_PROPERTY(WORD, ItemCode);
    //__REF_PROPERTY(DWORD, ChargeSubType); //< 蜡丰拳 何啊 加己(1: 沥咀力 2: 沥樊力 3: 家葛己 4:公力茄)
    //__REF_PROPERTY(BYTE, Enchant);
    //__REF_PROPERTY(BYTE, Rank);
};

#endif //SOLAR_SHOP_SINGLE_ITEM_H


#endif // not defined _NA_0_20110613_GAME_SHOP_RENEWAL
