#include "SunClientPrecompiledHeader.h"

#include "DropdownControl.h"
#include "InterfaceManager.h"
#include "Mouse.h"

//------------------------------------------------------------------------------ 
DropdownControl::DropdownControl() 
: text_control_(NULL)
, button_control_(NULL)
, picture_control_(NULL)
, simple_menu_(true)
, blend_picture_control_(NULL)
{

}
//------------------------------------------------------------------------------ 
DropdownControl::~DropdownControl()
{

}
//------------------------------------------------------------------------------ 
bool DropdownControl::Init(CControlWZ* _text_control, 
                           CCtrlButtonWZ* _button_control, 
                           CCtrlListWZ* _list_control,
                           CCtrlVScrollWZ* _vscroll_control,
                           CCtrlPictureWZ* _picture_control)
{
    if (ListboxScrollControlProduce::Init(_list_control, _vscroll_control) == false)
    {
        return false;
    }
    
    text_control_ = _text_control;
    button_control_ = _button_control;
    picture_control_ = _picture_control;

    if (list_control() != NULL)
    {
        list_control()->ShowWindowWZ(WZ_HIDE);
    }
    if (vscroll_control() != NULL)
    {
        vscroll_control()->ShowWindowWZ(WZ_HIDE);
    }

    return true;
}

