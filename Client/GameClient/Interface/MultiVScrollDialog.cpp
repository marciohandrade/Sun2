#include "SunClientPrecompiledHeader.h"

#include "MultiVScrollDialog.h"
#include "InterfaceManager.h"
#include "Mouse.h"
//------------------------------------------------------------------------------ 
MultiVScrollDialog::MultiVScrollDialog(CControlWZ* scroll_control, CControlWZ* list_control)
{
    vscroll_control_ = static_cast<CCtrlVScrollWZ*>(scroll_control);
    list_control_ = static_cast<CCtrlListWZ*>(list_control);
}
//------------------------------------------------------------------------------ 
MultiVScrollDialog::~MultiVScrollDialog()
{

}
//------------------------------------------------------------------------------ 
bool MultiVScrollDialog::IsOverControl(CControlWZ* control)
{
    if (control == NULL)
    {
        return false;
    }
    RECT rc = control->GetSizeRect();
    if (rc.left <= Mouse::Instance()->GetMouseX() && rc.right >= Mouse::Instance()->GetMouseX() && 
        rc.top <= Mouse::Instance()->GetMouseY()  && rc.bottom >= Mouse::Instance()->GetMouseY())
    {
        return true;
    }
    return false;
}
//------------------------------------------------------------------------------ 
bool MultiVScrollDialog::GetScrollArea(RECT& area)
{
    if (IsOverControl(list_control_) == true)
    {
        area = list_control_->GetSizeRect();
        return true;
    }
    return false;
}
//------------------------------------------------------------------------------ 
void MultiVScrollDialog::Update()
{
    RECT area;
    if (GetScrollArea(area))
    {
        BYTE wheel_msg = uiBase::GetMouseWheelStatus(&area);

        switch (wheel_msg)
        {
        case MOUSE_WHEEL_UP:    OnVScrollUp(NULL);  break;
        case MOUSE_WHEEL_DOWN:  OnVScrollDown(NULL); break;
        } 
    }
}
//------------------------------------------------------------------------------ 
void MultiVScrollDialog::MessageProc(SI_MESSAGE* msg)
{
    switch (msg->eResultMsg)
    {
    case RT_MSG_SB_LINETOP:
        OnVScrollUp(msg);
        break;

    case RT_MSG_SB_LINEBOTTOM:
        OnVScrollDown(msg);
        break;

    case RT_MSG_SB_THUMBTRACK:      //!< 썸을 드래그 하고 있다.
    case RT_MSG_SB_THUMBPOSITION:   //!< 썸을 드래그한후 마우스를 놓았다.
    case RT_MSG_SB_ENDSCROLL:       //!< 스크롤이 종료 되었다.
        OnVScrollThumb(msg);
        break;

    //default:
    //    uiBase::MessageProc(msg);
    //    break;
    }
}
//------------------------------------------------------------------------------ 
bool MultiVScrollDialog::OnVScrollThumb(SI_MESSAGE* msg)
{
    if (vscroll_control() == NULL)
    {
        return false;
    }

    int pos2 = vscroll_control()->GetScrollPosWZ();
    int pos = pos2;
    int mmin = 0, mmax = 0;
    vscroll_control()->GetScrollRangeWZ(&mmin, &mmax);

    pos = max(mmin, pos);
    pos = min(mmax, pos);

    if(pos != pos2)
    {
        vscroll_control()->SetScrollPosWZ(pos);
    }

    if (list_control_ != NULL)
    {
        list_control_->SetBeginDrawIndex(pos);
    }
    return true;
}
//------------------------------------------------------------------------------ 
bool MultiVScrollDialog::OnVScrollUp(SI_MESSAGE* msg)
{
    if (vscroll_control() == NULL)
    {
        return false;
    }

    int pos = vscroll_control()->GetScrollPosWZ();
    int mmin = 0, mmax = 0;
    vscroll_control()->GetScrollRangeWZ(&mmin, &mmax);
    if(pos > mmin)
    {
        --pos;
        vscroll_control()->SetScrollPosWZ(pos);
        if (list_control_ != NULL)
        {
            list_control_->SetBeginDrawIndex(pos);
        }

        return true;
    }
    return false;
}
//------------------------------------------------------------------------------ 
bool MultiVScrollDialog::OnVScrollDown(SI_MESSAGE* msg)
{
    if (vscroll_control() == NULL)
    {
        return false;
    }

    int pos = vscroll_control()->GetScrollPosWZ();
    int mmin = 0, mmax = 0;
    vscroll_control()->GetScrollRangeWZ(&mmin, &mmax);

    if (pos < mmax)
    {
        ++pos;
        vscroll_control()->SetScrollPosWZ(pos);
        if (list_control_ != NULL)
        {
            list_control_->SetBeginDrawIndex(pos);
        }

        return true;
    }
    return false;
}
//------------------------------------------------------------------------------ 
void MultiVScrollDialog::ResizeingVScrollControl(int scroll_max_num, int scroll_pos)
{
    if (vscroll_control() == NULL)
    {
        return;
    }

    int thumb_size = max(10, 100 / (scroll_max_num + 1)); 
    int range_min = 0;
    int range_max = 0;
    int scroll_position = vscroll_control()->GetScrollPosWZ();
    vscroll_control()->GetScrollRangeWZ(&range_min, &range_max);
    if (range_max != scroll_max_num)
    {
        vscroll_control()->SetScrollRangeWZ(0, scroll_max_num);
        vscroll_control()->SetChangeSizeThumb(thumb_size);

        scroll_pos = scroll_pos <= scroll_max_num ? scroll_pos : scroll_max_num;
        vscroll_control()->SetScrollPosWZ(scroll_pos);
    }
}
