#include "SunClientPrecompiledHeader.h"

#include "uiConvenienceStore_Ask.h"
#include "InterfaceManager.h"
#include "uiConvenienceStoreMan.h"
#include "uiConvenienceStore_Dialog.h"
#include "uiConvenienceStore_Confirm.h"
#include "GlobalFunc.h"
#include "Application.h"
#include "FTextParser.h"
#include "FTextSentence.h"
#include "uiConvenienceStore_Counter.h"
#include "uiConvenienceStore_Purchase.h"
#include "uiConvenienceStore_Basket.h"
#include "PurchaseItem.h"
#include "StorageItem.h"

//------------------------------------------------------------------------------ 
//155360	알림
//155507	정말 구입 하시겠습니까?
//155508	정말 선물 하시겠습니까?
//155454	구매 후 7일 이내 구매 철회가 가능하며, 사용된 아이템 및\n거래로 주고 받은 아이템은 구매철회가 불가능합니다.
//155455	선물보내기로 전송된 아이템은 구매철회가 불가능합니다.
//155509	구매 후 7일 이내 구매 철회가 가능하며,\n사용된 아이템 및 거래로 주고 받은 아이템은\n구매 철회가 불가능합니다.
//155510	선물 보내기로 전송된 아이템은 구매 철회가\n불가능 합니다.
//------------------------------------------------------------------------------ 
enum UIControlPos
{
    kControl_CheckBT_C000 = 0,
    kControl_Button_B004,
    kControl_Button_B028,
    kControl_Button_B000,
    kControl_Button_B001,
    kControl_Text_S000,
    kControl_Text_S001,
    kControl_Text_S002,
    kControl_Text_S003,
    kControl_VScroll_V000,
    kControl_Picture_P002,
    kControl_Picture_P007,
    kControl_Picture_P000,
    kControl_Picture_P001,
    kControl_Size,
};
//------------------------------------------------------------------------------ 
const DWORD uiConvenienceStore_Ask::kDialog_ID = StrToWzID("0827");;

//------------------------------------------------------------------------------ 
uiConvenienceStore_Ask::uiConvenienceStore_Ask(InterfaceManager* ui_man) : uiBase(ui_man), owner_(0)
#ifdef _NA_000000_20130710_CHARGE_SUB_FIXED_AMOUNT_EX2
,is_now_ask_item_(NULL)
#endif
{
    RegisterControlPosition(StrToWzID("C000"), kControl_CheckBT_C000);
    RegisterControlPosition(StrToWzID("B004"), kControl_Button_B004);
    RegisterControlPosition(StrToWzID("B028"), kControl_Button_B028);
    RegisterControlPosition(StrToWzID("B000"), kControl_Button_B000);
    RegisterControlPosition(StrToWzID("B001"), kControl_Button_B001);
    RegisterControlPosition(StrToWzID("S000"), kControl_Text_S000);
    RegisterControlPosition(StrToWzID("S001"), kControl_Text_S001);
    RegisterControlPosition(StrToWzID("S002"), kControl_Text_S002);
    RegisterControlPosition(StrToWzID("S003"), kControl_Text_S003);
    RegisterControlPosition(StrToWzID("V000"), kControl_VScroll_V000);
    RegisterControlPosition(StrToWzID("P002"), kControl_Picture_P002);
    RegisterControlPosition(StrToWzID("P007"), kControl_Picture_P007);
    RegisterControlPosition(StrToWzID("P000"), kControl_Picture_P000);
    RegisterControlPosition(StrToWzID("P001"), kControl_Picture_P001);

    ask_show_type_ = uiConvenienceStore_Dialog::kAskShow_Size;
}

//------------------------------------------------------------------------------ 
uiConvenienceStore_Ask::~uiConvenienceStore_Ask( void )
{
    Release();
}
//------------------------------------------------------------------------------ 
void uiConvenienceStore_Ask::Init(CDrawBase* drawbase)
{
    ASSERT(true && "uiConvenienceStore_Ask::Init false");
}
//------------------------------------------------------------------------------ 
void uiConvenienceStore_Ask::Release(void)
{

}

