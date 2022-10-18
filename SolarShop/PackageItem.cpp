
#include "stdafx.h"

#ifndef _NA_0_20110613_GAME_SHOP_RENEWAL



#include ".\packageitem.h"
#include ".\ShopManager.h"
#include <OleAuto.h>
#include <SolarDateTime.h>
#include <ArchiveOfStream.h>

__IMPL_CUSTOMPOOL_PTR(PackageItem);

//==================================================================================================

PackageItem::PackageItem()
{
    ZeroMemory(&package_item_data_, sizeof(package_item_data_));
}

PackageItem::~PackageItem()
{
}

void PackageItem::Release()
{
    ShopItem::Release();
    ZeroMemory(&package_item_data_, sizeof(package_item_data_));
    ZeroMemory(item_pieces_, sizeof(item_pieces_));
}

#ifdef _NA_0_20101015_SOLAR_SHOP_TEMP_REFACTORING
void PackageItem::Set(const tblSellItemContainer& container, ShopManager& mgr)
{
    Set(*(container.packageitem_info), mgr);
    
    ShopItemData* const shop_item_data = GetEditableShopItemData();
    shop_item_data->partition_index_ = static_cast<uint8_t>(container.partition_index);
    shop_item_data->item_filter_ = container.shopitem_filter;
    shop_item_data->category_ = static_cast<uint8_t>(container.category_index);
    shop_item_data->cache_index_ = static_cast<uint16_t>(container.cache_index);
}
#endif

