#include "SunClientPrecompiledHeader.h"

#include "uiConvenienceStoreMan.h"
#include "uiConvenienceStore_Counter.h"
#include "uiConvenienceStore_Basket.h"
#include "uiConvenienceStore_PopupMenuFour.h"
#include "uiConvenienceStore_PopupMenuThree.h"
#include "uiConvenienceStore_PopupMenuTwo.h"
#include "uiConvenienceStore_PopupMenuFive.h"
#include "uiConvenienceStore_Purchase.h"
#include "uiConvenienceStore_Confirm.h"
#include "uiConvenienceStore_Gift.h"
#include "uiConvenienceStore_FriendList.h"
#include "uiConvenienceStore_PopupPackage.h"
#include "uiConvenienceStore_ItemDetail.h"
#include "uiConvenienceStore_Ask.h"
#include "uiConvenienceStore_CashType.h"
#include "ShopDefine.h"
#include "GlobalFunc.h"
#include "ItemManager.h"
#include "uiToolTipMan\uiToolTipMan.h"
#include "GameFramework.h"
#include "Hero.h"
#include "HeroEquipmentContainer.h"
#include "PacketRequestManager.h"
#include "PurchaseItem.h"
#include "ProductInfo.h"
#include "StorageItem.h"

//------------------------------------------------------------------------------ 
uiConvenienceStoreMan::uiConvenienceStoreMan(InterfaceManager* manager) : 
    uiBaseMan(manager)
{
}
//------------------------------------------------------------------------------ 
void uiConvenienceStoreMan::OnInitialize()
{
#ifdef _NA_0_20110613_GAME_SHOP_RENEWAL
    CreateUIDialog<uiConvenienceStore_Counter>(uiConvenienceStore_Counter::kDialog_ID, 
                                            "Data\\Interface\\47_2_Cash_itemshop.iwz", this, FALSE);
    CreateUIDialog<uiConvenienceStore_PopupMenuFour>(uiConvenienceStore_PopupMenuFour::kDialog_ID, 
                                            "Data\\Interface\\47_4_Button_list01.iwz", this, FALSE);
    CreateUIDialog<uiConvenienceStore_PopupMenuThree>(uiConvenienceStore_PopupMenuThree::kDialog_ID, 
                                            "Data\\Interface\\47_13_Button_list03.iwz", this, FALSE);
    CreateUIDialog<uiConvenienceStore_PopupMenuTwo>(uiConvenienceStore_PopupMenuTwo::kDialog_ID, 
                                            "Data\\Interface\\47_5_Button_list02.iwz", this, FALSE);
    CreateUIDialog<uiConvenienceStore_PopupMenuFive>(uiConvenienceStore_PopupMenuFive::kDialog_ID, 
                                            "Data\\Interface\\47_12_Button_list03.iwz", this, FALSE);

    CreateUIDialog<uiConvenienceStore_Purchase>(uiConvenienceStore_Purchase::kDialog_ID, 
                                            "Data\\Interface\\47_6_Purchase_popup01.iwz", this, TRUE);

    CreateUIDialog<uiConvenienceStore_Confirm>(uiConvenienceStore_Confirm::kDialog_ID, 
                                            "Data\\Interface\\47_7_Purchase_popup02.iwz", this, TRUE);

    CreateUIDialog<uiConvenienceStore_Gift>(uiConvenienceStore_Gift::kDialog_ID, 
                                            "Data\\Interface\\47_8_Gift_popup.iwz", this, FALSE);

    CreateUIDialog<uiConvenienceStore_FriendList>(uiConvenienceStore_FriendList::kDialog_ID, 
                                            "Data\\Interface\\47_9_List_popup.iwz", this, FALSE);

    CreateUIDialog<uiConvenienceStore_PopupPackage>(uiConvenienceStore_PopupPackage::kDialog_ID, 
                                            "Data\\Interface\\47_10_Package_List.iwz", this, FALSE);
    

    CreateUIDialog<uiConvenienceStore_ItemDetail>(uiConvenienceStore_ItemDetail::kDialog_ID, 
                                            "Data\\Interface\\47_11_Gift_accept.iwz", this, FALSE);

    CreateUIDialog<uiConvenienceStore_Ask>(uiConvenienceStore_Ask::kDialog_ID, 
                                            "Data\\Interface\\82_7_Asking_Window.iwz", this, FALSE);

    CreateUIDialog<uiConvenienceStore_CashType>(uiConvenienceStore_CashType::kDialog_ID, 
                                            "Data\\Interface\\47_14_Cash_purchase.iwz", this, FALSE);

    //------------------------------------------------------------------------------ 
    CreateUIDialog<uiConvenienceStore_Basket>(uiConvenienceStore_Basket::kDialog_ID, 
                                                "Data\\Interface\\47_3_Cash_itemlist.iwz", this, TRUE, TRUE);

#endif //_NA_0_20110613_GAME_SHOP_RENEWAL
}
//------------------------------------------------------------------------------ 
uiConvenienceStore_Basket* uiConvenienceStoreMan::basket()
{
    return GET_CAST_DIALOG(uiConvenienceStore_Basket, uiConvenienceStore_Basket::kDialog_ID);
}
//------------------------------------------------------------------------------ 
uiConvenienceStore_Counter* uiConvenienceStoreMan::counter()
{
    return GET_CAST_DIALOG(uiConvenienceStore_Counter, uiConvenienceStore_Counter::kDialog_ID);
}
//------------------------------------------------------------------------------ 
bool uiConvenienceStoreMan::is_show_popup_menu()
{
    if (popup_menu_two() != NULL && popup_menu_two()->IsVisible())   return true;
    if (popup_menu_three() != NULL && popup_menu_three()->IsVisible()) return true;
    if (popup_menu_four() != NULL && popup_menu_four()->IsVisible()) return true;
    if (popup_menu_five() != NULL && popup_menu_five()->IsVisible()) return true;

    return false;
}
//------------------------------------------------------------------------------ 
uiConvenienceStore_PopupMenu* uiConvenienceStoreMan::popup_menu(ConvenienceStore::Define::PopupMenuKind e)
{
    switch (e)
    {
        case ConvenienceStore::Define::kPopupMenuKind_Two:        return popup_menu_two();
        case ConvenienceStore::Define::kPopupMenuKind_Three:      return popup_menu_three();
        case ConvenienceStore::Define::kPopupMenuKind_Four:       return popup_menu_four();
        case ConvenienceStore::Define::kPopupMenuKind_Five:       return popup_menu_five();
        //case kPopupMenuKind_Package:    return popup_package();
    }
    return NULL;
}
//------------------------------------------------------------------------------ 
uiConvenienceStore_PopupMenuFive* uiConvenienceStoreMan::popup_menu_five()
{
    return GET_CAST_DIALOG(uiConvenienceStore_PopupMenuFive, uiConvenienceStore_PopupMenuFive::kDialog_ID);
}
//------------------------------------------------------------------------------ 
uiConvenienceStore_PopupMenuFour* uiConvenienceStoreMan::popup_menu_four()
{
    return GET_CAST_DIALOG(uiConvenienceStore_PopupMenuFour, uiConvenienceStore_PopupMenuFour::kDialog_ID);
}
//------------------------------------------------------------------------------ 
uiConvenienceStore_PopupMenuThree* uiConvenienceStoreMan::popup_menu_three()
{
    return GET_CAST_DIALOG(uiConvenienceStore_PopupMenuThree, uiConvenienceStore_PopupMenuThree::kDialog_ID);
}
//------------------------------------------------------------------------------ 
uiConvenienceStore_PopupMenuTwo* uiConvenienceStoreMan::popup_menu_two()
{
    return GET_CAST_DIALOG(uiConvenienceStore_PopupMenuTwo, uiConvenienceStore_PopupMenuTwo::kDialog_ID);
}
//------------------------------------------------------------------------------ 
uiConvenienceStore_Confirm* uiConvenienceStoreMan::confirm()
{
    return GET_CAST_DIALOG(uiConvenienceStore_Confirm, uiConvenienceStore_Confirm::kDialog_ID);
}
//------------------------------------------------------------------------------ 
uiConvenienceStore_Purchase* uiConvenienceStoreMan::purchase()
{
    return GET_CAST_DIALOG(uiConvenienceStore_Purchase, uiConvenienceStore_Purchase::kDialog_ID);
}
//------------------------------------------------------------------------------ 
uiConvenienceStore_Gift* uiConvenienceStoreMan::gift()
{
    return GET_CAST_DIALOG(uiConvenienceStore_Gift, uiConvenienceStore_Gift::kDialog_ID);
}
//------------------------------------------------------------------------------ 
uiConvenienceStore_FriendList* uiConvenienceStoreMan::friend_list()
{
    return GET_CAST_DIALOG(uiConvenienceStore_FriendList, uiConvenienceStore_FriendList::kDialog_ID);
}
//------------------------------------------------------------------------------ 
uiConvenienceStore_PopupPackage* uiConvenienceStoreMan::popup_package()
{
    return GET_CAST_DIALOG(uiConvenienceStore_PopupPackage, uiConvenienceStore_PopupPackage::kDialog_ID);
}
//------------------------------------------------------------------------------ 
uiConvenienceStore_ItemDetail* uiConvenienceStoreMan::item_detail()
{
    return GET_CAST_DIALOG(uiConvenienceStore_ItemDetail, uiConvenienceStore_ItemDetail::kDialog_ID);
}
//------------------------------------------------------------------------------ 
uiConvenienceStore_Ask* uiConvenienceStoreMan::ask()
{
    return GET_CAST_DIALOG(uiConvenienceStore_Ask, uiConvenienceStore_Ask::kDialog_ID);
}
//------------------------------------------------------------------------------ 
uiConvenienceStore_CashType* uiConvenienceStoreMan::cashtype()
{
    return GET_CAST_DIALOG(uiConvenienceStore_CashType, uiConvenienceStore_CashType::kDialog_ID);
}
//------------------------------------------------------------------------------ 
void uiConvenienceStoreMan::Fill_TreeMenuInfo_SubCategory(BASE_TreeMenuInfo::Group_Type group_type, 
                                  CShopList* shop_list, 
                                  CShopCategory* category, 
                                  BYTE menu0,
                                  BYTE menu1,
                                  BYTE menu2,
                                  BYTE menu3,
                                  int deapth)
{
    if (category == NULL)
    {
        return;
    }

    bool is_res = false;
    BASE_TreeMenuInfo tree_menu_info;
    if (deapth == 0)
    {
        // 루터 카테고리
        is_res = TreeMenuInfoParser::Instance()->GetFirstTreeMenu(OUT tree_menu_info, BASE_TreeMenuInfo::kGroupType_CShop_Default_Root);
    }
    else
    {
        is_res = TreeMenuInfoParser::Instance()->GetFirstTreeMenu(OUT tree_menu_info, BASE_TreeMenuInfo::kGroupType_CShop_Default_Subject);
    }
    if (is_res == true)
    {
        tree_menu_info.tree_group = group_type;
        tree_menu_info.menu_depth[3] = menu0;
        tree_menu_info.menu_depth[2] = menu1;
        tree_menu_info.menu_depth[1] = menu2;
        tree_menu_info.menu_depth[0] = menu3;

        if (deapth == 0)
        {
            tree_menu_info.menu_type = BASE_TreeMenuInfo::kMenuType_Group;
        }
        else
        {
            tree_menu_info.menu_type = BASE_TreeMenuInfo::kMenuType_CheckButton;
        }

        tree_menu_info.menu_depth_width = deapth * 20;
        tree_menu_info.string_code = 0;   //< 확인
        StrnCopy(tree_menu_info.string_name, category->CategroyName, BASE_TreeMenuInfo::kNameLength);
        tree_menu_info.user_data = category->ProductDisplaySeq;    //reinterpret_cast<DWORD>(&category);

        TreeMenuInfoParser::Instance()->Insert(tree_menu_info);
    }

    int child_deapth = deapth + 1;
    if (child_deapth == 0) ++menu0;
    else if (child_deapth == 1) ++menu1;
    else if (child_deapth == 2) ++menu2;
    else if (child_deapth == 3) ++menu3;

    int sub_category_seq = 0; 
    category->SetChildCategorySeqFirst();
    while (category->GetChildCategorySeqNext(OUT sub_category_seq) == true)
    {
        CShopCategory* subcategory = NULL;
        if (shop_list->GetCategoryListPtr()->GetValueByKey(sub_category_seq, subcategory))
        {
            Fill_TreeMenuInfo_SubCategory(group_type, shop_list, subcategory, menu0, menu1, menu2, menu3, child_deapth);
        }
    }
}

