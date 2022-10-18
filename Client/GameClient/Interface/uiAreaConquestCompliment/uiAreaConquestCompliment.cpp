#include "SunClientPrecompiledHeader.h"
#include "uiAreaConquestMan/uiAreaConquestMan.h"
#include "uiAreaConquestDialog/uiAreaConquestDialog.h"
#include "uiAreaConquestDialog/AreaConquestDefine.h"
#include "uiAreaConquestCompliment.h"
#include "AchievementParser.h"
#include "GraphicResourceList.h"
#include "Mouse.h"
#ifdef _SCJ_LONG_TEXT_REDUCING
  #include "uiToolTipMan/uiToolTipMan.h"
#endif //_SCJ_LONG_TEXT_REDUCING
uiAreaConquestCompliment::uiAreaConquestCompliment(InterfaceManager* ui_manager):
uiBase(ui_manager),
is_mouse_over_(false),
current_info_(NULL)
{
    complimen_timer_.Clear();
#ifdef _SCJ_LONG_TEXT_REDUCING
    ZeroMemory(compliment_text_, sizeof(compliment_text_)); 
#endif //_SCJ_LONG_TEXT_REDUCING
}

uiAreaConquestCompliment::~uiAreaConquestCompliment(void)
{
}

void uiAreaConquestCompliment::Init(CDrawBase* draw_base)
{
    uiBase::Init(draw_base);

    RegisterControl();
}

void uiAreaConquestCompliment::Release()
{
}

void uiAreaConquestCompliment::OnUpdateSolarDialog()
{
    POINT mouse_pos;
    mouse_pos.x = Mouse::Instance()->GetMouseX();
    mouse_pos.y = Mouse::Instance()->GetMouseY();
    if (InterSectRect(mouse_pos))
    {
        is_mouse_over_ = true;
        complimen_timer_.SetTimer(kShow_Compliment_Time);
    }

    if (complimen_timer_.IsExpired())
    {
        if (ac_complimen_list_.empty() == false)
        {
            SetACComplimentData(ac_complimen_list_.front());
            ac_complimen_list_.pop_front();
            GetDialogWZ()->SetTransparency(0.0f);
            complimen_timer_.SetTimer(kShow_Compliment_Time);
        }
        else
        {
            ShowInterface(FALSE);
        }
    }
    else
    {
        GetDialogWZ()->SetTransparency(GetFadeValue());
    }
}

void uiAreaConquestCompliment::OnRenderSolarDialog()
{
#ifdef _SCJ_LONG_TEXT_REDUCING
    POINT mouse_pos;
    mouse_pos.x = Mouse::Instance()->GetMouseX();
    mouse_pos.y = Mouse::Instance()->GetMouseY();
    
    CControlWZ* text_control = GetControlWZ(GetControlID_byPosition(kControl_Text_S000));

    if (text_control != NULL)
    {
        RECT rect = text_control->GetSizeRect();

        if (rect.left <= mouse_pos.x && rect.right >= mouse_pos.x)
        {
            if (rect.top <= mouse_pos.y && rect.bottom >= mouse_pos.y)
            {
                uiToolTipMan* tooltip_manager = GET_DIALOG_MANAGER(uiToolTipMan, UIMAN_TOOLTIP);        
                if (tooltip_manager != NULL)
                {
                    tooltip_manager->RegisterTooltipSimpleText(compliment_text_);
					tooltip_manager->SetTooltipDialogID(GetDialogKey());
                }
            }
        }
    }
#endif //_SCJ_LONG_TEXT_REDUCING
}

void uiAreaConquestCompliment::OnShowWindow(BOOL is_show)
{
    if (is_show)
    {
        g_InterfaceManager.AttachFirstDialog(IM_AREACONQUEST_MANAGER::AREACONQUEST_COMPLIMENT);
    }
    else
    {
        g_InterfaceManager.DeAttachFirstDialog(IM_AREACONQUEST_MANAGER::AREACONQUEST_COMPLIMENT);
    }
}

