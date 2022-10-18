#include "SunClientPrecompiledHeader.h"

#include "GlobalFunc.h"
#include "uiGuideSystemMan/uiGuideSystemMan.h"
#include "uiGuideTutorialDialog/uiGuideTutorialDialog.h"
#include "uiGuideTutorialOpenDialog/uiGuideTutorialOpenDialog.h"
#include "TotalGuideParser.h"
#include "GuideMessageParser.h"

#include "GraphicResourceList.h"

const int kMaxLineAboutImage = 8;
const int kMaxLineRect = 12;
const TCHAR* kReturnFormatString = "<RETURN=1/><RETURN=1/><RETURN=1/><RETURN=1/><RETURN=1/><RETURN=1/><RETURN=1/><RETURN=1/><RETURN=1/><RETURN=1/>";
//------------------------------------------------------------------------------
void uiGuideTutorialDialog::RegisterControl()
{
    RegisterControlPosition(StrToWzID("B000"), kControl_Button_B000);//축소버튼
    RegisterControlPosition(StrToWzID("BT01"), kControl_Button_BT01);//닫기버튼
    RegisterControlPosition(StrToWzID("B003"), kControl_Button_B003);//전
    RegisterControlPosition(StrToWzID("B004"), kControl_Button_B004);//후
    RegisterControlPosition(StrToWzID("S000"), kControl_Text_S000);//제목
    RegisterControlPosition(StrToWzID("S001"), kControl_Text_S001);//내용
    RegisterControlPosition(StrToWzID("S002"), kControl_Text_S002);//페이지
    RegisterControlPosition(StrToWzID("P000"), kControl_Picture_P000);//그림영역
    RegisterControlPosition(StrToWzID("P001"), kControl_Picture_P001);//스크롤바배경
    RegisterControlPosition(StrToWzID("P002"), kControl_Picture_P002);//전체창 배경
    RegisterControlPosition(StrToWzID("VS00"), kControl_Scroll_VS00);//스크롤바
}

uiGuideTutorialDialog::uiGuideTutorialDialog(InterfaceManager* ui_manager) : uiBase(ui_manager)
{
}

uiGuideTutorialDialog::~uiGuideTutorialDialog(void)
{
}

void uiGuideTutorialDialog::Init(CDrawBase* drawbase)
{
    RegisterControl();

    guide_page_ = NULL;
    page_infomation_ = NULL;
    current_line_ = 0;

    ui_guide_system_manager_ = static_cast<uiGuideSystemMan*>(GetMan());
    assert(ui_guide_system_manager_);

    ftext_parser_.SetDefaultFont(StrToWzID("mn12"), 0xFFFFFFFF);  //ARGB

    CControlWZ* control_ptr = GetControlWZ_byPosition(kControl_Text_S001);
    if (control_ptr)
    {
        text_rect_ = control_ptr->GetSizeRect();
        view_rect_ = text_rect_;
    }
    ftext_render_.Init(GetDrawBase(), view_rect_);

    control_ptr = GetControlWZ_byPosition(kControl_Picture_P001);
    if (control_ptr)
    {
        image_rect_ = control_ptr->GetSizeRect();
        view_rect_.top = image_rect_.top;

        line_height_ = static_cast<float>(image_rect_.bottom - image_rect_.top);
        line_height_ = (line_height_ / kMaxLineAboutImage);
    }

    ftext_render_.SetRect(view_rect_);
    image_texture_ = INVALID_HANDLE_VALUE;
    last_page_ = 0;
}

void uiGuideTutorialDialog::Release()
{
    ftext_render_.ClearSentence();
    guide_page_ = NULL;
    page_infomation_ = NULL;

    if (image_texture_ != INVALID_HANDLE_VALUE)
    {
        g_pSunRenderer->x_pManagerTexture->DestroyTexture(image_texture_);
        image_texture_ = INVALID_HANDLE_VALUE;
    }
}

void uiGuideTutorialDialog::OnMouseWheelScroll(BYTE mouse_wheel_state)
{
    CControlWZ* control_ptr = GetControlWZ_byPosition(kControl_Scroll_VS00);
    if (control_ptr)
    {
        if (control_ptr->GetState() == WZ_HIDE)
        {
            return;
        }
    }
    else
    {
        return;
    }

    if (mouse_wheel_state == MOUSE_WHEEL_UP)
    {
        OnUpScroll(NULL);
    }
    else if (mouse_wheel_state == MOUSE_WHEEL_DOWN)
    {
        OnDownScroll(NULL);
    }
}

