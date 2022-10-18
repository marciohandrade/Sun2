#if !defined _NA_0_20110613_GAME_SHOP_RENEWAL


#ifdef _NA_0_20101015_SOLAR_SHOP_TEMP_REFACTORING


#ifndef SUN_GAMESERVER_GAMESHOP_SHOPITEMBUILDER_H_
#define SUN_GAMESERVER_GAMESHOP_SHOPITEMBUILDER_H_


#if _MSC_VER > 1000
#pragma once
#endif

#include "./GameShop.IndexGenerator.h"
#include <ShopManager.h>

namespace GameShop
{

//===============================================================================================
//
// class ShopItemBuilder
//
//===============================================================================================
class ShopItemBuilder
{
public:
    static IndexGenerator index_generator_;

    ShopItemBuilder(ShopManager* shop_manager);
    ~ShopItemBuilder();

    void BuildShopItem(tblSellItem* sell_item, uint count);
    void BuildShopItem(tblSellPackage* sell_item, uint count);
    void BuildShopItem(tblItemBranch* branch_item, uint count);
    void ClearAll();

private:
    void Build(tblSellItem* sell_item);
    void Build(tblSellPackage* sell_item);
    void Build(tblItemBranch* branch_item);

    uint get_category(BASE_ITEMINFO* item_info);
    uint get_character_class_filter(BASE_ITEMINFO* item_info);
    uint get_skin_category(BASE_ITEMINFO* item_info);

    ShopManager* shop_manager_;
};


} // namespace GameShop

#endif // SUN_GAMESERVER_GAMESHOP_SHOPITEMBUILDER_H_

#endif //_NA_0_20101015_SOLAR_SHOP_TEMP_REFACTORING


#endif //#if !defined _NA_0_20110613_GAME_SHOP_RENEWAL
