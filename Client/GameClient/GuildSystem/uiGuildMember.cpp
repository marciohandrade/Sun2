#include "SunClientPrecompiledHeader.h"
#ifdef _NA_003923_20120222_GUILD_RENEWAL_UI_RENEWAL
#include "GuildSystem/uiGuildMember.h"
#include "GuildSystem/GuildSystemFunction.h"
#include "GuildSystem/GuildSystemPacketFunction.h"
#include "GuildSystem/GuildSystemData.h"
#include "GuildSystem/uiGuildDutyVest.h"
#include "GuildSystem/uiGuildRelationAsk.h"
#include "GuildSystem/uiGuildInfo.h"
#include "GuildRightInfoParser.h"
#include "uiCommunityMan/uiCommunityMan.h"
#include "uiCommunityInfo/uiCommunityInfo.h"
#include "uiCharacterStatus/uiCharacterStatusCurrencyInfo.h"
#include "InterfaceManager.h"
#include "Hero.h"
#include "GlobalFunc.h"

const DWORD uiGuildMember::kDialog_ID = StrToWzID("7342");

uiGuildMember::uiGuildMember( InterfaceManager* ui_manager )
{

}

uiGuildMember::~uiGuildMember()
{

}

void uiGuildMember::Init( CDrawBase* draw_base )
{
    uiBase::Init(draw_base);

    RegisterControlPosition(StrToWzID("C000"), kControl_CheckBT_C000);
    RegisterControlPosition(StrToWzID("C001"), kControl_CheckBT_C001);
    RegisterControlPosition(StrToWzID("C002"), kControl_CheckBT_C002);
    RegisterControlPosition(StrToWzID("C003"), kControl_CheckBT_C003);
    RegisterControlPosition(StrToWzID("C004"), kControl_CheckBT_C004);
#ifdef _NA_008334_20150608_SONNENSCHEIN
    RegisterControlPosition(StrToWzID("C006"), kControl_CheckBT_C006);
#endif //_NA_008334_20150608_SONNENSCHEIN
    RegisterControlPosition(StrToWzID("B000"), kControl_Button_B000);
    RegisterControlPosition(StrToWzID("B001"), kControl_Button_B001);
    RegisterControlPosition(StrToWzID("B002"), kControl_Button_B002);
    RegisterControlPosition(StrToWzID("B003"), kControl_Button_B003);
    RegisterControlPosition(StrToWzID("B004"), kControl_Button_B004);
    RegisterControlPosition(StrToWzID("B005"), kControl_Button_B005);
    RegisterControlPosition(StrToWzID("B006"), kControl_Button_B006);
    RegisterControlPosition(StrToWzID("B007"), kControl_Button_B007);
    RegisterControlPosition(StrToWzID("B008"), kControl_Button_B008);
    RegisterControlPosition(StrToWzID("B009"), kControl_Button_B009);
    RegisterControlPosition(StrToWzID("B010"), kControl_Button_B010);
    RegisterControlPosition(StrToWzID("L000"), kControl_List_L000);
    RegisterControlPosition(StrToWzID("V000"), kControl_VScroll_V000);
    RegisterControlPosition(StrToWzID("C005"), kControl_CheckBT_C005);

    CCtrlListWZ* member_list = static_cast<CCtrlListWZ*>(GetControlWZ_byPosition(kControl_List_L000));
    if (member_list != NULL)
    {
        member_list->SetUseReport(true);
        //member_list->SetUnuseSelBar(true);
        member_list->SetSellHeight(18);
        member_list->SetColorSelBar(RGBA(150, 10, 10, 80));
        member_list->SetReportUseCnt(6);
        member_list->SetHideReportRender(TRUE);
        member_list->SetReportButtonInfo(0, 184, NULL); // 길드원 이름
        member_list->SetReportButtonInfo(1, 46, NULL); // 레벨
        member_list->SetReportButtonInfo(2, 88, NULL); // 클래스
        member_list->SetReportButtonInfo(3, 142, NULL); // 직책
        member_list->SetReportButtonInfo(4, 76, NULL); // 공헌도
        member_list->SetReportButtonInfo(5, 69, NULL); // 접속여부
    }
}

void uiGuildMember::Release()
{

}

