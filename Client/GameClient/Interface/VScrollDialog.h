#pragma once

//------------------------------------------------------------------------------ 
class VScrollDialog
{
public:
    virtual void Update();
    virtual void MessageProc(SI_MESSAGE* msg);

protected:
    virtual CCtrlVScrollWZ* vscroll_control() = 0;
    virtual bool GetScrollArea(RECT& area) = 0;
    virtual void ResizeingVScrollControl(int scroll_max_num, int scroll_pos);
    virtual bool OnVScrollDown(SI_MESSAGE* msg);
    virtual bool OnVScrollUp(SI_MESSAGE* msg);
    virtual bool OnVScrollThumb(SI_MESSAGE* msg);

};
