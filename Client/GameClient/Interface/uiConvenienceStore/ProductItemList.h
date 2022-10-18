#pragma once

#include "ProductItem.h"

namespace ConvenienceStore
{
    class ProductItemList
    {
    public:
        enum eProductType
        {
            PRODUCT_TYPE_CASH,
            PRODUCT_TYPE_PPCARD,
            PRODUCT_TYPE_MILEAGE,
            PRODUCT_TYPE_MAX,
        };

    private:
        int last_category_sequence_;
        eCHAR_TYPE last_class_filter_;
        eProductType product_type_;
        int image_type_index_;

        int current_page_index_;

        std::vector<ProductItem*> product_array_;

    public:
        ProductItemList();
        ~ProductItemList();

        void ClearProductArray();

        std::vector<ProductItem*>& GetProductArray() { return product_array_; }
        void SetLastCategorySequence(int sequence) { last_category_sequence_ = sequence; }
        void SetLastClassFilter(eCHAR_TYPE filter ) { last_class_filter_ = filter; }
        void SetCurrentPage( int page ) { current_page_index_ = page; }

        int GetLastCategorySequence() const { return last_category_sequence_; }
        eCHAR_TYPE GetLastCategoryFilter() const { return last_class_filter_; }
        int GetCurrentPage() const { return current_page_index_; }
        int GetMaxPage();

        bool IsValidProduct( ShopItemObject* pObject );

        void SortProductList(Define::ShopStringCode sort_type);

        void AddProduct( ProductItem* pProduct ) { product_array_.push_back( pProduct ); }
        size_t GetProductCount() { return product_array_.size(); }
        ProductItem* GetProduct( size_t index ) { return product_array_.at(index); }

        void BuildProductList( eProductType ProductType, int CategorySequence, eCHAR_TYPE ClassFilter, int ImageTypeIndex );
        void _FillProductList( CShopCategory* shop_category);
    };
}