void uiGuideTutorialDialog::OnUpdateSolarDialog()
{
    CDialogWZ* dialog_ptr = GetDialogWZ();
    if (dialog_ptr != NULL)
    {
        RECT enable_rect = dialog_ptr->GetClientWZ()->GetSizeRect();
        // 마우스휠 처리
        BYTE mouse_wheel_state = GetMouseWheelStatus(&enable_rect);
        OnMouseWheelScroll(mouse_wheel_state);
    }
}

HANDLE uiGuideTutorialDialog::LoadTextureImage(DWORD code)
{
    if (page_infomation_->image_code != 0)
    {
        if (image_texture_ != INVALID_HANDLE_VALUE)
        {
            g_pSunRenderer->x_pManagerTexture->DestroyTexture(image_texture_);
            image_texture_ = INVALID_HANDLE_VALUE;
        }
        const TotalGuideImage* image_name = TotalGuideParser::Instance()->GetImageManager()->GetGuideImage(code);
        if (image_name)
        {
            image_texture_ = g_pSunRenderer->x_pManagerTexture->LoadTexture(image_name->name_str, 
                TLS_ORIGINAL_SIZE | TLS_NO_MIPMAP | TLS_NO_MULTITHREAD);
        }
        else
        {
            TCHAR string[INTERFACE_STRING_LENGTH] = {0,};
            sprintf(string, "LibraryImageList에 경로가 없습니다. (code %d)", code);
            assert(!string);
        }
        return image_texture_;
    }
    else
    {
        return INVALID_HANDLE_VALUE;
    }
}

void uiGuideTutorialDialog::OnRenderSolarDialog()
{
    FRECT margin = {0.f,}; // 각 위치별 여백

    UpdateImageRect();
    ftext_render_.Render();

    int index = 0;
    if (image_texture_ != INVALID_HANDLE_VALUE)
    {
        float image_width = static_cast<float>(image_rect_.right - image_rect_.left);
        float image_height = static_cast<float>(image_rect_.bottom - image_rect_.top);
        float additional_height = (current_line_ * line_height_);
        float image_line_height = (static_cast<float>(image_height_) / kMaxLineAboutImage) * current_line_;

        FRECT image_rect = {0.f,};
#ifdef _NA_004523_20120323_CHANGE_TO_LOAD_GUIDE_IMAGE
        const TotalGuideImage* image = TotalGuideParser::Instance()->GetImageManager()->GetGuideImage(page_infomation_->image_code);
        if (image)
        {
            image_rect.left = static_cast<float>(image->image_rect.left);
            image_rect.top = static_cast<float>(image->image_rect.top);
            image_rect.right = static_cast<float>(image->image_rect.right);
            image_rect.bottom = static_cast<float>(image->image_rect.bottom);// - image_line_height);

            float virtual_image_width = image_width; 
            image_width = image_rect.right - image_rect.left;
            float virtual_image_height = image_height; 
            image_height = image_rect.bottom - image_rect.top;

            if (image_width < virtual_image_width)
            {
                float margin_width = virtual_image_width - image_width;
                margin.left = margin_width/2;
                margin.right = margin_width/2;
            }
            else
            {
                image_width = virtual_image_width;
            }
            if (image_height < virtual_image_height)
            {
                float margin_height = virtual_image_height - image_height;
                margin.top = margin_height/2;
                margin.bottom = margin_height/2;
            }
            else
            {
                image_height = virtual_image_height;
            }

            if (additional_height > 0)
            {
                float temp_height = 0.f;
                temp_height = margin.top - additional_height;

                if (temp_height < 0)
                {
                    additional_height = additional_height - margin.top;
                    margin.top = 0;
                }
                else
                {
                    additional_height = 0;
                    margin.top = temp_height;
                }
            }
        }
#else
        image_rect.left = 0.f;
        image_rect.top = static_cast<float>(image_line_height);
        image_rect.right = static_cast<float>(image_width_);
        image_rect.bottom = static_cast<float>(image_height_ - image_line_height);
#endif//_NA_004523_20120323_CHANGE_TO_LOAD_GUIDE_IMAGE
        if (image_rect.bottom <= 0.0f)
        {
            image_rect.bottom = 0.0f;
        }
        //page_infomation_->image_code
#ifdef _NA_004523_20120323_CHANGE_TO_LOAD_GUIDE_IMAGE
        g_pSunRenderer->RenderTexture(image_texture_,
            (float)image_rect_.left + margin.left,
            (float)image_rect_.top + margin.top,
            (float)image_width,
            (float)image_height - additional_height,
            (float)image_rect.left,
            (float)image_rect.top + ((additional_height>0)?image_line_height:0),
            (float)image_rect.right - image_rect.left,
            (float)image_rect.bottom - image_rect.top - ((additional_height>0)?image_line_height:0));
#else
        g_pSunRenderer->RenderTexture(image_texture_,
            (float)image_rect_.left,
            (float)image_rect_.top,
            (float)image_width,
            (float)image_height - additional_height,
            (float)0,
            (float)image_line_height,
            (float)image_width_,
            (float)image_height_ - image_line_height);
#endif//_NA_004523_20120323_CHANGE_TO_LOAD_GUIDE_IMAGE
    }
}

