#pragma once

#ifdef _NA_0_20110613_GAME_SHOP_RENEWAL
    #ifndef _YMS_TREECONTROL
        #error "Impossible Build!! - Reason: WZENGINEVER < 400!!"
    #endif //#if WZENGINEVER >= 400
#endif //#if WZENGINEVER >= 400

#include <slm/IncludeSLM.h>

#include "uiBaseMan.h"
#include "CashShopHeader.h"
#include "TreeMenuInfoParser.h"
#include "uiConvenienceStore_Basket.h"

class uiConvenienceStore_Basket;
class uiConvenienceStore_Counter;
class uiConvenienceStore_PopupMenuFour;
class uiConvenienceStore_PopupMenuThree;
class uiConvenienceStore_PopupMenuTwo;
class uiConvenienceStore_PopupMenuFive;
class uiConvenienceStore_Purchase;
class uiConvenienceStore_Confirm;
class uiConvenienceStore_Gift;
class uiConvenienceStore_FriendList;
class uiConvenienceStore_PopupPackage;
class uiConvenienceStore_ItemDetail;
class uiConvenienceStore_Ask;
class uiConvenienceStore_PopupMenu;
class uiConvenienceStore_CashType;


#include "CashShop.h"

//------------------------------------------------------------------------------ 
class uiConvenienceStoreMan 
    : public uiBaseMan
    , public ConvenienceStore::CashShop
{
private:
    ConvenienceStore::Define::ShopMenuType first_open_menu_;


private:
    // override ConvenienceStore::CashShop
    bool IsShowShop();

    void OnShowCashShop();
    void OnHideCashShop();

    void OnUpdateShopItemList();
    void OnUpdateCash();
    void OnUpdateStorageBuyList();
    void OnUpdateStorageGiftList();

    void OnBeginWaiting();
    void OnEndWaiting();

    void OnSuccessBuyItem( ShopItemInfoEx* pItemInfo, uint ItemCount );
    void OnSuccessGiftItem( ShopItemInfoEx* pItemInfo, uint ItemCount );

    void OnStartReceiveStorageItem( bool bAllReceive, bool bIsMyItem );
    void OnReceveStorageItem( LPCTSTR pMessage, bool bIsSuccess, bool bIsAllReceive, bool bIsMyItemReceive, bool bIsContinue );
    void OnResultNameCheck( bool bSuccess, LPCTSTR name, LEVELTYPE level, LPCTSTR guild_name );


public:
    uiConvenienceStoreMan(InterfaceManager* manager);

private:
    virtual void OnInitialize();
    virtual void NetworkProc(MSG_BASE* pk);

public:
    bool is_show_popup_menu();

    uiConvenienceStore_Counter* counter();
    uiConvenienceStore_Basket* basket();
    uiConvenienceStore_PopupMenuFive* popup_menu_five();
    uiConvenienceStore_PopupMenuFour* popup_menu_four();
    uiConvenienceStore_PopupMenuThree* popup_menu_three();
    uiConvenienceStore_PopupMenuTwo* popup_menu_two();
    uiConvenienceStore_PopupPackage* popup_package();
    uiConvenienceStore_PopupMenu* popup_menu(ConvenienceStore::Define::PopupMenuKind e);
    uiConvenienceStore_Purchase* purchase();
    uiConvenienceStore_Confirm* confirm();
    uiConvenienceStore_Gift* gift();
    uiConvenienceStore_FriendList* friend_list();
    uiConvenienceStore_ItemDetail* item_detail();
    uiConvenienceStore_Ask* ask();
    uiConvenienceStore_CashType* cashtype();

    void RegisterTooltip(ConvenienceStore::ProductInfo* product_info, DWORD64 wztime64);
    void RegisterTooltip(ConvenienceStore::ProductItem* product);
    void RegisterTooltip(SLOTCODE item_code);

    void RegisterTooltip(ConvenienceStore::ShopItemObject* pObject);

private:
    void Fill_TreeMenuInfo_MyPage();
    void Fill_TreeMenuInfo_Counter(CShopList* shop_list);
    void Fill_TreeMenuInfo_RootCategory(BASE_TreeMenuInfo::Group_Type group_type, 
        CShopList* info, 
        CShopCategory* category,
        int& menu0, 
        bool is_make_all_item);
    void Fill_TreeMenuInfo_SubCategory(BASE_TreeMenuInfo::Group_Type group_type, 
        CShopList* info, 
        CShopCategory* shop_category, 
        BYTE menu0, BYTE menu1, BYTE menu2, BYTE menu3, 
        int deapth);
};
