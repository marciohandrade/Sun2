#include "SunClientPrecompiledHeader.h"

#ifdef _NA_006826_20130722_DOMINATION_RENEWAL
#else

#include "uiDominationJoinList1.h"
#include "uiDominationTowerMan/uiDominationTowerMan.h"
#include "uiSystemMan/uiSystemMan.h"
#include "MapInfoParser.h"
#include "Hero.h"

namespace
{
    // 아군 클래스 리스트 ID
    const WzID kFriendClassImageID[MAX_DOMINATION_MEMBER] = 
    {
        StrToWzID("P001"),
        StrToWzID("P003"),
        StrToWzID("P005"),
        StrToWzID("P006"),
        StrToWzID("P009"),
        StrToWzID("P010"),
        StrToWzID("P011"),
        StrToWzID("P012"),
        StrToWzID("P017"),
        StrToWzID("P018"),
        StrToWzID("P019"),
        StrToWzID("P020"),
        StrToWzID("P025"),
        StrToWzID("P026"),
        StrToWzID("P027"),
        StrToWzID("P028"),
        StrToWzID("P033"),
        StrToWzID("P034"),
        StrToWzID("P035"),
        StrToWzID("P036"),
    };

    // 아군 레벨 리스트 ID
    const WzID kFriendLevelID[MAX_DOMINATION_MEMBER] = 
    {
        StrToWzID("S002"),
        StrToWzID("S006"),
        StrToWzID("S010"),
        StrToWzID("S012"),
        StrToWzID("S018"),
        StrToWzID("S020"),
        StrToWzID("S022"),
        StrToWzID("S024"),
        StrToWzID("S034"),
        StrToWzID("S036"),
        StrToWzID("S038"),
        StrToWzID("S040"),
        StrToWzID("S050"),
        StrToWzID("S052"),
        StrToWzID("S054"),
        StrToWzID("S056"),
        StrToWzID("S066"),
        StrToWzID("S068"),
        StrToWzID("S070"),
        StrToWzID("S072"),
    };

    // 아군 참가 캐릭터 이름 리스트 ID
    const WzID kFriendMemberNameID[MAX_DOMINATION_MEMBER] = 
    {
        StrToWzID("S003"),
        StrToWzID("S007"),
        StrToWzID("S011"),
        StrToWzID("S013"),
        StrToWzID("S019"),
        StrToWzID("S021"),
        StrToWzID("S023"),
        StrToWzID("S025"),
        StrToWzID("S035"),
        StrToWzID("S037"),
        StrToWzID("S039"),
        StrToWzID("S041"),
        StrToWzID("S051"),
        StrToWzID("S053"),
        StrToWzID("S055"),
        StrToWzID("S057"),
        StrToWzID("S067"),
        StrToWzID("S069"),
        StrToWzID("S071"),
        StrToWzID("S073"),
    };

    // 적군 클래스 리스트 ID
    const WzID kEnemyClassImageID[MAX_DOMINATION_MEMBER] = 
    {
        StrToWzID("P002"),
        StrToWzID("P004"),
        StrToWzID("P007"),
        StrToWzID("P008"),
        StrToWzID("P013"),
        StrToWzID("P014"),
        StrToWzID("P015"),
        StrToWzID("P016"),
        StrToWzID("P021"),
        StrToWzID("P022"),
        StrToWzID("P023"),
        StrToWzID("P024"),
        StrToWzID("P029"),
        StrToWzID("P030"),
        StrToWzID("P031"),
        StrToWzID("P032"),
        StrToWzID("P037"),
        StrToWzID("P038"),
        StrToWzID("P039"),
        StrToWzID("P040"),
    };

    // 적군 레벨 리스트 ID
    const WzID kEnemyLevelID[MAX_DOMINATION_MEMBER] = 
    {
        StrToWzID("S004"),
        StrToWzID("S008"),
        StrToWzID("S014"),
        StrToWzID("S016"),
        StrToWzID("S026"),
        StrToWzID("S028"),
        StrToWzID("S030"),
        StrToWzID("S032"),
        StrToWzID("S042"),
        StrToWzID("S044"),
        StrToWzID("S046"),
        StrToWzID("S048"),
        StrToWzID("S058"),
        StrToWzID("S060"),
        StrToWzID("S062"),
        StrToWzID("S064"),
        StrToWzID("S074"),
        StrToWzID("S076"),
        StrToWzID("S078"),
        StrToWzID("S080"),
    };

