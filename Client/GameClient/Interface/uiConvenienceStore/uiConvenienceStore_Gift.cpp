#include "SunClientPrecompiledHeader.h"

#include "uiConvenienceStore_Gift.h"
#include "uiConvenienceStoreMan.h"
#include "InterfaceManager.h"
#include "uiConvenienceStore_Purchase.h"
#include "uiConvenienceStore_Counter.h"
#include "GlobalFunc.h"
#include "uiConvenienceStore_FriendList.h"
#include "uiCommunityMan\uiCommunityMan.h"
#include "uiCommunityInfo\uiCommunityInfo.h"
#include "Application.h"
#include "uiSystemMan\uiSystemMan.h"

//------------------------------------------------------------------------------ 
const DWORD uiConvenienceStore_Gift::kDialog_ID = StrToWzID("0478");;

//------------------------------------------------------------------------------ 
uiConvenienceStore_Gift::uiConvenienceStore_Gift(InterfaceManager* ui_man) : uiConvenienceStore_Dialog(ui_man)
{
    RegisterControlPosition(StrToWzID("B004"), kControl_Button_B004);
    RegisterControlPosition(StrToWzID("B028"), kControl_Button_B028);
    RegisterControlPosition(StrToWzID("B000"), kControl_Button_B000);
    RegisterControlPosition(StrToWzID("B001"), kControl_Button_B001);
    RegisterControlPosition(StrToWzID("B002"), kControl_Button_B002);
    RegisterControlPosition(StrToWzID("S000"), kControl_Text_S000);
    RegisterControlPosition(StrToWzID("S002"), kControl_Text_S002);
    RegisterControlPosition(StrToWzID("S004"), kControl_Text_S004);
    RegisterControlPosition(StrToWzID("S005"), kControl_Text_S005);
    RegisterControlPosition(StrToWzID("S006"), kControl_Text_S006);
    RegisterControlPosition(StrToWzID("S001"), kControl_Text_S001);
    RegisterControlPosition(StrToWzID("E000"), kControl_Edit_E000);
    RegisterControlPosition(StrToWzID("E001"), kControl_Edit_E001);
    RegisterControlPosition(StrToWzID("P002"), kControl_Picture_P002);
    RegisterControlPosition(StrToWzID("P005"), kControl_Picture_P005);
    RegisterControlPosition(StrToWzID("P007"), kControl_Picture_P007);
    RegisterControlPosition(StrToWzID("P000"), kControl_Picture_P000);
    RegisterControlPosition(StrToWzID("P001"), kControl_Picture_P001);
    RegisterControlPosition(StrToWzID("P003"), kControl_Picture_P003);

    product_list().clear();
}
//------------------------------------------------------------------------------ 
uiConvenienceStore_Gift::~uiConvenienceStore_Gift( void )
{
    Release();
}
//------------------------------------------------------------------------------ 
void uiConvenienceStore_Gift::Init(CDrawBase* drawbase)
{
    CCtrlDxEditBox* name_editbox_control = GetDxEditBoxControlWZ(GetControlID_byPosition(kControl_Edit_E000));
    if (name_editbox_control != NULL)
    {
        name_editbox_control->SetLimitText(MAX_CHARNAME_LENGTH);
#if WZENGINEVER >= 420
        name_editbox_control->SetLimitText_MultibyteCheck(true); 
#endif //#if WZENGINEVER >= 410
    }
    CCtrlDxEditBox* meno_editbox_control = GetDxEditBoxControlWZ(GetControlID_byPosition(kControl_Edit_E001));
    if (meno_editbox_control != NULL)
    {
        meno_editbox_control->SetLimitText(GIFT_MESSAGE_LENGTH - 1);
#if WZENGINEVER >= 420
        meno_editbox_control->SetLimitText_MultibyteCheck(true);
#endif //#if WZENGINEVER >= 410
    }

    ASSERT(true && "uiConvenienceStore_Gift::Init false");
}
//------------------------------------------------------------------------------ 
void uiConvenienceStore_Gift::OnShowWindow(BOOL val)
{
    if (val == TRUE)
    {
        //------------------------------------------------------------------------------ 
        keyMsg msg;
        ZeroMemory(&msg,sizeof(msg));
        msg.dwType = uiConvenienceStore_Gift::kDialog_ID;
        msg.wParam = uiConvenienceStore_Gift::kDialog_ID; 
        msg.DoSomething = GlobalFunc::DoShowWindowEx;
        g_KeyQueueManager.PushBack(msg);
        //------------------------------------------------------------------------------ 

        //155501	선물 하기
        if (GetControlID_byPosition(kControl_Text_S000) &&
            GetControlWZ_byPosition(kControl_Button_B001) &&
            GetControlWZ_byPosition(kControl_Button_B028) &&
            GetControlWZ_byPosition(kControl_Text_S005) &&
            GetControlWZ_byPosition(kControl_Text_S001) &&
            GetControlWZ_byPosition(kControl_Text_S006) &&
            GetControlWZ_byPosition(kControl_Edit_E000) &&
            GetControlWZ_byPosition(kControl_Edit_E000) &&
            GetControlWZ_byPosition(kControl_Edit_E001) &&
            GetControlWZ_byPosition(kControl_Edit_E001))
        {
            GetStaticControlWZ(GetControlID_byPosition(kControl_Text_S000))->SetTextID(155501);
            GetControlWZ_byPosition(kControl_Button_B001)->EnableWindowWZ(false);
            GetControlWZ_byPosition(kControl_Button_B028)->EnableWindowWZ(false);
            GetControlWZ_byPosition(kControl_Text_S005)->SetTextWZ(""); 
            GetControlWZ_byPosition(kControl_Text_S001)->ShowWindowWZ(WZ_HIDE); 
            GetControlWZ_byPosition(kControl_Text_S006)->ShowWindowWZ(WZ_HIDE);
            GetControlWZ_byPosition(kControl_Edit_E000)->SetTextWZ("");
            GetControlWZ_byPosition(kControl_Edit_E000)->OnFocusOut();
            GetControlWZ_byPosition(kControl_Edit_E001)->SetTextWZ("");
            GetControlWZ_byPosition(kControl_Edit_E001)->OnFocusOut();
        }

        g_InterfaceManager.LockDlgsAll();
        g_InterfaceManager.UnLockDlg(uiConvenienceStore_Gift::kDialog_ID, false);
        g_InterfaceManager.UnLockDlg(uiConvenienceStore_FriendList::kDialog_ID, false);
    }
    else
    {
        g_KeyQueueManager.DeleteMsg(uiConvenienceStore_Gift::kDialog_ID);

        product_list().clear();
        if (GetManager()->friend_list()->IsVisible() == TRUE)
        {
            GetManager()->friend_list()->ShowInterface(FALSE);
        }
#if WZENGINEVER >= 410
        if (g_InterfaceManager.GetDialogManager()->GetLockDlgID() == 0)
        { 
            g_InterfaceManager.UnLockDlgs();
        }
#endif //#if WZENGINEVER >= 410
    }
}
//------------------------------------------------------------------------------ 
void uiConvenienceStore_Gift::ShowInterface(bool is_show, 
                                            std::vector<ConvenienceStore::ProductItem*>& basket_vector)
{
    if (basket_vector.size() <= 0)
    {
        return;
    }

    uiConvenienceStore_Dialog::ShowInterface(is_show);

    std::vector<ConvenienceStore::ProductItem*>::iterator begin = basket_vector.begin();
    std::vector<ConvenienceStore::ProductItem*>::iterator end = basket_vector.end();
    for (; begin != end; ++begin)
    {
        product_list().push_back(*begin);
    }
}
//------------------------------------------------------------------------------ 
void uiConvenienceStore_Gift::ShowInterface(bool is_show, 
                                            ConvenienceStore::ProductItem* product)
{
    if (product == NULL)
    {
        return;
    }

    uiConvenienceStore_Dialog::ShowInterface(is_show);

    product_list().push_back(product);
}

