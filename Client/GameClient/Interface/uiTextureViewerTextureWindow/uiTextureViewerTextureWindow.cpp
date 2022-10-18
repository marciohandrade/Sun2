#include "SunClientPrecompiledHeader.h"
#ifdef _DEV_VER
#ifdef _NA_006519_20130311_TEXTURE_VIEWER
#include "uiTextureViewerTextureWindow/uitextureviewertexturewindow.h"
#include "uiTextureViewerSearchWindow/uiTextureViewerSearchWindow.h"
#include "uiTextureViewerMan/uiTextureViewerMan.h"
#include "TextureListInfoParser.h"
#include "TextureListInfoHandleManager.h"
#include "InterfaceManager.h"
#include "Mouse.h"

uiTextureViewerTextureWindow::uiTextureViewerTextureWindow(InterfaceManager* ui_manager):
uiBase(ui_manager),
vscroll_control_(NULL),
width_rate_(0.0f),
height_rate_(0.0f),
image_width_rate_(0.0f),
image_height_rate_(0.0f),
vscroll_pos_(0),
hscroll_pos_(0),
hscroll_bar_size_(0.0f),
hscroll_bar_move_max_(0),
hscroll_bar_pos_(0),
Is_mouse_lbutton_down_(false),
Is_mouse_over_(false),
texture_scale(1.0f),
Is_Lbutton_down(false),
Is_rect_line_down_(false),
before_hscroll_pos_(0),
before_vscroll_pos_(0)
{
    ui_texture_handle_ = INVALID_HANDLE_VALUE;
    ui_width_plus_scroll_button_ = INVALID_HANDLE_VALUE;
    ui_width_minus_scroll_button_ = INVALID_HANDLE_VALUE;
    ui_width_bar_back_ = INVALID_HANDLE_VALUE;
    ui_width_bar_progress_ = INVALID_HANDLE_VALUE;

    hscroll_bar_thumb_rect_.left = 0;
    hscroll_bar_thumb_rect_.top = 0;
    hscroll_bar_thumb_rect_.right = 0;
    hscroll_bar_thumb_rect_.bottom = 0;

    drag_rect_.left = 0;
    drag_rect_.top = 0;
    drag_rect_.right = 0;
    drag_rect_.bottom = 0;
}

//-----------------------------------------------------------------------------------------//

uiTextureViewerTextureWindow::~uiTextureViewerTextureWindow(void)
{
    this->Release();
}

//-----------------------------------------------------------------------------------------//

void uiTextureViewerTextureWindow::Init( CDrawBase* draw_base )
{
    RegisterControl();

    SetMode(kCreateMode);

    uiBase::Init(draw_base);

    ui_texture_viewer_manager_ = static_cast<uiTextureViewerMan*>(GetMan());
    assert(ui_texture_viewer_manager_);

    vscroll_control_ = 
        static_cast<CCtrlVScrollWZ*>(GetControlWZ(GetControlID_byPosition(kControl_Scroll_V000)));
    assert(vscroll_control_);

    if (vscroll_control_)
    {
        vscroll_control_->ShowWindowWZ(WZ_SHOW);
        VScrollDialog::ResizeingVScrollControl(0, 0);
    }

    // hscroll 버튼들 rect 초기화 이건 나중에 따로 빼두자.
    hscroll_plus_button_rect_.left = 0.0f;
    hscroll_plus_button_rect_.top = 0.0f;
    hscroll_plus_button_rect_.right = 0.0f;
    hscroll_plus_button_rect_.bottom = 0.0f;

    hscroll_minus_button_rect_.left = 0.0f;
    hscroll_minus_button_rect_.top = 0.0f;
    hscroll_minus_button_rect_.right = 0.0f;
    hscroll_minus_button_rect_.bottom = 0.0f;

    texture_scale = 1.0f;
}

//-----------------------------------------------------------------------------------------//

void uiTextureViewerTextureWindow::Release()
{
    // 리소스 관리 부분을 볼 필요가 있다.
    if (ui_texture_handle_ != INVALID_HANDLE_VALUE)
    {
        g_pSunRenderer->x_pManagerTexture->DestroyTexture(ui_texture_handle_);
        ui_texture_handle_ = INVALID_HANDLE_VALUE;
    }
    ui_width_plus_scroll_button_ = INVALID_HANDLE_VALUE;
    ui_width_minus_scroll_button_ = INVALID_HANDLE_VALUE;
    ui_width_bar_back_ = INVALID_HANDLE_VALUE;
    ui_width_bar_progress_ = INVALID_HANDLE_VALUE;
}

//-----------------------------------------------------------------------------------------//

void uiTextureViewerTextureWindow::OnUpdateSolarDialog()
{
    CDialogWZ* dialog_ptr = GetDialogWZ();
    assert(dialog_ptr);
    if(dialog_ptr == NULL)
        return;

    if (dialog_ptr)
    {
        KeyCheck();

        // 마우스 휠 처리
        RECT client_rect;
        GetDialogWZ()->GetClientRect(&client_rect);
        BYTE mouse_wheel_state = GetMouseWheelStatus(&client_rect);

        OnMouseWheelScroll(mouse_wheel_state);

        // 가로 스크롤 보정
        if (hscroll_pos_ > hscroll_bar_move_max_)
        {
            hscroll_pos_ = hscroll_bar_move_max_;
            hscroll_bar_pos_ = hscroll_bar_move_max_;
        }
        int scroll_min = 0;
        int scroll_max = 0;
        int current_scroll_pos = vscroll_control_->GetScrollPosWZ();
        vscroll_control_->GetScrollRangeWZ(&scroll_min, &scroll_max);
        if (vscroll_pos_ > scroll_max)
        {
            vscroll_pos_ = scroll_max;
            current_scroll_pos = scroll_max;
            vscroll_control_->SetScrollPosWZ(current_scroll_pos);
        }
    }
}