    // 적군 참가 캐릭터 이름 리스트 ID
    const WzID kEnemyMemberNameID[MAX_DOMINATION_MEMBER] = 
    {
        StrToWzID("S005"),
        StrToWzID("S009"),
        StrToWzID("S015"),
        StrToWzID("S017"),
        StrToWzID("S027"),
        StrToWzID("S029"),
        StrToWzID("S031"),
        StrToWzID("S033"),
        StrToWzID("S043"),
        StrToWzID("S045"),
        StrToWzID("S047"),
        StrToWzID("S049"),
        StrToWzID("S059"),
        StrToWzID("S061"),
        StrToWzID("S063"),
        StrToWzID("S065"),
        StrToWzID("S075"),
        StrToWzID("S077"),
        StrToWzID("S079"),
        StrToWzID("S081"),
    };

    // 참가 취소 버튼 ID
    const WzID kMemberLeaveButtonID[MAX_DOMINATION_MEMBER] = 
    {
        StrToWzID("B007"),
        StrToWzID("B009"),
        StrToWzID("B011"),
        StrToWzID("B010"),
        StrToWzID("B012"),
        StrToWzID("B013"),
        StrToWzID("B014"),
        StrToWzID("B015"),
        StrToWzID("B016"),
        StrToWzID("B017"),
        StrToWzID("B018"),
        StrToWzID("B019"),
        StrToWzID("B020"),
        StrToWzID("B021"),
        StrToWzID("B022"),
        StrToWzID("B023"),
        StrToWzID("B024"),
        StrToWzID("B025"),
        StrToWzID("B026"),
        StrToWzID("B027"),
    };
}

uiDominationJoinList1::uiDominationJoinList1(InterfaceManager* ui_manager):
uiBase(ui_manager)
{

}

uiDominationJoinList1::~uiDominationJoinList1()
{

}

void uiDominationJoinList1::Init(CDrawBase* draw_base)
{
    uiBase::Init(draw_base);

    RegisterControl();
}

void uiDominationJoinList1::RegisterControl()
{
    RegisterControlPosition(StrToWzID("T999"), kControl_Static_T999); // 타이틀
    RegisterControlPosition(StrToWzID("B031"), kControl_Button_B031); // 새로고침 버튼
    RegisterControlPosition(StrToWzID("B999"), kControl_Button_B999); // 닫기 버튼

    RegisterControlPosition(StrToWzID("BT01"), kControl_Button_BT01); // 아군 클래스 버튼
    RegisterControlPosition(StrToWzID("B001"), kControl_Button_B001); // 아군 레벨 버튼
    RegisterControlPosition(StrToWzID("B003"), kControl_Button_B003); // 아군 캐릭터명 버튼

    RegisterControlPosition(StrToWzID("B000"), kControl_Button_B000); // 적군 클래스 버튼
    RegisterControlPosition(StrToWzID("B002"), kControl_Button_B002); // 적군 레벨 버튼
    RegisterControlPosition(StrToWzID("B004"), kControl_Button_B004); // 적군 캐릭터명 버튼

    // 아군 클래스 리스트
    for (int index = 0; index < MAX_DOMINATION_MEMBER; ++index)
    {
        RegisterControlPosition(kFriendClassImageID[index], kControl_Friend_Class_Image_Start + index);
    }

    // 아군 레벨 리스트
    for (index = 0; index < MAX_DOMINATION_MEMBER; ++index)
    {
        RegisterControlPosition(kFriendLevelID[index], kControl_Friend_Level_Start + index);
    }

    // 아군 참가 캐릭터 이름 리스트
    for (index = 0; index < MAX_DOMINATION_MEMBER; ++index)
    {
        RegisterControlPosition(kFriendMemberNameID[index], kControl_Friend_Member_Name_Start + index);
    }
    
    // 적군 클래스 리스트
    for (int index = 0; index < MAX_DOMINATION_MEMBER; ++index)
    {
        RegisterControlPosition(kEnemyClassImageID[index], kControl_Enemy_Class_Image_Start + index);
    }

    // 적군 레벨 리스트
    for (index = 0; index < MAX_DOMINATION_MEMBER; ++index)
    {
        RegisterControlPosition(kEnemyLevelID[index], kControl_Enemy_Level_Start + index);
    }

    // 적군 참가 캐릭터 이름 리스트
    for (index = 0; index < MAX_DOMINATION_MEMBER; ++index)
    {
        RegisterControlPosition(kEnemyMemberNameID[index], kControl_Enemy_Member_Name_Start + index);
    }

    // 참가 취소 버튼
    for (index = 0; index < MAX_DOMINATION_MEMBER; ++index)
    {
        RegisterControlPosition(kMemberLeaveButtonID[index], kControl_Member_Leave_Button_Start + index);
    }
}

