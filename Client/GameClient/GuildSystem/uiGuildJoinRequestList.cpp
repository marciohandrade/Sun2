#include "SunClientPrecompiledHeader.h"
#ifdef _NA_007330_20140620_GUILD_SYSTEM_EXTENSION
#include ".\uiguildjoinrequestlist.h"
#include "GlobalFunc.h"
#include "GuildSystem/GuildSystemData.h"
#include "GuildSystem/GuildSystemPacketFunction.h"
#include "uiCommunityMan/uiCommunityMan.h"
#include "uiCommunityInfo/uiCommunityInfo.h"
#include "GuildRightInfoParser.h"
#include "Hero.h"

const DWORD uiGuildJoinRequestList::kDialog_ID = StrToWzID("7348");

uiGuildJoinRequestList::uiGuildJoinRequestList(InterfaceManager* ui_manager_ptr)
: uiBase(ui_manager_ptr)
{
    set_current_page(0);
}

uiGuildJoinRequestList::~uiGuildJoinRequestList(void)
{
    Release();
}

void uiGuildJoinRequestList::Init( CDrawBase* draw_base )
{
    uiBase::Init(draw_base);

    RegisterControlPosition(StrToWzID("B999"), kControl_Button_B999);
    RegisterControlPosition(StrToWzID("BT00"), kControl_Button_BT00);
    RegisterControlPosition(StrToWzID("BT02"), kControl_Button_BT02);
    RegisterControlPosition(StrToWzID("BT01"), kControl_Button_BT01);
    RegisterControlPosition(StrToWzID("B003"), kControl_Button_B003);
    RegisterControlPosition(StrToWzID("B000"), kControl_Button_B000);
    RegisterControlPosition(StrToWzID("L001"), kControl_List_L001);
    RegisterControlPosition(StrToWzID("B001"), kControl_Button_B001);
    RegisterControlPosition(StrToWzID("B002"), kControl_Button_B002);
    RegisterControlPosition(StrToWzID("B010"), kControl_Button_B010);
    RegisterControlPosition(StrToWzID("B011"), kControl_Button_B011);
    RegisterControlPosition(StrToWzID("B012"), kControl_Button_B012);
    RegisterControlPosition(StrToWzID("B013"), kControl_Button_B013);
    RegisterControlPosition(StrToWzID("B014"), kControl_Button_B014);
    RegisterControlPosition(StrToWzID("B015"), kControl_Button_B015);
    RegisterControlPosition(StrToWzID("B016"), kControl_Button_B016);
    RegisterControlPosition(StrToWzID("B017"), kControl_Button_B017);
    RegisterControlPosition(StrToWzID("B018"), kControl_Button_B018);
    RegisterControlPosition(StrToWzID("B019"), kControl_Button_B019);

    RegisterControlPosition(StrToWzID("B023"), kControl_Button_B023);
    RegisterControlPosition(StrToWzID("B024"), kControl_Button_B024);

    RegisterControlPosition(StrToWzID("S000"), kControl_Text_S000);

    CCtrlListWZ* join_request_list = 
        static_cast<CCtrlListWZ*>(GetControlWZ_byPosition(kControl_List_L001));
    if (join_request_list)
    {
        join_request_list->SetUseReport(true);
        join_request_list->SetSellHeight(25);
        join_request_list->SetStartLineGapTop(5);
        join_request_list->SetColorSelBar(RGBA(150, 10, 10, 125));
        join_request_list->SetReportUseCnt(4);
        join_request_list->SetHideReportRender(TRUE);
        join_request_list->SetReportButtonInfo(0, 235, NULL); // 캐릭터명
        join_request_list->SetReportButtonInfo(1, 91, NULL); // 레벨
        join_request_list->SetReportButtonInfo(2, 123, NULL); // 클래스
        join_request_list->SetReportButtonInfo(3, 254 / 2, NULL); // 대기 시간
        join_request_list->SetDiscardMessage(true);
    }
    CCtrlStaticWZ* page_text = 
        static_cast<CCtrlStaticWZ*>(GetControlWZ_byPosition(kControl_Text_S000));
    if (page_text)
    {
        page_text->SetFontID(StrToWzID("st11"));
    }
    for (int i = kControl_Button_BT00; i <= kControl_Button_B003; ++i)
    {
        CCtrlButtonWZ* control_ptr = 
            static_cast<CCtrlButtonWZ*>(GetControlWZ_byPosition(i));
        if (control_ptr)
        {
            control_ptr->SetDiscardMessage(true);
        }
    }
    for (int i = 0; i < kMaxListLine; ++i)
    {
        CCtrlButtonWZ* control_ptr = 
            static_cast<CCtrlButtonWZ*>(GetControlWZ_byPosition(kControl_Button_B001 + i * 2));
        if (control_ptr)
        {
            accept_rejection_control[i].accept_control = control_ptr;
        }        
        control_ptr = 
            static_cast<CCtrlButtonWZ*>(GetControlWZ_byPosition(kControl_Button_B001 + (i * 2) + 1));
        if (control_ptr)
        {
            accept_rejection_control[i].rejection_control = control_ptr;
        }
        accept_rejection_control[i].guild_container_index = -1;
    }
}

