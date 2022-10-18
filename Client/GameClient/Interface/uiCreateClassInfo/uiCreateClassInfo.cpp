#include "SunClientPrecompiledHeader.h"
#include "uiCreateClassInfo.h"
#include "uiCharacterSelectMan/uiCharacterSelectMan.h"
#include "FTextParser.h"
#include "FTextSentence.h"
#include "GraphicResourceList.h"
#include "CharacterCreationInfoParser.h"

const DWORD kAnimate_Tick = 300;

namespace
{
    const DWORD kDrawWidth = 18;
    const DWORD kDrawHeight = 18;

    const DWORD kClassNameFont = StrToWzID("mi17");
    const DWORD kClassNameColor = WzColor_RGBA(199, 179, 137, 255);
    const DWORD kClassDescriptionFont = StrToWzID("st10");
    const DWORD kClassDescriptionColor = WzColor_RGBA(255, 255, 255, 255);
}

uiCreateClassInfo::uiCreateClassInfo(InterfaceManager* ui_manager):
uiBase(ui_manager)
{

}

uiCreateClassInfo::~uiCreateClassInfo()
{

}

void uiCreateClassInfo::Init( CDrawBase* draw_base )
{
    uiBase::Init(draw_base);

    is_animate_ = false;
    is_open_ani_ = false;
    start_tick_ = 0;
    base_width_ = 0;
    RegisterControl();
}

void uiCreateClassInfo::Release()
{

}

void uiCreateClassInfo::MessageProc( SI_MESSAGE * message )
{
    if (IsLock())
        return;

    static bool is_double_click = false;
    switch (message->eResultMsg)
    {
        // 스크롤 처리만 필요
    case RT_MSG_SB_PAGETOP://. 윗쪽 몸통을 눌렀다.
    case RT_MSG_SB_LINETOP://.윗쪽 화살표를 눌렀다.
        {
            OnUpScrollButton();
        }
        break;
    case RT_MSG_SB_PAGEBOTTOM://. 아래쪽 몸통을 눌렀다.
    case RT_MSG_SB_LINEBOTTOM://.아랫쪽 화살표를 눌렀다
        {
            OnDownScrollButton();
        }
        break;
    case RT_MSG_SB_THUMBTRACK://. 썸을 드래그 하고 있다.
    case RT_MSG_SB_THUMBPOSITION://. 썸을 드래그한후 마우스를 놓았다.
    case RT_MSG_SB_ENDSCROLL://. 스크롤이 종료 되었다.
        {
            OnThumbScroll();
        }
        break;
    default:
        SolarDialog::MessageProc(message);
        break;
    }
}

void uiCreateClassInfo::OnThumbScroll()
{
    WzID control_id = GetControlID_byPosition(kControl_VScroll_V000);
    CCtrlVScrollWZ* scroll_control = static_cast<CCtrlVScrollWZ*>(GetControlWZ(control_id));
    if (scroll_control != NULL)
    {
        int scroll_pos = scroll_control->GetScrollPosWZ();
        class_info_.SetBeginLineIndex(scroll_pos);
    }
}

void uiCreateClassInfo::OnUpScrollButton()
{
    WzID control_id = GetControlID_byPosition(kControl_VScroll_V000);
    CCtrlVScrollWZ* scroll_control = static_cast<CCtrlVScrollWZ*>(GetControlWZ(control_id));
    if (scroll_control != NULL)
    {
        int scroll_pos = scroll_control->GetScrollPosWZ();
        --scroll_pos;
        scroll_pos = max(scroll_pos, 0);
        scroll_control->SetScrollPosWZ(scroll_pos);
        class_info_.SetBeginLineIndex(scroll_pos);
    }
}

void uiCreateClassInfo::OnDownScrollButton()
{
    WzID control_id = GetControlID_byPosition(kControl_VScroll_V000);
    CCtrlVScrollWZ* scroll_control = static_cast<CCtrlVScrollWZ*>(GetControlWZ(control_id));
    if (scroll_control != NULL)
    {
        int scroll_pos;
        int max_scroll;
        scroll_control->GetScrollRangeWZ(&scroll_pos, &max_scroll);
        scroll_pos = scroll_control->GetScrollPosWZ();
        ++scroll_pos;
        scroll_pos = min(scroll_pos, max_scroll);
        scroll_control->SetScrollPosWZ(scroll_pos);
        class_info_.SetBeginLineIndex(scroll_pos);
    }
}

