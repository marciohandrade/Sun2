#include "SunClientPrecompiledHeader.h"

#include "CashShopHeader.h"

#include <SLM/ShopList.h>
#include <SLM/ListManager.h>

#include "IconMgr.h"
#include "TreeMenuInfoParser.h"
#include "GlobalFunc.h"
#include "ItemInfoParser.h"
#include "uiConvenienceStore_Basket.h"

#include "ProductInfo.h"
#include "ProductItem.h"
#include "CashShop.h"

namespace ConvenienceStore
{


    //------------------------------------------------------------------------------ 
    bool GetDisplayPriceStringByProduct(TCHAR* msg, int str_len, ProductItem* product)
    {
        bool buy_mileage = product->is_mileage_only();

        //155549	%s%s / %s%s
        //155564	일
        //155565	개
        if (product->is_package() || product->is_lotto())
        {
            TCHAR str_unit[INTERFACE_STRING_LENGTH] = {NULL, };
            g_InterfaceManager.GetInterfaceString(155565, str_unit, INTERFACE_STRING_LENGTH);

            GlobalFunc::MakeMessageFromStringTable(msg, INTERFACE_STRING_LENGTH, 155549,
                _T("1"),
                str_unit,
                product->GetShopPackage()->Price,
                buy_mileage ? (TCHAR*)g_InterfaceManager.GetInterfaceString(155459) : (TCHAR*)g_InterfaceManager.GetInterfaceString(155304));

            return true;
        }
        else if (product->GetInfoVector().size() > 0)
        {
            std::vector<ProductInfo>::const_reference product_info = product->GetInfoVector().at(0);

            return GetDisplayPriceStringByProductInfo(msg, str_len, (ProductInfo*)&(product_info), buy_mileage);
        }

        return false;
    }
    //------------------------------------------------------------------------------ 
    bool GetDisplayPriceStringByProductInfo(TCHAR* msg, int str_len, ProductInfo* product_info, bool buy_mileage)
    {
        return GetDisplayPriceStringByProductLink(msg, str_len, product_info->product_type(), product_info->GetPriceProperty().GetProductLink(), buy_mileage);
    }
    //------------------------------------------------------------------------------ 
    bool GetDisplayPriceStringByProductLink(TCHAR* msg, int str_len, Define::ShopProductType shop_product_type, CShopProductLink* shop_product_link, bool buy_mileage)
    {
        TCHAR str_price[INTERFACE_STRING_LENGTH] = {NULL, };
        GlobalFunc::MakeMessage_SignMoney(str_price, INTERFACE_STRING_LENGTH, shop_product_link->Price);

        if (shop_product_type == Define::kShopProduct_Date)
        {
            TCHAR str_unit[INTERFACE_STRING_LENGTH] = {NULL, };
            g_InterfaceManager.GetInterfaceString(155564, str_unit, INTERFACE_STRING_LENGTH);

            long sec_value = atol(shop_product_link->Value);
            int day_value = sec_value / 60 / 60 / 24;
            GlobalFunc::MakeMessageFromStringTable(msg, INTERFACE_STRING_LENGTH, 155549,
                day_value,
                str_unit,
                str_price/*shop_product_link->Price*/,
                buy_mileage ? (TCHAR*)g_InterfaceManager.GetInterfaceString(155459) : (TCHAR*)g_InterfaceManager.GetInterfaceString(155304));
        }
        else //Define::kShopProduct_Waste, kShopProduct_Infinite
        {
            TCHAR str_unit[INTERFACE_STRING_LENGTH] = {NULL, };
            g_InterfaceManager.GetInterfaceString(155565, str_unit, INTERFACE_STRING_LENGTH);

            GlobalFunc::MakeMessageFromStringTable(msg, INTERFACE_STRING_LENGTH, 155549,
                shop_product_link->Value,
                str_unit,
                str_price/*shop_product_link->Price*/,
                buy_mileage ? (TCHAR*)g_InterfaceManager.GetInterfaceString(155459) : (TCHAR*)g_InterfaceManager.GetInterfaceString(155304));
        }
        return true;
    }