void uiGuildJoinRequestList::Release()
{
    for (int i = 0; i < kMaxListLine; ++i)
    {        
        accept_rejection_control[i].accept_control = NULL;
        accept_rejection_control[i].rejection_control = NULL;
        accept_rejection_control[i].guild_container_index = -1;
    }
}

void uiGuildJoinRequestList::MessageProc( SI_MESSAGE* message )
{
    if (IsLock())
        return;

    if (GetMan() == NULL)
        return;

    switch (message->eResultMsg)
    {
    case RT_MSG_LBUTTONCLICK:
        {
            OnLButtonClick(message);
        }
        break;
    case RT_MSG_LBUTTONDCLICK:
        {
            OnLButtonDClick(message);
        }
        break;
    default:
        {
            SolarDialog::MessageProc(message);
        }
        break;
    }
}

void uiGuildJoinRequestList::OnShowWindow( BOOL is_show )
{
    if (is_show)
    {
        set_current_page(0);
        UpdateJoinRequestList();

        keyMsg msg;
        ZeroMemory(&msg, sizeof(msg));
        msg.dwType = uiGuildJoinRequestList::kDialog_ID;
        msg.wParam = uiGuildJoinRequestList::kDialog_ID;
        msg.DoSomething = GlobalFunc::DoShowWindowEx;
        g_KeyQueueManager.PushBack(msg);
    }
    else
    {
        g_KeyQueueManager.DeleteMsg(uiGuildJoinRequestList::kDialog_ID);
    }
}

void uiGuildJoinRequestList::OnUpdateSolarDialog()
{
    UpdateRemainTime();    
}

bool uiGuildJoinRequestList::OnLButtonClick( SI_MESSAGE* message )
{
    bool result = true;
    DWORD control_position = GetControlPosition_byID(message->dwCtrlID);
    switch (control_position)
    {
    case kControl_Button_B000: // 닫기
    case kControl_Button_B999: // x 버튼
        {
            ShowInterface(FALSE);
        }
        break;
    case kControl_Button_B023: // 좌측 페이지 버튼
        {
            OnClickPrevPageButton();
        }
        break;
    case kControl_Button_B024: // 우측 페이지 버튼
        {
            OnClickNextPageButton();
        }
        break;
    case kControl_Button_B001: // 수락1
    case kControl_Button_B002: // 거절1
    case kControl_Button_B010: // 수락2
    case kControl_Button_B011: // 거절2
    case kControl_Button_B012: // 수락3
    case kControl_Button_B013: // 거절3
    case kControl_Button_B014: // 수락4
    case kControl_Button_B015: // 거절4
    case kControl_Button_B016: // 수락5
    case kControl_Button_B017: // 거절5
    case kControl_Button_B018: // 수락6
    case kControl_Button_B019: // 거절6
        {
            OnResultButtonClick(message);
        }
        break;
    }

    return result;
}

