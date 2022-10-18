#include "SunClientPrecompiledHeader.h"

#include "uiConvenienceStore_Purchase.h"
#include "uiConvenienceStoreMan.h"
#include "InterfaceManager.h"
#include "uiConvenienceStore_Basket.h"
#include "uiConvenienceStore_Confirm.h"
#include "uiConvenienceStore_Ask.h"
#include "GlobalFunc.h"
#include "Application.h"
#include "uiConvenienceStore_PopupPackage.h"
#include "uiConvenienceStore_Counter.h"
#include "Mouse.h"
#include "PurchaseItem.h"
#include "ProductInfo.h"
#include "Hero.h"

//------------------------------------------------------------------------------ 
const DWORD uiConvenienceStore_Purchase::kDialog_ID = StrToWzID("0476");;

//------------------------------------------------------------------------------ 
uiConvenienceStore_Purchase::uiConvenienceStore_Purchase(InterfaceManager* ui_man) 
    :uiConvenienceStore_Dialog(ui_man)
{
    RegisterControlPosition(StrToWzID("B004"), kControl_Button_B004);
    RegisterControlPosition(StrToWzID("B001"), kControl_Button_B001);
    RegisterControlPosition(StrToWzID("B003"), kControl_Button_B003);
    RegisterControlPosition(StrToWzID("B006"), kControl_Button_B006);
    RegisterControlPosition(StrToWzID("B007"), kControl_Button_B007);
    RegisterControlPosition(StrToWzID("B008"), kControl_Button_B008);
    RegisterControlPosition(StrToWzID("B009"), kControl_Button_B009);
    //RegisterControlPosition(StrToWzID("B000"), kControl_Button_B000);
    //RegisterControlPosition(StrToWzID("B002"), kControl_Button_B002);
    //RegisterControlPosition(StrToWzID("B005"), kControl_Button_B005);
    //RegisterControlPosition(StrToWzID("B010"), kControl_Button_B010);
    //RegisterControlPosition(StrToWzID("B014"), kControl_Button_B014);
    RegisterControlPosition(StrToWzID("B011"), kControl_Button_B011);
    RegisterControlPosition(StrToWzID("B012"), kControl_Button_B012);
    RegisterControlPosition(StrToWzID("B013"), kControl_Button_B013);
    RegisterControlPosition(StrToWzID("B015"), kControl_Button_B015);
    RegisterControlPosition(StrToWzID("B016"), kControl_Button_B016);
    RegisterControlPosition(StrToWzID("B017"), kControl_Button_B017);
    RegisterControlPosition(StrToWzID("B024"), kControl_Button_B024);
    RegisterControlPosition(StrToWzID("B025"), kControl_Button_B025);
    RegisterControlPosition(StrToWzID("B026"), kControl_Button_B026);
    RegisterControlPosition(StrToWzID("B027"), kControl_Button_B027);
    RegisterControlPosition(StrToWzID("B018"), kControl_Button_B018);
    RegisterControlPosition(StrToWzID("B020"), kControl_Button_B020);
    RegisterControlPosition(StrToWzID("B021"), kControl_Button_B021);
    RegisterControlPosition(StrToWzID("B022"), kControl_Button_B022);
    RegisterControlPosition(StrToWzID("B023"), kControl_Button_B023);
    RegisterControlPosition(StrToWzID("B028"), kControl_Button_B028);
    RegisterControlPosition(StrToWzID("B029"), kControl_Button_B029);
    RegisterControlPosition(StrToWzID("ST05"), kControl_Text_ST05);
    RegisterControlPosition(StrToWzID("ST20"), kControl_Text_ST20);
    RegisterControlPosition(StrToWzID("ST12"), kControl_Text_ST12);
    RegisterControlPosition(StrToWzID("S000"), kControl_Text_S000);
    RegisterControlPosition(StrToWzID("S003"), kControl_Text_S003);
    RegisterControlPosition(StrToWzID("S004"), kControl_Text_S004);
    RegisterControlPosition(StrToWzID("S005"), kControl_Text_S005);
    RegisterControlPosition(StrToWzID("S006"), kControl_Text_S006);
    RegisterControlPosition(StrToWzID("S007"), kControl_Text_S007);
#ifdef _NA_20111118_CASHSHOP_UI_RENEWAL
    RegisterControlPosition(StrToWzID("B000"), kControl_Button_B000);
    RegisterControlPosition(StrToWzID("B005"), kControl_Button_B005);
    RegisterControlPosition(StrToWzID("B010"), kControl_Button_B010);
    RegisterControlPosition(StrToWzID("B014"), kControl_Button_B014);
    RegisterControlPosition(StrToWzID("B019"), kControl_Button_B019);
#else
    RegisterControlPosition(StrToWzID("S008"), kControl_Text_S008);
    RegisterControlPosition(StrToWzID("S009"), kControl_Text_S009);
    RegisterControlPosition(StrToWzID("S010"), kControl_Text_S010);
    RegisterControlPosition(StrToWzID("S011"), kControl_Text_S011);
    RegisterControlPosition(StrToWzID("S012"), kControl_Text_S012);
#endif //_NA_20111118_CASHSHOP_UI_RENEWAL
    RegisterControlPosition(StrToWzID("S013"), kControl_Text_S013);
    RegisterControlPosition(StrToWzID("S014"), kControl_Text_S014);
    RegisterControlPosition(StrToWzID("S015"), kControl_Text_S015);
    RegisterControlPosition(StrToWzID("S016"), kControl_Text_S016);
    RegisterControlPosition(StrToWzID("S001"), kControl_Text_S001);
    RegisterControlPosition(StrToWzID("S002"), kControl_Text_S002);
    RegisterControlPosition(StrToWzID("S017"), kControl_Text_S017);
    RegisterControlPosition(StrToWzID("S018"), kControl_Text_S018);
    RegisterControlPosition(StrToWzID("S019"), kControl_Text_S019);
    RegisterControlPosition(StrToWzID("S020"), kControl_Text_S020);
    RegisterControlPosition(StrToWzID("L000"), kControl_List_L000);
    RegisterControlPosition(StrToWzID("L005"), kControl_List_L005);
    RegisterControlPosition(StrToWzID("L006"), kControl_List_L006);
    RegisterControlPosition(StrToWzID("L007"), kControl_List_L007);
    RegisterControlPosition(StrToWzID("L008"), kControl_List_L008);
    RegisterControlPosition(StrToWzID("V000"), kControl_VScroll_V000);
    RegisterControlPosition(StrToWzID("P002"), kControl_Picture_P002);
    RegisterControlPosition(StrToWzID("P005"), kControl_Picture_P005);
    RegisterControlPosition(StrToWzID("P007"), kControl_Picture_P007);
    RegisterControlPosition(StrToWzID("PI11"), kControl_Picture_PI11);
    RegisterControlPosition(StrToWzID("PI13"), kControl_Picture_PI13);
    RegisterControlPosition(StrToWzID("P001"), kControl_Picture_P001);
    RegisterControlPosition(StrToWzID("P006"), kControl_Picture_P006);

#ifdef _NA_0_20120321_CASHSHOP_POPUP_EFFECT
    RegisterControlPosition(StrToWzID("P008"), kControl_Picture_P008);
    RegisterControlPosition(StrToWzID("P010"), kControl_Picture_P010);
    RegisterControlPosition(StrToWzID("P011"), kControl_Picture_P011);
    RegisterControlPosition(StrToWzID("P012"), kControl_Picture_P012);
    RegisterControlPosition(StrToWzID("P013"), kControl_Picture_P013);
#endif


    cash_type_ = ConvenienceStore::Define::kCashType_Cash;

    purchase_type_ = kPurchase_Buy;

    purchase_list().clear();
}
//------------------------------------------------------------------------------ 
uiConvenienceStore_Purchase::~uiConvenienceStore_Purchase( void )
{
    Release();
}
//------------------------------------------------------------------------------ 
void uiConvenienceStore_Purchase::Init(CDrawBase* drawbase)
{
    bool is_result = true;
    for (int i = 0; i < kListPerCount; ++i)
    {
        if (is_result)
        {
            is_result = dropdown_price(i)->Init(
#ifdef _NA_20111118_CASHSHOP_UI_RENEWAL
                GetControlWZ(GetControlID_byPosition(kControl_Button_B000 + i)),
#else
                GetStaticControlWZ(GetControlID_byPosition(kControl_Text_S008 + i)),
#endif //_NA_20111118_CASHSHOP_UI_RENEWAL
                                GetButtonControlWZ(GetControlID_byPosition(kControl_Button_B017 + i)), 
                                GetListControlWZ(GetControlID_byPosition(kControl_List_L000 + i)),
                                NULL,
                                NULL);

#ifdef _NA_0_20120321_CASHSHOP_POPUP_EFFECT
            dropdown_price(i)->SetBlendImage(GetPictureControlWZ(GetControlID_byPosition(kControl_Picture_P008 +i)));
#endif
            dropdown_price(i)->set_simple_menu(true);

            dropdown_price(i)->list_control()->SetStartLineGapTop(10);
            dropdown_price(i)->list_control()->SetSellHeight(16);
        }
    }

#ifdef _RUSSIA
    CCtrlStaticWZ* static_control_ptr = NULL;
    static_control_ptr = static_cast<CCtrlStaticWZ*> (GetControlWZ_byPosition(kControl_Text_S001));
    if (static_control_ptr)
    {        
        GlobalFunc::SetCaption(static_control_ptr, _T(""));
    }
#endif // _RUSSIA

    ASSERT(true && "uiConvenienceStore_Purchase::Init false");
}
//------------------------------------------------------------------------------ 
void uiConvenienceStore_Purchase::Release(void)
{
    for (int i = 0; i < kListPerCount; ++i)
    {
        dropdown_price(i)->Release();
    }
}
//------------------------------------------------------------------------------ 
void uiConvenienceStore_Purchase::OnShowWindow(BOOL val)
{
    if (val == TRUE)
    {
        //------------------------------------------------------------------------------ 
        keyMsg msg;
        ZeroMemory(&msg,sizeof(msg)); 
        msg.dwType = uiConvenienceStore_Purchase::kDialog_ID;
        msg.wParam = uiConvenienceStore_Purchase::kDialog_ID;
        msg.DoSomething = GlobalFunc::DoShowWindowEx;
        g_KeyQueueManager.PushBack(msg);
        //------------------------------------------------------------------------------ 


        //------------------------------------------------------------------------------ 
        if (purchase_type() == uiConvenienceStore_Purchase::kPurchase_Buy)
        {
            //2566	구매하기
            if (GetControlID_byPosition(kControl_Text_S000))
                GetStaticControlWZ(GetControlID_byPosition(kControl_Text_S000))->SetTextID(2566);

            ////155502	하단의 구입 버튼을  누르면 위 아이템이 구입됩니다.\n잘 확인 후 결제 하시길 바랍니다.
            //GlobalFunc::SetTextFromStringTable(GetControlWZ_byPosition(kControl_Text_S001), 155502);

            //155530	구입
            GlobalFunc::SetTextFromStringTable(GetControlWZ_byPosition(kControl_Button_B001), ConvenienceStore::Define::kShopStringCode_Shop_Popup_Buy);
        }
        else
        {
            //155501	선물 하기
            if (GetControlID_byPosition(kControl_Text_S000))
                GetStaticControlWZ(GetControlID_byPosition(kControl_Text_S000))->SetTextID(155501);

            ////155503	하단의 선물 버튼을  누르면 위 아이템이 구입됩니다.\n잘 확인 후 결제 하시길 바랍니다.
            //GlobalFunc::SetTextFromStringTable(GetControlWZ_byPosition(kControl_Text_S001), 155503);

            //155529	선물
            GlobalFunc::SetTextFromStringTable(GetControlWZ_byPosition(kControl_Button_B001), ConvenienceStore::Define::kShopStringCode_Shop_Popup_Gift);
        }

        //------------------------------------------------------------------------------ 
        g_InterfaceManager.LockDlgs(uiConvenienceStore_Purchase::kDialog_ID);
    }
    else
    {
        if (GetManager()->popup_package() )
        {
            if (GetManager()->popup_package()->owner() == this)
            {
                GetManager()->popup_package()->uiConvenienceStore_PopupMenu::ShowInterface(FALSE);
            }
        }

        RemovePurchase();

        g_KeyQueueManager.DeleteMsg(uiConvenienceStore_Purchase::kDialog_ID);

#if WZENGINEVER >= 410
        if (g_InterfaceManager.GetDialogManager()->GetLockDlgID() == uiConvenienceStore_Purchase::kDialog_ID)
        { 
            g_InterfaceManager.UnLockDlgs();
        }
#endif //#if WZENGINEVER >= 410
    }
}
//------------------------------------------------------------------------------ 
DropdownControl* uiConvenienceStore_Purchase::dropdown_price(int index)
{
    if (index >= 0 && index < uiConvenienceStore_Purchase::kListPerCount) 
    {
        return &(dropdown_price_[index]); 
    }
    return NULL;
}
//------------------------------------------------------------------------------
void uiConvenienceStore_Purchase::MessageProc(SI_MESSAGE* message)
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
    for (int i = 0; i < kListPerCount; ++i)
    {
        bool is_change = false;
        if (dropdown_price(i)->MessageProc(message, OUT is_change))
        {
            if (is_change)
            {
                CCtrlButtonWZ* button_control = backslot_button(i);
                ConvenienceStore::PurchaseItem* basket = reinterpret_cast<ConvenienceStore::PurchaseItem*>(button_control->GetUserData());

                basket->SetPriceSequence( dropdown_price(i)->CurrentUserData() );
            }
            return;
        }
    }

    //------------------------------------------------------------------------------ 
    switch (message->eResultMsg)
    {
    case RT_MSG_LBUTTONCLICK:
        OnLButtonClick(message);
        break;

    default:
        uiConvenienceStore_Dialog::MessageProc(message);
        VScrollDialog::MessageProc(message);
        break;
    }
}
//------------------------------------------------------------------------------ 
CCtrlVScrollWZ* uiConvenienceStore_Purchase::vscroll_control()
{
    return GetVScrollControlWZ(GetControlID_byPosition(kControl_VScroll_V000));
}
//------------------------------------------------------------------------------ 
bool uiConvenienceStore_Purchase::GetScrollArea(RECT& area)
{  
    if ((GetManager()->popup_package()->IsVisible() == TRUE) && 
        (GetManager()->popup_package()->InterSectRect(Mouse::Instance()->CheckMousePos()) == TRUE))
    {
        return false;
    }
    
    for (int i = 0; i < kListPerCount; ++i)
    {
        if (dropdown_price(i) != NULL)
        {
            if (dropdown_price(i)->IsOpened() == true)
            {
                return false;
            }
        }
    }
    for (int i = 0; i < kListPerCount; ++i)
    {
        CCtrlButtonWZ* button_control = backslot_button(i);
        if ((button_control != NULL) && (IsOverControl(button_control) == true))
        {
            area = button_control->GetSizeRect(); 
            return true;
        }
    }
    return false;
}
//------------------------------------------------------------------------------ 
bool uiConvenienceStore_Purchase::OnVScrollThumb(SI_MESSAGE* msg)
{
    if (vscroll_control() && VScrollDialog::OnVScrollThumb(msg) == true)
    {
        FillPurchase(vscroll_control()->GetScrollPosWZ());

        GetManager()->popup_package()->uiConvenienceStore_PopupMenu::ShowInterface(FALSE);

        return true;
    }
    return false;
}
//------------------------------------------------------------------------------ 
bool uiConvenienceStore_Purchase::OnVScrollUp(SI_MESSAGE* msg)
{
    if (vscroll_control() && VScrollDialog::OnVScrollUp(msg) == true)
    {
        FillPurchase(vscroll_control()->GetScrollPosWZ());

        GetManager()->popup_package()->uiConvenienceStore_PopupMenu::ShowInterface(FALSE);

        return true;
    }
    return false;
}
//------------------------------------------------------------------------------ 
bool uiConvenienceStore_Purchase::OnVScrollDown(SI_MESSAGE* msg)
{
    if (vscroll_control() && VScrollDialog::OnVScrollDown(msg) == true)
    {
        FillPurchase(vscroll_control()->GetScrollPosWZ());

        GetManager()->popup_package()->uiConvenienceStore_PopupMenu::ShowInterface(FALSE);

        return true;
    }
    return false;
}
//------------------------------------------------------------------------------
bool uiConvenienceStore_Purchase::OnLButtonClick(SI_MESSAGE* message)
{
    bool result = true;
    switch (GetControlPosition_byID(message->dwCtrlID))
    {
    case kControl_Button_B004:
    case kControl_Button_B028:
        {
            uiBase::ShowInterface(FALSE);
        }
        break;
    case kControl_Button_B001:
        {
            // 구매하기
            uiConvenienceStore_Dialog::AskShowType ask_show_type = kAskShow_None;
            if (purchase_type() == kPurchase_Buy)
            {
                //155507	정말 구입 하시겠습니까?
                //155454	구매 후 7일 이내 구매 철회가 가능하며, 사용된 아이템 및\n거래로 주고 받은 아이템은 구매철회가 불가능합니다.
                //155570	<font=tt12><color=ffffffff>-----------------------------------------------</color></font>
                GetManager()->ask()->OpenAsk( this, uiConvenienceStore_Dialog::kAskShow_Buy );
                GetManager()->ask()->AppendFText( g_InterfaceManager.GetInterfaceString(155507) );
#ifndef _RUSSIA
                GetManager()->ask()->AppendFText( g_InterfaceManager.GetInterfaceString(155454) );
#endif // _RUSSIA
                GetManager()->ask()->AppendFText( g_InterfaceManager.GetInterfaceString(155570) );

                for( size_t i=0;i!=purchase_list().size();++i)
                {
                    GetManager()->ask()->AppendItemText( purchase_list().at(i) );
                }
            }
            else if (purchase_type() == kPurchase_Gift)
            {
                //155508	정말 선물 하시겠습니까?
                //155455	선물보내기로 전송된 아이템은 구매철회가 불가능합니다.
                //155570	<font=tt12><color=ffffffff>-----------------------------------------------</color></font>
                GetManager()->ask()->OpenAsk( this, uiConvenienceStore_Dialog::kAskShow_Gift );
                GetManager()->ask()->AppendFText( g_InterfaceManager.GetInterfaceString(155508) );
#ifndef _RUSSIA
                GetManager()->ask()->AppendFText( g_InterfaceManager.GetInterfaceString(155455) );
#endif // _RUSSIA
                GetManager()->ask()->AppendFText( g_InterfaceManager.GetInterfaceString(155570) );

                for( size_t i=0;i!=purchase_list().size();++i)
                {
                    GetManager()->ask()->AppendItemText( purchase_list().at(i) );
                }
            }
        }
        break;
    case kControl_Button_B017:
        {
        }
        break;

    case kControl_Button_B018:
    case kControl_Button_B020:
    case kControl_Button_B021:
    case kControl_Button_B022:
    case kControl_Button_B023:
        {
            if (vscroll_control())
            {
                int index = GetControlPosition_byID(message->dwCtrlID) - kControl_Button_B018;
                RemovePurchase(vscroll_control()->GetScrollPosWZ() + index); 

                if (purchase_list().size() > 0)
                {
                    int scroll_max_num = max(0, (int)purchase_list().size() - kListPerCount);
                    ResizeingVScrollControl(scroll_max_num, vscroll_control()->GetScrollPosWZ());

                    FillPurchase(vscroll_control()->GetScrollPosWZ());
                }
                else
                {
                    uiBase::ShowInterface(FALSE);
                }
            }
        }
        break;

    case kControl_Button_B024:
        {
        }
        break;
    case kControl_Button_B025:
        {
        }
        break;
    case kControl_Button_B026:
        {
        }
        break;
    case kControl_Button_B027:
        {
        }
        break;

    case kControl_Button_B029:
        {
#if defined(_GLOBAL) || defined(_CHINA)
            //! 캐시 충전     //155566	http://www.sunonline.co.kr
            GlobalFunc::ExecWebSite(155566);
#else
            // 6825	준비중입니다. 웹페이지에서 확인해주세요.
            GetManager()->ask()->OpenAsk( this, uiConvenienceStore_Dialog::kAskShow_Message );
            GetManager()->ask()->AppendFText(g_InterfaceManager.GetInterfaceString( 6825 ));
            GetManager()->ask()->EnableCloseButton();
#endif
        }
        break;
    case kControl_Text_ST05:
        {
        }
        break;
    case kControl_Text_ST20:
        {
        }
        break;
    case kControl_Text_ST12:
        {
        }
        break;
    case kControl_Text_S000:
        {
        }
        break;
    case kControl_Text_S003:
        {
        }
        break;
    case kControl_Text_S004:
        {
        }
        break;
    case kControl_Text_S005:
        {
        }
        break;
    case kControl_Text_S006:
        {
        }
        break;
    case kControl_Text_S007:
        {
        }
        break;
#ifdef _NA_20111118_CASHSHOP_UI_RENEWAL
#else
    case kControl_Text_S008:
        {
        }
        break;
    case kControl_Text_S009:
        {
        }
        break;
    case kControl_Text_S010:
        {
        }
        break;
    case kControl_Text_S011:
        {
        }
        break;
    case kControl_Text_S012:
        {
        }
        break;
#endif //_NA_20111118_CASHSHOP_UI_RENEWAL
    case kControl_Text_S013:
        {
        }
        break;
    case kControl_Text_S014:
        {
        }
        break;
    case kControl_Text_S015:
        {
        }
        break;
    case kControl_Text_S016:
        {
        }
        break;
    case kControl_Text_S001:
        {
        }
        break;
    case kControl_Text_S002:
        {
        }
        break;
    case kControl_Text_S017:
        {
        }
        break;
    case kControl_Text_S018:
        {
        }
        break;
    //case kControl_Text_S019:
    //    {
    //    }
    //    break;
    case kControl_Text_S020:
        {
        }
        break;
    //case kControl_List_L000:
    //    {
    //    }
    //    break;
    //case kControl_List_L005:
    //    {
    //    }
    //    break;
    //case kControl_List_L006:
    //    {
    //    }
    //    break;
    //case kControl_List_L007:
    //    {
    //    }
    //    break;
    //case kControl_List_L008:
    //    {
    //    }
    //    break;
    //case kControl_VScroll_V000:
    //    {
    //    }
    //    break;
    case kControl_Picture_P002:
        {
        }
        break;
    case kControl_Picture_P005:
        {
        }
        break;
    case kControl_Picture_P007:
        {
        }
        break;
    case kControl_Picture_PI11:
        {
        }
        break;
    case kControl_Picture_PI13:
        {
        }
        break;
    case kControl_Picture_P001:
        {
        }
        break;
    case kControl_Picture_P006:
        {
        }
        break;
    default:
        {
        }
        break;
    }
    return result;
}

