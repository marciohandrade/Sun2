#pragma once

#include "CashShopHeader.h"


namespace ConvenienceStore
{

    class PropertyInfo
    {
    private:
        CShopProductLink* shop_link_;
        CShopProperty* shop_property_;

    public:
        PropertyInfo()
        {
            shop_link_ = NULL;
            shop_property_ = NULL;
        }

        CShopProductLink* GetProductLink() const { return shop_link_; }
        void SetProductLink(CShopProductLink* value) { shop_link_ = value; }

        CShopProperty* GetProperty() const { return shop_property_; }
        void SetProperty(CShopProperty* value) { shop_property_ = value; }

        int GetPropertyType() const { return shop_property_ ? shop_property_->PropertyType : 0; }
        int GetProductID() const { return shop_link_ ? shop_link_->InGameProductID : 0; }
        int GetPriceSequence() const { return shop_link_ ? shop_link_->PriceSeq : 0; }
        int GetPrice() const { return shop_link_ ? shop_link_->Price : 0; }
        int GetValue() const { return (shop_link_ && shop_property_ ) ? atoi(shop_link_->Value) : 0; }

        KindOfItemType::value_type GetAttributeType() const 
        {
            if ( GetPropertyType() == Define::kPropertyType_Attribute )
            {
                return static_cast<KindOfItemType::value_type>(GetValue());
            }
            return KindOfItemType::Counts;
        }

        bool IsSkin() { return GetAttributeType() == KindOfItemType::Shell; }
        bool IsEtheria() { return GetAttributeType() == KindOfItemType::Etheria; }
        bool IsEtheriaWithDivine() { return GetAttributeType() == KindOfItemType::EtheriaWithDivine; }
        bool IsDivine() { return GetAttributeType() == KindOfItemType::Divine; }
    };
}