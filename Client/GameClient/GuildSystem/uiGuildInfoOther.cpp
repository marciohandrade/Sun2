#include "SunClientPrecompiledHeader.h"
#ifdef _NA_003923_20120222_GUILD_RENEWAL_UI_RENEWAL
#include "GuildSystem/uiGuildInfoOther.h"
#include "GuildSystem/GuildSystemFunction.h"
#include "GuildSystem/GuildSystemData.h"
#include "GuildSystem/uiGuildPoint.h"
#include "GuildSystem/uiGuildMember.h"
#include "GuildSystem/uiGuildIntroOther.h"
#include "GuildSystem/uiGuildManagement.h"
#include "GuildSystem/uiGuildLog.h"
#include "GuildSystem/uiGuildInstitution.h"
#include "GuildSystem/uiGuildInfo.h"
#include "uiCharacterStatus/uiCharacterStatusCurrencyInfo.h"
#include "GuildRankInfoParser.h"
#include "CurrencyInfoParser.h"
#include "InterfaceManager.h"
#include "GlobalFunc.h"
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
#include "Hero.h"
#include "GuildSystem/GuildSystemPacketFunction.h"
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION

const DWORD uiGuildInfoOther::kDialog_ID = g_IDGenerator.GenerateID();

uiGuildInfoOther::uiGuildInfoOther( InterfaceManager* ui_manager )
    :uiBase(ui_manager)
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    ,guild_join_request_state_(eGUILD_JOIN_REQUEST_CANCEL)
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
{

}

uiGuildInfoOther::~uiGuildInfoOther()
{

}

void uiGuildInfoOther::Init( CDrawBase* draw_base )
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

    control_ptr = GetControlWZ_byPosition(kControl_Button_BT01);
    if (control_ptr != NULL)
    {
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
        control_ptr->ShowWindowWZ(WZ_SHOW);
        guild_join_request_state_ = eGUILD_JOIN_REQUEST_CANCEL;        
#else
        control_ptr->ShowWindowWZ(WZ_HIDE);
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    }
}

void uiGuildInfoOther::Release()
{

}

void uiGuildInfoOther::MessageProc( SI_MESSAGE* message )
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

bool uiGuildInfoOther::OnLButtonClick( SI_MESSAGE* message )
{
    bool result = true;
    DWORD control_position = GetControlPosition_byID(message->dwCtrlID);
    switch (control_position)
    {
    case kControl_Button_BT01:
        {
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
            OnAnswerJoinRequest();
#else
            OnClickRequestJoin();
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
        }
        break;
    case kControl_Button_B999:
        {
            ShowInterface(FALSE);
        }
        break;
    
        break;
    }

    return result;
}

void uiGuildInfoOther::OnClickRequestJoin()
{
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    GuildSystem::GuildSystemData::GuildRankingInfoContainer& guild_ranking_info_container = 
        GuildSystem::GetGuildSystemData().guild_ranking_info_container();
    if (static_cast<size_t>(guild_rank_index()) >= guild_ranking_info_container.size())
    {
        return;
    }

    GUILD_RANKING_INFO& ranking_info = guild_ranking_info_container[guild_rank_index()];
    GuildSystem::GuildSystemData::GuildRankingGuildInfoContainer& ranking_guild_info_container = 
        GuildSystem::GetGuildSystemData().guild_ranking_guild_info_container();
    GuildSystem::GuildSystemData::GuildRankingGuildInfoContainer::iterator found_itr = 
        ranking_guild_info_container.find(ranking_info.guild_guid);
    if (found_itr == ranking_guild_info_container.end())
    {
        return;
    }

    // [GUILD_RENEWAL] 가입요청
    MSG_CG_GUILD_JOIN_REQUEST_SYN send_packet;
    GuildSystem::GuildSystemData::GuildInfoForClient& guild_info = found_itr->second;
    send_packet.guild_guid = guild_info.game_guild_info_.m_GuildGuid;
    /*
    eGUILD_JOIN_REQUEST_OK = 0, //가입요청, 가입승인
    eGUILD_JOIN_REQUEST_CANCEL, //가입취소, 가입거절
    */
    send_packet.request_type = eGUILD_JOIN_REQUEST_OK;
    GuildSystem::PacketFunction::SEND_CG_GUILD_JOIN_REQUEST_SYN(send_packet);
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
}

