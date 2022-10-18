
#include "stdafx.h"

#ifndef _NA_0_20110613_GAME_SHOP_RENEWAL



#include ".\ShopItemPrice.h"
#include <ArchiveOfStream.h>

__IMPL_CUSTOMPOOL_PTR(ShopItemPrice);

// 타입에 따라 다른 정보 클라이언트로 보내야함!!!!!
void ShopItemPrice::Serialize(ArchiveOfStream& ar, eSHOP_SERIALIZE se)
{
    // TODO : serialize data by a block unit
    switch (se)
    {
    case eSHOP_SERIALIZE_ITEM_SAVE:
        {
            //ar.Read(&node_.price_guid_, sizeof(node_.price_guid_));
            ar.Read(&node_.use_time_, sizeof(node_.use_time_));
            ar.Read(&node_.amount_, sizeof(node_.amount_));
            ar.Read(&node_.price_, sizeof(node_.price_));
            ar.Read(&node_.sale_ratio_, sizeof(node_.sale_ratio_));
        }
        break;
    case eSHOP_SERIALIZE_ITEM_LOAD:
        {
            ar.Write(&node_.price_guid_, sizeof(node_.price_guid_));
            ar.Write(&node_.use_time_, sizeof(node_.use_time_));
            ar.Write(&node_.amount_, sizeof(node_.amount_));
            ar.Write(&node_.price_, sizeof(node_.price_));
            ar.Write(&node_.sale_ratio_, sizeof(node_.sale_ratio_));
        }
        break; 
    default:
        ASSERT(NULL);
        break;
    }
}

//==================================================================================================


void ShopItemPrice::Display()
{
#ifdef _SERVER
    printf(" PriceGuid:%d, UseTime:%d, Amount:%d, Price:%d, Sale:%d\n",
           node_.price_guid_, node_.use_time_, node_.amount_, node_.price_, node_.sale_ratio_);
#endif
}

// for server side
void ShopItemPrice::AlignFromShopSellItemPrice(const tblSellItemPrice& item_price_info,
                                               ShopItem* shop_item)
{
    ASSERT(SAFE_NUMERIC_TYPECAST(DWORD, item_price_info.dwSellRate, BYTE));
    ASSERT(SAFE_NUMERIC_TYPECAST(DWORD, item_price_info.dwAmount, BYTE));
    Clear();
    node_.price_guid_ = item_price_info.dwPriceGuid;
    node_.use_time_ = item_price_info.dwUseTime;
    node_.amount_ = static_cast<uint8_t>(item_price_info.dwAmount);
    node_.price_ = item_price_info.dwPrice;
    node_.sale_ratio_ = static_cast<uint8_t>(item_price_info.dwSellRate);
    node_.shop_item_ = shop_item;
}

void ShopItemPrice::AlignFromShopSellPackage(const tblSellPackage& sell_package, ShopItem* shop_item)
{
    Clear();
    node_.price_guid_ = sell_package.itemPrice.dwPriceGuid;
    node_.price_ = sell_package.itemPrice.dwPrice;
    node_.sale_ratio_ = static_cast<uint8_t>(sell_package.itemPrice.dwSellRate);
    node_.shop_item_ = shop_item;
}

void ShopItemPrice::AlignFromEachNode(PRICEGUID price_guid, DWORD price, uint8_t sale_ratio,
                                      ShopItem* shop_item)
{
    Clear();
    node_.price_guid_ = price_guid;
    node_.price_ = price;
    node_.sale_ratio_ = sale_ratio;
    node_.shop_item_ = shop_item;
}

//==================================================================================================

#endif // not defined _NA_0_20110613_GAME_SHOP_RENEWAL