//------------------------------------------------------------------------------
void uiConvenienceStore_Gift::MessageProc(SI_MESSAGE* message)
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
#if WZENGINEVER >= 440
    case RT_MSG_EDIT_KEYDOWN:
        OnKeyDown(message);
        break;
#endif //#if WZENGINEVER >= 440

#if WZENGINEVER >= 410
    case RT_MSG_EDIT_CHARDOWN:
        OnCharDown(message);
        break;
#endif //#if WZENGINEVER >= 410

    case RT_MSG_EDIT_ENTERKEYDOWN:
        OnEnterkeyDown(message);
        break;
    case RT_MSG_LBUTTONCLICK:
        OnLButtonClick(message);
        break;
    default:
        SolarDialog::MessageProc(message);
        break;
    }
}
//------------------------------------------------------------------------------ 
bool uiConvenienceStore_Gift::OnEnterkeyDown(SI_MESSAGE* message)
{
    bool result = true; 
    switch (GetControlPosition_byID(message->dwCtrlID))
    {
    case kControl_Edit_E000:
        {
            if (GetControlWZ_byPosition(kControl_Edit_E000) &&
                GetControlWZ_byPosition(kControl_Text_S005) &&
                GetControlWZ_byPosition(kControl_Text_S001))
            {
                GetControlWZ_byPosition(kControl_Edit_E000)->OnFocusOut();
                GetControlWZ_byPosition(kControl_Text_S005)->SetTextWZ("");
                GetControlWZ_byPosition(kControl_Text_S001)->ShowWindowWZ(WZ_HIDE);
            }
        }
        break;
    case kControl_Edit_E001:
        {
        }
        break;
    }
    return result;
}
//------------------------------------------------------------------------------ 
bool uiConvenienceStore_Gift::OnKeyDown(SI_MESSAGE* message)
{
    bool result = true;
    switch (GetControlPosition_byID(message->dwCtrlID))
    {
    case kControl_Edit_E000:
        {
            if (GetControlWZ_byPosition(kControl_Button_B001) &&
                GetControlWZ_byPosition(kControl_Button_B028) &&
                GetControlWZ_byPosition(kControl_Text_S005) &&
                GetControlWZ_byPosition(kControl_Text_S001))
            {
                GetControlWZ_byPosition(kControl_Button_B001)->EnableWindowWZ(true);
                GetControlWZ_byPosition(kControl_Button_B028)->EnableWindowWZ(false);
                GetControlWZ_byPosition(kControl_Text_S005)->SetTextWZ("");
                GetControlWZ_byPosition(kControl_Text_S001)->ShowWindowWZ(WZ_HIDE);
            }
        }
        break;

    case kControl_Edit_E001:
        {
        }
        break;
    }
    return result;
}
//------------------------------------------------------------------------------ 
bool uiConvenienceStore_Gift::OnCharDown(SI_MESSAGE* message)
{
    bool result = true;
    switch (GetControlPosition_byID(message->dwCtrlID))
    {
    case kControl_Edit_E000:
        {
            if (GetControlWZ_byPosition(kControl_Button_B001) &&
                GetControlWZ_byPosition(kControl_Button_B028) &&
                GetControlWZ_byPosition(kControl_Text_S005) &&
                GetControlWZ_byPosition(kControl_Text_S001))
            {
                GetControlWZ_byPosition(kControl_Button_B001)->EnableWindowWZ(true);
                GetControlWZ_byPosition(kControl_Button_B028)->EnableWindowWZ(false);
                GetControlWZ_byPosition(kControl_Text_S005)->SetTextWZ("");
                GetControlWZ_byPosition(kControl_Text_S001)->ShowWindowWZ(WZ_HIDE);
            }
        }
        break;
        
    case kControl_Edit_E001:
        {
        }
        break;
    }
    return result;
}
//------------------------------------------------------------------------------
bool uiConvenienceStore_Gift::OnLButtonClick(SI_MESSAGE* message)
{
    bool result = true;
    switch (GetControlPosition_byID(message->dwCtrlID))
    {
    case kControl_Button_B004:
    case kControl_Button_B002:
        {
            uiConvenienceStore_Dialog::ShowInterface(FALSE);
        }
        break;
    case kControl_Button_B028:
        {
            if (GetControlWZ_byPosition(kControl_Button_B001) &&
                GetControlWZ_byPosition(kControl_Button_B001)->GetDrawState() != E_DISABLE)
            {
                //! 아이디를 체크해주세요.
                int a = 0;
                break;
            }
            else
            {
                // 선물보내기
                CCtrlDxEditBox* name_editbox_control = GetDxEditBoxControlWZ(GetControlID_byPosition(kControl_Edit_E000));
                CCtrlDxEditBox* msg_editbox_control = GetDxEditBoxControlWZ(GetControlID_byPosition(kControl_Edit_E001));
                if (name_editbox_control != NULL && name_editbox_control != NULL)
                {
                    TCHAR char_name[MAX_PATH] = {NULL, };
                    TCHAR msg[MAX_PATH] = {NULL, };
                    name_editbox_control->GetTextWZ(char_name, MAX_PATH);
                    msg_editbox_control->GetTextWZ(msg, MAX_PATH);

                    if(_tcslen(msg) >= GIFT_MESSAGE_LENGTH)
                    {
                        uiSystemMan* system_man = (uiSystemMan*)g_InterfaceManager.GetUserInterfaceManager(UIMAN_SYSTEM);
                        if (system_man != NULL)
                        {
                            //5788	입력할 수 있는 쪽지 내용의 최대 글자 수를 초과하였습니다.(한글 100자, 숫자-영문 100자)
                            system_man->Sys_Verify(g_InterfaceManager.GetInterfaceString(5788));
                        }
                        break;
                    }
                    else
                    {

#ifdef _GLOBAL
                        // 글로벌은 PPCard만 선물이 가능하다
                        GetManager()->purchase()->SetCashType(ConvenienceStore::Define::kCashType_PPCard);
#elif defined(_CHINA)
                        // 선물목록을 검사해서 마일리지 상품이 포함되어있으면 마일리지로 선물하는것으로 판단한다
                        bool is_mileage = false;
                        for( size_t i=0;i!=product_list().size();++i)
                        {
                            ConvenienceStore::ProductItem* item = product_list().at(i);
                            if( item->is_mileage_only() == true )
                            {
                                is_mileage = true;
                                break;
                            }
                        }


                        if( is_mileage == true )
                            GetManager()->purchase()->SetCashType(ConvenienceStore::Define::kCashType_Mileage);
                        else 
                            GetManager()->purchase()->SetCashType(ConvenienceStore::Define::kCashType_Cash);
#endif

                        GetManager()->purchase()->SetGiftMessage( char_name, msg );
                        GetManager()->purchase()->OpenPurchase(uiConvenienceStore_Purchase::kPurchase_Gift);

                        for( size_t i=0;i!=product_list().size();++i)
                        {
                            ConvenienceStore::ProductItem* item = product_list().at(i);
                            GetManager()->purchase()->InsertPurchase(item, GetManager()->basket()->FindBasketIndex(item));
                        }
                    }
                }
            }

            uiConvenienceStore_Dialog::ShowInterface(FALSE);
        }
        break;
    case kControl_Button_B000:
        {
            // 친구 리스트
            GetManager()->friend_list()->ShowInterface(GetManager()->friend_list()->IsVisible() != TRUE);
        }
        break;
    case kControl_Button_B001:
        {
            // 아이디 체크
            CCtrlDxEditBox* editbox_control = GetDxEditBoxControlWZ(GetControlID_byPosition(kControl_Edit_E000));
            if (editbox_control != NULL)
            {
                TCHAR char_name[MAX_PATH] = {NULL, };
                editbox_control->GetTextWZ(char_name, MAX_PATH);

                //CheckName(char_name);
                GetManager()->CheckPresentTargetName(char_name);
                if (GetControlWZ_byPosition(kControl_Button_B001))
                    GetControlWZ_byPosition(kControl_Button_B001)->EnableWindowWZ(false);
            }
        }
        break;
    case kControl_Text_S000:
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
    case kControl_Text_S005:
        {
        }
        break;
    case kControl_Text_S006:
        {
        }
        break;
    case kControl_Text_S001:
        {
        }
        break;
    case kControl_Edit_E000:
        {
        }
        break;
    case kControl_Edit_E001:
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
    case kControl_Picture_P000:
        {
        }
        break;
    case kControl_Picture_P001:
        {
        }
        break;
    case kControl_Picture_P003:
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
void uiConvenienceStore_Gift::OnRenderSolarDialog()
{    
    //if (g_InterfaceManager.GetDialogManager()->GetLockDlgID() == uiConvenienceStore_Gift::kDialog_ID)
    { 
        POINT pt = g_pApplication->Get2DModeMapping();
        g_pSunRenderer->ColorFill(WzColor_RGBA(0,0,0,128), 0.0f, 0.0f, (float)pt.x,(float)pt.y);

        GetDialogWZ()->OnDraw(true);
        if (GetManager()->friend_list()->IsVisible() == TRUE)
        {
            GetManager()->friend_list()->GetDialogWZ()->OnDraw(true);
        }
        uiCommunityMan* community_man = static_cast<uiCommunityMan*>(g_InterfaceManager.GetUserInterfaceManager(UIMAN_COMMUNITY));
        if (community_man != NULL && 
            community_man->GetCommunityinfo_Dialog() != NULL && 
            community_man->GetCommunityinfo_Dialog()->IsVisible() == TRUE)
        {
            community_man->GetCommunityinfo_Dialog()->GetDialogWZ()->OnDraw(true);
        }

    }
}
//------------------------------------------------------------------------------ 
void uiConvenienceStore_Gift::OnUpdateSolarDialog()
{
    CCtrlDxIMEEditBox* editbox_control = (CCtrlDxIMEEditBox*)GetDxEditBoxControlWZ(GetControlID_byPosition(kControl_Edit_E000));
    if (editbox_control != NULL)
    {
        TCHAR szName[MAX_PATH] = {NULL, };
        editbox_control->GetTextWZ(szName, MAX_PATH);

        if (editbox_control->HaveFocus() || _tcslen(szName) > 0)
        {
            if (GetControlWZ_byPosition(kControl_Text_S004))
                GetControlWZ_byPosition(kControl_Text_S004)->ShowWindowWZ(WZ_HIDE);
        }
        else
        {
            if (GetControlWZ_byPosition(kControl_Text_S004))
                GetControlWZ_byPosition(kControl_Text_S004)->ShowWindowWZ(WZ_SHOW);
        }
    }
}
//------------------------------------------------------------------------------ 
void uiConvenienceStore_Gift::GetRecvCharacterName(TCHAR* name, int buffer_len)
{
    if (GetControlWZ_byPosition(kControl_Edit_E000))
        GetControlWZ_byPosition(kControl_Edit_E000)->GetTextWZ(name, buffer_len);
}
//------------------------------------------------------------------------------ 
void uiConvenienceStore_Gift::SetRecvCharInfo(const TCHAR* char_name, const LEVELTYPE char_level, const TCHAR* guild_name)
{
    TCHAR prev_name[INTERFACE_STRING_LENGTH] = {NULL, };
    if (GetControlWZ_byPosition(kControl_Edit_E000))
        GetControlWZ_byPosition(kControl_Edit_E000)->GetTextWZ(prev_name, INTERFACE_STRING_LENGTH);
    if (_tcscmp(prev_name, char_name) == 0)
    {
        return;
    }

    if (GetControlWZ_byPosition(kControl_Button_B001))
        GetControlWZ_byPosition(kControl_Button_B001)->EnableWindowWZ(true);
    if (GetControlWZ_byPosition(kControl_Button_B028))
        GetControlWZ_byPosition(kControl_Button_B028)->EnableWindowWZ(false);
    
    if (GetControlWZ_byPosition(kControl_Edit_E000))
        GetControlWZ_byPosition(kControl_Edit_E000)->SetTextWZ(char_name);

    TCHAR msg[INTERFACE_STRING_LENGTH] = {NULL, };
    if (char_level > 0)
    {
        Snprintf(msg, INTERFACE_STRING_LENGTH-1, _T("Lv.%d "), char_level);
    }
    if (char_name != NULL)
    {
        _tcscat(msg, char_name);
    }
    if (_tcslen(guild_name) > 0)
    {
        _tcscat(msg, _T("<"));
        _tcscat(msg, guild_name);
        _tcscat(msg, _T(">"));
    }
    if (GetControlWZ_byPosition(kControl_Text_S005))
        GetControlWZ_byPosition(kControl_Text_S005)->SetTextWZ(msg);
}
//------------------------------------------------------------------------------ 
void uiConvenienceStore_Gift::CheckOk(const TCHAR* char_name, const LEVELTYPE char_level, const TCHAR* guild_name)
{
    if (char_name == NULL || guild_name == NULL)
    {
        return;
    }

    if (GetControlWZ_byPosition(kControl_Edit_E000))
        GetControlWZ_byPosition(kControl_Edit_E000)->SetTextWZ(char_name);

    TCHAR msg[INTERFACE_STRING_LENGTH] = {NULL, };
    Snprintf(msg, INTERFACE_STRING_LENGTH-1, _T("Lv.%d "), char_level);

    if (char_name != NULL)
    {
        _tcscat(msg, char_name);
    }
    if (_tcslen(guild_name) > 0)
    {
        _tcscat(msg, _T("<"));
        _tcscat(msg, guild_name);
        _tcscat(msg, _T(">"));
    }

    if (GetControlWZ_byPosition(kControl_Text_S005) &&
        GetControlWZ_byPosition(kControl_Text_S005) &&
        GetControlWZ_byPosition(kControl_Text_S006) &&
        GetControlWZ_byPosition(kControl_Text_S001) &&
        GetControlWZ_byPosition(kControl_Button_B001) &&
        GetControlWZ_byPosition(kControl_Button_B028))
    {
        GetControlWZ_byPosition(kControl_Text_S005)->SetTextWZ(msg);

        GetControlWZ_byPosition(kControl_Text_S005)->ShowWindowWZ(WZ_SHOW);
        GetControlWZ_byPosition(kControl_Text_S006)->ShowWindowWZ(WZ_SHOW);
        GetControlWZ_byPosition(kControl_Text_S001)->ShowWindowWZ(WZ_HIDE);
        GetControlWZ_byPosition(kControl_Button_B001)->EnableWindowWZ(false);
        GetControlWZ_byPosition(kControl_Button_B028)->EnableWindowWZ(true);
    }
}
//------------------------------------------------------------------------------ 
void uiConvenienceStore_Gift::CheckFail()
{
    if (GetControlWZ_byPosition(kControl_Text_S005) &&
        GetControlWZ_byPosition(kControl_Text_S005) &&
        GetControlWZ_byPosition(kControl_Text_S006) &&
        GetControlWZ_byPosition(kControl_Text_S001) &&
        GetControlWZ_byPosition(kControl_Button_B001) &&
        GetControlWZ_byPosition(kControl_Button_B028))
    {
        GetControlWZ_byPosition(kControl_Text_S005)->SetTextWZ("");
        GetControlWZ_byPosition(kControl_Text_S005)->ShowWindowWZ(WZ_HIDE);
        GetControlWZ_byPosition(kControl_Text_S006)->ShowWindowWZ(WZ_HIDE);
        GetControlWZ_byPosition(kControl_Text_S001)->ShowWindowWZ(WZ_SHOW);
        GetControlWZ_byPosition(kControl_Button_B001)->EnableWindowWZ(false);
        GetControlWZ_byPosition(kControl_Button_B028)->EnableWindowWZ(false);
    }
}