//-----------------------------------------------------------------------------------------//

void uiTextureViewerTextureWindow::OnRenderSolarDialog()
{
    if (ui_texture_handle_ != INVALID_HANDLE_VALUE)
    {
        RECT rcDialog = GetControlWZ_byPosition(kControl_Picture_P001)->GetSizeRect();

        RECT rc = {rcDialog.left, rcDialog.top, rcDialog.right, rcDialog.bottom};

        TextureRender(rc);

        ScrollMinusButtonRender(rc);

        ScrollPlusButtonRender(rc);

        BackgroundScrollRender(rc);

        HScrollRender(rc);

        GridRender(rc);

        DragRectRender(rc);

        TextureInfoPrint();
    }
}

//-----------------------------------------------------------------------------------------//

void uiTextureViewerTextureWindow::MessageProc( SI_MESSAGE* message )
{
    if (IsLock())
        return;

    switch (message->eResultMsg)
    {
    case RT_MSG_LBUTTONCLICK:
        {
            OnLButtonClick(message);

            Is_rect_line_down_ = false;
        }
        break;
    case RT_MSG_LBUTTONDCLICK:
        {
            OnDoubleLButtonClick(message);
        }
        break;
    case RT_MSG_MOUSEOVER:
        {
            Is_mouse_over_ = true;
        }
        break;
    case RT_MSG_EDIT_KEYDOWN:
        {

        }
    default:
        {
            SolarDialog::MessageProc(message);
            VScrollDialog::MessageProc(message);
            Is_mouse_over_ = false;
        }
        break;
    }
}

//-----------------------------------------------------------------------------------------//

void uiTextureViewerTextureWindow::MouseOver_MsgProc( SI_MESSAGE* message )
{

}

//-----------------------------------------------------------------------------------------//

void uiTextureViewerTextureWindow::OnShowWindow( BOOL is_show )
{
    if (is_show)
    {

    }
}

//-----------------------------------------------------------------------------------------//

void uiTextureViewerTextureWindow::Process( DWORD tick_count )
{

}

//-----------------------------------------------------------------------------------------//

void uiTextureViewerTextureWindow::Render( const RECT& DestRect )
{

}

//-----------------------------------------------------------------------------------------//

void uiTextureViewerTextureWindow::RegisterControl(void)
{
    RegisterControlPosition(StrToWzID("B004"), kControl_Button_B004);
    RegisterControlPosition(StrToWzID("V000"), kControl_Scroll_V000);
    RegisterControlPosition(StrToWzID("P000"), kControl_Picture_P000);
    RegisterControlPosition(StrToWzID("P001"), kControl_Picture_P001);
    RegisterControlPosition(StrToWzID("S000"), kControl_Text_S000);
}

//-----------------------------------------------------------------------------------------//

bool uiTextureViewerTextureWindow::OnLButtonClick( SI_MESSAGE* message )
{
    bool result = true;

    switch (GetControlPosition_byID(message->dwCtrlID))
    {
    case kControl_Button_B004: // 닫기
        {
            ShowInterface(FALSE);
        }
        break;
    case kControl_Scroll_V000: // 스크롤
        {

        }
        break;
    default:
        {

        }
        break;
    }

    plus_button_click_check();
    minus_button_click_check();

    return result;
}

//-----------------------------------------------------------------------------------------//

bool uiTextureViewerTextureWindow::OnRButtonClick( SI_MESSAGE* message )
{
    bool result = true;

    return result;
}

//-----------------------------------------------------------------------------------------//

bool uiTextureViewerTextureWindow::OnMouseOver( SI_MESSAGE* message )
{
    bool result = true;

    return result;
}

//-----------------------------------------------------------------------------------------//

bool uiTextureViewerTextureWindow::OnDoubleLButtonClick( SI_MESSAGE* message )
{
    bool result = true;

    return result;
}

void uiTextureViewerTextureWindow::SetMode( BYTE mode )
{
    CCtrlStaticWZ* static_constrol_ptr = NULL;
    switch (mode)
    {
    case kCreateMode:
        {
            // 다이얼로그 제목
            static_constrol_ptr = static_cast<CCtrlStaticWZ*>(GetControlWZ_byPosition(kControl_Text_S000));
            if (static_constrol_ptr)
            {
                char* temp = "텍스쳐";
                static_constrol_ptr->SetTextWZA(temp);
            }
        }
        break;
    }
}

//-----------------------------------------------------------------------------------------//