//------------------------------------------------------------------------------ 
void uiConvenienceStoreMan::Fill_TreeMenuInfo_RootCategory(BASE_TreeMenuInfo::Group_Type group_type, 
                                                           CShopList* shop_list,
                                                           CShopCategory* shop_category, 
                                                           int& menu0,
                                                           bool is_make_all_item)
{

#ifdef _NA_20120313_CASHSHOP_DISABLE_ALLITEM
    // 모든아이템은 표시하지 않는다
    is_make_all_item  = false;
#endif

    BASE_TreeMenuInfo root_menu_info;
    if (is_make_all_item && 
        (TreeMenuInfoParser::Instance()->GetFirstTreeMenu(OUT root_menu_info, BASE_TreeMenuInfo::kGroupType_CShop_Default_Root) == true))
    {
        root_menu_info.tree_group = group_type;
        root_menu_info.menu_depth[3] = menu0;
        root_menu_info.menu_depth[2] = 0;
        root_menu_info.menu_depth[1] = 0;
        root_menu_info.menu_depth[0] = 0;
        root_menu_info.menu_type = BASE_TreeMenuInfo::kMenuType_Group;
        root_menu_info.menu_depth_width = 0;
        root_menu_info.string_code = 155008;   //< 모든아이템
        root_menu_info.user_data = shop_category->ProductDisplaySeq; //reinterpret_cast<DWORD>(&shop_category);
        root_menu_info.user_parameter = 0;
        TreeMenuInfoParser::Instance()->Insert(root_menu_info);

        std::set<int> image_tag_vector;
        image_tag_vector.clear();
        ConvenienceStore::GetImageTag(shop_category, image_tag_vector);

        int sub_menu_depth = 0;
        int sub_menu_string_code = 0;
        int image_tag_type = 0;
        std::set<int>::iterator begin = image_tag_vector.begin();
        std::set<int>::iterator end = image_tag_vector.end();
        for (; begin != end; ++begin, ++sub_menu_depth)
        {
            //3.30.	이미지태그 유형(ImageTagType)
            //1	680	NEW
            //2	681	HOT
            //3	682	SALE
            //4	683	EVENT
            image_tag_type = (*begin);
            switch (image_tag_type)
            {
            case 680: sub_menu_string_code = 155009; break; //155009	신규상품
            case 681: sub_menu_string_code = 155010; break; //155010	인기상품
            case 682: sub_menu_string_code = 155011; break; //155011	세일
            case 683: sub_menu_string_code = 155012; break; //155012	이벤트
            default: continue; break;
            }

            //! 하위 카테고리
            TreeMenuInfoParser::Instance()->GetFirstTreeMenu(OUT root_menu_info, BASE_TreeMenuInfo::kGroupType_CShop_Default_Subject);

            root_menu_info.menu_depth[3] = 1;
            root_menu_info.menu_depth[2] = 1 + sub_menu_depth;
            root_menu_info.menu_depth[1] = 0;
            root_menu_info.menu_depth[0] = 0;
            root_menu_info.menu_type = BASE_TreeMenuInfo::kMenuType_CheckButton;
            root_menu_info.menu_depth_width = 20;
            root_menu_info.string_code = sub_menu_string_code; 
            root_menu_info.user_data = shop_category->ProductDisplaySeq;
            root_menu_info.user_parameter = image_tag_type;
            TreeMenuInfoParser::Instance()->Insert(root_menu_info);
        }
    }

    int sub_category_seq = 0; 
    shop_category->SetChildCategorySeqFirst();
    while (shop_category->GetChildCategorySeqNext(OUT sub_category_seq))
    {
        CShopCategory* shop_sub_category = NULL;
        if (shop_list->GetCategoryListPtr()->GetValueByKey(sub_category_seq, shop_sub_category))
        {
            ++menu0;
            Fill_TreeMenuInfo_SubCategory(group_type, 
                                shop_list, 
                                shop_sub_category, 
                                menu0, //root_menu_info.menu_depth[3] + 1,
                                0, //root_menu_info.menu_depth[2],
                                0, //root_menu_info.menu_depth[1],
                                0, //root_menu_info.menu_depth[0],
                                0);
        }
    }
}
//------------------------------------------------------------------------------ 
void uiConvenienceStoreMan::Fill_TreeMenuInfo_Counter(CShopList* shop_list)
{
    shop_list->GetCategoryListPtr()->SetFirst();

    int index = 0;
    int menu0 = 1;
    CShopCategory* shop_category = NULL;
    while (shop_list->GetCategoryListPtr()->GetNext(shop_category))
    {
        if (0 == shop_category->Root) 
        {
            continue; 
        }

        if (index == 0)
        {
            Fill_TreeMenuInfo_RootCategory(BASE_TreeMenuInfo::kGroupType_CShop_Cash, shop_list, shop_category, menu0, true);
        }
        else //if (index == 1)
        {
            Fill_TreeMenuInfo_RootCategory(BASE_TreeMenuInfo::kGroupType_CShop_Cash, shop_list, shop_category, menu0, false);
        }
        ++index; 
    }
}
//------------------------------------------------------------------------------ 
void uiConvenienceStoreMan::Fill_TreeMenuInfo_MyPage()
{
    BASE_TreeMenuInfo root_menu_info;  
    if (TreeMenuInfoParser::Instance()->GetFirstTreeMenu(OUT root_menu_info, BASE_TreeMenuInfo::kGroupType_CShop_Default_Root) == true)
    {
        // 1.
        root_menu_info.tree_group = BASE_TreeMenuInfo::kGroupType_CShop_MyPage;
        root_menu_info.menu_depth[3] = 1;
        root_menu_info.menu_depth[2] = 0;
        root_menu_info.menu_depth[1] = 0;
        root_menu_info.menu_depth[0] = 0;
        root_menu_info.menu_type = BASE_TreeMenuInfo::kMenuType_None;
        root_menu_info.menu_depth_width = 0;
        root_menu_info.string_code = 155552;   //< 155552	구매한 선물함
        root_menu_info.user_data = ConvenienceStore::Define::kMyPageType_BuyBox;
        root_menu_info.user_parameter = 0;
        //root_menu_info.image_code[BASE_TreeMenuInfo::kIconType_Collpase] = 10000;
        //root_menu_info.image_code[BASE_TreeMenuInfo::kIconType_Unfold] = 10001;
        root_menu_info.image_code[BASE_TreeMenuInfo::kIconType_Enable] = 10001;

        TreeMenuInfoParser::Instance()->Insert(root_menu_info);

        // 2.
        root_menu_info.menu_depth[3] = 2;
        root_menu_info.string_code = 155140;   //< 155140	받은 선물함
        root_menu_info.user_data = ConvenienceStore::Define::kMyPageType_RecvBox;
        TreeMenuInfoParser::Instance()->Insert(root_menu_info);
    }
}



