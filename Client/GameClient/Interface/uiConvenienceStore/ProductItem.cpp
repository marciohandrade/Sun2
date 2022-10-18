#include "SunClientPrecompiledHeader.h"
#include "ProductItem.h"

#include "ProductInfo.h"

namespace ConvenienceStore
{

    //------------------------------------------------------------------------------ 
    ProductItem::ProductItem()
    {
        product_item_type_ = Define::kShopProduct_Size;

        shop_category_ptr_ = NULL;
        shop_package_ptr_ = NULL;
        package_image_ptr_ = NULL;
        product_info_array_.clear();
    }
    //------------------------------------------------------------------------------ 
    ProductItem::ProductItem( ProductItem* form_ptr )
    {
        product_item_type_ = form_ptr->product_item_type_;

        shop_category_ptr_ = form_ptr->GetShopCategory();
        shop_package_ptr_ = form_ptr->GetShopPackage();
        package_image_ptr_ = ICONMGR()->Add(atoi(shop_package_ptr_->InGamePackageID));

        std::vector<ProductInfo>& InfoVector = form_ptr->GetInfoVector();
        for( size_t i=0;i!=InfoVector.size();++i)
        {
            product_info_array_.push_back( InfoVector[i] );
        }
    }
    //------------------------------------------------------------------------------ 
    ProductItem::ProductItem(CShopCategory* shop_category, CShopPackage* shop_package)
    {
        shop_category_ptr_ = shop_category;
        shop_package_ptr_ = shop_package;
        package_image_ptr_ = ICONMGR()->Add(atoi(shop_package_ptr_->InGamePackageID));
    }
    //------------------------------------------------------------------------------ 
    ProductItem::~ProductItem()
    {
        if (package_image_ptr_ != NULL)
        {
            ICONMGR()->RemoveIcon(package_image_ptr_);
            package_image_ptr_ = NULL;
        }

        std::vector<ProductInfo>::iterator begin = product_info_array_.begin();
        std::vector<ProductInfo>::iterator end = product_info_array_.end();
        for (; begin != end; ++begin)
        {
            if (IconImage* pIconImage = begin->GetIconImage() )
            {
                ICONMGR()->RemoveIcon(pIconImage);
                begin->SetIconImage( NULL );
            }
        }
        product_info_array_.clear();
    }
    //------------------------------------------------------------------------------ 
    ProductInfo* ProductItem::first_info()
    {
        if (static_cast<int>(product_info_array_.size()) > 0)
        {
            return &(product_info_array_[0]);
        }
        return NULL;
    }

    //------------------------------------------------------------------------------ 
    CASH ProductItem::GetPrice(int shop_price_seq)
    {
        if (is_lotto() || is_package()) 
        {
            return shop_package_ptr_->Price;
        }
        else
        {
            std::vector<ProductInfo>::iterator begin = product_info_array_.begin();
            std::vector<ProductInfo>::iterator end = product_info_array_.end();
            for (; begin != end; ++begin)
            {
                if (shop_price_seq == -1)
                {
                    return begin->GetPriceProperty().GetPrice();
                }
                else if (begin->GetPriceProperty().GetPriceSequence() == shop_price_seq )
                {
                    return begin->GetPriceProperty().GetPrice();
                }
            }
        }
        return 0;
    }
    //------------------------------------------------------------------------------ 
    SLOTCODE ProductItem::item_code(int index)
    {
        if (index >= 0 && index < static_cast<int>(product_info_array_.size()))
        {
            return (product_info_array_[index].GetItemInfo()->m_Code);
        }
        return 0;
    }
    //------------------------------------------------------------------------------ 
    bool ProductItem::is_package()
    {
        if (is_lotto() == false)
        {
            if (shop_package_ptr_ != NULL)
            {
                return (1 < shop_package_ptr_->GetProductCount());
            }
        }
        return false;
    }

    //------------------------------------------------------------------------------ 
    bool ProductItem::is_mileage_only()
    {
        if( shop_package_ptr_ != NULL )
        {
            return shop_package_ptr_->DeductMileageFlag == true;
        }

        return false;
    }

    //------------------------------------------------------------------------------ 
    bool ProductItem::is_cash_only()
    {
        if( shop_package_ptr_ != NULL )
        {
            return shop_package_ptr_->CashType == 508;
        }
        return false;
    }