    //------------------------------------------------------------------------------ 
    Define::ShopProductType GetProductType(IN CShopProduct* const shop_product)
    {
        if (shop_product->ProductType == Define::kProductType[Define::kShopProduct_Waste])    return Define::kShopProduct_Waste;
        if (shop_product->ProductType == Define::kProductType[Define::kShopProduct_Infinite]) return Define::kShopProduct_Infinite;
        if (shop_product->ProductType == Define::kProductType[Define::kShopProduct_Date])     return Define::kShopProduct_Date;

        return Define::kShopProduct_Size;
    }


    //------------------------------------------------------------------------------ 
    CShopCategoryList* category_list()
    {
        if( CShopListManager* shop_list_manager = CashShop::instance()->GetShopListManager() )
        {
            return shop_list_manager->GetListPtr()->GetCategoryListPtr();
        }
        return NULL;
    }
    //------------------------------------------------------------------------------ 
    CShopPackageList* package_list()
    {
        if( CShopListManager* shop_list_manager = CashShop::instance()->GetShopListManager() )
        {
            return shop_list_manager->GetListPtr()->GetPackageListPtr();
        }
        return NULL;
    }
    //------------------------------------------------------------------------------ 
    CShopProductList* product_list()
    {
        if( CShopListManager* shop_list_manager = CashShop::instance()->GetShopListManager() )
        {
            return shop_list_manager->GetListPtr()->GetProductListPtr();
        }
        return NULL;
    }
    //------------------------------------------------------------------------------ 
    CShopPropertyList* property_list()
    {
        if( CShopListManager* shop_list_manager = CashShop::instance()->GetShopListManager() )
        {
            return shop_list_manager->GetListPtr()->GetPropertyListPtr();
        }
        return NULL;
    }
    //------------------------------------------------------------------------------ 
    CShopProductLinkList* productlink_list()
    {
        if( CShopListManager* shop_list_manager = CashShop::instance()->GetShopListManager() )
        {
            return shop_list_manager->GetListPtr()->GetProductLinkListPtr();
        }
        return NULL;
    }
    //------------------------------------------------------------------------------ 
    Define::CashType GetCashType(BYTE basket_type /*uiConvenienceStore_Basket::BasketType*/)
    {
        return Define::kCashType_Cash;
    }
    //------------------------------------------------------------------------------ 
    bool FindShopProduct(CShopPackage* shop_package, int price_seq, 
        OUT CShopProduct*& shop_product, 
        OUT CShopProductLink*& shop_product_link)
    {
        //! 단일 상품만 가능하다.
        if (1 < shop_package->GetProductCount())
        {
            return false;
        }

        int shop_product_seq = 0;
        if (shop_package->GetProductSeqFirst(shop_product_seq) == false)
        {
            return false;
        }
        if (ConvenienceStore::product_list()->GetValueByKey(shop_product_seq, OUT shop_product) == false)
        {
            return false;
        }
        productlink_list()->SetProductFirst(shop_product->ProductSeq, price_seq);
        if (productlink_list()->GetProductNext(shop_product_link) == false)
        {
            return false;
        }

        return true;
    }
    //------------------------------------------------------------------------------ 
    void GetImageTag(CShopCategory* shop_category, std::set<int>& image_tag_vector)
    {
        image_tag_vector.clear();

        int shop_package_seq = 0;
        shop_category->SetAllChildPackageSeqFirst();
        while (shop_category->GetAllChildPackageSeqNext(shop_package_seq) == true)
        {
            CShopPackage* shop_package = NULL;
            if (package_list()->GetValueByKey(shop_package_seq, OUT shop_package) == true)
            {
                if (shop_package->ImageTagType > 0)
                {
                    image_tag_vector.insert(shop_package->ImageTagType);
                }
            }
        }
    }
    //------------------------------------------------------------------------------
    void test_shop_script_list()
    {
        CShopList* Info = CashShop::instance()->GetShopListManager()->GetListPtr();

        CShopCategoryList*		pCategoryList		= Info->GetCategoryListPtr();
        CShopPackageList*		pPackageList		= Info->GetPackageListPtr();
        CShopProductLinkList*	pProductLinkList	= Info->GetProductLinkListPtr();
        CShopProductList*		pProductList		= Info->GetProductListPtr();
        CShopPropertyList*		pPropertyList		= Info->GetPropertyListPtr();

        if( FILE* fp = fopen("./out_shop_script.txt", "wt") )
        {
            fprintf(fp, _T("Category Count: %d\n"), pCategoryList->GetSize());
            fprintf(fp, _T("Package  Count: %d\n"), pPackageList->GetSize());
            fprintf(fp, _T("Link     Count: %d\n"), pProductLinkList->GetSize());
            fprintf(fp, _T("ProductItem  Count: %d\n"), pProductList->GetSize());
            fprintf(fp, _T("Property Count: %d\n"), pPropertyList->GetSize());
            fprintf(fp, _T("\n"));

            pCategoryList->SetFirst();

            CShopCategory category;
            while (pCategoryList->GetNext(category))
            {
                if(0 == category.Root) continue; 
                fprintf(fp, _T("CategorySeq: %d, CategroyName: %s\n"), category.ProductDisplaySeq, category.CategroyName);

                test_shop_script_list_category(fp, category, 0);

            }

            fclose(fp);
        }
    }
    //------------------------------------------------------------------------------ 
    void test_shop_script_list_category(FILE* fp, CShopCategory &category, int deapth)
    {
        CShopList* Info = CashShop::instance()->GetShopListManager()->GetListPtr();

        CShopCategoryList*		pCategoryList		= Info->GetCategoryListPtr();
        CShopPackageList*		pPackageList		= Info->GetPackageListPtr();
        CShopProductLinkList*	pProductLinkList	= Info->GetProductLinkListPtr();
        CShopProductList*		pProductList		= Info->GetProductListPtr();
        CShopPropertyList*		pPropertyList		= Info->GetPropertyListPtr();

        category.SetChildCategorySeqFirst();

        int nCategorySeq = 0;
        while(category.GetChildCategorySeqNext(nCategorySeq))
        {
            CShopCategory subcategory;
            if (pCategoryList->GetValueByKey(nCategorySeq, subcategory) == true)
            {
                TCHAR temp[256] = {NULL, };
                _tcscat(temp, _T("\t"));
                for (int i = 0; i < deapth; ++i)
                {
                    _tcscat(temp, _T("\t"));
                }
                fprintf(fp, _T("%s====================================================================================\n"), temp);
                fprintf(fp, _T("%sSub CategorySeq: %d, CategroyName: %s\n"), temp, subcategory.ProductDisplaySeq, subcategory.CategroyName);

                test_shop_script_list_category(fp, subcategory, deapth + 1);
            }
        }

        test_shop_script_list_package(fp, category, deapth);
    }
    //------------------------------------------------------------------------------ 
    void test_shop_script_list_package(FILE* fp, CShopCategory &subcategory, int deapth)
    {
        CShopList* Info = CashShop::instance()->GetShopListManager()->GetListPtr();

        CShopCategoryList*		pCategoryList		= Info->GetCategoryListPtr();
        CShopPackageList*		pPackageList		= Info->GetPackageListPtr();
        CShopProductLinkList*	pProductLinkList	= Info->GetProductLinkListPtr();
        CShopProductList*		pProductList		= Info->GetProductListPtr();
        CShopPropertyList*		pPropertyList		= Info->GetPropertyListPtr();

        subcategory.SetChildPackagSeqFirst();

        TCHAR temp[256] = {NULL, };
        _tcscat(temp, _T("\t"));
        for (int i = 0; i < deapth; ++i)
        {
            _tcscat(temp, _T("\t"));
        }

        int nPackage = 0;
        while(subcategory.GetChildPackagSeqNext(nPackage))
        {
            fprintf(fp, _T("%scategory_no %d ------------------------------------------------------------------------------------\n"), 
                temp, subcategory.ProductDisplaySeq);
            CShopPackage package;
            pPackageList->GetValueByKey(nPackage, package);
            fprintf(fp, _T("%sPackageSeq: %d, PackageName: %s, InGamePackageID:%s, ProductDisplaySeq:%d, price_property:%d, CapsuleFlag:%d, CapsuleCount:%d\n"), 
                temp, 
                package.PackageProductSeq, 
                package.PackageProductName, 
                package.InGamePackageID, 
                package.ProductDisplaySeq,
                package.Price,
                package.CapsuleFlag,
                package.CapsuleCount);

            if(1 < package.GetProductCount())
            {
                fprintf(fp, _T("%s\t[ ProductItem Count : %d ]\n"), temp, package.GetProductCount());

                package.SetProductSeqFirst();

                int productSeq = 0;
                while(package.GetProductSeqNext(productSeq))
                {
                    CShopProduct product;
                    pProductList->GetValueByKey(productSeq, product);

                    fprintf(fp, _T("%s\tProductSeq: %d, ProductName: %s, ProductType: %d\n"), 
                        temp, product.ProductSeq, product.ProductName,
                        product.ProductType);

                    pProductLinkList->SetProductFirst(productSeq);

                    CShopProductLink link;
                    while(pProductLinkList->GetProductNext(link))
                    {
                        CShopProperty Property;
                        pPropertyList->GetValueByKey(link.PropertySeq, Property);

                        fprintf(fp, _T("%s\t\tInGameProductID : %d, PriceSeq: %d, Price: %d, PropertySeq: %d, Value: %s, PropertyName: %s, UnitName: %s, PropertyType:%d\n"),
                            temp, link.InGameProductID, link.PriceSeq, link.Price, link.PropertySeq, link.Value, 
                            Property.PropertyName, Property.UnitName, Property.PropertyType);
                    }
                }
            }
            else
            {
                fprintf(fp, _T("%s\t[ Price Count : %d ]\n"), temp, package.GetPriceCount());



                int product_count = package.GetProductCount();
                package.SetProductSeqFirst();
                int productSeq = 0;
                while (package.GetProductSeqNext(productSeq)); 
                {
                    CShopProduct product;
                    pProductList->GetValueByKey(productSeq, product);

                    package.SetPriceSeqFirst();
                    int PriceSeq = 0;
                    while(package.GetPriceSeqNext(PriceSeq))
                    {
                        fprintf(fp, _T("%s\tProductSeq: %d, PriceSeq: %d, ProductName: %s, ProductType: %d\n"), 
                            temp, product.ProductSeq, PriceSeq, product.ProductName,
                            product.ProductType);

                        pProductLinkList->SetProductFirst(productSeq, PriceSeq);

                        CShopProductLink link;
                        while(pProductLinkList->GetProductNext(link))
                        {
                            CShopProperty Property;
                            pPropertyList->GetValueByKey(link.PropertySeq, Property);

                            fprintf(fp, _T("%s\t\tInGameProductID : %d, PriceSeq: %d, Price: %d, PropertySeq: %d, Value: %s, PropertyName: %s, UnitName: %s, PropertyType:%d\n"),
                                temp, link.InGameProductID, link.PriceSeq, link.Price, link.PropertySeq, link.Value, 
                                Property.PropertyName, Property.UnitName, Property.PropertyType);
                        }
                    }
                }
            }
        } //while(subcategory.GetChildPackagSeqNext(nPackage))
    }

};