void uiConvenienceStoreMan::OnStartReceiveStorageItem( bool bAllReceive, bool bIsMyItem )
{
    if( bIsMyItem == true )
    {
        ask()->OpenAsk( counter(), uiConvenienceStore_Dialog::kAskShow_StorageBuy_AllRecv_Confirm );
        ask()->AppendFText(g_InterfaceManager.GetInterfaceString( 155568 ));
        ask()->AppendFText(_T("---------------------------"));
    }
    else 
    {
        ask()->OpenAsk( counter(), uiConvenienceStore_Dialog::kAskShow_StorageGift_AllRecv_Confirm );
        ask()->AppendFText(g_InterfaceManager.GetInterfaceString( 155568 ));
        ask()->AppendFText(_T("---------------------------"));
    }
}

void uiConvenienceStoreMan::OnBeginWaiting()
{
    // 155577	서버의 응답을 기다리고 있습니다
    ask()->OpenAsk( counter(), uiConvenienceStore_Dialog::kAskShow_Message );
    ask()->AppendFText( g_InterfaceManager.GetInterfaceString(155577) );
    ask()->DisableCloseButton();
}

void uiConvenienceStoreMan::OnEndWaiting()
{
    ask()->uiBase::ShowInterface( FALSE );
}

void uiConvenienceStoreMan::OnReceveStorageItem( LPCTSTR pMessage, bool bIsSuccess, bool bIsAllReceive, bool bIsMyItemReceive, bool bIsContinue )
{
    if( bIsSuccess == true )
    {
        counter()->UpdatePage();

        // 아이템 이름 추가
        ask()->AppendFText( pMessage ); 

        if( bIsContinue == false )
        {
            //155595	아이템 받기가 진행 중 입니다.
            //155596	아이템 받기가 완료 되었습니다.
            //155568	아이템 모두 받기가 진행 중 입니다.
            //155569	아이템 모두 받기가 완료 되었습니다.
            ask()->AppendFText(_T("---------------------------"));
            ask()->AppendFText(g_InterfaceManager.GetInterfaceString( bIsAllReceive ? 155569 : 155596));

            //다 받았으니 닫기버튼을 활성화
            ask()->EnableCloseButton();
        }
    }
    else
    {
        // 오류 코드 추가
        ask()->AppendFText( pMessage ); 

        //155596 아이템 받기가 완료 되었습니다.
        //155597 아이템 받기가 중단 되었습니다.
        ask()->AppendFText(_T("---------------------------"));
        ask()->AppendFText(g_InterfaceManager.GetInterfaceString(155597));

        ask()->EnableCloseButton();
    }
}