void uiTextureViewerTextureWindow::change_texture( TCHAR* full_path )
{
    if (ui_texture_handle_)
    {
        g_pSunRenderer->x_pManagerTexture->DestroyTexture(ui_texture_handle_);

        ui_texture_handle_ = INVALID_HANDLE_VALUE;

        if (ui_texture_handle_ == INVALID_HANDLE_VALUE)
        {
            ui_texture_handle_ = g_pSunRenderer->x_pManagerTexture->LoadTexture(full_path,
                TLS_ORIGINAL_SIZE | TLS_NO_MIPMAP | TLS_NO_MULTITHREAD);

            St_TextureContainer* TexInfo = g_pSunRenderer->x_pManagerTexture->GetTextureInfo(ui_texture_handle_);

            RECT DestRect = GetControlWZ_byPosition(kControl_Picture_P001)->GetSizeRect();

            if (TexInfo)
            {
                width_rate_ = (float(DestRect.right - DestRect.left)) / ((float)TexInfo->m_nImageWidth);
                height_rate_ = (float(DestRect.bottom - DestRect.top)) / ((float)TexInfo->m_nImageHeight);

                if (width_rate_ > 1.0f)
                    width_rate_ = 1.0f;
                if (height_rate_ > 1.0f)
                    height_rate_ = 1.0f;

                image_width_rate_ = TexInfo->m_nImageWidth * (width_rate_ - 1.0f);
                image_height_rate_ = TexInfo->m_nImageHeight * (height_rate_ - 1.0f);

                if (height_rate_ < 1.0f)
                {
                    int height_max = (TexInfo->m_nImageHeight) - (DestRect.bottom - DestRect.top);

                    VScrollDialog::ResizeingVScrollControl(/*스크롤 max*/ height_max, /*pos*/0 );
                }
                else
                    VScrollDialog::ResizeingVScrollControl(/*스크롤 max*/ 0, /*pos*/0 );

                vscroll_pos_ = 0;
                vscroll_control_->SetScrollPosWZ(vscroll_pos_);
                hscroll_pos_ = 0;
                hscroll_bar_move_max_ = 0;
                hscroll_bar_pos_ = 0;
                hscroll_bar_size_ = 0;
                texture_scale = 1.0f;

                drag_rect_.left = 0;
                drag_rect_.top = 0;
                drag_rect_.right = 0;
                drag_rect_.bottom = 0;

                before_hscroll_pos_ = 0;
                before_vscroll_pos_ = 0;
            }
        }
    }
}

//-----------------------------------------------------------------------------------------//

CCtrlVScrollWZ* uiTextureViewerTextureWindow::vscroll_control()
{
    return GetVScrollControlWZ(GetControlID_byPosition(kControl_Scroll_V000));
}

//-----------------------------------------------------------------------------------------//

bool uiTextureViewerTextureWindow::GetScrollArea( RECT& area )
{
    if (IsOverControl(GetControlWZ_byPosition(kControl_Picture_P001)) == true)
    {
        area = GetControlWZ_byPosition(kControl_Picture_P001)->GetSizeRect();

        return true;
    }

    return false;
}

//-----------------------------------------------------------------------------------------//

bool uiTextureViewerTextureWindow::OnVScrollUp( SI_MESSAGE* message )
{
    bool result = true;

    if (VScrollDialog::OnVScrollThumb(message) == true)
    {
        WzID scroll_control_id = GetControlID_byPosition(kControl_Scroll_V000);
        CCtrlVScrollWZ* scroll_control = 
            static_cast<CCtrlVScrollWZ*> (GetControlWZ(scroll_control_id));
        if (scroll_control != NULL)
        {
            int scroll_min = 0;
            int scroll_max = 0;
            int current_scroll_pos = scroll_control->GetScrollPosWZ();

            --current_scroll_pos;
            current_scroll_pos = max(0, current_scroll_pos);
            scroll_control->SetScrollPosWZ(current_scroll_pos);
            vscroll_pos_ = current_scroll_pos;
        }
    }

    return result;
}

//-----------------------------------------------------------------------------------------//

bool uiTextureViewerTextureWindow::OnVScrollDown( SI_MESSAGE* message )
{
    bool result = true;

    WzID scroll_control_id = GetControlID_byPosition(kControl_Scroll_V000);
    CCtrlVScrollWZ* scroll_control = 
        static_cast<CCtrlVScrollWZ*> (GetControlWZ(scroll_control_id));
    if (scroll_control != NULL)
    {
        int scroll_min = 0;
        int scroll_max = 0;
        int current_scroll_pos = scroll_control->GetScrollPosWZ();
        scroll_control->GetScrollRangeWZ(&scroll_min, &scroll_max);

        ++current_scroll_pos;
        current_scroll_pos = min(scroll_max, current_scroll_pos);
        scroll_control->SetScrollPosWZ(current_scroll_pos);
        vscroll_pos_ = current_scroll_pos;
    }

    return result;
}

//-----------------------------------------------------------------------------------------//

bool uiTextureViewerTextureWindow::OnVScrollThumb( SI_MESSAGE* message )
{
    bool result = true;

    WzID scroll_control_id = GetControlID_byPosition(kControl_Scroll_V000);
    CCtrlVScrollWZ* scroll_control = 
        static_cast<CCtrlVScrollWZ*> (GetControlWZ(scroll_control_id));
    if (scroll_control != NULL)
    {
        int scroll_min = 0;
        int scroll_max = 0;
        int current_scroll_pos = scroll_control->GetScrollPosWZ();
        scroll_control->GetScrollRangeWZ(&scroll_min, &scroll_max);
        current_scroll_pos = max(0, current_scroll_pos);
        current_scroll_pos = min(scroll_max, current_scroll_pos);

        vscroll_pos_ = current_scroll_pos;
    }
    return result;
}

//-----------------------------------------------------------------------------------------//

bool uiTextureViewerTextureWindow::plus_button_click_check( void )
{
    bool result = false;

    GetDialogWZ()->static_mousecursor_pos;
    RECT rc;
    rc.left = (LONG)hscroll_plus_button_rect_.left;
    rc.top = (LONG)hscroll_plus_button_rect_.top;
    rc.right = (LONG)hscroll_plus_button_rect_.right;
    rc.bottom = (LONG)hscroll_plus_button_rect_.bottom;

    POINT pt = Mouse::Instance()->GetMousePoint();

    if (PtInRect(&rc, pt))
    {        
        result = true;

        ++hscroll_pos_;

        ++hscroll_bar_pos_;

        if (hscroll_bar_pos_ > hscroll_bar_move_max_)
        {
            hscroll_bar_pos_ = hscroll_bar_move_max_;
            hscroll_pos_ = hscroll_bar_move_max_;
        }

        return result;
    }

    return result;
}

//-----------------------------------------------------------------------------------------//

