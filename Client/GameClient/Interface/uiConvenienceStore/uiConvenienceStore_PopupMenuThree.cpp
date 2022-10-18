#include "SunClientPrecompiledHeader.h"

#include "uiConvenienceStore_PopupMenuThree.h"
#include "InterfaceManager.h"
#include "uiConvenienceStoreMan.h"
#include "uiConvenienceStore_Dialog.h"
#include "GlobalFunc.h"

//------------------------------------------------------------------------------ 
const DWORD uiConvenienceStore_PopupMenuThree::kDialog_ID = StrToWzID("4713");;

//------------------------------------------------------------------------------ 
uiConvenienceStore_PopupMenuThree::uiConvenienceStore_PopupMenuThree(InterfaceManager* ui_man) : uiConvenienceStore_PopupMenu(ui_man)
{
    RegisterControlPosition(StrToWzID("B001"), kControl_Button_B001);
    RegisterControlPosition(StrToWzID("B000"), kControl_Button_B000);
    RegisterControlPosition(StrToWzID("B002"), kControl_Button_B002);
}

//------------------------------------------------------------------------------ 
uiConvenienceStore_PopupMenuThree::~uiConvenienceStore_PopupMenuThree( void )
{
    Release();
}
//------------------------------------------------------------------------------ 
void uiConvenienceStore_PopupMenuThree::Init(CDrawBase* drawbase)
{
    //ASSERT(is_result && "uiConvenienceStore_PopupMenuThree::Init false");
}
//------------------------------------------------------------------------------ 
void uiConvenienceStore_PopupMenuThree::Release(void)
{

}
//------------------------------------------------------------------------------ 
void uiConvenienceStore_PopupMenuThree::OnShowWindow(BOOL val)
{
    uiConvenienceStore_PopupMenu::OnShowWindow(val);
    if (val == TRUE)
    {
    }
    else
    {
        set_owner(NULL, NULL);

        if (GetControlID_byPosition(kControl_Button_B001) &&
            GetControlID_byPosition(kControl_Button_B000) &&
            GetControlID_byPosition(kControl_Button_B002))
        {
            GetControlWZ(GetControlID_byPosition(kControl_Button_B001))->SetUserData(NULL);
            GetControlWZ(GetControlID_byPosition(kControl_Button_B000))->SetUserData(NULL);
            GetControlWZ(GetControlID_byPosition(kControl_Button_B002))->SetUserData(NULL);
        }
    }
}
//------------------------------------------------------------------------------
void uiConvenienceStore_PopupMenuThree::OnUpdateSolarDialog()
{
#ifdef _RU_0000_20120312_RUSSIA_MOD_CASH_SHOP_UI
    // 리사이즈
    // 다이얼로그 리사이즈
    RECT dialog_rect;
    GetDialogWZ()->GetClientRect(&dialog_rect);
    GetDialogWZ()->MoveWindowWZ(static_cast<float>(dialog_rect.left), 
        static_cast<float>(dialog_rect.top), 
        static_cast<float>(extra_text_width() + kDialogMargin), 
        static_cast<float>(dialog_rect.bottom - dialog_rect.top));
    // 컨트롤 리사이즈
    CCtrlButtonWZ* button_control = NULL;
    for (int button_index = 0; button_index < kControl_Size; ++button_index)
    {
        button_control = GetButtonControlWZ(GetControlID_byPosition(kControl_Button_B001 + button_index));
        RECT control_rect = button_control->GetSizeRect();
        button_control->MoveWindowWZ(static_cast<float>(control_rect.left), 
            static_cast<float>(control_rect.top), 
            static_cast<float>(extra_text_width()), 
            static_cast<float>(control_rect.bottom - control_rect.top));
    }
#endif //_RU_0000_20120312_RUSSIA_MOD_CASH_SHOP_UI
}
//------------------------------------------------------------------------------ 
void uiConvenienceStore_PopupMenuThree::Process(DWORD tick)
{

}
//------------------------------------------------------------------------------ 
void uiConvenienceStore_PopupMenuThree::NetworkProc(MSG_BASE* msgbase)
{

}
//------------------------------------------------------------------------------
void uiConvenienceStore_PopupMenuThree::MessageProc(SI_MESSAGE* message)
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
bool uiConvenienceStore_PopupMenuThree::OnLButtonClick(SI_MESSAGE* message)
{
    bool result = true;
    switch (GetControlPosition_byID(message->dwCtrlID))
    {
    case kControl_Button_B001:  
    case kControl_Button_B000: 
    case kControl_Button_B002:  
        if (owner() != NULL) 
        {
            PopupCommandMap::iterator iter = command_map().find(message->dwCtrlID);
            if (iter != command_map().end())
            {
                owner()->OnClickPopupMenu(iter->second.first, iter->second.second); 
            }
            //uiConvenienceStore_PopupMenu::ShowInterface(FALSE);;
        }
        break;
    }
    return result;
     
}
//------------------------------------------------------------------------------ 
void uiConvenienceStore_PopupMenuThree::SetMenu(int button_index, 
                                                DWORD string_code, 
                                                bool is_enable, 
                                                uiConvenienceStore_Dialog::PopupCommandType command_type, 
                                                DWORD user_data)
{
    CCtrlButtonWZ* button_control = GetButtonControlWZ(GetControlID_byPosition(kControl_Button_B001 + button_index));
    if (button_control != NULL)
    {
        if (string_code > 0)
        {
            button_control->ShowWindowWZ(WZ_SHOW);
            button_control->EnableWindowWZ(is_enable);
            button_control->SetTextID(string_code);

            command_map().insert(std::make_pair(button_control->GetCtrlID(), std::make_pair(command_type, user_data)));

#ifdef _RU_0000_20120312_RUSSIA_MOD_CASH_SHOP_UI
            TCHAR menu_string[INTERFACE_STRING_LENGTH] = {'\0', };
            g_InterfaceManager.GetInterfaceString(string_code, menu_string);
            int text_width = GlobalFunc::GetTextWidth(menu_string, button_control->GetFontID());
            text_width += kControlMargin;
            if (text_width > extra_text_width())
            {
                set_extra_text_width(text_width);
            }
#endif //_RU_0000_20120312_RUSSIA_MOD_CASH_SHOP_UI
        }
        else
        {
            button_control->ShowWindowWZ(WZ_HIDE);
        }
    }
}