//void PackageItem::Set(BYTE* pInfo IN, ShopManager& mgr IN)
void PackageItem::Set(const tblSellPackage& rPack, ShopManager& mgr)
{
    ASSERT(SAFE_NUMERIC_TYPECAST(DWORD,rPack.itemPrice.dwSellRate,BYTE));
    ASSERT(SAFE_NUMERIC_TYPECAST(DWORD,rPack.dwCategory,BYTE));
    ASSERT(SAFE_NUMERIC_TYPECAST(DWORD,rPack.dwBuyType,BYTE));
    assert(!GetShopItemPrice());

    ShopItemData* const shop_item_data = GetEditableShopItemData();
    shop_item_data->item_guid_ = rPack.dwPackageGuid;

    package_item_data_.image_code_ = rPack.dwImageCode;
    package_item_data_.shop_item_price_ = mgr.CreateShopItemPrice();

    // f100905.3L
    ShopItemPrice* const shop_item_price = GetShopItemPrice();
    shop_item_price->AlignFromShopSellPackage(rPack, this);
    const PRICEGUID price_guid = shop_item_price->GetPriceGuid();
    // 이건 tblSellItem 정보에 포함되어있지 않기때문에 직접 mgr에 넣어줘야 한다.
    // 제발이런구조로 만들지말자..좀!!..좀 잘해줄래??울화통이터지겠넹..정말.오xx
    // f100905.3L, ... -.- The pot calls the kettle black.
    if (ShopItemPrice* integrity_check_desire_empty_in_table = mgr.FindShopItemPrice(price_guid))
    {
        ASSERT(integrity_check_desire_empty_in_table != NULL);
        SUNLOG(eCRITICAL_LOG,
               _T("[PackageItem::Set] Error!! PriceGuid is Exist, Guid = %u, ExistItemName = %s"),
               price_guid, integrity_check_desire_empty_in_table->GetShopItem()->GetName());
    }
    mgr.AddShopItemPrice(shop_item_price);

    shop_item_data->category_ = static_cast<uint8_t>(rPack.dwCategory);
    shop_item_data->money_type_ = static_cast<uint8_t>(rPack.dwBuyType);
    //shop_item_data->cool_time_ = rPack.dwCoolTime;
    shop_item_data->limit_sell_count_ = rPack.dwLimitSellCount;
    ;{
        SYSTEMTIME systime;
        ::VariantTimeToSystemTime(rPack.dbVarEndDate, &systime);
        util::GetDateTime_YYYYMMDDHHMMSS(systime.wYear, systime.wMonth, systime.wDay,
                                         systime.wHour, systime.wMinute, systime.wSecond,
                                         shop_item_data->sell_expiration_date_);
    };
    shop_item_data->state_ = rPack.dwState;

    ASSERT(sizeof(package_name_) == sizeof(rPack.szPackageName)+1);
    ASSERT(sizeof(package_desc_) == sizeof(rPack.szPackageDesc)+1);
    strncpy(package_name_, rPack.szPackageName, MAX_PACKAGE_NAME_LENGTH);
    package_name_[MAX_PACKAGE_NAME_LENGTH] ='\0';
    strncpy(package_desc_, rPack.szPackageDesc, MAX_PACKAGE_DESC_LENGTH);
    package_desc_[MAX_PACKAGE_DESC_LENGTH] ='\0';

    DWORD number_of_items = rPack.dwInItemCount;
    if (FlowControl::FCAssert(number_of_items <= MAX_PACKAGE_ITEM_SIZE) == false) {
        number_of_items = MAX_PACKAGE_ITEM_SIZE;
    }

    package_item_data_.number_of_item_pieces_ = static_cast<uint8_t>(number_of_items);
    for (DWORD i = 0; i < number_of_items; ++i)
    {
        const tblSellItemPrice& sell_item_price = rPack.inItemPrices[i];
        ShopItemPrice* shop_item_price =  mgr.FindShopItemPrice(sell_item_price.dwPriceGuid);
        if (shop_item_price == NULL)
        {
            SUNLOG(eCRITICAL_LOG, "[ERROR] PriceInfo Not Found : PriceGuid=%u",
                   sell_item_price.dwPriceGuid);
            continue;
        };
        ShopItem* shop_item = shop_item_price->GetShopItem();
        // f100930.3L, fixed an opposite condition check problem
        if (FlowControl::FCAssert(shop_item->GetType() == ShopItem::SINGLE))
        {
            SingleItem* single_item = static_cast<SingleItem*>(shop_item);
            ItemPiece* const piece = &item_pieces_[i];
            piece->ItemCode = single_item->GetItemCode();
            piece->Amount   = shop_item_price->GetAmount();
            piece->UseTime  = shop_item_price->GetUseTime();
            piece->Enchant  = single_item->GetEnchant();
            piece->Rank     = single_item->GetRank();
            piece->ItemGuid = sell_item_price.dwItemGuid;
        }
    }
}