void uiCreateClassInfo::OnShowWindow( BOOL val )
{
    static bool once_load = false;

    if (val)
    {
        if (once_load != true)
        {
            once_load = true;
            if (background_)
            {
                RECT ui_position;
                GetDialogWZ()->GetClientRect(&ui_position);

                RECT position = background_->GetSize();
                basic_position_.left = static_cast<float>(position.left);
                basic_position_.top = static_cast<float>(position.top);
                basic_position_.right = static_cast<float>(position.right);
                basic_position_.bottom = static_cast<float>(position.bottom);

                center_position_ = (basic_position_.right/2);
            }
        }

        DWORD control_id = GetControlID_byPosition(kControl_Static_S000);
        CControlWZ* control = GetControlWZ(control_id);
        if (control != NULL)
        {
            RECT control_rect = control->GetSizeRect();
            class_info_.SetRect(control_rect);
            class_info_.convertLineInfo();
        }

        is_animate_ = true;
        is_open_ani_ = true;
        start_tick_ = clock_function::GetTickCount();
        end_tick_ = start_tick_ + kAnimate_Tick;
    }
    else
    {
        is_animate_ = true;
        is_open_ani_ = false;
    }
}

void uiCreateClassInfo::OnRenderSolarDialog()
{
    if (is_animate_ == false)
    {
        class_info_.Render();
    }
}

void uiCreateClassInfo::OnUpdateSolarDialog()
{
    CDialogWZ* dialog_ptr = GetDialogWZ();
    if (dialog_ptr != NULL)
    {
        RECT enable_rect = dialog_ptr->GetClientWZ()->GetSizeRect();;
        BYTE mouse_wheel_state = GetMouseWheelStatus(&enable_rect);

        if (mouse_wheel_state == MOUSE_WHEEL_UP)
        {
            OnUpScrollButton();
        }
        else if (mouse_wheel_state == MOUSE_WHEEL_DOWN)
        {
            OnDownScrollButton();
        }
    }

    if (is_animate_ == true)
    {
        if (is_open_ani_ == true)
        {
            AnimateOpen();
        }
        else
        {
            AnimateClose();
        }        
    }
}

void uiCreateClassInfo::AddClassInfo(BYTE class_type, DWORD class_name_code)
{
    CREATION_INFO* creation_info = CharacterCreationInfoParser::Instance()->GetCreationInfo(class_type);
    if (creation_info != NULL)
    {
        class_info_.ClearSentence();//초기화

        TCHAR description_string[INTERFACE_STRING_LENGTH] = {'\0', };
        TCHAR temp_string[INTERFACE_STRING_LENGTH] = {'\0', };
        FTextParser parser;
        FTextSentence sentence;
        DWORD offset_texture_y = ((class_type - 1) * ClassMarkInfo::kClassMarkTextureHeight);

        /*g_InterfaceManager.GetInterfaceString(class_name_code, temp_string);
        // 마크 및 클래스명
        Snprintf(description_string, INTERFACE_STRING_LENGTH - 1, 
            "<ICON_CODE=%d,%d,%d,%d,%d,%d,%d> %s", 
            ClassMarkInfo::kClassMarkResourceCode, 
            ClassMarkInfo::kClassMarkTextureX, 
            ClassMarkInfo::kClassMarkTextureY + offset_texture_y, 
            ClassMarkInfo::kClassMarkTextureWidth, 
            ClassMarkInfo::kClassMarkTextureHeight, 
            kDrawWidth, 
            kDrawHeight,
            temp_string);
        parser.SetDefaultFont(kClassNameFont, kClassNameColor);
        parser.Parser(description_string, sentence);
        class_info_.Append(sentence);*/

        // 클래스 설명
        g_InterfaceManager.GetInterfaceString(creation_info->class_description_code, description_string);
        parser.SetDefaultFont(kClassDescriptionFont, kClassDescriptionColor);
        parser.Parser(description_string, sentence);
        class_info_.Append(sentence);

        class_info_.convertLineInfo();

        DWORD control_id = GetControlID_byPosition(kControl_VScroll_V000);
        CCtrlVScrollWZ* scroll_control = static_cast<CCtrlVScrollWZ*>(GetControlWZ(control_id));

        control_id = GetControlID_byPosition(kControl_Picture_P003);
        CCtrlPictureWZ* picture_control = static_cast<CCtrlPictureWZ*>(GetControlWZ(control_id));
        if (scroll_control != NULL)
        {
            int max_scroll = class_info_.GetMaxLine() - class_info_.GetDrawLineInRect();
            if (max_scroll > 1)
            {
                scroll_control->ShowWindowWZ(WZ_SHOW);
                if (picture_control)
                {
                    picture_control->ShowWindowWZ(WZ_SHOW);
                }
            }
            else
            {
                scroll_control->ShowWindowWZ(WZ_HIDE);
                if (picture_control)
                {
                    picture_control->ShowWindowWZ(WZ_HIDE);
                }
            }
            scroll_control->SetScrollRangeWZ(0, max_scroll);
            scroll_control->SetScrollPosWZ(0);
            scroll_control->SetChangeSizeThumb(30);
        }

        class_info_.SetBeginLineIndex(0);
    }
}

