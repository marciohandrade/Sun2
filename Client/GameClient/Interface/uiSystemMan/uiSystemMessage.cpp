#include "SunClientPrecompiledHeader.h"
#include "uiSystemMan/uiSystemMessage.h"
#include "uiSystemMan.h"
#include "Application.h"
#include "GlobalFunc.h"
#include "FTextParser.h"
#include "FTextSentence.h"
#include "KeyQueueManager.h"

uiSystemMessage::UserDataContainer uiSystemMessage::user_data_container_;
DWORD uiSystemMessage::current_dialog_key_ = 0;

uiSystemMessage::uiSystemMessage(InterfaceManager* interface_manager)
{
    set_callback_function(NULL);
    set_expired_time(0);
}

uiSystemMessage::~uiSystemMessage()
{

}

void uiSystemMessage::Init(CDrawBase* draw_base)
{
    RegisterControlPosition(StrToWzID("S000"), kControl_Title_Text);
    RegisterControlPosition(StrToWzID("S003"), kControl_Message_Text);
    RegisterControlPosition(StrToWzID("V000"), kControl_VScroll);
    RegisterControlPosition(StrToWzID("P000"), kControl_VScroll_Picture);
    RegisterControlPosition(StrToWzID("B001"), kControl_Verify_Button);
    RegisterControlPosition(StrToWzID("B000"), kControl_Confirm_Button);
    RegisterControlPosition(StrToWzID("B028"), kControl_Rejection_Button);
    RegisterControlPosition(StrToWzID("B004"), kControl_Close_Button);
}

void uiSystemMessage::Clear()
{

}

void uiSystemMessage::Release()
{
}

void uiSystemMessage::OnShowWindow(BOOL is_show)
{
    if (is_show == TRUE)
    {
        keyMsg keyqueue_message;
        ZeroMemory(&keyqueue_message, sizeof(keyqueue_message));
        keyqueue_message.dwType = GetDialogKey();
        keyqueue_message.wParam = 0;
        keyqueue_message.lParam = GetDialogKey();
        keyqueue_message.DoSomething = DoSystemMessage;
        g_KeyQueueManager.PushBack(keyqueue_message);
    }
    else
    {
        if (is_modal() == true)
        {
            this->GetUIMan()->UnLockDlgs();
            this->GetUIMan()->DeAttachFirstDialog(GetDialogKey());
        }

        if (callback_function() != NULL)
        {
            DWORD dialog_key = GetDialogKey();
            CurrentDialogKey() = dialog_key;
            callback_function()(kClose_Button);
        }

        UserDataContainer::iterator found_itr = GetUserDataContainer().find(GetDialogKey());
        if (found_itr != GetUserDataContainer().end())
        {
            GetUserDataContainer().erase(found_itr);
        }

        g_KeyQueueManager.DeleteMsg(GetDialogKey());
    }
}

void uiSystemMessage::MessageProc(SI_MESSAGE* message)
{
    if (IsLock())
        return;

    if (GetMan() == NULL)
        return;

    switch (message->eResultMsg)
    {
    case RT_MSG_LBUTTONCLICK:
        OnLButtonClick(message);
        break;
    case RT_MSG_SB_LINETOP:
        OnVScrollUp(message);
        break;
    case RT_MSG_SB_LINEBOTTOM:
        OnVScrollDown(message);
        break;
    case RT_MSG_SB_THUMBTRACK:      //!< 썸을 드래그 하고 있다.
    case RT_MSG_SB_THUMBPOSITION:   //!< 썸을 드래그한후 마우스를 놓았다.
    case RT_MSG_SB_ENDSCROLL:       //!< 스크롤이 종료 되었다.
        OnVScrollThumb(message);
    default:
        SolarDialog::MessageProc(message);
        break;
    }
}

bool uiSystemMessage::OnLButtonClick(SI_MESSAGE* message)
{
    bool result = true;
    DWORD control_position = GetControlPosition_byID(message->dwCtrlID);
    switch (control_position)
    {
    case kControl_Verify_Button:
    case kControl_Confirm_Button:
    case kControl_Rejection_Button:
    case kControl_Close_Button:
        {
            DWORD dialog_key = GetDialogKey();
            CurrentDialogKey() = dialog_key;
            
            ButtonIndex button_index = 
                static_cast<ButtonIndex>(control_position - kControl_Verify_Button);
            if (callback_function() != NULL)
            {
                callback_function()(button_index);
                set_callback_function(NULL);
                
                UserDataContainer::iterator found_itr = GetUserDataContainer().find(dialog_key);
                if (found_itr != GetUserDataContainer().end())
                {
                    GetUserDataContainer().erase(found_itr);
                }
            }

            SetDestroyWindow(TRUE);
        }
        break;
    default:
        {
        }
        break;
    }
    return result;
}

