#pragma once


#include "ProductItem.h"


namespace ConvenienceStore
{

    class PurchaseItem 
        : public ProductItem
    {
    public:
        static PurchaseItem* CreatePurchase(ProductItem* product, int basket_product_index); 

    public:
        PurchaseItem();
        PurchaseItem(ProductItem* product);
        virtual ~PurchaseItem();

        virtual ShopItemObjectType GetObjectType() { return OBJECT_TYPE_PURCHASE; }
        virtual bool is_equl(const ProductItem* purchase) const { return (this == purchase); }

        int GetPriceSequence() { return shop_price_seq_; }
        void SetPriceSequence( int price_sequence ) { shop_price_seq_ = price_sequence; }

        int GetBasketProduceIndex() const { return basket_product_index_; }
        void SetBasketProductIndex(int value) { basket_product_index_ = value; }

        ProductInfo& GetCurProductInfo();

    private:
        int shop_price_seq_;

        //! 장바구니에담긴 순서(Counter에서 넘어왔으면 -1)
        int basket_product_index_;
    };
}