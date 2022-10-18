#include "SunClientPrecompiledHeader.h"

#include "uiConvenienceStore_Counter.h"
#include "TreeMenuInfoParser.h"
#include "TextureListInfoHandleManager.h"
#include "InterfaceManager.h"
#include "uiConvenienceStoreMan.h"
#include "uiConvenienceStore_Basket.h"
#include "uiConvenienceStore_PopupMenuFive.h"
#include "uiConvenienceStore_PopupMenuFour.h"
#include "uiConvenienceStore_PopupMenuTwo.h"
#include "uiConvenienceStore_PopupPackage.h"
#include "GlobalFunc.h"
#include "Mouse.h"
#include "SolarDialog.h"
#include "ItemInfoParser.h"
#include "uiConvenienceStore_Purchase.h"
#include "uiConvenienceStore_Gift.h"
#include "uiConvenienceStore_ItemDetail.h"
#include "uiConvenienceStore_PopupMenu.h"
#include "uiTargetMan\uiTargetMan.h"
#include "uiRadarMan\uiRadarMan.h"
#include "uiSkillMan\uiNewSkillMan.h"
#include "Hero.h"
#include "uiConvenienceStore_Ask.h"
#include "uiConvenienceStore_Confirm.h"
#include "uiConvenienceStore_PopupMenuThree.h"
#include "uiToolTipMan\uiToolTipMan.h"
#include "StorageItem.h"
#include "ProductItem.h"


//------------------------------------------------------------------------------ 
const DWORD uiConvenienceStore_Counter::kDialog_ID = StrToWzID("0472");;

//------------------------------------------------------------------------------ 
//! static function
void uiConvenienceStore_Counter::Callback_IconRender(CControlWZ* control_ptr, void* parameter)
{
    if( uiConvenienceStore_Counter* onwer = static_cast<uiConvenienceStore_Counter*>(parameter) )
    {
        if( ConvenienceStore::ShopItemObject* pObject = reinterpret_cast<ConvenienceStore::ShopItemObject*>(control_ptr->GetUserData()) )
        {
            ConvenienceStore::ProductInfo* product_info = NULL;

            if( pObject->IsStorageItem() )
            {
                if( IconImage* pIconImage = pObject->GetIconImage() )
                {
                    pIconImage->Render( &(control_ptr->GetSizeRect() ) );
                }

                ConvenienceStore::StorageItem* StorageItemData = static_cast<ConvenienceStore::StorageItem*>(pObject);
                product_info = &(StorageItemData->GetProductInfo());
            }
            else
            {
                if( ConvenienceStore::CashShop::instance()->GetProductItemData().IsValidProduct( pObject ) == false )
                    return;

                if ( IconImage* pIconImage = pObject->GetIconImage() )
                {
                    pIconImage->Render(&(control_ptr->GetSizeRect() ) );
                }

                ConvenienceStore::ProductItem* pProduct = static_cast<ConvenienceStore::ProductItem*>(pObject);
                if (pProduct->is_lotto() == false && pProduct->is_package() == false)
                {
                    product_info = pProduct->first_info();
                }
            }

            //! 커버 이미지
            if ((product_info != NULL) && (product_info->GetItemInfo() != NULL))
            {
                RECT area = control_ptr->GetSizeRect();
                if (product_info->is_skin())
                {
                    product_info->GetIconImage()->RenderCoverImage(&area, WzColor_RGBA(0, 0, 0, 175));    
                }
                if (product_info->is_etheria() || product_info->is_etheria_with_divine())
                {
                    if (product_info->GetItemInfo()->m_wType == eITEMTYPE_PET)
                    {
                        g_InterfaceManager.pet_etheria_icon()->Render(&area);
                    }
                    else
                    {
                        g_InterfaceManager.etheria_icon()->Render(&area);
                    }
                }
            }
        }
    }
}

//------------------------------------------------------------------------------ 
uiConvenienceStore_Counter::uiConvenienceStore_Counter(InterfaceManager* ui_man) : 
    uiConvenienceStore_Dialog(ui_man)
{

    RegisterControlPosition(StrToWzID("C001"), kControl_CheckBT_C001);
    RegisterControlPosition(StrToWzID("C020"), kControl_CheckBT_C020);
    RegisterControlPosition(StrToWzID("C002"), kControl_CheckBT_C002);
    RegisterControlPosition(StrToWzID("C021"), kControl_CheckBT_C021);
    RegisterControlPosition(StrToWzID("CT01"), kControl_CheckBT_CT01);
    RegisterControlPosition(StrToWzID("CT02"), kControl_CheckBT_CT02);
    RegisterControlPosition(StrToWzID("C033"), kControl_CheckBT_C033);
    RegisterControlPosition(StrToWzID("C034"), kControl_CheckBT_C034);

    RegisterControlPosition(StrToWzID("BT12"), kControl_Button_BT12);
    RegisterControlPosition(StrToWzID("BT13"), kControl_Button_BT13);
    RegisterControlPosition(StrToWzID("B004"), kControl_Button_B004);
    RegisterControlPosition(StrToWzID("B000"), kControl_Button_B000);
    RegisterControlPosition(StrToWzID("B006"), kControl_Button_B006);
    RegisterControlPosition(StrToWzID("B042"), kControl_Button_B042);
    RegisterControlPosition(StrToWzID("B043"), kControl_Button_B043);
    RegisterControlPosition(StrToWzID("B044"), kControl_Button_B044);
    RegisterControlPosition(StrToWzID("B045"), kControl_Button_B045);
    RegisterControlPosition(StrToWzID("B046"), kControl_Button_B046);
    RegisterControlPosition(StrToWzID("B047"), kControl_Button_B047);
    RegisterControlPosition(StrToWzID("B048"), kControl_Button_B048);
    RegisterControlPosition(StrToWzID("B049"), kControl_Button_B049);
    RegisterControlPosition(StrToWzID("B050"), kControl_Button_B050);
    RegisterControlPosition(StrToWzID("ST01"), kControl_Text_ST01);
#ifdef _NA_20111118_CASHSHOP_UI_RENEWAL
    RegisterControlPosition(StrToWzID("B008"), kControl_Button_B008);
#else
    RegisterControlPosition(StrToWzID("S000"), kControl_Text_S000);
#endif //_NA_20111118_CASHSHOP_UI_RENEWAL
    RegisterControlPosition(StrToWzID("S007"), kControl_Text_S007);
    RegisterControlPosition(StrToWzID("S008"), kControl_Text_S008);
    RegisterControlPosition(StrToWzID("S009"), kControl_Text_S009);
    RegisterControlPosition(StrToWzID("S010"), kControl_Text_S010);
    RegisterControlPosition(StrToWzID("S011"), kControl_Text_S011);
    RegisterControlPosition(StrToWzID("S012"), kControl_Text_S012);
    RegisterControlPosition(StrToWzID("S013"), kControl_Text_S013);
    RegisterControlPosition(StrToWzID("S014"), kControl_Text_S014);
    RegisterControlPosition(StrToWzID("S015"), kControl_Text_S015);
    RegisterControlPosition(StrToWzID("S016"), kControl_Text_S016);
    RegisterControlPosition(StrToWzID("S017"), kControl_Text_S017);
    RegisterControlPosition(StrToWzID("S018"), kControl_Text_S018);
    RegisterControlPosition(StrToWzID("S019"), kControl_Text_S019);
    RegisterControlPosition(StrToWzID("S020"), kControl_Text_S020);
    RegisterControlPosition(StrToWzID("S021"), kControl_Text_S021);
    RegisterControlPosition(StrToWzID("S022"), kControl_Text_S022);
    RegisterControlPosition(StrToWzID("S023"), kControl_Text_S023);
    RegisterControlPosition(StrToWzID("S024"), kControl_Text_S024);
    RegisterControlPosition(StrToWzID("S025"), kControl_Text_S025);
    RegisterControlPosition(StrToWzID("S026"), kControl_Text_S026);
    RegisterControlPosition(StrToWzID("S027"), kControl_Text_S027);
    RegisterControlPosition(StrToWzID("S028"), kControl_Text_S028);
    RegisterControlPosition(StrToWzID("S029"), kControl_Text_S029);
    RegisterControlPosition(StrToWzID("S030"), kControl_Text_S030);
    RegisterControlPosition(StrToWzID("S031"), kControl_Text_S031);
    RegisterControlPosition(StrToWzID("S032"), kControl_Text_S032);
    RegisterControlPosition(StrToWzID("S033"), kControl_Text_S033);
    RegisterControlPosition(StrToWzID("S034"), kControl_Text_S034);
    RegisterControlPosition(StrToWzID("S035"), kControl_Text_S035);
    RegisterControlPosition(StrToWzID("S036"), kControl_Text_S036);
    RegisterControlPosition(StrToWzID("S037"), kControl_Text_S037);
    RegisterControlPosition(StrToWzID("S038"), kControl_Text_S038);
    RegisterControlPosition(StrToWzID("S039"), kControl_Text_S039);
    RegisterControlPosition(StrToWzID("S040"), kControl_Text_S040);
    RegisterControlPosition(StrToWzID("S041"), kControl_Text_S041);
    RegisterControlPosition(StrToWzID("S042"), kControl_Text_S042);
    RegisterControlPosition(StrToWzID("S043"), kControl_Text_S043);
    RegisterControlPosition(StrToWzID("S044"), kControl_Text_S044);
    RegisterControlPosition(StrToWzID("S045"), kControl_Text_S045);
    RegisterControlPosition(StrToWzID("S046"), kControl_Text_S046);
    RegisterControlPosition(StrToWzID("S047"), kControl_Text_S047);
    RegisterControlPosition(StrToWzID("S048"), kControl_Text_S048);
    RegisterControlPosition(StrToWzID("S049"), kControl_Text_S049);
    RegisterControlPosition(StrToWzID("S050"), kControl_Text_S050);
    RegisterControlPosition(StrToWzID("S051"), kControl_Text_S051);
    RegisterControlPosition(StrToWzID("S052"), kControl_Text_S052);
    RegisterControlPosition(StrToWzID("S053"), kControl_Text_S053);
    RegisterControlPosition(StrToWzID("S054"), kControl_Text_S054);
    RegisterControlPosition(StrToWzID("S055"), kControl_Text_S055);
    RegisterControlPosition(StrToWzID("S056"), kControl_Text_S056);
    //RegisterControlPosition(StrToWzID("S001"), kControl_Text_S001);
    //RegisterControlPosition(StrToWzID("S002"), kControl_Text_S002);
    //RegisterControlPosition(StrToWzID("S003"), kControl_Text_S003);
    //RegisterControlPosition(StrToWzID("S004"), kControl_Text_S004);
    //RegisterControlPosition(StrToWzID("S005"), kControl_Text_S005);
    //RegisterControlPosition(StrToWzID("S006"), kControl_Text_S006);
    //RegisterControlPosition(StrToWzID("S057"), kControl_Text_S057);
    //RegisterControlPosition(StrToWzID("S058"), kControl_Text_S058);
    //RegisterControlPosition(StrToWzID("S059"), kControl_Text_S059);
    //RegisterControlPosition(StrToWzID("S060"), kControl_Text_S060);

#ifdef _NA_20111118_CASHSHOP_UI_RENEWAL
    RegisterControlPosition(StrToWzID("B007"), kControl_Button_B007);
#else
    RegisterControlPosition(StrToWzID("S061"), kControl_Text_S061);
#endif //_NA_20111118_CASHSHOP_UI_RENEWAL

    RegisterControlPosition(StrToWzID("L000"), kControl_List_L000);
    RegisterControlPosition(StrToWzID("L001"), kControl_List_L001);
    RegisterControlPosition(StrToWzID("L002"), kControl_List_L002);
    RegisterControlPosition(StrToWzID("P001"), kControl_Picture_P001);
    RegisterControlPosition(StrToWzID("P002"), kControl_Picture_P002);
    RegisterControlPosition(StrToWzID("P003"), kControl_Picture_P003);
    RegisterControlPosition(StrToWzID("PI00"), kControl_Picture_PI00);
    RegisterControlPosition(StrToWzID("P004"), kControl_Picture_P004);
    RegisterControlPosition(StrToWzID("P000"), kControl_Picture_P000);
    RegisterControlPosition(StrToWzID("P005"), kControl_Picture_P005);
    RegisterControlPosition(StrToWzID("P006"), kControl_Picture_P006);
    RegisterControlPosition(StrToWzID("P007"), kControl_Picture_P007);
    RegisterControlPosition(StrToWzID("P008"), kControl_Picture_P008);
    RegisterControlPosition(StrToWzID("P009"), kControl_Picture_P009);
    RegisterControlPosition(StrToWzID("P010"), kControl_Picture_P010);
    RegisterControlPosition(StrToWzID("P011"), kControl_Picture_P011);
    RegisterControlPosition(StrToWzID("P012"), kControl_Picture_P012);
    RegisterControlPosition(StrToWzID("P013"), kControl_Picture_P013);
    RegisterControlPosition(StrToWzID("P014"), kControl_Picture_P014);
    RegisterControlPosition(StrToWzID("P015"), kControl_Picture_P015);
    RegisterControlPosition(StrToWzID("P016"), kControl_Picture_P016);
    RegisterControlPosition(StrToWzID("P017"), kControl_Picture_P017);
    RegisterControlPosition(StrToWzID("P018"), kControl_Picture_P018);
    RegisterControlPosition(StrToWzID("P019"), kControl_Picture_P019);
    RegisterControlPosition(StrToWzID("P020"), kControl_Picture_P020);
    RegisterControlPosition(StrToWzID("P021"), kControl_Picture_P021);
    RegisterControlPosition(StrToWzID("P022"), kControl_Picture_P022);
    RegisterControlPosition(StrToWzID("P023"), kControl_Picture_P023);
    RegisterControlPosition(StrToWzID("P024"), kControl_Picture_P024);
    RegisterControlPosition(StrToWzID("B001"), kControl_TextImage_B001);
    RegisterControlPosition(StrToWzID("I001"), kControl_TextImage_I001);
    RegisterControlPosition(StrToWzID("I002"), kControl_TextImage_I002);
    RegisterControlPosition(StrToWzID("I003"), kControl_TextImage_I003);
    RegisterControlPosition(StrToWzID("I004"), kControl_TextImage_I004);
    RegisterControlPosition(StrToWzID("I005"), kControl_TextImage_I005);
    RegisterControlPosition(StrToWzID("I006"), kControl_TextImage_I006);
    RegisterControlPosition(StrToWzID("I007"), kControl_TextImage_I007);
    RegisterControlPosition(StrToWzID("I008"), kControl_TextImage_I008);
    RegisterControlPosition(StrToWzID("I009"), kControl_TextImage_I009);
    RegisterControlPosition(StrToWzID("B002"), KControl_button_B002);

#ifdef _YMS_TREECONTROL
    tree_menu().set_extend_always(true);
#endif //_YMS_TREECONTROL
    tree_menu().Release();

    ClearProductList();
}