void uiAreaConquestCompliment::MessageProc(SI_MESSAGE* message)
{
    if (IsLock())
        return;

    if (GetMan() == NULL)
        return;

    switch (message->eResultMsg)
    {
    case RT_MSG_LBUTTONCLICK:
        {
            ShowInterface(FALSE);
            uiAreaConquestDialog* ac_dialog = GET_CAST_DIALOG(uiAreaConquestDialog, IM_AREACONQUEST_MANAGER::AREACONQUEST_DIALOG);
            if ((ac_dialog != NULL) && 
                (current_info_ != NULL))
            {
                // 해당 지역/항목으로 세팅
                ac_dialog->ShowInterface(TRUE);

                ac_dialog->OnSelectAcinfoIndex(current_info_->index_);
                ac_dialog->OnSelectArea(current_info_->location_code_);
                ac_dialog->AllClearACDialog(current_info_->location_code_);

                current_info_ = NULL;
            }
        }
        break;
    default:
        SolarDialog::MessageProc(message);
        break;
    }
}

void uiAreaConquestCompliment::RegisterControl()
{
    RegisterControlPosition(StrToWzID("B000"), kControl_Button_B000);
    RegisterControlPosition(StrToWzID("S000"), kControl_Text_S000);
    RegisterControlPosition(StrToWzID("S001"), kControl_Text_S001);
    RegisterControlPosition(StrToWzID("P000"), kControl_Picture_P000);

#ifdef _SCJ_LONG_TEXT_REDUCING
    CControlWZ* text_control = GetControlWZ(GetControlID_byPosition(kControl_Text_S000));
    if (text_control != NULL)
    {
        text_control->SetTextReducing(true);
    }
#endif //_SCJ_LONG_TEXT_REDUCING
}

void uiAreaConquestCompliment::ShowACCompliment(WORD index_number)
{
    Achievement_ParserInfo* ac_parser_info = 
        AchievementParser::Instance()->GetAchievement_ParserInfo(index_number);
    if (ac_parser_info != NULL)
    {
        ac_complimen_list_.push_back(index_number);
    }

    if ((ac_complimen_list_.empty() == false) &&
        (IsVisible() == FALSE))
    {
        SetACComplimentData(ac_complimen_list_.front());
        ac_complimen_list_.pop_front();
        GetDialogWZ()->SetTransparency(0.0f);        
        complimen_timer_.SetTimer(kShow_Compliment_Time);
        ShowInterface(TRUE);
    }
}

void uiAreaConquestCompliment::DeleteList()
{
    ac_complimen_list_.clear();
    ShowInterface(FALSE);
}

void uiAreaConquestCompliment::SetACComplimentData(WORD index_number)
{
    current_info_ = AchievementParser::Instance()->GetAchievement_ParserInfo(index_number);
    if (current_info_ == NULL)
    {
        return;
    }

    is_mouse_over_ = false;

    TCHAR ac_complimen_text[INTERFACE_STRING_LENGTH] = {0, };


    // 아이콘 출력
    SetControlImage(GetControlID_byPosition(kControl_Button_B000), current_info_->icon_code_, 0, 0, 32, 32);

    // 과제명 출력
    CControlWZ* object_text_control = GetControlWZ(GetControlID_byPosition(kControl_Text_S000));
    if (object_text_control != NULL)
    {
        g_InterfaceManager.GetInterfaceString(current_info_->object_string_code_, 
                                              ac_complimen_text);
        object_text_control->SetTextWZ(ac_complimen_text);
#ifdef _SCJ_LONG_TEXT_REDUCING
        StrCopy(compliment_text_, ac_complimen_text);
#endif //_SCJ_LONG_TEXT_REDUCING
    }

    CControlWZ* reward_text_control = GetControlWZ(GetControlID_byPosition(kControl_Text_S001));
    CControlWZ* reward_image_control = GetControlWZ(GetControlID_byPosition(kControl_Picture_P000));
    // 보상출력
    if (current_info_->rewards_[0].reward_type_ == ACHIEVEMENT_REWARD_ACCOMPLISHMENT)
    {
        // 달성률
        if (reward_text_control != NULL)
        {
            Snprintf(ac_complimen_text, INTERFACE_STRING_LENGTH - 1, "%.1f", 
                     (current_info_->rewards_[0].reward_code_ / 100.0f));
            reward_text_control->SetTextWZ(ac_complimen_text);

            reward_text_control->ShowWindowWZ(WZ_SHOW);
        }
        if (reward_image_control != NULL)
        {
            reward_image_control->ShowWindowWZ(WZ_HIDE);
        }
    }
    else
    {
        // 아이템보상
        if (reward_image_control != NULL)
        {
            reward_image_control->ShowWindowWZ(WZ_SHOW);
        }

        if (reward_text_control != NULL)
        {
            reward_text_control->ShowWindowWZ(WZ_HIDE);
        }
    }
}