void uiGuildMember::MessageProc( SI_MESSAGE* message )
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
    case RT_MSG_LBUTTONDCLICK:
        OnLButtonDClick(message);
        break;
    case RT_MSG_SB_LINETOP:
        OnVScrollUp(message);
        break;
    case RT_MSG_SB_LINEBOTTOM:
        OnVScrollDown(message);
        break;
    case RT_MSG_SB_THUMBTRACK:      //!< 썸을 드래그 하고 있다.
    case RT_MSG_SB_THUMBPOSITION:   //!< 썸을 드래그한후 마우스를 놓았다.
    case RT_MSG_SB_ENDSCROLL:       //!< 스크롤이 종료 되었다.
        OnVScrollThumb(message);
        break;
    default:
        SolarDialog::MessageProc(message);
        break;
    }
}

bool uiGuildMember::OnLButtonClick( SI_MESSAGE* message )
{
    bool result = true;
    DWORD control_position = GetControlPosition_byID(message->dwCtrlID);
    switch (control_position)
    {
    case kControl_CheckBT_C000:
    case kControl_CheckBT_C001:
    case kControl_CheckBT_C002:
    case kControl_CheckBT_C003:
    case kControl_CheckBT_C004:
#ifdef _NA_008334_20150608_SONNENSCHEIN
    case kControl_CheckBT_C006:
#endif //_NA_008334_20150608_SONNENSCHEIN
        {
            uiGuildInfo* ui_guild_info = GET_CAST_DIALOG(uiGuildInfo, uiGuildInfo::kDialog_ID);
            if (ui_guild_info != NULL)
            {
                ui_guild_info->tab_button_controller().OnClickTab(
                    control_position - kControl_CheckBT_C000);
            }
            RefreshTab();
        }
        break;
    case kControl_Button_B000:
    case kControl_Button_B001:
    case kControl_Button_B002:
    case kControl_Button_B003:
    case kControl_Button_B004:
    case kControl_Button_B005:
        {
            GuildSystem::GuildSystemData::GuildMemberSortType sort_type = 
                static_cast<GuildSystem::GuildSystemData::GuildMemberSortType>(control_position - kControl_Button_B000);
            GuildSystem::GetGuildSystemData().SortGuildMember(sort_type);
        }
        break;
    case kControl_Button_B006:
        {
            OnClickInvateGuildMember();
        }
        break;
    case kControl_Button_B007:
        {
            GuildSystem::Function::ShowConfirmKickGuildMember();
        }
        break;
    case kControl_Button_B008:
        {
            OnClickDutyVest();
        }
        break;
    case kControl_Button_B010:
        {
            GuildSystem::Function::ShowConfirmWithdrawGuild();
        }
        break;
    case kControl_Button_B009:
        {
            GuildSystem::Function::ShowConfirmDelegateMaster();
        }
        break;
    }

    return result;
}

bool uiGuildMember::OnLButtonDClick( SI_MESSAGE* message )
{
    bool result = true;
    DWORD control_position = GetControlPosition_byID(message->dwCtrlID);
    switch (control_position)
    {
    case kControl_List_L000:
        {
            GUILD_MEMBER_INFO* member_info = GetSelectedMemberInfo();
            if ((member_info != NULL) && 
#ifndef _NA_0_20100527_ADD_CHAR_RELATION_MEMO   //#if'N'def
                (member_info->m_bLogin == TRUE) && 
#endif //_NA_0_20100527_ADD_CHAR_RELATION_MEMO
                (g_pHero != NULL) &&
                (_tcscmp(g_pHero->GetName(), member_info->m_tszCharName) != 0))
            {
                GuildSystem::PacketFunction::SEND_CW_FRIEND_STATUS_REQ(
                    member_info->m_tszCharName, 
                    member_info->m_CharGuid);

                uiCommunityMan* community_manager = 
                    GET_DIALOG_MANAGER(uiCommunityMan, UIMAN_COMMUNITY);
                if ((community_manager != NULL) && 
                    (community_manager->GetCommunityinfo_Dialog() != NULL))
                {
                    community_manager->GetCommunityinfo_Dialog()->set_request_uimanager_id(UIMAN_GUILD);
                }
            }
        }
        break;
    }
    return true;
}

bool uiGuildMember::OnVScrollUp( SI_MESSAGE* message )
{
    CCtrlVScrollWZ* scroll_control = 
        static_cast<CCtrlVScrollWZ*>(GetControlWZ_byPosition(kControl_VScroll_V000));
    CCtrlListWZ* member_list = static_cast<CCtrlListWZ*>(GetControlWZ_byPosition(kControl_List_L000));
    if ((scroll_control != NULL) && 
        (member_list != NULL))
    {
        int range_min = 0;
        int range_max = 0;
        int currenct_position = scroll_control->GetScrollPosWZ();
        scroll_control->GetScrollRangeWZ(&range_min, &range_max);
        --currenct_position;
        currenct_position = max(currenct_position, 0);
        currenct_position = min(currenct_position, range_max);
        scroll_control->SetScrollPosWZ(currenct_position);

        member_list->SetBeginDrawIndex(currenct_position);
    }

    return true;
}