void uiGuildInfoOther::OnShowWindow( BOOL is_show )
{
    uiGuildIntroOther* ui_guild_intro_other = 
        GET_CAST_DIALOG(uiGuildIntroOther, uiGuildIntroOther::kDialog_ID);
    if (ui_guild_intro_other != NULL)
    {
        ui_guild_intro_other->set_guild_rank_index(guild_rank_index());
        ui_guild_intro_other->ShowInterface(is_show);
    }

    if (is_show == TRUE)
    {
        keyMsg msg;
        ZeroMemory(&msg,sizeof(msg));
        msg.dwType = uiGuildInfoOther::kDialog_ID;
        msg.wParam = uiGuildInfoOther::kDialog_ID;
        msg.DoSomething = GlobalFunc::DoShowWindowEx;
        g_KeyQueueManager.PushBack(msg);

        uiBase* ui_guild_info = GET_DIALOG(uiGuildInfo::kDialog_ID);
        if (ui_guild_info != NULL)
        {
            ui_guild_info->ShowInterface(FALSE);

            RECT control_rect;
            ui_guild_info->GetDialogWZ()->GetClientRect(&control_rect);

            MoveWindowWZ(static_cast<float>(control_rect.left), 
                static_cast<float>(control_rect.top));
        }

        MoveSubDialog();
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
        RequestJoinButtonReset();
        SetJoinRequestButton(eGUILD_JOIN_REQUEST_CANCEL); // 기본 버튼 상태
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
    }
    else
    {
        g_KeyQueueManager.DeleteMsg(uiGuildInfoOther::kDialog_ID);
    }
}

void uiGuildInfoOther::OnUpdateSolarDialog()
{
    MoveSubDialog();

    UpdateGuildInfo();
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
#else
    UpdateButtonState();
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION

    uiBase* ui_guild_intro_other = GET_DIALOG(uiGuildIntroOther::kDialog_ID);
    if (ui_guild_intro_other != NULL)
    {
        if (HaveFocus() == TRUE)
        {
            GlobalFunc::MoveToTopDialog(ui_guild_intro_other->GetDialogWZ()->GetDlgID());
        }
        else if (ui_guild_intro_other->HaveFocus() == TRUE)
        {
            GlobalFunc::MoveToTopDialog(GetDialogWZ()->GetDlgID());
        }
    }

    uiBase* ui_guild_info = GET_DIALOG(uiGuildInfo::kDialog_ID);
    if (ui_guild_info != NULL)
    {
        RECT control_rect;
        GetDialogWZ()->GetClientRect(&control_rect);

        ui_guild_info->MoveWindowWZ(static_cast<float>(control_rect.left), 
            static_cast<float>(control_rect.top));
    }
}

void uiGuildInfoOther::OnRenderSolarDialog()
{
    GuildSystem::GuildSystemData::GuildRankingInfoContainer& guild_ranking_info_container = 
        GuildSystem::GetGuildSystemData().guild_ranking_info_container();
    if (static_cast<size_t>(guild_rank_index()) >= guild_ranking_info_container.size())
    {
        return;
    }

    GUILD_RANKING_INFO& ranking_info = guild_ranking_info_container[guild_rank_index()];
    GuildSystem::GuildSystemData::GuildRankingGuildInfoContainer& ranking_guild_info_container = 
        GuildSystem::GetGuildSystemData().guild_ranking_guild_info_container();
    GuildSystem::GuildSystemData::GuildRankingGuildInfoContainer::iterator found_itr = 
        ranking_guild_info_container.find(ranking_info.guild_guid);
    if (found_itr == ranking_guild_info_container.end())
    {
        return;
    }

    // 길드마크 출력
    CCtrlPictureWZ* mark_picture = 
        static_cast<CCtrlPictureWZ*>(GetControlWZ_byPosition(kControl_Picture_PI00));
    if (mark_picture != NULL)
    {
        GuildSystem::GuildSystemData::GuildInfoForClient& guild_info = found_itr->second;
        DWORD background_index = guild_info.game_guild_info_.m_BackGroundIdx;
        DWORD pattern_index = guild_info.game_guild_info_.m_PatternIdx;
        RECT mark_rect = mark_picture->GetSizeRect();
        GuildSystem::Function::RenderGuildMark(background_index, mark_rect);
        GuildSystem::Function::RenderGuildMark(pattern_index, mark_rect);
    }
}