bool uiTextureViewerTextureWindow::minus_button_click_check( void )
{
    bool result = false;

    GetDialogWZ()->static_mousecursor_pos;
    RECT rc;
    rc.left = (LONG)hscroll_minus_button_rect_.left;
    rc.top = (LONG)hscroll_minus_button_rect_.top;
    rc.right = (LONG)hscroll_minus_button_rect_.right;
    rc.bottom = (LONG)hscroll_minus_button_rect_.bottom;

    POINT pt = Mouse::Instance()->GetMousePoint();

    if (PtInRect(&rc, pt))
    {
        result = true;

        --hscroll_pos_;

        --hscroll_bar_pos_;

        if (hscroll_bar_pos_ < 0)
        {
            hscroll_bar_pos_ = 0;
            hscroll_pos_ = 0;
        }

        return result;
    }

    return result;
}

//-----------------------------------------------------------------------------------------//

void uiTextureViewerTextureWindow::TextureInfoPrint( void )
{
    if (Is_mouse_over_)
    {
        POINT pt = Mouse::Instance()->GetMousePoint();

        HDC dcHandle = GetDC(g_hWndMain);
        COLORREF color = GetPixel(dcHandle, pt.x, pt.y);
        BYTE r = GetRValue(color);
        BYTE g = GetGValue(color);
        BYTE b = GetBValue(color);

        // 다이얼로그 제목
        CCtrlStaticWZ* static_constrol_ptr = static_cast<CCtrlStaticWZ*> (GetControlWZ_byPosition(kControl_Text_S000));
        if (static_constrol_ptr)
        {
            POINT pt = Mouse::Instance()->GetMousePoint();

            RECT client_rect = GetControlWZ_byPosition(kControl_Picture_P001)->GetSizeRect();

            char* temp = "텍스쳐";

            if (PtInRect(&client_rect, pt))
            {
                pt.x -= client_rect.left - hscroll_pos_;
                pt.y -= client_rect.top - vscroll_pos_;

                pt.x = LONG((float)pt.x / texture_scale);
                pt.y = LONG((float)pt.y / texture_scale);

                if (ui_texture_handle_ == INVALID_HANDLE_VALUE)
                {
                    sprintf(rgb_text_, "%s\tR : %d\tG : %d\tB : %d\tX : %d\tY : %d", temp, r, g, b, pt.x, pt.y);
                }
                else
                {
                    St_TextureContainer* TexInfo = g_pSunRenderer->x_pManagerTexture->GetTextureInfo(ui_texture_handle_);
                    if (ui_texture_handle_)
                    {
                        if(TexInfo)
                        {
                            sprintf(rgb_text_, "%s(%d,%d)\tR : %d\tG : %d\tB : %d\tX : %d\tY : %d", 
                                temp, TexInfo->m_nImageWidth, TexInfo->m_nImageHeight, r, g, b, pt.x, pt.y);
                        }                        
                    }
                }                
            }
            else
            {
                if (ui_texture_handle_ == INVALID_HANDLE_VALUE)
                {
                    sprintf(rgb_text_, "%s", temp);
                }
                else
                {
                    if(ui_texture_handle_)
                    {
                        St_TextureContainer* TexInfo = g_pSunRenderer->x_pManagerTexture->GetTextureInfo(ui_texture_handle_);
                        if(TexInfo)
                        {
                            sprintf(rgb_text_, "%s(%d, %d)\t", temp, TexInfo->m_nImageWidth, TexInfo->m_nImageHeight);
                        }                        
                    }                    
                }                
            }
            static_constrol_ptr->SetTextWZA(rgb_text_);
        }
    }
    else
    {
        // 다이얼로그 제목
        CCtrlStaticWZ* static_constrol_ptr = static_cast<CCtrlStaticWZ*>(GetControlWZ_byPosition(kControl_Text_S000));
        if (static_constrol_ptr)
        {
            TCHAR* temp = "텍스쳐";
            static_constrol_ptr->SetTextWZA(temp);
        }
    }
}

//-----------------------------------------------------------------------------------------//

bool uiTextureViewerTextureWindow::OnMouseWheelScroll( BYTE mouse_wheel_state )
{
    if (mouse_wheel_state == MOUSE_WHEEL_UP)
    {
        texture_scale += 0.1f;

        if (texture_scale > 1.5f)
        {
            texture_scale = 1.5f;
        }
    }
    else if (mouse_wheel_state == MOUSE_WHEEL_DOWN)
    {
        texture_scale -= 0.1f;
        if (texture_scale < 1.0f)
            texture_scale = 1.0f;
    }

    adjust_vscroll();
    adjust_hscroll();

    return true;
}

//-----------------------------------------------------------------------------------------//

void uiTextureViewerTextureWindow::adjust_vscroll( void )
{
    St_TextureContainer* TexInfo = g_pSunRenderer->x_pManagerTexture->GetTextureInfo(ui_texture_handle_);

    RECT DestRect = GetControlWZ_byPosition(kControl_Picture_P001)->GetSizeRect();

    if (TexInfo)
    {
        width_rate_ = (float(DestRect.right - DestRect.left)) / (((float)TexInfo->m_nImageWidth) * texture_scale);
        height_rate_ = (float(DestRect.bottom - DestRect.top)) / ((float)TexInfo->m_nImageHeight * texture_scale);

        if(width_rate_ > 1.0f)
            width_rate_ = 1.0f;
        if(height_rate_ > 1.0f)
            height_rate_ = 1.0f;

        image_width_rate_ = (TexInfo->m_nImageWidth * texture_scale) * (width_rate_ - 1.0f);
        image_height_rate_ = (TexInfo->m_nImageHeight * texture_scale) * (height_rate_ - 1.0f);

        if (height_rate_ < 1.0f)
        {
            int height_max = int(((float)TexInfo->m_nImageHeight * (float)texture_scale)) - (DestRect.bottom - DestRect.top);

            VScrollDialog::ResizeingVScrollControl(/*스크롤 max*/ height_max, /*pos*/0 );
        }
        else
            VScrollDialog::ResizeingVScrollControl(/*스크롤 max*/ 0, /*pos*/0 );
    }
}