void uiCreateClassInfo::AnimateOpen()
{
    if (background_ && image_left_ && image_right_)
    {
        float rate = (clock_function::GetTickCount() - start_tick_) / (kAnimate_Tick * 1.0f);
        base_width_ = static_cast<DWORD>(center_position_ * rate);

        if (base_width_ < center_position_)
        {
            float position_x = static_cast<float>(basic_position_.left + center_position_ - base_width_);
            float position_y = static_cast<float>(basic_position_.top);
            float total_width = static_cast<float>(base_width_ * 2);
            float height = static_cast<float>(basic_position_.bottom);

            background_->MoveWindowWZ(position_x, position_y, total_width, height);

            float image_width = image_left_->GetSizeWidth();
            float image_height = image_left_->GetSizeHeight();
            image_left_->MoveWindowWZ((position_x - image_width), position_y, image_width, image_height);

            image_width = image_right_->GetSizeWidth();
            image_height = image_right_->GetSizeHeight();
            image_right_->MoveWindowWZ((position_x + total_width), position_y, image_width, image_height);
        }
        else
        {
            is_animate_ = false;
            base_width_ = 0;
        }
    }
}

void uiCreateClassInfo::AnimateClose()
{

}

void uiCreateClassInfo::RegisterControl()
{
    RegisterControlPosition(StrToWzID("P000"), kControl_Picture_P000);
    RegisterControlPosition(StrToWzID("P001"), kControl_Picture_P001);
    RegisterControlPosition(StrToWzID("P002"), kControl_Picture_P002);
    RegisterControlPosition(StrToWzID("P003"), kControl_Picture_P003);
    RegisterControlPosition(StrToWzID("S000"), kControl_Static_S000);
    RegisterControlPosition(StrToWzID("V000"), kControl_VScroll_V000);

    DWORD control_id = GetControlID_byPosition(kControl_Static_S000);
    text_area_ = static_cast<CCtrlStaticWZ*>(GetControlWZ(control_id));
    assert(text_area_);
    
    control_id = GetControlID_byPosition(kControl_Picture_P001);
    background_ = static_cast<CCtrlPictureWZ*>(GetControlWZ(control_id));
    assert(background_);

    control_id = GetControlID_byPosition(kControl_Picture_P000);
    image_left_ = static_cast<CCtrlPictureWZ*>(GetControlWZ(control_id));
    assert(image_left_);

    control_id = GetControlID_byPosition(kControl_Picture_P002);
    image_right_ = static_cast<CCtrlPictureWZ*>(GetControlWZ(control_id));
    assert(image_right_);

    GraphicResourceList::Instance()->AddItemInfo(ClassMarkInfo::kClassMarkResourceCode, "Data\\Interface\\Dlg3_6.tga");

    RECT control_rect = text_area_->GetSizeRect();
    class_info_.Init(GetDrawBase(), control_rect);
    class_info_.SetVCenterAlign(true);
    class_info_.ClearSentence();

    /*AddClassInfo(eCHAR_BERSERKER, eST_CHARACTER_CLASS_NAME_BERSERKER);
    AddClassInfo(eCHAR_DRAGON, eST_CHARACTER_CLASS_NAME_DRAGON);
    AddClassInfo(eCHAR_SHADOW, eST_CHARACTER_CLASS_NAME_SHADOW);
    AddClassInfo(eCHAR_VALKYRIE, eST_CHARACTER_CLASS_NAME_VALKYRIE);
    AddClassInfo(eCHAR_ELEMENTALIST, eST_CHARACTER_CLASS_NAME_MAGICIAN);
    class_info_.convertLineInfo();

    control_id = GetControlID_byPosition(kControl_VScroll_V000);
    CCtrlVScrollWZ* scroll_control = static_cast<CCtrlVScrollWZ*>(GetControlWZ(control_id));
    if (scroll_control != NULL)
    {
        int max_scroll = class_info_.GetMaxLine() - class_info_.GetDrawLineInRect();
        scroll_control->SetScrollRangeWZ(0, max_scroll);
        scroll_control->SetScrollPosWZ(0);
        scroll_control->SetChangeSizeThumb(30);
    }

    class_info_.SetBeginLineIndex(0);*/
}