void uiDominationJoinList1::Release()
{

}

void uiDominationJoinList1::MessageProc( SI_MESSAGE * message )
{
    if (IsLock())
        return;

    static bool is_double_click = false;
    switch (message->eResultMsg)
    {
    case RT_MSG_LBUTTONCLICK:
        {
            if (is_double_click == true)
            {
                is_double_click = false;
            }
            else
            {
                OnLButtonClick(message);
            }            
        }        
        break;
    case RT_MSG_LBUTTONDCLICK:
        {
            OnLButtonDClick(message);
            is_double_click = true;
        }        
        break;
    default:
        SolarDialog::MessageProc(message);
        break;
    }
}

void uiDominationJoinList1::OnShowWindow(BOOL val)
{
    if (val)
    {
        MoveDefaultWindowPos();

        uiBase* domination_main_dialog = GET_DIALOG(IM_DOMINATION_TOWER_MANAGER::DOMINATIONWAR_TOWER_MAIN_DIALOG);
        if (domination_main_dialog != NULL)
        {
            RECT dialog_rect;
            float position_x = GetDialogWZ()->GetDlgX();
            float position_y = GetDialogWZ()->GetDlgY();
            domination_main_dialog->GetDialogWZ()->GetDlgSize(&dialog_rect);
            position_x -= static_cast<float>(dialog_rect.right);
            domination_main_dialog->GetDialogWZ()->MoveWindowWZ(position_x, position_y);
        }
    }
    else
    {
        uiBase* domination_main_dialog = GET_DIALOG(IM_DOMINATION_TOWER_MANAGER::DOMINATIONWAR_TOWER_MAIN_DIALOG);
        if ((domination_main_dialog != NULL) && 
            (domination_main_dialog->IsVisible() == TRUE))
        {
            domination_main_dialog->ShowInterface(FALSE);
        }
    }
}


bool uiDominationJoinList1::OnLButtonClick(SI_MESSAGE* message)
{
    bool result = true;
    DWORD control_position = GetControlPosition_byID(message->dwCtrlID);

    if ((control_position >= kControl_Member_Leave_Button_Start) && 
        (control_position <= kControl_Member_Leave_Button_End))
    {
        OnClickMemberLeaveButton(control_position);
        return result;
    }

    switch (control_position)
    {
    case kControl_Button_B031:
        {
            OnClickRefreshListButton();
        }
        break;
    case kControl_Button_B999:
        {
            OnClickCloseButton();
        }
        break;
    default:
        {
            
        }
        break;
    }
    return result;
}

