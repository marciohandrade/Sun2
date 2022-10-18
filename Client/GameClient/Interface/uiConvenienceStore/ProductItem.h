#pragma once


#include "ShopItemObject.h"
#include "ProductInfo.h"

namespace ConvenienceStore
{


    class ProductItem 
        : public ShopItemObject
    {
    public:
        static ProductItem* CreateProduct( ProductItem* pProductItem );
        static ProductItem* CreateProduct( CShopCategory* shop_category_ptr_, CShopPackage* shop_package_ptr_, eCHAR_TYPE char_type);

    private:
        CShopCategory* shop_category_ptr_;
        CShopPackage* shop_package_ptr_; 

        IconImage* package_image_ptr_;

        std::vector<ProductInfo> product_info_array_;

        Define::ShopProductType product_item_type_;

    protected:
        ProductItem();
        ProductItem(CShopCategory* shop_category, CShopPackage* shop_package);

    public:
        ProductItem( ProductItem* form_ptr );
        virtual ~ProductItem();

        virtual LPCTSTR GetName() { return shop_package_ptr_->PackageProductName; }
        virtual ShopItemObjectType GetObjectType() { return OBJECT_TYPE_PRODUCT; }
        virtual IconImage* GetIconImage() { return package_image_ptr_; }


        virtual bool is_equl(const ProductItem* product) const { return (this == product); }
        virtual bool is_package(); //!<CShopPackage* shop_package_ptr_ 확인해서 다시 코딩
        virtual bool is_lotto();
        virtual bool is_possible_sales();
        virtual bool is_possible_gift();
        virtual bool has_preiview_rider_equip_parts();
        virtual bool has_preiview_player_equip_parts(OUT eCHAR_TYPE* char_class_type = NULL);

        // for global
        bool is_cash_only();
        bool is_pcash_only();
        bool is_valid_cashtype( Define::CashType cash_type );

        // for china
        bool is_mileage_only();

        virtual ProductInfo* first_info();
        virtual SLOTCODE item_code(int index);

        //! 묶음상품일 경우에는 seq에 상관없이 합계를 리턴함
        virtual CASH GetPrice(int shop_price_seq = -1);
        virtual tm& GetStartDate() { return shop_package_ptr_->StartDate; }
        virtual tm& GetEndDate() { return shop_package_ptr_->EndDate; }


        LPCTSTR GetDescription() { return shop_package_ptr_->Description; }
        int GetDisplaySequence() { return shop_category_ptr_->ProductDisplaySeq; }
        int GetPackageSequence() { return shop_package_ptr_->PackageProductSeq; }
        LPCTSTR GetCautionText() { return shop_package_ptr_->Caution; }


    public:

        bool IsWasteItem() { return product_item_type_ == Define::kShopProduct_Waste; }      // 소모성
        bool IsInfiniteItem() { return product_item_type_ == Define::kShopProduct_Infinite; }// 영구성
        bool IsDateItem() { return product_item_type_ == Define::kShopProduct_Date; }        // 기간제한

    public:
        static bool SortProductByDateInc(ProductItem*& ref_left, ProductItem*& ref_right);
        static bool SortProductByDateDesc(ProductItem*& ref_left, ProductItem*& ref_right);
        static bool SortProductByPriceInc(ProductItem*& ref_left, ProductItem*& ref_right);
        static bool SortProductByPriceDesc(ProductItem*& ref_left, ProductItem*& ref_right);
        static bool SortProductByNameInc(ProductItem*& ref_left, ProductItem*& ref_right);
        static bool SortProductByNameDesc(ProductItem*& ref_left, ProductItem*& ref_right);

        size_t GetProductCount() { return shop_package_ptr_->GetProductCount(); }


        std::vector<ProductInfo>& GetInfoVector() { return product_info_array_; }
        CShopCategory* GetShopCategory() const { return shop_category_ptr_; }
        CShopPackage* GetShopPackage() const { return shop_package_ptr_; }
    };
}