bool uiGuildJoinRequestList::OnLButtonDClick( SI_MESSAGE* message )
{
    bool result = true;
    DWORD control_position = GetControlPosition_byID(message->dwCtrlID);
    switch (control_position)
    {
    case kControl_List_L001:
        {
            ShowCharacterInfo();
        }
        break;
    }

    return result;
}


void uiGuildJoinRequestList::OnResultButtonClick( SI_MESSAGE* message )
{
    DWORD control_position = GetControlPosition_byID(message->dwCtrlID);    

    CCtrlButtonWZ* select_button =  
        static_cast<CCtrlButtonWZ*>(GetControlWZ_byPosition(control_position));

    int select_index = -1;
    bool is_accept_button = false;
    for (int i = 0; i < kMaxListLine; ++i)
    {
        if (accept_rejection_control[i].accept_control == select_button)
        {
            is_accept_button = true;
            select_index = i;
            break;
        }
        if (accept_rejection_control[i].rejection_control == select_button)
        {
            is_accept_button = false;
            select_index = i;
            break;
        }
    }

    if (select_index != -1)
    {
        GuildSystem::GuildSystemData::GuildJoinRequestInfoContainer& guild_join_request_info_container = 
            GuildSystem::GetGuildSystemData().guild_join_request_info_container();

        GUILD_JOIN_REQUEST_INFO& join_request_info = guild_join_request_info_container[select_index];

        if (is_accept_button) // 수락 버튼 클릭
        {
            /*
            CHARGUID    char_guid;
            TCHAR       char_name[MAX_CHARNAME_LENGTH + 1];
            BYTE        char_class;
            LEVELTYPE   char_level;
            */
            MSG_CG_GUILD_JOIN_REQUEST_APPROVE_SYN msg;
            msg.char_guid = join_request_info.m_char_guid;
            _tcsncpy(msg.char_name, join_request_info.m_char_name, _countof(join_request_info.m_char_name));
            msg.char_class = join_request_info.m_char_class;
            msg.char_level = join_request_info.m_char_level;
            GuildSystem::PacketFunction::SEND_CG_GUILD_JOIN_REQUEST_APPROVE_SYN(msg);
        }
        else // 거절 버튼 클릭
        {
            /*
            CHARGUID char_guid;
            */
            MSG_CG_GUILD_JOIN_REQUEST_REJECT_SYN msg;
            msg.char_guid = join_request_info.m_char_guid;
            GuildSystem::PacketFunction::SEND_CG_GUILD_JOIN_REQUEST_REJECT_SYN(msg);
        }
    }
}


void uiGuildJoinRequestList::OnClickPrevPageButton()
{
    int page = current_page();
    --page;
    page = max(0, page);
    set_current_page(page);

    UpdateJoinRequestList();
}

void uiGuildJoinRequestList::OnClickNextPageButton()
{
    GuildSystem::GuildSystemData::GuildJoinRequestInfoContainer& guild_join_request_info_container = 
        GuildSystem::GetGuildSystemData().guild_join_request_info_container();
    int num_of_info = static_cast<int>(guild_join_request_info_container.size());
    int page = current_page();
    ++page;

    if ((page * kInfoPerPage) < num_of_info)
    {
        page = min(kMaxPage, page);
        set_current_page(page);

        UpdateJoinRequestList();
    }    
}