//------------------------------------------------------------------------------ 
uiConvenienceStore_Counter::~uiConvenienceStore_Counter( void )
{
    Release();
}
//------------------------------------------------------------------------------ 
void uiConvenienceStore_Counter::Init(CDrawBase* drawbase)
{
    GetDialogWZ()->SetCallbackTooltip(uiToolTipMan::Callback_Tooltip);

    //------------------------------------------------------------------------------ 
    //! tree menu
#ifdef _YMS_TREECONTROL
    tree_menu().Init(BASE_TreeMenuInfo::kGroupType_CShop_Cash, 
                    GetListControlWZ(GetControlID_byPosition(kControl_List_L000)), 
                    NULL, 
                    true, 
                    NULL);
    tree_menu().set_menu_height_gab(20);
#endif //#ifdef _YMS_TREECONTROL

    //------------------------------------------------------------------------------ 
    bool is_result = true;
    if (is_result)
    {
#ifdef _NA_20111118_CASHSHOP_UI_RENEWAL
        // 필터 콤보 박스
        is_result = dropdown_class_filter().Init(GetControlWZ(GetControlID_byPosition(kControl_Button_B007)), 
                                                    GetButtonControlWZ(GetControlID_byPosition(kControl_Button_B004)), 
                                                    GetListControlWZ(GetControlID_byPosition(kControl_List_L002)),
                                                    NULL,
                                                    GetPictureControlWZ(GetControlID_byPosition(kControl_Picture_P004)));
#else
        is_result = dropdown_class_filter().Init(GetControlWZ(GetControlID_byPosition(kControl_Text_S061)), 
                                                    GetButtonControlWZ(GetControlID_byPosition(kControl_Button_B000)), 
                                                    GetListControlWZ(GetControlID_byPosition(kControl_List_L002)),
                                                    NULL,
                                                    GetPictureControlWZ(GetControlID_byPosition(kControl_Picture_P004)));

#endif //_NA_20111118_CASHSHOP_UI_RENEWAL

        if (dropdown_class_filter().list_control())
        {
            dropdown_class_filter().list_control()->SetUnuseSelBar(true);
            dropdown_class_filter().list_control()->DeleteItemAll();
            dropdown_class_filter().list_control()->SetStartLineGapTop(8);
            dropdown_class_filter().list_control()->SetSellHeight(16);
        }

        dropdown_class_filter().InsertItem(ConvenienceStore::Define::kShopStringCode_Filter_ALL,       (void*)eCHAR_NONE, StrToWzID("st10"), DT_VCENTER | DT_CENTER);
        dropdown_class_filter().InsertItem(ConvenienceStore::Define::kShopStringCode_Filter_Berserker, (void*)eCHAR_BERSERKER, StrToWzID("st10"), DT_VCENTER | DT_CENTER);
        dropdown_class_filter().InsertItem(ConvenienceStore::Define::kShopStringCode_Filter_Dragon,    (void*)eCHAR_DRAGON, StrToWzID("st10"), DT_VCENTER | DT_CENTER);
        dropdown_class_filter().InsertItem(ConvenienceStore::Define::kShopStringCode_Filter_Shadow,    (void*)eCHAR_SHADOW, StrToWzID("st10"), DT_VCENTER | DT_CENTER);
        dropdown_class_filter().InsertItem(ConvenienceStore::Define::kShopStringCode_Filter_Valkyrie,  (void*)eCHAR_VALKYRIE, StrToWzID("st10"), DT_VCENTER | DT_CENTER);
        dropdown_class_filter().InsertItem(ConvenienceStore::Define::kShopStringCode_Filter_Magician,  (void*)eCHAR_MAGICIAN, StrToWzID("st10"), DT_VCENTER | DT_CENTER);
        dropdown_class_filter().InsertItem(ConvenienceStore::Define::kShopStringCode_Filter_Mystic,  (void*)eCHAR_MYSTIC, StrToWzID("st10"), DT_VCENTER | DT_CENTER); //_NA_004965_20120613_NEW_CHARACTER_MYSTIC
#ifdef _NA_000000_20130812_NEW_CHARACTER_HELLROID
        dropdown_class_filter().InsertItem(ConvenienceStore::Define::kShopStringCode_Filter_Hellroid,  (void*)eCHAR_HELLROID, StrToWzID("st10"), DT_VCENTER | DT_CENTER);
#endif //_NA_000000_20130812_NEW_CHARACTER_HELLROID
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
        dropdown_class_filter().InsertItem(ConvenienceStore::Define::kShopStringCode_Filter_WitchBladae,  (void*)eCHAR_WITCHBLADE, StrToWzID("st10"), DT_VCENTER | DT_CENTER);
#endif //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
        dropdown_class_filter().ResetVScroll();
        dropdown_class_filter().Select(0);
    }
    if (is_result)
    {
#ifdef _NA_20111118_CASHSHOP_UI_RENEWAL

        // 소팅 콤보 박스
        is_result = dropdown_sorting().Init(GetControlWZ(GetControlID_byPosition(kControl_Button_B008)), 
                                            GetButtonControlWZ(GetControlID_byPosition(kControl_Button_B000)), 
                                            GetListControlWZ(GetControlID_byPosition(kControl_List_L001)),
                                            NULL,
                                            GetPictureControlWZ(GetControlID_byPosition(kControl_Picture_P000)));
#else
        is_result = dropdown_sorting().Init(GetControlWZ(GetControlID_byPosition(kControl_Text_S000)), 
                                            GetButtonControlWZ(GetControlID_byPosition(kControl_Button_B004)), 
                                            GetListControlWZ(GetControlID_byPosition(kControl_List_L001)),
                                            NULL,
                                            GetPictureControlWZ(GetControlID_byPosition(kControl_Picture_P000)));

#endif //_NA_20111118_CASHSHOP_UI_RENEWAL

        if (dropdown_sorting().list_control())
        {
            dropdown_sorting().list_control()->SetUnuseSelBar(true);
            dropdown_sorting().list_control()->DeleteItemAll();
            dropdown_sorting().list_control()->SetStartLineGapTop(8);
            dropdown_sorting().list_control()->SetSellHeight(16);
        }

        dropdown_sorting().InsertItem(ConvenienceStore::Define::kShopStringCode_Filter_Date_Inc,   
                                    (void*)ConvenienceStore::Define::kShopStringCode_Filter_Date_Desc, 
                                    StrToWzID("st10"), DT_VCENTER | DT_CENTER);
        dropdown_sorting().InsertItem(ConvenienceStore::Define::kShopStringCode_Filter_Date_Desc,   
                                    (void*)ConvenienceStore::Define::kShopStringCode_Filter_Date_Inc, 
                                    StrToWzID("st10"), DT_VCENTER | DT_CENTER);
        dropdown_sorting().InsertItem(ConvenienceStore::Define::kShopStringCode_Filter_Price_Desc,  
                                    (void*)ConvenienceStore::Define::kShopStringCode_Filter_Price_Desc, 
                                    StrToWzID("st10"), DT_VCENTER | DT_CENTER);
        dropdown_sorting().InsertItem(ConvenienceStore::Define::kShopStringCode_Filter_Price_Inc,   
                                    (void*)ConvenienceStore::Define::kShopStringCode_Filter_Price_Inc, 
                                    StrToWzID("st10"), DT_VCENTER | DT_CENTER);
        dropdown_sorting().InsertItem(ConvenienceStore::Define::kShopStringCode_Filter_Name_Inc,    
                                    (void*)ConvenienceStore::Define::kShopStringCode_Filter_Name_Inc, 
                                    StrToWzID("st10"), DT_VCENTER | DT_CENTER);
        dropdown_sorting().InsertItem(ConvenienceStore::Define::kShopStringCode_Filter_Name_Desc,   
                                    (void*)ConvenienceStore::Define::kShopStringCode_Filter_Name_Desc, 
                                    StrToWzID("st10"), DT_VCENTER | DT_CENTER);

        dropdown_sorting().ResetVScroll();
        dropdown_sorting().Select(0);

    }

    //! tab button
    if (is_result)
    {
        
        menu_tab().Push(GetCheckButtonControlWZ(GetControlID_byPosition(kControl_CheckBT_C033)), ConvenienceStore::Define::kShopMenuType_Cash);
        menu_tab().Push(GetCheckButtonControlWZ(GetControlID_byPosition(kControl_CheckBT_C034)), ConvenienceStore::Define::kShopMenuType_Mypage);

        if (GetControlWZ_byPosition(kControl_CheckBT_C001) &&
            GetControlWZ_byPosition(kControl_CheckBT_C002) &&
            GetControlWZ_byPosition(kControl_CheckBT_C021) &&
            GetControlWZ_byPosition(kControl_CheckBT_CT01) &&
            GetControlID_byPosition(kControl_CheckBT_C033))
        {
            GetControlWZ_byPosition(kControl_CheckBT_C001)->ShowWindowWZ(WZ_HIDE);
            GetControlWZ_byPosition(kControl_CheckBT_C002)->ShowWindowWZ(WZ_HIDE);
            GetControlWZ_byPosition(kControl_CheckBT_C021)->ShowWindowWZ(WZ_HIDE);
            GetControlWZ_byPosition(kControl_CheckBT_CT01)->ShowWindowWZ(WZ_HIDE);
            //! 기본 선택
            menu_tab().Click(GetCheckButtonControlWZ(GetControlID_byPosition(kControl_CheckBT_C033)));
        }
    }

    if (is_result)
    {
        is_result = page_control().Init(GetStaticControlWZ(GetControlID_byPosition(kControl_Text_ST01)), 
                                        GetButtonControlWZ(GetControlID_byPosition(kControl_Button_BT13)), 
                                        GetButtonControlWZ(GetControlID_byPosition(kControl_Button_BT12)));
    }

    //------------------------------------------------------------------------------ 
    //! 특정 control 메세지 못받게 함.
    for (int i = 0; i < kPagePerCount; ++i)
    {  
        if (GetControlWZ(GetControlID_byPosition(kControl_Text_S007 + i)) &&
            GetControlWZ(GetControlID_byPosition(kControl_Text_S017 + i)) &&
            GetControlWZ(GetControlID_byPosition(kControl_Text_S027 + i)) &&
            GetControlWZ(GetControlID_byPosition(kControl_Text_S037 + i)) &&
            GetControlWZ(GetControlID_byPosition(kControl_Text_S047 + i)) &&
            GetControlWZ(GetControlID_byPosition(kControl_Picture_P005 + i)) &&
            GetControlWZ(GetControlID_byPosition(kControl_Picture_P015 + i)) &&
            GetControlWZ(GetControlID_byPosition(kControl_TextImage_B001 + i)))
        {
            GetControlWZ(GetControlID_byPosition(kControl_Text_S007 + i))->SetDiscardMessage(true);
            GetControlWZ(GetControlID_byPosition(kControl_Text_S017 + i))->SetDiscardMessage(true);
            GetControlWZ(GetControlID_byPosition(kControl_Text_S027 + i))->SetDiscardMessage(true);
            GetControlWZ(GetControlID_byPosition(kControl_Text_S037 + i))->SetDiscardMessage(true);
            GetControlWZ(GetControlID_byPosition(kControl_Text_S047 + i))->SetDiscardMessage(true);
            GetControlWZ(GetControlID_byPosition(kControl_Picture_P005 + i))->SetDiscardMessage(true);
            GetControlWZ(GetControlID_byPosition(kControl_Picture_P015 + i))->SetDiscardMessage(true);
            GetControlWZ(GetControlID_byPosition(kControl_TextImage_B001 + i))->SetDiscardMessage(true);
        }
    }

#ifdef _KOREA
#else
    //타국가와는 전소개정이 틀림. 그럼으로 이용약관 바로가기는 안보여도됨.
   CControlWZ* controller =  GetControlWZ(GetControlID_byPosition(KControl_button_B002));
   if(controller)
   {
       controller->ShowWindowWZ(WZ_HIDE);
   }
#endif
    ASSERT(is_result && "uiConvenienceStore_Counter::Init false");
}
//------------------------------------------------------------------------------ 
void uiConvenienceStore_Counter::Release(void)
{
    ClearProductList();
    

    tree_menu().Release();

    dropdown_class_filter().Release();
    dropdown_sorting().Release();
    menu_tab().Release();
    page_control().Release();
}
//------------------------------------------------------------------------------
void uiConvenienceStore_Counter::ChangeTabMenu(ConvenienceStore::Define::ShopMenuType menu_type, bool is_gift_menu )
{
    DWORD user_data = 0; 
    if (menu_tab().Current(user_data) != NULL)
    {
        if (static_cast<ConvenienceStore::Define::ShopMenuType>(user_data) != menu_type)
        {
            menu_tab().Click(menu_type);
        }
    }

    //------------------------------------------------------------------------------ 
    if (menu_type == ConvenienceStore::Define::kShopMenuType_Mypage)
    {
        if (GetControlWZ_byPosition(kControl_CheckBT_C020))
            GetControlWZ_byPosition(kControl_CheckBT_C020)->ShowWindowWZ(WZ_SHOW);
        if (GetControlWZ_byPosition(kControl_CheckBT_CT02))
            GetControlWZ_byPosition(kControl_CheckBT_CT02)->ShowWindowWZ(WZ_SHOW);
        dropdown_sorting().SetVisible(false);
        dropdown_class_filter().SetVisible(false);
    }
    else
    {
        if (GetControlWZ_byPosition(kControl_CheckBT_C020))
            GetControlWZ_byPosition(kControl_CheckBT_C020)->ShowWindowWZ(WZ_HIDE);
        if (GetControlWZ_byPosition(kControl_CheckBT_CT02))
            GetControlWZ_byPosition(kControl_CheckBT_CT02)->ShowWindowWZ(WZ_HIDE);
        dropdown_sorting().SetVisible(true);
        dropdown_class_filter().SetVisible(true);
    }

    ChangeTreeObject(menu_type);

    if (is_gift_menu == true )
    {
        TreeMenuGroup::TreeObject* tree_object = tree_menu().Find(2, 0, 0, 0 );
        if (tree_object != NULL)
        {
            tree_menu().Select(tree_object);
        }
    }

    UpdatePage();
}
//------------------------------------------------------------------------------ 
void uiConvenienceStore_Counter::OnShowWindow(BOOL val)
{
    if (val == TRUE)
    {
        //! 초기 세팅
        dropdown_sorting().Select(0);
        dropdown_class_filter().Select(0);

        //if( is_open_with_gift_menu_ == true )
        //{
        //    menu_tab().Click(ConvenienceStore::Define::kShopMenuType_Mypage);
        //    ChangeTabMenu(ConvenienceStore::Define::kShopMenuType_Mypage, 1);

        //    is_open_with_gift_menu_ = false;
        //}
        //else
        //{
        //    menu_tab().Click(ConvenienceStore::Define::kShopMenuType_Cash);
        //    ChangeTabMenu(ConvenienceStore::Define::kShopMenuType_Cash);
        //}
    }
    else
    {
        if (GetManager()->popup_package()->owner() == this)
        {
            GetManager()->popup_package()->uiConvenienceStore_PopupMenu::ShowInterface(FALSE);
        }
    }
}
//------------------------------------------------------------------------------ 
void uiConvenienceStore_Counter::Process(DWORD tick)
{

}
//------------------------------------------------------------------------------ 
void uiConvenienceStore_Counter::OnUpdateSolarDialog()
{
    tree_menu().UpdateMouseWheel();

    dropdown_class_filter().Update();
    dropdown_sorting().Update();

    bool is_possible = IsPossibleShopPopup();
    UpdatePopupMenu(is_possible, IsShowTooltip());
    UpdateTooltip(is_possible);


    if (GetDialogWZ()->HaveFocus() == true)
    {
        GlobalFunc::MoveToTopDialog(GetManager()->basket()->GetDialogWZ()->GetDlgID());
    }    

    //------------------------------------------------------------------------------ 
    if (CurrentShopMenuType() == ConvenienceStore::Define::kShopMenuType_Mypage)
    {
        if( TreeMenuGroup::TreeObject* tree_object = tree_menu().Find(1, 0, 0, 0) )
        {
            if( ConvenienceStore::CashShop::instance()->GetStorageItemData().GetTotalItemCount() > 0 )
            {
                tree_object->add_data_flag(TreeMenuGroup::kDataFlag_IconBlink);
            }
            else
            {
                tree_object->remove_data_flag(TreeMenuGroup::kDataFlag_IconBlink);
            }
        }

        if( TreeMenuGroup::TreeObject* tree_object = tree_menu().Find(2, 0, 0, 0) )
        {
            if( ConvenienceStore::CashShop::instance()->GetGiftStorageItemData().GetTotalItemCount() > 0 )
            {
               tree_object->add_data_flag(TreeMenuGroup::kDataFlag_IconBlink);
            }
            else
            {
                tree_object->remove_data_flag(TreeMenuGroup::kDataFlag_IconBlink);
            }
        }
    }
}
//------------------------------------------------------------------------------ 
void uiConvenienceStore_Counter::NetworkProc(MSG_BASE* msgbase)
{

}
//------------------------------------------------------------------------------
void uiConvenienceStore_Counter::MessageProc(SI_MESSAGE* message)
{
    if (IsLock())
    {
        return;
    }
    if (GetManager() == NULL)
    {
        return;
    }

    //------------------------------------------------------------------------------ 
    if (menu_tab().MessageProc(message)) 
    {
        DWORD user_data = 0; 
        if (menu_tab().Current(user_data) != NULL)
        {
            ChangeTabMenu(static_cast<ConvenienceStore::Define::ShopMenuType>(user_data), false);
        }
        return;
    }

    //! 트리 메뉴 처리
    TreeMenuGroup::TreeObject* tree_object = NULL;
    if (tree_menu().MessageProc(message, OUT tree_object) == true)
    {
        if (tree_object != NULL) 
        {
            UpdatePage();
        }
        return;
    }

    //! 콤보 박스 처리
    bool is_change = false;
    if (dropdown_class_filter().MessageProc(message, OUT is_change))
    {
        if (is_change)
        {
            UpdatePage();
        }
        return;
    }
    is_change = false;
    if (dropdown_sorting().MessageProc(message, OUT is_change)) 
    {
        if (is_change)
        {
            UpdatePage();
        }
        return;
    }

    is_change = false;
    if (page_control().MessageProc(message, OUT is_change))
    {
        if (is_change)
        {
            if( IsSellPage() )
            {
                ConvenienceStore::ProductItemList& ProductList = ConvenienceStore::CashShop::instance()->GetProductItemData();
                ProductList.SetCurrentPage( page_control().current_page() );
                UpdatePage();
            }
            else if( IsMyStoragePage() )
            {
                int page = page_control().current_page();
                ConvenienceStore::StorageItemList& BuyStorage = ConvenienceStore::CashShop::instance()->GetStorageItemData();
                if( BuyStorage.GetPage() != page )
                {
                    ConvenienceStore::CashShop::instance()->RequestBuyStorageList(page);
                }
            }
            else if( IsGiftStoragePage() )
            {
                int page = page_control().current_page();
                ConvenienceStore::StorageItemList& GiftStorage = ConvenienceStore::CashShop::instance()->GetGiftStorageItemData();

                if( GiftStorage.GetPage() != page )
                {
                    ConvenienceStore::CashShop::instance()->RequestGiftStorageList(page);
                }
            }
        }
        return;
    }

    //------------------------------------------------------------------------------ 
    switch (message->eResultMsg)
    {
    case RT_MSG_LBUTTONCLICK:
        OnLButtonClick(message);
        break;
    case RT_MSG_RBUTTONCLICK:
        OnRButtonClick(message);
        break;
    default:
        SolarDialog::MessageProc(message);
        break;
    }
}
//------------------------------------------------------------------------------
bool uiConvenienceStore_Counter::OnLButtonClick(SI_MESSAGE* message)
{
    bool result = true;
    bool is_cashItem_NewRing = false;
    ConvenienceStore::ShopItemObject* product_item = NULL;
    switch (GetControlPosition_byID(message->dwCtrlID))
    {
    case kControl_CheckBT_C020:
        {
           
            //모두 받기
            if (CurrentMyPageType() == ConvenienceStore::Define::kMyPageType_BuyBox)
            {
                if (ConvenienceStore::CashShop::instance()->GetStorageItemData().GetCountPerPage() > 0 )
                {
#ifdef _NA_000000_20130710_CHARGE_SUB_FIXED_AMOUNT_EX2
                    ConvenienceStore::StorageItemList& StorageData = ConvenienceStore::CashShop::instance()->GetStorageItemData();
                    for( size_t i=0;i!=StorageData.GetCountPerPage();++i)
                    {
                        if(StorageData.GetItemData(i)->GetProductInfo().GetItemInfo() == NULL)
                            continue;

                        const BASE_ITEMINFO* item_info = ItemInfoParser::Instance()->GetItemInfo(
                            StorageData.GetItemData(i)->GetProductInfo().GetItemInfo()->m_Code); 

                        if(item_info && item_info->m_ChargeSubType == eCHARGE_SUB_FIXED_AMOUNT_EX2)
                        {
                            product_item = StorageData.GetItemData(i);
                            is_cashItem_NewRing = true;
                            break;
                        }
                    }

                    if(is_cashItem_NewRing)
                    {
                        //조화와 균형의 반지(60297)이 하나라도 있을경우 메시지 띠움.
                        GetManager()->ask()->OpenAsk( this, uiConvenienceStore_Dialog::kAskShow_StorageNewTypeBuy_AllRecv );
                        GetManager()->ask()->AppendItemTextToString(158534,product_item);
                    }
                    else
#endif
                    {
                        //155542	아래 아이템을 보관함에서 꺼내시겠습니까?
                        //155543	보관함에서 꺼낸 아이템은 인벤토리에서\n확인이 가능하며, 인벤토리로 이동된 모든\n아이템들은 구입 취소가 불가능합니다.\n(단, 복권류 아이템은 무조건 취소불가)
                        //155570	<font=tt12><color=ffffffff>-----------------------------------------------</color></font>
                        GetManager()->ask()->OpenAsk( this, uiConvenienceStore_Dialog::kAskShow_StorageBuy_AllRecv );
                        GetManager()->ask()->AppendFText( g_InterfaceManager.GetInterfaceString(155542) );
                        GetManager()->ask()->AppendFText( g_InterfaceManager.GetInterfaceString(155543) );
                        GetManager()->ask()->AppendFText( g_InterfaceManager.GetInterfaceString(155570) );

                        ConvenienceStore::StorageItemList& StorageData = ConvenienceStore::CashShop::instance()->GetStorageItemData();
                        for( size_t i=0;i!=StorageData.GetCountPerPage();++i)
                        {
                            GetManager()->ask()->AppendItemText( StorageData.GetItemData(i) );
                        }
                    }
                }
            }
            else if (CurrentMyPageType() == ConvenienceStore::Define::kMyPageType_RecvBox)
            {
#ifdef _NA_000000_20130710_CHARGE_SUB_FIXED_AMOUNT_EX2
                if (ConvenienceStore::CashShop::instance()->GetGiftStorageItemData().GetCountPerPage() > 0)
                {
                    ConvenienceStore::StorageItemList& StorageData = ConvenienceStore::CashShop::instance()->GetGiftStorageItemData();
                    for( size_t i=0;i!=StorageData.GetCountPerPage();++i)
                    {
                        if(StorageData.GetItemData(i)->GetProductInfo().GetItemInfo() == NULL)
                            continue;

                        const BASE_ITEMINFO* item_info = ItemInfoParser::Instance()->GetItemInfo(
                            StorageData.GetItemData(i)->GetProductInfo().GetItemInfo()->m_Code); 

                        if(item_info && item_info->m_ChargeSubType == eCHARGE_SUB_FIXED_AMOUNT_EX2)
                        {
                            product_item = StorageData.GetItemData(i);
                            is_cashItem_NewRing = true;
                            break;
                        }
                    }

                    if(is_cashItem_NewRing)
                    {
                        //조화와 균형의 반지(60297)이 하나라도 있을경우 메시지 띠움.
                        GetManager()->ask()->OpenAsk( this, uiConvenienceStore_Dialog::kAskShow_StorageNewTypeBuy_AllRecv );
                        GetManager()->ask()->AppendItemTextToString(158534,product_item);
                    }
                    else
#endif
                    {
                        //155542	아래 아이템을 보관함에서 꺼내시겠습니까?
                        //155543	보관함에서 꺼낸 아이템은 인벤토리에서\n확인이 가능하며, 인벤토리로 이동된 모든\n아이템들은 구입 취소가 불가능합니다.\n(단, 복권류 아이템은 무조건 취소불가)
                        //155570	<font=tt12><color=ffffffff>-----------------------------------------------</color></font>
                        GetManager()->ask()->OpenAsk( this, uiConvenienceStore_Dialog::kAskShow_StorageGift_AllRecv );
                        GetManager()->ask()->AppendFText( g_InterfaceManager.GetInterfaceString(155542) );
                        GetManager()->ask()->AppendFText( g_InterfaceManager.GetInterfaceString(155543) );
                        GetManager()->ask()->AppendFText( g_InterfaceManager.GetInterfaceString(155570) );

                        ConvenienceStore::StorageItemList& StorageData = ConvenienceStore::CashShop::instance()->GetGiftStorageItemData();
                        for( size_t i=0;i!=StorageData.GetCountPerPage();++i)
                        {
                            GetManager()->ask()->AppendItemText( StorageData.GetItemData(i) );
                        }
                    }
                }
            }
        }
        break;
    case kControl_CheckBT_CT02:
        {
#if defined(_GLOBAL) || defined(_CHINA)
            //! 구매내역     //155567	http://www.sunonline.co.kr
            GlobalFunc::ExecWebSite(155567);
#else
            // 6825	준비중입니다. 웹페이지에서 확인해주세요.
            GetManager()->ask()->OpenAsk( this, uiConvenienceStore_Dialog::kAskShow_Message );
            GetManager()->ask()->AppendFText( g_InterfaceManager.GetInterfaceString( 6825 ) );
            GetManager()->ask()->EnableCloseButton();
#endif

        }
        break;
    case kControl_Button_BT12:
        {
        }
        break;
    case kControl_Button_BT13:
        {
        }
        break;
    case kControl_Button_B004:
        {
        }
        break;
    case kControl_Button_B000:
        {
        }
        break;

    case kControl_Button_B006:
    case kControl_Button_B042:
    case kControl_Button_B043:
    case kControl_Button_B044:
    case kControl_Button_B045:
    case kControl_Button_B046:
    case kControl_Button_B047:
    case kControl_Button_B048:
    case kControl_Button_B049:
    case kControl_Button_B050:
        {
            if (GetManager()->popup_menu_four()->IsVisible() == TRUE)
            {
                g_InterfaceManager.AttachFirstDialog(uiConvenienceStore_PopupMenuFour::kDialog_ID);
            }
            if (GetManager()->popup_menu_three()->IsVisible() == TRUE)
            {
                g_InterfaceManager.AttachFirstDialog(uiConvenienceStore_PopupMenuThree::kDialog_ID);
            }
            if (GetManager()->popup_menu_two()->IsVisible() == TRUE)
            {
                g_InterfaceManager.AttachFirstDialog(uiConvenienceStore_PopupMenuTwo::kDialog_ID);
            }
            if (GetManager()->popup_package()->IsVisible() == TRUE)
            {
                g_InterfaceManager.AttachFirstDialog(uiConvenienceStore_PopupPackage::kDialog_ID);
            }

            //! 
            if( ConvenienceStore::ShopItemObject* pObject = reinterpret_cast<ConvenienceStore::ShopItemObject*>(GetControlWZ(message->dwCtrlID)->GetUserData()) )
            {
                if( pObject->IsProduct() )
                {
                    ConvenienceStore::ProductItem* product = static_cast<ConvenienceStore::ProductItem*>(pObject);
                    if (product->is_package() == true)
                    {
                        if (uiConvenienceStore_Basket::HasPreviewItem(product))
                        {
                            OnClickPopupMenu(uiConvenienceStore_Dialog::kPopupCommand_Counter_Preview, (DWORD)pObject);
                        }
                    }
                    else if (product->is_lotto() == false)
                    {
                        if ((product->first_info()->is_etheria() == false) && 
                            (product->first_info()->is_etheria_with_divine() == false))
                        {
                            if (uiConvenienceStore_Basket::IsPossiblePreview(product->item_code(0)))
                            {
                                OnClickPopupMenu(uiConvenienceStore_Dialog::kPopupCommand_Counter_Preview, (DWORD)pObject);
                            }
                            else
                            {
                                GetManager()->basket()->ClearProduct();
                                GetManager()->basket()->UpdateEquipControl();
                                GetManager()->basket()->SetActiveProductTab();
                            }

                        }
                    }
                }
            }
        }
        break;

    case kControl_Text_ST01:
        {
        }
        break;
    case kControl_List_L002:
        {
        }
        break;
    case kControl_Picture_P001:
        {
        }
        break;
    case kControl_Picture_P002:
        {
        }
        break;
    case kControl_Picture_P003:
        {
        }
        break;
    case kControl_Picture_PI00:
        {
        }
        break;
    case kControl_Picture_P004:
        {
        }
        break;
    case kControl_Picture_P000:
        {
        }
        break;
    case kControl_Picture_P005:
        {
        }
        break;
    case kControl_Picture_P006:
        {
        }
        break;
    case kControl_Picture_P007:
        {
        }
        break;
    case kControl_Picture_P008:
        {
        }
        break;
    case kControl_Picture_P009:
        {
        }
        break;
    case kControl_Picture_P010:
        {
        }
        break;
    case kControl_Picture_P011:
        {
        }
        break;
    case kControl_Picture_P012:
        {
        }
        break;
    case kControl_Picture_P013:
        {
        }
        break;
    case kControl_Picture_P014:
        {
        }
        break;
    case kControl_Picture_P015:
        {
        }
        break;
    case kControl_Picture_P016:
        {
        }
        break;
    case kControl_Picture_P017:
        {
        }
        break;
    case kControl_Picture_P018:
        {
        }
        break;
    case kControl_Picture_P019:
        {
        }
        break;
    case kControl_Picture_P020:
        {
        }
        break;
    case kControl_Picture_P021:
        {
        }
        break;
    case kControl_Picture_P022:
        {
        }
        break;
    case kControl_Picture_P023:
        {
        }
        break;
    case kControl_Picture_P024:
        {
        }
        break;
    case KControl_button_B002:
        {
            TCHAR register_url[INTERFACE_STRING_LENGTH];
            //이용약관 바로가기
           g_InterfaceManager.GetInterfaceString(155843, register_url, INTERFACE_STRING_LENGTH);
            SHELLEXECUTEINFO shell_execute_info;

            shell_execute_info.cbSize = sizeof(SHELLEXECUTEINFO);
            shell_execute_info.fMask = NULL;
            shell_execute_info.hwnd = g_hWndMain;
            shell_execute_info.lpVerb = NULL;
            shell_execute_info.lpFile = register_url;
            shell_execute_info.lpParameters = NULL;
            shell_execute_info.lpDirectory = NULL;
            shell_execute_info.nShow = SW_MAXIMIZE;
            shell_execute_info.hInstApp = NULL;

            ShellExecuteEx(&shell_execute_info);
        }
        break;
    default:
        {
        }
        break;
    }
    return result;
}
//------------------------------------------------------------------------------ 
bool uiConvenienceStore_Counter::OnRButtonClick(SI_MESSAGE* message)
{
    bool result = true;
    switch (GetControlPosition_byID(message->dwCtrlID))
    {
    case kControl_Button_B006:
    case kControl_Button_B042:
    case kControl_Button_B043:
    case kControl_Button_B044:
    case kControl_Button_B045:
    case kControl_Button_B046:
    case kControl_Button_B047:
    case kControl_Button_B048:
    case kControl_Button_B049:
    case kControl_Button_B050:
        {

            //! 좌클릭 장바구니에 담기
            ConvenienceStore::ShopItemObject* pObject = reinterpret_cast<ConvenienceStore::ShopItemObject*>(GetControlWZ(message->dwCtrlID)->GetUserData());
            if(pObject && pObject->IsProduct() )
            {
                ConvenienceStore::ProductItem* pProduct = (ConvenienceStore::ProductItem*)pObject;
                if(pProduct && pProduct->GetPrice() > 0 )
                {
                    OnClickPopupMenu(uiConvenienceStore_Dialog::kPopupCommand_Counter_Pickup, (DWORD)pObject);
                }
            }
        }
        break;
    default:
        {
            CControlWZ* control = GetControlWZ(message->dwCtrlID);
            DWORD pos = GetControlPosition_byID(message->dwCtrlID);
            int a = 0;
        }
        break;
    }
    return result;
}
//------------------------------------------------------------------------------ 
void uiConvenienceStore_Counter::ClearProductList()
{
    //------------------------------------------------------------------------------ 
    //! control에 userdata 형태로 걸려 있는 넘은 모두 없앤다.
    for (int i = 0; i < kPagePerCount; ++i)
    {  
        if( CControlWZ* picture_control = GetControlWZ_byPosition(kControl_TextImage_B001 + i) )
        {
            picture_control->SetUserData(NULL);
        }
        if( CControlWZ* button_control = GetControlWZ_byPosition(kControl_Button_B006 + i) )
        {
            button_control->SetUserData(NULL);
        }
    }

    //ConvenienceStore::CashShop::instance()->GetProductItemData().ClearProductArray();
}
//------------------------------------------------------------------------------ 
bool uiConvenienceStore_Counter::OnClickPopupMenu(const uiConvenienceStore_Dialog::PopupCommandType command_type, const DWORD user_data)
{
    ConvenienceStore::ShopItemObject* pObject = reinterpret_cast<ConvenienceStore::ShopItemObject*>(user_data);
    if (pObject == NULL)
    {
        return false;
    }

    if (pObject->IsStorageItem() )
    {
        //ConvenienceStore::Storage* storage = static_cast<ConvenienceStore::Storage*>(obj);

        if (command_type == uiConvenienceStore_Dialog::kPopupCommand_MyPage_Recv)
        {
            if (CurrentMyPageType() == ConvenienceStore::Define::kMyPageType_BuyBox)
            {
#ifdef _NA_000000_20130710_CHARGE_SUB_FIXED_AMOUNT_EX2
                
                ConvenienceStore::StorageItem* product_item = static_cast<ConvenienceStore::StorageItem*>(pObject);
                
                if(product_item && product_item->GetProductInfo().GetItemInfo())
                {
                    const BASE_ITEMINFO* item_info = ItemInfoParser::Instance()->GetItemInfo(
                        product_item->GetProductInfo().GetItemInfo()->m_Code); 

                    if(item_info && item_info->m_ChargeSubType == eCHARGE_SUB_FIXED_AMOUNT_EX2)
                    {
                        GetManager()->ask()->OpenAsk(this, uiConvenienceStore_Dialog::KAskShow_ItemCashNewType );
                        GetManager()->ask()->AppendItemTextToString(158534,pObject);
                    }
                    else
                    {
                        GetManager()->ask()->OpenAsk(this, uiConvenienceStore_Dialog::kAskShow_StorageBuy_OneRecv );
                        GetManager()->ask()->AppendFText( g_InterfaceManager.GetInterfaceString( 155542 ) );
                        GetManager()->ask()->AppendFText( g_InterfaceManager.GetInterfaceString( 155543 ) );
                        GetManager()->ask()->AppendFText( g_InterfaceManager.GetInterfaceString( 155570 ) );

                        GetManager()->ask()->AppendItemText( pObject );
                        GetManager()->ask()->SetItemObject( pObject );
                    }
                }
                else
#endif
                {
                    //155542	아래 아이템을 보관함에서 꺼내시겠습니까?
                    //155543	보관함에서 꺼낸 아이템은 인벤토리에서\n확인이 가능하며, 인벤토리로 이동된 모든\n아이템들은 구입 취소가 불가능합니다.\n(단, 복권류 아이템은 무조건 취소불가)
                    //155570	<font=tt12><color=ffffffff>-----------------------------------------------</color></font>

                    GetManager()->ask()->OpenAsk(this, uiConvenienceStore_Dialog::kAskShow_StorageBuy_OneRecv );
                    GetManager()->ask()->AppendFText( g_InterfaceManager.GetInterfaceString( 155542 ) );
                    GetManager()->ask()->AppendFText( g_InterfaceManager.GetInterfaceString( 155543 ) );
                    GetManager()->ask()->AppendFText( g_InterfaceManager.GetInterfaceString( 155570 ) );

                    GetManager()->ask()->AppendItemText( pObject );
                    GetManager()->ask()->SetItemObject( pObject );
                }
            }
            else if (CurrentMyPageType() == ConvenienceStore::Define::kMyPageType_RecvBox)
            {
#ifdef _NA_000000_20130710_CHARGE_SUB_FIXED_AMOUNT_EX2      
                ConvenienceStore::StorageItem* product_item = static_cast<ConvenienceStore::StorageItem*>(pObject);

                if(product_item && product_item->GetProductInfo().GetItemInfo())
                {
                    const BASE_ITEMINFO* item_info = ItemInfoParser::Instance()->GetItemInfo(
                        product_item->GetProductInfo().GetItemInfo()->m_Code); 


                    if(item_info && item_info->m_ChargeSubType == eCHARGE_SUB_FIXED_AMOUNT_EX2)
                    {
                        GetManager()->ask()->OpenAsk(this, uiConvenienceStore_Dialog::KAskShow_ItemCashGifeNewType );
                        GetManager()->ask()->AppendItemTextToString(158534,pObject);
                    }
                    else
#endif
                    {
                        //155545	아래 아이템을 받은 선물함에서 꺼내시겠습니까?
                        //155546	선물함에 보관중인 모든 아이템은\n반송 및 구입 취소가 불가능 합니다.
                        //155570	<font=tt12><color=ffffffff>-----------------------------------------------</color></font>
                        GetManager()->ask()->OpenAsk(this, uiConvenienceStore_Dialog::kAskShow_StorageGift_OneRecv );
                        GetManager()->ask()->AppendFText( g_InterfaceManager.GetInterfaceString( 155545 ) );
                        GetManager()->ask()->AppendFText( g_InterfaceManager.GetInterfaceString( 155546 ) );
                        GetManager()->ask()->AppendFText( g_InterfaceManager.GetInterfaceString( 155570 ) );

                        GetManager()->ask()->AppendItemText( pObject );
                        GetManager()->ask()->SetItemObject( pObject );
                    }
                }
            }
        }
        else if (command_type == uiConvenienceStore_Dialog::kPopupCommand_MyPage_Msg)
        {
            GetManager()->item_detail()->ShowInterface(pObject);
        }
    }
    else if( pObject->IsProduct() )
    {
        ConvenienceStore::ProductItem* pProduct = static_cast<ConvenienceStore::ProductItem*>(pObject);

        if (command_type == uiConvenienceStore_Dialog::kPopupCommand_Counter_Pickup)
        {
            if (GetManager()->basket()->InsertBasket(pProduct) == false)
            {
                //155563	장바구니가 가득차서 해당 아이템을 담을 수 없습니다.\n장바구니를 비우고 다시 시도하시길 바랍니다.	
                GetManager()->ask()->OpenAsk(this, uiConvenienceStore_Dialog::kAskShow_Message );
                GetManager()->ask()->AppendFText(g_InterfaceManager.GetInterfaceString(155563));
                GetManager()->ask()->EnableCloseButton();
            }
        }
        else if (command_type == uiConvenienceStore_Dialog::kPopupCommand_Counter_Preview)
        {
            bool is_res = GetManager()->basket()->CreateProduct(pProduct);

        }
        else if (command_type == uiConvenienceStore_Dialog::kPopupCommand_Counter_Buy)
        {
#ifdef _GLOBAL
            if( pProduct->is_pcash_only() )
            {
                GetManager()->purchase()->SetCashType(ConvenienceStore::Define::kCashType_PPCard);
                GetManager()->purchase()->OpenPurchase(uiConvenienceStore_Purchase::kPurchase_Buy);
                GetManager()->purchase()->InsertPurchase(pProduct, -1);
            }
            else if( pProduct->is_cash_only() )
            {
                GetManager()->purchase()->SetCashType(ConvenienceStore::Define::kCashType_Cash);
                GetManager()->purchase()->OpenPurchase(uiConvenienceStore_Purchase::kPurchase_Buy);
                GetManager()->purchase()->InsertPurchase(pProduct, -1);
            }
            else
            {
                GetManager()->cashtype()->SetBuyProductInfo(pProduct,-1);
                GetManager()->cashtype()->OpenSelectCashType();
            }
#elif defined(_CHINA)
            if( pProduct->is_mileage_only() )
            {
                GetManager()->purchase()->SetCashType( ConvenienceStore::Define::kCashType_Mileage );
                GetManager()->purchase()->OpenPurchase(uiConvenienceStore_Purchase::kPurchase_Buy);
                GetManager()->purchase()->InsertPurchase(pProduct, -1);
            }
            else
            {
                GetManager()->purchase()->SetCashType( ConvenienceStore::Define::kCashType_Cash );
                GetManager()->purchase()->OpenPurchase(uiConvenienceStore_Purchase::kPurchase_Buy);
                GetManager()->purchase()->InsertPurchase(pProduct, -1);
            }

#else
            GetManager()->purchase()->SetCashType(ConvenienceStore::Define::kCashType_Cash);
            GetManager()->purchase()->OpenPurchase(uiConvenienceStore_Purchase::kPurchase_Buy);
            GetManager()->purchase()->InsertPurchase(pProduct, -1);
#endif
        }
        else if (command_type == uiConvenienceStore_Dialog::kPopupCommand_Counter_Gift)
        {
            GetManager()->gift()->ShowInterface(true, pProduct);
        }
    }

    return true;
}

