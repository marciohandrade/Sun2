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
    //! Qualifier: �޺��ڽ� ���
    //! Parameter: CCtrlStaticWZ * _text_control : ���õǾ������ ��µ� text control
    //! Parameter: CCtrlButtonWZ * _button_control : ����Ʈ �ڽ�(_list_control)�� ������ ��ư
    //! Parameter: CCtrlListWZ * _list_control : �޺� ����
    //! Parameter: CCtrlVScrollWZ * _vscroll_control : ��ũ�� ��Ʈ���� ������ �־� �ش�. ������ NULL
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
    //! Returns:   bool : true : ó���Ǿ����� ���� �޼��� �����ص���, false : �޼��� ó�� ���� ����
    //! Qualifier: dropdown ���� ��Ʈ�� �޼��� ó��
    //! Parameter: SI_MESSAGE * msg
    //------------------------------------------------------------------------------
    bool MessageProc(SI_MESSAGE* msg, OUT bool& is_change);
    void Update();

    bool Select(int list_sell_index);
    bool SelectByUserData(DWORD user_data);

    //------------------------------------------------------------------------------
    //! Returns:   DWORD : InsertItem �Ҷ� �Է��� void* user_data �̴�
    DWORD CurrentUserData();

    template<class T>
    T CurrentUserData(){return static_cast<T>(CurrentUserData());}

    //! ����â ����Ʈ �ڽ��� �����ִ���
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
    CCtrlPictureWZ* picture_control_;   //!< ����

    //! ���콺�� �������� �ڵ����� ������ �Ѵ�.
    bool simple_menu_;
};


//#endif //#ifdef _YMS_TREECONTROL