void uiGuildJoinRequestList::UpdateJoinRequestList()
{
    CCtrlListWZ* guild_join_request_list = 
        static_cast<CCtrlListWZ*>(GetControlWZ_byPosition(kControl_List_L001));
    CCtrlStaticWZ* page_text = static_cast<CCtrlStaticWZ*>(GetControlWZ_byPosition(kControl_Text_S000));
    if (!guild_join_request_list)
        return;

    if (!page_text)
        return;
    int page = current_page();
    guild_join_request_list->DeleteItemAll();

    for (int i = 0; i < kMaxListLine; ++i)
    {
        SetAcceptAndRejectionButtonShow(i, WZ_HIDE, -1);
        SetAcceptAndRejectionButtonEnable(i, false, -1);
    }

    bool is_duty = IsAccessJoinRequestAcceptAndReject();

    TCHAR info_string[INTERFACE_STRING_LENGTH] = {0, };

    GuildSystem::GuildSystemData::GuildJoinRequestInfoContainer& guild_join_request_info_container = 
        GuildSystem::GetGuildSystemData().guild_join_request_info_container();
    int start_index = page * kInfoPerPage;
    int end_index = start_index + kInfoPerPage;
    int num_of_info = static_cast<int>(guild_join_request_info_container.size());
    end_index = min(end_index, num_of_info);

    // 현재 페이지 출력
    {
        TCHAR current_number[INTERFACE_STRING_LENGTH] = {0, };
        TCHAR max_number[INTERFACE_STRING_LENGTH] = {0, };        

        _itot((page + 1), current_number, 10);
        int total_page = num_of_info / kMaxListLine;
        bool is_margin = (num_of_info % kMaxListLine) >= 1;
        if (is_margin == true || total_page == 0)
        {
            total_page += 1;
        }
        _itot(total_page, max_number, 10);
        Snprintf(info_string, INTERFACE_STRING_LENGTH, "%s / %s", current_number, max_number);        
        page_text->SetTextWZ(info_string);
    }
    
    for ( ; start_index < end_index; ++start_index)
    {
        GUILD_JOIN_REQUEST_INFO& join_request_info = guild_join_request_info_container[start_index];

        int list_index = guild_join_request_list->InsertItemBack();
        guild_join_request_list->SetItemBKColor(list_index, 0, RGBA(150, 10, 10, 180));

        // 캐릭터명
        guild_join_request_list->SetItemWZ(list_index,
            0,
            join_request_info.m_char_name,
            StrToWzID("st11"),
            DT_CENTER | DT_VCENTER,
            RGBA(0, 0, 0, 0),
            RGBA(255, 255, 255, 255),
            RGBA(255, 255, 255, 255));

        // 레벨
        _itot(join_request_info.m_char_level, info_string, 10);
        guild_join_request_list->SetItemWZ(list_index,
            1,
            info_string,
            StrToWzID("st11"),
            DT_CENTER | DT_VCENTER,
            RGBA(0, 0, 0, 0),
            RGBA(255, 255, 255, 255),
            RGBA(255, 255, 255, 255));

        // 클래스        
        switch (join_request_info.m_char_class)
        {
        case eCHAR_BERSERKER:
            {
                g_InterfaceManager.GetInterfaceString(10001, info_string);
            }
            break;
        case eCHAR_DRAGON:
            {
                g_InterfaceManager.GetInterfaceString(10003, info_string);
            }
            break;
        case eCHAR_VALKYRIE:
            {
                g_InterfaceManager.GetInterfaceString(10007, info_string);
            }
            break;
        case eCHAR_SHADOW:
            {
                g_InterfaceManager.GetInterfaceString(10005, info_string);
            }
            break;
        case eCHAR_ELEMENTALIST:
            {
                g_InterfaceManager.GetInterfaceString(10009, info_string);
            }
            break;
        case eCHAR_MYSTIC:
            {
                g_InterfaceManager.GetInterfaceString(10002, info_string);
            }
            break;
        case eCHAR_HELLROID:
            {
                g_InterfaceManager.GetInterfaceString(10006, info_string);
            }
            break;
        case eCHAR_WITCHBLADE: //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
            {
                g_InterfaceManager.GetInterfaceString(2685, info_string);
            }
            break;
        default:
            {
                Snprintf(info_string, INTERFACE_STRING_LENGTH, "%s", "UnknownClass");
            }
            break;
        }

        guild_join_request_list->SetItemWZ(list_index,
            2,
            info_string,
            StrToWzID("st11"),
            DT_CENTER | DT_VCENTER,
            RGBA(0, 0, 0, 0),
            RGBA(255, 255, 255, 255),
            RGBA(255, 255, 255, 255));

        // 대기 시간
        _itot(join_request_info.m_request_time, info_string, 10);
        guild_join_request_list->SetItemWZ(list_index,
            3,
            info_string,
            StrToWzID("st11"),
            DT_CENTER | DT_VCENTER,
            RGBA(0, 0, 0, 0),
            RGBA(255, 255, 255, 255),
            RGBA(255, 255, 255, 255));

        SetAcceptAndRejectionButtonShow(list_index, WZ_SHOW, start_index);
        if (is_duty)
        {
            // 가입을 수락 거절 할 권한이 있는지 검사해서 활성화 비활성화 한다.
            SetAcceptAndRejectionButtonEnable(list_index, is_duty, start_index);
        }
    }
}