    //------------------------------------------------------------------------------ 
    bool ProductItem::is_pcash_only()
    {
        if( shop_package_ptr_ != NULL )
        {
            return shop_package_ptr_->CashType == 509;
        }
        return false;
    }
    //------------------------------------------------------------------------------ 
    bool ProductItem::is_valid_cashtype( Define::CashType cash_type )
    {
        if (shop_package_ptr_ != NULL)
        {
            if( shop_package_ptr_->CashType == 0 )
                return true;
            else if( shop_package_ptr_->CashType == 508 && cash_type == Define::kCashType_Cash )
                return true;
            else if( shop_package_ptr_->CashType == 509 && cash_type == Define::kCashType_PPCard )
                return true;
        }

        return false;
    }

    //------------------------------------------------------------------------------ 
    bool ProductItem::is_lotto()
    {
        if (shop_package_ptr_ != NULL)
        {
            return (shop_package_ptr_->CapsuleFlag == Define::kCapsuleFlag_Lotto);
        }
        return false;
    }
    //------------------------------------------------------------------------------ 
    bool ProductItem::is_possible_sales()
    {
        return (shop_package_ptr_ && (shop_package_ptr_->SalesFlag == 182));
    }
    //------------------------------------------------------------------------------ 
    bool ProductItem::is_possible_gift()
    {
        //! 가격이 0인것들도 선물 안되게 함
        return (shop_package_ptr_ && (shop_package_ptr_->GiftFlag == 184) && (GetPrice() > 0));
    }
    //------------------------------------------------------------------------------ 
    bool ProductItem::has_preiview_rider_equip_parts()
    {
        if (is_lotto())
        {
            return false;
        }
        if (is_package())
        {
            std::vector<ProductInfo>::iterator begin = product_info_array_.begin();
            std::vector<ProductInfo>::iterator end = product_info_array_.end();
            for (; begin != end; ++begin)
            {
                if (begin->GetItemInfo()->IsRiderPartsItem())
                {
                    return true;
                }
            }
        }
        else
        {
            return first_info()->GetItemInfo()->IsRiderPartsItem();
        }
        return false;
    }
    //------------------------------------------------------------------------------ 
    bool ProductItem::has_preiview_player_equip_parts(OUT eCHAR_TYPE* char_class_type/* = NULL*/)
    {
        if (is_lotto())
        {
            return false;
        }
        if (is_package())
        {
            std::vector<ProductInfo>::iterator begin = product_info_array_.begin();
            std::vector<ProductInfo>::iterator end = product_info_array_.end();
            for (; begin != end; ++begin)
            {
                BASE_ITEMINFO* item_info = begin->GetItemInfo();

                if ((item_info->IsWeapon() == true) ||
                    (item_info->m_wType == eITEMTYPE_ARMOR) ||
                    (item_info->m_wType == eITEMTYPE_HELMET) ||
                    (item_info->m_wType == eITEMTYPE_PANTS) ||
                    (item_info->m_wType == eITEMTYPE_BOOTS) ||
                    (item_info->m_wType == eITEMTYPE_GLOVE) ||
                    (item_info->m_wType == eITEMTYPE_CHANGE_CHARACTER_FACE) ||
                    (item_info->m_wType == eITEMTYPE_CHANGE_CHARACTER_HAIR))
                {
                    if (char_class_type != NULL)
                    {
                        if (item_info->m_wEqClass1) *char_class_type = eCHAR_BERSERKER;
                        else if (item_info->m_wEqClass2) *char_class_type = eCHAR_DRAGON;
                        else if (item_info->m_wEqClass3) *char_class_type = eCHAR_SHADOW;
                        else if (item_info->m_wEqClass4) *char_class_type = eCHAR_VALKYRIE;
                        else if (item_info->m_wEqClass5) *char_class_type = eCHAR_MAGICIAN; 
                        else if (item_info->m_wEqClass6) *char_class_type = eCHAR_MYSTIC; //_NA_004965_20120613_NEW_CHARACTER_MYSTIC
                        else if (item_info->m_wEqClass7) *char_class_type = eCHAR_HELLROID; //_NA_000000_20130812_NEW_CHARACTER_HELLROID
                        else if (item_info->m_wEqClass8) *char_class_type = eCHAR_WITCHBLADE; //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
                    }
                    return true;
                }
            }
        }
        else
        {
            BASE_ITEMINFO* item_info = first_info()->GetItemInfo();

            if ((item_info->IsWeapon() == true) ||
                (item_info->m_wType == eITEMTYPE_ARMOR) ||
                (item_info->m_wType == eITEMTYPE_HELMET) ||
                (item_info->m_wType == eITEMTYPE_PANTS) ||
                (item_info->m_wType == eITEMTYPE_BOOTS) ||
                (item_info->m_wType == eITEMTYPE_GLOVE) ||
                (item_info->m_wType == eITEMTYPE_CHANGE_CHARACTER_FACE) ||
                (item_info->m_wType == eITEMTYPE_COSTUME) ||
                (item_info->m_wType == eITEMTYPE_CHANGE_CHARACTER_HAIR))
            {
                if (char_class_type != NULL)
                {
                    if (item_info->m_wEqClass1) *char_class_type = eCHAR_BERSERKER;
                    else if (item_info->m_wEqClass2) *char_class_type = eCHAR_DRAGON;
                    else if (item_info->m_wEqClass3) *char_class_type = eCHAR_SHADOW;
                    else if (item_info->m_wEqClass4) *char_class_type = eCHAR_VALKYRIE;
                    else if (item_info->m_wEqClass5) *char_class_type = eCHAR_MAGICIAN; 
                    else if (item_info->m_wEqClass6) *char_class_type = eCHAR_MYSTIC; //_NA_004965_20120613_NEW_CHARACTER_MYSTIC
                    else if (item_info->m_wEqClass7) *char_class_type = eCHAR_HELLROID; //_NA_000000_20130812_NEW_CHARACTER_HELLROID
                    else if (item_info->m_wEqClass8) *char_class_type = eCHAR_WITCHBLADE; //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
                }

                return true;
            }
        }
        return false;
    }