bool uiGuildMember::OnVScrollDown( SI_MESSAGE* message )
{
    CCtrlVScrollWZ* scroll_control = 
        static_cast<CCtrlVScrollWZ*>(GetControlWZ_byPosition(kControl_VScroll_V000));
    CCtrlListWZ* member_list = static_cast<CCtrlListWZ*>(GetControlWZ_byPosition(kControl_List_L000));
    if ((scroll_control != NULL) && 
        (member_list != NULL))
    {
        int range_min = 0;
        int range_max = 0;
        int currenct_position = scroll_control->GetScrollPosWZ();
        scroll_control->GetScrollRangeWZ(&range_min, &range_max);
        ++currenct_position;
        currenct_position = max(currenct_position, 0);
        currenct_position = min(currenct_position, range_max);
        scroll_control->SetScrollPosWZ(currenct_position);

        member_list->SetBeginDrawIndex(currenct_position);
    }

    return true;
}

bool uiGuildMember::OnVScrollThumb( SI_MESSAGE* message )
{
    CCtrlVScrollWZ* scroll_control = 
        static_cast<CCtrlVScrollWZ*>(GetControlWZ_byPosition(kControl_VScroll_V000));
    CCtrlListWZ* member_list = static_cast<CCtrlListWZ*>(GetControlWZ_byPosition(kControl_List_L000));
    if ((scroll_control != NULL) && 
        (member_list != NULL))
    {
        int currenct_position = scroll_control->GetScrollPosWZ();
        member_list->SetBeginDrawIndex(currenct_position);
    }

    return true;
}

void uiGuildMember::OnShowWindow( BOOL is_show )
{
    if (is_show == TRUE)
    {
        RefreshTab();

    }
    else
    {
    }
}

void uiGuildMember::OnUpdateSolarDialog()
{
    UpdateMemberInfo();
    UpdateButtonState();
    UpdateWheelScroll();
}

void uiGuildMember::OnRenderSolarDialog()
{

}

