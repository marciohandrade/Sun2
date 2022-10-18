#include "SunClientPrecompiledHeader.h"
#include "uiConvenienceStore_ItemDetail.h"

#include "InterfaceManager.h"
#include "uiConvenienceStoreMan.h"
#include "uiConvenienceStore_Dialog.h"
#include "GlobalFunc.h"
#include "Application.h"
#include "StorageItem.h"

//------------------------------------------------------------------------------ 
const DWORD uiConvenienceStore_ItemDetail::kDialog_ID = StrToWzID("4711");;

//------------------------------------------------------------------------------ 
//! static function
void uiConvenienceStore_ItemDetail::Callback_IconRender(CControlWZ* control_ptr, void* parameter)
{
    uiConvenienceStore_ItemDetail* onwer = static_cast<uiConvenienceStore_ItemDetail*>(parameter);
    if ((control_ptr != NULL) && (onwer != NULL))
    {
        if( ConvenienceStore::StorageItem* pData = reinterpret_cast<ConvenienceStore::StorageItem*>(control_ptr->GetUserData()) )
        {
            if( IconImage* pIconImage = pData->GetIconImage() )
            {
                pIconImage->Render(&control_ptr->GetSizeRect());
            }
        }
    }
}
//------------------------------------------------------------------------------ 
uiConvenienceStore_ItemDetail::uiConvenienceStore_ItemDetail(InterfaceManager* ui_man) : uiConvenienceStore_Dialog(ui_man)
{
    RegisterControlPosition(StrToWzID("B004"), kControl_Button_B004);
    RegisterControlPosition(StrToWzID("B028"), kControl_Button_B028);
    RegisterControlPosition(StrToWzID("S000"), kControl_Text_S000);
    RegisterControlPosition(StrToWzID("S002"), kControl_Text_S002);
    RegisterControlPosition(StrToWzID("S005"), kControl_Text_S005);
    RegisterControlPosition(StrToWzID("S001"), kControl_Text_S001);
    RegisterControlPosition(StrToWzID("S003"), kControl_Text_S003);
    RegisterControlPosition(StrToWzID("S004"), kControl_Text_S004);
    RegisterControlPosition(StrToWzID("E000"), kControl_Edit_E000);
    RegisterControlPosition(StrToWzID("P002"), kControl_Picture_P002);
    RegisterControlPosition(StrToWzID("P005"), kControl_Picture_P005);
#ifdef _NA_20111118_CASHSHOP_UI_RENEWAL
#else
    RegisterControlPosition(StrToWzID("P007"), kControl_Picture_P007);
#endif //_NA_20111118_CASHSHOP_UI_RENEWAL
    RegisterControlPosition(StrToWzID("P001"), kControl_Picture_P001);
    RegisterControlPosition(StrToWzID("P000"), kControl_Picture_P000);
    RegisterControlPosition(StrToWzID("P003"), kControl_Picture_P003);
    RegisterControlPosition(StrToWzID("P004"), kControl_Picture_P004);
    RegisterControlPosition(StrToWzID("P006"), kControl_Picture_P006);
    RegisterControlPosition(StrToWzID("P008"), kControl_Picture_P008);
}