//------------------------------------------------------------------------------ 
void DropdownControl::Release()
{
    ListboxScrollControlProduce::Release();
    text_control_ = NULL;
    button_control_ = NULL;
    picture_control_ = NULL;
}
//------------------------------------------------------------------------------ 
void DropdownControl::InsertItem(DWORD string_code, 
                                 void* user_data, 
                                 DWORD font_id /*= StrToWzID("st10")*/,
                                 DWORD style /*= DT_VCENTER | DT_LEFT*/, 
                                 COLORREF back_color /*= RGBA(0, 0, 0, 0)*/, 
                                 COLORREF text_color /*= RGBA(255, 255, 255, 255)*/, 
                                 COLORREF select_text_color /*= RGBA(0, 220, 0, 255)*/)
{
    TCHAR msg_text[INTERFACE_STRING_LENGTH] = {0,};

    g_InterfaceManager.GetInterfaceString(string_code, msg_text, INTERFACE_STRING_LENGTH);

    int index = list_control()->InsertItemBack();

    list_control()->SetItemWZA(index, 
                            msg_text, 
                            font_id, 
                            style, 
                            back_color, 
                            text_color, 
                            select_text_color);
#if WZENGINEVER >= 400
    list_control()->SetItemUserData(index, 0, user_data);
#endif //#if WZENGINEVER >= 400
}
//------------------------------------------------------------------------------ 
void DropdownControl::InsertItem(TCHAR* msg_text, 
                                 void* user_data, 
                                 DWORD font_id /*= StrToWzID("st10")*/,
                                 DWORD style /*= DT_VCENTER | DT_LEFT*/, 
                                 COLORREF back_color /*= RGBA(0, 0, 0, 0)*/, 
                                 COLORREF text_color /*= RGBA(255, 255, 255, 255)*/, 
                                 COLORREF select_text_color /*= RGBA(0, 220, 0, 255)*/)
{
    int index = list_control()->InsertItemBack();

    list_control()->SetItemWZA(index, 
                            msg_text, 
                            font_id, 
                            style, 
                            back_color, 
                            text_color, 
                            select_text_color);
#if WZENGINEVER >= 400
    list_control()->SetItemUserData(index, 0, user_data);
#endif //#if WZENGINEVER >= 400
}
//------------------------------------------------------------------------------ 
void DropdownControl::DeleteItemAll()
{
    if (list_control() != NULL)
    {
        list_control()->DeleteItemAll();
    }
    if (text_control() != NULL)
    {
        text_control()->SetTextWZ("");
        text_control()->SetUserData(NULL);
    }
}
//------------------------------------------------------------------------------ 
bool DropdownControl::MessageProc(SI_MESSAGE* msg, OUT bool& is_change)
{
    switch (msg->eResultMsg)
    {
    case RT_MSG_LBUTTONCLICK:
        return OnLButtonClick(msg, is_change);

    case RT_MSG_SB_LINETOP:
        return OnVScrollUp(msg);

    case RT_MSG_SB_LINEBOTTOM:
        return OnVScrollDown(msg);

    case RT_MSG_SB_THUMBTRACK:      //!< 썸을 드래그 하고 있다.
    case RT_MSG_SB_THUMBPOSITION:   //!< 썸을 드래그한후 마우스를 놓았다.
    case RT_MSG_SB_ENDSCROLL:       //!< 스크롤이 종료 되었다.
        return OnVScrollThumb(msg);
    }

    return false;
}
//------------------------------------------------------------------------------ 
bool DropdownControl::OnLButtonClick(SI_MESSAGE* msg, OUT bool& is_change)
{
    if (list_control() != NULL)
    {
        if (list_control()->GetCtrlID() == msg->dwCtrlID) 
        {
            int cur_index = list_control()->GetCurSel();
            if (cur_index >= 0)
            {
                is_change = Select(cur_index);
            }
            return true;
        }
        else if ((button_control() != NULL) && (button_control()->GetCtrlID() == msg->dwCtrlID) ||
            (text_control() != NULL) && (text_control()->GetCtrlID() == msg->dwCtrlID) )

        {
            ENUM_STATEWZ state = WZ_SHOW;
            if ((list_control()->GetListRowCnt() <= 0) || (list_control()->GetState() == WZ_SHOW))
            {
                state = WZ_HIDE;
            }

            list_control()->ShowWindowWZ(state);
            if (vscroll_control() != NULL)
            {
                vscroll_control()->ShowWindowWZ(state);
            }
            return true;
        }
        return false;
    }
    return false;
}
//------------------------------------------------------------------------------ 
bool DropdownControl::Select(int list_sell_index)
{
    if ((list_control() == NULL) || (list_control()->GetListRowCnt() <= list_sell_index))
    {
        return false;
    }

    if (list_sell_index != list_control()->GetCurSel())
    {
        list_control()->SetCurSel(list_sell_index);
    }
    
    if (text_control() != NULL)
    {
#if WZENGINEVER >= 400
        text_control()->SetTextWZ(list_control()->GetItemText(list_sell_index, 0));
        text_control()->SetUserData((DWORD)(list_control()->GetItemUserData(list_sell_index, 0)));
#endif //#if WZENGINEVER >= 400
    }

    list_control()->ShowWindowWZ(WZ_HIDE);
    if (vscroll_control() != NULL)
    {
        vscroll_control()->ShowWindowWZ(WZ_HIDE);
    }

    return true;
}
//------------------------------------------------------------------------------ 
bool DropdownControl::SelectByUserData(DWORD user_data)
{
    if (list_control() == NULL)
    {
        return false;
    }

    int total_count = list_control()->GetListRowCnt();
    for (int i = 0; i < total_count; ++i)
    {
#if WZENGINEVER >= 410
        if (DWORD(list_control()->GetItemUserData(i, 0)) == user_data)
        {
            return Select(i);
        }
#endif //#if WZENGINEVER >= 410
    }
    return false;
}
//------------------------------------------------------------------------------ 
void DropdownControl::Update()
{
    if (is_simple_menu() && 
        (list_control()->GetState() == WZ_SHOW) && 
        (IsMouseOver() == false))
    {
        list_control()->ShowWindowWZ(WZ_HIDE);

    }

    if( blend_picture_control_ != NULL )
    {
        ENUM_STATEWZ state = WZ_SHOW;
        if( list_control()->GetDrawState() == E_DISABLE )
        {
            state = WZ_HIDE;
        }
        else if( list_control()->GetState() == WZ_SHOW )
        {
            state = WZ_HIDE;
        }

        blend_picture_control_->ShowWindowWZ(state);


        // 블랜딩을 위한 알파값을 구함 (0.0 ~ 1.0 구간)
        DWORD current_tick = g_pSunRenderer->x_Clock.GetCurrentTick();
        static DWORD last_time = current_tick;
        static float alpha_blend_rate = 0.0f;
        static bool alpha_blend_inc = true;


        static float s_rate = 0.003f;
        float delta_tick = (float)(current_tick - last_time) * 0.003f;
        last_time = current_tick;

        if( alpha_blend_inc == true )
        {
            alpha_blend_rate += delta_tick;
            if( alpha_blend_rate >= 1.0f )
            {
                alpha_blend_inc = false;
                alpha_blend_rate = 1.0f;
            }
        }
        else
        {
            alpha_blend_rate -= delta_tick;
            if( alpha_blend_rate <= 0.0f )
            {
                alpha_blend_inc = true;
                alpha_blend_rate = 0.0f;
            }
        }

        blend_picture_control_->SetTransprency( alpha_blend_rate );
    }

    UpdateMouseWheel();
}
//------------------------------------------------------------------------------ 
bool DropdownControl::IsMouseOver()
{
    POINT pt;
    pt.x = Mouse::Instance()->GetMouseX();
    pt.y = Mouse::Instance()->GetMouseY();
    

    RECT mouse_over_rect;
    mouse_over_rect.left = 0;
    mouse_over_rect.top = 0;
    mouse_over_rect.right = 0;
    mouse_over_rect.bottom = 0;

    bool is_first_control = true;

    // 컨트롤들의 영역을 취합
    if (list_control() != NULL)
    {
        CollectControlRect(list_control(), mouse_over_rect, is_first_control);
    }
    if (text_control() != NULL)
    {
        CollectControlRect(text_control(), mouse_over_rect, is_first_control);
    }
    if (vscroll_control() != NULL)
    {
        CollectControlRect(vscroll_control(), mouse_over_rect, is_first_control);
    }
    if (button_control() != NULL)
    {
        CollectControlRect(button_control(), mouse_over_rect, is_first_control);
    }

    bool is_in = DEF_IntersectRect(mouse_over_rect, pt);

    return is_in;
}
//------------------------------------------------------------------------------ 
DWORD DropdownControl::CurrentUserData()
{
    if (text_control() != NULL && IsVisible())
    {
        return text_control()->GetUserData();
    }
    return 0xffffffff;
}
////------------------------------------------------------------------------------ 
//template <class T>
//T DropdownControl<T>::CurrentUserData()
//{
//    return 0;
//}
//------------------------------------------------------------------------------ 
bool DropdownControl::IsOpened()
{
    if (list_control() != NULL)
    {
        return (list_control()->GetState() == WZ_SHOW);
    }
    return false;
}
//------------------------------------------------------------------------------ 
void DropdownControl::SetEnable(bool is_enable)
{
    text_control()->EnableWindowWZ(is_enable);
    button_control()->EnableWindowWZ(is_enable);
    list_control()->EnableWindowWZ(is_enable);

    if( blend_picture_control_ != NULL )
        blend_picture_control_->ShowWindowWZ( is_enable ? WZ_SHOW : WZ_HIDE );
}
//------------------------------------------------------------------------------ 
void DropdownControl::SetVisible(bool is_visible)
{
    text_control()->ShowWindowWZ(is_visible ? WZ_SHOW : WZ_HIDE);
    button_control()->ShowWindowWZ(is_visible ? WZ_SHOW : WZ_HIDE);
    list_control()->ShowWindowWZ(is_visible ? WZ_SHOW : WZ_HIDE);

    if( picture_control_ != NULL )
        picture_control_->ShowWindowWZ(is_visible ? WZ_SHOW : WZ_HIDE);

    if( blend_picture_control_ != NULL )
        blend_picture_control_->ShowWindowWZ( is_visible ? WZ_SHOW : WZ_HIDE );

}
//------------------------------------------------------------------------------ 
bool DropdownControl::IsVisible()
{
    if (text_control() != NULL)
    {
        return (text_control()->GetState() != WZ_HIDE);
    }
    return false;
}

