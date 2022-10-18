#include "SunClientPrecompiledHeader.h"

#include "TabCheckButtonControl.h"
#include "InterfaceManager.h"

//------------------------------------------------------------------------------ 
TabCheckButtonControl::TabCheckButtonControl()
{
    is_discard_message_ = false;
    button_list().clear();
}
//------------------------------------------------------------------------------ 
TabCheckButtonControl::~TabCheckButtonControl()
{
    Release();
}
//------------------------------------------------------------------------------ 
void TabCheckButtonControl::Release()
{
    button_list().clear();
}
//------------------------------------------------------------------------------ 
void TabCheckButtonControl::Push(CCtrlButtonCheckWZ* check_button, DWORD user_data)
{
    if (check_button != NULL)
    {
        check_button->SetUserData(user_data);
        button_list().push_back(check_button);
    }
}
//------------------------------------------------------------------------------ 
bool TabCheckButtonControl::MessageProc(SI_MESSAGE* msg)
{
    switch (msg->eResultMsg)
    {
    case RT_MSG_LBUTTONCLICK:
        return OnLButtonClick(msg);
    }

    return false;
}
//------------------------------------------------------------------------------ 
bool TabCheckButtonControl::OnLButtonClick(SI_MESSAGE* msg)
{
    std::list<CCtrlButtonCheckWZ*>::iterator begin = button_list().begin();
    std::list<CCtrlButtonCheckWZ*>::iterator end = button_list().end();
    for (; begin != end; ++begin)
    {
        if ((*begin)->GetCtrlID() == msg->dwCtrlID)
        {
            return Click(*begin);
        }
    }
    return false;
}
//------------------------------------------------------------------------------ 
bool TabCheckButtonControl::Click(CCtrlButtonCheckWZ* check_button, bool is_ignore_drawstate/* = true*/)
{
    if (check_button != NULL)
    {
        //for_each(button_list().begin(), 
        //        button_list().end(), 
        //        std::bind2nd(std::mem_fun(&CCtrlButtonCheckWZ::SetCheckState), false)); 

        std::list<CCtrlButtonCheckWZ*>::iterator begin = button_list().begin();
        std::list<CCtrlButtonCheckWZ*>::iterator end = button_list().end();
        for (; begin != end; ++begin)
        {
            if (is_ignore_drawstate == true || (*begin)->GetDrawState() != E_DISABLE)
            {
                (*begin)->SetCheckState(false);
            }
            (*begin)->SetDiscardMessage(is_discard_message() == true);
        }

        if (is_ignore_drawstate == true || check_button->GetDrawState() != E_DISABLE)
        {
            check_button->SetCheckState(true);
        }
        check_button->SetDiscardMessage(true);

        return true;
    }
    return false;
}
//------------------------------------------------------------------------------ 
bool TabCheckButtonControl::Click(DWORD user_data, bool is_ignore_drawstate/* = true*/)
{
    std::list<CCtrlButtonCheckWZ*>::iterator begin = button_list().begin();
    std::list<CCtrlButtonCheckWZ*>::iterator end = button_list().end();
    for (; begin != end; ++begin)
    {
        if (is_ignore_drawstate == true || (*begin)->GetDrawState() != E_DISABLE)
        {
            (*begin)->SetCheckState(false);
        }
        (*begin)->SetDiscardMessage(is_discard_message() == true);

        if ((*begin)->GetUserData() == user_data)
        {
            if (is_ignore_drawstate == true || (*begin)->GetDrawState() != E_DISABLE)
            {
                (*begin)->SetCheckState(true);
            }
            (*begin)->SetDiscardMessage(true);
        }
    }
    return true;
}
//------------------------------------------------------------------------------ 
CCtrlButtonCheckWZ* TabCheckButtonControl::Current(OUT DWORD& user_data)
{
    std::list<CCtrlButtonCheckWZ*>::iterator begin = button_list().begin();
    std::list<CCtrlButtonCheckWZ*>::iterator end = button_list().end();
    for (; begin != end; ++begin)
    {
        if ((*begin)->GetCheckState())
        {
            user_data = (*begin)->GetUserData();
            return (*begin);
        }
    }
    return NULL;
}
//------------------------------------------------------------------------------ 
DWORD TabCheckButtonControl::CurrentUserData()
{
    std::list<CCtrlButtonCheckWZ*>::iterator begin = button_list().begin();
    std::list<CCtrlButtonCheckWZ*>::iterator end = button_list().end();
    for (; begin != end; ++begin)
    {
        if ((*begin)->GetCheckState())
        {
            return (*begin)->GetUserData();
        }
    }
    return 0xffffffff;
}
//------------------------------------------------------------------------------ 
void TabCheckButtonControl::set_discard_message(bool val)
{
    is_discard_message_ = val;

    std::list<CCtrlButtonCheckWZ*>::iterator begin = button_list().begin();
    std::list<CCtrlButtonCheckWZ*>::iterator end = button_list().end();
    for (; begin != end; ++begin)
    {
        (*begin)->SetDiscardMessage(true);
    }

    if (is_discard_message() == false)
    {
        DWORD user_data = 0;
        CCtrlButtonCheckWZ* btn = Current(user_data);
        if (btn != NULL)
        {
            btn->SetDiscardMessage(false);
        }
    }
}
//------------------------------------------------------------------------------ 
bool TabCheckButtonControl::IsVisible()
{
    std::list<CCtrlButtonCheckWZ*>::iterator begin = button_list().begin();
    std::list<CCtrlButtonCheckWZ*>::iterator end = button_list().end();
    for (; begin != end; ++begin)
    {
        if( (*begin)->GetState() == WZ_SHOW )
        {
            return true;
        }
    }
    return false;
}
//------------------------------------------------------------------------------ 
void TabCheckButtonControl::SetVisible(bool is_visible)
{
    std::list<CCtrlButtonCheckWZ*>::iterator begin = button_list().begin();
    std::list<CCtrlButtonCheckWZ*>::iterator end = button_list().end();
    for (; begin != end; ++begin)
    {
        (*begin)->ShowWindowWZ(is_visible ? WZ_SHOW : WZ_HIDE);
    }
}
//------------------------------------------------------------------------------ 
void TabCheckButtonControl::SetVisible(DWORD user_data, bool is_visible)
{
    CCtrlButtonCheckWZ* control = GetButtonControl(user_data);
    if (control != NULL)
    {
        control->ShowWindowWZ(is_visible ? WZ_SHOW : WZ_HIDE);
    }
}
//------------------------------------------------------------------------------ 
void TabCheckButtonControl::SetEnable(bool is_enable)
{
    std::list<CCtrlButtonCheckWZ*>::iterator begin = button_list().begin();
    std::list<CCtrlButtonCheckWZ*>::iterator end = button_list().end();
    for (; begin != end; ++begin)
    {
        (*begin)->EnableWindowWZ(is_enable);
    }
}
//------------------------------------------------------------------------------ 
void TabCheckButtonControl::SetEnable(DWORD user_data, bool is_enable)
{
    CCtrlButtonCheckWZ* control = GetButtonControl(user_data);
    if (control != NULL)
    {
        control->EnableWindowWZ(is_enable);
    }
}
//------------------------------------------------------------------------------ 
CCtrlButtonCheckWZ* TabCheckButtonControl::GetButtonControl(IN DWORD user_data)
{
    std::list<CCtrlButtonCheckWZ*>::iterator begin = button_list().begin();
    std::list<CCtrlButtonCheckWZ*>::iterator end = button_list().end();
    for (; begin != end; ++begin)
    {
        if ((*begin)->GetUserData() == user_data)
        {
            return (*begin);
        }
    }
    return NULL;
}