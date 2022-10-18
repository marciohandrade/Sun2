#include "SunClientPrecompiledHeader.h"

#include "ListboxScrollControlProduce.h"
#include "uiBase.h"

//------------------------------------------------------------------------------ 
ListboxScrollControlProduce::ListboxScrollControlProduce() : 
    list_control_(0)
    ,vscroll_control_(0)
{

}
//------------------------------------------------------------------------------ 
ListboxScrollControlProduce::~ListboxScrollControlProduce()
{

}
//------------------------------------------------------------------------------ 
bool ListboxScrollControlProduce::Init(CCtrlListWZ* _list_control, CCtrlVScrollWZ* _vscroll_control)
{
    list_control_ = _list_control;
    vscroll_control_ = _vscroll_control;
    return true;
}
//------------------------------------------------------------------------------ 
void ListboxScrollControlProduce::Release()
{
    list_control_ = NULL;
    vscroll_control_ = NULL;
}
//------------------------------------------------------------------------------ 
bool ListboxScrollControlProduce::OnVScrollThumb(SI_MESSAGE* msg)
{
    if ((msg == NULL) || 
        (list_control() == NULL) || 
        (vscroll_control() == NULL) || 
        (msg->dwCtrlID != vscroll_control()->GetCtrlID()))
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

    list_control()->SetBeginDrawIndex(pos);

    return true;
}
//------------------------------------------------------------------------------ 
bool ListboxScrollControlProduce::OnVScrollUp(SI_MESSAGE* msg)
{
    if ((msg == NULL) || 
        (list_control() == NULL) || 
        (vscroll_control() == NULL) || 
        (msg->dwCtrlID != vscroll_control()->GetCtrlID()))
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
    }

    list_control()->SetBeginDrawIndex(pos);

    return true;
}
//------------------------------------------------------------------------------ 
bool ListboxScrollControlProduce::OnVScrollDown(SI_MESSAGE* msg)
{
    if ((msg == NULL) || 
        (list_control() == NULL) || 
        (vscroll_control() == NULL) || 
        (msg->dwCtrlID != vscroll_control()->GetCtrlID()))
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
    }

    list_control()->SetBeginDrawIndex(pos);

    return true;
}
//------------------------------------------------------------------------------ 
bool ListboxScrollControlProduce::UpdateMouseWheel()
{
    if (list_control() != NULL && vscroll_control() != NULL)
    {
        RECT area = list_control()->GetSizeRect();
        BYTE wheel_msg = uiBase::GetMouseWheelStatus(&area);

        SI_MESSAGE msg;
        msg.dwCtrlID = vscroll_control()->GetCtrlID();

        switch (wheel_msg)
        {
            case MOUSE_WHEEL_UP:    return OnVScrollUp(&msg);
            case MOUSE_WHEEL_DOWN:  return OnVScrollDown(&msg);
        }
    }

    return false;
}
//------------------------------------------------------------------------------ 
void ListboxScrollControlProduce::ResetVScroll()
{
    if (list_control() == NULL || vscroll_control() == NULL)
    {
        return;
    }

#if WZENGINEVER >= 400
    int scroll_max_num = max(0, list_control()->GetListRowCnt() - list_control()->GetDrawRowCount());

    int thumb_size = 100 / (scroll_max_num + 1);
    thumb_size = max(10, thumb_size);
    vscroll_control()->SetChangeSizeThumb(thumb_size);
    vscroll_control()->SetScrollRangeWZ(0, scroll_max_num);

    int begin_index = list_control()->GetBeginDrawIndex();

    int new_draw_index = begin_index <= scroll_max_num ? begin_index : scroll_max_num;

    list_control()->SetBeginDrawIndex(new_draw_index);
    vscroll_control()->SetScrollPosWZ(new_draw_index);
#endif //#if WZENGINEVER >= 400


}