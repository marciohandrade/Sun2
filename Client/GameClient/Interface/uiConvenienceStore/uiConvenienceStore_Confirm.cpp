#include "SunClientPrecompiledHeader.h"

#include "uiConvenienceStore_Confirm.h"
#include "uiConvenienceStoreMan.h"
#include "InterfaceManager.h"
#include "uiConvenienceStore_Purchase.h"
#include "Application.h"
#include "GlobalFunc.h"
#include "FTextParser.h"
#include "FTextSentence.h"
#include "ObjectBase.h"
#include "Hero.h"
#ifdef _NA_0_20110613_GAME_SHOP_RENEWAL
#include "uiConvenienceStore/CashShop.h"
#endif
   
////------------------------------------------------------------------------------ 
const DWORD uiConvenienceStore_Confirm::kDialog_ID = StrToWzID("0477");;

//------------------------------------------------------------------------------ 
uiConvenienceStore_Confirm::uiConvenienceStore_Confirm(InterfaceManager* ui_man) : uiConvenienceStore_Dialog(ui_man)
{
    RegisterControlPosition(StrToWzID("B004"), kControl_Button_B004);
    RegisterControlPosition(StrToWzID("B028"), kControl_Button_B028);
    RegisterControlPosition(StrToWzID("S000"), kControl_Text_S000);
    RegisterControlPosition(StrToWzID("S003"), kControl_Text_S003);
    RegisterControlPosition(StrToWzID("S013"), kControl_Text_S013);
    RegisterControlPosition(StrToWzID("S014"), kControl_Text_S014);
    RegisterControlPosition(StrToWzID("S015"), kControl_Text_S015);
    RegisterControlPosition(StrToWzID("S016"), kControl_Text_S016);
    RegisterControlPosition(StrToWzID("S002"), kControl_Text_S002);
    RegisterControlPosition(StrToWzID("S004"), kControl_Text_S004);
    RegisterControlPosition(StrToWzID("S001"), kControl_Text_S001);
    RegisterControlPosition(StrToWzID("V000"), kControl_VScroll_V000);
    RegisterControlPosition(StrToWzID("P002"), kControl_Picture_P002);
    RegisterControlPosition(StrToWzID("P005"), kControl_Picture_P005);
    RegisterControlPosition(StrToWzID("P007"), kControl_Picture_P007);
    RegisterControlPosition(StrToWzID("PI13"), kControl_Picture_PI13);
    RegisterControlPosition(StrToWzID("P001"), kControl_Picture_P001);
    RegisterControlPosition(StrToWzID("P006"), kControl_Picture_P006);
    RegisterControlPosition(StrToWzID("P000"), kControl_Picture_P000);
}
//------------------------------------------------------------------------------ 
uiConvenienceStore_Confirm::~uiConvenienceStore_Confirm( void )
{
    Release();
}
//------------------------------------------------------------------------------ 
void uiConvenienceStore_Confirm::Init(CDrawBase* drawbase)
{
    ASSERT(true && "uiConvenienceStore_Confirm::Init false");
}
//------------------------------------------------------------------------------ 
void uiConvenienceStore_Confirm::Release(void)
{
}
//------------------------------------------------------------------------------
void uiConvenienceStore_Confirm::MessageProc(SI_MESSAGE* message)
{
    if (IsLock())
    {
        return;
    }

    if (GetManager() == NULL)
    {
        return;
    }

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
bool uiConvenienceStore_Confirm::GetScrollArea(RECT& area)
{
    if (GetControlWZ_byPosition(kControl_Text_S003) &&
        IsOverControl(GetControlWZ_byPosition(kControl_Text_S003)) == true)
    {
        area = GetControlWZ_byPosition(kControl_Text_S003)->GetSizeRect();
        return true;
    }
    return false;
}
//------------------------------------------------------------------------------ 
CCtrlVScrollWZ* uiConvenienceStore_Confirm::vscroll_control()
{
    return GetVScrollControlWZ(GetControlID_byPosition(kControl_VScroll_V000));
}
//------------------------------------------------------------------------------ 
bool uiConvenienceStore_Confirm::OnVScrollThumb(SI_MESSAGE* msg)
{
    if (vscroll_control() && VScrollDialog::OnVScrollThumb(msg) == true)
    {
        ftext_render().SetBeginLineIndex(vscroll_control()->GetScrollPosWZ());

        return true;
    }
    return false;
}
//------------------------------------------------------------------------------ 
bool uiConvenienceStore_Confirm::OnVScrollUp(SI_MESSAGE* msg)
{
    if (vscroll_control() && VScrollDialog::OnVScrollUp(msg) == true)
    {
        ftext_render().SetBeginLineIndex(vscroll_control()->GetScrollPosWZ());

        return true;
    }
    return false;
}
//------------------------------------------------------------------------------ 
bool uiConvenienceStore_Confirm::OnVScrollDown(SI_MESSAGE* msg)
{
    if (vscroll_control() && VScrollDialog::OnVScrollDown(msg) == true)
    {
        ftext_render().SetBeginLineIndex(vscroll_control()->GetScrollPosWZ());

        return true;
    }
    return false;
}
//------------------------------------------------------------------------------
bool uiConvenienceStore_Confirm::OnLButtonClick(SI_MESSAGE* message)
{
    bool result = true;
    switch (GetControlPosition_byID(message->dwCtrlID))
    {
    case kControl_Button_B004:
    case kControl_Button_B028:
        {
            uiConvenienceStore_Dialog::ShowInterface(FALSE);
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
    //case kControl_Text_S013:
    //    {
    //    }
    //    break;
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
    case kControl_Text_S002:
        {
        }
        break;
    case kControl_Text_S004:
        {
        }
        break;
    case kControl_VScroll_V000:
        {
        }
        break;
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
    case kControl_Picture_P000:
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
//------------------------------------------------------------------------------ 
void uiConvenienceStore_Confirm::OnRenderSolarDialog()
{
#if WZENGINEVER >= 410
    if (g_InterfaceManager.GetDialogManager()->GetLockDlgID() == uiConvenienceStore_Confirm::kDialog_ID)
    { 
        POINT pt = g_pApplication->Get2DModeMapping();
        g_pSunRenderer->ColorFill(WzColor_RGBA(0,0,0,128), 0.0f, 0.0f, (float)pt.x,(float)pt.y);
        GetDialogWZ()->OnDraw(true);
    }
#endif //#if WZENGINEVER >= 410

    ftext_render().Render();
}
//------------------------------------------------------------------------------ 
void uiConvenienceStore_Confirm::OnUpdateSolarDialog()
{
    VScrollDialog::Update();

    CControlWZ* area_control = GetControlWZ_byPosition(kControl_Text_S003);
    if (area_control != NULL)
    {
        ftext_render().SetRect(area_control->GetSizeRect());
    }
}
//------------------------------------------------------------------------------ 
void uiConvenienceStore_Confirm::OnShowWindow(BOOL val)
{
    if (val == TRUE)
    {
        //------------------------------------------------------------------------------ 
        keyMsg msg;
        ZeroMemory(&msg,sizeof(msg)); 
        msg.dwType = uiConvenienceStore_Confirm::kDialog_ID;
        msg.wParam = uiConvenienceStore_Confirm::kDialog_ID;
        msg.DoSomething = GlobalFunc::DoShowWindowEx;
        g_KeyQueueManager.PushBack(msg);
        //------------------------------------------------------------------------------ 

        if (GetControlWZ_byPosition(kControl_Text_S001) &&
            GetControlWZ_byPosition(kControl_Text_S013) &&
            GetControlWZ_byPosition(kControl_Text_S015) &&
            GetControlWZ_byPosition(kControl_Text_S014) &&
            GetControlWZ_byPosition(kControl_Picture_PI13))
        {
            GetControlWZ_byPosition(kControl_Text_S001)->ShowWindowWZ(WZ_HIDE);
            GetControlWZ_byPosition(kControl_Text_S013)->ShowWindowWZ(WZ_HIDE);
            GetControlWZ_byPosition(kControl_Text_S015)->ShowWindowWZ(WZ_HIDE);
            GetControlWZ_byPosition(kControl_Text_S014)->ShowWindowWZ(WZ_HIDE);
            GetControlWZ_byPosition(kControl_Picture_PI13)->ShowWindowWZ(WZ_HIDE);
        }
#if defined(_GLOBAL)
        if (GetControlWZ_byPosition(kControl_Text_S001) &&
            GetControlWZ_byPosition(kControl_Text_S013) &&
            GetControlWZ_byPosition(kControl_Text_S015) &&
            GetControlWZ_byPosition(kControl_Picture_PI13))
        {
            GetControlWZ_byPosition(kControl_Text_S001)->ShowWindowWZ(WZ_SHOW);

            GetControlWZ_byPosition(kControl_Text_S013)->ShowWindowWZ(WZ_SHOW);
            GetControlWZ_byPosition(kControl_Text_S015)->ShowWindowWZ(WZ_SHOW);
            GetControlWZ_byPosition(kControl_Picture_PI13)->ShowWindowWZ(WZ_SHOW);
        }

#elif defined(_YJW_SHOP_USING_MILEAGE)
        if (GetControlWZ_byPosition(kControl_Text_S014))
            GetControlWZ_byPosition(kControl_Text_S014)->ShowWindowWZ(WZ_SHOW);
#else //_KOREA
        if (GetControlWZ_byPosition(kControl_Text_S013) &&
            GetControlWZ_byPosition(kControl_Text_S015) &&
            GetControlWZ_byPosition(kControl_Text_S014) &&
            GetControlWZ_byPosition(kControl_Picture_PI13))
        {
            GetControlWZ_byPosition(kControl_Text_S013)->ShowWindowWZ(WZ_SHOW);
            GetControlWZ_byPosition(kControl_Text_S015)->ShowWindowWZ(WZ_SHOW);
            GetControlWZ_byPosition(kControl_Text_S014)->ShowWindowWZ(WZ_SHOW);
            GetControlWZ_byPosition(kControl_Picture_PI13)->ShowWindowWZ(WZ_SHOW);
        }
        
        //155499	구입 가격
        if (GetControlID_byPosition(kControl_Text_S013))
            GetStaticControlWZ(GetControlID_byPosition(kControl_Text_S013))->SetTextID(155499);
#endif //

        g_InterfaceManager.LockDlgs(uiConvenienceStore_Confirm::kDialog_ID);
    }
    else
    {
        g_KeyQueueManager.DeleteMsg(uiConvenienceStore_Confirm::kDialog_ID);

        g_InterfaceManager.UnLockDlgs();
    }
}
//------------------------------------------------------------------------------ 
void uiConvenienceStore_Confirm::InitFText(FTextParser& parser, FTextSentence& sentence)
{
    ftext_render().Release(); 

    CCtrlStaticWZ* area_control = GetStaticControlWZ(GetControlID_byPosition(kControl_Text_S003));
    if (area_control != NULL)
    {
        ftext_render().Release();
        ftext_render().Init(GetDrawBase(), area_control->GetSizeRect());
        ftext_render().SetVCenterAlign(false);
        ftext_render().ClearSentence();
        ftext_render().SetBeginLineIndex(0);

        parser.SetDefaultFont(area_control->GetFontID(), area_control->GetTextColor());
    }
}
//------------------------------------------------------------------------------ 
void uiConvenienceStore_Confirm::ShowInterface(bool is_show,    
                                               uiConvenienceStore_Purchase::PurchaseType purchase_type,
                                               ShopItemInfoEx* shop_item_info_ex, 
                                               int item_info_ex_count)
{
    uiConvenienceStore_Dialog::ShowInterface(is_show); 

    if (is_show)
    {
        if (ConvenienceStore::CashShop::instance() == NULL)
        {
            return;
        }
        if (ConvenienceStore::package_list() == NULL)
        {
            return;
        }
        if (GetControlWZ_byPosition(kControl_Button_B004) == NULL)
        {
            // 초기화(생성자호출)도 안됬는데 호출됨?
            return;
        }

        if (GetControlWZ_byPosition(kControl_Button_B004))
            GetControlWZ_byPosition(kControl_Button_B004)->EnableWindowWZ(true);
        if (GetControlWZ_byPosition(kControl_Button_B028))
            GetControlWZ_byPosition(kControl_Button_B028)->EnableWindowWZ(true);

        //------------------------------------------------------------------------------ 
        FTextParser parser;
        FTextSentence sentence;
        InitFText(parser, sentence);

        //------------------------------------------------------------------------------ 
        int buy_success_count = 0;
        for (int i = 0; i < item_info_ex_count; ++i)
        {
            if (shop_item_info_ex[i].error_code_ == 0)
            {
                ++buy_success_count;
            }
            else
            {
                TCHAR msg_error[INTERFACE_STRING_LENGTH] = {NULL, };
                DWORD msg_color;
                ConvenienceStore::CashShop::instance()->GetErrorStringByErrorCode(shop_item_info_ex[i].error_code_, msg_error, INTERFACE_STRING_LENGTH, &msg_color);
                GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, msg_error);
                g_InterfaceManager.ShowCenterMessageBox(msg_error, 5000, &msg_color, FALSE);
            }
        }
        if (purchase_type == uiConvenienceStore_Purchase::kPurchase_Buy)
        {
            //구매 확인
            if (GetControlID_byPosition(kControl_Text_S000))
                GetStaticControlWZ(GetControlID_byPosition(kControl_Text_S000))->SetTextID(155516);

            //155511	총 %s개의 아이템을 구입 하셨습니다.
            GlobalFunc::SetFTextFromStringTable(ftext_render(), parser, sentence, 155511, buy_success_count);
        }
        else if (purchase_type == uiConvenienceStore_Purchase::kPurchase_Gift)
        {
            //155517	선물 확인
            if (GetControlID_byPosition(kControl_Text_S000))
                GetStaticControlWZ(GetControlID_byPosition(kControl_Text_S000))->SetTextID(155517);

            //155512	총 %s개의 아이템을 선물 하셨습니다.
            GlobalFunc::SetFTextFromStringTable(ftext_render(), parser, sentence, 155512, buy_success_count);
        }

        //155570	<font=tt12><color=ffffffff>-----------------------------------------------</color></font>
        GlobalFunc::SetFTextFromStringTable(ftext_render(), parser, sentence, 155570);

        int total_buy_price = 0;
        for (int i = 0; i < item_info_ex_count; ++i)
        {
            CShopPackage* shop_package = NULL;

            if (ConvenienceStore::package_list()->GetValueByKey(shop_item_info_ex[i].package_product_sequence_, 
                                                                shop_package) == false)
            {
                continue;;
            }

            TCHAR msg[INTERFACE_STRING_LENGTH] = {NULL, };
            _tcscat(msg, _T("<color=fff0c979>"));
            _tcscat(msg,  shop_package->PackageProductName);
            _tcscat(msg, _T("</color>"));
            if (shop_item_info_ex[i].error_code_ != 0)
            {
                //155550 성공
                //155551 실패
                _tcscat(msg, _T(" <color=ffff0000>("));
                _tcscat(msg, (TCHAR*)g_InterfaceManager.GetInterfaceString(155551));
                _tcscat(msg, _T(")</color>"));
            }
            _tcscat(msg, _T("<return=1/>"));
            //------------------------------------------------------------------------------ 
            if (1 < shop_package->GetProductCount())
            {
                //! 묶음상품
                TCHAR str_unit[INTERFACE_STRING_LENGTH] = {NULL, };
                g_InterfaceManager.GetInterfaceString(155565, str_unit, INTERFACE_STRING_LENGTH);

                TCHAR str_price[INTERFACE_STRING_LENGTH] = {NULL, };
                GlobalFunc::MakeMessage_SignMoney(str_price, INTERFACE_STRING_LENGTH, shop_package->Price);

                TCHAR msg_price[INTERFACE_STRING_LENGTH] = {NULL, };
                GlobalFunc::MakeMessageFromStringTable(msg_price, INTERFACE_STRING_LENGTH, 155549,
                                            _T("1"),
                                            str_unit,
                                            str_price/*shop_package->Price*/,
                                            (TCHAR*)g_InterfaceManager.GetInterfaceString(155304));
                _tcscat(msg, _T("  "));
                _tcscat(msg,  msg_price);

                if (shop_item_info_ex[i].error_code_ == 0)
                {
                    total_buy_price += shop_package->Price;
                }
            }
            else
            {
                CShopProduct* shop_product = NULL;
                CShopProductLink* shop_product_link = NULL;
                if (ConvenienceStore::FindShopProduct(shop_package, 
                                                    shop_item_info_ex[i].price_sequence_, 
                                                    OUT shop_product, 
                                                    OUT shop_product_link) == false)
                {
                    continue;
                }

                TCHAR msg_price[INTERFACE_STRING_LENGTH] = {NULL, };
                ConvenienceStore::GetDisplayPriceStringByProductLink(msg_price, 
                                                    INTERFACE_STRING_LENGTH, 
                                                    ConvenienceStore::GetProductType(shop_product), 
                                                    shop_product_link,
                                                    shop_package->DeductMileageFlag == true );
                _tcscat(msg, _T("  "));
                _tcscat(msg,  msg_price);

                if (shop_item_info_ex[i].error_code_ == 0)
                {
                    total_buy_price += shop_product_link->Price;
                }
            }

            GlobalFunc::SetFTextFromMsg(ftext_render(), parser, sentence, msg);
        }
        ftext_render().convertLineInfo();
        ftext_render().SetBeginLineIndex(0);

        int max_line_count = ftext_render().GetMaxLine();
        int draw_line_count = ftext_render().GetDrawLineInRect();
        int scroll_max_num = max(0, max_line_count - draw_line_count);
        VScrollDialog::ResizeingVScrollControl(scroll_max_num, 0);

        //------------------------------------------------------------------------------ 
        // 토탈 구매 가격
        TCHAR msg_total_price[INTERFACE_STRING_LENGTH] = {NULL, };
        GlobalFunc::MakeMessage_SignMoney(msg_total_price, INTERFACE_STRING_LENGTH, total_buy_price);
        _tcscat(msg_total_price, (TCHAR*)g_InterfaceManager.GetInterfaceString(155304));
        if (GetControlWZ_byPosition(kControl_Text_S015))
            GetControlWZ_byPosition(kControl_Text_S015)->SetTextWZ(msg_total_price);
    }
}
//------------------------------------------------------------------------------ 
void uiConvenienceStore_Confirm::UpdateCashInfo()
{
    if (IsLoaded() == FALSE)
    {
        return;
    }

#if defined(_GLOBAL)
    if (GetControlWZ_byPosition(kControl_Text_S015))
        GetControlWZ_byPosition(kControl_Text_S015)->SetMoney(static_cast<ULONGLONG>(ConvenienceStore::CashShop::instance()->GetMyCash()));
    if (GetControlWZ_byPosition(kControl_Text_S016))
        GetControlWZ_byPosition(kControl_Text_S016)->SetMoney(static_cast<ULONGLONG>(ConvenienceStore::CashShop::instance()->GetMyPPCash()));
#else

    TCHAR msg_total_price[INTERFACE_STRING_LENGTH] = {NULL, };
    GlobalFunc::MakeMessage_SignMoney(msg_total_price, INTERFACE_STRING_LENGTH, static_cast<ULONGLONG>(ConvenienceStore::CashShop::instance()->GetMyCash()));
    _tcscat(msg_total_price, (TCHAR*)g_InterfaceManager.GetInterfaceString(155304));
    if (GetControlWZ_byPosition(kControl_Text_S016))
        GetControlWZ_byPosition(kControl_Text_S016)->SetTextWZ(msg_total_price);
    //GetControlWZ_byPosition(kControl_Text_S016)->SetMoney(static_cast<ULONGLONG>(cash_info.cash_));

#if defined(_YJW_SHOP_USING_MILEAGE)
    if (GetControlWZ_byPosition(kControl_Text_S015))
        GetControlWZ_byPosition(kControl_Text_S015)->SetMoney(static_cast<ULONGLONG>(ConvenienceStore::CashShop::instance()->GetMyMileage()));
#endif

#endif //
}
//------------------------------------------------------------------------------ 
void uiConvenienceStore_Confirm::SetRecvName(const TCHAR* char_name)
{
    if (IsLoaded() == FALSE)
    {
        return;
    }

    if (GetControlWZ_byPosition(kControl_Text_S004))
        GetControlWZ_byPosition(kControl_Text_S004)->SetTextWZ(char_name);
}
