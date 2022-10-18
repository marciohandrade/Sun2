#include "stdafx.h"

#ifndef _NA_0_20110613_GAME_SHOP_RENEWAL



#include ".\shopitem.h"
#include <ArchiveOfStream.h>
#include <algorithm>
#include "ShopManager.h"
#include <SolarDateTime.h>

ShopItem::ShopItem()
{
    BOOST_STATIC_ASSERT(MAX_PACKAGE_NAME_LENGTH == MAX_ITEM_NAME_LENGTH &&
                        MAX_PACKAGE_DESC_LENGTH == MAX_ITEM_DESC_LENGTH);
    //
    ZeroMemory(&shop_item_data_, sizeof(shop_item_data_));
    ZeroMemory(package_name_, sizeof(package_name_));
    ZeroMemory(package_desc_, sizeof(package_desc_));
}

ShopItem::~ShopItem()
{
}

void ShopItem::Release()
{
    ZeroMemory(&shop_item_data_, sizeof(shop_item_data_));
    ZeroMemory(package_name_, sizeof(package_name_));
    ZeroMemory(package_desc_, sizeof(package_desc_));
    shop_item_price_list_.clear();
}


ShopItemPrice* ShopItem::FindShopItemPrice(const PRICEGUID guid)
{
    // let's use predicate~~
    FOREACH_CONTAINER(const SHOPITEMPRICE_LIST::value_type& node, shop_item_price_list_,
                      SHOPITEMPRICE_LIST)
    {
        ShopItemPrice* shop_item_price = node;
        if (shop_item_price->GetPriceGuid() == guid) {
            return shop_item_price;
        }
    }
    //std::find_if(shop_item_price_list_.begin(), shop_item_price_list_.end(), 
    //  std::equal_to<PRICEGUID>(mem_fun(&ShopItemPrice::GetPriceGuid), guid));
    /*std::find_if(shop_item_price_list_.begin(), shop_item_price_list_.end(), 
    bind1st(bind2nd(std::equal_to<PRICEGUID>(), guid), mem_fun(&ShopItemPrice::GetPriceGuid)));*/
    return NULL;
}

void ShopItem::AddPriceInfo(PRICEGUID price_guid, ShopItemPrice* new_shop_item_price)
{
    ASSERT(0 != price_guid);
#ifdef _DEBUG
    FOREACH_CONTAINER(const SHOPITEMPRICE_LIST::value_type& node, shop_item_price_list_,
                      SHOPITEMPRICE_LIST)
    {
        const ShopItemPrice* shop_item_price = node;
        const PRICEGUID guid_of_node = shop_item_price->GetPriceGuid();
        assert(shop_item_price != new_shop_item_price && guid_of_node != price_guid);
        if (shop_item_price == new_shop_item_price || guid_of_node == price_guid) {
            return;
        }
    }
#endif
    shop_item_price_list_.push_back(new_shop_item_price);
}


void ShopItem::Serialize(ShopManager& mgr, ArchiveOfStream& ar, eSHOP_SERIALIZE se)
{
    switch(se)
    {
    case eSHOP_SERIALIZE_ITEM_SAVE:
        {
            vSerialize(mgr, ar, se);

            BYTE number_of_prices = 0;
            PRICEGUID price_guid = 0;

            ar >> shop_item_data_.money_type_; //< 0:캐쉬,1:하임 //< eSHOP_MONEY_TYPE
            ar >> shop_item_data_.cool_time_;  //< 구매 쿨타임
            ar >> shop_item_data_.sell_expiration_date_;

            ar >> number_of_prices;  //< 가격 정보 개수

            ASSERT(number_of_prices <= MAX_PRICE_INFO_NUM);
            for (BYTE x = 0 ; x < number_of_prices ; ++x)
            {
                ar >> price_guid;
                // f100905.3L
                ShopItemPrice* shop_item_price = mgr.FindShopItemPrice(price_guid);
                if (shop_item_price == NULL)
                {
                    shop_item_price = mgr.CreateShopItemPrice();
                    shop_item_price->AlignFromEachNode(price_guid, 0, 0, NULL);
                    AddPriceInfo(price_guid, shop_item_price);
                    mgr.AddShopItemPrice(shop_item_price);
                }
                shop_item_price->Serialize(ar, se);
            }

        }
        break;
    case eSHOP_SERIALIZE_ITEM_LIST_SAVE:
        {
            //ar >> m_Guid;
            vSerialize(mgr, ar, se);
        }
        break;
    case eSHOP_SERIALIZE_ITEM_LIST_LOAD:
        {
            ar << shop_item_data_.item_guid_;
            vSerialize(mgr, ar, se);
        }
        break;
    case eSHOP_SERIALIZE_CLIENT_LOAD:
    case eSHOP_SERIALIZE_ITEM_LOAD:
        {
            vSerialize(mgr, ar, se);

            ar << shop_item_data_.money_type_;  //< 0:캐쉬,1:하임 //< eSHOP_MONEY_TYPE
            ar << shop_item_data_.cool_time_;   //< 구매 쿨타임
            ar << shop_item_data_.sell_expiration_date_;
            ar << (BYTE)(shop_item_price_list_.size()); //< 가격 개수

            //ASSERT(SAFE_NUMERIC_TYPECAST(int,m_Category,BYTE));
            //ASSERT(SAFE_NUMERIC_TYPECAST(int,(int)shop_item_price_list_.size(),BYTE));

            SHOPITEMPRICE_LIST::iterator it = shop_item_price_list_.begin(),
                                         end = shop_item_price_list_.end();
            for (; it != end ; ++it)
            {
                ShopItemPrice* shop_item_price = *it;
                shop_item_price->Serialize(ar, se);
            }
        }
        break;
    default:
        ASSERT(NULL);
        break;
    }
}

void ShopItem::Display()
{
#ifdef _SERVER
    printf("Guid:%d,Money:%d,Cool:%d,Date:%I64d,Limit:%d,Stat:%d",
           shop_item_data_.item_guid_,
           shop_item_data_.money_type_,
           shop_item_data_.cool_time_,
           shop_item_data_.sell_expiration_date_,
           shop_item_data_.limit_sell_count_,
           shop_item_data_.state_);
#endif
}

void ShopItem::DisplayElement()
{
    FOREACH_CONTAINER(const SHOPITEMPRICE_LIST::value_type& node, shop_item_price_list_,
                      SHOPITEMPRICE_LIST)
    {
        ShopItemPrice* shop_item_price = node;
        shop_item_price->Display();
    }
}

BOOL ShopItem::IsSellEndDate()
{
    if (0ULL == shop_item_data_.sell_expiration_date_) {
        return false;
    }

    DWORD64 current_time = 0;
    util::GetDateTime_YYYYMMDDHHMMSS(current_time);
    if (current_time > shop_item_data_.sell_expiration_date_)
    {
        //판매마감시간이 다되었으면 종료
        return true;
    }

    return false;
}

#endif // not defined _NA_0_20110613_GAME_SHOP_RENEWAL
