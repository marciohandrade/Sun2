#include "SunClientPrecompiledHeader.h"
#include "ShopItemObject.h"
#include "ProductInfo.h"
#include "ItemInfoParser.h"


namespace ConvenienceStore
{

    //------------------------------------------------------------------------------ 
    bool ShopItemObject::CheckProperty(CShopProduct* shop_product,
                                        CShopProductLink* shop_product_link, 
                                        int waste_seq, int infinite_seq, int date_seq)
    {
        if ((shop_product->ProductType != Define::kProductType[Define::kShopProduct_Waste]) &&
            (shop_product->ProductType != Define::kProductType[Define::kShopProduct_Infinite]) &&
            (shop_product->ProductType != Define::kProductType[Define::kShopProduct_Date]))
        {
            return false;
        }
        if ((shop_product->ProductType == Define::kProductType[Define::kShopProduct_Waste]) && 
            (shop_product_link->PropertySeq != waste_seq))
        {
            //갯수형 상품인데 갯수 정보가 없으면
            return false;
        }
        if ((shop_product->ProductType == Define::kProductType[Define::kShopProduct_Infinite]) && 
            (shop_product_link->PropertySeq != infinite_seq))
        {
            //영구형 상품인데 갯수 정보가 없으면
            return false;
        }
        if ((shop_product->ProductType == Define::kProductType[Define::kShopProduct_Date]) && 
            (shop_product_link->PropertySeq != date_seq))
        {
            //날자형 상품인데 날자정보가 없으면
            return false;
        }
        return true;
    }
    //------------------------------------------------------------------------------ 
    bool ShopItemObject::FindProperty(Define::ShopPropertyType property_type, CShopProduct* shop_product, 
                                    int price_seq, eCHAR_TYPE char_type, 
                                    OUT CShopProductLink*& shop_product_link, OUT CShopProperty*& shop_property)
    {
        shop_product_link = NULL;
        productlink_list()->SetProductFirst(shop_product->ProductSeq, price_seq);
        while (productlink_list()->GetProductNext(shop_product_link) == true)
        {
            if (CheckProperty(shop_product, 
                shop_product_link, 
                Define::kPropertySeq_Waste[property_type],
                Define::kPropertySeq_Infinite[property_type],
                Define::kPropertySeq_Date[property_type]) == false)
            {
                continue;
            }
            BASE_ITEMINFO* item_info = ItemInfoParser::Instance()->GetItemInfo(shop_product_link->InGameProductID);
            if (item_info == NULL)
            {
                continue;
            }
            if (item_info->IsRiderItem() == false && item_info->IsRiderPartsItem() == false)
            {
                //! 파츠 장비 일경우에는 클래스를 체크 하지 않는다.
                if ((char_type >= eCHAR_BERSERKER) && 
                    (char_type <= (eCHAR_TYPE_MAX - 1)) &&
                    (item_info->CheckValidEquipClass(char_type) == false))
                {
                    continue;
                }
            }

            shop_property = NULL;
            if (property_list()->GetValueByKey(shop_product_link->PropertySeq, shop_property) == true)
            {
                bool is_price_property = (property_type == Define::kShopProperty_Price) && 
                    (shop_property->PropertyType == Define::kPropertyType_Price);
                bool is_attribute_property = (property_type != Define::kShopProperty_Price) && 
                    (shop_property->PropertyType == Define::kPropertyType_Attribute);
                if (is_price_property || is_attribute_property)
                {
                    break;
                }
            }
        }

        return (shop_product_link != NULL && shop_property != NULL);
    }

    //------------------------------------------------------------------------------ 
    bool ShopItemObject::FillProductInfo(CShopProduct* shop_product, int price_seq, eCHAR_TYPE char_type, OUT ProductInfo& product_info)
    {
        if (shop_product == NULL)
        {
            return false;
        }
        CShopProductLink* shop_product_link = NULL;
        CShopProperty* shop_property = NULL;
        if (FindProperty(Define::kShopProperty_Price, shop_product, price_seq, char_type, 
            OUT shop_product_link, OUT shop_property) == false)
        {
            return false;
        }
        product_info.set_price(shop_product_link, shop_property);

        for (int i = Define::kShopProperty_SellKind; i < Define::kShopProperty_Size; ++i)
        {
            shop_product_link = NULL;
            shop_property = NULL;
            Define::ShopPropertyType property_type = static_cast<Define::ShopPropertyType>(i);
            if (FindProperty(property_type, 
                shop_product, 
                price_seq, 
                eCHAR_NONE, 
                OUT shop_product_link, 
                OUT shop_property) == true)
            {
                product_info.property_link(property_type)->SetProductLink(shop_product_link);
                product_info.property_link(property_type)->SetProperty(shop_property);
            }
        }
        return true;
    }

}