    //------------------------------------------------------------------------------ 
    bool ProductItem::SortProductByDateInc(ProductItem*& ref_left, ProductItem*& ref_right)
    {
        if (ref_left->GetStartDate().tm_year < ref_right->GetStartDate().tm_year) return true;
        if (ref_left->GetStartDate().tm_year > ref_right->GetStartDate().tm_year) return false;

        if (ref_left->GetStartDate().tm_mon < ref_right->GetStartDate().tm_mon) return true;
        if (ref_left->GetStartDate().tm_mon > ref_right->GetStartDate().tm_mon) return false;

        if (ref_left->GetStartDate().tm_mday < ref_right->GetStartDate().tm_mday) return true;
        if (ref_left->GetStartDate().tm_mday > ref_right->GetStartDate().tm_mday) return false;

        if (ref_left->GetStartDate().tm_hour < ref_right->GetStartDate().tm_hour) return true;
        if (ref_left->GetStartDate().tm_hour > ref_right->GetStartDate().tm_hour) return false;

        if (ref_left->GetStartDate().tm_min < ref_right->GetStartDate().tm_min) return true;
        if (ref_left->GetStartDate().tm_min > ref_right->GetStartDate().tm_min) return false;

        if (ref_left->GetStartDate().tm_sec < ref_right->GetStartDate().tm_sec) return true;
        if (ref_left->GetStartDate().tm_sec > ref_right->GetStartDate().tm_sec) return false;

        return false;
    }
    //------------------------------------------------------------------------------ 
    bool ProductItem::SortProductByDateDesc(ProductItem*& ref_left, ProductItem*& ref_right)
    { 
        if (ref_left->GetStartDate().tm_year < ref_right->GetStartDate().tm_year) return false;
        if (ref_left->GetStartDate().tm_year > ref_right->GetStartDate().tm_year) return true;

        if (ref_left->GetStartDate().tm_mon < ref_right->GetStartDate().tm_mon) return false;
        if (ref_left->GetStartDate().tm_mon > ref_right->GetStartDate().tm_mon) return true;

        if (ref_left->GetStartDate().tm_mday < ref_right->GetStartDate().tm_mday) return false;
        if (ref_left->GetStartDate().tm_mday > ref_right->GetStartDate().tm_mday) return true;

        if (ref_left->GetStartDate().tm_hour < ref_right->GetStartDate().tm_hour) return false;
        if (ref_left->GetStartDate().tm_hour > ref_right->GetStartDate().tm_hour) return true;

        if (ref_left->GetStartDate().tm_min < ref_right->GetStartDate().tm_min) return false;
        if (ref_left->GetStartDate().tm_min > ref_right->GetStartDate().tm_min) return true;

        if (ref_left->GetStartDate().tm_sec < ref_right->GetStartDate().tm_sec) return false;
        if (ref_left->GetStartDate().tm_sec > ref_right->GetStartDate().tm_sec) return true;

        return false;
    }
    //------------------------------------------------------------------------------ 
    bool ProductItem::SortProductByPriceInc(ProductItem*& ref_left, ProductItem*& ref_right)
    {
        return ref_left->GetPrice() < ref_right->GetPrice();
    }
    //------------------------------------------------------------------------------ 
    bool ProductItem::SortProductByPriceDesc(ProductItem*& ref_left, ProductItem*& ref_right)
    {
        return ref_left->GetPrice() > ref_right->GetPrice();
    }
    //------------------------------------------------------------------------------ 
    bool ProductItem::SortProductByNameInc(ProductItem*& ref_left, ProductItem*& ref_right)
    {
        return Strcmp(ref_left->GetName(), ref_right->GetName()) < 0;
    }
    //------------------------------------------------------------------------------ 
    bool ProductItem::SortProductByNameDesc(ProductItem*& ref_left, ProductItem*& ref_right)
    {
        return Strcmp(ref_right->GetName(), ref_left->GetName()) < 0;
    }