bool uiDominationJoinList1::OnLButtonDClick( SI_MESSAGE* message )
{
    bool result = true;

    uiDominationTowerMan* ui_domination_manager = GET_DIALOG_MANAGER(uiDominationTowerMan, UIMAN_DOMINATIONWAR_TOWER);
    if (ui_domination_manager == NULL)
    {
        return result;
    }

    DWORD control_position = GetControlPosition_byID(message->dwCtrlID);
    switch (control_position)
    {
    case kControl_Button_BT01:
        {
            uiDominationTowerMan::MemberListOrder list_oder = ui_domination_manager->friend_member_list_order();
            if (list_oder == uiDominationTowerMan::kListOrderClassLess)
            {
                list_oder = uiDominationTowerMan::kListOrderClassGreater;
            }
            else
            {
                list_oder = uiDominationTowerMan::kListOrderClassLess;
            }
            ui_domination_manager->set_friend_member_list_order(list_oder);
            SortMemberList();
            SetMemberList();
        }
        break;
    case kControl_Button_B001:
        {
            uiDominationTowerMan::MemberListOrder list_oder = ui_domination_manager->friend_member_list_order();
            if (list_oder == uiDominationTowerMan::kListOrderLevelGreater)
            {
                list_oder = uiDominationTowerMan::kListOrderLevelLess;
            }
            else
            {
                list_oder = uiDominationTowerMan::kListOrderLevelGreater;
            }
            ui_domination_manager->set_friend_member_list_order(list_oder);
            SortMemberList();
            SetMemberList();
        }
        break;
    case kControl_Button_B003:
        {
            uiDominationTowerMan::MemberListOrder list_oder = ui_domination_manager->friend_member_list_order();
            if (list_oder == uiDominationTowerMan::kListOrderNameLess)
            {
                list_oder = uiDominationTowerMan::kListOrderNameGreater;
            }
            else
            {
                list_oder = uiDominationTowerMan::kListOrderNameLess;
            }
            ui_domination_manager->set_friend_member_list_order(list_oder);
            SortMemberList();
            SetMemberList();
        }
        break;
    case kControl_Button_B000:
        {
            uiDominationTowerMan::MemberListOrder list_oder = ui_domination_manager->enemy_member_list_order();
            if (list_oder == uiDominationTowerMan::kListOrderClassLess)
            {
                list_oder = uiDominationTowerMan::kListOrderClassGreater;
            }
            else
            {
                list_oder = uiDominationTowerMan::kListOrderClassLess;
            }
            ui_domination_manager->set_enemy_member_list_order(list_oder);
            SortMemberList();
            SetMemberList();
        }
        break;
    case kControl_Button_B002:
        {
            uiDominationTowerMan::MemberListOrder list_oder = ui_domination_manager->enemy_member_list_order();
            if (list_oder == uiDominationTowerMan::kListOrderLevelGreater)
            {
                list_oder = uiDominationTowerMan::kListOrderLevelLess;
            }
            else
            {
                list_oder = uiDominationTowerMan::kListOrderLevelGreater;
            }
            ui_domination_manager->set_enemy_member_list_order(list_oder);
            SortMemberList();
            SetMemberList();
        }
        break;
    case kControl_Button_B004:
        {
            uiDominationTowerMan::MemberListOrder list_oder = ui_domination_manager->enemy_member_list_order();
            if (list_oder == uiDominationTowerMan::kListOrderNameLess)
            {
                list_oder = uiDominationTowerMan::kListOrderNameGreater;
            }
            else
            {
                list_oder = uiDominationTowerMan::kListOrderNameLess;
            }
            ui_domination_manager->set_enemy_member_list_order(list_oder);
            SortMemberList();
            SetMemberList();
        }
        break;
    default:
        break;
    }
    return result;
}

void uiDominationJoinList1::OnClickMemberLeaveButton(DWORD control_position)
{
    uiDominationTowerMan* ui_domination_manager = GET_DIALOG_MANAGER(uiDominationTowerMan, UIMAN_DOMINATIONWAR_TOWER);
    if (ui_domination_manager == NULL)
    {
        return;
    }

    DWORD index = control_position - kControl_Member_Leave_Button_Start;
    if (index >= friend_member_list_.size())
    {
        return;
    }

    uiSystemMan* ui_system_manager = GET_DIALOG_MANAGER(uiSystemMan, UIMAN_SYSTEM);
    if (ui_system_manager == NULL)
    {
        return;
    }

    DominationMemberInfo& member_info = (friend_member_list_[index]);
    ui_domination_manager->set_leave_member_info(member_info);
    
    TCHAR comfirm_message[INTERFACE_STRING_LENGTH] = {'\0', };
    if (Strcmp(member_info.member_name_, g_pHero->GetCharInfo()->m_tszCharName) == 0)
    //if (member_info.member_guid_ == g_pHero->GetCharInfo()->m_CharGuid)
    {
        // 3458 해당 지역의 출전 신청을 취소 하시겠습니까?
        g_InterfaceManager.GetInterfaceString(3458, comfirm_message);
    }
    else
    {
        // 3457 해당 길드원을 피의성전 출전 신청 목록에서 제외하겠습니까?
        g_InterfaceManager.GetInterfaceString(3457, comfirm_message);
    }
    ui_system_manager->Sys_ConfirmLock(comfirm_message, uiDominationTowerMan::ComfirmDominationLeave);
}

void uiDominationJoinList1::OnClickRefreshListButton()
{
    uiDominationTowerMan* ui_domination_manager = GET_DIALOG_MANAGER(uiDominationTowerMan, UIMAN_DOMINATIONWAR_TOWER);
    if (ui_domination_manager == NULL)
    {
        return;
    }

    MAPCODE map_code = ui_domination_manager->GetDominationManagedMapCode();
    ui_domination_manager->SEND_DOMINATION_REFRESH_MEMBERLIST_REQ(map_code);
}