//------------------------------------------------------------------------------ 
void uiConvenienceStore_Ask::_OnClickEscape(DWORD wParam,DWORD lParam)
{
    if( uiConvenienceStore_Ask* pDlg = (uiConvenienceStore_Ask*)g_InterfaceManager.GetDialog((WzID)wParam) )
    {
        if( pDlg->IsEnableCloseButton() )
        {
            pDlg->OnCancel();
        }
    }
}

//------------------------------------------------------------------------------ 
void uiConvenienceStore_Ask::OnShowWindow(BOOL val)
{
    if (val == TRUE)
    {
        //------------------------------------------------------------------------------ 
        keyMsg msg;
        ZeroMemory(&msg,sizeof(msg)); 
        msg.dwType = uiConvenienceStore_Ask::kDialog_ID;
        msg.wParam = uiConvenienceStore_Ask::kDialog_ID;
        msg.DoSomething = _OnClickEscape;
        g_KeyQueueManager.PushBack(msg);
        //------------------------------------------------------------------------------ 

#if WZENGINEVER >= 410
        //155360	알림
        if (GetControlWZ_byPosition(kControl_Text_S000))
        {
            GetControlWZ_byPosition(kControl_Text_S000)->SetTextID(155360);
        }
#endif //#if WZENGINEVER >= 410

        g_InterfaceManager.LockDlgs(uiConvenienceStore_Ask::kDialog_ID);
    }
    else
    {
        g_KeyQueueManager.DeleteMsg(uiConvenienceStore_Ask::kDialog_ID);

        if ((owner() != NULL) &&
            (owner()->IsVisible() == TRUE) && 
            (owner()->IsNeedLockDialog() == true)) 
        {
            g_InterfaceManager.LockDlgs(owner()->GetDialogWZ()->GetDlgID());
        }
        else
        {
            g_InterfaceManager.UnLockDlgs();
        }
    }
}
//------------------------------------------------------------------------------ 
void uiConvenienceStore_Ask::Process(DWORD tick)
{

}
//------------------------------------------------------------------------------ 
void uiConvenienceStore_Ask::OnUpdateSolarDialog()
{
#if WZENGINEVER >= 410
    DWORD lock_dlg_id = g_InterfaceManager.GetDialogManager()->GetLockDlgID();
    if ((lock_dlg_id != 0) && (lock_dlg_id != uiConvenienceStore_Ask::kDialog_ID))
    {
        return; 
    }
#endif //#if WZENGINEVER >= 410


    CCtrlStaticWZ* area_control = NULL;
    if (GetControlWZ_byPosition(kControl_CheckBT_C000) &&
        GetControlWZ_byPosition(kControl_CheckBT_C000)->GetState() == WZ_SHOW)
    {
        area_control = GetStaticControlWZ(GetControlID_byPosition(kControl_Text_S001));
    }
    else
    {
        area_control = GetStaticControlWZ(GetControlID_byPosition(kControl_Text_S003));
    }
    if (area_control != NULL)
    {
        ftext_render().SetRect(area_control->GetSizeRect());
    }

    //------------------------------------------------------------------------------ 
    VScrollDialog::Update();
}
//------------------------------------------------------------------------------ 
void uiConvenienceStore_Ask::OnRenderSolarDialog()
{ 
#if WZENGINEVER >= 410
    if (g_InterfaceManager.GetDialogManager()->GetLockDlgID() == uiConvenienceStore_Ask::kDialog_ID)
    { 
        POINT pt = g_pApplication->Get2DModeMapping();
        g_pSunRenderer->ColorFill(WzColor_RGBA(0,0,0,128), 0.0f, 0.0f, (float)pt.x,(float)pt.y);
        GetDialogWZ()->OnDraw(true);
    }
#endif //#if WZENGINEVER >= 410

    ftext_render().Render();
}
//------------------------------------------------------------------------------ 
void uiConvenienceStore_Ask::NetworkProc(MSG_BASE* msgbase)
{

}
//------------------------------------------------------------------------------
void uiConvenienceStore_Ask::MessageProc(SI_MESSAGE* message)
{
    if (IsLock())
    {
        return;
    }
    //if (GetManager() == NULL)
    //{
    //    return;
    //}

    //------------------------------------------------------------------------------ 
    switch (message->eResultMsg)
    {
    case RT_MSG_LBUTTONCLICK:
        OnLButtonClick(message);
        break;

    default:
        uiBase::MessageProc(message);
        VScrollDialog::MessageProc(message);
        break;
    }
}
//------------------------------------------------------------------------------ 
CCtrlVScrollWZ* uiConvenienceStore_Ask::vscroll_control()
{
    return GetVScrollControlWZ(GetControlID_byPosition(kControl_VScroll_V000));
}
//------------------------------------------------------------------------------ 
bool uiConvenienceStore_Ask::OnVScrollThumb(SI_MESSAGE* msg)
{
    if (vscroll_control() && VScrollDialog::OnVScrollThumb(msg) == true)
    {
        ftext_render().SetBeginLineIndex(vscroll_control()->GetScrollPosWZ());

        return true;
    }
    return false;
}
//------------------------------------------------------------------------------ 
bool uiConvenienceStore_Ask::OnVScrollUp(SI_MESSAGE* msg)
{
    if (vscroll_control() && VScrollDialog::OnVScrollUp(msg) == true)
    {
        ftext_render().SetBeginLineIndex(vscroll_control()->GetScrollPosWZ());

        return true;
    }
    return false;
}
//------------------------------------------------------------------------------ 
bool uiConvenienceStore_Ask::OnVScrollDown(SI_MESSAGE* msg)
{
    if (vscroll_control() && VScrollDialog::OnVScrollDown(msg) == true)
    {
        ftext_render().SetBeginLineIndex(vscroll_control()->GetScrollPosWZ());

        return true;
    }
    return false;
}
//------------------------------------------------------------------------------
void uiConvenienceStore_Ask::OnOK()
{
    uiBase::ShowInterface(FALSE);
    if (owner() != NULL)
    {
        owner()->OnClickAskDialog(uiConvenienceStore_Dialog::kAskCommand_OK, ask_show_type_, user_data_);
    }
}

