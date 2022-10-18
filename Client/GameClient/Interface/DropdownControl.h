#pragma once

//#ifdef _YMS_TREECONTROL

#include "ListboxScrollControlProduce.h"

//------------------------------------------------------------------------------ 
class DropdownControl : public ListboxScrollControlProduce
{
public:
    DropdownControl();
    virtual ~DropdownControl();

    //------------------------------------------------------------------------------
    //! Returns:   bool
    //! Qualifier: 콤보박스 모듈
    //! Parameter: CCtrlStaticWZ * _text_control : 선택되었을경우 출력될 text control
    //! Parameter: CCtrlButtonWZ * _button_control : 리스트 박스(_list_control)를 보여줄 버튼
    //! Parameter: CCtrlListWZ * _list_control : 콤보 복록
    //! Parameter: CCtrlVScrollWZ * _vscroll_control : 스크롤 컨트롤일 있으면 넣어 준다. 없으면 NULL
    //------------------------------------------------------------------------------
    bool Init(CControlWZ* _text_control, 
                CCtrlButtonWZ* _button_control, 
                CCtrlListWZ* _list_control,
                CCtrlVScrollWZ* _vscroll_control,
                CCtrlPictureWZ* _picture_control);
    void Release();

    void InsertItem(DWORD string_code, 
                    void* user_data,
                    DWORD font_id = StrToWzID("st10"), 
                    DWORD style = DT_VCENTER | DT_LEFT, 
                    COLORREF back_color = RGBA(0, 0, 0, 0), 
                    COLORREF text_color = RGBA(255, 255, 255, 255), 
                    COLORREF select_text_color = RGBA(0, 220, 0, 255));
    void InsertItem(TCHAR* msg_text, 
                    void* user_data,
                    DWORD font_id = StrToWzID("st10"), 
                    DWORD style = DT_VCENTER | DT_LEFT, 
                    COLORREF back_color = RGBA(0, 0, 0, 0), 
                    COLORREF text_color = RGBA(255, 255, 255, 255), 
                    COLORREF select_text_color = RGBA(0, 220, 0, 255));
    void DeleteItemAll();

    //------------------------------------------------------------------------------
    //! Returns:   bool : true : 처리되었을때 이후 메세지 무시해도됨, false : 메세지 처리 하지 않음
    //! Qualifier: dropdown 관련 컨트롤 메세지 처리
    //! Parameter: SI_MESSAGE * msg
    //------------------------------------------------------------------------------
    bool MessageProc(SI_MESSAGE* msg, OUT bool& is_change);
    void Update();

    bool Select(int list_sell_index);
    bool SelectByUserData(DWORD user_data);

    //------------------------------------------------------------------------------
    //! Returns:   DWORD : InsertItem 할때 입력한 void* user_data 이다
    DWORD CurrentUserData();

    template<class T>
    T CurrentUserData(){return static_cast<T>(CurrentUserData());}

    //! 선택창 리스트 박스가 열려있는지
    bool IsOpened();

    void SetEnable(bool is_enable);
    void SetVisible(bool is_visible);
    bool IsVisible();

    bool is_simple_menu() const { return simple_menu_; }
    void set_simple_menu(bool val) { simple_menu_ = val; }
    CControlWZ* text_control() { return text_control_; }
    CCtrlButtonWZ* button_control() { return button_control_; }
    CCtrlPictureWZ* picture_control() { return picture_control_; }

private:
    CCtrlPictureWZ* blend_picture_control_;

public:
    void SetBlendImage( CCtrlPictureWZ* _blend_picture_control );
	void OnDraw();

private:
    bool OnLButtonClick(SI_MESSAGE* msg, OUT bool& is_change);
    bool IsMouseOver();
    void CollectControlRect(CControlWZ* control_pointer,
        RECT& rect_pointer,
        bool& is_first_control);

    CControlWZ* text_control_;
    CCtrlButtonWZ* button_control_;
    CCtrlPictureWZ* picture_control_;   //!< 백판

    //! 마우스가 나갔으면 자동으로 닫히게 한다.
    bool simple_menu_;
};


//#endif //#ifdef _YMS_TREECONTROL