void uiGuildJoinRequestList::UpdateRemainTime()
{
    CCtrlListWZ* guild_join_request_list = 
        static_cast<CCtrlListWZ*>(GetControlWZ_byPosition(kControl_List_L001));
    CCtrlStaticWZ* page_text = static_cast<CCtrlStaticWZ*>(GetControlWZ_byPosition(kControl_Text_S000));
    if (!guild_join_request_list)
        return;

    if (!page_text)
        return;
    int page = current_page();

    GuildSystem::GuildSystemData::GuildJoinRequestInfoContainer& guild_join_request_info_container = 
        GuildSystem::GetGuildSystemData().guild_join_request_info_container();
    int start_index = page * kInfoPerPage;
    int end_index = start_index + kInfoPerPage;
    int num_of_info = static_cast<int>(guild_join_request_info_container.size());
    end_index = min(end_index, num_of_info);    

    int save_index = guild_join_request_list->GetCurSel();    
    for ( ; start_index < end_index; ++start_index)
    {
        GUILD_JOIN_REQUEST_INFO& join_request_info = guild_join_request_info_container[start_index];        
        time_t add_time = join_request_info.m_remain_time;
        time_t end_time = join_request_info.m_request_time + add_time;
        time_t cur_time;
        util::TimeSync::time(&cur_time);
        ::localtime(&cur_time);
        ::localtime(&end_time);
        double remain_time = difftime(end_time, cur_time);

        int sec, min, hr;
        sec = (int)remain_time;
        min = sec / 60;
        sec = sec % 60;

        hr = min / 60;
        min = min % 60;

        TCHAR time_string[INTERFACE_STRING_LENGTH] = {0, };
        if (sec >= 0 && min >= 0 && hr >= 0)
        {
            Snprintf(time_string, INTERFACE_STRING_LENGTH, "%02d:%02d:%02d", hr, min, sec);
        }
        else
        {
            Snprintf(time_string, INTERFACE_STRING_LENGTH, "%02d:%02d:%02d", 0, 0, 0);
        }
        
        int list_index = start_index % kMaxListLine;

        // 대기 시간        
        guild_join_request_list->SetItemWZ(list_index,
            3,
            time_string,
            StrToWzID("st11"),
            DT_CENTER | DT_VCENTER,
            RGBA(0, 0, 0, 0),
            RGBA(255, 255, 255, 255),
            RGBA(255, 255, 255, 255));
    }    
}

