#pragma once

#include "CashShopHeader.h"

class CShopProduct;
class CShopProductLink;

namespace ConvenienceStore
{
    //------------------------------------------------------------------------------ 
    enum ShopItemObjectType
    {
        OBJECT_TYPE_PRODUCT,
        OBJECT_TYPE_PURCHASE,
        OBJECT_TYPE_STORAGE_BUY,
        OBJECT_TYPE_STORAGE_GIFT,
    };


    //------------------------------------------------------------------------------ 
    class ShopItemObject
    {
    protected:
        ShopItemObject() {}

    public:
        virtual ShopItemObjectType GetObjectType() PURE;
        virtual LPCTSTR GetName() PURE;
        virtual IconImage* GetIconImage() PURE;


        bool IsProductItem() { return GetObjectType() == OBJECT_TYPE_PRODUCT; }
        bool IsPurchase() { return GetObjectType() == OBJECT_TYPE_PURCHASE; }

        bool IsStorageBuy() { return GetObjectType() == OBJECT_TYPE_STORAGE_BUY; }
        bool IsStorageGift() { return GetObjectType() == OBJECT_TYPE_STORAGE_GIFT; }

        bool IsProduct() { return IsProductItem() || IsPurchase(); }
        bool IsStorageItem() { return IsStorageBuy() || IsStorageGift(); }


    protected:
        static bool CheckProperty(CShopProduct* shop_product, CShopProductLink* shop_product_link, int waste_seq, int infinite_seq, int date_seq);
        static bool FindProperty(Define::ShopPropertyType property_type, CShopProduct* shop_product, int price_seq, eCHAR_TYPE char_type, OUT CShopProductLink*& shop_product_link, OUT CShopProperty*& shop_property);
        static bool FillProductInfo(CShopProduct* shop_product, int price_seq, eCHAR_TYPE char_type, OUT ProductInfo& product_info);
    };
}
