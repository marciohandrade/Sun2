#pragma once

#ifdef _SCJ_TOGGLE_EDITBOX_CONTROL


//------------------------------------------------------------------------------ 
class ToggleEditBox
{
public:
    ToggleEditBox(CControlWZ* edit_control, CControlWZ* static_control);
    ~ToggleEditBox();

    bool MessageProc(SI_MESSAGE* msg);
    void ActiveEditControl(bool is_show);
    bool is_active() { return is_active_; }
    bool is_block() { return is_block_; }
    void set_is_block(bool is_bool) { is_block_ = is_bool; }
    void Block();
    void ClearText();
    void GetText(TCHAR* string, int count);
    void SetText(TCHAR* string);

    CCtrlDxEditBox* edit_control() { return edit_control_; }
    CCtrlStaticWZ* static_control() { return static_control_; }

private:
    bool OnLButtonClick(SI_MESSAGE* msg);
    bool OnLButtonDClick(SI_MESSAGE* msg);

    bool is_active_;
    bool is_block_;  // 활성화 시 블럭 씌우기
    CCtrlDxEditBox* edit_control_;
    CCtrlStaticWZ* static_control_;
};
#endif //_SCJ_TOGGLE_EDITBOX_CONTROL
