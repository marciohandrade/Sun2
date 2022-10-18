#include "SunClientPrecompiledHeader.h"
#ifdef _NA_003923_20120222_GUILD_RENEWAL_UI_RENEWAL
#include "GuildSystem/uiGuildRanking.h"
#include "GuildSystem/GuildSystemPacketFunction.h"
#include "GuildSystem/GuildSystemData.h"
#include "GuildSystem/uiGuildInfoOther.h"
#include "GlobalFunc.h"

const DWORD uiGuildRanking::kDialog_ID = StrToWzID("7333");

uiGuildRanking::uiGuildRanking( InterfaceManager* ui_manager )
:uiBase(ui_manager)
{
    set_current_page(0);
}

uiGuildRanking::~uiGuildRanking()
{

}

void uiGuildRanking::Init( CDrawBase* draw_base )
{
    uiBase::Init(draw_base);

    RegisterControlPosition(StrToWzID("B001"), kControl_Button_B001);
    RegisterControlPosition(StrToWzID("B002"), kControl_Button_B002);
    RegisterControlPosition(StrToWzID("B003"), kControl_Button_B003);
    RegisterControlPosition(StrToWzID("S000"), kControl_Text_S000);
    RegisterControlPosition(StrToWzID("S001"), kControl_Text_S001);
    RegisterControlPosition(StrToWzID("S002"), kControl_Text_S002);
    RegisterControlPosition(StrToWzID("S003"), kControl_Text_S003);
    RegisterControlPosition(StrToWzID("S004"), kControl_Text_S004);
    RegisterControlPosition(StrToWzID("S005"), kControl_Text_S005);
    RegisterControlPosition(StrToWzID("S006"), kControl_Text_S006);
    RegisterControlPosition(StrToWzID("S007"), kControl_Text_S007);
    RegisterControlPosition(StrToWzID("S008"), kControl_Text_S008);
    RegisterControlPosition(StrToWzID("L000"), kControl_List_L000);

    CCtrlListWZ* ranking_list = static_cast<CCtrlListWZ*>(GetControlWZ_byPosition(kControl_List_L000));
    if (ranking_list != NULL)
    {
        ranking_list->SetUseReport(true);
        ranking_list->SetSellHeight(18);
        ranking_list->SetColorSelBar(RGBA(150, 10, 10, 125));
        ranking_list->SetReportUseCnt(6);
        ranking_list->SetHideReportRender(TRUE);
        ranking_list->SetReportButtonInfo(0, 40, NULL); // 랭킹
        ranking_list->SetReportButtonInfo(1, 195, NULL); // 길드명
        ranking_list->SetReportButtonInfo(2, 182, NULL); // 길드마스터
        ranking_list->SetReportButtonInfo(3, 85, NULL); // 지역점령
        ranking_list->SetReportButtonInfo(4, 85, NULL); // 길드레벨
        ranking_list->SetReportButtonInfo(5, 79, NULL); // 길드원
    }
}

void uiGuildRanking::Release()
{

}

void uiGuildRanking::MessageProc( SI_MESSAGE* message )
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
    default:
        SolarDialog::MessageProc(message);
        break;
    }
}

bool uiGuildRanking::OnLButtonClick( SI_MESSAGE* message )
{
    bool result = true;
    DWORD control_position = GetControlPosition_byID(message->dwCtrlID);
    switch (control_position)
    {
    case kControl_Button_B001:
        {
            ShowInterface(FALSE);
        }
        break;
    case kControl_Button_B002:
        {
            OnClickPrevPageButton();
        }
        break;
    case kControl_Button_B003:
        {
            OnClickNextPageButton();
        }
        break;
    }

    return result;
}

bool uiGuildRanking::OnLButtonDClick( SI_MESSAGE* message )
{
    bool result = true;
    DWORD control_position = GetControlPosition_byID(message->dwCtrlID);
    switch (control_position)
    {
    case kControl_List_L000:
        {
            ShowGuildInfo();
        }
        break;
    }

    return result;
}

void uiGuildRanking::OnShowWindow( BOOL is_show )
{
    if (is_show == TRUE)
    {
        set_current_page(0);
        UpdateRankList();

        keyMsg msg;
        ZeroMemory(&msg,sizeof(msg));
        msg.dwType = uiGuildRanking::kDialog_ID;
        msg.wParam = uiGuildRanking::kDialog_ID;
        msg.DoSomething = GlobalFunc::DoShowWindowEx;
        g_KeyQueueManager.PushBack(msg);
    }
    else
    {
        g_KeyQueueManager.DeleteMsg(uiGuildRanking::kDialog_ID);
    }
}

void uiGuildRanking::OnUpdateSolarDialog()
{

}

void uiGuildRanking::OnClickPrevPageButton()
{
    int page = current_page();
    --page;
    page = max(0, page);
    set_current_page(page);

    UpdateRankList();
}

void uiGuildRanking::OnClickNextPageButton()
{
    GuildSystem::GuildSystemData::GuildRankingInfoContainer& guild_ranking_info_container = 
        GuildSystem::GetGuildSystemData().guild_ranking_info_container();
    int num_of_info = static_cast<int>(guild_ranking_info_container.size());
    int page = current_page();
    
    ++page;
    if ((page * kInfoPerPage) < num_of_info)
    {
        page = min(kMaxPage, page);
        set_current_page(page);

        UpdateRankList();
    }
}