void uiConvenienceStore_Counter::FillProductPage()
{
    // 소팅하고
    ConvenienceStore::ProductItemList& ProductList = ConvenienceStore::CashShop::instance()->GetProductItemData();
    ProductList.SortProductList(static_cast<ConvenienceStore::Define::ShopStringCode>(dropdown_sorting().CurrentUserData()));

    int page = ProductList.GetCurrentPage() - 1;

    for (int i = 0; i < kPagePerCount; ++i)
    {  
        int product_index = (page * kPagePerCount) + i;

        CCtrlStaticWZ* name_control = GetStaticControlWZ(GetControlID_byPosition(kControl_Text_S007 + i));
        CCtrlStaticWZ* price_control = GetStaticControlWZ(GetControlID_byPosition(kControl_Text_S017 + i));
        CCtrlStaticWZ* desc_control = GetStaticControlWZ(GetControlID_byPosition(kControl_Text_S027 + i));
        CCtrlStaticWZ* sned_name_control = GetStaticControlWZ(GetControlID_byPosition(kControl_Text_S037 + i));
        CCtrlStaticWZ* to_inventory_control = GetStaticControlWZ(GetControlID_byPosition(kControl_Text_S047 + i));
        CCtrlImageStaticWZ* picture_control = GetImageStaticControlWZ(GetControlID_byPosition(kControl_TextImage_B001 + i));
        CCtrlButtonWZ* button_control = GetButtonControlWZ(GetControlID_byPosition(kControl_Button_B006 + i));

        if (name_control == NULL || 
            price_control == NULL || 
            desc_control == NULL || 
            sned_name_control == NULL || 
            to_inventory_control == NULL || 
            picture_control == NULL ||
            button_control == NULL) 
        {
            continue;
        }

        name_control->SetTextWZ("");
        price_control->SetTextWZ("");
        sned_name_control->SetTextWZ("");
        to_inventory_control->SetTextWZ("");
        desc_control->SetTextWZ("");
        button_control->SetUserData(NULL);
        picture_control->SetUserData(NULL);
        picture_control->SetCallbackDraw(NULL, this);

        button_control->EnableWindowWZ(false);

        if (product_index < (int)(ProductList.GetProductCount()))
        {
            ConvenienceStore::ProductItem* pProduct = ProductList.GetProduct(product_index); 

            TCHAR msg_name[INTERFACE_STRING_LENGTH] = {NULL, };
            TCHAR msg_desc1[INTERFACE_STRING_LENGTH] = {NULL, };
            TCHAR msg_desc2[INTERFACE_STRING_LENGTH] = {NULL, };

            StrnCopy(msg_name, pProduct->GetName(), SHOPLIST_LENGTH_PACKAGENAME); 
#ifdef _NA_000000_20150130_ITEMSHOP_NOT_SLOTTEXT_ALL_FILL
			GlobalFunc::ProcessWordWrapByFontSize(msg_desc2, 
				pProduct->GetDescription(),
				desc_control->GetFontID(),
				_tcslen(pProduct->GetDescription()),
				desc_control->GetSize().right,
				2, /*2줄*/
				false /*말줄임하지않음*/);
#else
            GlobalFunc::ProcessWordWrapByFontSize(msg_desc2, 
                pProduct->GetDescription(),
                desc_control->GetFontID(),
                _tcslen(pProduct->GetDescription()),
                desc_control->GetSize().right,
                2, /*2줄*/
                true /*말줄임*/);
#endif // _NA_000000_20150130_ITEMSHOP_NOT_SLOTTEXT_ALL_FILL
            desc_control->SetStyle(DT_LEFT | DT_VCENTER);
            ConvenienceStore::GetDisplayPriceStringByProduct(msg_desc1, INTERFACE_STRING_LENGTH, pProduct);

            name_control->SetTextWZ(msg_name);
            price_control->SetTextWZ(msg_desc1);
            desc_control->SetTextWZ(msg_desc2); 


            button_control->EnableWindowWZ(true);
            button_control->SetUserData(reinterpret_cast<DWORD>(pProduct));
            picture_control->SetUserData(reinterpret_cast<DWORD>(pProduct));
            picture_control->SetCallbackDraw(uiConvenienceStore_Counter::Callback_IconRender, this);
        }
    }
}

