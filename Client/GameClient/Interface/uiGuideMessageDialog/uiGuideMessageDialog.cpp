#include "SunClientPrecompiledHeader.h"

#include "GlobalFunc.h"
#include "uiGuideSystemMan/uiGuideSystemMan.h"
#include "uiGuideMessageDialog/uiGuideMessageDialog.h"
#include "TotalGuideParser.h"
#include "GuideMessageParser.h"

//------------------------------------------------------------------------------
void uiGuideMessageDialog::RegisterControl()
{
    RegisterControlPosition(StrToWzID("BT01"), kControl_Button_BT01);
    RegisterControlPosition(StrToWzID("B000"), kControl_Button_B000);
    RegisterControlPosition(StrToWzID("B001"), kControl_Button_B001);
    RegisterControlPosition(StrToWzID("B002"), kControl_Button_B002);
    RegisterControlPosition(StrToWzID("S000"), kControl_Text_S000);
    RegisterControlPosition(StrToWzID("S001"), kControl_Text_S001);
}

uiGuideMessageDialog::uiGuideMessageDialog(InterfaceManager* ui_manager) : uiBase(ui_manager), is_fade_in_(false)
{
}

uiGuideMessageDialog::~uiGuideMessageDialog(void)
{

}

void uiGuideMessageDialog::Init(CDrawBase* drawbase)
{
    RegisterControl();

    ui_guide_system_manager_ = static_cast<uiGuideSystemMan*>(GetMan());
    assert(ui_guide_system_manager_);

    SetOpenType(TotalGuideNotice::kViewAndNextView);//Test Code
    is_fade_in_ = true;
    open_type_ = TotalGuideNotice::kGuideWindow;

    ftext_parser_.SetDefaultFont(StrToWzID("mn12"), 0xFFFFFFFF);  //ARGB
    CControlWZ* control_ptr = GetControlWZ_byPosition(kControl_Text_S001);
    if (control_ptr)
    {
        ftext_render_.Init(GetDrawBase(), control_ptr->GetSizeRect());
    }
    guide_code_ = 0;
}

void uiGuideMessageDialog::Release()
{
    ftext_render_.ClearSentence();
    guide_code_ = 0;
    guide_message_list_.clear();
}

void uiGuideMessageDialog::OnUpdateSolarDialog()
{
    if (dialog_timer_.GetRemainedTime() <= 0)
    {
        if (is_fade_in_)
        {
            if (guide_message_list_.size() > 0)
            {
                ShowWindowFade(FALSE);// 정보가 더 있다면 서서히 닫는다.
            }
            else
            {
                fade_value_ = GetFadeValue();
                GetDialogWZ()->SetTransparency(fade_value_);
            }
        }
        else
        {
            ShowInterface(FALSE);
            if (guide_message_list_.size() > 0)
            {
                PopData(); // 정보가 더 있다면 빼고 연다.
            }
        }
    }
    else
    {
        if (is_fade_in_)
        {
            fade_value_ = GetFadeValue();
            GetDialogWZ()->SetTransparency(fade_value_);
        }
        else
        {
            fade_value_ = 1.0f - GetFadeValue();
            GetDialogWZ()->SetTransparency(fade_value_);
        }
    }
}

void uiGuideMessageDialog::OnRenderSolarDialog()
{
    CControlWZ* control = GetControlWZ_byPosition(kControl_Text_S001);
    if (control)
    {
        ftext_render_.SetAlphaModulate(TRUE);
        BYTE byte_of_fade = static_cast<BYTE>(fade_value_ * 255);
        ftext_render_.SetAlphaValue(byte_of_fade);

        ftext_render_.SetRect(control->GetSizeRect());
        ftext_render_.Render();
    }
}

void uiGuideMessageDialog::ShowWindowFade(BOOL val)
{
    if (val)
    {
        dialog_timer_.SetTimer(kFade_InOut_Time + kShow_Compliment_Time);
        is_fade_in_ = true;
        keyMsg msg;
        ZeroMemory(&msg, sizeof(msg));
        msg.dwType = eDoType_GuideMessage_Dialog;
        msg.wParam = IM_GUIDE_SYSTEM_MANAGER::HELP_ALERT;
        msg.lParam = FALSE;
        msg.DoSomething = uiGuideMessageDialog::ShowFade;
        g_KeyQueueManager.PushBack(msg);
    }
    else
    {
        dialog_timer_.SetTimer(kFade_InOut_Time);
        is_fade_in_ = false;
        g_KeyQueueManager.DeleteMsg(eDoType_GuideMessage_Dialog);
    }
}

void uiGuideMessageDialog::ShowFade(DWORD wParam,DWORD lParam) // 콜백함수용
{
    uiGuideMessageDialog* dialog = GET_CAST_DIALOG(uiGuideMessageDialog, IM_GUIDE_SYSTEM_MANAGER::HELP_ALERT);
    if (dialog == NULL)
    {
        return;
    }

    if (lParam)
    {
        dialog->ShowWindowFade(TRUE);
    }
    else
    {
        dialog->ShowWindowFade(FALSE);
    }
}

void uiGuideMessageDialog::OnShowWindow(BOOL val)
{
    fade_value_ = 0.0f;
    GetDialogWZ()->SetTransparency(0.0f);

    if (val)
    {
        is_fade_in_ = true;
        ShowWindowFade(TRUE);
    }
    else
    {
        is_fade_in_ = false;
        if (button_type_ == TotalGuideNotice::kView &&
            open_type_ == TotalGuideNotice::kMiniWindow)
        {
            ui_guide_system_manager_->OpenGuideDialog(open_type_, guide_code_);// 위 조건일때 닫힌다면 미니정보창을 연다.
        }
    }
}

