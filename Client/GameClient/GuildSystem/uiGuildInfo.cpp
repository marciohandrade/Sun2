#include "SunClientPrecompiledHeader.h"
#ifdef _NA_003923_20120222_GUILD_RENEWAL_UI_RENEWAL
#include "GuildSystem/uiGuildInfo.h"
#include "GuildSystem/GuildSystemFunction.h"
#include "GuildSystem/GuildSystemData.h"
#include "GuildSystem/uiGuildPoint.h"
#include "GuildSystem/uiGuildMember.h"
#include "GuildSystem/uiGuildIntro.h"
#include "GuildSystem/uiGuildManagement.h"
#ifdef _NA_008334_20150608_SONNENSCHEIN
  #include "GuildSystem/uiGuildPvE.h"
#endif //_NA_008334_20150608_SONNENSCHEIN
#include "GuildSystem/uiGuildLog.h"
#include "GuildSystem/uiGuildInstitution.h"
#include "GuildSystem/uiGuildInfoOther.h"
#include "uiCharacterStatus/uiCharacterStatusCurrencyInfo.h"
#include "GuildRankInfoParser.h"
#include "CurrencyInfoParser.h"
#include "InterfaceManager.h"
#include "GlobalFunc.h"
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
#include "Mouse.h"
#include "uiTooltipMan/uiToolTipMan.h"
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION

const DWORD uiGuildInfo::kDialog_ID = StrToWzID("7340");

uiGuildInfo::uiGuildInfo( InterfaceManager* ui_manager )
    :uiBase(ui_manager)
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    ,tooltip_manager_ptr_(NULL)
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
{

}

uiGuildInfo::~uiGuildInfo()
{
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    tooltip_manager_ptr_ = NULL;
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION

}

void uiGuildInfo::Init( CDrawBase* draw_base )
{
    uiBase::Init(draw_base);

    RegisterControlPosition(StrToWzID("C000"), kControl_CheckBT_C000);
    RegisterControlPosition(StrToWzID("B999"), kControl_Button_B999);
    RegisterControlPosition(StrToWzID("BT01"), kControl_Button_BT01);
    RegisterControlPosition(StrToWzID("S004"), kControl_Text_S004);
    RegisterControlPosition(StrToWzID("S005"), kControl_Text_S005);
    RegisterControlPosition(StrToWzID("TI02"), kControl_TextImage_TI02);
    RegisterControlPosition(StrToWzID("I006"), kControl_TextImage_I006);
    RegisterControlPosition(StrToWzID("I007"), kControl_TextImage_I007);
    RegisterControlPosition(StrToWzID("I008"), kControl_TextImage_I008);
    RegisterControlPosition(StrToWzID("I009"), kControl_TextImage_I009);
    RegisterControlPosition(StrToWzID("I010"), kControl_TextImage_I010);
    RegisterControlPosition(StrToWzID("PI00"), kControl_Picture_PI00);

    CControlWZ* control_ptr = GetControlWZ(StrToWzID("P005"));
    if (control_ptr != NULL)
    {
        control_ptr->SetDiscardMessage(true);
    }

    control_ptr = GetControlWZ_byPosition(kControl_CheckBT_C000);
    if (control_ptr != NULL)
    {
        control_ptr->ShowWindowWZ(WZ_HIDE);
    }

    control_ptr = GetControlWZ_byPosition(kControl_Text_S004);
    if (control_ptr != NULL)
    {
        control_ptr->ShowWindowWZ(WZ_HIDE);
    }
}

void uiGuildInfo::Release()
{
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    tooltip_manager_ptr_ = NULL;
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
}

