#pragma once

#ifdef _DEV_VER
#ifdef _NA_006519_20130311_TEXTURE_VIEWER

#include "uiBase.h"
#include "VScrollDialog.h"

class uiTextureViewerMan;
class uiTextureViewerTextureWindow : public uiBase, public VScrollDialog
{
    enum UIControlPos
    {
        kControl_Button_B004 = 0, // X��ư
        kControl_Scroll_V000,     // ��ũ�ѹ�ư
        kControl_Picture_P000,    // Texture
        kControl_Picture_P001,    // Texture
        kControl_Text_S000,       // Text
    };

public:
    enum UIMode
    {
        kCreateMode = 0,
        kModeMax,
    };

public:
    uiTextureViewerTextureWindow(InterfaceManager* ui_manager);
    virtual ~uiTextureViewerTextureWindow(void);

public:
    virtual void Init(CDrawBase* draw_base);
    virtual void Release();
    virtual void OnUpdateSolarDialog();
    virtual void OnRenderSolarDialog();
    virtual void MessageProc(SI_MESSAGE* message);
    virtual void MouseOver_MsgProc( SI_MESSAGE* message);
    virtual void OnShowWindow(BOOL is_show);
    virtual void Process(DWORD tick_count);

public:
    void SetMode(BYTE mode); // ���⼭ ��Ʈ�� ����
    void change_texture(TCHAR* full_path);
private:
    void Render(const RECT& DestRect);
    void TextureRender(const RECT& DestRect);
    void ScrollMinusButtonRender(const RECT& DestRect);
    void ScrollPlusButtonRender(const RECT& DestRect);
    void BackgroundScrollRender(const RECT& DestRect);
    void HScrollRender(const RECT& DestRect);
    void GridRender(const RECT& DestRect);
    void DragRectRender(const RECT& DestRect);

private:
    bool OnLButtonClick(SI_MESSAGE* message);
    bool OnRButtonClick(SI_MESSAGE* message);
    bool OnMouseOver(SI_MESSAGE* message);
    bool OnDoubleLButtonClick(SI_MESSAGE* message);
    bool OnVScrollUp(SI_MESSAGE* message);
    bool OnVScrollDown(SI_MESSAGE* message);
    bool OnVScrollThumb(SI_MESSAGE* message);
    bool plus_button_click_check(void);
    bool minus_button_click_check(void);
    void TextureInfoPrint(void);
    bool OnMouseWheelScroll(BYTE mouse_wheel_state);

    void adjust_vscroll(void); // �������� ���� �Ǿ��� �� vscroll ����
    void adjust_hscroll(void); // �������� ���� �Ǿ��� �� hscroll ����
    void KeyCheck(void);
private:
    void RegisterControl();
protected:
    virtual CCtrlVScrollWZ* vscroll_control();// �߻� �޼ҵ�
    virtual bool GetScrollArea(RECT& area);   // �߻� �޼ҵ�

private:
    uiTextureViewerMan* ui_texture_viewer_manager_;
    HANDLE              ui_texture_handle_;
    CCtrlVScrollWZ*     vscroll_control_;

    float width_rate_;
    float height_rate_;
    float image_width_rate_;
    float image_height_rate_;
    int   vscroll_pos_;
    int   hscroll_pos_;

    HANDLE ui_width_plus_scroll_button_;
    HANDLE ui_width_minus_scroll_button_;
    HANDLE ui_width_bar_back_;
    HANDLE ui_width_bar_progress_;

    FRECT hscroll_plus_button_rect_;    // + �ؽ��� RECT
    FRECT hscroll_minus_button_rect_;   // - �ؽ��� RECT

    float hscroll_bar_size_;
    FRECT hscroll_bar_background_size_; // ��ũ�� �� ��� ������
    int   hscroll_bar_move_max_; // �̵� �� �� �ִ� �ִ� ��
    int   hscroll_bar_pos_; // ��ũ�� �� ��ġ
    RECT  hscroll_bar_thumb_rect_; // Thumb RECT
    POINT first_point;             // Thumb �̵� ��ų �� ó�� ��ġ
    bool  Is_mouse_lbutton_down_;  // Thumb �̵� ��ų �� ó�� ��������
    bool  Is_mouse_over_;          // RGB�� ��� �� �� ���콺�� Ŭ���̾�Ʈ ��ǥ ������ �ִ��� Ȯ�� �ϱ� ����
    TCHAR rgb_text_[256];          // RGB �ؽ�Ʈ�� ����
    float texture_scale;           // �ؽ�ó Ȯ�� ��Ҹ� ���� ����

    POINT first_drag_point_; // �巡�� �ؼ� �̹��� �̵� ��Ű�� ����
    bool   Is_Lbutton_down;

    POINT first_rect_drag_point_; // �巡�� �ؼ� ��Ʈ�� �׷��ֱ� ����
    bool   Is_rect_line_down_;    // �巡�� �ؼ� ��Ʈ�� �׸��� ���� Ű�� �������� üũ
    FRECT drag_rect_;

    int before_hscroll_pos_;
    int before_vscroll_pos_;
};

#endif
#endif