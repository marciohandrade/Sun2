#pragma once

//------------------------------------------------------------------------------ 
class MultiVScrollDialog
{
public:
    MultiVScrollDialog(CControlWZ* scroll_control, CControlWZ* list_control);
    virtual ~MultiVScrollDialog();
    void Update();
    void MessageProc(SI_MESSAGE* msg);
    void ResizeingVScrollControl(int scroll_max_num, int scroll_pos);
    CCtrlVScrollWZ* vscroll_control() { return vscroll_control_; }
    bool GetScrollArea(RECT& area);
    bool OnVScrollDown(SI_MESSAGE* msg);
    bool OnVScrollUp(SI_MESSAGE* msg);
    bool OnVScrollThumb(SI_MESSAGE* msg);
    bool IsOverControl(CControlWZ* control);

private:
    CCtrlVScrollWZ* vscroll_control_;
    CCtrlListWZ* list_control_;
};