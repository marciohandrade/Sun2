#include "SunClientPrecompiledHeader.h"
#include "ProductItemList.h"


namespace ConvenienceStore
{

    ProductItemList::ProductItemList()
    {
        last_category_sequence_ = 0;
        last_class_filter_ = eCHAR_NONE;
        current_page_index_ = 1;
    }
    ProductItemList::~ProductItemList()
    {
        ClearProductArray();
    }

    void ProductItemList::ClearProductArray()
    {
        for( size_t i=0;i!=product_array_.size();++i)
        {
            ProductItem* pProduct = product_array_.at(i);
            SAFE_DELETE(pProduct);
        }

        product_array_.clear();

        last_category_sequence_ = 0;
        last_class_filter_ = eCHAR_NONE;
        current_page_index_ = 1;
    }

    int ProductItemList::GetMaxPage()
    {
        if( product_array_.size() <= 10 )
        {
            return 1;
        }

        return (product_array_.size() -1) / 10 +1;
    }

    bool ProductItemList::IsValidProduct( ShopItemObject* pObject )
    {
        for( size_t i=0;i!=product_array_.size();++i)
        {
            if( product_array_[i] == pObject )
                return true;
        }
        return false;
    }

    void ProductItemList::_FillProductList(CShopCategory* pShopCategory )
    {
    #ifdef _YMS_TREECONTROL
        int category_seq = 0;
        pShopCategory->SetChildCategorySeqFirst();
        while (pShopCategory->GetChildCategorySeqNext(category_seq) == true)
        {
            CShopCategory* pShopSubCategory = NULL;
            if ((category_list()->GetValueByKey(category_seq, pShopSubCategory) == false) || 
                (pShopSubCategory == NULL))
            {
                continue;
            }

            _FillProductList(pShopSubCategory);
        }


        int package_seq = 0;
        pShopCategory->SetChildPackagSeqFirst();
        while (pShopCategory->GetChildPackagSeqNext(package_seq) == true)
        {
            CShopPackage* pShopPackage = NULL;
            if ((package_list()->GetValueByKey(package_seq, pShopPackage) == false) || 
                (pShopPackage == NULL))
            {
                continue;
            }


            if ( product_type_ == PRODUCT_TYPE_CASH )
            {
            }
            else if (product_type_ == PRODUCT_TYPE_PPCARD )
            {
                if (pShopPackage->CashType != 2)
                {
                    continue;
                }
            }
            else if ( product_type_ == PRODUCT_TYPE_MILEAGE )
            {
                if (pShopPackage->DeductMileageFlag == false)
                {
                    continue;
                }
            }

            // 이미지 테그가 있는데 같지 않다면 등록하지 않는다.
            if ( image_type_index_ > 0 && pShopPackage->ImageTagType != image_type_index_)
            {
                continue;
            }

            if( ProductItem* pProduct = ProductItem::CreateProduct(pShopCategory, pShopPackage, last_class_filter_) )
            {
                product_array_.push_back( pProduct );
            }
        }
    #endif //_YMS_TREECONTROL
    }

    void ProductItemList::BuildProductList(eProductType ProductType, int CategorySequence, eCHAR_TYPE ClassFilter, int ImageTypeIndex )
    {
        if( last_category_sequence_ == CategorySequence && last_class_filter_ == ClassFilter && product_type_ == ProductType )
        {
            return;
        }

        ClearProductArray();

        last_category_sequence_ = CategorySequence;
        last_class_filter_ = ClassFilter;
        product_type_ = ProductType;
        image_type_index_ = ImageTypeIndex;



        if( last_category_sequence_ > 0 )
        {
            CShopCategory* pShopCategory = NULL;
            if (category_list()->GetValueByKey(last_category_sequence_, OUT pShopCategory) )
            {
                _FillProductList( pShopCategory);
            }
        }

        current_page_index_ = 1;
    }

    void ProductItemList::SortProductList(Define::ShopStringCode sort_type)
    {
        //kShopStringCode_Filter_New_Inc = 2557,    //신상품 순
        //kShopStringCode_Filter_New_Desc = 2558,    //오래된 순
        //kShopStringCode_Filter_Hit_Desc = 2559, 	//인기 순
        //kShopStringCode_Filter_Price_Inc = 2560,    //가격 순
        //kShopStringCode_Filter_Price_Desc = 2561,    //가격 역순
        //kShopStringCode_Filter_Name_Inc = 2562,    //이름 순
        //kShopStringCode_Filter_Name_Desc = 2563,    //이름 역순
        //kShopStringCode_Filter_Date_Inc = 2564,    //날짜 순
        //kShopStringCode_Filter_Date_Desc = 2565,    //날짜 역순

        if (sort_type == Define::kShopStringCode_Filter_Date_Inc)
        {
            std::sort(product_array_.begin(), product_array_.end(), ProductItem::SortProductByDateInc);
        }
        else if (sort_type == Define::kShopStringCode_Filter_Date_Desc)
        {
            std::sort(product_array_.begin(), product_array_.end(), ProductItem::SortProductByDateDesc);
        }
        else if (sort_type == Define::kShopStringCode_Filter_Price_Inc)
        {
            std::sort(product_array_.begin(), product_array_.end(), ProductItem::SortProductByPriceInc);
        }
        else if (sort_type == Define::kShopStringCode_Filter_Price_Desc)
        {
            std::sort(product_array_.begin(), product_array_.end(), ProductItem::SortProductByPriceDesc);
        }
        else if (sort_type == Define::kShopStringCode_Filter_Name_Inc)
        {
            std::sort(product_array_.begin(), product_array_.end(), ProductItem::SortProductByNameInc);
        }
        else if (sort_type == Define::kShopStringCode_Filter_Name_Desc)
        {
            std::sort(product_array_.begin(), product_array_.end(), ProductItem::SortProductByNameDesc);
        }
    }
}