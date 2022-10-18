#pragma once

#include "CashShopHeader.h"
#include "PropertyInfo.h"


namespace ConvenienceStore
{

    class ProductInfo
    {
    public:
        ProductInfo();
        ProductInfo( const ProductInfo& from );
        ProductInfo(CShopProduct* shop_product);
        ProductInfo(CShopProduct* shop_product, CShopProductLink* price_productlink, CShopProperty* _price_property);
        virtual ~ProductInfo();

    public:
        PropertyInfo* property_link(Define::ShopPropertyType e);
        Define::ShopProductType product_type();
        void set_price(CShopProductLink* shop_productlink, CShopProperty* shop_property);
        void set_sell(CShopProductLink* shop_productlink, CShopProperty* shop_property);
        void set_inchant(CShopProductLink* shop_productlink, CShopProperty* shop_property);
        void set_socket(CShopProductLink* shop_productlink, CShopProperty* shop_property);


        bool is_skin() { return sell_property_.IsSkin(); }
        bool is_etheria() { return sell_property_.IsEtheria(); }
        bool is_etheria_with_divine() { return sell_property_.IsEtheriaWithDivine(); }

        LPCTSTR GetProductName() const { return shop_product_ptr_->ProductName; }

        IconImage* GetIconImage() { return icon_image_ptr_; }
        void SetIconImage( IconImage* pIconImage ) { icon_image_ptr_ = pIconImage; }

        const PropertyInfo& GetPriceProperty() const { return price_property_; }
        const PropertyInfo& GetSellProperty() const { return sell_property_; }
        const PropertyInfo& GetInchantProperty() const { return inchant_property_; }
        const PropertyInfo& GetSocketProperty() const { return socket_property_; }
        BASE_ITEMINFO* GetItemInfo() { return item_info_ptr_; }

        void SetShopProduct( CShopProduct* pShopProduct ) { shop_product_ptr_ = pShopProduct; }

        int GetProductType() const { return shop_product_ptr_ ? shop_product_ptr_->ProductType: 0; }

    private:
        CShopProduct* shop_product_ptr_;

        PropertyInfo price_property_;         //! 3일짜리 2개 형태는 지원하지 않음
        PropertyInfo sell_property_;
        PropertyInfo inchant_property_;
        PropertyInfo socket_property_;

        BASE_ITEMINFO* item_info_ptr_;
        IconImage* icon_image_ptr_;
    };

}