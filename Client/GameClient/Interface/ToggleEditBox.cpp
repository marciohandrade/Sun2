#include "SunClientPrecompiledHeader.h"

#ifdef _SCJ_TOGGLE_EDITBOX_CONTROL

#include "ToggleEditBox.h"
#include "Mouse.h"

ToggleEditBox::ToggleEditBox(CControlWZ* edit_control, CControlWZ* static_control)
{
    edit_control_ = static_cast<CCtrlDxEditBox*>(edit_control);
    static_control_ = static_cast<CCtrlStaticWZ*>(static_control);

    is_active_ = false;
    is_block_ = false;
}
ToggleEditBox::~ToggleEditBox()
{

}
bool ToggleEditBox::MessageProc(SI_MESSAGE* msg)
{
    switch (msg->eResultMsg)
    {
        case RT_MSG_LBUTTONCLICK:
            return OnLButtonClick(msg);
        case RT_MSG_LBUTTONDCLICK:
            return OnLButtonDClick(msg);
        default:
            return false;
    }
}
bool ToggleEditBox::OnLButtonClick(SI_MESSAGE* msg)
{
    if ((msg == NULL) || 
        (edit_control() == NULL) || 
        (static_control() == NULL))
    {
        return false;
    }

    bool is_result = false; // 토글 될 때 true 리턴.

    RECT static_rect = static_control()->GetSizeRect();
    int mouse_x = Mouse::Instance()->GetMouseX();
    int mouse_y = Mouse::Instance()->GetMouseY();
    if (mouse_x > static_rect.left && mouse_x < static_rect.right
        && mouse_y > static_rect.top && mouse_y < static_rect.bottom)
    {
        if (is_active_ == false)
        {
            is_result = true;
        }

        ActiveEditControl(true);

        if (is_block())
        {
            Block();
        }
    }
    else
    {
        if (is_active_ == true)
        {
            is_result = false;
        }

        ActiveEditControl(false);
    }

    return is_result;
}
bool ToggleEditBox::OnLButtonDClick(SI_MESSAGE* msg)
{
    return OnLButtonClick(msg);
}
void ToggleEditBox::ActiveEditControl(bool is_show)
{
    if ((edit_control() == NULL) || (static_control() == NULL))
    {
        return;
    }

    TCHAR string_buffer[INTERFACE_STRING_LENGTH] = {0,};
    if (is_show)
    {
        edit_control()->ShowWindowWZ(WZ_SHOW);
        edit_control()->SetFocusWZ();
        static_control()->ShowWindowWZ(WZ_HIDE);
    }
    else
    {
        edit_control()->GetTextWZ(string_buffer, INTERFACE_STRING_LENGTH-1);
        static_control()->SetTextWZ(string_buffer);

        edit_control()->ShowWindowWZ(WZ_HIDE);
        static_control()->ShowWindowWZ(WZ_SHOW);
    }

    is_active_ = is_show;
}
void ToggleEditBox::Block()
{
    // 블럭지정
    SI_MESSAGE message;
    message.lnReceiveMsg = WM_CHAR;
    message.wParam = (WCHAR)1;
    edit_control()->SendMessageWZ(&message);

}
void ToggleEditBox::ClearText()
{
    if ((edit_control() == NULL) ||
        (static_control() == NULL))
    {
        return;
    }

    edit_control()->SetTextWZ("\0");
    static_control()->SetTextWZ("\0");
}
void ToggleEditBox::GetText(TCHAR* string, int count)
{
    if (edit_control() == NULL)
    {
        string = "\0";
        return;
    }

    edit_control()->GetTextWZ(string, count);
}
void ToggleEditBox::SetText(TCHAR* string)
{
    if (edit_control() == NULL)
    {
        return;
    }

    edit_control()->SetTextWZ(string);
}
#endif //_SCJ_TOGGLE_EDITBOX_CONTROL