void uiConvenienceStoreMan::OnResultNameCheck( bool bSuccess, LPCTSTR name, LEVELTYPE level, LPCTSTR guild_name )
{
    if( bSuccess == true )
    {
        gift()->CheckOk( name, level, guild_name );
    }
    else
    {
        gift()->CheckFail();
    }
}

//------------------------------------------------------------------------------ 
void uiConvenienceStoreMan::NetworkProc(MSG_BASE* pk)
{
#ifdef _NA_0_20110613_GAME_SHOP_RENEWAL

    ConvenienceStore::CashShop::NetworkProc(pk);

#endif //_NA_0_20110613_GAME_SHOP_RENEWAL
}

void uiConvenienceStoreMan::RegisterTooltip(ConvenienceStore::ShopItemObject* pObject)
{
    if( pObject->IsStorageItem() )
    {
        ConvenienceStore::StorageItem* pItemData = (ConvenienceStore::StorageItem*)pObject;
        RegisterTooltip( &(pItemData->GetProductInfo()), 0 );
    }
    else if( pObject->IsProduct() )
    {
        RegisterTooltip( (ConvenienceStore::ProductItem*)pObject );
    }
}


//------------------------------------------------------------------------------ 
void uiConvenienceStoreMan::RegisterTooltip(ConvenienceStore::ProductInfo* product_info, DWORD64 wztime64)
{
    if (product_info == NULL)
    {
        return;
    }
    uiToolTipMan* man = (uiToolTipMan*)g_InterfaceManager.GetUserInterfaceManager(UIMAN_TOOLTIP);
    if (man == NULL)
    {
        return;
    }

    man->RegisterTooltipItemForCashShop(product_info->GetItemInfo()->m_Code, 
                                        0, 
                                        wztime64, 
                                        product_info->GetInchantProperty().GetValue(),
                                        product_info->GetSellProperty().GetValue());
    man->SetTooltipDialogID(0);
}
//------------------------------------------------------------------------------ 
void uiConvenienceStoreMan::RegisterTooltip(ConvenienceStore::ProductItem* product)
{
    if (product == NULL)
    {
        return;
    }

    DWORD64 wztime64 = 0;
    util::GetDateTime_YYYYMMDDHHMMSS(product->GetShopPackage()->EndDate.tm_year + 1900,
                                    product->GetShopPackage()->EndDate.tm_mon + 1, 
                                    product->GetShopPackage()->EndDate.tm_mday, 
                                    product->GetShopPackage()->EndDate.tm_hour,
                                    product->GetShopPackage()->EndDate.tm_min,
                                    product->GetShopPackage()->EndDate.tm_sec, 
                                    OUT wztime64);

    if (product->is_package())
    {
    }
    else
    {
        if (product->is_lotto())
        {
            uiToolTipMan* man = (uiToolTipMan*)g_InterfaceManager.GetUserInterfaceManager(UIMAN_TOOLTIP);
            if (man != NULL)
            {
                man->RegisterTooltipItemForCashShopForLotto(product->GetName(),
                                                product->GetDescription(),
                                                product->GetCautionText(),
                                                wztime64, 
                                                true);
                man->SetTooltipDialogID(0);
            }
        }
        else
        {
            RegisterTooltip(product->first_info(), wztime64);
        }
    }
}
//------------------------------------------------------------------------------ 
void uiConvenienceStoreMan::RegisterTooltip(SLOTCODE item_code)
{
    uiToolTipMan* man = (uiToolTipMan*)g_InterfaceManager.GetUserInterfaceManager(UIMAN_TOOLTIP);
    if (man != NULL)
    {
        man->RegisterTooltipItemForCashShop(item_code, 0, 0, 
            0,  //m_ItemCodeForTooltip.byEnchant, 
            0); //m_ItemCodeForTooltip.byRank);
        man->SetTooltipDialogID(0);
    }
}