void uiGuideTutorialDialog::SetPageButton(BYTE page)
{
    CControlWZ* control_ptr = NULL;

    if (guide_page_ && page == guide_page_->page_size)
    {
        control_ptr = GetControlWZ_byPosition(kControl_Button_B004);
        if (control_ptr)
        {
            control_ptr->ShowWindowWZ(WZ_HIDE);
        }
        control_ptr = GetControlWZ_byPosition(kControl_Button_BT01);
        if (control_ptr)
        {
            control_ptr->ShowWindowWZ(WZ_SHOW);
        }
    }
    else
    {
        control_ptr = GetControlWZ_byPosition(kControl_Button_B004);
        if (control_ptr)
        {
            control_ptr->ShowWindowWZ(WZ_SHOW);
        }
        control_ptr = GetControlWZ_byPosition(kControl_Button_BT01);
        if (control_ptr)
        {
            control_ptr->ShowWindowWZ(WZ_HIDE);
        }
    }

    control_ptr = GetControlWZ_byPosition(kControl_Button_B003);
    if (control_ptr)
    {
        if (page == 1)
        {
            control_ptr->ShowWindowWZ(WZ_HIDE); // 1페이지면 앞으로 버튼 숨기기/보이기
        }
        else
        {
            control_ptr->ShowWindowWZ(WZ_SHOW);
        }
    }
}