//------------------------------------------------------------------------------ 
void uiConvenienceStore_Counter::UpdatePage()
{
    if( IsSellPage() )
    {
        //! 리스트를 다시 구정해야 하기 때문에 팝업을 해당 화면의 팝업은 다 내린다.
        uiConvenienceStore_Dialog::UpdatePopupMenu(false, false);

        ConvenienceStore::ProductItemList& ProductList = ConvenienceStore::CashShop::instance()->GetProductItemData();
        ProductList.BuildProductList( ConvenienceStore::ProductItemList::PRODUCT_TYPE_CASH, GetCurrentSellCategorySequence(), GetFilterType(), GetCurrentSellCategoryImageType() );

        //! 페이지 다시 계산 후 첫번째 페이지로 설정
        page_control().SetPage(ProductList.GetCurrentPage(), ProductList.GetMaxPage());

        //int shop_category_sequence = GetCurrentSellCategorySequence();
        //int shop_category_image_type = GetCurrentSellCategoryImageType();
        //eCHAR_TYPE class_filter = GetFilterType();

        //if( last_sell_category_sequence_ != shop_category_sequence || 
        //    last_sell_category_class_filter_ != class_filter )
        //{
        //    // 카테고리가 변경되면 목록을 새로 만든다.
        //    ClearProductList();

        //    if( shop_category_sequence > 0 )
        //    {
        //        CShopCategory* shop_category = NULL;
        //        if (ConvenienceStore::category_list()->GetValueByKey(shop_category_sequence, OUT shop_category) == false)
        //        {
        //            return;
        //        }

        //        _FillProductList( CurrentShopMenuType(), shop_category, shop_category_image_type );

        //    }

        //    //! 페이지 다시 계산 후 첫번째 페이지로 설정
        //    page_control().SetPage(ProductList.GetCurrentPage(), ProductList.GetMaxPage());

        //    last_sell_category_sequence_ = shop_category_sequence;
        //    last_sell_category_class_filter_ = class_filter;
        //}

        FillProductPage();
    }
    else if( IsMyStoragePage() )
    {
        FillMyStoragePage();
    }
    else if( IsGiftStoragePage() )
    {
        FillGiftStoragePage();
    }
}