void uiConvenienceStore_Purchase::OpenPurchase( PurchaseType _purchase_type )
{
    purchase_type_ = _purchase_type;

    uiConvenienceStore_Dialog::ShowInterface(TRUE);

    _UpdateControlState();
}

//------------------------------------------------------------------------------ 
void uiConvenienceStore_Purchase::_UpdateControlState()
{
    if( purchase_type_ == kPurchase_Gift )
    {
        if (GetControlWZ_byPosition(kControl_Text_S019))
            GetControlWZ_byPosition(kControl_Text_S019)->SetTextWZ(gift_recv_name_.c_str());
    }
    else
    {
        if (GetControlWZ_byPosition(kControl_Text_S019))
            GetControlWZ_byPosition(kControl_Text_S019)->SetTextWZ(g_pHero->GetName());
    }
}

//------------------------------------------------------------------------------ 
void uiConvenienceStore_Purchase::SetGiftMessage( LPCTSTR target_name, LPCTSTR send_message )
{
    if (target_name != NULL)
    {
        gift_recv_name_ = target_name;
    }
    if (send_message != NULL)
    {
        gift_msg_ = send_message;
    }
}

//------------------------------------------------------------------------------ 
void uiConvenienceStore_Purchase::UpdateUI(TCHAR* recv_character_name)
{
    if (GetControlWZ_byPosition(kControl_Text_S019))
        GetControlWZ_byPosition(kControl_Text_S019)->SetTextWZ(recv_character_name);
}
//------------------------------------------------------------------------------ 
bool uiConvenienceStore_Purchase::InsertPurchase(ConvenienceStore::ProductItem* product, int basket_product_index)
{
    if (product == NULL)
    {
        return false;
    }
    if ((purchase_type() == kPurchase_Buy && product->is_possible_sales() == false) ||
        (purchase_type() == kPurchase_Gift && product->is_possible_gift() == false))
    {
        return false;
    }

    ConvenienceStore::PurchaseItem* basket = ConvenienceStore::PurchaseItem::CreatePurchase(product, basket_product_index);
    if (basket == NULL)
    {
        return false;
    }

    purchase_list().push_back(basket);

    //------------------------------------------------------------------------------ 
    int scroll_max_num = max(0, (int)purchase_list().size() - kListPerCount);
    ResizeingVScrollControl(scroll_max_num, 0); 
    FillPurchase(vscroll_control()->GetScrollPosWZ());

    return true;
}