//-----------------------------------------------------------------------------------------//

void uiTextureViewerTextureWindow::adjust_hscroll( void )
{

}

//-----------------------------------------------------------------------------------------//

void uiTextureViewerTextureWindow::KeyCheck( void )
{
    // 가로 스크롤 Thumb를 위한
    if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
    {
        POINT pt = Mouse::Instance()->GetMousePoint();
        if (PtInRect(&hscroll_bar_thumb_rect_, pt))
        {
            if(Is_mouse_lbutton_down_ != true)
            {
                Is_mouse_lbutton_down_ = true;
                first_point = pt;
            }
            else
            {
                Is_mouse_lbutton_down_ = false;
                pt.x = pt.x - first_point.x;
                pt.y = pt.y - first_point.y;

                hscroll_bar_pos_ += pt.x;
                hscroll_pos_ += pt.x;

                if (hscroll_bar_pos_ < 0)
                {
                    hscroll_bar_pos_ = 0;
                    hscroll_pos_ = 0;
                }
                if (hscroll_bar_pos_ > hscroll_bar_move_max_)
                {
                    hscroll_bar_pos_ = hscroll_bar_move_max_;
                    hscroll_pos_ = hscroll_bar_move_max_;
                }                

                first_point.x = 0;
                first_point.y = 0;
            }
        }

        // 마우스로 이미지 드래그 하기 위해
        RECT rcDialog = GetControlWZ_byPosition(kControl_Picture_P001)->GetSizeRect();
        if (PtInRect(&rcDialog, pt))
        {
            if (GetAsyncKeyState(VK_CONTROL) & 0x8000)
            {
                if (texture_scale <= 1.0f)
                {
                    if (Is_rect_line_down_ != true)
                    {
                        Is_rect_line_down_ = true;
                        first_rect_drag_point_ = pt;
                        drag_rect_.left = static_cast<float> (pt.x);
                        drag_rect_.top = static_cast<float> (pt.y);
                        drag_rect_.right = static_cast<float> (pt.x);
                        drag_rect_.bottom = static_cast<float> (pt.y);
                    }
                    else
                    {
                        drag_rect_.right = static_cast<float> (pt.x);
                        drag_rect_.bottom = static_cast<float> (pt.y);

                        first_rect_drag_point_.x = 0;
                        first_rect_drag_point_.y = 0;
                    }
                }

                // 그리고 있는 상황 일 때는 스크롤이 되지 않도록 return 한다.
                return;
            }

            if (Is_Lbutton_down != true)
            {
                Is_Lbutton_down = true;
                first_drag_point_ = pt;
            }
            else
            {
                Is_Lbutton_down = false;
                pt.x = pt.x - first_drag_point_.x;
                pt.y = pt.y - first_drag_point_.y;

                hscroll_bar_pos_ -= pt.x;
                hscroll_pos_ -= pt.x;

                int scroll_min = 0;
                int scroll_max = 0;
                int current_scroll_pos = vscroll_control_->GetScrollPosWZ();
                current_scroll_pos -= pt.y;
                vscroll_pos_ -= pt.y;
                vscroll_control_->GetScrollRangeWZ(&scroll_min, &scroll_max);
                if (vscroll_pos_ > scroll_max)
                {
                    vscroll_pos_ = scroll_max;
                    current_scroll_pos = scroll_max;
                    vscroll_control_->SetScrollPosWZ(current_scroll_pos);
                }
                else if (vscroll_pos_ < scroll_min)
                {
                    vscroll_pos_ = scroll_min;
                    current_scroll_pos = scroll_min;
                    vscroll_control_->SetScrollPosWZ(current_scroll_pos);
                }
                else
                {
                    vscroll_control_->SetScrollPosWZ(current_scroll_pos);
                }

                if (hscroll_bar_pos_ < 0)
                {
                    hscroll_bar_pos_ = 0;
                    hscroll_pos_ = 0;
                }
                else if (hscroll_bar_pos_ > hscroll_bar_move_max_)
                {
                    hscroll_bar_pos_ = hscroll_bar_move_max_;
                    hscroll_pos_ = hscroll_bar_move_max_;
                }

                first_drag_point_.x = 0;
                first_drag_point_.y = 0;
            }
        }
    }
    else
    {
        Is_mouse_lbutton_down_ = false;
        first_point.x = 0;
        first_point.y = 0;

        first_drag_point_.x = 0;
        first_drag_point_.y = 0;
        Is_Lbutton_down = false;

        first_rect_drag_point_.x = 0;
        first_rect_drag_point_.y = 0;
        Is_rect_line_down_ = false;
    }

    if (GetAsyncKeyState(VK_RBUTTON) & 0x8000)
    {
        drag_rect_.left = 0;
        drag_rect_.top = 0;
        drag_rect_.right = 0;
        drag_rect_.bottom = 0;
    }
}

//-----------------------------------------------------------------------------------------//