void uiGuildInfoOther::UpdateGuildInfo()
{
    GuildSystem::GuildSystemData::GuildRankingInfoContainer& guild_ranking_info_container = 
        GuildSystem::GetGuildSystemData().guild_ranking_info_container();
    if (static_cast<size_t>(guild_rank_index()) >= guild_ranking_info_container.size())
    {
        return;
    }

    GUILD_RANKING_INFO& ranking_info = guild_ranking_info_container[guild_rank_index()];
    GuildSystem::GuildSystemData::GuildRankingGuildInfoContainer& ranking_guild_info_container = 
        GuildSystem::GetGuildSystemData().guild_ranking_guild_info_container();
    GuildSystem::GuildSystemData::GuildRankingGuildInfoContainer::iterator found_itr = 
        ranking_guild_info_container.find(ranking_info.guild_guid);
    if (found_itr == ranking_guild_info_container.end())
    {
        return;
    }

    TCHAR info_string[INTERFACE_STRING_LENGTH] = {'\0', };
    GuildSystem::GuildSystemData::GuildInfoForClient& guild_info = found_itr->second;

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
        info_text_control->SetTextWZ(guild_info.game_guild_info_.m_tszGuildName);
    }

    // 길드마스터
    info_text_control = 
        static_cast<CCtrlImageStaticWZ*>(GetControlWZ_byPosition(kControl_TextImage_I007));
    if (info_text_control != NULL)
    {
        info_text_control->SetTextWZ(ranking_info.master_name);
    }

    // 길드랭킹
    info_text_control = 
        static_cast<CCtrlImageStaticWZ*>(GetControlWZ_byPosition(kControl_TextImage_I008));
    if (info_text_control != NULL)
    {
        Snprintf(info_string, INTERFACE_STRING_LENGTH - 1, "%d", guild_rank_index() + 1);
        info_text_control->SetTextWZ(info_string);
    }

    // 길드레벨
    info_text_control = 
        static_cast<CCtrlImageStaticWZ*>(GetControlWZ_byPosition(kControl_TextImage_I009));
    if (info_text_control != NULL)
    {
        Snprintf(info_string, INTERFACE_STRING_LENGTH - 1, "%d", guild_info.game_guild_info_.m_GuildGrade);
        info_text_control->SetTextWZ(info_string);
    }

    // 길드멤버수
    info_text_control = 
        static_cast<CCtrlImageStaticWZ*>(GetControlWZ_byPosition(kControl_TextImage_I010));
    const GuildRankInfo* guild_rank_info = 
        GuildRankInfoParser::Instance()->FindGuildRankInfo(guild_info.game_guild_info_.m_GuildGrade);
    if ((info_text_control != NULL) && 
        (guild_rank_info != NULL))
    {
        TCHAR current_number[INTERFACE_STRING_LENGTH] = {'\0', };
        TCHAR max_number[INTERFACE_STRING_LENGTH] = {'\0', };

        _itot(ranking_info.member_count, 
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
    if (text_control != NULL)
    {
        text_control->ShowWindowWZ(WZ_HIDE);
    }
}

#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
#else
void uiGuildInfoOther::UpdateButtonState()
{
}
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION

void uiGuildInfoOther::MoveSubDialog()
{
    RECT control_rect;
    GetDialogWZ()->GetClientRect(&control_rect);
    uiBase* sub_dialog = NULL;

    // 서브 탭 다이얼로그
    sub_dialog = GET_DIALOG(uiGuildIntroOther::kDialog_ID);
    if (sub_dialog != NULL)
    {
        sub_dialog->MoveWindowWZ(static_cast<float>(control_rect.left), 
            static_cast<float>(control_rect.bottom));
    }
}

#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
void uiGuildInfoOther::OnAnswerJoinRequest()
{    
    if (guild_join_request_state_ == eGUILD_JOIN_REQUEST_CANCEL)
        return;

    GuildSystem::GuildSystemData::GuildRankingInfoContainer& guild_ranking_info_container = 
        GuildSystem::GetGuildSystemData().guild_ranking_info_container();
    if (static_cast<size_t>(guild_rank_index()) >= guild_ranking_info_container.size())
    {
        return;
    }

    GUILD_RANKING_INFO& ranking_info = guild_ranking_info_container[guild_rank_index()];
    GuildSystem::GuildSystemData::GuildRankingGuildInfoContainer& ranking_guild_info_container = 
        GuildSystem::GetGuildSystemData().guild_ranking_guild_info_container();
    GuildSystem::GuildSystemData::GuildRankingGuildInfoContainer::iterator found_itr = 
        ranking_guild_info_container.find(ranking_info.guild_guid);
    if (found_itr == ranking_guild_info_container.end())
    {
        return;
    }

    uiSystemMan* ui_system_manager_ptr = GET_DIALOG_MANAGER(uiSystemMan, UIMAN_SYSTEM);
    if (ui_system_manager_ptr)
    {
        TCHAR temp_message[INTERFACE_STRING_LENGTH] = {0, };
        TCHAR guild_name[INTERFACE_STRING_LENGTH] = {0, };
        GuildSystem::GuildSystemData::GuildInfoForClient& guild_info = found_itr->second;
        Snprintf(guild_name, MAX_GUILDNAME_LENGTH+1, guild_info.game_guild_info_.m_tszGuildName);
        g_InterfaceManager.GetInterfaceStringFormat(temp_message, INTERFACE_STRING_LENGTH, 7297, guild_name);
        ui_system_manager_ptr->Sys_SystemMessage(temp_message, 
            NULL, 
            GuildSystem::Function::Callback_GuildJoinRequestSend,
            uiSystemMessage::kConfirm_Type,
            true);
    }
}

void uiGuildInfoOther::SetJoinRequestButton( const eGUILD_JOIN_REQUEST& guild_join_request_state )
{
    /*
    eGUILD_JOIN_REQUEST_OK = 0, //가입요청 하기, 가입요청 버튼상태, 가입승인
    eGUILD_JOIN_REQUEST_CANCEL, //가입취소 하기, 가입취소 버튼상태, 가입거절
    eGUILD_JOIN_REQUEST_UPDATE, //가입 요청 내용 변경
    */
    guild_join_request_state_ = guild_join_request_state;
    CCtrlButtonWZ* control_ptr = static_cast<CCtrlButtonWZ*>(GetControlWZ_byPosition(kControl_Button_BT01));
    if (NULL == control_ptr)
        return;
    
    TCHAR button_string[INTERFACE_STRING_LENGTH] = {0, };
    switch (guild_join_request_state)
    {
    case eGUILD_JOIN_REQUEST_OK: // 길드 가입 요청 버튼 상태
        {            
            // 길드가입 요청
            control_ptr->EnableWindowWZ(TRUE);
            g_InterfaceManager.GetInterfaceString(7280, button_string);
            control_ptr->SetTextWZ(button_string);
        }
        break;
    case eGUILD_JOIN_REQUEST_CANCEL: // 길드 가입 대기상태 버튼 상태
        {
            // 길드가입 대기
            g_InterfaceManager.GetInterfaceString(7296, button_string);
            control_ptr->SetTextWZ(button_string);
            control_ptr->EnableWindowWZ(FALSE);
        }
        break;
    default:
        {
            // 나머지 상태는 없다.
        }
    }
}

void uiGuildInfoOther::RequestJoinButtonReset()
{
    CCtrlButtonWZ* button_control = 
        static_cast<CCtrlButtonWZ*>(GetControlWZ_byPosition(kControl_Button_BT01));
    if (!button_control)
        return;

    if (!g_pHero)
        return;

    if (g_pHero->IsExistGuild() == TRUE)
    {
        button_control->ShowWindowWZ(WZ_HIDE);
    }
    else
    {
        button_control->ShowWindowWZ(WZ_SHOW);
    }
}
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
#endif //_NA_003923_20120222_GUILD_RENEWAL_UI_RENEWAL