void uiGuideTutorialDialog::SetData(TotalGuideNotice::Index code, BYTE page)
{
    CControlWZ* control_ptr = NULL;
    TCHAR temp_string[INTERFACE_STRING_LENGTH] = {0,};
    TCHAR* string_ptr = NULL;
    CODETYPE string_code = 0;
    guide_page_ = TotalGuideParser::Instance()->GetGuide(code);
    current_page_ = page;
    if (guide_page_)
    {
        if (ui_guide_system_manager_)
        {
            ui_guide_system_manager_->DeleteInfoNotice(code);
        }
        ftext_render_.SetBeginLineIndex(0);
        CCtrlHScrollWZ* scroll_control = static_cast<CCtrlHScrollWZ*>(GetControlWZ_byPosition(kControl_Scroll_VS00));
        if (scroll_control)
        {
            scroll_control->SetScrollPosWZ(0);
        }
        current_line_ = 0;

        control_ptr = GetControlWZ_byPosition(kControl_Text_S002);
        if (control_ptr) // 페이지 설정
        {
            Sprintf(temp_string, "%d/%d", page, guide_page_->page_size);
            control_ptr->SetTextWZ(temp_string);
            SetPageButton(page);
        }
        
        page_infomation_ = guide_page_->GetPage(page);
        if (page_infomation_)
        {
            g_InterfaceManager.SetUIBlinkGrid(page_infomation_->highlight_target_id, 
                page_infomation_->highlight_child_target_id, true);

            control_ptr = GetControlWZ_byPosition(kControl_Text_S000);
            if (control_ptr) // 제목 설정
            {
                string_ptr = GuideMessageParser::Instance()->GetGuideMessage(page_infomation_->head_str_code);
                control_ptr->SetTextWZ(string_ptr);
            }

            control_ptr = GetControlWZ_byPosition(kControl_Picture_P000);
            if (control_ptr) // 이미지 설정
            {
                image_texture_ = LoadTextureImage(page_infomation_->image_code);
                St_TextureContainer* texture_infomation = g_pSunRenderer->x_pManagerTexture->GetTextureInfo(image_texture_);

#ifdef _NA_004523_20120323_CHANGE_TO_LOAD_GUIDE_IMAGE
                const TotalGuideImage* image = NULL;

				if ( page_infomation_ )
				{
					image = TotalGuideParser::Instance()->GetImageManager()->GetGuideImage(page_infomation_->image_code);
				}

                if (image && image_texture_)
                {
                    image_width_ = image->image_rect.right - image->image_rect.left;
                    image_height_ = image->image_rect.bottom - image->image_rect.top;
                }
#else
                if (texture_infomation)
                {
                    image_width_ = texture_infomation->m_nWidth;
                    image_height_ = texture_infomation->m_nHeight;
                }
#endif//_NA_004523_20120323_CHANGE_TO_LOAD_GUIDE_IMAGE
                else
                {
                    image_width_ = 0;
                    image_height_ = 0;
                }
            }

            control_ptr = GetControlWZ_byPosition(kControl_Text_S001);
            if (control_ptr) // 내용 설정
            {
                string_ptr = GuideMessageParser::Instance()->GetGuideMessage(page_infomation_->content_str_code);
                //control_ptr->SetTextWZ(string);
                ftext_render_.ClearSentence();

                if (page_infomation_->image_code != 0)
                {
                    //8135	                          <RETURN=1/><RETURN=1/><RETURN=1/><RETURN=1/><RETURN=1/><RETURN=1/><RETURN=1/><RETURN=1/><RETURN=1/><RETURN=1/>
                    //const TCHAR* kReturnFormatString = "<RETURN=1/><RETURN=1/><RETURN=1/><RETURN=1/><RETURN=1/><RETURN=1/><RETURN=1/><RETURN=1/><RETURN=1/><RETURN=1/>";
                    TCHAR msg_return[INTERFACE_STRING_LENGTH] = {NULL, };
                    if (g_InterfaceManager.GetInterfaceStringWithCheck(8135, msg_return, INTERFACE_STRING_LENGTH) == true)
                    {
                        Sprintf(temp_string, "%s%s", msg_return, string_ptr);
                    }
                    else
                    {
                        Sprintf(temp_string, "%s%s", kReturnFormatString, string_ptr);
                    }
                    string_ptr = temp_string;
                }

                if (ftext_parser_.Parser(string_ptr, ftext_sentence_))
                {
                    ftext_render_.Append(ftext_sentence_);
                    ftext_sentence_.Release();

                    ftext_param_.eType = FORMAT_TEXT_PARAM_RETURN;
                    ftext_param_.dwValue = 1; // 1칸 띄워라.
#ifdef _INTERNATIONAL_UI
                    ftext_param_.wzFont = StrToWzID("st10");
#else
                    ftext_param_.wzFont = StrToWzID("mn12");
#endif//_INTERNATIONAL_UI
                    ftext_sentence_.Pushback(ftext_param_);

                    ftext_render_.Append(ftext_sentence_);
                    ftext_render_.convertLineInfo();

                    ENUM_STATEWZ scrollbar_state = WZ_HIDE;
                    if (ftext_render_.GetMaxLine() > kMaxLineRect)
                    {
                        scrollbar_state = WZ_SHOW;
                        if (scroll_control)
                        {
                            scroll_control->SetScrollRangeWZ(0, ftext_render_.GetMaxLine() - ftext_render_.GetDrawLineInRect());
                        }
                    }
                    CControlWZ* control_ptr = GetControlWZ_byPosition(kControl_Scroll_VS00);
                    if (control_ptr)
                    {
                        control_ptr->ShowWindowWZ(scrollbar_state);
                    }
                    control_ptr = GetControlWZ_byPosition(kControl_Picture_P000);
                    if (control_ptr)
                    {
                        control_ptr->ShowWindowWZ(scrollbar_state);
                    }
                }
            }
        }
    }
}