void uiGuideMessageDialog::BlinkButton()
{
    //dialog_timer_.IsExpiredManual(TRUE);
}
//------------------------------------------------------------------------------
void uiGuideMessageDialog::MessageProc(SI_MESSAGE* message)
{
    if (IsLock())
        return;

    switch (message->eResultMsg)
    {
    case RT_MSG_LBUTTONCLICK:
        OnLButtonClick(message);
        break;
    default:
        SolarDialog::MessageProc(message);
        break;
    }
}


//------------------------------------------------------------------------------
bool uiGuideMessageDialog::OnLButtonClick(SI_MESSAGE* message)
{
    bool result = true;
    DWORD control_id = message->dwCtrlID;
    DWORD control_postion = GetControlPosition_byID(control_id);

    switch (control_postion)
    {
    case kControl_Button_BT01: // 정보보기
    case kControl_Button_B001: // 정보보기(단일)
        {   
            if (dialog_timer_.GetRemainedTime() <= 0 && fade_value_ >= 1.0f)
            {
                ShowWindowFade(FALSE);
                ui_guide_system_manager_->OpenGuideDialog(open_type_, guide_code_);
            }
        }
        break;
    case kControl_Button_B000: // 다음에 보기
    case kControl_Button_B002: // 확인
        {
            if (dialog_timer_.GetRemainedTime() <= 0 && fade_value_ >= 1.0f)
            {
                ShowWindowFade(FALSE);
            }
        }
        break;
    default:
        {
        }
        break;
    }
    return result;
}

void uiGuideMessageDialog::AddData(WORD message_code)
{
    const TotalGuideNotice* data = TotalGuideParser::Instance()->GetNotice(message_code);
    if (data)
    {
        guide_message_list_.push_back(message_code);
    }

    if (is_fade_in_ == false) // 창이 닫힌 상태에서
    {
        if (guide_message_list_.size() == 1) // 데이터가 한개라면 바로 실행
        {
            PopData();
        }
    }
}

void uiGuideMessageDialog::PopData()
{
    if (guide_message_list_.size() > 0)
    {
        SetData(guide_message_list_.front());
        guide_message_list_.pop_front();

        if(GAMEOPTION->GetShowHelp() == TRUE)
        {
            ShowDialog(TRUE);
        }
    }
}

void uiGuideMessageDialog::SetData(WORD message_code)
{
    const TotalGuideNotice* data = TotalGuideParser::Instance()->GetNotice(message_code); // 삽입시 null체크 완료함

    TCHAR* string_ptr = NULL;
    CODETYPE string_code = 0;
    CControlWZ* control_ptr;

    string_code = data->title_str_code;
    control_ptr = GetControlWZ_byPosition(kControl_Text_S000);
    if (control_ptr)
    {
        string_ptr = GuideMessageParser::Instance()->GetGuideMessage(string_code);
        control_ptr->SetTextWZ(string_ptr);
    }

    string_code = data->desc_str_code;
    control_ptr = GetControlWZ_byPosition(kControl_Text_S001);
    if (control_ptr)
    {
        string_ptr = GuideMessageParser::Instance()->GetGuideMessage(string_code);
        ftext_render_.ClearSentence();
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
        }
    }

    open_type_ = data->open_type;
    guide_code_ = data->guide_code;

    if (ui_guide_system_manager_)
    {
        ui_guide_system_manager_->AddInfoNotice(guide_code_);
    }
    SetOpenType(data->button_style);
}

void uiGuideMessageDialog::SetOpenType(BYTE button_type)
{
    button_type_ = button_type;
    CControlWZ* control_ptr;
    // hide all buttons : 모든 버튼 숨기기
    for (int i = kControl_Button_BT01; i <= kControl_Button_B002; ++i)
    {
        control_ptr = GetControlWZ_byPosition(i);
        if (control_ptr)
        {
            control_ptr->ShowWindowWZ(WZ_HIDE);
        }
    }

    // show button(s) from type of button : 버튼 타입에 맞게 버튼 보기
    switch (button_type)
    {
    case TotalGuideNotice::kViewAndNextView:
        {
            control_ptr = GetControlWZ_byPosition(kControl_Button_BT01);
            if (control_ptr)
            {
                control_ptr->ShowWindowWZ(WZ_SHOW);
            }
            control_ptr = GetControlWZ_byPosition(kControl_Button_B000);
            if (control_ptr)
            {
                control_ptr->ShowWindowWZ(WZ_SHOW);
            }
        }
        break;
    case TotalGuideNotice::kOK:
        {
            control_ptr = GetControlWZ_byPosition(kControl_Button_B002);
            if (control_ptr)
            {
                control_ptr->ShowWindowWZ(WZ_SHOW);
            }
        }
        break;
    case TotalGuideNotice::kView:
        {
            control_ptr = GetControlWZ_byPosition(kControl_Button_B001);
            if (control_ptr)
            {
                control_ptr->ShowWindowWZ(WZ_SHOW);
                g_KeyQueueManager.DeleteMsg(eDoType_GuideMessage_Dialog);//정보보기Only 인 경우 ESC로 닫을 수 없음
            }
        }
        break;
    }
}
