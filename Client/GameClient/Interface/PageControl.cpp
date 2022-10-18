#include "SunClientPrecompiledHeader.h"

//#ifdef _YMS_TREECONTROL

#include "PageControl.h"
#include "InterfaceManager.h"

//------------------------------------------------------------------------------ 
PageControl::PageControl() : 
    text_control_(0),
    left_button_control_(0),
    right_button_control_(0),
    current_page_(1),
    total_page_(1)
{
}
//------------------------------------------------------------------------------ 
PageControl::~PageControl()
{
    Release();
}
//------------------------------------------------------------------------------ 
bool PageControl::Init(CCtrlStaticWZ* _text_control, CCtrlButtonWZ* _left_button_control, CCtrlButtonWZ* _left_list_control)
{
    text_control_ = _text_control;
    left_button_control_ = _left_button_control;
    right_button_control_ = _left_list_control;

    current_page_ = total_page_ = 1;

    return IsInit();
}
//------------------------------------------------------------------------------ 
void PageControl::Release()
{
    text_control_ = NULL;
    left_button_control_ = NULL;
    right_button_control_ = NULL;
}
//------------------------------------------------------------------------------ 
bool PageControl::MessageProc(SI_MESSAGE* msg, OUT bool& is_change)
{
    switch (msg->eResultMsg)
    {
    case RT_MSG_LBUTTONCLICK:
        return OnLButtonClick(msg, is_change);
    }

    return false;
}
//------------------------------------------------------------------------------ 
bool PageControl::OnLButtonClick(SI_MESSAGE* msg, OUT bool& is_change)
{
    if (IsInit() == false)
    {
        return false;
    }

    if (left_button_control()->GetCtrlID() == msg->dwCtrlID)
    {
        is_change = Prev();
        return true;
    }
    else if (right_button_control()->GetCtrlID() == msg->dwCtrlID)
    {
        is_change = Next();
        return true;
    }

    return false;
}
//------------------------------------------------------------------------------ 
void PageControl::SetPage(int _current_page, int _total_page)
{
    if (_current_page < 1 || _total_page < 1 || _current_page > _total_page)
    {
        return;
    }

    current_page_ = _current_page;
    total_page_ = _total_page;

    Apply();
}

//------------------------------------------------------------------------------ 
void PageControl::SetTotalPage( int _total_page )
{
    if (_total_page < 1 )
    {
        return;
    }

    if (current_page_ > _total_page)
    {
        current_page_ = _total_page;
    }

    total_page_ = _total_page;

    Apply();
}

//------------------------------------------------------------------------------ 
bool PageControl::Next()
{
    if (current_page_ < total_page_)
    {
        ++current_page_;
        Apply();
        return true;
    }
    return false;
}
//------------------------------------------------------------------------------ 
bool PageControl::Prev()
{
    if (current_page_ > 1)
    {
        --current_page_;
        Apply();
        return true;
    }
    return false;
}
//------------------------------------------------------------------------------ 
void PageControl::Apply()
{
    TCHAR msg[INTERFACE_STRING_LENGTH] = {NULL, };
    // 91013	%d / %d
    // 91000	%s / %s
    g_InterfaceManager.GetInterfaceStringFormat(msg, INTERFACE_STRING_LENGTH, 91013, current_page_, total_page_);
    text_control()->SetTextWZ(msg);
}
//#endif //#ifdef _YMS_TREECONTROL