bool uiSystemMessage::OnVScrollUp(SI_MESSAGE* message)
{
    CCtrlVScrollWZ* scroll_control = 
        static_cast<CCtrlVScrollWZ*>(GetControlWZ_byPosition(kControl_VScroll));
    if (scroll_control == NULL)
    {
        return false;
    }

    int scroll_position = scroll_control->GetScrollPosWZ();
    int range_min = 0, range_max = 0;
    scroll_control->GetScrollRangeWZ(&range_min, &range_max);
    if(scroll_position > range_min)
    {
        --scroll_position;
        scroll_control->SetScrollPosWZ(scroll_position);
    }

    message_text_renderer().SetBeginLineIndex(scroll_position);

    return true;
}

bool uiSystemMessage::OnVScrollDown(SI_MESSAGE* message)
{
    CCtrlVScrollWZ* scroll_control = 
        static_cast<CCtrlVScrollWZ*>(GetControlWZ_byPosition(kControl_VScroll));
    if (scroll_control == NULL)
    {
        return false;
    }

    int scroll_position = scroll_control->GetScrollPosWZ();
    int range_min = 0, range_max = 0;
    scroll_control->GetScrollRangeWZ(&range_min, &range_max);
    if(scroll_position < range_max)
    {
        ++scroll_position;
        scroll_control->SetScrollPosWZ(scroll_position);
    }

    message_text_renderer().SetBeginLineIndex(scroll_position);

    return true;
}

bool uiSystemMessage::OnVScrollThumb(SI_MESSAGE* message)
{
    CCtrlVScrollWZ* scroll_control = 
        static_cast<CCtrlVScrollWZ*>(GetControlWZ_byPosition(kControl_VScroll));
    if (scroll_control == NULL)
    {
        return false;
    }

    int scroll_position = scroll_control->GetScrollPosWZ();
    message_text_renderer().SetBeginLineIndex(scroll_position);

    return true;
}

void uiSystemMessage::OnUpdateSolarDialog()
{
    UpdateMessage();

    CControlWZ* message_text = GetControlWZ_byPosition(kControl_Message_Text);
    if (message_text != NULL)
    {
        RECT message_rect = message_text->GetSizeRect();
        BYTE wheel_message = uiBase::GetMouseWheelStatus(&message_rect);
        switch (wheel_message)
        {
        case MOUSE_WHEEL_UP:
            OnVScrollUp(NULL);
            break;
        case MOUSE_WHEEL_DOWN:
            OnVScrollDown(NULL);
            break;
        }
    }

    if (expired_time() > 0)
    {
        if (clock_function::GetTickCount() >= expired_time())
        {
            SetDestroyWindow(TRUE);
        }
    }
}

