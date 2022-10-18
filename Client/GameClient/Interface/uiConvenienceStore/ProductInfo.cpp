#include "SunClientPrecompiledHeader.h"
#include "ProductInfo.h"
#include "ItemInfoParser.h"

namespace ConvenienceStore
{

    //------------------------------------------------------------------------------ 
    ProductInfo::ProductInfo() 
        : shop_product_ptr_(0)
        , item_info_ptr_(0)
        , icon_image_ptr_(0)
    {
    }
    //------------------------------------------------------------------------------ 
    ProductInfo::ProductInfo( const ProductInfo& from )
    {
        shop_product_ptr_ = from.shop_product_ptr_;
        price_property_ = from.price_property_;
        sell_property_ = from.sell_property_;
        inchant_property_ = from.inchant_property_;
        socket_property_ = from.socket_property_;
        item_info_ptr_ = from.item_info_ptr_;
        icon_image_ptr_ = ICONMGR()->Add(from.item_info_ptr_->m_wVICode);
    }
    //------------------------------------------------------------------------------ 
    ProductInfo::ProductInfo(CShopProduct* _shop_product)
    {
        shop_product_ptr_ = _shop_product;
        item_info_ptr_ = NULL;
        icon_image_ptr_ = NULL;
    }
    //------------------------------------------------------------------------------ 
    ProductInfo::ProductInfo(CShopProduct* _shop_product, CShopProductLink* _price_productlink, CShopProperty* _price_property)
    {
        shop_product_ptr_ = _shop_product;

        price_property_.SetProductLink( _price_productlink );
        price_property_.SetProperty( _price_property );

        item_info_ptr_ = ItemInfoParser::Instance()->GetItemInfo(price_property_.GetProductID());
        icon_image_ptr_ = ICONMGR()->Add(item_info_ptr_->m_wVICode);
    }
    //------------------------------------------------------------------------------ 
    ProductInfo::~ProductInfo()
    {
    }
    //------------------------------------------------------------------------------ 
    void ProductInfo::set_price(CShopProductLink* _shop_productlink, CShopProperty* _shop_property)
    {
        price_property_.SetProductLink( _shop_productlink );
        price_property_.SetProperty( _shop_property );

        if (icon_image_ptr_ != NULL)
        {
            ICONMGR()->RemoveIcon(icon_image_ptr_);
            icon_image_ptr_ = NULL;
        }
        item_info_ptr_ = ItemInfoParser::Instance()->GetItemInfo(price_property_.GetProductID());
        icon_image_ptr_ = ICONMGR()->Add(item_info_ptr_->m_wVICode);
    }
    //------------------------------------------------------------------------------ 
    void ProductInfo::set_sell(CShopProductLink* _shop_productlink, CShopProperty* _shop_property)
    {
        sell_property_.SetProductLink( _shop_productlink );
        sell_property_.SetProperty( _shop_property );
    }
    //------------------------------------------------------------------------------ 
    void ProductInfo::set_inchant(CShopProductLink* _shop_productlink, CShopProperty* _shop_property)
    {
        price_property_.SetProductLink( _shop_productlink );
        price_property_.SetProperty( _shop_property );
    }
    //------------------------------------------------------------------------------ 
    void ProductInfo::set_socket(CShopProductLink* _shop_productlink, CShopProperty* _shop_property)
    {
        socket_property_.SetProductLink( _shop_productlink );
        socket_property_.SetProperty( _shop_property );
    }
    //------------------------------------------------------------------------------ 
    PropertyInfo* ProductInfo::property_link(Define::ShopPropertyType e)
    {
        switch (e)
        {
        case Define::kShopProperty_Price: return &price_property_;
        case Define::kShopProperty_SellKind: return &sell_property_;
        case Define::kShopProperty_Inchant: return &inchant_property_;
        case Define::kShopProperty_Socket: return &socket_property_;
        }
        return NULL;
    }
    //------------------------------------------------------------------------------ 
    Define::ShopProductType ProductInfo::product_type()
    {
        return ConvenienceStore::GetProductType(shop_product_ptr_);
    }
}