void uiGuideTutorialDialog::UpdateImageRect()
{
    CControlWZ* control_ptr = GetControlWZ_byPosition(kControl_Text_S001);
    if (control_ptr)
    {
        text_rect_ = control_ptr->GetSizeRect();
        view_rect_ = text_rect_;
    }

    control_ptr = GetControlWZ_byPosition(kControl_Picture_P001);
    if (control_ptr)
    {
        image_rect_ = control_ptr->GetSizeRect();
    }
    view_rect_.top = image_rect_.top;
    ftext_render_.SetRect(view_rect_);
}

void uiGuideTutorialDialog::OnShowWindow(BOOL val)
{  
    if (val)
    {
        keyMsg msg;
        ZeroMemory(&msg, sizeof(msg));
        msg.dwType = eDoType_GuideTutorial_Dialog;
        msg.wParam = IM_GUIDE_SYSTEM_MANAGER::TUTORIAL;
        msg.DoSomething = GlobalFunc::DoShowWindowEx;
        g_KeyQueueManager.PushBack(msg);

        if (page_infomation_)
        {
            g_InterfaceManager.SetUIBlinkGrid(page_infomation_->highlight_target_id, 
                page_infomation_->highlight_child_target_id, true);
        }
        SetPageButton(current_page_);
    }
    else
    {
        if (page_infomation_)
        {
            g_InterfaceManager.SetUIBlinkGrid(page_infomation_->highlight_target_id, 
                page_infomation_->highlight_child_target_id, false);
        }
        g_KeyQueueManager.DeleteMsg(eDoType_GuideTutorial_Dialog);
    }
}
//------------------------------------------------------------------------------
void uiGuideTutorialDialog::MessageProc(SI_MESSAGE* message)
{
    if (IsLock())
        return;

    switch (message->eResultMsg)
    {
    case RT_MSG_LBUTTONCLICK:
        OnLButtonClick(message);
        break;
    case RT_MSG_SB_LINETOP:
    case RT_MSG_SB_PAGETOP:
        {
            OnUpScroll(message);
        }
        break;

    case RT_MSG_SB_LINEBOTTOM:
    case RT_MSG_SB_PAGEBOTTOM:
        {
            OnDownScroll(message);
        }
        break;
    case RT_MSG_SB_THUMBTRACK:				//. 썸을 드래그 하고 있다.
    case RT_MSG_SB_ENDSCROLL:				//. 스크롤이 종료 되었다.
    case RT_MSG_SB_THUMBPOSITION:
        {
            //.썸을 드래그 하고 있다.
            OnThumbScroll(message);
        }
        break;
    default:
        SolarDialog::MessageProc(message);
        break;
    }
}


bool uiGuideTutorialDialog::OnUpScroll(SI_MESSAGE* message)
{
    bool result = true;

    WzID scroll_control_id = GetControlID_byPosition(kControl_Scroll_VS00);
    CCtrlVScrollWZ* scroll_control = 
        static_cast<CCtrlVScrollWZ*>(GetControlWZ(scroll_control_id));
    if (scroll_control != NULL)
    {
        int scroll_min = 0;
        int scroll_max = 0;
        int current_scroll_pos = scroll_control->GetScrollPosWZ();
        scroll_control->GetScrollRangeWZ(&scroll_min, &scroll_max);
        --current_scroll_pos;
        current_scroll_pos = max(0, current_scroll_pos);
        scroll_control->SetScrollPosWZ(current_scroll_pos);

        current_line_ = current_scroll_pos;
        ftext_render_.SetBeginLineIndex(current_scroll_pos);
        scroll_control->SetScrollPosWZ(current_scroll_pos);
    }

    return result;
}