void uiGuildMember::UpdateMemberInfo()
{
    CCtrlListWZ* member_list = static_cast<CCtrlListWZ*>(GetControlWZ_byPosition(kControl_List_L000));
    CCtrlVScrollWZ* scroll_control = static_cast<CCtrlVScrollWZ*>(GetControlWZ_byPosition(kControl_VScroll_V000));
    CCtrlButtonCheckWZ* hide_offline_check = 
        static_cast<CCtrlButtonCheckWZ*>(GetControlWZ_byPosition(kControl_CheckBT_C005));

    if ((member_list != NULL) && 
        (scroll_control != NULL))
    {
        int current_select = member_list->GetCurSel();
        int begin_index = scroll_control->GetScrollPosWZ();

        member_list->DeleteItemAll();

        GuildSystem::GuildSystemData::GuildMemberContainer guild_member_container = 
            GuildSystem::GetGuildSystemData().guild_member_container();

        TCHAR info_string[INTERFACE_STRING_LENGTH] = {'\0', };
        const DWORD kTextColor[3] = {RGBA(255,255,255,255), RGBA(240,200,120,255), RGBA(128, 128, 128, 255)};
        int color_index = 0;

        int inset_index = 0;
        size_t member_number = guild_member_container.size();
        for (size_t list_index = 0; list_index < member_number; ++list_index)
        {
            GUILD_MEMBER_INFO& member_info = guild_member_container[list_index];

            if ((hide_offline_check != NULL) && 
                (hide_offline_check->GetCheckState() == true) && 
                (member_info.m_bLogin == FALSE))
            {
                continue;
            }

            color_index = 0;
            if ((g_pHero != NULL) && 
                (StrnCmp(g_pHero->GetName(), member_info.m_tszCharName, MAX_CHARNAME_LENGTH) == 0))
            {
                color_index = 1;
            }
            else if (member_info.m_bLogin == FALSE)
            {
                color_index = 2;
            }

            inset_index = member_list->InsertItemBack();
            // 이름
            member_list->SetItemWZ(inset_index, 
                0, 
                member_info.m_tszCharName, 
                StrToWzID("st11"), 
                DT_CENTER | DT_VCENTER, 
                RGBA(0,0,0,0), 
                kTextColor[color_index], 
                kTextColor[color_index]);

            // 레벨
            Snprintf(info_string, INTERFACE_STRING_LENGTH - 1, "%d", member_info.m_Level);
            member_list->SetItemWZ(inset_index, 
                1, 
                info_string, 
                StrToWzID("st11"), 
                DT_CENTER | DT_VCENTER, 
                RGBA(0,0,0,0), 
                kTextColor[color_index], 
                kTextColor[color_index]);

            // 클래스
            member_list->SetItemWZ(inset_index, 
                2, 
                GlobalFunc::GetClassString(static_cast<eCHAR_TYPE>(member_info.m_byClass)), 
                StrToWzID("st11"), 
                DT_CENTER | DT_VCENTER, 
                RGBA(0,0,0,0), 
                kTextColor[color_index], 
                kTextColor[color_index]);

            // 직책
            g_InterfaceManager.GetInterfaceString(
                GuildSystem::Function::GetGuildDutyStringCode(member_info.m_eGuildDuty), 
                info_string);
            member_list->SetItemWZ(inset_index, 
                3, 
                info_string, 
                StrToWzID("st11"), 
                DT_CENTER | DT_VCENTER, 
                RGBA(0,0,0,0), 
                kTextColor[color_index], 
                kTextColor[color_index]);

            // 공헌도
            TCHAR guild_coin_point[INTERFACE_STRING_LENGTH] = {'0', '\0', };
#ifdef _NA004034_20120102_GUILD_POINT_SYSTEM
            _itot(member_info.guild_coin_, guild_coin_point, 10);
#endif //_NA004034_20120102_GUILD_POINT_SYSTEM
            Snprintf(info_string, INTERFACE_STRING_LENGTH - 1, "%s", guild_coin_point);
            member_list->SetItemWZ(inset_index, 
                4, 
                info_string, 
                StrToWzID("st11"), 
                DT_CENTER | DT_VCENTER, 
                RGBA(0,0,0,0), 
                kTextColor[color_index], 
                kTextColor[color_index]);

            // 접속여부
            // 7365 Y
            // 7366 N
            int login_state_code = member_info.m_bLogin == TRUE ? 7365 : 7366;
            
            // 오른쪽 정렬을 할 때 일폰어 폰트가 짤리는 경우가 발생하여 중앙정렬+왼쪽 공백을 넣어줌 (오른쪽 공백을 넣어봤지만 사라짐)
            Snprintf(info_string,
                INTERFACE_STRING_LENGTH-1,
                "       %s",
                g_InterfaceManager.GetInterfaceString(login_state_code));

            member_list->SetItemWZ(inset_index, 
                5, 
                info_string, 
                StrToWzID("st11"), 
                DT_CENTER | DT_VCENTER, 
                RGBA(0,0,0,0), 
                kTextColor[color_index], 
                kTextColor[color_index]);

            member_list->SetItemData(inset_index, (void*)list_index);
        }

        int scroll_range = (inset_index + 1) - member_list->GetDrawRowCount();
        int range_min = 0;
        int range_max = 0;
        scroll_range = max(0, scroll_range);
        begin_index = max(0, begin_index);
        begin_index = min(begin_index, scroll_range);
        
        scroll_control->GetScrollRangeWZ(&range_min, &range_max);
        if (scroll_range != range_max)
        {
            scroll_control->SetScrollRangeWZ(0, scroll_range);
            scroll_control->SetScrollPosWZ(begin_index);
        }        

        member_list->SetBeginDrawIndex(begin_index);
        member_list->SetCurSel(current_select);
    }
}