void DropdownControl::CollectControlRect(CControlWZ* control_pointer,
                                         RECT& rect_pointer,
                                         bool& is_first_control)
{
    if (control_pointer != NULL)
    {
        RECT rc = control_pointer->GetSizeRect();
        if (is_first_control == true)
        {
            rect_pointer.left = rc.left;
            rect_pointer.top = rc.top;
            rect_pointer.right = rc.right;
            rect_pointer.bottom = rc.bottom;
            is_first_control = false;
        }
        else
        {
            rect_pointer.left = min(rect_pointer.left, rc.left);
            rect_pointer.top = min(rect_pointer.top, rc.top);
            rect_pointer.right = max(rect_pointer.right, rc.right);
            rect_pointer.bottom = max(rect_pointer.bottom, rc.bottom);
        }
    }
}

void DropdownControl::SetBlendImage( CCtrlPictureWZ* _blend_picture_control )
{
    blend_picture_control_ = _blend_picture_control;
    if( blend_picture_control_ != NULL )
    {
        blend_picture_control_->SetDiscardMessage(true);
    }
}

void DropdownControl::OnDraw()
{
	if(text_control_)
	{
		text_control_->OnDraw(true);
	}

	if(button_control_)
	{
		button_control_->OnDraw(true);
	}

	if(picture_control_)
	{
		picture_control_->OnDraw(true);
	}
}