void uiDominationJoinList1::OnClickCloseButton()
{
    ShowInterface(FALSE);
}

void uiDominationJoinList1::SetDominationJoinList(MSG_CG_DOMINATION_MEMBERLIST_NTF* recv_packet)
{
    uiDominationTowerMan* ui_domination_manager = GET_DIALOG_MANAGER(uiDominationTowerMan, UIMAN_DOMINATIONWAR_TOWER);
    if (ui_domination_manager == NULL)
    {
        return;
    }

    is_enable_leave_member_ = recv_packet->is_delete_member_;

    // 자신의 길드 아군, 상대 길드 적군
    // 공/수 길드가 아니면 수비가 아군, 공격이 적군
    GUILDGUID domination_guild_guid = ui_domination_manager->GetDominationGuildInfo()->m_GuildGuid;
    GUILDGUID challenger_guild_guid = ui_domination_manager->GetChallengerGuildInfo()->m_GuildGuid;
    GUILDGUID my_guild_guid = g_pHero->GetPlayerGuildID();
    
    bool friend_flag = (challenger_guild_guid == my_guild_guid);

    friend_member_list_.clear();
    enemy_member_list_.clear();

    int data_count = recv_packet->data_count_;
    for (int index = 0; index < data_count; ++index)
    {
        DominationMemberInfo* member_info = &(recv_packet->member_list_[index]);
        if (member_info->guild_type_ == friend_flag)
        {
            // 아군 멤버
            friend_member_list_.push_back((*member_info));
        }
        else
        {
            // 적군 멤버
            enemy_member_list_.push_back((*member_info));
            
        }
    }

    SortMemberList();

    SetMemberList();

    SetDialogTitle();
}

DominationMemberInfo* uiDominationJoinList1::GetDominationMemberInfo(CHARGUID char_guid)
{
    DominationMemberInfo* found_member_info = NULL;
    std::vector<DominationMemberInfo>::iterator member_info_itr = friend_member_list_.begin();
    std::vector<DominationMemberInfo>::iterator member_info_itr_end = friend_member_list_.end();
    for ( ; member_info_itr != member_info_itr_end; ++member_info_itr)
    {
        if (member_info_itr->member_guid_ == char_guid)
        {
            found_member_info = &(*member_info_itr);
            break;
        }
    }
    return found_member_info;
}

void uiDominationJoinList1::SetDialogTitle()
{
    CCtrlStaticWZ* title_control = static_cast<CCtrlStaticWZ*>(GetControlWZ_byPosition(kControl_Static_T999));
    if (title_control == NULL)
    {
        return;
    }

    uiDominationTowerMan* ui_domination_manager = GET_DIALOG_MANAGER(uiDominationTowerMan, UIMAN_DOMINATIONWAR_TOWER);
    if (ui_domination_manager == NULL)
    {
        return;
    }

    MAPCODE map_code = ui_domination_manager->GetDominationManagedMapCode();
    const sMAPINFO* map_info = MapInfoParser::Instance()->FindMapInfo(map_code);
    if (map_info == NULL)
    {
        return;
    }

    TCHAR title_string[INTERFACE_STRING_LENGTH] = {'\0', };
    TCHAR map_name_string[INTERFACE_STRING_LENGTH] = {'\0', };
    TCHAR temp_string[INTERFACE_STRING_LENGTH] = {'\0', };
    g_InterfaceManager.GetInterfaceString(map_info->dwNCode, map_name_string);
    // 3462 %s 출전 리스트
    g_InterfaceManager.GetInterfaceString(3462, temp_string);
    Snprintf(title_string, INTERFACE_STRING_LENGTH - 1, temp_string, map_name_string);
    title_control->SetTextWZ(title_string);
}

