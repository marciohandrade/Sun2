#pragma once


#include <slm/IncludeSLM.h>

#include "StorageItemList.h"
#include "ProductItemList.h"

class CShopListManager;

namespace ConvenienceStore
{
    class CashShop
    {
    private:
        SalesZoneScriptVersion builded_sales_zone_script_version_;
        CShopListManager* shop_list_manager_;

        SalesZoneScriptVersion sale_zone_script_version_;
        BannerZoneScriptVersion banner_zone_script_version_;

        StorageItemList storage_item_list_;
        StorageItemList storage_gift_list_;
        ProductItemList product_list_;

        CashInfo cash_info_;


        bool is_my_item_receive_;
        bool is_all_item_receive_;
        bool has_new_present_;


    protected:
        CashShop();
        virtual ~CashShop();

        bool is_open_with_gift_menu_;

    private:
        static CashShop* static_instance_;

    public:
        static CashShop* instance() { return static_instance_; }

        CShopListManager* GetShopListManager() const { return shop_list_manager_; }
        StorageItemList& GetStorageItemData() { return storage_item_list_; }
        StorageItemList& GetGiftStorageItemData() { return storage_gift_list_; }
        ProductItemList& GetProductItemData() { return product_list_; }

        void SetCashInfo(CashInfo value) { cash_info_ = value; }

        double GetMyCash() { return cash_info_.cash_; }
        double GetMyPPCash() { return cash_info_.pp_cash_; }
        double GetMyMileage() { return cash_info_.mileage_; }

    public:
        void OpenCashShopWithGiftMenu();
        virtual bool IsShowShop();
        virtual void ToggleOpenKey();
        virtual void ShowCashShop();
        virtual void HideCashShop();

    public:
        void CheckNewPresnet() { SEND_CG_SHOP_EXIST_GIFT_SYN(); }
        void CheckPresentTargetName( LPCTSTR name ) { SEND_CG_SHOP_CHECK_CHAR_NAME_SYN(name); }
        void ReceiveStorageItem( bool bAllReceive, bool bIsMyItem );

        void SendBuyItem(std::vector<PurchaseItem*>& purchase_list, Define::CashType cash_type);// { SEND_CG_SHOP_BUY_ITEM_SYN(purchase_list, cash_type); }
        void SendGiftItem(std::vector<PurchaseItem*>& purchase_list, Define::CashType cash_type, LPCTSTR recv_name, LPCTSTR recv_msg);// { SEND_CG_SHOP_SEND_GIFT_SYN(purchase_list, cash_type, recv_name, recv_msg); }
        void SendUseageStorageBuyItemOnce() { SEND_CG_SHOP_USE_STORAGE_ITEM_SYN(storage_item_list_.GetItemData(0)); }
        void SendUseageStorageGiftItemOnce() { SEND_CG_SHOP_USE_STORAGE_ITEM_SYN(storage_gift_list_.GetItemData(0)); }
        void SendUseageStorageBuyItemOnce( StorageItem* pItemData ) { SEND_CG_SHOP_USE_STORAGE_ITEM_SYN(storage_item_list_.Find(pItemData)); }
        void SendUseageStorageGiftItemOnce( StorageItem* pItemData ) { SEND_CG_SHOP_USE_STORAGE_ITEM_SYN(storage_gift_list_.Find(pItemData)); }
        bool IsEmptyBuyStorage() { return storage_item_list_.GetCountPerPage() <= 0; }
        bool IsEmptyGiftStorage() { return storage_gift_list_.GetCountPerPage() <= 0; }

        void RequestBuyStorageList( int page_index ) { SEND_CG_SHOP_BUY_STORAGE_LIST_SYN( page_index, 10 ); }
        void RequestGiftStorageList( int page_index ) { SEND_CG_SHOP_GIFT_STORAGE_LIST_SYN( page_index, 10 ); }

        void SendNameCheck( LPCTSTR name );
        void CheckNewPresent() { SEND_CG_SHOP_EXIST_GIFT_SYN(); }

        bool HasNewPresent() const { return has_new_present_; }
        void SetHasNewPresent(bool value) { has_new_present_ = value; }

        void GetErrorStringByErrorCode(int error_code, OUT TCHAR* output_msg, int string_size, DWORD* string_color = NULL );

    protected:
        void NetworkProc(MSG_BASE* pk);

    private:
        bool BuildShopList();
        void OutputErrorMessage( int error_code );

    private:
        void SEND_CG_SHOP_BUY_ITEM_SYN(std::vector<PurchaseItem*>& purchase_list, Define::CashType cash_type);
        void SEND_CG_SHOP_SEND_GIFT_SYN(std::vector<PurchaseItem*>& purchase_list, Define::CashType cash_type, LPCTSTR recv_name, LPCTSTR recv_msg );
        void SEND_CG_SHOP_VERSION_SYN();
        void SEND_CG_SHOP_OPEN_SYN();
        void SEND_CG_SHOP_CLOSE_NTF();
        void SEND_CG_SHOP_CASH_SYN();
        void SEND_CG_SHOP_BUY_STORAGE_LIST_SYN( int page_index, int request_count);
        void SEND_CG_SHOP_GIFT_STORAGE_LIST_SYN( int page_index, int request_count);

        void SEND_CG_SHOP_USE_STORAGE_ITEM_SYN(StorageItem* pItemData);
        void SEND_CG_SHOP_CHECK_CHAR_NAME_SYN(LPCTSTR name);
        void SEND_CG_SHOP_EXIST_GIFT_SYN();

    private:
        virtual void OnShowCashShop() {};
        virtual void OnHideCashShop() {};
        virtual void OnUpdateCash() {};
        virtual void OnUpdateStorageBuyList() {}
        virtual void OnUpdateStorageGiftList() {}
        virtual void OnUpdateShopItemList(){}

        virtual void OnSuccessBuyItem( ShopItemInfoEx* pItemArray, uint ItemCount ) {}
        virtual void OnSuccessGiftItem( ShopItemInfoEx* pItemArray, uint ItemCount ) {}
        
        virtual void OnBeginWaiting() {}
        virtual void OnEndWaiting() {}

        virtual void OnReceveStorageItem( LPCTSTR pMessage, bool bIsSuccess, bool bIsAllReceive, bool bIsMyItemReceive, bool bIsContinue ) {}

        virtual void OnStartReceiveStorageItem( bool bAllReceive, bool bIsMyItem ) {}
        virtual void OnContinueReceiveStorageItem( bool bAllReceive, bool bIsMyItem ) {}

        virtual void OnResultNameCheck( bool bSuccess, LPCTSTR name, LEVELTYPE level, LPCTSTR guild_name ) {}


    };
}