//------------------------------------------------------------------------------ 
uiConvenienceStore_ItemDetail::~uiConvenienceStore_ItemDetail( void )
{
    Release();
}
//------------------------------------------------------------------------------ 
void uiConvenienceStore_ItemDetail::Init(CDrawBase* drawbase)
{
    ASSERT(true && "uiConvenienceStore_ItemDetail::Init false");
}
//------------------------------------------------------------------------------ 
void uiConvenienceStore_ItemDetail::Release(void)
{

}
//------------------------------------------------------------------------------ 
void uiConvenienceStore_ItemDetail::OnShowWindow(BOOL val)
{
    if (val == TRUE)
    {
        //------------------------------------------------------------------------------ 
        keyMsg msg;
        ZeroMemory(&msg,sizeof(msg)); 
        msg.dwType = uiConvenienceStore_ItemDetail::kDialog_ID;
        msg.wParam = uiConvenienceStore_ItemDetail::kDialog_ID;
        msg.DoSomething = GlobalFunc::DoShowWindowEx;
        g_KeyQueueManager.PushBack(msg);
        //------------------------------------------------------------------------------ 

        //------------------------------------------------------------------------------ 
        g_InterfaceManager.LockDlgs(uiConvenienceStore_ItemDetail::kDialog_ID);

        CCtrlMultilineEditWZ* multi_control = GetMultilineEditControlWZ(GetControlID_byPosition(kControl_Edit_E000));
        if (multi_control != NULL)
        {
            multi_control->OnFocusOut();
            multi_control->EnableWindowWZ(false);
            multi_control->GetVScroll()->ShowWindowWZ(WZ_HIDE);
        }
    }
    else
    {
        if (GetControlWZ_byPosition(kControl_Picture_P008))
            GetControlWZ_byPosition(kControl_Picture_P008)->SetUserData(NULL);
        if (GetControlWZ_byPosition(kControl_Picture_P008))
            GetControlWZ_byPosition(kControl_Picture_P008)->SetCallbackDraw(NULL, NULL);
        g_KeyQueueManager.DeleteMsg(uiConvenienceStore_ItemDetail::kDialog_ID);
        g_InterfaceManager.UnLockDlgs();
    }
}
//------------------------------------------------------------------------------ 
void uiConvenienceStore_ItemDetail::Process(DWORD tick)
{

}
//------------------------------------------------------------------------------ 
void uiConvenienceStore_ItemDetail::OnUpdateSolarDialog()
{
    CControlWZ* control = GetControlWZ_byPosition(kControl_Picture_P008);

    if ((IsOverControl(control) == true) && (control->GetUserData() != NULL))
    {
        GetManager()->RegisterTooltip(reinterpret_cast<ConvenienceStore::ShopItemObject*>(control->GetUserData()));

    }
}
//------------------------------------------------------------------------------ 
void uiConvenienceStore_ItemDetail::OnRenderSolarDialog()
{
#if WZENGINEVER >= 410
    if (g_InterfaceManager.GetDialogManager()->GetLockDlgID() == uiConvenienceStore_ItemDetail::kDialog_ID)
    { 
        POINT pt = g_pApplication->Get2DModeMapping();
        g_pSunRenderer->ColorFill(WzColor_RGBA(0,0,0,128), 0.0f, 0.0f, (float)pt.x,(float)pt.y);
        GetDialogWZ()->OnDraw(true);
    }
#endif //#if WZENGINEVER >= 410
}
//------------------------------------------------------------------------------ 
void uiConvenienceStore_ItemDetail::NetworkProc(MSG_BASE* msgbase)
{

}
//------------------------------------------------------------------------------
void uiConvenienceStore_ItemDetail::MessageProc(SI_MESSAGE* message)
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
    switch (message->eResultMsg)
    {
    case RT_MSG_LBUTTONCLICK:
        OnLButtonClick(message);
        break;
    default:
        SolarDialog::MessageProc(message);
        break;
    }
}
//------------------------------------------------------------------------------
bool uiConvenienceStore_ItemDetail::OnLButtonClick(SI_MESSAGE* message)
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
    default:
        {
        }
        break;
    }
    return result;
}
//------------------------------------------------------------------------------ 
void uiConvenienceStore_ItemDetail::ShowInterface(ConvenienceStore::ShopItemObject* pObject)
{
    if( pObject->IsStorageItem() == false )
        return;

    ConvenienceStore::StorageItem* pItemData = (ConvenienceStore::StorageItem*)pObject;

    uiConvenienceStore_Dialog::ShowInterface(TRUE);
    if (GetControlWZ_byPosition(kControl_Text_S001))
        GetControlWZ_byPosition(kControl_Text_S001)->SetTextWZA(pObject->GetName());

    //5781	보낸이
    TCHAR msg_text[INTERFACE_STRING_LENGTH] = {NULL, };
    ZeroMemory(msg_text, sizeof(msg_text));
    _tcscat(msg_text, (TCHAR*)g_InterfaceManager.GetInterfaceString(5781));
    _tcscat(msg_text, _T(" : "));
    _tcscat(msg_text, pItemData->GetSendCharacterName() );
    if (GetControlWZ_byPosition(kControl_Text_S003))
        GetControlWZ_byPosition(kControl_Text_S003)->SetTextWZ(msg_text);

    //155541	아이템 받기
    ZeroMemory(msg_text, sizeof(msg_text));
    ConvenienceStore::GetDisplayPriceStringByProductInfo(msg_text, INTERFACE_STRING_LENGTH, &(pItemData->GetProductInfo()), false);

    time_t temp_time = pItemData->GetDeleteDate();
    temp_time += (24 * 60 * 60);        //#define _DAY_SEC           (24 * 60 * 60)       /* secs in a day */
    tm local_tm = *(localtime(&temp_time));
    local_tm.tm_year += 1900 -1;
    local_tm.tm_mon += 1;

    // 받은 날짜
    TCHAR msg_time[INTERFACE_STRING_LENGTH] = {NULL, };
    Snprintf(msg_time, INTERFACE_STRING_LENGTH, " %04d.%02d.%02d", local_tm.tm_year, local_tm.tm_mon, local_tm.tm_mday);

    _tcscat(msg_text, " / ");
    _tcscat(msg_text, msg_time);
    _tcscat(msg_text, " ");
    _tcscat(msg_text, g_InterfaceManager.GetInterfaceString(155576)); // '받음'

    if (GetControlWZ_byPosition(kControl_Text_S005))
        GetControlWZ_byPosition(kControl_Text_S005)->SetTextWZ(msg_text);

    //! 내용
    //GetControlWZ_byPosition(kControl_Text_S004)->SetTextWZ(storage_recv->info_ex.gift_message_);
    if (GetControlWZ_byPosition(kControl_Edit_E000))
        GetControlWZ_byPosition(kControl_Edit_E000)->SetTextWZ(pItemData->GetGiftMessage());

    if (GetControlWZ_byPosition(kControl_Picture_P008))
        GetControlWZ_byPosition(kControl_Picture_P008)->SetUserData(reinterpret_cast<DWORD>(pItemData));
    if (GetControlWZ_byPosition(kControl_Picture_P008))
        GetControlWZ_byPosition(kControl_Picture_P008)->SetCallbackDraw(uiConvenienceStore_ItemDetail::Callback_IconRender, this);
}