    //------------------------------------------------------------------------------ 
    ProductItem* ProductItem::CreateProduct( ProductItem* pProductItem )
    {
        return new ProductItem(pProductItem);
    }

    //------------------------------------------------------------------------------ 
    ProductItem* ProductItem::CreateProduct(CShopCategory* pShopCategory, CShopPackage* pShopPackage, eCHAR_TYPE char_type)
    {
        if (pShopCategory == NULL || pShopPackage == NULL)
        {
            return NULL;
        }

        //------------------------------------------------------------------------------ 
        if (1 < pShopPackage->GetProductCount())
        {
            pShopPackage->SetProductSeqFirst();

            //------------------------------------------------------------------------------ 
            ProductItem* pProductItem = new ProductItem(pShopCategory, pShopPackage);

            //            //------------------------------------------------------------------------------
            //            //! 임시
            //#ifdef _DEBUG
            //            if (CGeneralGameParam::Instance()->IsNet() == FALSE)
            //            {
            //                if (shop_package_ptr_->PackageProductSeq == 6080)
            //                {
            //                    //! 복권 상품으로 셋팅
            //                    shop_package_ptr_->CapsuleFlag = kCapsuleFlag_Lotto;
            //                }
            //            }
            //#endif //_DEBUG

            int shop_product_seq = 0;
            while (pShopPackage->GetProductSeqNext(shop_product_seq))
            {
                CShopProduct* shop_product = NULL;
                if (product_list()->GetValueByKey(shop_product_seq, shop_product) == false)
                {
                    continue;
                }

                ProductInfo product_info(shop_product);
                //! 패키지일때는 클래스 필터를 넣을건지 확인 필요(탈것 패키지등은 필터를 넣으면 안나온다.)
                if (FillProductInfo(shop_product, 0, /*eCHAR_NONE*/ char_type, product_info))
                {
                    pProductItem->GetInfoVector().push_back(product_info);
                }
            }
            if (pProductItem->GetInfoVector().size() <= 0)
            {
                SAFE_DELETE(pProductItem);
            }
            return pProductItem;
        }
        else
        {
            int shop_product_seq = 0;
            if (pShopPackage->GetProductSeqFirst(shop_product_seq) == false)
            {
                return NULL;
            }
            CShopProduct* pShopProduct = NULL;
            if (product_list()->GetValueByKey(shop_product_seq, OUT pShopProduct) == false)
            {
                return NULL;
            }

            //------------------------------------------------------------------------------ 
            ProductItem* product = new ProductItem(pShopCategory, pShopPackage);

            // 생성할떄 타입을 정해둔다.
            if (pShopProduct->ProductType == Define::kProductType[Define::kShopProduct_Waste])
                product->product_item_type_ = Define::kShopProduct_Waste;
            else if (pShopProduct->ProductType == Define::kProductType[Define::kShopProduct_Infinite]) 
                product->product_item_type_ = Define::kShopProduct_Infinite;
            else if (pShopProduct->ProductType == Define::kProductType[Define::kShopProduct_Date])
                product->product_item_type_ = Define::kShopProduct_Date;
            else
                product->product_item_type_ = Define::kShopProduct_Size;
            

            // 
            pShopPackage->SetPriceSeqFirst(); 
            int price_seq = 0;
            while (pShopPackage->GetPriceSeqNext(price_seq))
            {
                ProductInfo product_info(pShopProduct);
                if (FillProductInfo(pShopProduct, price_seq, char_type, product_info))
                {
                    product->GetInfoVector().push_back(product_info);
                }
            }

            if (product->GetInfoVector().size() <= 0)
            {
                SAFE_DELETE(product);
            }

            return product;
        }
        return NULL;
    }
}