void uiTextureViewerTextureWindow::TextureRender( const RECT& DestRect )
{
    // 텍스쳐 출력
    if (ui_texture_handle_ != INVALID_HANDLE_VALUE)
    {
        St_TextureContainer* TexInfo = g_pSunRenderer->x_pManagerTexture->GetTextureInfo(ui_texture_handle_);

        if (TexInfo)
        {
            // 비율을 계산해서 적용
            width_rate_ = (static_cast<float> (DestRect.right - DestRect.left)) / 
                (((static_cast<float> (TexInfo->m_nImageWidth)) * texture_scale));
            height_rate_ = (static_cast<float> (DestRect.bottom - DestRect.top)) / 
                (((static_cast<float> (TexInfo->m_nImageHeight)) * texture_scale));

            if (width_rate_ > 1.0f)
                width_rate_ = 1.0f;
            if (height_rate_ > 1.0f)
                height_rate_ = 1.0f;

            image_width_rate_ = (TexInfo->m_nImageWidth * texture_scale) * (width_rate_ - 1.0f);
            image_height_rate_ = (TexInfo->m_nImageHeight * texture_scale) * (height_rate_ - 1.0f);

            float origin_image_width_rate = (static_cast<float> (TexInfo->m_nImageWidth)) * 
                (width_rate_ - 1.0f);
            float origin_image_height_rate = (static_cast<float> (TexInfo->m_nImageHeight)) * 
                (height_rate_ - 1.0f);

            float result_Image_width = TexInfo->m_nImageWidth + origin_image_width_rate;
            float result_Image_height = TexInfo->m_nImageHeight + origin_image_height_rate;

            g_pSunRenderer->RenderTexture(ui_texture_handle_,
                static_cast<float> (DestRect.left),
                static_cast<float> (DestRect.top),
                static_cast<float> (((static_cast<float> (TexInfo->m_nImageWidth)) * texture_scale) + 
                image_width_rate_),
                static_cast<float> (((static_cast<float> (TexInfo->m_nImageHeight)) * texture_scale) + 
                image_height_rate_),
                static_cast<float> (hscroll_pos_ / texture_scale),
                static_cast<float> (vscroll_pos_ / texture_scale),
                static_cast<float> (result_Image_width),
                static_cast<float> (result_Image_height),
                NULL);
        }
    }
}

//-----------------------------------------------------------------------------------------//

void uiTextureViewerTextureWindow::ScrollMinusButtonRender( const RECT& DestRect )
{
    BASE_TextureListInfo* tex_minus_info = NULL;

    ui_width_minus_scroll_button_ = TextureListInfoHandleManager::Instance()
        ->GetTexture(10003, tex_minus_info);

    // 마이너스 스크롤 버튼 뿌리기
    if (ui_width_minus_scroll_button_ != INVALID_HANDLE_VALUE &&
        tex_minus_info != NULL)
    {
        g_pSunRenderer->RenderTexture(ui_width_minus_scroll_button_,
            static_cast<float> (DestRect.left + 3),
            static_cast<float> (DestRect.bottom + 3),
            static_cast<float> (tex_minus_info->tex_size.right),
            static_cast<float> (tex_minus_info->tex_size.bottom),
            static_cast<float> (tex_minus_info->tex_size.left),
            static_cast<float> (tex_minus_info->tex_size.top),
            static_cast<float> (tex_minus_info->tex_size.right),
            static_cast<float> (tex_minus_info->tex_size.bottom));

        hscroll_minus_button_rect_.left = static_cast<float> (DestRect.left + 3);
        hscroll_minus_button_rect_.top = static_cast<float> (DestRect.bottom + 3);
        hscroll_minus_button_rect_.right = static_cast<float> (DestRect.left + 3) + 
            static_cast<float> (tex_minus_info->tex_size.right);
        hscroll_minus_button_rect_.bottom = static_cast<float> (DestRect.bottom + 3) + 
            static_cast<float> (tex_minus_info->tex_size.bottom);
    }
}

//-----------------------------------------------------------------------------------------//

void uiTextureViewerTextureWindow::ScrollPlusButtonRender( const RECT& DestRect )
{
    BASE_TextureListInfo* tex_plus_info = NULL;

    ui_width_plus_scroll_button_ = TextureListInfoHandleManager::Instance()
        ->GetTexture(10002, tex_plus_info);

    // 플러스 스크롤 버튼 뿌리기
    if (ui_width_plus_scroll_button_ != INVALID_HANDLE_VALUE && 
        tex_plus_info != NULL)
    {
        g_pSunRenderer->RenderTexture(ui_width_plus_scroll_button_,
            static_cast<float> (DestRect.right - 3),
            static_cast<float> (DestRect.bottom + 3),
            static_cast<float> (tex_plus_info->tex_size.right),
            static_cast<float> (tex_plus_info->tex_size.bottom),
            static_cast<float> (tex_plus_info->tex_size.left),
            static_cast<float> (tex_plus_info->tex_size.top),
            static_cast<float> (tex_plus_info->tex_size.right),
            static_cast<float> (tex_plus_info->tex_size.bottom));

        hscroll_plus_button_rect_.left = float(DestRect.right - 3);
        hscroll_plus_button_rect_.top = float(DestRect.bottom + 3);
        hscroll_plus_button_rect_.right = float(DestRect.right - 3) + float(tex_plus_info->tex_size.right);
        hscroll_plus_button_rect_.bottom = float(DestRect.bottom + 3) + float(tex_plus_info->tex_size.bottom);
    }
}

//-----------------------------------------------------------------------------------------//