void uiDominationJoinList1::SortMemberList()
{
    uiDominationTowerMan* ui_domination_manager = GET_DIALOG_MANAGER(uiDominationTowerMan, UIMAN_DOMINATIONWAR_TOWER);
    if (ui_domination_manager == NULL)
    {
        return;
    }

    uiDominationTowerMan::MemberListOrder list_order = ui_domination_manager->friend_member_list_order();
    switch (list_order)
    {
    case uiDominationTowerMan::kListOrderClassLess:
        {
            std::sort(friend_member_list_.begin(), 
                friend_member_list_.end(), 
                uiDominationTowerMan::SortMemberListByClassLess);
        }
        break;
    case uiDominationTowerMan::kListOrderClassGreater:
        {
            std::sort(friend_member_list_.begin(), 
                friend_member_list_.end(), 
                uiDominationTowerMan::SortMemberListByClassGreater);
        }
        break;
    case uiDominationTowerMan::kListOrderLevelLess:
        {
            std::sort(friend_member_list_.begin(), 
                friend_member_list_.end(), 
                uiDominationTowerMan::SortMemberListByLevelLess);
        }
        break;
    case uiDominationTowerMan::kListOrderLevelGreater:
        {
            std::sort(friend_member_list_.begin(), 
                friend_member_list_.end(), 
                uiDominationTowerMan::SortMemberListByLevelGreater);
        }
        break;
    case uiDominationTowerMan::kListOrderNameLess:
        {
            std::sort(friend_member_list_.begin(), 
                friend_member_list_.end(), 
                uiDominationTowerMan::SortMemberListByNameLess);
        }
        break;
    case uiDominationTowerMan::kListOrderNameGreater:
        {
            std::sort(friend_member_list_.begin(), 
                friend_member_list_.end(), 
                uiDominationTowerMan::SortMemberListByNameGreater);
        }
        break;
    }

    list_order = ui_domination_manager->enemy_member_list_order();
    switch (list_order)
    {
    case uiDominationTowerMan::kListOrderClassLess:
        {
            std::sort(enemy_member_list_.begin(), 
                enemy_member_list_.end(), 
                uiDominationTowerMan::SortMemberListByClassLess);
        }
        break;
    case uiDominationTowerMan::kListOrderClassGreater:
        {
            std::sort(enemy_member_list_.begin(), 
                enemy_member_list_.end(), 
                uiDominationTowerMan::SortMemberListByClassGreater);
        }
        break;
    case uiDominationTowerMan::kListOrderLevelLess:
        {
            std::sort(enemy_member_list_.begin(), 
                enemy_member_list_.end(), 
                uiDominationTowerMan::SortMemberListByLevelLess);
        }
        break;
    case uiDominationTowerMan::kListOrderLevelGreater:
        {
            std::sort(enemy_member_list_.begin(), 
                enemy_member_list_.end(), 
                uiDominationTowerMan::SortMemberListByLevelGreater);
        }
        break;
    case uiDominationTowerMan::kListOrderNameLess:
        {
            std::sort(enemy_member_list_.begin(), 
                enemy_member_list_.end(), 
                uiDominationTowerMan::SortMemberListByNameLess);
        }
        break;
    case uiDominationTowerMan::kListOrderNameGreater:
        {
            std::sort(enemy_member_list_.begin(), 
                enemy_member_list_.end(), 
                uiDominationTowerMan::SortMemberListByNameGreater);
        }
        break;
    }
}