//------------------------------------------------------------------------------ 
void uiConvenienceStore_Purchase::RemovePurchase()
{
    std::vector<ConvenienceStore::PurchaseItem*>::iterator begin = purchase_list().begin();
    std::vector<ConvenienceStore::PurchaseItem*>::iterator end = purchase_list().end();
    for (; begin != end; ++ begin)
    {
        ConvenienceStore::PurchaseItem* basket = (*begin);
        SAFE_DELETE(basket)
    }
    purchase_list().clear();
}
//------------------------------------------------------------------------------ 
bool uiConvenienceStore_Purchase::RemovePurchase(int purchase_index)
{
    if (purchase_index < 0 || purchase_index >= static_cast<int>(purchase_list().size()))
    {
        return false; 
    }
    std::vector<ConvenienceStore::PurchaseItem*>::iterator iter = purchase_list().begin() + purchase_index;
    if (iter != purchase_list().end())
    {
        ConvenienceStore::PurchaseItem* basket = (*iter);
        SAFE_DELETE(basket);

        purchase_list().erase(iter);
    }

    return true;
}
//------------------------------------------------------------------------------ 
void uiConvenienceStore_Purchase::OnUpdateSolarDialog()
{
#if WZENGINEVER >= 410
    if ((g_InterfaceManager.GetDialogManager()->GetLockDlgID() != 0) && 
        (g_InterfaceManager.GetDialogManager()->GetLockDlgID() != uiConvenienceStore_Purchase::kDialog_ID))
    {
        return; 
    }
#endif //#if WZENGINEVER >= 410

    //------------------------------------------------------------------------------ 
    VScrollDialog::Update();

    //------------------------------------------------------------------------------ 
    //가격표시
    CASH purchase_price = GetPurchasePrice(); 
    CASH my_price = (CASH)ConvenienceStore::CashShop::instance()->GetMyCash();
    if( cash_type_ == ConvenienceStore::Define::kCashType_PPCard )
        my_price = (CASH)ConvenienceStore::CashShop::instance()->GetMyPPCash();
    else if( cash_type_ == ConvenienceStore::Define::kCashType_Mileage )
        my_price = (CASH)ConvenienceStore::CashShop::instance()->GetMyMileage();

    GlobalFunc::SetText_SignMoney(GetControlWZ_byPosition(kControl_Text_ST05), my_price);
    GlobalFunc::SetText_SignMoney(GetControlWZ_byPosition(kControl_Text_S015), purchase_price);
    GlobalFunc::SetText_SignMoney(GetControlWZ_byPosition(kControl_Text_S016), (INT64)my_price - (INT64)purchase_price);

    if (GetControlWZ_byPosition(kControl_Button_B001))
    {
        ENUM_DRAWSTATE draw_state = GetControlWZ_byPosition(kControl_Button_B001)->GetDrawState();
        if ((my_price < purchase_price) && (draw_state != E_DISABLE))
        {
            GetControlWZ_byPosition(kControl_Button_B001)->EnableWindowWZ(false);
        }
        else if ((my_price >= purchase_price) && (draw_state == E_DISABLE))
        {
            GetControlWZ_byPosition(kControl_Button_B001)->EnableWindowWZ(true);
        }
    }

    //155496	총 %s개의 아이템을 선택 하셨습니다.
    GlobalFunc::SetTextFromStringTable(GetControlWZ_byPosition(kControl_Text_ST20), 155496, (int)purchase_list().size());

    //------------------------------------------------------------------------------ 
    if (my_price < purchase_price)
    {
        //155504	현재 보유하신 소지금이 부족하여 정상적인 구입을\n하실 수 없습니다. 캐시 충전 후에 구입 하시길 바랍니다.
        if (GetControlID_byPosition(kControl_Text_S001))
            GetStaticControlWZ(GetControlID_byPosition(kControl_Text_S001))->SetTextColor(RGBA(255, 0, 0, 255));
        GlobalFunc::SetTextFromStringTable(GetControlWZ_byPosition(kControl_Text_S001), 155504);
    }
    else
    {
#ifdef _RUSSIA
        CCtrlStaticWZ* static_control_ptr = NULL;
        static_control_ptr = static_cast<CCtrlStaticWZ*> (GetControlWZ_byPosition(kControl_Text_S001));
        if (static_control_ptr)
        {        
            GlobalFunc::SetCaption(static_control_ptr, _T(""));
        }
#else
        if (GetControlID_byPosition(kControl_Text_S001))
            GetStaticControlWZ(GetControlID_byPosition(kControl_Text_S001))->SetTextColor(RGBA(255, 255, 255, 255));
        if (purchase_type() == uiConvenienceStore_Purchase::kPurchase_Buy)
        {
            //155502	하단의 구입 버튼을  누르면 위 아이템이 구입됩니다.\n잘 확인 후 결제 하시길 바랍니다.
            GlobalFunc::SetTextFromStringTable(GetControlWZ_byPosition(kControl_Text_S001), 155502);
        }
        else
        {
            //155503	하단의 선물 버튼을  누르면 위 아이템이 구입됩니다.\n잘 확인 후 결제 하시길 바랍니다.
            GlobalFunc::SetTextFromStringTable(GetControlWZ_byPosition(kControl_Text_S001), 155503);
        }
#endif // _RUSSIA
    }

    //------------------------------------------------------------------------------ 
    for (int i = 0; i < kListPerCount; ++i)
    {
        if (dropdown_price(i) != NULL)
        {
            dropdown_price(i)->Update();
        }
    }

    UpdateTooltip();
}
//------------------------------------------------------------------------------ 
void uiConvenienceStore_Purchase::OnRenderSolarDialog()
{
#if WZENGINEVER >= 410
    if (g_InterfaceManager.GetDialogManager()->GetLockDlgID() == uiConvenienceStore_Purchase::kDialog_ID)
    { 
        POINT pt = g_pApplication->Get2DModeMapping();
        g_pSunRenderer->ColorFill(WzColor_RGBA(0,0,0,128), 0.0f, 0.0f, (float)pt.x,(float)pt.y);
        GetDialogWZ()->OnDraw(true);
    }
#endif //#if WZENGINEVER >= 410

    for (int i = 0; i < kListPerCount; ++i)
    {
        CCtrlButtonWZ* button_control = backslot_button(i);
        CControlWZ* image_control = GetControlWZ_byPosition(kControl_Button_B011 + i);
        if (button_control != NULL && image_control != NULL)
        {
            ConvenienceStore::PurchaseItem* basket = reinterpret_cast<ConvenienceStore::PurchaseItem*>(button_control->GetUserData());
            if (basket != NULL && basket->GetIconImage() != NULL)
            {
                basket->GetIconImage()->Render(&(image_control->GetSizeRect()));
            }
        }
    }
}
//------------------------------------------------------------------------------ 
void uiConvenienceStore_Purchase::FillPurchase(int begin_purchase_index)
{
    bool buy_mileage = cash_type_ == ConvenienceStore::Define::kCashType_Mileage;
    
    for (int i = 0; i < kListPerCount; ++i)
    {
        CCtrlStaticWZ* name_control = GetStaticControlWZ(GetControlID_byPosition(kControl_Text_S003 + i));
        CCtrlButtonWZ* button_control = backslot_button(i);
        CControlWZ* close_control = GetControlWZ_byPosition(kControl_Button_B018 + i);
        
        dropdown_price(i)->DeleteItemAll();
        dropdown_price(i)->SetEnable(false);
        
        int index = begin_purchase_index + i; 
        if (index >= 0 && index < (int)purchase_list().size())
        {
            std::vector<ConvenienceStore::PurchaseItem*>::const_reference basket = purchase_list().at(index);

            close_control->EnableWindowWZ(true);
            name_control->SetTextWZ(basket->GetName());
            button_control->SetUserData(reinterpret_cast<DWORD>(basket)); 

            if (basket->is_lotto() || basket->is_package())
            {
                TCHAR msg_text[INTERFACE_STRING_LENGTH] = {NULL, };
                ConvenienceStore::GetDisplayPriceStringByProduct(msg_text, INTERFACE_STRING_LENGTH, basket);

                dropdown_price(i)->InsertItem(msg_text, (void*)(basket->GetPriceSequence()), StrToWzID("st10"), DT_VCENTER | DT_CENTER);

                dropdown_price(i)->ResetVScroll();
                dropdown_price(i)->SelectByUserData(basket->GetPriceSequence());
            }
            else
            {
                std::vector<ConvenienceStore::ProductInfo>::iterator begin = basket->GetInfoVector().begin();
                std::vector<ConvenienceStore::ProductInfo>::iterator end = basket->GetInfoVector().end();
                int sel_index = 0;
                for (; begin != end; ++begin, ++sel_index)
                {
                    TCHAR msg_text[INTERFACE_STRING_LENGTH] = {NULL, };
                    ConvenienceStore::GetDisplayPriceStringByProductInfo(msg_text, INTERFACE_STRING_LENGTH, (ConvenienceStore::ProductInfo*)&(*begin), buy_mileage);

                    dropdown_price(i)->InsertItem(msg_text, (void*)begin->GetPriceProperty().GetPriceSequence(), StrToWzID("st10"), DT_VCENTER | DT_CENTER);
                }
                dropdown_price(i)->SetEnable(true);
                dropdown_price(i)->ResetVScroll();
                dropdown_price(i)->SelectByUserData(basket->GetPriceSequence());
            }
        }
        else
        {
            close_control->EnableWindowWZ(false);
            name_control->SetTextWZ("");
            button_control->SetUserData(NULL);
        }
    }
}
//------------------------------------------------------------------------------ 
CCtrlButtonWZ* uiConvenienceStore_Purchase::backslot_button(int slot_index)
{
    return GetButtonControlWZ(GetControlID_byPosition(kControl_Button_B003 + slot_index));
}
//------------------------------------------------------------------------------ 
CASH uiConvenienceStore_Purchase::GetPurchasePrice()
{
    std::vector<ConvenienceStore::PurchaseItem*>::iterator begin = purchase_list().begin();
    std::vector<ConvenienceStore::PurchaseItem*>::iterator end = purchase_list().end();

    CASH total_price = 0;;
    for (; begin != end; ++begin)
    {
        total_price += (*begin)->GetPrice((*begin)->GetPriceSequence());
    }

    return total_price;
}
//------------------------------------------------------------------------------ 
void uiConvenienceStore_Purchase::UpdateTooltip()
{
#if WZENGINEVER >= 410
    if ((g_InterfaceManager.GetDialogManager()->GetLockDlgID() != 0) && 
        (g_InterfaceManager.GetDialogManager()->GetLockDlgID() != uiConvenienceStore_Purchase::kDialog_ID))
    {
        return;  
    }
#endif //#if WZENGINEVER >= 410

    //! 묶음 상품 툴팁이 떠 있는지 확인한다.
    if (GetManager()->popup_package()->IsVisible() == TRUE)
    {
        if ((GetManager()->popup_package()->InterSectRect(Mouse::Instance()->CheckMousePos()) == TRUE) ||
            (IsOverControl(GetManager()->popup_package()->owner_control()) == true))
        {
            return;
        }
    }

    for (int i = 0; i < kListPerCount; ++i)
    {
        CCtrlButtonWZ* button_control = backslot_button(i);
        CControlWZ* picture_control = GetControlWZ_byPosition(kControl_Button_B011 + i);
        if ((IsOverControl(picture_control) == true) && (button_control->GetUserData() != NULL))
        {
            ConvenienceStore::PurchaseItem* basket = reinterpret_cast<ConvenienceStore::PurchaseItem*>(button_control->GetUserData());
            if (basket->IsPurchase() == false)
            {
                continue;
            }
            if (basket->is_package() == true)
            {
                GetManager()->popup_package()->ShowInterface(TRUE, basket);  
                GetManager()->popup_package()->Reset();
                GetManager()->popup_package()->set_owner(this, picture_control);

                GetManager()->popup_package()->MoveWindowWZ(button_control->GetSizeRect().left, 
                                                            picture_control->GetSizeRect().bottom);
                return;
            }
            else
            {
                GetManager()->RegisterTooltip(basket);
            }
            break;
        }
    }
    GetManager()->popup_package()->uiConvenienceStore_PopupMenu::ShowInterface(FALSE);
}
//------------------------------------------------------------------------------ 
bool uiConvenienceStore_Purchase::OnClickAskDialog(uiConvenienceStore_Dialog::AskCommandType command_type, uiConvenienceStore_Dialog::AskShowType show_type, void* user_data)
{
    switch( show_type )
    {
    case uiConvenienceStore_Dialog::kAskShow_Buy:
    case uiConvenienceStore_Dialog::kAskShow_Gift:
    case uiConvenienceStore_Dialog::kAskShow_StorageBuy_OneRecv:
    case uiConvenienceStore_Dialog::kAskShow_StorageBuy_AllRecv:
    case uiConvenienceStore_Dialog::kAskShow_StorageBuy_AllRecv_Confirm: //!< 받고 나서 확인창
    case uiConvenienceStore_Dialog::kAskShow_StorageGift_OneRecv:
    case uiConvenienceStore_Dialog::kAskShow_StorageGift_AllRecv:
    case uiConvenienceStore_Dialog::kAskShow_StorageGift_AllRecv_Confirm: //!< 받고 나서 확인창

        // 구매의 확인
        if (command_type == uiConvenienceStore_Dialog::kAskCommand_OK)
        {
            if( purchase_list().size() == 0 )
            {
                return true;
            }

            // 결제 수단 설정
            ConvenienceStore::Define::CashType cash_type = cash_type_;

            if (purchase_type() == uiConvenienceStore_Purchase::kPurchase_Buy)
            {
                ConvenienceStore::CashShop::instance()->SendBuyItem(purchase_list(), cash_type );
                GetManager()->confirm()->SetRecvName(g_pHero->GetName());
            }
            else if (purchase_type() == uiConvenienceStore_Purchase::kPurchase_Gift)
            {
                ConvenienceStore::CashShop::instance()->SendGiftItem(purchase_list(), cash_type, gift_recv_name_.c_str(), gift_msg_.c_str() );
                GetManager()->confirm()->SetRecvName( gift_recv_name_.c_str() );
            }

            //! basket 비운다.
            std::vector<ConvenienceStore::PurchaseItem*>::reverse_iterator rbegin = purchase_list().rbegin();
            std::vector<ConvenienceStore::PurchaseItem*>::reverse_iterator rend = purchase_list().rend();
            for (; rbegin != rend; ++rbegin)
            {
                GetManager()->basket()->RemoveBasket((*rbegin)->GetBasketProduceIndex());
            }
            uiConvenienceStore_Dialog::ShowInterface(FALSE);
        }
        break;

    case uiConvenienceStore_Dialog::kAskShow_Message:
        break;
    }


    return true;
}