bool uiConvenienceStoreMan::IsShowShop()
{
    return counter()->IsVisible() != FALSE;
}

void _OnKeyInputEscape(DWORD wParam,DWORD lParam)
{
    ConvenienceStore::CashShop::instance()->HideCashShop();
}

void uiConvenienceStoreMan::OnShowCashShop()
{

    //------------------------------------------------------------------------------ 
    keyMsg msg;
    ZeroMemory(&msg,sizeof(msg));
    msg.dwType = uiConvenienceStore_Counter::kDialog_ID;
    msg.wParam = uiConvenienceStore_Counter::kDialog_ID;
    msg.DoSomething = _OnKeyInputEscape;
    g_KeyQueueManager.PushBack(msg);
    //------------------------------------------------------------------------------ 

    counter()->PlayOpenDialogSound();
    counter()->ShowInterface( TRUE );

    if( is_open_with_gift_menu_ == true )
    {
        counter()->ChangeTabMenu(ConvenienceStore::Define::kShopMenuType_Mypage, true);
        is_open_with_gift_menu_ = false;
    }
    else
    {
        counter()->ChangeTabMenu(ConvenienceStore::Define::kShopMenuType_Cash, false);
    }

    basket()->ShowInterface(TRUE);
}

void uiConvenienceStoreMan::OnHideCashShop()
{
    g_KeyQueueManager.DeleteMsg(uiConvenienceStore_Counter::kDialog_ID);
    g_InterfaceManager.UnLockDlgs();


    popup_menu_five()->uiConvenienceStore_PopupMenu::ShowInterface(FALSE);
    popup_menu_three()->uiConvenienceStore_PopupMenu::ShowInterface(FALSE);
    popup_menu_four()->uiConvenienceStore_PopupMenu::ShowInterface(FALSE);
    popup_menu_two()->uiConvenienceStore_PopupMenu::ShowInterface(FALSE);
    popup_package()->uiConvenienceStore_PopupMenu::ShowInterface(FALSE);
    purchase()->uiConvenienceStore_Dialog::ShowInterface(FALSE);
    confirm()->uiConvenienceStore_Dialog::ShowInterface(FALSE);
    gift()->uiConvenienceStore_Dialog::ShowInterface(FALSE);
    friend_list()->ShowInterface(FALSE);
    item_detail()->uiConvenienceStore_Dialog::ShowInterface(FALSE);
    ask()->uiBase::ShowInterface(FALSE);

    counter()->PlayCloseDialogSound();
    counter()->ShowInterface(FALSE);
    basket()->ShowInterface(FALSE);
}