//------------------------------------------------------------------------------
void uiConvenienceStore_Ask::OnCancel()
{
    uiBase::ShowInterface(FALSE); 
    if (owner() != NULL)
    {
        owner()->OnClickAskDialog(uiConvenienceStore_Dialog::kAskCommand_Cancel, ask_show_type_, user_data_);
    }
}

//------------------------------------------------------------------------------
bool uiConvenienceStore_Ask::OnLButtonClick(SI_MESSAGE* message)
{
    bool result = true;
    switch (GetControlPosition_byID(message->dwCtrlID))
    {
    case kControl_CheckBT_C000:
        {
            if (GetControlID_byPosition(kControl_CheckBT_C000) && GetControlWZ_byPosition(kControl_Button_B000))
            {
                bool is_checked = GetCheckButtonControlWZ(GetControlID_byPosition(kControl_CheckBT_C000))->GetCheckState();

                GetControlWZ_byPosition(kControl_Button_B000)->EnableWindowWZ(is_checked);
            }
        }
        break;
    case kControl_Button_B004:
    case kControl_Button_B028:
        {
            OnCancel();
        }
        break;
    case kControl_Button_B000:
    case kControl_Button_B001:
        {
            OnOK();
        }
    default:
        {
        }
        break;
    }
    return result;
}
//------------------------------------------------------------------------------ 
bool uiConvenienceStore_Ask::GetScrollArea(RECT& area)
{
    if (GetControlWZ_byPosition(kControl_CheckBT_C000) &&
        GetControlWZ_byPosition(kControl_CheckBT_C000)->GetState() == WZ_SHOW)
    {
        //! 작은창
        if (GetControlWZ_byPosition(kControl_Text_S001) &&
            IsOverControl(GetControlWZ_byPosition(kControl_Text_S001)) == true)
        {
            area = GetControlWZ_byPosition(kControl_Text_S001)->GetSizeRect();
            return true;
        }
    }
    else
    {
        //! 큰창
        if (GetControlWZ_byPosition(kControl_Text_S003) &&
            IsOverControl(GetControlWZ_byPosition(kControl_Text_S003)) == true)
        {
            area = GetControlWZ_byPosition(kControl_Text_S003)->GetSizeRect();
            return true;
        }
    }
    return false;
}