void uiGuildRanking::UpdateRankList()
{
    CCtrlListWZ* ranking_list = static_cast<CCtrlListWZ*>(GetControlWZ_byPosition(kControl_List_L000));
    CCtrlStaticWZ* page_text = static_cast<CCtrlStaticWZ*>(GetControlWZ_byPosition(kControl_Text_S008));
    int page = current_page();
    ranking_list->DeleteItemAll();

    TCHAR info_string[INTERFACE_STRING_LENGTH] = {'\0', };

    // 현재 페이지 출력
    if (page_text != NULL)
    {
        _itot((page + 1), info_string, 10);
        page_text->SetTextWZ(info_string);
    }

    GuildSystem::GuildSystemData::GuildRankingInfoContainer& guild_ranking_info_container = 
        GuildSystem::GetGuildSystemData().guild_ranking_info_container();
    int start_index = page * kInfoPerPage;
    int end_index = start_index + kInfoPerPage;
    int num_of_info = static_cast<int>(guild_ranking_info_container.size());
    end_index = min(end_index, num_of_info);
    for ( ; start_index < end_index; ++start_index)
    {
        GUILD_RANKING_INFO& ranking_info = guild_ranking_info_container[start_index];

        int list_index = ranking_list->InsertItemBack();
        ranking_list->SetItemBKColor(list_index, 0, RGBA (150, 10, 10, 180));

        // 랭킹
        _itot(start_index + 1, info_string, 10);
        ranking_list->SetItemWZ(list_index, 
            0, 
            info_string, 
            StrToWzID("st11"), 
            DT_CENTER | DT_VCENTER, 
            RGBA(0,0,0,0), 
            RGBA(255,255,255,255), 
            RGBA(255,255,255,255));

        // 길드명
        ranking_list->SetItemWZ(list_index, 
            1, 
            ranking_info.guild_name, 
            StrToWzID("st11"), 
            DT_CENTER | DT_VCENTER, 
            RGBA(0,0,0,0), 
            RGBA(255,255,255,255), 
            RGBA(255,255,255,255));

        // 길드마스터
        ranking_list->SetItemWZ(list_index, 
            2, 
            ranking_info.master_name, 
            StrToWzID("st11"), 
            DT_CENTER | DT_VCENTER, 
            RGBA(0,0,0,0), 
            RGBA(255,255,255,255), 
            RGBA(255,255,255,255));

        // 지역점령
        _itot(ranking_info.domination_count, info_string, 10);
        ranking_list->SetItemWZ(list_index, 
            3, 
            info_string, 
            StrToWzID("st11"), 
            DT_CENTER | DT_VCENTER, 
            RGBA(0,0,0,0), 
            RGBA(255,255,255,255), 
            RGBA(255,255,255,255));

        // 길드레벨
        _itot(ranking_info.guild_level, info_string, 10);
        ranking_list->SetItemWZ(list_index, 
            4, 
            info_string, 
            StrToWzID("st11"), 
            DT_CENTER | DT_VCENTER, 
            RGBA(0,0,0,0), 
            RGBA(255,255,255,255), 
            RGBA(255,255,255,255));

        // 길드원
        Snprintf(info_string, INTERFACE_STRING_LENGTH, "  %d", ranking_info.member_count);
        ranking_list->SetItemWZ(list_index, 
            5, 
            info_string, 
            StrToWzID("st11"), 
            DT_CENTER | DT_VCENTER, 
            RGBA(0,0,0,0), 
            RGBA(255,255,255,255), 
            RGBA(255,255,255,255));
    }
}

void uiGuildRanking::ShowGuildInfo()
{
    CCtrlListWZ* ranking_list = static_cast<CCtrlListWZ*>(GetControlWZ_byPosition(kControl_List_L000));
    if (ranking_list == NULL)
    {
        return;
    }

    int selected_index = ranking_list->GetCurSel();
    selected_index += (current_page() * kInfoPerPage);

    GuildSystem::GuildSystemData::GuildRankingInfoContainer& guild_ranking_info_container = 
        GuildSystem::GetGuildSystemData().guild_ranking_info_container();
    if (static_cast<size_t>(selected_index) >= guild_ranking_info_container.size())
    {
        return;
    }

    GUILD_RANKING_INFO& ranking_info = guild_ranking_info_container[selected_index];
    GUILDGUID guild_guid = ranking_info.guild_guid;
    GuildSystem::PacketFunction::SEND_CG_ETC_GUILDINFO_SYN(guild_guid);

    uiGuildInfoOther* guild_info_other = 
        GET_CAST_DIALOG(uiGuildInfoOther, uiGuildInfoOther::kDialog_ID);
    if (guild_info_other != NULL)
    {
        guild_info_other->ShowInterface(FALSE);
        guild_info_other->set_guild_rank_index(selected_index);
    }
}
#endif //_NA_003923_20120222_GUILD_RENEWAL_UI_RENEWAL