int uiConvenienceStore_Counter::GetCurrentSellCategoryImageType()
{
    if( IsSellPage() == true )
    {
        if( TreeMenuGroup::TreeObject* pTreeObject = tree_menu().CurrectTreeObject() )
        {
            if( pTreeObject->info != NULL )
            {
                return pTreeObject->info->user_parameter;
            }
        }
    }
    return 0;
}

int uiConvenienceStore_Counter::GetCurrentSellCategorySequence()
{
    if( IsSellPage() == true )
    {
        if( TreeMenuGroup::TreeObject* pTreeObject = tree_menu().CurrectTreeObject() )
        {
            if( pTreeObject->info != NULL )
            {
                return pTreeObject->info->user_data;
            }
        }
    }
    return 0;
}


eCHAR_TYPE uiConvenienceStore_Counter::GetFilterType()
{
    eCHAR_TYPE char_type = eCHAR_NONE;
    if (dropdown_class_filter().IsVisible())
    {
        char_type = static_cast<eCHAR_TYPE>(dropdown_class_filter().CurrentUserData());
    }
    return char_type;
}

bool uiConvenienceStore_Counter::IsSellPage() 
{
    return CurrentShopMenuType() == ConvenienceStore::Define::kShopMenuType_Cash || CurrentShopMenuType() == ConvenienceStore::Define::kShopMenuType_PPCard || CurrentShopMenuType() == ConvenienceStore::Define::kShopMenuType_Mileage; 
}