//------------------------------------------------------------------------------ 
void uiConvenienceStore_Ask::AppendFText(const TCHAR* msg)
{
    if (IsLoaded() == FALSE || IsVisible() == FALSE)
    {
        return;
    }
    FTextParser parser;
    FTextSentence sentence;
    CCtrlStaticWZ* area_control = NULL;// GetStaticControlWZ(GetControlID_byPosition(kControl_Text_S003));
    if (GetControlWZ_byPosition(kControl_CheckBT_C000) == NULL ||
        GetControlID_byPosition(kControl_Text_S001) == NULL || 
        GetControlID_byPosition(kControl_Text_S003) == NULL)
    {
        return;
    }
    if (GetControlWZ_byPosition(kControl_CheckBT_C000)->GetState() == WZ_SHOW)
    {
        area_control = GetStaticControlWZ(GetControlID_byPosition(kControl_Text_S001));
    }
    else
    {
        area_control = GetStaticControlWZ(GetControlID_byPosition(kControl_Text_S003));
    }
    if (area_control != NULL)
    {
        parser.SetDefaultFont(area_control->GetFontID(), area_control->GetTextColor());
    }

    GlobalFunc::SetFTextFromMsg(ftext_render(), parser, sentence, msg);

    UpdateTextScroll();
}

//------------------------------------------------------------------------------ 
bool uiConvenienceStore_Ask::IsEnableCloseButton()
{
    if (GetControlWZ_byPosition(kControl_Button_B004) == NULL ||
        GetControlWZ_byPosition(kControl_Button_B001) == NULL)
    {
        return false;
    }
    return GetControlWZ_byPosition(kControl_Button_B004)->GetDrawState() != E_DISABLE ||
        GetControlWZ_byPosition(kControl_Button_B001)->GetDrawState() != E_DISABLE;
}
//------------------------------------------------------------------------------ 
void uiConvenienceStore_Ask::DisableCloseButton()
{
    //! 확인
    if (GetControlWZ_byPosition(kControl_Button_B004))
    {
        GetControlWZ_byPosition(kControl_Button_B004)->EnableWindowWZ(false);
    }
    if (GetControlWZ_byPosition(kControl_Button_B001))
    {
        GetControlWZ_byPosition(kControl_Button_B001)->EnableWindowWZ(false);
    }
}
//------------------------------------------------------------------------------ 
void uiConvenienceStore_Ask::EnableCloseButton()
{
    if (GetControlWZ_byPosition(kControl_Button_B000) &&
        GetControlWZ_byPosition(kControl_Button_B000)->GetState() == WZ_SHOW)
    {
        // 확인, 취소
        if (GetControlWZ_byPosition(kControl_Button_B004))
            GetControlWZ_byPosition(kControl_Button_B004)->EnableWindowWZ(true);
        if (GetControlWZ_byPosition(kControl_Button_B028))
            GetControlWZ_byPosition(kControl_Button_B028)->EnableWindowWZ(true);
        if (GetControlWZ_byPosition(kControl_Button_B000))
            GetControlWZ_byPosition(kControl_Button_B000)->EnableWindowWZ(true);
    }
    else if (GetControlWZ_byPosition(kControl_Button_B001) &&
        GetControlWZ_byPosition(kControl_Button_B001)->GetState() == WZ_SHOW)
    {
        //! 확인
        if (GetControlWZ_byPosition(kControl_Button_B004))
            GetControlWZ_byPosition(kControl_Button_B004)->EnableWindowWZ(true);
        if (GetControlWZ_byPosition(kControl_Button_B001))
            GetControlWZ_byPosition(kControl_Button_B001)->EnableWindowWZ(true);
    }
}

