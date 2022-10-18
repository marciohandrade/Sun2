
#ifndef _NA_0_20110613_GAME_SHOP_RENEWAL



#pragma once
#ifndef SOLAR_SHOP_ITEM_PRICE_H
#define SOLAR_SHOP_ITEM_PRICE_H

//==================================================================================================
/// ShopItemPrice class
/**
    @author Kim Min Wook < taiyo@webzen.com >
    @since  2006. 12. 3(日)
    @remark
        - 하나의 샵 아이템에대한 개별적인 가격 정보 클래스
    @note
        - ShopItemPrice은 공유되지 않는 클래스이다.
    @history 
    - 
*/
//==================================================================================================

#include "ShopDefine.h"

class ArchiveOfStream;
class ShopItem;

typedef struct sellItemPrice tblSellItemPrice;

//==================================================================================================

class ShopItemPrice
{
    __DECL_CUSTOMPOOL_PTR(ShopItemPrice)
public:
    ShopItemPrice();
    ~ShopItemPrice();

    void Clear();
    void Serialize(ArchiveOfStream& ar, eSHOP_SERIALIZE se);

    // Properties
    PRICEGUID GetPriceGuid() const;
    DWORD GetUseTime() const;
    BYTE GetAmount() const;
    DWORD GetPrice() const;
    uint8_t GetSaleRatio() const;
    ShopItem* GetShopItem() const;
    // NOTE, terrible operations... data alignment operations of the shop item streaming process
    // based on case by case. remain setters until the matter is resolved.
    //void SetPriceGuid(PRICEGUID price_guid) { node_.price_guid_ = price_guid; }
    //void SetPrice(DWORD price) { node_.price_ = price; }
    //void SetSailRatio(uint8_t sale_ratio) { node_.sale_ratio_ = sale_ratio; }
    //void SetShopItem(ShopItem* shop_item) { node_.shop_item_ = shop_item; };
    //
    void Display();
    //
    // for server side
    void AlignFromShopSellItemPrice(const tblSellItemPrice& item_price_info, ShopItem* shop_item);
    // f100905.3L
    void AlignFromShopSellPackage(const tblSellPackage& sell_package, ShopItem* shop_item);
    void AlignFromEachNode(PRICEGUID price_guid, DWORD price, uint8_t sale_ratio, ShopItem* shop_item);
private:
    struct Node {
        PRICEGUID price_guid_;
        DWORD use_time_;    //< (정액제, 정량제:시간
        DWORD price_;
        uint8_t amount_;       //< 소모성,무제한:수량)
        uint8_t sale_ratio_;   //< 세일 비율값 (0 ~ 100)
        ShopItem* shop_item_;
    } node_;
};

//==================================================================================================

inline ShopItemPrice::ShopItemPrice() {
    ZeroMemory(&node_, sizeof(node_));
}

inline ShopItemPrice::~ShopItemPrice() {
}

inline void ShopItemPrice::Clear() {
    ZeroMemory(&node_, sizeof(node_));
}

inline PRICEGUID ShopItemPrice::GetPriceGuid() const {
    return node_.price_guid_;
}

inline DWORD ShopItemPrice::GetUseTime() const {
    return node_.use_time_;
}

inline BYTE ShopItemPrice::GetAmount() const {
    return node_.amount_;
}

inline DWORD ShopItemPrice::GetPrice() const {
    return node_.price_;
}

inline BYTE ShopItemPrice::GetSaleRatio() const {
    return node_.sale_ratio_;
}

inline ShopItem* ShopItemPrice::GetShopItem() const {
    return node_.shop_item_;
}

#endif //SOLAR_SHOP_ITEM_PRICE_H


#endif // not defined _NA_0_20110613_GAME_SHOP_RENEWAL