bool uiConvenienceStore_Counter::IsGiftStoragePage() 
{
    if( CurrentShopMenuType() == ConvenienceStore::Define::kShopMenuType_Mypage )
    {
        if( TreeMenuGroup::TreeObject* pTreeObject = tree_menu().CurrectTreeObject() )
        {
            if( pTreeObject->info != NULL )
            {
                ConvenienceStore::Define::MyPageType PageType = static_cast<ConvenienceStore::Define::MyPageType>(pTreeObject->info->user_data);
                return PageType == ConvenienceStore::Define::kMyPageType_RecvBox;
            }
        }
    }
    return false;
}

bool uiConvenienceStore_Counter::IsMyStoragePage() 
{
    if( CurrentShopMenuType() == ConvenienceStore::Define::kShopMenuType_Mypage )
    {
        if( TreeMenuGroup::TreeObject* pTreeObject = tree_menu().CurrectTreeObject() )
        {
            if( pTreeObject->info != NULL )
            {
                ConvenienceStore::Define::MyPageType PageType = static_cast<ConvenienceStore::Define::MyPageType>(pTreeObject->info->user_data);
                return PageType == ConvenienceStore::Define::kMyPageType_BuyBox;
            }
        }
    }
    return false;
}

void uiConvenienceStore_Counter::FillMyStoragePage()
{
    //! 리스트를 다시 구정해야 하기 때문에 팝업을 해당 화면의 팝업은 다 내린다.
    uiConvenienceStore_Dialog::UpdatePopupMenu(false, false);

    ConvenienceStore::StorageItemList& Data = ConvenienceStore::CashShop::instance()->GetStorageItemData();

    // 페이지 설정
    int total_page = 0;
    if (Data.GetTotalItemCount() > 0)
    {
        total_page = (Data.GetTotalItemCount() -1) / kPagePerCount + 1;
    }
    if (total_page == 0)
    {
        page_control().SetPage( 1, 1 );
    }
    else
    {
        page_control().SetPage( Data.GetPage(), total_page );
    }

    // 페이지 설정
    for (int i = 0; i < kPagePerCount; ++i)
    {

        CCtrlStaticWZ* name_control = GetStaticControlWZ(GetControlID_byPosition(kControl_Text_S007 + i));
        CCtrlStaticWZ* price_control = GetStaticControlWZ(GetControlID_byPosition(kControl_Text_S017 + i));
        CCtrlStaticWZ* desc_control = GetStaticControlWZ(GetControlID_byPosition(kControl_Text_S027 + i));
        CCtrlStaticWZ* sned_name_control = GetStaticControlWZ(GetControlID_byPosition(kControl_Text_S037 + i));
        CCtrlStaticWZ* to_inventory_control = GetStaticControlWZ(GetControlID_byPosition(kControl_Text_S047 + i));
        CCtrlImageStaticWZ* picture_control = GetImageStaticControlWZ(GetControlID_byPosition(kControl_TextImage_B001 + i));
        CCtrlButtonWZ* button_control = GetButtonControlWZ(GetControlID_byPosition(kControl_Button_B006 + i));

        if (name_control == NULL || 
            price_control == NULL || 
            desc_control == NULL || 
            sned_name_control == NULL || 
            to_inventory_control == NULL || 
            picture_control == NULL ||
            button_control == NULL) 
        {
            continue;
        }

        name_control->SetTextWZ("");
        price_control->SetTextWZ("");
        sned_name_control->SetTextWZ("");
        to_inventory_control->SetTextWZ("");
        desc_control->SetTextWZ("");
        button_control->SetUserData(NULL);
        picture_control->SetUserData(NULL);
        picture_control->SetCallbackDraw(NULL, this);

        button_control->EnableWindowWZ(false);

        if( i < Data.GetCountPerPage() )
        {
            ConvenienceStore::StorageItem* pItemData = Data.GetItemData((size_t)i);

            name_control->SetTextWZ( pItemData->GetName() );

            //155505	구입 
            TCHAR msg_text[INTERFACE_STRING_LENGTH] = {NULL, };
            ConvenienceStore::GetDisplayPriceStringByProductInfo(msg_text, INTERFACE_STRING_LENGTH, &(pItemData->GetProductInfo() ), false );

#ifdef _RU_0000_20120312_RUSSIA_MOD_CASH_SHOP_UI
            // no operation
#else
            time_t temp_time = pItemData->GetDeleteDate();
            tm local_tm;
            TCHAR msg_time[INTERFACE_STRING_LENGTH] = {NULL, };

            if( temp_time == 0xffffffff )
            {
                memset(&local_tm,0,sizeof(tm));
            }
            else
            {
                local_tm = *(localtime(&temp_time));
                local_tm.tm_year += 1900;
                local_tm.tm_mon += 1;
            }

            //155572	만료일: %s
            GlobalFunc::MakeMessageFromStringTable_tm(msg_time, INTERFACE_STRING_LENGTH, 155572, &local_tm);


            _tcscat(msg_text, " / ");
            _tcscat(msg_text, msg_time);
#endif //_RU_0000_20120312_RUSSIA_MOD_CASH_SHOP_UI

            price_control->SetTextWZ(msg_text);

            //155553	인벤토리에 받은 후 사용가능
            desc_control->SetTextID(155553);


            button_control->EnableWindowWZ(true);
            button_control->SetUserData(reinterpret_cast<DWORD>(pItemData));
            picture_control->SetUserData(reinterpret_cast<DWORD>(pItemData));
            picture_control->SetCallbackDraw(uiConvenienceStore_Counter::Callback_IconRender, this);
        }
    }

}

void uiConvenienceStore_Counter::FillGiftStoragePage()
{
    //! 리스트를 다시 구정해야 하기 때문에 팝업을 해당 화면의 팝업은 다 내린다.
    uiConvenienceStore_Dialog::UpdatePopupMenu(false, false);

    ConvenienceStore::StorageItemList& Data = ConvenienceStore::CashShop::instance()->GetGiftStorageItemData();

    // 페이지 설정
    int total_page = 0;
    if (Data.GetTotalItemCount() > 0)
    {
        total_page = (Data.GetTotalItemCount() -1) / kPagePerCount + 1;
    }

    if (total_page == 0)
    {
        page_control().SetPage( 1, 1 );
    }
    else
    {
        page_control().SetPage( Data.GetPage(), total_page );
    }

    // 페이지 설정
    for (int i = 0; i < kPagePerCount; ++i)
    {

        CCtrlStaticWZ* name_control = GetStaticControlWZ(GetControlID_byPosition(kControl_Text_S007 + i));
        CCtrlStaticWZ* price_control = GetStaticControlWZ(GetControlID_byPosition(kControl_Text_S017 + i));
        CCtrlStaticWZ* desc_control = GetStaticControlWZ(GetControlID_byPosition(kControl_Text_S027 + i));
        CCtrlStaticWZ* sned_name_control = GetStaticControlWZ(GetControlID_byPosition(kControl_Text_S037 + i));
        CCtrlStaticWZ* to_inventory_control = GetStaticControlWZ(GetControlID_byPosition(kControl_Text_S047 + i));
        CCtrlImageStaticWZ* picture_control = GetImageStaticControlWZ(GetControlID_byPosition(kControl_TextImage_B001 + i));
        CCtrlButtonWZ* button_control = GetButtonControlWZ(GetControlID_byPosition(kControl_Button_B006 + i));

        if (name_control == NULL || 
            price_control == NULL || 
            desc_control == NULL || 
            sned_name_control == NULL || 
            to_inventory_control == NULL || 
            picture_control == NULL ||
            button_control == NULL) 
        {
            continue;
        }

        name_control->SetTextWZ("");
        price_control->SetTextWZ("");
        sned_name_control->SetTextWZ("");
        to_inventory_control->SetTextWZ("");
        desc_control->SetTextWZ("");
        button_control->SetUserData(NULL);
        picture_control->SetUserData(NULL);
        picture_control->SetCallbackDraw(NULL, this);

        button_control->EnableWindowWZ(false);

        if( i < Data.GetCountPerPage() )
        {
            ConvenienceStore::StorageItem* pItemData = Data.GetItemData((size_t)i);
            name_control->SetTextWZ( pItemData->GetName() );

            //155506	선물
            TCHAR msg_text[INTERFACE_STRING_LENGTH] = {NULL, };
            ConvenienceStore::GetDisplayPriceStringByProductInfo(msg_text, INTERFACE_STRING_LENGTH, &(pItemData->GetProductInfo()), false);

#ifdef _RU_0000_20120312_RUSSIA_MOD_CASH_SHOP_UI
            // no operation
#else
            time_t temp_time = pItemData->GetDeleteDate();
            tm local_tm = *(localtime(&temp_time));
            local_tm.tm_year += 1900;
            local_tm.tm_mon += 1;
            //155572	만료일: %s
            TCHAR msg_time[INTERFACE_STRING_LENGTH] = {NULL, };
            GlobalFunc::MakeMessageFromStringTable_tm(msg_time, INTERFACE_STRING_LENGTH, 155572, &local_tm);

            _tcscat(msg_text, " / ");
            _tcscat(msg_text, msg_time);
#endif //_RU_0000_20120312_RUSSIA_MOD_CASH_SHOP_UI

            price_control->SetTextWZ(msg_text);

            //5781	보낸이
            ZeroMemory(msg_text, sizeof(msg_text));
            _tcscat(msg_text, (TCHAR*)g_InterfaceManager.GetInterfaceString(5781));
            _tcscat(msg_text, _T(" : "));
            _tcscat(msg_text, pItemData->GetSendCharacterName() );
            sned_name_control->SetTextWZ(msg_text);

            //155553	인벤토리에 받은 후 사용가능
            to_inventory_control->SetTextID(155553);


            button_control->EnableWindowWZ(true);
            button_control->SetUserData(reinterpret_cast<DWORD>(pItemData));
            picture_control->SetUserData(reinterpret_cast<DWORD>(pItemData));
            picture_control->SetCallbackDraw(uiConvenienceStore_Counter::Callback_IconRender, this);
        }
    }

}