//------------------------------------------------------------------------------ 
void uiConvenienceStore_Ask::InitFText(FTextParser& parser)
{
    ftext_render().Release(); 

    CCtrlStaticWZ* area_control = NULL;
    if (GetControlWZ_byPosition(kControl_CheckBT_C000) &&
        GetControlWZ_byPosition(kControl_CheckBT_C000)->GetState() == WZ_SHOW)
    {
        if (GetControlID_byPosition(kControl_Text_S001))
        {
            area_control = GetStaticControlWZ(GetControlID_byPosition(kControl_Text_S001));
        }
    }
    else
    {
        if (GetControlID_byPosition(kControl_Text_S003))
        {
            area_control = GetStaticControlWZ(GetControlID_byPosition(kControl_Text_S003));
        }
    }

    if (area_control != NULL)
    {
        ftext_render().Init(GetDrawBase(), area_control->GetSizeRect());
        ftext_render().SetVCenterAlign(false);
        ftext_render().ClearSentence();
        ftext_render().SetBeginLineIndex(0);

        parser.SetDefaultFont(area_control->GetFontID(), area_control->GetTextColor());
    }
}

void uiConvenienceStore_Ask::MakeItemFullName( ConvenienceStore::ShopItemObject* item_object_ptr, OUT LPTSTR full_name )
{
    _tcscat(full_name, _T("<color=fff0c979>"));
    _tcscat(full_name,  item_object_ptr->GetName());
    _tcscat(full_name, _T("</color>"));

    if( item_object_ptr->IsStorageItem() )
    {
        ConvenienceStore::StorageItem* item = (ConvenienceStore::StorageItem*)item_object_ptr;

        TCHAR msg_text[INTERFACE_STRING_LENGTH] = {NULL, };
        ConvenienceStore::GetDisplayPriceStringByProductInfo(msg_text, INTERFACE_STRING_LENGTH, &(item->GetProductInfo() ), false );

        _tcscat(full_name, _T("  "));
        _tcscat(full_name,  msg_text);
    }
    else if( item_object_ptr->IsPurchase() )
    {
        ConvenienceStore::PurchaseItem* item = (ConvenienceStore::PurchaseItem*)item_object_ptr;

        if (1 < item->GetProductCount() )
        {
            //! 묶음상품
            TCHAR str_unit[INTERFACE_STRING_LENGTH] = {NULL, };
            g_InterfaceManager.GetInterfaceString(155565, str_unit, INTERFACE_STRING_LENGTH);

            TCHAR str_price[INTERFACE_STRING_LENGTH] = {NULL, };
            GlobalFunc::MakeMessage_SignMoney(str_price, INTERFACE_STRING_LENGTH, item->GetPrice() );

            TCHAR msg_price[INTERFACE_STRING_LENGTH] = {NULL, };
            GlobalFunc::MakeMessageFromStringTable(msg_price, INTERFACE_STRING_LENGTH, 155549,
                _T("1"),
                str_unit,
                str_price/*shop_package->Price*/,
                (TCHAR*)g_InterfaceManager.GetInterfaceString(155304));

            _tcscat(full_name, _T("  "));
            _tcscat(full_name,  msg_price);
        }
        else
        {
            TCHAR msg_text[INTERFACE_STRING_LENGTH] = {NULL, };
            ConvenienceStore::GetDisplayPriceStringByProductInfo(msg_text, INTERFACE_STRING_LENGTH, &(item->GetCurProductInfo() ), item->is_mileage_only() );

            _tcscat(full_name, _T("  "));
            _tcscat(full_name,  msg_text);
        }
    }
    else if( item_object_ptr->IsProductItem() )
    {
        ConvenienceStore::ProductItem* item = (ConvenienceStore::ProductItem*)item_object_ptr;

        if (1 < item->GetProductCount() )
        {
            //! 묶음상품
            TCHAR str_unit[INTERFACE_STRING_LENGTH] = {NULL, };
            g_InterfaceManager.GetInterfaceString(155565, str_unit, INTERFACE_STRING_LENGTH);

            TCHAR str_price[INTERFACE_STRING_LENGTH] = {NULL, };
            GlobalFunc::MakeMessage_SignMoney(str_price, INTERFACE_STRING_LENGTH, item->GetPrice() );

            TCHAR msg_price[INTERFACE_STRING_LENGTH] = {NULL, };
            GlobalFunc::MakeMessageFromStringTable(msg_price, INTERFACE_STRING_LENGTH, 155549,
                _T("1"),
                str_unit,
                str_price/*shop_package->Price*/,
                (TCHAR*)g_InterfaceManager.GetInterfaceString(155304));

            _tcscat(full_name, _T("  "));
            _tcscat(full_name,  msg_price);
        }
        else
        {
            TCHAR msg_text[INTERFACE_STRING_LENGTH] = {NULL, };
            //ConvenienceStore::GetDisplayPriceStringByProductInfo(msg_text, INTERFACE_STRING_LENGTH, &(item->GetCurProductInfo() ) );

            _tcscat(full_name, _T("  "));
            _tcscat(full_name,  msg_text);
        }
    }
}

