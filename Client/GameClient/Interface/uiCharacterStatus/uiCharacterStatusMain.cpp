#include "SunClientPrecompiledHeader.h"
#ifdef _NA_003027_20111013_HONOR_SYSTEM
#include "uiCharacterStatusMain.h"
#include "uiCharacterStatusTitleList.h"
#include "uiCharacterStatusMan/uiCharacterStatusMan.h"
#include "HonorSystem/HonorSystem.h"
#include "uiToolTipMan/uiToolTipMan.h"
#ifdef _NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
    #include "GuildSystem/GuildSystemData.h"
#else
    #include "uiGuildMan/uiGuildMan.h"
#endif //_NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
#include "GlobalFunc.h"
#include "Hero.h"
#include "TitleListParser.h"
#include "GuildMarkInfoParser.h"

const DWORD uiCharacterStatusMain::kDialog_ID = StrToWzID("0401");

uiCharacterStatusMain::uiCharacterStatusMain(InterfaceManager* interface_manager)
:uiBase(interface_manager)
{

}

uiCharacterStatusMain::~uiCharacterStatusMain()
{

}

void uiCharacterStatusMain::Init(CDrawBase* draw_base)
{
    uiBase::Init(draw_base);
    GetDialogWZ()->SetCallbackTooltip(uiToolTipMan::Callback_Tooltip);

    RegisterControlPosition(StrToWzID("T999"), kControl_Text_T999);
    RegisterControlPosition(StrToWzID("TI03"), kControl_TextImage_TI03);
    RegisterControlPosition(StrToWzID("TI02"), kControl_TextImage_TI02);
    RegisterControlPosition(StrToWzID("TI01"), kControl_TextImage_TI01);
    RegisterControlPosition(StrToWzID("TI00"), kControl_TextImage_TI00);
    RegisterControlPosition(StrToWzID("PI00"), kControl_Picture_PI00);
    RegisterControlPosition(StrToWzID("B000"), kControl_Button_B000);
    RegisterControlPosition(StrToWzID("TI34"), kControl_TextImage_TI34);
    RegisterControlPosition(StrToWzID("TI35"), kControl_TextImage_TI35);
    RegisterControlPosition(StrToWzID("BT01"), kControl_Button_BT01);
    RegisterControlPosition(StrToWzID("B999"), kControl_Button_B999);
    
    CControlWZ* picture_control = GetControlWZ(StrToWzID("P005"));
    if (picture_control != NULL)
    {
        picture_control->SetDiscardMessage(true);
    }

    GetDialogWZ()->ADDType(DLG_PMOVE);
}

void uiCharacterStatusMain::Release()
{

}

void uiCharacterStatusMain::OnShowWindow( BOOL is_show )
{
    uiCharacterStatusMan* ui_manager = static_cast<uiCharacterStatusMan*>(GetMan());

    if (is_show == TRUE)
    {
        keyMsg key_message;
        ZeroMemory(&key_message, sizeof(key_message));
        key_message.dwType = eDoType_dialog_charSkill;
        key_message.wParam = uiCharacterStatusMain::kDialog_ID;
        key_message.DoSomething = GlobalFunc::DoShowWindowEx;
        g_KeyQueueManager.PushBack(key_message);

        if (ui_manager != NULL)
        {
            TabCheckButtonControllManager& dialog_tab_manager = ui_manager->status_dialog_tab_manager();
#ifdef _NA_008472_20150903_IMPROVED_BALANCE
            // 2016.01.12 [SUNJP-1802] 스탯ui 조작 중에 임시 분배된 스탯 초기화 관련
            // 초기화된 첫 창을 열겠다는 의미인데 prev_tab_index는 초기화는 안되서 추가함.
            // ShowDialog() 함수 안에서 prev_tab_index = current_tab_index 됨
            dialog_tab_manager.SetCurrentTabIndex(uiCharacterStatusMan::kStatus_Info);
#endif //_NA_008472_20150903_IMPROVED_BALANCE
            dialog_tab_manager.ShowDialog(uiCharacterStatusMan::kStatus_Info, true);
            MoveSubDialog();
        }
    }
    else
    {
        g_KeyQueueManager.DeleteMsg(eDoType_dialog_charSkill);
        if (ui_manager != NULL)
        {
            TabCheckButtonControllManager& dialog_tab_manager = ui_manager->status_dialog_tab_manager();
            dialog_tab_manager.ShowCurrentDialog(false);
        }

        uiBase* title_list = GET_DIALOG(uiCharacterStatusTitleList::kDialog_ID);
        if (title_list != NULL)
        {
            title_list->ShowInterface(FALSE);
        }
    }
}