void uiDominationJoinList1::SetMemberList()
{
    uiDominationTowerMan* ui_domination_manager = GET_DIALOG_MANAGER(uiDominationTowerMan, UIMAN_DOMINATIONWAR_TOWER);
    if (ui_domination_manager == NULL)
    {
        return;
    }

    GUILDGUID domination_guild_guid = ui_domination_manager->GetDominationGuildInfo()->m_GuildGuid;
    GUILDGUID challenger_guild_guid = ui_domination_manager->GetChallengerGuildInfo()->m_GuildGuid;
    PLAYER_GUILD_RENDER_INFO& guild_info = g_pHero->GetGuildRenderInfo();
    GUILDGUID my_guild_guid = guild_info.m_GuildGuid;
    BASE_PLAYERINFO* player_info = g_pHero->GetCharInfo();

    bool is_friend = (domination_guild_guid == my_guild_guid) || (challenger_guild_guid == my_guild_guid);
    bool is_guild_marster = (guild_info.m_GuildDuty == eGUILD_DUTY_MASTER);

    // 아군 멤버 리스트 출력
    std::vector<DominationMemberInfo>::iterator member_list_itr = friend_member_list_.begin();
    std::vector<DominationMemberInfo>::iterator member_list_itr_end = friend_member_list_.end();
    for (int index = 0; index < MAX_DOMINATION_MEMBER; ++index)
    {
        ShowControl(kControl_Friend_Class_Image_Start + index, false);
        ShowControl(kControl_Friend_Level_Start + index, false);
        ShowControl(kControl_Friend_Member_Name_Start + index, false);
        ShowControl(kControl_Member_Leave_Button_Start + index, false);

        if (member_list_itr != member_list_itr_end)
        {
            // 참가 멤버 정보 세팅
            SetClassImage(kControl_Friend_Class_Image_Start + index, member_list_itr->member_class_);
            SetMemberLevel(kControl_Friend_Level_Start + index, member_list_itr->member_level_);
            SetMemberName(kControl_Friend_Member_Name_Start + index, member_list_itr->member_name_);

            // 참가 제외 버튼 세팅
            if ((is_friend == true) && 
                (is_enable_leave_member_ == true))
            {
                if ((is_guild_marster == true) || 
                    (Strcmp(member_list_itr->member_name_, g_pHero->GetCharInfo()->m_tszCharName) == 0))
                {
                    ShowControl(kControl_Member_Leave_Button_Start + index, true);
                }
            }
            ++member_list_itr;
        }
    }

    // 적군 멤버 리스트 출력
    member_list_itr = enemy_member_list_.begin();
    member_list_itr_end = enemy_member_list_.end();
    for (index = 0; index < MAX_DOMINATION_MEMBER; ++index)
    {
        if (member_list_itr != member_list_itr_end)
        {
            // 참가 멤버 정보 세팅
            SetClassImage(kControl_Enemy_Class_Image_Start + index, member_list_itr->member_class_);
            SetMemberLevel(kControl_Enemy_Level_Start + index, member_list_itr->member_level_);
            SetMemberName(kControl_Enemy_Member_Name_Start + index, member_list_itr->member_name_);

            ++member_list_itr;
        }
        else
        {
            ShowControl(kControl_Enemy_Class_Image_Start + index, false);
            ShowControl(kControl_Enemy_Level_Start + index, false);
            ShowControl(kControl_Enemy_Member_Name_Start + index, false);
        }
    }
}

void uiDominationJoinList1::SetClassImage(DWORD control_position, BYTE member_class)
{
    CCtrlPictureWZ* class_image_control = static_cast<CCtrlPictureWZ*>(GetControlWZ_byPosition(control_position));
    if (class_image_control == NULL)
    {
        return;
    }

    class_image_control->ShowWindowWZ(WZ_SHOW);

    // 클래스 이미지 세팅
    const int kClassImageSize = 15;
    if (member_class == 3)
    {
        member_class = 4;
    }
    else if (member_class == 4)
    {
        member_class = 3;
    }
    int image_offset = (member_class - 1) * kClassImageSize;
    SetControlImageFilename(class_image_control->GetCtrlID(), "Data\\Interface\\all_character.tga", 201 + image_offset, 0, 15, 15);
}

void uiDominationJoinList1::SetMemberLevel(DWORD control_position, LEVELTYPE member_level)
{
    CCtrlStaticWZ* level_control = static_cast<CCtrlStaticWZ*>(GetControlWZ_byPosition(control_position));
    if (level_control == NULL)
    {
        return;
    }

    level_control->ShowWindowWZ(WZ_SHOW);

    TCHAR level_string[INTERFACE_STRING_LENGTH] = {'\0', };
    Snprintf(level_string, INTERFACE_STRING_LENGTH - 1, "%d", member_level);
    level_control->SetTextWZ(level_string);
}

void uiDominationJoinList1::SetMemberName(DWORD control_position, TCHAR* member_name)
{
    CCtrlStaticWZ* member_name_control = static_cast<CCtrlStaticWZ*>(GetControlWZ_byPosition(control_position));
    if ((member_name_control == NULL) || 
        (member_name == NULL))
    {
        return;
    }
    member_name_control->ShowWindowWZ(WZ_SHOW);

    member_name_control->SetTextWZ(member_name);
}

void uiDominationJoinList1::ShowControl(DWORD control_position, bool is_show)
{
    CControlWZ* control_ptr = GetControlWZ_byPosition(control_position);
    if (control_ptr != NULL)
    {
        ENUM_STATEWZ state = (is_show == true ? WZ_SHOW : WZ_HIDE);
        control_ptr->ShowWindowWZ(state);
    }
}

#endif //_NA_006826_20130722_DOMINATION_RENEWAL