void uiConvenienceStore_Ask::OpenAsk( uiConvenienceStore_Dialog* owner, uiConvenienceStore_Dialog::AskShowType ask_show_type )
{
    uiBase::ShowInterface(TRUE);

    owner_ = owner;
    user_data_ = NULL;
    ask_show_type_ = ask_show_type;

    UpdateButtonState();

    FTextParser parser;
    InitFText(parser);
}

void uiConvenienceStore_Ask::UpdateButtonState()
{
    if (GetControlWZ_byPosition(kControl_Button_B028))
        GetControlWZ_byPosition(kControl_Button_B028)->ShowWindowWZ(WZ_HIDE);
    if (GetControlWZ_byPosition(kControl_Button_B000))
        GetControlWZ_byPosition(kControl_Button_B000)->ShowWindowWZ(WZ_HIDE);
    if (GetControlWZ_byPosition(kControl_Picture_P007))
        GetControlWZ_byPosition(kControl_Picture_P007)->ShowWindowWZ(WZ_HIDE);
    if (GetControlWZ_byPosition(kControl_Button_B001))
        GetControlWZ_byPosition(kControl_Button_B001)->ShowWindowWZ(WZ_HIDE);
    if (GetControlWZ_byPosition(kControl_Picture_P001))
        GetControlWZ_byPosition(kControl_Picture_P001)->ShowWindowWZ(WZ_SHOW);
    if (GetControlWZ_byPosition(kControl_CheckBT_C000))
        GetControlWZ_byPosition(kControl_CheckBT_C000)->ShowWindowWZ(WZ_HIDE);
    if (GetControlWZ_byPosition(kControl_Text_S002))
        GetControlWZ_byPosition(kControl_Text_S002)->ShowWindowWZ(WZ_HIDE);

    switch (ask_show_type_)
    {
    case uiConvenienceStore_Dialog::kAskShow_Buy:
    case uiConvenienceStore_Dialog::kAskShow_Gift:
        if (GetControlWZ_byPosition(kControl_Button_B028))
            GetControlWZ_byPosition(kControl_Button_B028)->ShowWindowWZ(WZ_SHOW);
        if (GetControlWZ_byPosition(kControl_Button_B000))
            GetControlWZ_byPosition(kControl_Button_B000)->ShowWindowWZ(WZ_SHOW);
        if (GetControlWZ_byPosition(kControl_Picture_P007))
            GetControlWZ_byPosition(kControl_Picture_P007)->ShowWindowWZ(WZ_SHOW);
        if (GetControlWZ_byPosition(kControl_Button_B000))
            GetControlWZ_byPosition(kControl_Button_B000)->EnableWindowWZ(false);

        if (GetControlWZ_byPosition(kControl_CheckBT_C000))
            GetControlWZ_byPosition(kControl_CheckBT_C000)->ShowWindowWZ(WZ_SHOW);
        if (GetControlWZ_byPosition(kControl_Text_S002))
            GetControlWZ_byPosition(kControl_Text_S002)->ShowWindowWZ(WZ_SHOW);
        if (GetControlID_byPosition(kControl_CheckBT_C000))
            GetCheckButtonControlWZ(GetControlID_byPosition(kControl_CheckBT_C000))->SetCheckState(false);
        break;

    case uiConvenienceStore_Dialog::kAskShow_StorageBuy_OneRecv:
    case uiConvenienceStore_Dialog::kAskShow_StorageBuy_AllRecv:
    case uiConvenienceStore_Dialog::kAskShow_StorageGift_OneRecv:
    case uiConvenienceStore_Dialog::kAskShow_StorageGift_AllRecv:
#ifdef _NA_000000_20130710_CHARGE_SUB_FIXED_AMOUNT_EX2
    case uiConvenienceStore_Dialog::KAskShow_ItemCashNewType:
    case uiConvenienceStore_Dialog::KAskShow_ItemCashGifeNewType:
    case uiConvenienceStore_Dialog::kAskShow_StorageNewTypeBuy_AllRecv:
    case uiConvenienceStore_Dialog::kAskShow_StorageNewTypeGift_AllRecv:
#endif
        if (GetControlWZ_byPosition(kControl_Button_B028))
            GetControlWZ_byPosition(kControl_Button_B028)->ShowWindowWZ(WZ_SHOW);
        if (GetControlWZ_byPosition(kControl_Button_B000))
            GetControlWZ_byPosition(kControl_Button_B000)->ShowWindowWZ(WZ_SHOW);
        if (GetControlWZ_byPosition(kControl_Picture_P007))
            GetControlWZ_byPosition(kControl_Picture_P007)->ShowWindowWZ(WZ_SHOW);
        break;

    case uiConvenienceStore_Dialog::kAskShow_Message:
        if (GetControlWZ_byPosition(kControl_Button_B001))
            GetControlWZ_byPosition(kControl_Button_B001)->ShowWindowWZ(WZ_SHOW);
        if (GetControlWZ_byPosition(kControl_Picture_P001))
            GetControlWZ_byPosition(kControl_Picture_P001)->ShowWindowWZ(WZ_SHOW);
        break;

    case uiConvenienceStore_Dialog::kAskShow_StorageBuy_AllRecv_Confirm: //!< 받고 나서 확인창
    case uiConvenienceStore_Dialog::kAskShow_StorageGift_AllRecv_Confirm: //!< 받고 나서 확인창
        if (GetControlWZ_byPosition(kControl_Button_B001))
            GetControlWZ_byPosition(kControl_Button_B001)->ShowWindowWZ(WZ_SHOW);
        if (GetControlWZ_byPosition(kControl_Picture_P001))
            GetControlWZ_byPosition(kControl_Picture_P001)->ShowWindowWZ(WZ_SHOW);
        if (GetControlWZ_byPosition(kControl_Button_B001))
            GetControlWZ_byPosition(kControl_Button_B001)->EnableWindowWZ(false);
        if (GetControlWZ_byPosition(kControl_Button_B004))
            GetControlWZ_byPosition(kControl_Button_B004)->EnableWindowWZ(false);
        break;
    }
}


