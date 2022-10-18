#pragma once

#include "ShopItemObject.h"
#include "ProductInfo.h"

namespace ConvenienceStore
{

    class StorageItem
        : public ShopItemObject
    {
    private:
        ShopItemObjectType object_type_;

        StorageItemInfo* item_info_ptr_;
        ProductInfo product_info_;

    public:
        static StorageItem* Create( const StorageItemInfoBuy& ItemInfo );
        static StorageItem* Create( const StorageItemInfoGift& ItemInfo );

        ~StorageItem();

    private:
        StorageItem();


    public:
        bool Compare( long storage_sequence, long storage_item_sequence, char item_type );

    public:
        StorageItemInfo* GetItemInfo() const { return item_info_ptr_; }

        // override ShopItemObject
        ShopItemObjectType GetObjectType() { return object_type_; }
        LPCTSTR GetName(){ return product_info_.GetProductName(); }
        ProductInfo& GetProductInfo() { return product_info_; }
        IconImage* GetIconImage() { return product_info_.GetIconImage(); }


        uint GetDeleteDate() { return item_info_ptr_->delete_date_; }
        LPCTSTR GetSendCharacterName();
        LPCTSTR GetGiftMessage();
    };
}