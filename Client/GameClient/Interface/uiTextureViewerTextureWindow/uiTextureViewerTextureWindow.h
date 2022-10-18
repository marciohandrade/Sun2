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
        kControl_Button_B004 = 0, // X버튼
        kControl_Scroll_V000,     // 스크롤버튼
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
    void SetMode(BYTE mode); // 여기서 스트링 지정
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

    void adjust_vscroll(void); // 스케일이 조정 되었을 때 vscroll 조정
    void adjust_hscroll(void); // 스케일이 조정 되었을 때 hscroll 조정
    void KeyCheck(void);
private:
    void RegisterControl();
protected:
    virtual CCtrlVScrollWZ* vscroll_control();// 추상 메소드
    virtual bool GetScrollArea(RECT& area);   // 추상 메소드

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

    FRECT hscroll_plus_button_rect_;    // + 텍스쳐 RECT
    FRECT hscroll_minus_button_rect_;   // - 텍스쳐 RECT

    float hscroll_bar_size_;
    FRECT hscroll_bar_background_size_; // 스크롤 바 배경 사이즈
    int   hscroll_bar_move_max_; // 이동 할 수 있는 최대 값
    int   hscroll_bar_pos_; // 스크롤 바 위치
    RECT  hscroll_bar_thumb_rect_; // Thumb RECT
    POINT first_point;             // Thumb 이동 시킬 때 처음 위치
    bool  Is_mouse_lbutton_down_;  // Thumb 이동 시킬 때 처음 눌렀는지
    bool  Is_mouse_over_;          // RGB를 출력 할 때 마우스가 클라이언트 좌표 영역에 있는지 확인 하기 위해
    TCHAR rgb_text_[256];          // RGB 텍스트를 저장
    float texture_scale;           // 텍스처 확대 축소를 위한 변수

    POINT first_drag_point_; // 드래그 해서 이미지 이동 시키기 위해
    bool   Is_Lbutton_down;

    POINT first_rect_drag_point_; // 드래그 해서 렉트를 그려주기 위해
    bool   Is_rect_line_down_;    // 드래그 해서 렉트를 그리기 위해 키를 눌렀는지 체크
    FRECT drag_rect_;

    int before_hscroll_pos_;
    int before_vscroll_pos_;
};

#endif
#endif