void uiTextureViewerTextureWindow::BackgroundScrollRender( const RECT& DestRect )
{
    BASE_TextureListInfo* tex_width_bar_back_info = NULL;
    ui_width_bar_back_ = TextureListInfoHandleManager::Instance()
        ->GetTexture(10022, tex_width_bar_back_info);
    BASE_TextureListInfo* tex_plus_info = NULL;
    ui_width_plus_scroll_button_ = TextureListInfoHandleManager::Instance()
        ->GetTexture(10002, tex_plus_info);
    BASE_TextureListInfo* tex_minus_info = NULL;
    ui_width_minus_scroll_button_ = TextureListInfoHandleManager::Instance()
        ->GetTexture(10003, tex_minus_info);

    // 뒤에 스크롤 기본 배경 뿌리기
    if (ui_width_bar_back_ != INVALID_HANDLE_VALUE &&
        tex_plus_info != NULL &&
        tex_minus_info != NULL &&
        tex_width_bar_back_info != NULL)
    {
        g_pSunRenderer->RenderTexture(ui_width_bar_back_,
            static_cast<float> (DestRect.left + tex_minus_info->tex_size.right),
            static_cast<float> (DestRect.bottom + 3),
            static_cast<float> (DestRect.right - DestRect.left - tex_plus_info->tex_size.right - 3),
            static_cast<float> (tex_plus_info->tex_size.bottom),
            static_cast<float> (tex_width_bar_back_info->tex_size.left + 3),
            static_cast<float> (tex_width_bar_back_info->tex_size.top),
            static_cast<float> (283),
            static_cast<float> (tex_width_bar_back_info->tex_size.bottom));

        hscroll_bar_background_size_.left = 0.0f;
        hscroll_bar_background_size_.top = 0.0f;
        hscroll_bar_background_size_.right = static_cast<float> ((DestRect.right - DestRect.left - 
            tex_plus_info->tex_size.right - 3));
        hscroll_bar_background_size_.bottom = static_cast<float> (tex_plus_info->tex_size.bottom);

        RECT DestRect = GetControlWZ_byPosition(kControl_Picture_P001)->GetSizeRect();

        // 스크롤 뿌리기 전에 비율 계산
        if (width_rate_ < 1.0f)
        {
            St_TextureContainer* TexInfo = g_pSunRenderer->x_pManagerTexture->GetTextureInfo(ui_texture_handle_);

            int width_max = static_cast<int> (((float)TexInfo->m_nImageWidth * (float)texture_scale) 
                - (DestRect.right - DestRect.left));

            hscroll_bar_move_max_ = width_max;

            width_max = static_cast<int> ((hscroll_bar_background_size_.right - hscroll_bar_background_size_.left)
                - width_max);

            hscroll_bar_size_ = static_cast<float> (width_max);
        }
        else
            hscroll_bar_move_max_ = 0;
    }
}

//-----------------------------------------------------------------------------------------//

void uiTextureViewerTextureWindow::HScrollRender( const RECT& DestRect )
{
    BASE_TextureListInfo* tex_width_bar_progress_info = NULL;
    ui_width_bar_progress_ = TextureListInfoHandleManager::Instance()
        ->GetTexture(10024, tex_width_bar_progress_info);
    BASE_TextureListInfo* tex_plus_info = NULL;
    ui_width_plus_scroll_button_ = TextureListInfoHandleManager::Instance()
        ->GetTexture(10002, tex_plus_info);

    // 스크롤 뿌리기
    if (ui_width_bar_progress_ != INVALID_HANDLE_VALUE &&
        tex_plus_info != NULL &&
        tex_width_bar_progress_info != NULL)
    {
        g_pSunRenderer->RenderTexture(ui_width_bar_progress_,
            static_cast<float> (DestRect.left + tex_plus_info->tex_size.right + 2.0f + hscroll_bar_pos_),
            static_cast<float> (DestRect.bottom + 6.0f),
            static_cast<float> (hscroll_bar_size_),
            static_cast<float> (tex_plus_info->tex_size.bottom - 6.0f),
            static_cast<float> (tex_width_bar_progress_info->tex_size.left),
            static_cast<float> (tex_width_bar_progress_info->tex_size.top),
            static_cast<float> (tex_width_bar_progress_info->tex_size.right),
            static_cast<float> (tex_width_bar_progress_info->tex_size.bottom));

        // Thumb 충돌 처리를 위해서 저장 해 둔다.
        hscroll_bar_thumb_rect_.left = static_cast<LONG>(DestRect.left + tex_plus_info->tex_size.right + 2.0f + hscroll_bar_pos_);
        hscroll_bar_thumb_rect_.top  = static_cast<LONG>(DestRect.bottom + 6.0f);
        hscroll_bar_thumb_rect_.right = static_cast<LONG>(hscroll_bar_thumb_rect_.left + hscroll_bar_size_);
        hscroll_bar_thumb_rect_.bottom = static_cast<LONG>(hscroll_bar_thumb_rect_.top + tex_plus_info->tex_size.bottom - 6.0f);
    }
}

//-----------------------------------------------------------------------------------------//

