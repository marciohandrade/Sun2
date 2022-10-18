#include "SunClientPrecompiledHeader.h"
#include "PurchaseItem.h"
#include "ProductInfo.h"

namespace ConvenienceStore
{

    //------------------------------------------------------------------------------ 
    PurchaseItem* PurchaseItem::CreatePurchase(ProductItem* pProductItem, int basket_product_index)
    {
        if (pProductItem == NULL)
        {
            return NULL;
        }

        PurchaseItem* pItem = new PurchaseItem(pProductItem);
        pItem->SetBasketProductIndex( basket_product_index );

        if (pItem->is_package() == false)
        {
            //! 기본은 첫번째것으로 셋팅해 놓는다.
            pItem->SetPriceSequence( pItem->first_info()->GetPriceProperty().GetPriceSequence() );
        }

        return pItem;
    }

    //------------------------------------------------------------------------------ 
    PurchaseItem::PurchaseItem() 
    : ProductItem()
    , shop_price_seq_(0)
    , basket_product_index_(-1)
    {

    }


    //------------------------------------------------------------------------------ 
    PurchaseItem::PurchaseItem(ProductItem* product)
    : ProductItem( product )
    , basket_product_index_(-1)
    , shop_price_seq_(0)
    {
    }
    //------------------------------------------------------------------------------ 
    PurchaseItem::~PurchaseItem()
    {

    }

    //------------------------------------------------------------------------------ 
    ProductInfo& PurchaseItem::GetCurProductInfo()
    {
        std::vector<ProductInfo>& product_info_array = GetInfoVector();
        for( size_t i=0;i!=product_info_array.size();++i)
        {
            if (shop_price_seq_ == -1)
            {
                return product_info_array[i];
            }
            else if (product_info_array[i].GetPriceProperty().GetPriceSequence() == shop_price_seq_ )
            {
                return product_info_array[i];
            }
        }
        return product_info_array[0];
    }
}