void uiSystemMessage::UpdateMessage()
{
    CControlWZ* message_text = GetControlWZ_byPosition(kControl_Message_Text);
    if (message_text == NULL)
    {
        return;
    }

    RECT message_rect = message_text->GetSizeRect();
    message_rect.right -= 15;
    FTextParser parser;
    FTextSentence sentence;
    parser.SetDefaultFont(c_wiSystemMsgFont, WzColor_RGBA(255, 255, 255, 255));
    message_text_renderer().Init(GetDrawBase(), message_rect);
    message_text_renderer().SetVCenterAlign(true);
    message_text_renderer().ClearSentence();

    BOOL result_value = parser.Parser(message_string().c_str(), sentence);
    if (result_value == TRUE)
    {
        message_text_renderer().Append(sentence);
    }
    if (expired_time() > 0)
    {
        DWORD remain_time = 0;
        if (expired_time() > clock_function::GetTickCount())
        {
            remain_time = expired_time() - clock_function::GetTickCount();
        }
        TCHAR remain_time_text[INTERFACE_STRING_LENGTH] = {'\0', };
        // 70062 남은 시간.... ( %d )
        g_InterfaceManager.GetInterfaceStringFormat(
            remain_time_text, 
            INTERFACE_STRING_LENGTH - 1, 
            70062, 
            (remain_time / 1000));

        result_value = parser.Parser(remain_time_text, sentence);
        if (result_value == TRUE)
        {
            message_text_renderer().Append(sentence);
        }
    }

    message_text_renderer().convertLineInfo();

    CCtrlVScrollWZ* scroll_control = 
        static_cast<CCtrlVScrollWZ*>(GetControlWZ_byPosition(kControl_VScroll));
    if (scroll_control != NULL)
    {
        size_t max_line = message_text_renderer().GetMaxLine();
        size_t draw_line = message_text_renderer().GetDrawLineInRect();
        if (max_line > draw_line)
        {
            int scroll_position = scroll_control->GetScrollPosWZ();
            int scroll_max = max(0, max_line - draw_line);
            int thumb_size = 100 / (scroll_max + 1);
            scroll_position = max(0, scroll_position);
            scroll_position = min(scroll_position, scroll_max);
            scroll_control->SetScrollRangeWZ(0, scroll_max);
            scroll_control->SetChangeSizeThumb(thumb_size);
            scroll_control->SetScrollPosWZ(scroll_position);
            scroll_control->EnableWindowWZ(true);
            message_text_renderer().SetBeginLineIndex(scroll_position);
        }
        else
        {
            scroll_control->SetScrollRangeWZ(0, 0);
            scroll_control->SetChangeSizeThumb(100);
            scroll_control->SetScrollPosWZ(0);
            scroll_control->EnableWindowWZ(false);
            message_text_renderer().SetBeginLineIndex(0);
        }
    }
}

void uiSystemMessage::OnRenderSolarDialog()
{
    if (is_show_lock_effect() == true)
    {
        RECT screen_rect;
        POINT screen_max = g_pApplication->Get2DModeMapping();
        SetRect(&screen_rect, 0, 0, screen_max.x, screen_max.y);

        g_pSunRenderer->ColorFill(WzColor_RGBA(0,0,0,128), 
            0, 
            0, 
            static_cast<float>(screen_rect.right), 
            static_cast<float>(screen_rect.bottom));

        GetDialogWZ()->OnDraw(true);
    }    

    message_text_renderer().Render();
}

void uiSystemMessage::SetTitle(const TCHAR* title)
{
    CCtrlStaticWZ* title_text = 
        static_cast<CCtrlStaticWZ*>(GetControlWZ_byPosition(kControl_Title_Text));
    if (title_text == NULL)
    {
        return;
    }

    if (title == NULL)
    {
        // 기본타이틀 : 4533 알림!
        title_text->SetTextID(4533);
    }
    else
    {
        title_text->SetTextWZ(title);
    }
}

void uiSystemMessage::SetMessage(const TCHAR* message)
{
    message_string().clear();
    if (message == NULL)
    {
        return;
    }

    for (int char_index = 0; *(message + char_index) != '\0'; ++char_index)
    {
        if ((message[char_index] == '\\') && 
            (message[char_index + 1] == 'n'))
        {
            message_string() += "<RETURN=1/>";
            ++char_index;
        }
        else
        {
            message_string() += message[char_index];
        }
    }
}