//------------------------------------------------------------------------------ 
void uiConvenienceStore_Counter::ChangeTreeObject(ConvenienceStore::Define::ShopMenuType menu_type)
{
    BASE_TreeMenuInfo::Group_Type group_type = BASE_TreeMenuInfo::kGroupType_Max;
    if (menu_type == ConvenienceStore::Define::kShopMenuType_Cash)
    {
        group_type = BASE_TreeMenuInfo::kGroupType_CShop_Cash;
    }
    else if (menu_type == ConvenienceStore::Define::kShopMenuType_PPCard)
    {
        group_type = BASE_TreeMenuInfo::kGroupType_CShop_PPCard;
    }
    else if (menu_type == ConvenienceStore::Define::kShopMenuType_Mileage)
    {
        group_type = BASE_TreeMenuInfo::kGroupType_CShop_Mileage;
    }
    else if (menu_type == ConvenienceStore::Define::kShopMenuType_Mypage)
    {
        group_type = BASE_TreeMenuInfo::kGroupType_CShop_MyPage;
    }

    tree_menu().Cleanup();
    
    bool is_res = tree_menu().ApplyTreeGroup(group_type);

    tree_menu().Fill();
    tree_menu().Select();
}
//------------------------------------------------------------------------------ 
bool uiConvenienceStore_Counter::UpdatePopupMenu(bool is_possible, bool is_now_show_tooltip)
{
    if (uiConvenienceStore_Dialog::UpdatePopupMenu(is_possible, is_now_show_tooltip) == false)
    {
        return false;
    }
    //------------------------------------------------------------------------------ 
    for (int i = 0; i < kPagePerCount; ++i)
    {  
        CControlWZ* control = GetControlWZ(GetControlID_byPosition(kControl_Button_B006 + i));
        if ((IsOverControl(control) == true) && (control->GetUserData() != NULL))
        {
            ConvenienceStore::ShopItemObject* pObject = reinterpret_cast<ConvenienceStore::ShopItemObject*>(control->GetUserData());
            if (pObject == NULL)
            {
                break;
            }

            if ( pObject->IsStorageItem() )
            {
                if (GetManager()->popup_menu_two()->IsVisible() == FALSE)
                {
                    GetManager()->popup_menu_two()->ShowInterface(TRUE);
                }

                if ( pObject->IsStorageBuy() )
                {
                    GetManager()->popup_menu_two()->Reset();
                    GetManager()->popup_menu_two()->set_owner(this, control);
                    GetManager()->popup_menu_two()->SetMenu(0, //uiConvenienceStore_PopupMenu::kShopPopup_One_Recv, 
                                                        ConvenienceStore::Define::kShopStringCode_Shop_Popup_Recv, 
                                                        true, 
                                                        uiConvenienceStore_Dialog::kPopupCommand_MyPage_Recv,
                                                        control->GetUserData());
                }
                else if ( pObject->IsStorageGift() )
                {
                    GetManager()->popup_menu_two()->Reset();
                    GetManager()->popup_menu_two()->set_owner(this, control);
                    GetManager()->popup_menu_two()->SetMenu(0, //uiConvenienceStore_PopupMenu::kShopPopup_Two_Msg, 
                                                        ConvenienceStore::Define::kShopStringCode_Shop_Popup_Msg, 
                                                        true, 
                                                        uiConvenienceStore_Dialog::kPopupCommand_MyPage_Msg,
                                                        control->GetUserData());
                    GetManager()->popup_menu_two()->SetMenu(1, //uiConvenienceStore_PopupMenu::kShopPopup_Two_Recv,
                                                        ConvenienceStore::Define::kShopStringCode_Shop_Popup_Recv, 
                                                        true, 
                                                        uiConvenienceStore_Dialog::kPopupCommand_MyPage_Recv,
                                                        control->GetUserData());
                }

                RECT menu_rect;
                GetManager()->popup_menu_two()->GetDialogWZ()->GetDlgSize(&menu_rect);

                RECT control_area = control->GetSizeRect();
                int cx = static_cast<int>(control_area.right - (menu_rect.right * 0.5f));
                int cy = static_cast<int>(control_area.top + ((control_area.bottom - control_area.top) * 0.5f) - (menu_rect.bottom * 0.5f));

                GetManager()->popup_menu_two()->MoveWindowWZ(cx, cy);
            }
            else if ( pObject->IsProduct() )
            {
                if (GetManager()->popup_menu_four()->IsVisible() == FALSE)
                {
                    GetManager()->popup_menu_four()->ShowInterface(TRUE);
                }

                ConvenienceStore::ProductItem* pProduct = static_cast<ConvenienceStore::ProductItem*>(pObject);

                bool is_possible_preview = false;
                if (pProduct->is_package() == true)
                {
                    is_possible_preview = uiConvenienceStore_Basket::HasPreviewItem(pProduct);
                }
                else if (pProduct->is_lotto() == false)
                {
                    //! 단품이닷
                    if ((pProduct->first_info()->is_etheria() == false) && 
                        (pProduct->first_info()->is_etheria_with_divine() == false))
                    {
                        is_possible_preview = uiConvenienceStore_Basket::IsPossiblePreview(pProduct->item_code(0));
                    }
                }

                GetManager()->popup_menu_four()->Reset();
                GetManager()->popup_menu_four()->set_owner(this, control);
                GetManager()->popup_menu_four()->SetMenu(0, //uiConvenienceStore_PopupMenu::kShopPopup_Four_Pickup, 
                                                    ConvenienceStore::Define::kShopStringCode_Shop_Popup_Pickup, 
                                                    pProduct->GetPrice() > 0 && pProduct->is_mileage_only() == false,   // 마일리지 구매상품은 담지않는다
                                                    uiConvenienceStore_Dialog::kPopupCommand_Counter_Pickup,
                                                    control->GetUserData());
                GetManager()->popup_menu_four()->SetMenu(1, //uiConvenienceStore_PopupMenu::kShopPopup_Four_Preview,
                                                    ConvenienceStore::Define::kShopStringCode_Shop_Popup_Preview, 
                                                    is_possible_preview, 
                                                    uiConvenienceStore_Dialog::kPopupCommand_Counter_Preview,
                                                    control->GetUserData());
                GetManager()->popup_menu_four()->SetMenu(2, //uiConvenienceStore_PopupMenu::kShopPopup_Four_Gift, 
                                                    ConvenienceStore::Define::kShopStringCode_Shop_Popup_Gift, 
                                                    pProduct->is_possible_gift(), 
                                                    uiConvenienceStore_Dialog::kPopupCommand_Counter_Gift,
                                                    control->GetUserData());
                GetManager()->popup_menu_four()->SetMenu(3, //uiConvenienceStore_PopupMenu::kShopPopup_Four_Buy, 
                                                    ConvenienceStore::Define::kShopStringCode_Shop_Popup_Buy, 
                                                    pProduct->is_possible_sales(), 
                                                    uiConvenienceStore_Dialog::kPopupCommand_Counter_Buy,
                                                    control->GetUserData());


                RECT menu_rect;
                GetManager()->popup_menu_four()->GetDialogWZ()->GetDlgSize(&menu_rect);

                RECT control_area = control->GetSizeRect();
                int cx = static_cast<int>(control_area.right - (menu_rect.right * 0.5f));
                int cy = static_cast<int>(control_area.top + ((control_area.bottom - control_area.top) * 0.5f) - (menu_rect.bottom * 0.5f));

                GetManager()->popup_menu_four()->MoveWindowWZ(cx, cy);
            }
            return true;
        }
    }
    GetManager()->popup_menu_four()->ShowInterface(FALSE);
    GetManager()->popup_menu_two()->ShowInterface(FALSE);
    //GetManager()->popup_menu_three()->ShowInterface(FALSE);

    return true;
}
//------------------------------------------------------------------------------ 
bool uiConvenienceStore_Counter::IsShowTooltip()
{
    if (GetManager()->popup_package()->IsVisible())
    {
        return true;
    }
    for (int i = 0; i < kPagePerCount; ++i)
    {  
        CCtrlImageStaticWZ* picture_control = GetImageStaticControlWZ(GetControlID_byPosition(kControl_TextImage_B001 + i));

        if ((IsOverControl(picture_control) == true) && (picture_control->GetUserData() != NULL))
        {
            return true;
        }
    }
    return false;
}
//------------------------------------------------------------------------------ 
void uiConvenienceStore_Counter::UpdateTooltip(bool is_possible)
{ 
    if (is_possible == false)
    {
        return; 
    }

    //! 묶음 상품 툴팁이 떠 있는지 확인한다.
    if (GetManager()->popup_package()->IsVisible() == TRUE)
    {
        if ((GetManager()->popup_package()->InterSectRect(Mouse::Instance()->CheckMousePos()) == TRUE) ||
            (IsOverControl(GetManager()->popup_package()->owner_control()) == true))
        {
            return;
        }
    }

    //------------------------------------------------------------------------------ 
    for (int i = 0; i < kPagePerCount; ++i)
    {  
        CCtrlImageStaticWZ* picture_control = GetImageStaticControlWZ(GetControlID_byPosition(kControl_TextImage_B001 + i));
        CControlWZ* back_button_control = GetControlWZ(GetControlID_byPosition(kControl_Button_B006 + i));

        if ((IsOverControl(picture_control) == true) && (picture_control->GetUserData() != NULL))
        {
            ConvenienceStore::ShopItemObject* pObject = reinterpret_cast<ConvenienceStore::ShopItemObject*>(picture_control->GetUserData());
            if (pObject == NULL)
            {
                break;
            } 

            if ( pObject->IsStorageItem() )
            {
                GetManager()->RegisterTooltip(pObject);
                GetManager()->popup_package()->uiConvenienceStore_PopupMenu::ShowInterface(FALSE);
            }
            else if ( pObject->IsProduct() )
            {
                ConvenienceStore::ProductItem* pProduct = static_cast<ConvenienceStore::ProductItem*>(pObject);

                if (pProduct->is_package() == true)
                {
                    GetManager()->popup_package()->ShowInterface(TRUE, pProduct);
                    GetManager()->popup_package()->Reset();
                    GetManager()->popup_package()->set_owner(this, picture_control);

                    GetManager()->popup_package()->MoveWindowWZ(back_button_control->GetSizeRect().left, 
                                                                picture_control->GetSizeRect().bottom);
                }
                else
                {
                    GetManager()->RegisterTooltip(pProduct);
                    GetManager()->popup_package()->uiConvenienceStore_PopupMenu::ShowInterface(FALSE);
                }
            }
            return;
        }
    }
    GetManager()->popup_package()->uiConvenienceStore_PopupMenu::ShowInterface(FALSE);
}
//------------------------------------------------------------------------------ 
bool uiConvenienceStore_Counter::IsPossibleShopPopup()
{
#if WZENGINEVER >= 410
    if ((g_InterfaceManager.GetDialogManager()->GetLockDlgID() != 0) && 
        (g_InterfaceManager.GetDialogManager()->GetLockDlgID() != uiConvenienceStore_Counter::kDialog_ID))
    {
        return false; 
    } 

    if (dropdown_class_filter().IsOpened() || dropdown_sorting().IsOpened())
    {
        return false;
    }

    STD_LIST::reverse_iterator rbegin = g_InterfaceManager.GetDialogManager()->GetDlgManager()->GetDrawList().rbegin();
    STD_LIST::reverse_iterator rend = g_InterfaceManager.GetDialogManager()->GetDlgManager()->GetDrawList().rend();
    for (; rbegin != rend; ++rbegin) 
    { 
        DWORD dlg_id = (*rbegin);
        if (dlg_id == uiConvenienceStore_Counter::kDialog_ID)
        {
            break;
        }

        if ((dlg_id == uiConvenienceStore_Basket::kDialog_ID) || 
            (dlg_id == uiConvenienceStore_PopupPackage::kDialog_ID) || 
            (dlg_id == uiConvenienceStore_PopupMenuFour::kDialog_ID) || 
            (dlg_id == uiConvenienceStore_PopupMenuThree::kDialog_ID) || 
            (dlg_id == uiConvenienceStore_PopupMenuFive::kDialog_ID) || 
            (dlg_id == uiConvenienceStore_PopupMenuTwo::kDialog_ID) ||
            (dlg_id == StrToWzID("0210")) ||                        //HeroGaugeDialog
            (dlg_id == StrToWzID("0230")) ||                        //QuickContainer
#ifdef _NA_000000_20130114_RENEWER_UI
#else
            (dlg_id == IM_SKILL_MANAGER::SKILL_PERK_SLOT_DLG) ||    //uiPerkSlot
#endif
            (dlg_id == IM_TARGET_MANAGER::TARGET_DIALOG) ||         //TargetDialog
            (dlg_id == IM_RADAR_MANAGER::RADAR_DIALOG) ||           //RadarDialog
            (dlg_id == IM_RADAR_MANAGER::RADAR_DIALOG_PLUS)      //RadarPlusDialog
           )
        {
            continue;
        }
        SolarDialog* dlg = g_InterfaceManager.GetDialog(dlg_id);  
        if (dlg == NULL || dlg->IsVisible() == FALSE)
        {
            continue;
        }
        return false;
    }
#endif //#if WZENGINEVER >= 410


    //BOOL is_res = FALSE;
    //if (is_res == FALSE)    is_res = g_InterfaceManager.IsHaveFocus(uiConvenienceStore_Counter::kDialog_ID);
    //if (is_res == FALSE)    is_res = g_InterfaceManager.IsHaveFocus(uiConvenienceStore_Basket::kDialog_ID);
    //if (is_res == FALSE)    is_res = g_InterfaceManager.IsHaveFocus(uiConvenienceStore_BasketMypage::kDialog_ID);
    //if (is_res == FALSE)    is_res = g_InterfaceManager.IsHaveFocus(uiConvenienceStore_BasketPPCard::kDialog_ID);
    //if (is_res == FALSE)    is_res = g_InterfaceManager.IsHaveFocus(uiConvenienceStore_BasketMileage::kDialog_ID);
    //if (is_res == FALSE)    is_res = g_InterfaceManager.IsHaveFocus(uiConvenienceStore_PopupPackage::kDialog_ID);
    //if (is_res == FALSE)    is_res = g_InterfaceManager.IsHaveFocus(uiConvenienceStore_PopupMenuFour::kDialog_ID);
    //if (is_res == FALSE)    is_res = g_InterfaceManager.IsHaveFocus(uiConvenienceStore_PopupMenuFive::kDialog_ID);
    //if (is_res == FALSE)    is_res = g_InterfaceManager.IsHaveFocus(uiConvenienceStore_PopupMenuTwo::kDialog_ID);
    //if (is_res == FALSE)    is_res = g_InterfaceManager.IsHaveFocus(StrToWzID("0210"));                       //HeroGaugeDialog
    //if (is_res == FALSE)    is_res = g_InterfaceManager.IsHaveFocus(StrToWzID("0230"));                       //QuickContainer
    //if (is_res == FALSE)    is_res = g_InterfaceManager.IsHaveFocus(IM_SKILL_MANAGER::SKILL_PERK_SLOT_DLG);   //uiPerkSlot
    //if (is_res == FALSE)    is_res = g_InterfaceManager.IsHaveFocus(IM_TARGET_MANAGER::TARGET_DIALOG);        //TargetDialog
    //if (is_res == FALSE)    is_res = g_InterfaceManager.IsHaveFocus(IM_RADAR_MANAGER::RADAR_DIALOG);          //RadarDialog
    //if (is_res == FALSE)    is_res = g_InterfaceManager.IsHaveFocus(IM_RADAR_MANAGER::RADAR_DIALOG_PLUS);      //RadarPlusDialog

    //return (is_res == TRUE);

    return true;
}
//------------------------------------------------------------------------------ 
bool uiConvenienceStore_Counter::OnClickAskDialog(uiConvenienceStore_Dialog::AskCommandType command_type, uiConvenienceStore_Dialog::AskShowType show_type, void* user_data)
{
    if (command_type == uiConvenienceStore_Dialog::kAskCommand_OK)
    {
        if (CurrentShopMenuType() == ConvenienceStore::Define::kShopMenuType_Mypage)
        {
            if (show_type == uiConvenienceStore_Dialog::kAskShow_StorageBuy_AllRecv )
            {
                is_recv_all_storage_ = true;
                ConvenienceStore::CashShop::instance()->ReceiveStorageItem( true, true );

                // 우선 아이템 하나 받기 요청
                ConvenienceStore::CashShop::instance()->SendUseageStorageBuyItemOnce();

                GetManager()->ask()->OpenAsk(this, uiConvenienceStore_Dialog::kAskShow_StorageGift_AllRecv_Confirm);
                GetManager()->ask()->AppendFText(g_InterfaceManager.GetInterfaceString( 155568 ));
                GetManager()->ask()->AppendFText(_T("---------------------------"));
            }
            else if(show_type == uiConvenienceStore_Dialog::kAskShow_StorageGift_AllRecv )
            {
                is_recv_all_storage_ = true;
                ConvenienceStore::CashShop::instance()->ReceiveStorageItem( true, false );


                ConvenienceStore::CashShop::instance()->SendUseageStorageGiftItemOnce();

                GetManager()->ask()->OpenAsk(this, uiConvenienceStore_Dialog::kAskShow_StorageGift_AllRecv_Confirm);
                GetManager()->ask()->AppendFText(g_InterfaceManager.GetInterfaceString( 155568 ));
                GetManager()->ask()->AppendFText(_T("---------------------------"));
            }
            else if(show_type == uiConvenienceStore_Dialog::kAskShow_StorageBuy_OneRecv )
            {
                is_recv_all_storage_ = false;
                ConvenienceStore::CashShop::instance()->ReceiveStorageItem( false, true );


                ConvenienceStore::StorageItem* pObject = reinterpret_cast<ConvenienceStore::StorageItem*>(user_data);
                ConvenienceStore::CashShop::instance()->SendUseageStorageBuyItemOnce(pObject);

                GetManager()->ask()->OpenAsk(this, uiConvenienceStore_Dialog::kAskShow_StorageGift_AllRecv_Confirm);
                GetManager()->ask()->AppendFText(g_InterfaceManager.GetInterfaceString( 155568 ));
                GetManager()->ask()->AppendFText(_T("---------------------------"));
            }
            else if(show_type == uiConvenienceStore_Dialog::kAskShow_StorageGift_OneRecv )
            {
                is_recv_all_storage_ = false;

                ConvenienceStore::CashShop::instance()->ReceiveStorageItem( false, false );


                ConvenienceStore::StorageItem* pObject = reinterpret_cast<ConvenienceStore::StorageItem*>(user_data);
                ConvenienceStore::CashShop::instance()->SendUseageStorageGiftItemOnce(pObject);

                GetManager()->ask()->OpenAsk(this, uiConvenienceStore_Dialog::kAskShow_StorageGift_AllRecv_Confirm);
                GetManager()->ask()->AppendFText(g_InterfaceManager.GetInterfaceString( 155568 ));
                GetManager()->ask()->AppendFText(_T("---------------------------"));
            }
#ifdef _NA_000000_20130710_CHARGE_SUB_FIXED_AMOUNT_EX2
            else if(show_type == uiConvenienceStore_Dialog::KAskShow_ItemCashNewType )
            {
                ConvenienceStore::ShopItemObject* pObject = GetManager()->ask()->GetNowAskItem();
                GetManager()->ask()->OpenAsk(this, uiConvenienceStore_Dialog::kAskShow_StorageBuy_OneRecv );
                GetManager()->ask()->AppendFText( g_InterfaceManager.GetInterfaceString( 155542 ) );
                GetManager()->ask()->AppendFText( g_InterfaceManager.GetInterfaceString( 155543 ) );
                GetManager()->ask()->AppendFText( g_InterfaceManager.GetInterfaceString( 155570 ) );

                GetManager()->ask()->AppendItemText( pObject );
                GetManager()->ask()->SetItemObject( pObject );

            }
            else if(show_type == uiConvenienceStore_Dialog::KAskShow_ItemCashGifeNewType )
            {
                //155545	아래 아이템을 받은 선물함에서 꺼내시겠습니까?
                //155546	선물함에 보관중인 모든 아이템은\n반송 및 구입 취소가 불가능 합니다.
                //155570	<font=tt12><color=ffffffff>-----------------------------------------------</color></font>
                ConvenienceStore::ShopItemObject* pObject = GetManager()->ask()->GetNowAskItem();
                GetManager()->ask()->OpenAsk(this, uiConvenienceStore_Dialog::kAskShow_StorageGift_OneRecv );
                GetManager()->ask()->AppendFText( g_InterfaceManager.GetInterfaceString( 155545 ) );
                GetManager()->ask()->AppendFText( g_InterfaceManager.GetInterfaceString( 155546 ) );
                GetManager()->ask()->AppendFText( g_InterfaceManager.GetInterfaceString( 155570 ) );

                GetManager()->ask()->AppendItemText( pObject );
                GetManager()->ask()->SetItemObject( pObject );
            }
            else if(show_type == uiConvenienceStore_Dialog::kAskShow_StorageNewTypeBuy_AllRecv )
            {
                GetManager()->ask()->OpenAsk( this, uiConvenienceStore_Dialog::kAskShow_StorageBuy_AllRecv );
                GetManager()->ask()->AppendFText( g_InterfaceManager.GetInterfaceString(155542) );
                GetManager()->ask()->AppendFText( g_InterfaceManager.GetInterfaceString(155543) );
                GetManager()->ask()->AppendFText( g_InterfaceManager.GetInterfaceString(155570) );

                ConvenienceStore::StorageItemList& StorageData = ConvenienceStore::CashShop::instance()->GetStorageItemData();
                for( size_t i=0;i!=StorageData.GetCountPerPage();++i)
                {
                    GetManager()->ask()->AppendItemText( StorageData.GetItemData(i) );
                }
            }
            else if(show_type == uiConvenienceStore_Dialog::kAskShow_StorageNewTypeGift_AllRecv )
            {
                //155542	아래 아이템을 보관함에서 꺼내시겠습니까?
                //155543	보관함에서 꺼낸 아이템은 인벤토리에서\n확인이 가능하며, 인벤토리로 이동된 모든\n아이템들은 구입 취소가 불가능합니다.\n(단, 복권류 아이템은 무조건 취소불가)
                //155570	<font=tt12><color=ffffffff>-----------------------------------------------</color></font>
                GetManager()->ask()->OpenAsk( this, uiConvenienceStore_Dialog::kAskShow_StorageGift_AllRecv );
                GetManager()->ask()->AppendFText( g_InterfaceManager.GetInterfaceString(155542) );
                GetManager()->ask()->AppendFText( g_InterfaceManager.GetInterfaceString(155543) );
                GetManager()->ask()->AppendFText( g_InterfaceManager.GetInterfaceString(155570) );

                ConvenienceStore::StorageItemList& StorageData = ConvenienceStore::CashShop::instance()->GetGiftStorageItemData();
                for( size_t i=0;i!=StorageData.GetCountPerPage();++i)
                {
                    GetManager()->ask()->AppendItemText( StorageData.GetItemData(i) );
                }
            }
#endif
        }
        return true;
    }

    return true;
}

//------------------------------------------------------------------------------ 
ConvenienceStore::Define::MyPageType uiConvenienceStore_Counter::CurrentMyPageType()
{
    if (CurrentShopMenuType() == ConvenienceStore::Define::kShopMenuType_Mypage)
    {
        TreeMenuGroup::TreeObject* tree_object = tree_menu().CurrectTreeObject();
        if ((tree_object != NULL) && (tree_object->info != NULL))
        {
            return static_cast<ConvenienceStore::Define::MyPageType>(tree_object->info->user_data);
        }
    }
    return ConvenienceStore::Define::kMyPageType_Size;
}
//------------------------------------------------------------------------------ 
ConvenienceStore::Define::ShopMenuType uiConvenienceStore_Counter::CurrentShopMenuType()
{
    return static_cast<ConvenienceStore::Define::ShopMenuType>(menu_tab().CurrentUserData());
}