void uiGuildInfo::MessageProc( SI_MESSAGE* message )
{
    if (IsLock())
        return;

    if (GetMan() == NULL)
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

bool uiGuildInfo::OnLButtonClick( SI_MESSAGE* message )
{
    bool result = true;
    DWORD control_position = GetControlPosition_byID(message->dwCtrlID);
    switch (control_position)
    {
    case kControl_Button_BT01:
        {
            OnClickDonateGuildCoin();
        }
        break;
    case kControl_CheckBT_C000:
        {
        }
        break;
    case kControl_Button_B999:
        {
            ShowInterface(FALSE);
        }
        break;
    }

    return result;
}

void uiGuildInfo::OnClickDonateGuildCoin()
{
    const BYTE max_guild_grade = GuildRankInfoParser::Instance()->GetGuildMaxGrade();
    const BYTE current_guild_grade = GuildSystem::GetGuildSystemData().GetGuildInfo().m_GuildGrade;

    if (current_guild_grade >= max_guild_grade)
    {
        // 5195 더이상 랭크 업 할수 없습니다.
        GlobalFunc::VerifyMessage(5195);

        return;
    }

    uiGuildPoint* guild_point = GET_CAST_DIALOG(uiGuildPoint, uiGuildPoint::kDialog_ID);
    if (guild_point != NULL)
    {
        guild_point->ShowInterface(TRUE);
    }
}

void uiGuildInfo::OnShowWindow( BOOL is_show )
{
    if (is_show == TRUE)
    {
        set_show_flag(false);

        keyMsg msg;
        ZeroMemory(&msg,sizeof(msg));
        msg.dwType = uiGuildInfo::kDialog_ID;
        msg.wParam = uiGuildInfo::kDialog_ID;
        msg.DoSomething = GlobalFunc::DoShowWindowEx;
        g_KeyQueueManager.PushBack(msg);

        tab_button_controller().ShowCurrentDialog(true);
        MoveSubDialog();

        uiBase* ui_guild_info = GET_DIALOG(uiGuildInfoOther::kDialog_ID);
        if (ui_guild_info != NULL)
        {
            ui_guild_info->ShowInterface(FALSE);
        }

#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
        if (tooltip_manager_ptr_ == NULL)
        {
            tooltip_manager_ptr_ = GET_DIALOG_MANAGER(uiToolTipMan, UIMAN_TOOLTIP);
            assert(tooltip_manager_ptr_);
        }
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    }
    else
    {
        g_KeyQueueManager.DeleteMsg(uiGuildInfo::kDialog_ID);

        tab_button_controller().ShowCurrentDialog(false);

#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
        tooltip_manager_ptr_ = NULL;
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    }
}

void uiGuildInfo::OnUpdateSolarDialog()
{
    MoveSubDialog();

    UpdateGuildInfo();
    UpdateButtonState();

    tab_button_controller().UpdateDialogFocus(this);
}

void uiGuildInfo::OnRenderSolarDialog()
{
    // 길드마크 출력
    CCtrlPictureWZ* mark_picture = 
        static_cast<CCtrlPictureWZ*>(GetControlWZ_byPosition(kControl_Picture_PI00));
    if (mark_picture != NULL)
    {
        DWORD background_index = GuildSystem::GetGuildSystemData().GetGuildInfo().m_BackGroundIdx;
        DWORD pattern_index = GuildSystem::GetGuildSystemData().GetGuildInfo().m_PatternIdx;
        RECT mark_rect = mark_picture->GetSizeRect();
        GuildSystem::Function::RenderGuildMark(background_index, mark_rect);
        GuildSystem::Function::RenderGuildMark(pattern_index, mark_rect);
    }

#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    if (tooltip_manager_ptr_ == NULL)
    {
        return;
    }

    RenderTooltip();
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
}

void uiGuildInfo::UpdateGuildInfo()
{
    TCHAR info_string[INTERFACE_STRING_LENGTH] = {'\0', };
    const GUILD_INFO& guild_info =  GuildSystem::GetGuildSystemData().GetGuildInfo();

    // 길드연합
    CCtrlImageStaticWZ* info_text_control = 
        static_cast<CCtrlImageStaticWZ*>(GetControlWZ_byPosition(kControl_TextImage_TI02));
    if (info_text_control != NULL)
    {
        // [GUILD_RENEWAL] 길드연합은 추후 구현
        info_text_control->SetTextWZ("");
    }

    // 길드명
    info_text_control = 
        static_cast<CCtrlImageStaticWZ*>(GetControlWZ_byPosition(kControl_TextImage_I006));
    if (info_text_control != NULL)
    {
        info_text_control->SetTextWZ(guild_info.m_tszGuildName);
    }

    // 길드마스터
    info_text_control = 
        static_cast<CCtrlImageStaticWZ*>(GetControlWZ_byPosition(kControl_TextImage_I007));
    if (info_text_control != NULL)
    {
        info_text_control->SetTextWZ(GuildSystem::GetGuildSystemData().GetMasterName());
    }

    // 길드랭킹
    info_text_control = 
        static_cast<CCtrlImageStaticWZ*>(GetControlWZ_byPosition(kControl_TextImage_I008));
    if (info_text_control != NULL)
    {
        if (GuildSystem::GetGuildSystemData().guild_rank() > 0)
        {
            Snprintf(info_string, INTERFACE_STRING_LENGTH - 1, "%d", GuildSystem::GetGuildSystemData().guild_rank());
            info_text_control->SetTextWZ(info_string);
        }
        else
        {
            info_text_control->SetTextWZ("");
        }
    }

    // 길드레벨
    info_text_control = 
        static_cast<CCtrlImageStaticWZ*>(GetControlWZ_byPosition(kControl_TextImage_I009));
    if (info_text_control != NULL)
    {
        Snprintf(info_string, INTERFACE_STRING_LENGTH - 1, "%d", guild_info.m_GuildGrade);
        info_text_control->SetTextWZ(info_string);
    }

    // 길드멤버수
    info_text_control = 
        static_cast<CCtrlImageStaticWZ*>(GetControlWZ_byPosition(kControl_TextImage_I010));
    const GuildRankInfo* guild_rank_info = 
        GuildRankInfoParser::Instance()->FindGuildRankInfo(guild_info.m_GuildGrade);
    if ((info_text_control != NULL) && 
        (guild_rank_info != NULL))
    {
        TCHAR current_number[INTERFACE_STRING_LENGTH] = {'\0', };
        TCHAR max_number[INTERFACE_STRING_LENGTH] = {'\0', };

        _itot(GuildSystem::GetGuildSystemData().guild_member_container().size(), 
            current_number, 
            10);

        _itot(guild_rank_info->max_member_count_, 
            max_number, 
            10);

        // 7290 %s / %s
        g_InterfaceManager.GetInterfaceStringFormat(
            info_string, 
            INTERFACE_STRING_LENGTH - 1, 
            7290, 
            current_number, 
            max_number);

        info_text_control->SetTextWZ(info_string);
    }

    // 길드코인
    CCtrlStaticWZ* text_control = 
        static_cast<CCtrlStaticWZ*>(GetControlWZ_byPosition(kControl_Text_S005));
    uiCharacterStatusCurrencyInfo* currency_dialog = 
        GET_CAST_DIALOG(uiCharacterStatusCurrencyInfo, uiCharacterStatusCurrencyInfo::kDialog_ID);
    if (currency_dialog != NULL)
    {
        TCHAR guild_coin_point[INTERFACE_STRING_LENGTH] = {'\0', };
        uiCharacterStatusCurrencyInfo::CurrencyPointInfo* currency_point = 
            currency_dialog->GetCurrencyPointInfo(CURRENCY_INFO_GUILD_COIN);

        if (currency_point != NULL)
        {
            _itot(currency_point->currency_point, 
                guild_coin_point, 
                10);
        }

        // 7275 길드코인: %s
        g_InterfaceManager.GetInterfaceStringFormat(info_string, 
            INTERFACE_STRING_LENGTH - 1, 
            7275, 
            guild_coin_point);

        text_control->SetTextWZ(info_string);

        CCtrlButtonWZ* button_control = 
            static_cast<CCtrlButtonWZ*>(GetControlWZ_byPosition(kControl_Button_BT01));
        if (button_control != NULL)
        {
            if (currency_point->currency_point > 0)
            {
                button_control->EnableWindowWZ(true);
            }
            else
            {
                button_control->EnableWindowWZ(false);
            }
        }
    }
}

void uiGuildInfo::UpdateButtonState()
{
    // 코인기부 및 가입요청 버튼 갱신
    CCtrlButtonWZ* button_control = 
        static_cast<CCtrlButtonWZ*>(GetControlWZ_byPosition(kControl_Button_BT01));
    if (button_control != NULL)
    {
        // 7276 기부하기
        button_control->SetTextID(7276);
    }    

    // 가입요청차단 버튼 갱신
    // [GUILD_RENEWAL]
    CControlWZ* check_button = GetControlWZ_byPosition(kControl_CheckBT_C000);
    if (check_button != NULL)
    {
        check_button->EnableWindowWZ(false);
    }
}

void uiGuildInfo::MoveSubDialog()
{
    RECT control_rect;
    GetDialogWZ()->GetClientRect(&control_rect);
    uiBase* sub_dialog = NULL;

    // 서브 탭 다이얼로그
    sub_dialog = tab_button_controller().GetCurrentTabDialog();
    if (sub_dialog != NULL)
    {
        sub_dialog->MoveWindowWZ(static_cast<float>(control_rect.left), 
            static_cast<float>(control_rect.bottom));
    }
}

void uiGuildInfo::InitTabController()
{
    tab_button_controller().Initialize(kTab_Max, 0, NULL, kGuildIntro);
    tab_button_controller().PushTab(kGuildIntro, GET_DIALOG(uiGuildIntro::kDialog_ID));
    tab_button_controller().PushTab(kGuildMember, GET_DIALOG(uiGuildMember::kDialog_ID));
    tab_button_controller().PushTab(kGuildInstitution, GET_DIALOG(uiGuildInstitution::kDialog_ID));
    tab_button_controller().PushTab(kGuildManagement, GET_DIALOG(uiGuildManagement::kDialog_ID));
#ifdef _NA_008334_20150608_SONNENSCHEIN
    tab_button_controller().PushTab(kGuildPvE, GET_DIALOG(uiGuildPvE::kDialog_ID));
#endif //_NA_008334_20150608_SONNENSCHEIN
    tab_button_controller().PushTab(kGuildLog, GET_DIALOG(uiGuildLog::kDialog_ID));
}

#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
void uiGuildInfo::RenderTooltip()
{
    // 툴팁 계산을 위해서
    int mouse_x = Mouse::Instance()->GetMouseX();
    int mouse_y = Mouse::Instance()->GetMouseY();
    CControlWZ* control_ptr = NULL;
    control_ptr = GetControlWZ_byPosition(kControl_Text_S005);
    if (control_ptr)
    {
        RECT control_rect = control_ptr->GetSizeRect();
        if (control_rect.left <= mouse_x && control_rect.right >= mouse_x
            && control_rect.top <= mouse_y && control_rect.bottom >= mouse_y)
        {
            tooltip_manager_ptr_->InitTooltip();
            TCHAR result_message[INTERFACE_STRING_LENGTH] = {0, };
            g_InterfaceManager.GetInterfaceString(7283, result_message);
            tooltip_manager_ptr_->RegisterTooltip(result_message, 
                -1,
                22,
                StrToWzID("s210"),
                WzColor_RGBA(255, 255, 255, 255),
                TOOLTIP_BG_WZCOLOR,
                c_dwTOOLTIP_EXTRA_INFO_DT_LEFT);

            tooltip_manager_ptr_->AddEmptyLineForTooltip();

            tooltip_manager_ptr_->SetTooltipDialogID(GetDialogKey());
        }
    }
}
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
#endif //_NA_003923_20120222_GUILD_RENEWAL_UI_RENEWAL