void uiSystemMessage::SetMessageType(MessageType type)
{
    CControlWZ* verify_button = GetControlWZ_byPosition(kControl_Verify_Button);
    CControlWZ* confirm_button = GetControlWZ_byPosition(kControl_Confirm_Button);
    CControlWZ* rejection_button = GetControlWZ_byPosition(kControl_Rejection_Button);
    CControlWZ* close_button = GetControlWZ_byPosition(kControl_Close_Button);
    
    if ((verify_button == NULL) || 
        (confirm_button == NULL) || 
        (rejection_button == NULL) ||
        (close_button == NULL))
    {
        assert(!"uiSystemMessage::SetMessageType() : button control is NULL");
        return;
    }

    switch (type)
    {
    case kVerify_Type:
        {
            verify_button->ShowWindowWZ(WZ_SHOW);
            confirm_button->ShowWindowWZ(WZ_HIDE);
            rejection_button->ShowWindowWZ(WZ_HIDE);
            close_button->ShowWindowWZ(WZ_SHOW);
        }
        break;
    case kConfirm_Type:
        {
            verify_button->ShowWindowWZ(WZ_HIDE);
            confirm_button->ShowWindowWZ(WZ_SHOW);
            rejection_button->ShowWindowWZ(WZ_SHOW);
            close_button->ShowWindowWZ(WZ_SHOW);
        }
        break;
    case kVerify_And_Dont_Close_Type:
        {
            verify_button->ShowWindowWZ(WZ_SHOW);
            confirm_button->ShowWindowWZ(WZ_HIDE);
            rejection_button->ShowWindowWZ(WZ_HIDE);
            close_button->ShowWindowWZ(WZ_HIDE);
            g_KeyQueueManager.DeleteMsg(GetDialogKey()); // esc큐 삭제(큐 삽입은 create 할 때 이미 들어감)
            CDialogWZ* dialog_pointer = GetDialogWZ();
            if (dialog_pointer)
            {
                RECT move_rect;
                move_rect.left = 0;
                move_rect.top = 0;
                move_rect.right = -1;
                move_rect.bottom = -1;
                dialog_pointer->SetMoveRECT(move_rect);  // 마우스로 이동 x
            }
        }
        break;
    case kConfirm_And_Dont_Close_Type:
        {
            verify_button->ShowWindowWZ(WZ_HIDE);
            confirm_button->ShowWindowWZ(WZ_SHOW);
            rejection_button->ShowWindowWZ(WZ_SHOW);
            close_button->ShowWindowWZ(WZ_HIDE);
            g_KeyQueueManager.DeleteMsg(GetDialogKey()); // esc큐 삭제(큐 삽입은 create 할 때 이미 들어감)
            CDialogWZ* dialog_pointer = GetDialogWZ();
            if (dialog_pointer)
            {
                RECT move_rect;
                move_rect.left = 0;
                move_rect.top = 0;
                move_rect.right = -1;
                move_rect.bottom = -1;
                dialog_pointer->SetMoveRECT(move_rect);  // 마우스로 이동 x
            }
        }
        break;
    default:
        {
            verify_button->ShowWindowWZ(WZ_HIDE);
            confirm_button->ShowWindowWZ(WZ_HIDE);
            rejection_button->ShowWindowWZ(WZ_HIDE);
            close_button->ShowWindowWZ(WZ_SHOW);
        }
    }
}

void uiSystemMessage::SetTimer(DWORD life_time)
{
    if (life_time == 0)
    {
        set_expired_time(0);
        return;
    }

    DWORD currenct_time = clock_function::GetTickCount();
    set_expired_time(currenct_time + life_time);
}

void uiSystemMessage::SetButtonText(ButtonIndex button_index, DWORD string_code)
{
    DWORD control_position = button_index + kControl_Verify_Button;
    CCtrlButtonWZ* button_control = static_cast<CCtrlButtonWZ*>(GetControlWZ_byPosition(control_position));
    if (button_control == NULL)
    {
        return;
    }

    button_control->SetTextID(string_code);
}

void uiSystemMessage::SetButtonText(ButtonIndex button_index, const TCHAR* button_text)
{
    DWORD control_position = button_index + kControl_Verify_Button;
    CCtrlButtonWZ* button_control = static_cast<CCtrlButtonWZ*>(GetControlWZ_byPosition(control_position));
    if (button_control == NULL)
    {
        return;
    }

    button_control->SetTextWZ(button_text);
}

void uiSystemMessage::SetModal(bool show_lock_effect)
{
    set_modal(true);
    set_show_lock_effect(show_lock_effect);

    DWORD dialog_key = GetDialogKey();

    g_InterfaceManager.AttachFirstDialog(dialog_key);
    g_InterfaceManager.LockDlgs(dialog_key);
}

void uiSystemMessage::SetUserData( DWORD user_data )
{
    DWORD dialog_key = GetDialogKey();
    UserDataContainer::iterator found_itr = GetUserDataContainer().find(dialog_key);
    if (found_itr != GetUserDataContainer().end())
    {
        found_itr->second = user_data;
    }
    else
    {
        GetUserDataContainer().insert(UserDataContainer::value_type(dialog_key, user_data));
    }
}

void uiSystemMessage::DoSystemMessage(DWORD l_param, DWORD r_param)
{
    uiBase* system_message = GET_DIALOG(r_param);
    if (system_message != NULL)
    {
        system_message->SetDestroyWindow(TRUE);
    }
}

DWORD uiSystemMessage::GetUserData()
{
    DWORD user_data = 0;
    UserDataContainer::iterator found_itr = GetUserDataContainer().find(CurrentDialogKey());
    if (found_itr != GetUserDataContainer().end())
    {
        user_data = found_itr->second;
    }

    return user_data;
}