void uiConvenienceStore_Ask::UpdateTextScroll()
{
    ftext_render().convertLineInfo();
    ftext_render().SetBeginLineIndex(0);

    int max_line_count = ftext_render().GetMaxLine();
    int draw_line_count = ftext_render().GetDrawLineInRect();
    int scroll_max_num = max(0, max_line_count - draw_line_count);
    VScrollDialog::ResizeingVScrollControl(scroll_max_num, 0);
}

void uiConvenienceStore_Ask::AppendItemText( ConvenienceStore::ShopItemObject* object )
{
    is_now_ask_item_ = object;
    TCHAR full_name[INTERFACE_STRING_LENGTH] = {NULL, };
    MakeItemFullName(object, full_name);

    AppendFText(full_name);
}
#ifdef _NA_000000_20130710_CHARGE_SUB_FIXED_AMOUNT_EX2
void uiConvenienceStore_Ask::AppendItemTextToString( DWORD index,ConvenienceStore::ShopItemObject* object )
{
    is_now_ask_item_ = object;

    TCHAR full_name[INTERFACE_STRING_LENGTH] = {NULL, };
    TCHAR String_Final[INTERFACE_STRING_LENGTH] = {NULL, };
    MakeItemFullName(object, full_name);

    g_InterfaceManager.GetInterfaceStringFormat(String_Final,INTERFACE_STRING_LENGTH,index,full_name);

    AppendFText(String_Final);

}
#endif