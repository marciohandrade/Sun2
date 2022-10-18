#include "stdafx.h"

#ifndef _NA_0_20110613_GAME_SHOP_RENEWAL



#include ".\SingleItem.h"
#include <ArchiveOfStream.h>
#include <algorithm>
#include <oleauto.h>
#include <SolarDateTime.h>
#include ".\ShopManager.h"
#include "../SolarSlot/SCSlotHeader.h"

__IMPL_CUSTOMPOOL_PTR(SingleItem);

SingleItem::SingleItem()
{
    ZeroMemory(&single_item_data_, sizeof(single_item_data_));
}

SingleItem::~SingleItem()
{
}

void SingleItem::Release()
{
    ShopItem::Release();
    ZeroMemory(&single_item_data_, sizeof(single_item_data_));
}

void SingleItem::vSerialize(ShopManager& mgr IN, ArchiveOfStream& ar, eSHOP_SERIALIZE se)
{
    __UNUSED(mgr);
    switch(se)
    {
    case eSHOP_SERIALIZE_ITEM_SAVE:
    case eSHOP_SERIALIZE_ITEM_LIST_SAVE:
        {
            ar >> single_item_data_.item_code_;
            ar >> single_item_data_.enchant_;
            ar >> single_item_data_.item_type_;
        }
        break;
    case eSHOP_SERIALIZE_ITEM_LOAD:
    case eSHOP_SERIALIZE_ITEM_LIST_LOAD:
        {
            ar << single_item_data_.item_code_;
            ar << single_item_data_.enchant_;
            ar << single_item_data_.item_type_;
        }
        break;
    default:
        ASSERT(NULL);
        break;
    }
}

void SingleItem::Display()
{
#ifdef _SERVER
    ShopItem::Display();
    printf(",Code:%d,Type:%d,Enchant:%d,ItemType:%d\n",
           single_item_data_.item_code_, single_item_data_.charge_sub_type_,
           single_item_data_.enchant_, single_item_data_.item_type_);

    ShopItem::DisplayElement();
#endif //
}


// for server side
#ifdef _NA_0_20101015_SOLAR_SHOP_TEMP_REFACTORING
void SingleItem::Set(const tblSellItemContainer& container, ShopManager& mgr)
{
    Set(*(container.singleitem_info), mgr);

    ShopItemData* const shop_item_data = GetEditableShopItemData();
    shop_item_data->partition_index_ = static_cast<uint8_t>(container.partition_index);
    shop_item_data->item_filter_ = container.shopitem_filter;
    shop_item_data->category_ = static_cast<uint8_t>(container.category_index);
    shop_item_data->cache_index_ = static_cast<uint16_t>(container.cache_index);

    if (container.is_skin_item)
        SetRank(KindOfItemType::Shell);
}
#endif

//void SingleItem::Set(BYTE* pInfo IN, ShopManager& mgr IN)
void SingleItem::Set(const tblSellItem& rItem, ShopManager& mgr)
{
    ASSERT(SAFE_NUMERIC_TYPECAST(DWORD,rItem.dwItemCODE,WORD));
    ASSERT(SAFE_NUMERIC_TYPECAST(DWORD,rItem.dwCategoryID,BYTE));
    ASSERT(SAFE_NUMERIC_TYPECAST(DWORD,rItem.dwBuyType,BYTE));

    ShopItemData* const shop_item_data = GetEditableShopItemData();
    shop_item_data->item_guid_ = rItem.dwItemGuid;
    shop_item_data->category_ = static_cast<uint8_t>(rItem.dwCategoryID);
    single_item_data_.item_code_ = static_cast<SLOTCODE>(rItem.dwItemCODE);
    single_item_data_.charge_sub_type_ = rItem.dwUseType;
    shop_item_data->money_type_ = static_cast<uint8_t>(rItem.dwBuyType);
    shop_item_data->cool_time_ = rItem.dwCoolTime;

    ;{
        SYSTEMTIME systime;
        ::VariantTimeToSystemTime(rItem.dbVarEndDate, &systime);
        util::GetDateTime_YYYYMMDDHHMMSS(systime.wYear, systime.wMonth, systime.wDay,
                                         systime.wHour, systime.wMinute, systime.wSecond,
                                         shop_item_data->sell_expiration_date_);
        //SetSellEndDate(rItem.dbVarEndDate);
    };
    shop_item_data->limit_sell_count_ = rItem.dwLimitSellCount;
    shop_item_data->state_ = rItem.dwState;
    BOOST_STATIC_ASSERT(sizeof(package_name_) == sizeof(rItem.szItemName) + 1 &&
                        sizeof(package_desc_) == sizeof(rItem.szItemDesc) + 1);
    BOOST_STATIC_ASSERT(_countof(package_name_) == _countof(rItem.szItemName) + 1 &&
                        _countof(package_desc_) == _countof(rItem.szItemDesc) + 1);
    strncpy(package_name_, rItem.szItemName, MAX_PACKAGE_NAME_LENGTH);
    package_name_[MAX_PACKAGE_NAME_LENGTH] ='\0';
    strncpy(package_desc_, rItem.szItemDesc, MAX_PACKAGE_DESC_LENGTH);
    package_desc_[MAX_PACKAGE_DESC_LENGTH] ='\0';

    DWORD number_of_prices = rItem.dwPriceCount;
    if (FlowControl::FCAssert(number_of_prices <= MAX_PRICE_INFO_NUM) == false) {
        number_of_prices = MAX_PRICE_INFO_NUM;
    }
    // f100905.3L
    for (DWORD i = 0; i < number_of_prices; ++i)
    {
        DWORD item_guid = GetGuid();
        const tblSellItemPrice& sell_item_price = rItem.itemPrice[i];
        if (FlowControl::FCAssert(item_guid == sell_item_price.dwItemGuid) == false) {
            continue;
        }
        const PRICEGUID price_guid = sell_item_price.dwPriceGuid;
        ShopItemPrice* shop_item_price = mgr.FindShopItemPrice(price_guid);
        const bool new_insert = (shop_item_price == NULL);
        if (new_insert)
        {
            shop_item_price = mgr.CreateShopItemPrice();
            shop_item_price->Clear();
            AddPriceInfo(price_guid, shop_item_price);
        }
        shop_item_price->AlignFromShopSellItemPrice(sell_item_price, this);
    }
}

/*
#define DATETIME_FORMAT "%04d/%02d/%02d %02d:%02d"
double dbDateTime
SYSTEMTIME systime;
::VariantTimeToSystemTime(dbDateTime, &systime);
char szDateTime[32];
sprintf(szDateTime, DATETIME_FORMAT, systime.wYear, systime.wMonth, systime.wDay, systime.wDay, systime.wMinute);
*/


#endif // not defined _NA_0_20110613_GAME_SHOP_RENEWAL
