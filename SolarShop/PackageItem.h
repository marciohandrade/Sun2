
#ifndef _NA_0_20110613_GAME_SHOP_RENEWAL



#pragma once
#ifndef SOLAR_SHOP_PACKAGE_ITEM_H
#define SOLAR_SHOP_PACKAGE_ITEM_H

#include "ShopDefine.h"
#include "ShopItem.h"
#include "ShopItemPrice.h"

class ShopManager;

class PackageItem : public ShopItem
{
    __DECL_CUSTOMPOOL_PTR(PackageItem)
public:
    struct ItemPiece
    {
        WORD    ItemCode;
        BYTE    Amount;
        DWORD   UseTime;
        BYTE    Enchant;
        BYTE    Rank;
        DWORD   ItemGuid;
    };
    //
    PackageItem();
    virtual ~PackageItem();

    virtual eTYPE GetType() const { return ShopItem::PACKAGE; }
    virtual void Release();
    virtual void Display();

    // for server side
#ifdef _NA_0_20101015_SOLAR_SHOP_TEMP_REFACTORING
    virtual void Set(const tblSellItemContainer& container, ShopManager& mgr);
#endif
    void Set(const tblSellPackage& rPack, ShopManager& mgr);
    // 패키지 가격정보 {
    DWORD GetImageCode() const { return package_item_data_.image_code_; }
    BYTE GetPieceCount() const { return package_item_data_.number_of_item_pieces_; }
    // }
    ShopItemPrice* GetShopItemPrice() const { return package_item_data_.shop_item_price_; };
    // NOTE, f100905.3L, the getter of data block will replace getters by an index.
    const ItemPiece* GetItemPiece(uint8_t piece_index) const;
    // {
    WORD GetItemCode(BYTE piece_idx) const { return item_pieces_[piece_idx].ItemCode; }
    BYTE GetAmount(BYTE piece_idx) const { return item_pieces_[piece_idx].Amount; }
    DWORD GetUseTime(BYTE piece_idx) const { return item_pieces_[piece_idx].UseTime; }

    BYTE GetEnchant(BYTE piece_idx) const { return item_pieces_[piece_idx].Enchant; }
    BYTE GetRank(BYTE piece_idx) const { return item_pieces_[piece_idx].Rank; }
    //DWORD GetItemGuid(BYTE piece_idx) const { return item_pieces_[piece_idx].ItemGuid; }
    //
    void SetEnchant(BYTE piece_idx, BYTE Enchant) { item_pieces_[piece_idx].Enchant = Enchant; }
    void SetRank(BYTE piece_idx, BYTE Rank) { item_pieces_[piece_idx].Rank = Rank; }
    // }
protected:
    virtual void vSerialize(ShopManager& mgr IN, ArchiveOfStream& ar, eSHOP_SERIALIZE se);

private:
    struct PackageItemData {
        DWORD image_code_;
        uint8_t number_of_item_pieces_;
        ShopItemPrice* shop_item_price_; //< 항목 사용안함 UseTime, Amount
    } package_item_data_;
    //
    ItemPiece item_pieces_[MAX_PACKAGE_ITEM_SIZE];
};

//==================================================================================================

inline const PackageItem::ItemPiece* PackageItem::GetItemPiece(uint8_t piece_index) const
{
    if (FlowControl::FCAssert(piece_index < package_item_data_.number_of_item_pieces_)) {
        return &item_pieces_[piece_index];
    }
    return NULL;
}

#endif //SOLAR_SHOP_PACKAGE_ITEM_H



#endif // not defined _NA_0_20110613_GAME_SHOP_RENEWAL