bool uiGuideTutorialDialog::OnDownScroll(SI_MESSAGE* message)
{
    bool result = true;

    WzID scroll_control_id = GetControlID_byPosition(kControl_Scroll_VS00);
    CCtrlVScrollWZ* scroll_control = 
        static_cast<CCtrlVScrollWZ*>(GetControlWZ(scroll_control_id));
    if (scroll_control != NULL)
    {
        int scroll_min = 0;
        int scroll_max = 0;
        int current_scroll_pos = scroll_control->GetScrollPosWZ();
        scroll_control->GetScrollRangeWZ(&scroll_min, &scroll_max);
        ++current_scroll_pos;
        current_scroll_pos = min(scroll_max, current_scroll_pos);
        scroll_control->SetScrollPosWZ(current_scroll_pos);

        current_line_ = current_scroll_pos;
        ftext_render_.SetBeginLineIndex(current_scroll_pos);
        scroll_control->SetScrollPosWZ(current_scroll_pos);
    }

    return result;
}

bool uiGuideTutorialDialog::OnThumbScroll(SI_MESSAGE* message)
{
    bool result = true;

    WzID scroll_control_id = GetControlID_byPosition(kControl_Scroll_VS00);
    CCtrlVScrollWZ* scroll_control = 
        static_cast<CCtrlVScrollWZ*>(GetControlWZ(scroll_control_id));
    if (scroll_control != NULL)
    {
        int scroll_min = 0;
        int scroll_max = 0;
        int current_scroll_pos = scroll_control->GetScrollPosWZ();
        scroll_control->GetScrollRangeWZ(&scroll_min, &scroll_max);
        current_scroll_pos = max(0, current_scroll_pos);
        current_scroll_pos = min(scroll_max, current_scroll_pos);

        current_line_ = current_scroll_pos;
        ftext_render_.SetBeginLineIndex(current_scroll_pos);
        scroll_control->SetScrollPosWZ(current_scroll_pos);
    }
    return result;
}
//------------------------------------------------------------------------------
bool uiGuideTutorialDialog::OnLButtonClick(SI_MESSAGE* message)
{
    bool result = true;
    switch (GetControlPosition_byID(message->dwCtrlID))
    {
    case kControl_Button_B000://축소
        {
            uiGuideTutorialOpenDialog* tutorial_open_dialog =
                GET_CAST_DIALOG(uiGuideTutorialOpenDialog, IM_GUIDE_SYSTEM_MANAGER::TUTORIAL_OPEN);
            if (tutorial_open_dialog)
            {
                tutorial_open_dialog->ShowInterface(TRUE);
            }
            ShowInterface(FALSE);
        }
        break;
    case kControl_Button_BT01://닫기
        {
            ShowInterface(FALSE);
        }
        break;
    case kControl_Button_B003://이전
        {
            if (page_infomation_ && guide_page_ && 1 < current_page_)
            {
                g_InterfaceManager.SetUIBlinkGrid(page_infomation_->highlight_target_id, 
                    page_infomation_->highlight_child_target_id, false);

                SetData(guide_page_->code, (current_page_ - 1));
            }
        }
        break;
    case kControl_Button_B004://다음
        {
            if (guide_page_->page_size >= current_page_ && page_infomation_)
            {
                bool is_unhold = false;
                // 홀드 페이지 체크
                if (page_infomation_ && page_infomation_->hold_window_id != 0)
                {
                    SolarDialog *dialog = GET_DIALOG(page_infomation_->hold_window_id);

                    if (dialog && dialog->IsVisible())
                    {
                        is_unhold = true;
                    }
                }
                else
                {
                    is_unhold = true;
                }

                if (is_unhold)
                {
                    g_InterfaceManager.SetUIBlinkGrid(page_infomation_->highlight_target_id, 
                        page_infomation_->highlight_child_target_id, false);
                    SetData(guide_page_->code, (current_page_ + 1));
                }
                else
                {
                    TCHAR* string_ptr = NULL;
                    string_ptr = GuideMessageParser::Instance()->GetGuideMessage(page_infomation_->hold_str_code);
                    if (string_ptr)
                    {
                        GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_TOP_SHOW, string_ptr);
                    }
                }
            }
            
            if (guide_page_->page_size == current_page_)
            {
                CControlWZ* control_ptr = GetControlWZ_byPosition(kControl_Button_BT01);
                if (control_ptr)
                {
                    control_ptr->ShowWindowWZ(WZ_SHOW);
                }
            }
        }
        break;
    default:
        {
        }
    }
    return result;
}