void uiConvenienceStoreMan::OnUpdateCash()
{
    basket()->UpdateCashInfo();
    confirm()->UpdateCashInfo();
}

void uiConvenienceStoreMan::OnUpdateStorageBuyList()
{
    if (counter()->IsMyStoragePage() )
        counter()->UpdatePage();
}

void uiConvenienceStoreMan::OnUpdateStorageGiftList()
{
    if (counter()->IsGiftStoragePage() )
        counter()->UpdatePage();
}

void uiConvenienceStoreMan::OnSuccessBuyItem( ShopItemInfoEx* pItemInfo, uint ItemCount )
{
    confirm()->ShowInterface(true, uiConvenienceStore_Purchase::kPurchase_Buy, pItemInfo, ItemCount );

    counter()->ChangeTabMenu( ConvenienceStore::Define::kShopMenuType_Mypage, false );
}

void uiConvenienceStoreMan::OnSuccessGiftItem( ShopItemInfoEx* pItemInfo, uint ItemCount )
{
    confirm()->ShowInterface(true, uiConvenienceStore_Purchase::kPurchase_Gift, pItemInfo, ItemCount );
}

void uiConvenienceStoreMan::OnUpdateShopItemList()
{
    TreeMenuInfoParser::Instance()->Remove(BASE_TreeMenuInfo::kGroupType_CShop_Cash);
    TreeMenuInfoParser::Instance()->Remove(BASE_TreeMenuInfo::kGroupType_CShop_PPCard);
    TreeMenuInfoParser::Instance()->Remove(BASE_TreeMenuInfo::kGroupType_CShop_Mileage);
    TreeMenuInfoParser::Instance()->Remove(BASE_TreeMenuInfo::kGroupType_CShop_MyPage);

    // TreeMenuInfoParser 다시 체움
    Fill_TreeMenuInfo_Counter( GetShopListManager()->GetListPtr() );
    Fill_TreeMenuInfo_MyPage();


    //------------------------------------------------------------------------------ 
    uiToolTipMan* man = (uiToolTipMan*)g_InterfaceManager.GetUserInterfaceManager(UIMAN_TOOLTIP);
    if (man != NULL)
    {
        man->InitTooltip();
    }
    if (g_pHero != NULL)
    {
        g_pHero->GetHeroEquipContainer()->m_iTooltipStringCode = 0;
    }
}