void uiGuildJoinRequestList::SetAcceptAndRejectionButtonShow( const int& index, 
                                                              const ENUM_STATEWZ& is_state, 
                                                              const int& guild_container_index)
{
    if ((index > kMaxListLine - 1) || (index < 0))
        return;
    
    if (accept_rejection_control[index].accept_control)
    {
        accept_rejection_control[index].accept_control->ShowWindowWZ(is_state);
    }
    if (accept_rejection_control[index].rejection_control)
    {
        accept_rejection_control[index].rejection_control->ShowWindowWZ(is_state);
    }
    accept_rejection_control[index].guild_container_index = guild_container_index;
}

void uiGuildJoinRequestList::SetAcceptAndRejectionButtonEnable( const int& index, 
                                                                const bool& is_enable, 
                                                                const int& guild_container_index )
{
    if ((index > kMaxListLine - 1) || (index < 0))
        return;

    if (accept_rejection_control[index].accept_control)
    {
        accept_rejection_control[index].accept_control->EnableWindowWZ(is_enable);
    }
    if (accept_rejection_control[index].rejection_control)
    {
        accept_rejection_control[index].rejection_control->EnableWindowWZ(is_enable);
    }
    accept_rejection_control[index].guild_container_index = guild_container_index;
}

int uiGuildJoinRequestList::get_max_page()
{
    int default_page = 1;
    GuildSystem::GuildSystemData::GuildJoinRequestInfoContainer& guild_join_request_info_container = 
        GuildSystem::GetGuildSystemData().guild_join_request_info_container();

    int size = static_cast<int>(guild_join_request_info_container.size());
    default_page = size / kMaxListLine;
    if (default_page == 0)
        default_page = 1;

    return default_page;
}

void uiGuildJoinRequestList::ShowCharacterInfo()
{
    CCtrlListWZ* join_request_list = static_cast<CCtrlListWZ*>(GetControlWZ_byPosition(kControl_List_L001));
    if (join_request_list == NULL)
    {
        return;
    }
    
    int selected_index = join_request_list->GetCurSel();
    selected_index;
    const TCHAR* character_name = join_request_list->GetItemText(selected_index, 0);
    if (NULL == character_name)
    {
        return;
    }

    GuildSystem::GuildSystemData::GuildJoinRequestInfoContainer& guild_join_request_info_container = 
        GuildSystem::GetGuildSystemData().guild_join_request_info_container();
    int container_index = -1;
    CHARGUID character_guid = 0;
    for (unsigned int i = 0; i < guild_join_request_info_container.size(); ++i)
    {
        GUILD_JOIN_REQUEST_INFO& join_request_info = guild_join_request_info_container[i];
        if (strcmp(join_request_info.m_char_name, character_name) == 0)
        {
            container_index = i;
            character_guid = join_request_info.m_char_guid;
            break;
        }
    }

    if (container_index != -1)
    {
        uiCommunityMan* ui_community_manager =
            GET_DIALOG_MANAGER(uiCommunityMan, UIMAN_COMMUNITY);
        if (!ui_community_manager)
            return;

        ui_community_manager->Send_CW_FRIEND_STATUS_REQ(const_cast<TCHAR*>(character_name), character_guid);
        
        if (ui_community_manager->GetCommunityinfo_Dialog())
        {
            ui_community_manager->GetCommunityinfo_Dialog()->set_request_uimanager_id(UIMAN_COMMUNITY);
        }        
    }    
}

bool uiGuildJoinRequestList::IsAccessJoinRequestAcceptAndReject()
{
    if (!g_pHero)
        return false;

    PLAYER_GUILD_RENDER_INFO& guild_render_info = g_pHero->GetGuildRenderInfo();
    const eGUILD_DUTY guild_duty = static_cast<eGUILD_DUTY>(guild_render_info.m_GuildDuty);

    if (GuildRightInfoParser::Instance()->CheckDutyForRight(eGUILD_RIGHTS_JOIN, guild_duty))
    {
        return true;
    }

    return false;
}
#endif // _NA_007330_20140620_GUILD_SYSTEM_EXTENSION