void PackageItem::vSerialize(ShopManager& mgr IN, ArchiveOfStream& ar, eSHOP_SERIALIZE se)
{
    switch(se)
    {
    case eSHOP_SERIALIZE_ITEM_LIST_SAVE:
        {
            //Release(); // f100905.3L, f100930.4L, remove clear routine
            ShopItemData* const shop_item_data = GetEditableShopItemData();
            // 기본 ShopItem -------------------------------------------------------------
            ar >> shop_item_data->money_type_; //< 0:캐쉬,1:하임 //< eSHOP_MONEY_TYPE
            ar >> shop_item_data->cool_time_; //< 구매 쿨타임
            ar >> shop_item_data->sell_expiration_date_;

            BYTE len = 0;
            ar >> len;
            ar.Read(package_name_, len);
            package_name_[len] = '\0';
            ar >> len;
            ar.Read(package_desc_, len);
            package_desc_[len] = '\0';
            // 기본 ShopItem -------------------------------------------------------------

            PRICEGUID PriceGuid = 0;
            DWORD Price = 0;
            BYTE SaleRatio = 0;
            ar >> PriceGuid;
            ar >> Price;
            ar >> SaleRatio;

            package_item_data_.shop_item_price_ = mgr.FindShopItemPrice(PriceGuid);
            if (package_item_data_.shop_item_price_ == NULL)
            {
                package_item_data_.shop_item_price_ = mgr.CreateShopItemPrice();
                ShopItemPrice* shop_item_price = GetShopItemPrice();
                shop_item_price->AlignFromEachNode(PriceGuid, Price, SaleRatio, this);
                mgr.AddShopItemPrice(shop_item_price);
            }

            ar >> package_item_data_.image_code_; //< 이미지 코드
            ar >> package_item_data_.number_of_item_pieces_;
            for (BYTE i = 0; i < package_item_data_.number_of_item_pieces_; ++i)
            {
                ItemPiece* piece = &item_pieces_[i];
                ar >> piece->ItemCode;
                ar >> piece->Amount;
                ar >> piece->UseTime;
                ar >> piece->Enchant;
                ar >> piece->Rank;
                ar >> piece->ItemGuid;
            }
        }
        break;
    case eSHOP_SERIALIZE_ITEM_LIST_LOAD:
        {
            // 기본 ShopItem -------------------------------------------------------------
            ShopItemData* const shop_item_data = GetEditableShopItemData();
            ar << shop_item_data->money_type_; //< 0:캐쉬,1:하임 //< eSHOP_MONEY_TYPE
            ar << shop_item_data->cool_time_; //< 구매 쿨타임
            ar << shop_item_data->sell_expiration_date_;

            BYTE len = (BYTE)strlen(package_name_);
            ar << len;
            ar.Write(package_name_, len);
            len = (BYTE)strlen(package_desc_);
            ar << len;
            ar.Write(package_desc_, len);
            // 기본 ShopItem -------------------------------------------------------------

            PRICEGUID PriceGuid = 0;
            DWORD Price = 0;
            BYTE SaleRatio = 0;
            if (ShopItemPrice* shop_item_price = GetShopItemPrice())
            {
                PriceGuid = shop_item_price->GetPriceGuid();
                Price = shop_item_price->GetPrice();
                SaleRatio = shop_item_price->GetSaleRatio();
            }
            ar << PriceGuid;
            ar << Price;
            ar << SaleRatio;

            ar << package_item_data_.image_code_;            //< 이미지 코드
            ar << package_item_data_.number_of_item_pieces_; //< 가격 개수
            for (BYTE i = 0; i < package_item_data_.number_of_item_pieces_; ++i)
            {
                const ItemPiece* piece = &item_pieces_[i];
                ar << piece->ItemCode;
                ar << piece->Amount;
                ar << piece->UseTime;
                ar << piece->Enchant;
                ar << piece->Rank;
                ar << piece->ItemGuid;
            }
        }
        break;
    default:
        {
            ASSERT(NULL);
        }
    }
}

void PackageItem::Display()
{
    ShopItem::Display();
    char buffer[4096]; // (128 + 1)* 16 < 4096
    ShopItemPrice* shop_item_price = package_item_data_.shop_item_price_;
    int written = _snprintf(buffer, _countof(buffer),
                            ",PriceGuid:%d,Price:%d,ImageCode:%d,SaleRatio:%d\n",
                            shop_item_price->GetPriceGuid(), shop_item_price->GetPrice(),
                            package_item_data_.image_code_, shop_item_price->GetSaleRatio());
    const int number_of_pieces = package_item_data_.number_of_item_pieces_;
    for (int i = 0; i < number_of_pieces; ++i)
    {
        const ItemPiece& piece = item_pieces_[i];
        written += _snprintf(&buffer[written], _countof(buffer) - written,
                             " ItemCode:%d,Amount:%d,UseTime:%d,Enchant:%d,Rank:%d\n",
                             piece.ItemCode, piece.Amount, piece.UseTime, piece.Enchant, piece.Rank);
    }
    buffer[written] = '\0';
    printf("%s", buffer);
    ShopItem::DisplayElement();
}



#endif // not defined _NA_0_20110613_GAME_SHOP_RENEWAL