void uiCharacterStatusMain::MessageProc(SI_MESSAGE* message)
{
    if (IsLock())
    {
        return;
    }

    if (GetMan() == NULL)
    {
        return;
    }

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

void uiCharacterStatusMain::OnUpdateSolarDialog()
{
    MoveSubDialog();   

    uiCharacterStatusMan* ui_manager = static_cast<uiCharacterStatusMan*>(GetMan());

    if (g_pHero != NULL)
    {
        TCHAR info_string[INTERFACE_STRING_LENGTH] = {'\0', };

        // 캐릭터 이름
        CControlWZ* control_ptr = GetControlWZ_byPosition(kControl_Text_T999);
        if (control_ptr != NULL)
        {
            GlobalFunc::SetCaption(static_cast<CCtrlStaticWZ*>(control_ptr), g_pHero->GetName());
        }

        // 캐릭터 레벨
        control_ptr = GetControlWZ_byPosition(kControl_TextImage_TI03);
        if (control_ptr != NULL)
        {
            Snprintf(info_string, INTERFACE_STRING_LENGTH - 1, _T("%d"), g_pHero->GetLevel());
            GlobalFunc::SetCaption(static_cast<CCtrlStaticWZ*>(control_ptr), info_string);
        }

        // 클래스
        control_ptr = GetControlWZ_byPosition(kControl_TextImage_TI02);
        if (control_ptr != NULL)
        {
            g_pHero->GetClassExName(info_string);
            GlobalFunc::SetCaption(static_cast<CCtrlStaticWZ*>(control_ptr), info_string);
        }

        // 길드명
        control_ptr = GetControlWZ_byPosition(kControl_TextImage_TI01);
        if (control_ptr != NULL)
        {
            GlobalFunc::SetCaption(static_cast<CCtrlStaticWZ*>(control_ptr), g_pHero->GetGuildName());
        }

        // 칭호
        control_ptr = GetControlWZ_byPosition(kControl_TextImage_TI00);
        if (control_ptr != NULL)
        {
            info_string[0] = '\0';

             WORD title_code = g_pHero->GetTitleCode();
             TitleListParser::TitleInfo* title_info = TitleListParser::Instance()->GetTitleInfo(title_code);
             if (title_info != NULL)
             {
                 g_InterfaceManager.GetInterfaceString(title_info->title_name_code, info_string);
             }
             else 
             {
                 g_InterfaceManager.GetInterfaceString(HonorSystem::kNoTitleMenuStringCode, info_string);
             }
            GlobalFunc::SetCaption(static_cast<CCtrlStaticWZ*>(control_ptr), info_string);
        }

        // 접속포인트 - 플레이시간 및 코인 출력 hour, minute;
        if (ui_manager != NULL)
        {                
            // 누적된 플레이타임(초)
            control_ptr = GetControlWZ_byPosition(kControl_TextImage_TI34);
            if (control_ptr != NULL)
            {
                DWORD accumulate_time = ui_manager->accumulate_time() + (accumulate_timer_.GetProcessTime() / 1000);
                DWORD accumulate_time_second = accumulate_time % 60;
                accumulate_time /= 60;
                TCHAR day_string[INTERFACE_STRING_LENGTH] = {0, };
                // 46635 : Day
                g_InterfaceManager.GetInterfaceString(46635, day_string);
                Snprintf(info_string, INTERFACE_STRING_LENGTH - 1, _T("%02d:%02d:%02d(%d%s)"), 
                    accumulate_time/60, accumulate_time%60, accumulate_time_second, accumulate_time/(60*24), day_string);
                GlobalFunc::SetCaption(static_cast<CCtrlStaticWZ*>(control_ptr), info_string);
            }

            // 누적된 포인트
            control_ptr = GetControlWZ_byPosition(kControl_TextImage_TI35);
            if (control_ptr != NULL)
            {
                Snprintf(info_string, INTERFACE_STRING_LENGTH - 1, _T("%d"), ui_manager->accumulate_point());
                GlobalFunc::SetCaption(static_cast<CCtrlStaticWZ*>(control_ptr), info_string);
            }
            
            // 보상버튼 활성/비활성
            control_ptr = GetControlWZ_byPosition(kControl_Button_BT01);
            if (control_ptr != NULL)
            {
                CCtrlButtonWZ* reward_button = static_cast<CCtrlButtonWZ*>(control_ptr);
                ENUM_DRAWSTATE draw_state = reward_button->GetDrawState();
                if (ui_manager->accumulate_point() >= ui_manager->require_point())
                {
                    if (draw_state == E_DISABLE)
                    {
                        reward_button->EnableWindowWZ(true);
                    }
                }
                else
                {
                    if (draw_state != E_DISABLE)
                    {
                        reward_button->EnableWindowWZ(false);
                    }
                }
            }
        }
    }

    if (GetDialogWZ()->HaveFocus() == true)
    {
        uiCharacterStatusMan* ui_manager = static_cast<uiCharacterStatusMan*>(GetMan());
        if (ui_manager != NULL)
        {
            TabCheckButtonControllManager& dialog_tab_manager = ui_manager->status_dialog_tab_manager();
            uiBase* sub_dialog = dialog_tab_manager.GetCurrentTabDialog();
            if (sub_dialog != NULL)
            {
                GlobalFunc::MoveToTopDialog(sub_dialog->GetDialogWZ()->GetDlgID());
            }
        }
    }    
}

void uiCharacterStatusMain::OnRenderSolarDialog()
{
    RenderGuildMark();

    uiToolTipMan* tooltip_manager = GET_DIALOG_MANAGER(uiToolTipMan, UIMAN_TOOLTIP);
    if (tooltip_manager != NULL)
    {
        TCHAR tooltip_string[INTERFACE_STRING_LENGTH] = {0, };

        if (IsOverControl(GetControlWZ_byPosition(kControl_Button_BT01)))
        {
            //! 접속포인트 보상버튼 툴팁
            uiCharacterStatusMan* ui_manager = static_cast<uiCharacterStatusMan*>(GetMan());
            if (ui_manager != NULL)
            {
                TCHAR temp_string[INTERFACE_STRING_LENGTH] = {0, };
                g_InterfaceManager.GetInterfaceString(6145, temp_string);
                Snprintf(tooltip_string, INTERFACE_STRING_LENGTH - 1, temp_string, ui_manager->require_point());

                tooltip_manager->InitTooltip();
				tooltip_manager->SetTooltipDialogID(GetDialogKey());
                tooltip_manager->RegisterTooltipLineChange(tooltip_string, _tcslen(tooltip_string));
            }
        }
        if (IsOverControl(GetControlWZ_byPosition(kControl_Button_B000)))
        {
            TCHAR tooltip_string[INTERFACE_STRING_LENGTH] = {0, };
            g_InterfaceManager.GetInterfaceString(6784, tooltip_string);

            tooltip_manager->InitTooltip();
			tooltip_manager->SetTooltipDialogID(GetDialogKey());
            tooltip_manager->RegisterTooltipLineChange(tooltip_string, _tcslen(tooltip_string));
        }
    }
}

void uiCharacterStatusMain::RenderGuildMark()
{
    if (g_pHero == NULL)
    {
        return;
    }

    if (g_pHero->IsExistGuildMark() == false)
    {
        return;
    }

    CCtrlPictureWZ* gulid_mark_control = static_cast<CCtrlPictureWZ*>(GetControlWZ_byPosition(kControl_Picture_PI00));
    if (gulid_mark_control == NULL)
    {
        return;
    }

#ifdef _NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER
    HANDLE guild_mark_texture = GuildSystem::GetGuildSystemData().GetGuildMarkTexture();
#else
    uiGuildMan* guild_manager = GET_DIALOG_MANAGER(uiGuildMan, UIMAN_GUILD);
    if (guild_manager == NULL)
    {
        return ;
    }

    HANDLE guild_mark_texture = guild_manager->GetGuildMarkTexture();
    if (guild_mark_texture == INVALID_HANDLE_VALUE)
    {
        return;
    }
#endif //_NA_003923_20120217_GUILD_RENEWAL_UI_GUILD_MANAGER    

    BASE_GuildMarkInfo* guild_mark_info = NULL;
    ENUM_ALPHABLEND old_alpha_blend_mode = GetDrawBase()->GetAlphaBlendMode();
    g_pSunRenderer->SetAlphaBlendMode(AB_ALPHA);

    RECT control_rect = gulid_mark_control->GetSizeRect();

    const float kMark_Size = 32.0f;
    float draw_width = static_cast<float>(control_rect.right - control_rect.left);
    float draw_height = static_cast<float>(control_rect.bottom - control_rect.top);

    guild_mark_info =  GuildMarkInfoParser::Instance()->GetGuildMarkInfo(g_pHero->GetGuildRenderInfo().m_BackGroundIndex);
    if (guild_mark_info)
    {
        g_pSunRenderer->RenderTexture(guild_mark_texture, 
            static_cast<float>(control_rect.left), 
            static_cast<float>(control_rect.top), 
            draw_width, 
            draw_height,
            guild_mark_info->iRow * kMark_Size, 
            guild_mark_info->iColumn * kMark_Size, 
            kMark_Size, 
            kMark_Size);
    }

    guild_mark_info =  GuildMarkInfoParser::Instance()->GetGuildMarkInfo(g_pHero->GetGuildRenderInfo().m_PatternIndex);
    if (guild_mark_info)
    {
        g_pSunRenderer->RenderTexture(guild_mark_texture, 
            static_cast<float>(control_rect.left), 
            static_cast<float>(control_rect.top), 
            draw_width, 
            draw_height,
            guild_mark_info->iRow * kMark_Size, 
            guild_mark_info->iColumn * kMark_Size, 
            kMark_Size, 
            kMark_Size);
    }

    g_pSunRenderer->SetAlphaBlendMode(old_alpha_blend_mode);
}

void uiCharacterStatusMain::MoveSubDialog()
{
    RECT control_rect;
    GetDialogWZ()->GetClientRect(&control_rect);
    uiBase* sub_dialog = NULL;

    // 서브 탭 다이얼로그
    uiCharacterStatusMan* ui_manager = static_cast<uiCharacterStatusMan*>(GetMan());
    if (ui_manager != NULL)
    {
        TabCheckButtonControllManager& dialog_tab_manager = ui_manager->status_dialog_tab_manager();
        sub_dialog = dialog_tab_manager.GetCurrentTabDialog();
        if (sub_dialog != NULL)
        {
            sub_dialog->MoveWindowWZ(static_cast<float>(control_rect.left), 
                static_cast<float>(control_rect.bottom));
        }        
    }

    // 칭호리스트 다이얼로그
    sub_dialog = GET_DIALOG(uiCharacterStatusTitleList::kDialog_ID);
    if (sub_dialog != NULL)
    {
        sub_dialog->MoveWindowWZ(static_cast<float>(control_rect.right), 
            static_cast<float>(control_rect.top));
    }
}

bool uiCharacterStatusMain::OnLButtonClick(SI_MESSAGE* message)
{
    bool result = true;
    switch (GetControlPosition_byID(message->dwCtrlID))
    {
    case kControl_Button_B000:
        {
            // 칭호버튼
            uiBase* title_list = GET_DIALOG(uiCharacterStatusTitleList::kDialog_ID);
            if (title_list != NULL)
            {
                BOOL is_visible = title_list->IsVisible();
                is_visible = (is_visible == FALSE) ? TRUE : FALSE;
                title_list->ShowInterfaceWithSound(is_visible);
            }
        }
        break;
    case kControl_Button_BT01:
        {
            // 접속자포인트 보상버튼
            uiCharacterStatusMan* ui_manager = static_cast<uiCharacterStatusMan*>(GetMan());
            if (ui_manager != NULL)
            {
                ui_manager->SEND_MSG_CG_ITEM_QUERY_ACCUMULATE_POINT_ITEM_SYN(true);
            }
        }
        break;
    case kControl_Button_B999:
        {
            // 닫기 버튼
            ShowInterfaceWithSound(FALSE);
            uiBase* title_list = GET_DIALOG(uiCharacterStatusTitleList::kDialog_ID);
            if (title_list != NULL)
            {
                title_list->ShowInterfaceWithSound(FALSE);
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

#endif //_NA_003027_20111013_HONOR_SYSTEM