void uiGuildMember::UpdateButtonState()
{
    // 자신의 직책에 따른 버튼 활성화
    
    if (g_pHero == NULL)
    {
        return;
    }

    PLAYER_GUILD_RENDER_INFO& guild_render_info = g_pHero->GetGuildRenderInfo();

    // 길드초대
    CCtrlButtonWZ* button_control = 
        static_cast<CCtrlButtonWZ*>(GetControlWZ_byPosition(kControl_Button_B006));
    bool have_right = GuildRightInfoParser::Instance()->CheckDutyForRight(
        eGUILD_RIGHTS_JOIN, static_cast<eGUILD_DUTY>(guild_render_info.m_GuildDuty));
    if (button_control != NULL)
    {
        GlobalFunc::EnableControl(button_control, have_right);
    }

    // 길드제명
    button_control = static_cast<CCtrlButtonWZ*>(GetControlWZ_byPosition(kControl_Button_B007));
    have_right = GuildRightInfoParser::Instance()->CheckDutyForRight(
        eGUILD_RIGHTS_KICK, static_cast<eGUILD_DUTY>(guild_render_info.m_GuildDuty));
    if (button_control != NULL)
    {
        GlobalFunc::EnableControl(button_control, have_right);
    }

    // 직책변경
    button_control = static_cast<CCtrlButtonWZ*>(GetControlWZ_byPosition(kControl_Button_B008));
    have_right = GuildRightInfoParser::Instance()->CheckDutyForRight(
        eGUILD_RIGHTS_CHANGE_DUTY, static_cast<eGUILD_DUTY>(guild_render_info.m_GuildDuty));
    if (button_control != NULL)
    {
        GlobalFunc::EnableControl(button_control, have_right);
    }

    // 길드탈퇴
    button_control = static_cast<CCtrlButtonWZ*>(GetControlWZ_byPosition(kControl_Button_B010));
    have_right = GuildRightInfoParser::Instance()->CheckDutyForRight(
        eGUILD_RIGHTS_WITHDRAW, static_cast<eGUILD_DUTY>(guild_render_info.m_GuildDuty));
    if (button_control != NULL)
    {
        GlobalFunc::EnableControl(button_control, have_right);
    }

    // 길드장위임
    button_control = static_cast<CCtrlButtonWZ*>(GetControlWZ_byPosition(kControl_Button_B009));
    have_right = GuildRightInfoParser::Instance()->CheckDutyForRight(
        eGUILD_RIGHTS_CHANGE_MASTER, static_cast<eGUILD_DUTY>(guild_render_info.m_GuildDuty));
    if (button_control != NULL)
    {
        GlobalFunc::EnableControl(button_control, have_right);
    }
}

void uiGuildMember::UpdateWheelScroll()
{
    CControlWZ* wheel_rect_control = GetControlWZ(StrToWzID("P006"));
    if (wheel_rect_control == NULL)
    {
        return;
    }

    RECT wheel_rect	= wheel_rect_control->GetSizeRect();
    switch (GetMouseWheelStatus(&wheel_rect))
    {
    case MOUSE_WHEEL_UP:
        OnVScrollUp(NULL);
        break; 
    case MOUSE_WHEEL_DOWN:
        OnVScrollDown(NULL);
        break;
    }
}

void uiGuildMember::OnClickInvateGuildMember()
{
    uiGuildRelationAsk* guild_ask = GET_CAST_DIALOG(uiGuildRelationAsk, uiGuildRelationAsk::kDialog_ID);
    if (guild_ask != NULL)
    {
        guild_ask->set_guild_ask_type(uiGuildRelationAsk::kGuild_Invite);
        guild_ask->ShowInterface(TRUE);
    }
}

void uiGuildMember::OnClickDutyVest()
{
    GUILD_MEMBER_INFO* member_info = GetSelectedMemberInfo();
    if (member_info == NULL)
    {
        return;
    }

    uiGuildDutyVest* ui_guild_duty_vest = GET_CAST_DIALOG(uiGuildDutyVest, uiGuildDutyVest::kDialog_ID);
    if (ui_guild_duty_vest != NULL)
    {
        ui_guild_duty_vest->ShowInterface(TRUE);
    }
}

GUILD_MEMBER_INFO* uiGuildMember::GetSelectedMemberInfo()
{
    int list_index = -1;

    CCtrlListWZ* member_list = 
        static_cast<CCtrlListWZ*>(GetControlWZ_byPosition(kControl_List_L000));
    if (member_list != NULL)
    {
        list_index = member_list->GetCurSel();
    }

    size_t member_index = (size_t)(member_list->GetItemData(list_index));
    
    GuildSystem::GuildSystemData& guild_system_data = GuildSystem::GetGuildSystemData();
    if ((member_index < 0) || 
        (static_cast<size_t>(member_index) >= guild_system_data.guild_member_container().size()))
    {
        return NULL;
    }

    GUILD_MEMBER_INFO& member_info = guild_system_data.guild_member_container()[member_index];

    return &member_info;
}

void uiGuildMember::RefreshTab()
{
    CCtrlButtonCheckWZ* check_button = NULL;
    for (int position = kControl_CheckBT_C000; position < (kControl_CheckBT_C002 + 1); ++position)
    {
        check_button = 
            static_cast<CCtrlButtonCheckWZ*>(GetControlWZ(GetControlID_byPosition(position)));
        if (check_button == NULL)
        {
            continue;
        }
        check_button->SetCheckState((position == kControl_CheckBT_C001) ? (true) : (false));
    }
}

#endif //_NA_003923_20120222_GUILD_RENEWAL_UI_RENEWAL