void uiTextureViewerTextureWindow::GridRender( const RECT& DestRect )
{
    uiTextureViewerMan* manager = static_cast<uiTextureViewerMan*> (GetMan());

    uiBase* search_window = 
        (manager->GetTextureViewerWindow(UI_TEXTURE_VIEWER_SEARCH_WINDOW));

    if(((uiTextureViewerSearchWindow*) (search_window))->get_Is_grid_mode() == false)
    {
        return;
    }

    float interval = 32.0f * texture_scale;

    St_TextureContainer* TexInfo = g_pSunRenderer->x_pManagerTexture->GetTextureInfo(ui_texture_handle_);
    //assert(TexInfo);

    if(!TexInfo)
        return;

    // 렉트를 다시 정의 해 줌으로서 해결 해 줄 수 있을 것 같다.
    int width_line_num = static_cast<int>((TexInfo->m_nHeight) * texture_scale) / static_cast<int>(interval);

    FRECT DestFRect = { static_cast<float>(DestRect.left) - hscroll_pos_, static_cast<float>(DestRect.top) - vscroll_pos_,
        (static_cast<float>(DestRect.left) - hscroll_pos_) + (static_cast<float>(TexInfo->m_nWidth) * texture_scale),
        (static_cast<float>(DestRect.top) - vscroll_pos_) + (static_cast<float>(TexInfo->m_nHeight) * texture_scale)};

    /*
    |--------------|
    |--------------|
    |______________|
    */

    FRECT widthRect = {DestFRect.left, DestFRect.top, DestFRect.right, DestFRect.bottom };

    if (DestFRect.left < DestRect.left)
        widthRect.left = static_cast<float> (DestRect.left);
    if (DestFRect.right > DestRect.right)
        widthRect.right = static_cast<float> (DestRect.right);

    for (int i = 0; i <= width_line_num; ++i)
    {
        if ((DestFRect.top) + (i * interval) >= DestRect.top
            && (DestFRect.top) + (i * interval) <= DestRect.bottom)
        {
            g_pSunRenderer->Render2DLine(static_cast<float> (widthRect.left),
                static_cast<float> (widthRect.top) + (i * interval),
                static_cast<float> (widthRect.right),
                static_cast<float> (widthRect.top) + (i * interval),
                WzColor_RGBA(255, 0, 0, 255));
        }
    }

    int height_line_num = static_cast<int> ((TexInfo->m_nWidth) * texture_scale) / static_cast<int> (interval);

    FRECT heightRect = { DestFRect.left, DestFRect.top, DestFRect.right, DestFRect.bottom };

    if (DestFRect.top < DestRect.top)
        heightRect.top = static_cast<float> (DestRect.top);
    if (DestFRect.bottom > DestRect.bottom)
        heightRect.bottom = static_cast<float> (DestRect.bottom);

    for (int j = 0; j <= height_line_num; ++j)
    {
        if ((DestFRect.left) + (j * interval) <= DestRect.right
            && (DestFRect.left) + (j * interval) >= DestRect.left)
        {
            g_pSunRenderer->Render2DLine(static_cast<float> (heightRect.left) + (j * interval),
                static_cast<float> (heightRect.top),
                static_cast<float> (heightRect.left) + (j * interval),
                static_cast<float> (heightRect.bottom),
                WzColor_RGBA(255, 0, 0, 255));
        }
    }
}

//-----------------------------------------------------------------------------------------//

void uiTextureViewerTextureWindow::DragRectRender( const RECT& DestRect )
{
    if(hscroll_pos_ != before_hscroll_pos_)
    {
        int differ = static_cast<int> ((hscroll_pos_ / texture_scale) - (before_hscroll_pos_ / texture_scale));

        drag_rect_.left -= (differ / texture_scale);
        drag_rect_.right -= (differ / texture_scale);
        before_hscroll_pos_ = hscroll_pos_;
    }
    if(vscroll_pos_ != before_vscroll_pos_)
    {
        int differ = vscroll_pos_ - before_vscroll_pos_;

        drag_rect_.top -= (differ / texture_scale);
        drag_rect_.bottom -= (differ / texture_scale);

        before_vscroll_pos_ = vscroll_pos_;
    }

    // 마우스 드래그 rect Render
    FRECT render_drag_rect = drag_rect_;

    float dist_left = render_drag_rect.left - DestRect.left;
    float dist_top = render_drag_rect.top - DestRect.top;
    float dist_right = render_drag_rect.right - DestRect.left;
    float dist_bottom = render_drag_rect.bottom - DestRect.top;

    render_drag_rect.left = DestRect.left + (dist_left * texture_scale);
    render_drag_rect.top = DestRect.top + (dist_top * texture_scale);
    render_drag_rect.right = DestRect.left + (dist_right * texture_scale);
    render_drag_rect.bottom = DestRect.top + (dist_bottom * texture_scale);

    if (drag_rect_.left > static_cast<float> (DestRect.left) && drag_rect_.top > 0.0f)
    {
        FRECT resultRect = { render_drag_rect.left, render_drag_rect.top, render_drag_rect.right, render_drag_rect.bottom };

        if (render_drag_rect.top >= DestRect.top && render_drag_rect.top <= DestRect.bottom)
        {
            if (resultRect.right > DestRect.right)
                resultRect.right = static_cast<float> (DestRect.right);
            if (resultRect.left < DestRect.left)
                resultRect.left = static_cast<float> (DestRect.left);

            g_pSunRenderer->Render2DLine(resultRect.left, resultRect.top,
                resultRect.right, resultRect.top, WzColor_RGBA(255, 255, 255, 255));
        }

        if ( render_drag_rect.right <= DestRect.right && render_drag_rect.right >= DestRect.left)
        {
            if (render_drag_rect.bottom > DestRect.bottom)
                resultRect.bottom = static_cast<float> (DestRect.bottom);
            if(render_drag_rect.top < DestRect.top)
                resultRect.top = static_cast<float> (DestRect.top);

            g_pSunRenderer->Render2DLine(resultRect.right, resultRect.top,
                resultRect.right, resultRect.bottom, WzColor_RGBA(255, 255, 255, 255));
        }

        if( render_drag_rect.bottom <= DestRect.bottom && render_drag_rect.bottom >= DestRect.top)
        {
            if (render_drag_rect.right > DestRect.right)
                resultRect.right = static_cast<float> (DestRect.right);
            if (render_drag_rect.left < DestRect.left)
                resultRect.left = static_cast<float> (DestRect.left);

            g_pSunRenderer->Render2DLine(resultRect.left, resultRect.bottom,
                resultRect.right, resultRect.bottom, WzColor_RGBA(255, 255, 255, 255));
        }

        if (render_drag_rect.left >= DestRect.left && render_drag_rect.left <= DestRect.right)
        {
            if (render_drag_rect.top < DestRect.top)
                resultRect.top = static_cast<float> (DestRect.top);
            if(render_drag_rect.bottom > DestRect.bottom)
                resultRect.bottom = static_cast<float> (DestRect.bottom);

            g_pSunRenderer->Render2DLine(resultRect.left, resultRect.bottom,
                resultRect.left, resultRect.top, WzColor_RGBA(255, 255, 255, 255));
        }
    }
}

//-----------------------------------------------------------------------------------------//
#endif
#endif