#include "stdafx.h"
#include "GameShop.ShopItemBuilder.h"

#if !defined _NA_0_20110613_GAME_SHOP_RENEWAL



#ifdef _NA_0_20101015_SOLAR_SHOP_TEMP_REFACTORING
#include <ItemInfoParser.h>
#include <CashCategoryParser.h>


namespace GameShop
{

    
IndexGenerator ShopItemBuilder::index_generator_;


//===============================================================================================
//
// class ShopItemBuilder
//
//===============================================================================================

ShopItemBuilder::ShopItemBuilder(ShopManager* shop_manager)
: shop_manager_(shop_manager)
{

}

ShopItemBuilder::~ShopItemBuilder()
{

}


void ShopItemBuilder::BuildShopItem(tblSellItem* sell_item, uint count)
{
    for (uint idx = 0 ; idx < count ; ++idx)
    {
        Build(&(sell_item[idx]));
    }
}

void ShopItemBuilder::BuildShopItem(tblSellPackage* sell_item, uint count)
{
    for (uint idx = 0 ; idx < count ; ++idx)
    {
        Build(&(sell_item[idx]));
    }
}

void ShopItemBuilder::BuildShopItem(tblItemBranch* branch_item, uint count)
{
    for (uint idx = 0 ; idx < count ; ++idx)
    {
        Build(&(branch_item[idx]));
    }
    
    shop_manager_->SetTransaction(false);
}

void ShopItemBuilder::Build(tblSellItem* sell_item)
{
    sellItemContainer container;
    ZeroMemory(&container, sizeof(container));
    // WARNING: f110711.2L, using packet pointer, don't store other container
    container.singleitem_info = sell_item;
    container.partition_index = SHOP_PARTITION_SINGLEITEM;
    container.cache_index = index_generator_.new_index();
    container.category_index = SHOP_CATEGORY_ALL;
    container.is_skin_item = false;
    container.shopitem_filter = SHOP_FILTER_NONE;


    BASE_ITEMINFO* item_info = ItemInfoParser::Instance()->GetItemInfo(sell_item->dwItemCODE);
    ASSERT(item_info);
    if (item_info == NULL)
    {
        SUNLOG(eCRITICAL_LOG, "[ShopManager::Translate] Invalid Item Code [code : %d]", sell_item->dwItemCODE);
        return;
    }

    switch (sell_item->dwCategoryID)
    {
#ifdef __NA_00387_20070605_SHOP_MILEAGE2
    case CT_MILEAGE:
        container.partition_index = SHOP_PARTITION_MILEAGEITEM;
        break;
#endif
    case CT_PACKAGE:
    case CT_TEMP:
        SUNLOG(eCRITICAL_LOG, "[ShopManager::Translate] Invalid Item Category [guid : %d]", sell_item->dwItemGuid);
        return;

    case CT_WEAPON:
        container.category_index = get_skin_category(item_info);
        container.is_skin_item = true;
        break;

    case CT_PCROOM:
        container.category_index = SHOP_CATEGORY_PCROOM;
        break;
    case CT_PPCASH:
        container.category_index = SHOP_CATEGORY_PPCASH;
        break;
    case CT_PERIOD_EXTEND:
        container.category_index = SHOP_CATEGORY_PERIOD_EXTEND;
        break;

    default:
        container.category_index = get_category(item_info);
        break;
    }

    // 스크립트에 정의된 카테고리를 찾아 넣는다.
    uint category_index = CashCategoryParser::Instance()->get_category_index(sell_item->dwItemCODE);
    if (category_index != SHOP_CATEGORY_INVALID)
        container.category_index = category_index;

    // 아이템 사용 가능한 캐릭터의 필터 설정.
    container.shopitem_filter = get_character_class_filter(item_info);

    shop_manager_->BuildShopItem(container);
}


void ShopItemBuilder::Build(tblSellPackage* sell_item)
{
    sellItemContainer container;
    ZeroMemory(&container, sizeof(container));
    container.partition_index = SHOP_PARTITION_PACKAGEITEM;
    container.category_index = SHOP_CATEGORY_ALL;
    // WARNING: f110711.2L, using packet pointer, don't store other container
    container.packageitem_info = sell_item;
    container.cache_index = index_generator_.new_index();
    container.shopitem_filter = SHOP_FILTER_NONE;

    uint count = sell_item->dwInItemCount;
    for (uint idx = 0 ; idx < count ; ++idx)
    {
        const tblSellItemPrice& sell_item_price = sell_item->inItemPrices[idx];
        ShopItemPrice* shop_item_price = shop_manager_->FindShopItemPrice(sell_item_price.dwPriceGuid);
        if (shop_item_price == NULL)
        {
            SUNLOG(eCRITICAL_LOG, "[ERROR] PriceInfo Not Found : PriceGuid=%u", sell_item_price.dwPriceGuid);
            continue;
        }

        ShopItem* shopitem = shop_item_price->GetShopItem();
        ASSERT(shopitem);
        if (shopitem && (shopitem->GetType() == ShopItem::SINGLE))
        {
            SingleItem* single_item = static_cast<SingleItem*>(shopitem);
            BASE_ITEMINFO* item_info = ItemInfoParser::Instance()->GetItemInfo(single_item->GetItemCode());
            ASSERT(item_info);
            if (item_info && (item_info->IsArmor() || item_info->IsWeapon()))
            {
                // 아이템 사용 가능한 캐릭터의 필터 설정.
                container.shopitem_filter |= get_character_class_filter(item_info);
            }
        }
    }

    if (container.shopitem_filter == SHOP_FILTER_NONE)
        container.shopitem_filter = SHOP_FILTER_CHAR_ALL;

    shop_manager_->BuildShopItem(container);
}

void ShopItemBuilder::Build(tblItemBranch* branch_item)
{
    DWORD temp_filter = 0;
    switch (branch_item->dwBranchID)
    {
    case CT_NEWARRIVAL:
        temp_filter = SHOP_FILTER_NEWARRIVAL;
        break;
    case CT_DISCOUNT:
        temp_filter = SHOP_FILTER_DISCOUNT;
        break;
    case CT_POPULARITEM:
        temp_filter = SHOP_FILTER_POPULAR;
        break;
    case CT_EVENT:
        temp_filter = SHOP_FILTER_EVENT;
        break;
    default:
        return;
    }

    ShopItem* shopitem = shop_manager_->FindShopItem(branch_item->dwGuid);
    if (shopitem)
    {
        DWORD filter = shopitem->GetItemFilter();
        filter |= temp_filter;
        shopitem->SetItemFilter(filter);
    }
    else
    {
        SUNLOG(eCRITICAL_LOG, "[ShopItemBuilder : Branch Fail] Not Found Shop Item [guid : %d]", branch_item->dwGuid);
    }
}


uint ShopItemBuilder::get_character_class_filter(BASE_ITEMINFO* item_info)
{
    uint character = SHOP_FILTER_NONE;

    if(item_info->m_wEqClass1)
        character |= SHOP_FILTER_CHAR_BERSERKER;

    if(item_info->m_wEqClass2)
        character |= SHOP_FILTER_CHAR_PALADIN;

    if(item_info->m_wEqClass3)
        character |= SHOP_FILTER_CHAR_SHADOW;

    if(item_info->m_wEqClass4)
        character |= SHOP_FILTER_CHAR_VALKYRIE;

    if(item_info->m_wEqClass5)
        character |= SHOP_FILTER_CHAR_ELEMENTALIST;

    return (character == SHOP_FILTER_NONE) ? SHOP_FILTER_CHAR_ALL : character;
}

uint ShopItemBuilder::get_category(BASE_ITEMINFO* item_info)
{
    uint category_index = SHOP_CATEGORY_ALL;

    switch (item_info->m_wType)
    {
        // 커스터마이징
    case eITEMTYPE_CHANGE_CHARACTER_APPEARANCE_ALL: 
        category_index = SHOP_CATEGORY_CUSTOMIZING_ALL;         
        break;
    case eITEMTYPE_CHANGE_CHARACTER_FACE:  
        category_index = SHOP_CATEGORY_CUSTOMIZING_FACE;         
        break;
    case eITEMTYPE_CHANGE_CHARACTER_HAIR:  
        category_index = SHOP_CATEGORY_CUSTOMIZING_HAIR;         
        break;
    case eITEMTYPE_CHANGE_CHARACTER_HEIGHT:
        category_index = SHOP_CATEGORY_CUSTOMIZING_ETC;         
        break;



        // 장비
    case eITEMTYPE_ARMOR:
        category_index = SHOP_CATEGORY_EQUIP_ARMOR; 
        break;
    case eITEMTYPE_PROTECTOR:
        category_index = SHOP_CATEGORY_EQUIP_PROTECTOR; 
        break;
    case eITEMTYPE_HELMET:
        category_index = SHOP_CATEGORY_EQUIP_HELMET; 
        break;
    case eITEMTYPE_PANTS:
        category_index = SHOP_CATEGORY_EQUIP_PANTS; 
        break;
    case eITEMTYPE_BOOTS:
        category_index = SHOP_CATEGORY_EQUIP_BOOTS; 
        break;
    case eITEMTYPE_GLOVE:
        category_index = SHOP_CATEGORY_EQUIP_GLOVES; 
        break;
    case eITEMTYPE_BELT:
        category_index = SHOP_CATEGORY_EQUIP_BELT; 
        break;
    case eITEMTYPE_SHIRTS:
        category_index = SHOP_CATEGORY_EQUIP_SHIRTS; 
        break;

    case eITEMTYPE_TWOHANDAXE:
    case eITEMTYPE_TWOHANDSWORD:
    case eITEMTYPE_ONEHANDSWORD:
    case eITEMTYPE_SPEAR:
    case eITEMTYPE_DAGGER:
    case eITEMTYPE_WHIP:
    case eITEMTYPE_ONEHANDCROSSBOW:
    case eITEMTYPE_ETHERWEAPON:
    case eITEMTYPE_SCIMITAR:
    case eITEMTYPE_STAFF:
    case eITEMTYPE_ORB:
    case eITEMTYPE_FISHING_POLE:
    case eITEMTYPE_PUNCH:
    case eITEMTYPE_GUNTLET: // _NA_004965_20120613_NEW_CHARACTER_MYSTIC
    case eITEMTYPE_POLEARM: // _NA_004965_20120613_NEW_CHARACTER_MYSTIC
#ifdef _NA_000000_20130812_NEW_CHARACTER_HELLROID
    case eITEMTYPE_HELLROID_1:
    case eITEMTYPE_HELLROID_2:
#endif //_NA_000000_20130812_NEW_CHARACTER_HELLROID
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    case eITEMTYPE_ARCBLADE:
#endif //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
        category_index = SHOP_CATEGORY_EQUIP_WEAPON;
        break;


        // 탈것
    case eITEMTYPE_RIDER:
        category_index = SHOP_CATEGORY_VEHICLE;
        break;
    case eITEMTYPE_RIDER_PARTS:
        {
            switch (item_info->m_wEqPos)
            {
            case eRIDER_PARTS_POS::HeadPart: category_index = SHOP_CATEGORY_VEHICLE_HEAD; break;
            case eRIDER_PARTS_POS::BodyPart: category_index = SHOP_CATEGORY_VEHICLE_BODY; break;
            case eRIDER_PARTS_POS::LegsPart: category_index = SHOP_CATEGORY_VEHICLE_LEGS; break;
            }
        }
        break;


        // 펫
    case eITEMTYPE_PET:
        category_index = SHOP_CATEGORY_PET;
        break;
    case eITEMTYPE_PET_FEED:
        category_index = SHOP_CATEGORY_PET_FEED;
        break;

        // 아이템
    default:
        category_index = SHOP_CATEGORY_ITEM_ALL;
        break;
    }

    return category_index;
}

uint ShopItemBuilder::get_skin_category(BASE_ITEMINFO* item_info)
{
    uint category_index = SHOP_CATEGORY_SKIN_ALL;

    switch (item_info->m_wType)
    {
        // 장비
    case eITEMTYPE_ARMOR:
        category_index = SHOP_CATEGORY_SKIN_ARMOR; 
        break;
    case eITEMTYPE_PROTECTOR:
        category_index = SHOP_CATEGORY_SKIN_PROTECTOR; 
        break;
    case eITEMTYPE_HELMET:
        category_index = SHOP_CATEGORY_SKIN_HELMET; 
        break;
    case eITEMTYPE_PANTS:
        category_index = SHOP_CATEGORY_SKIN_PENTS; 
        break;
    case eITEMTYPE_BOOTS:
        category_index = SHOP_CATEGORY_SKIN_BOOTS; 
        break;
    case eITEMTYPE_GLOVE:
        category_index = SHOP_CATEGORY_SKIN_GLOVES; 
        break;
    case eITEMTYPE_BELT:
        category_index = SHOP_CATEGORY_SKIN_BELT; 
        break;
    case eITEMTYPE_SHIRTS:
        category_index = SHOP_CATEGORY_SKIN_SHIRTS; 
        break;

    case eITEMTYPE_TWOHANDAXE:
    case eITEMTYPE_TWOHANDSWORD:
    case eITEMTYPE_ONEHANDSWORD:
    case eITEMTYPE_SPEAR:
    case eITEMTYPE_DAGGER:
    case eITEMTYPE_WHIP:
    case eITEMTYPE_ONEHANDCROSSBOW:
    case eITEMTYPE_ETHERWEAPON:
    case eITEMTYPE_SCIMITAR:
    case eITEMTYPE_STAFF:
    case eITEMTYPE_ORB:
    case eITEMTYPE_FISHING_POLE:
    case eITEMTYPE_PUNCH:
    case eITEMTYPE_GUNTLET: // _NA_004965_20120613_NEW_CHARACTER_MYSTIC
    case eITEMTYPE_POLEARM: // _NA_004965_20120613_NEW_CHARACTER_MYSTIC
#ifdef _NA_000000_20130812_NEW_CHARACTER_HELLROID
    case eITEMTYPE_HELLROID_1:
    case eITEMTYPE_HELLROID_2:
#endif //_NA_000000_20130812_NEW_CHARACTER_HELLROID
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    case eITEMTYPE_ARCBLADE:
#endif //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
        category_index = SHOP_CATEGORY_SKIN_WEAPON;
        break;

    case eITEMTYPE_PET:
        category_index = SHOP_CATEGORY_PET_SKIN;
        break;
    }

    return category_index;
}

void ShopItemBuilder::ClearAll()
{
    ASSERT(shop_manager_);
    index_generator_.clear();
}


} //namespace GameShop

#endif // _NA_0_20101015_SOLAR_SHOP_TEMP_REFACTORING


#endif //#if !defined _NA_0_20110613_GAME_SHOP_RENEWAL
