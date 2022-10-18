#pragma once

//------------------------------------------------------------------------------ 
//! ListboxScrollControlProduce
//! 리스트 박스 옆에 스크롤바가 붙은 컨트롤 메세지 처리
//------------------------------------------------------------------------------ 
class ListboxScrollControlProduce
{
public:
    ListboxScrollControlProduce();
    virtual ~ListboxScrollControlProduce();

    virtual bool Init(CCtrlListWZ* _list_control, CCtrlVScrollWZ* _vscroll_control);
    virtual void Release();

    virtual bool UpdateMouseWheel();
    virtual void ResetVScroll();

    CCtrlListWZ* list_control() { return list_control_; }
    CCtrlVScrollWZ* vscroll_control() { return vscroll_control_; }

protected:
    virtual bool OnVScrollUp(SI_MESSAGE* msg);
    virtual bool OnVScrollDown(SI_MESSAGE* msg);
    virtual bool OnVScrollThumb(SI_MESSAGE* msg);

private:
    CCtrlListWZ* list_control_;
    CCtrlVScrollWZ* vscroll_control_;
};
