#include "SunClientPrecompiledHeader.h"
#ifdef _NA_003923_20120222_GUILD_RENEWAL_UI_RENEWAL
#include "GuildSystem/uiGuildDominationAreaInfo.h"
#include "GuildSystem/GuildSystemFunction.h"
#include "GuildSystem/GuildSystemData.h"
#include "TreeMenuInfoParser.h"
#include "OptionListParser.h"
#include "Hero.h"
#include "GlobalFunc.h"

const DWORD uiGuildDominationAreaInfo::kDialog_ID = StrToWzID("7347");
const int kListSellHeight = 21;

uiGuildDominationAreaInfo::uiGuildDominationAreaInfo( InterfaceManager* ui_manager )
    :uiBase(ui_manager)
{

}

uiGuildDominationAreaInfo::~uiGuildDominationAreaInfo()
{

}

void uiGuildDominationAreaInfo::Init( CDrawBase* draw_base )
{
    uiBase::Init(draw_base);

    RegisterControlPosition(StrToWzID("C001"), kControl_CheckBT_C001);
    RegisterControlPosition(StrToWzID("C004"), kControl_CheckBT_C004);
    RegisterControlPosition(StrToWzID("B999"), kControl_Button_B999);
    RegisterControlPosition(StrToWzID("L000"), kControl_List_L000);
    RegisterControlPosition(StrToWzID("VS02"), kControl_VScroll_VS02);

    domination_info_menu().Release();
}

void uiGuildDominationAreaInfo::Release()
{
    domination_info_menu().Release();
}

void uiGuildDominationAreaInfo::MessageProc( SI_MESSAGE* message )
{
    if (IsLock())
        return;

    if (GetMan() == NULL)
        return;

    TreeMenuGroup::TreeObject* tree_object = NULL;
    if (domination_info_menu().MessageProc(message, OUT tree_object) == true)
    {
    }

    switch (message->eResultMsg)
    {
    case RT_MSG_LBUTTONCLICK:
        OnLButtonClick(message);
        break;
    case RT_MSG_SB_LINETOP:
        OnVScrollUp(message);
    case RT_MSG_SB_LINEBOTTOM:
        OnVScrollDown(message);
    case RT_MSG_SB_THUMBTRACK:      //!< 썸을 드래그 하고 있다.
    case RT_MSG_SB_THUMBPOSITION:   //!< 썸을 드래그한후 마우스를 놓았다.
    case RT_MSG_SB_ENDSCROLL:       //!< 스크롤이 종료 되었다.
        OnVScrollThumb(message);
    default:
        SolarDialog::MessageProc(message);
        break;
    }
}

bool uiGuildDominationAreaInfo::OnLButtonClick( SI_MESSAGE* message )
{
    bool result = true;
    DWORD control_position = GetControlPosition_byID(message->dwCtrlID);
    switch (control_position)
    {
    case kControl_CheckBT_C001:
    case kControl_CheckBT_C004:
        {
            SetCurrentListIndex(control_position - kControl_CheckBT_C001);
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

bool uiGuildDominationAreaInfo::OnVScrollUp( SI_MESSAGE* message )
{
    CCtrlVScrollWZ* scroll_control = 
        static_cast<CCtrlVScrollWZ*>(GetControlWZ_byPosition(kControl_VScroll_VS02));
    CCtrlListWZ* info_list = static_cast<CCtrlListWZ*>(GetControlWZ_byPosition(kControl_List_L000));
    if ((scroll_control != NULL) && 
        (info_list != NULL))
    {
        int range_min = 0;
        int range_max = 0;
        int currenct_position = scroll_control->GetScrollPosWZ();
        scroll_control->GetScrollRangeWZ(&range_min, &range_max);
        --currenct_position;
        currenct_position = max(currenct_position, 0);
        currenct_position = min(currenct_position, range_max);
        scroll_control->SetScrollPosWZ(currenct_position);

        info_list->SetBeginDrawIndex(currenct_position);
    }

    return true;
}

bool uiGuildDominationAreaInfo::OnVScrollDown( SI_MESSAGE* message )
{
    CCtrlVScrollWZ* scroll_control = 
        static_cast<CCtrlVScrollWZ*>(GetControlWZ_byPosition(kControl_VScroll_VS02));
    CCtrlListWZ* info_list = static_cast<CCtrlListWZ*>(GetControlWZ_byPosition(kControl_List_L000));
    if ((scroll_control != NULL) && 
        (info_list != NULL))
    {
        int range_min = 0;
        int range_max = 0;
        int currenct_position = scroll_control->GetScrollPosWZ();
        scroll_control->GetScrollRangeWZ(&range_min, &range_max);
        ++currenct_position;
        currenct_position = max(currenct_position, 0);
        currenct_position = min(currenct_position, range_max);
        scroll_control->SetScrollPosWZ(currenct_position);

        info_list->SetBeginDrawIndex(currenct_position);
    }

    return true;
}

bool uiGuildDominationAreaInfo::OnVScrollThumb( SI_MESSAGE* message )
{
    CCtrlVScrollWZ* scroll_control = 
        static_cast<CCtrlVScrollWZ*>(GetControlWZ_byPosition(kControl_VScroll_VS02));
    CCtrlListWZ* info_list = static_cast<CCtrlListWZ*>(GetControlWZ_byPosition(kControl_List_L000));
    if ((scroll_control != NULL) && 
        (info_list != NULL))
    {
        int currenct_position = scroll_control->GetScrollPosWZ();
        info_list->SetBeginDrawIndex(currenct_position);
    }

    return true;
}

void uiGuildDominationAreaInfo::OnShowWindow( BOOL is_show )
{
    if (is_show == TRUE)
    {
        SetCurrentListIndex(kDominationAreaList);

        keyMsg msg;
        ZeroMemory(&msg,sizeof(msg));
        msg.dwType = uiGuildDominationAreaInfo::kDialog_ID;
        msg.wParam = uiGuildDominationAreaInfo::kDialog_ID;
        msg.DoSomething = GlobalFunc::DoShowWindowEx;
        g_KeyQueueManager.PushBack(msg);
    }
    else
    {
        g_KeyQueueManager.DeleteMsg(uiGuildDominationAreaInfo::kDialog_ID);
    }
}

void uiGuildDominationAreaInfo::OnUpdateSolarDialog()
{
    CCtrlButtonCheckWZ* tab_control = NULL;
    switch (current_list_index())
    {
    case kDominationAreaList:
        {
            domination_info_menu().UpdateMouseWheel();
            tab_control = 
                static_cast<CCtrlButtonCheckWZ*>(GetControlWZ_byPosition(kControl_CheckBT_C001));
        }
        break;
    case kDominationOptionList:
        {
            UpdateMouseWheel();
            tab_control = 
                static_cast<CCtrlButtonCheckWZ*>(GetControlWZ_byPosition(kControl_CheckBT_C001));
        }
        break;
    }

    RefreshTab();
}

void uiGuildDominationAreaInfo::InitDominationTree()
{
    CCtrlListWZ* list_control = 
        static_cast<CCtrlListWZ*>(GetControlWZ_byPosition(kControl_List_L000));
    if (list_control != NULL)
    {
        list_control->DeleteItemAll();
    }

    domination_info_menu().Release();
    
    RefreshDominationTree();

    domination_info_menu().Init(BASE_TreeMenuInfo::kGroupType_GuildDomination_Info, 
        list_control, 
        static_cast<CCtrlVScrollWZ*>(GetControlWZ_byPosition(kControl_VScroll_VS02)), 
        false, 
        Callback_DominationAreaInfoMenuRender);

    domination_info_menu().set_menu_height_gab(kListSellHeight);

    domination_info_menu().Fill();

    domination_info_menu().Select();

#ifdef _NA_008334_20150608_SONNENSCHEIN
    if (GuildSystem::GetGuildSystemData().get_sonnenschein_phase() != 0 &&
        reward_info().GetRewardOptionInfo().size() != 0)    // 브라키온, 세리엔트대륙 상위 트리가 있다면  둘다 펼쳐줌
    {
        TreeMenuGroup::TreeObject* tree_object = NULL;
        if (tree_object = domination_info_menu().Find(2,0,0,0))
        {
            domination_info_menu().Select(tree_object);
        }
    }
#endif //_NA_008334_20150608_SONNENSCHEIN
}

void uiGuildDominationAreaInfo::UpdateRewardOptionList()
{
    calculated_option_map().clear();

    CCtrlListWZ* list_control = 
        static_cast<CCtrlListWZ*>(GetControlWZ_byPosition(kControl_List_L000));
    CCtrlVScrollWZ* scroll_control = 
        static_cast<CCtrlVScrollWZ*>(GetControlWZ_byPosition(kControl_VScroll_VS02));
    if ((list_control == NULL) || 
        (scroll_control == NULL))
    {
        assert(list_control);
        assert(scroll_control);
        return;
    }

    {
        CalculatedOption temp_option;

        const DominationInfoParser::SelectedRewardOptions& selected_option_info = 
            reward_info().GetSelectedRewardOptionInfo();

        DominationInfoParser::SelectedRewardOptions::const_iterator begin_itr = selected_option_info.begin();
        DominationInfoParser::SelectedRewardOptions::const_iterator end_itr = selected_option_info.end();
        for ( ; begin_itr != end_itr; ++begin_itr)
        {
            const DominationInfoParser::SelectedRewardOption& selected_option = begin_itr->second;

            DWORD option_kind = selected_option.option_info->m_wOptionKind;
            CalculatedOptionMap::iterator found_itr = calculated_option_map().find(option_kind);
            if (found_itr == calculated_option_map().end())
            {
                calculated_option_map().insert(
                    CalculatedOptionMap::value_type(option_kind, CalculatedOption()));
                found_itr = calculated_option_map().find(option_kind);
            }
            CalculatedOption& temp_option = found_itr->second;
            temp_option.OptionKind = option_kind;

            if (selected_option.option_info->m_byOptionType == VALUE_TYPE_VALUE)
            {
                temp_option.OptionValue += selected_option.option_info->m_iOptionValue;
            }
            else
            {
                temp_option.OptionRatio += selected_option.option_info->m_iOptionValue;
            }

            BASE_OptionList* option_list = OptionListParser::Instance()->GetOptionList(option_kind);
            if (option_list != NULL)
            {
                temp_option.OptionNC = option_list->mdwNameCode;
            }
        }
    }
    //////////////////////////////////////////////////////////////////////////
    {
        TCHAR option_string[INTERFACE_STRING_LENGTH] = {'\0', };
        TCHAR temp_string[INTERFACE_STRING_LENGTH] = {'\0', };

        list_control->DeleteItemAll();
        CalculatedOptionMap::iterator begin_itr = calculated_option_map().begin();
        CalculatedOptionMap::iterator end_itr = calculated_option_map().end();
        for ( ; begin_itr != end_itr; ++begin_itr)
        {
            CalculatedOption& calculated_option = begin_itr->second;

            int list_index = list_control->InsertItemBack();

            // 옵션명
            g_InterfaceManager.GetInterfaceString(calculated_option.OptionNC, option_string);
            strcat(option_string, " : ");

            // 수치
            bool is_first_value = true;
            if (calculated_option.OptionValue != 0)
            {
                Snprintf(temp_string, INTERFACE_STRING_LENGTH, _T("%+d"), calculated_option.OptionValue);
                BASE_OptionList* option_list = OptionListParser::Instance()->GetOptionList(calculated_option.OptionKind);
                GlobalFunc::AddOptionSuffix(VALUE_TYPE_VALUE, option_list, temp_string);
                strcat(option_string, temp_string);
                is_first_value = false;
            }
            if (calculated_option.OptionRatio != 0)
            {
                Snprintf(temp_string, 
                    INTERFACE_STRING_LENGTH, 
                    _T("%+.1f%%"), 
                    (calculated_option.OptionRatio / 10.0f));
                if (is_first_value == false)
                {
                    strcat(option_string, ", ");
                }
                strcat(option_string, temp_string);
            }
            
            list_control->SetItemWZA(list_index, 
                option_string, 
                StrToWzID("st10"), 
                DT_VCENTER | DT_LEFT, 
                RGBA(0, 0, 0, 0), 
                RGBA (255, 255, 255, 255), 
                RGBA (255, 255, 255, 255));
        }
    }

    int list_count = list_control->GetListRowCnt();
    int draw_count = list_control->GetDrawRowCount();
    int scroll_range = list_count - draw_count;
    scroll_range = max(scroll_range, 0);
    
    list_control->SetBeginDrawIndex(0);
    scroll_control->SetScrollRangeWZ(0, scroll_range);
    scroll_control->SetChangeSizeThumb(100 / (scroll_range + 1));
    scroll_control->SetScrollPosWZ(0);
}

void uiGuildDominationAreaInfo::UpdateMouseWheel()
{
    CCtrlListWZ* info_list = static_cast<CCtrlListWZ*>(GetControlWZ_byPosition(kControl_List_L000));
    if (info_list != NULL)
    {
        RECT area = info_list->GetSizeRect();
        BYTE wheel_msg = uiBase::GetMouseWheelStatus(&area);

        switch (wheel_msg)
        {
        case MOUSE_WHEEL_UP:
            OnVScrollUp(NULL);
            break;
        case MOUSE_WHEEL_DOWN:
            OnVScrollDown(NULL);
            break;
        }
    }
}

void uiGuildDominationAreaInfo::SetCurrentListIndex( DWORD list_index )
{
    reward_info().Clear();
    if (g_pHero != NULL)
    {
        reward_info().SetZoneMarkInfo(g_pHero->GetDominationStateInfo()
#ifdef _NA_008334_20150608_SONNENSCHEIN
            , GuildSystem::GetGuildSystemData().get_sonnenschein_phase()
#endif //_NA_008334_20150608_SONNENSCHEIN
            );
    }

    current_list_index_ = list_index;
    switch (list_index)
    {
    case kDominationAreaList:
        {
            InitDominationTree();
        }
        break;
    case kDominationOptionList:
        {
            domination_info_menu().Release();
            UpdateRewardOptionList();
        }
        break;
    }
}

void uiGuildDominationAreaInfo::RefreshDominationTree()
{
    TreeMenuInfoParser::Instance()->Remove(BASE_TreeMenuInfo::kGroupType_GuildDomination_Info);

    std::vector<BASE_TreeMenuInfo*> template_info_container;
    TreeMenuInfoParser::Instance()->GetTreeMenu(template_info_container, 
        BASE_TreeMenuInfo::kGroupType_GuildDomination_Info_Template);

    if (template_info_container.begin() == template_info_container.end())
    {
        return;
    }

    BASE_TreeMenuInfo* template_folder_info = template_info_container.front();
    BASE_TreeMenuInfo* template_info = template_info_container.back();
    if ((template_folder_info == NULL) || 
        (template_info == NULL) || 
        (template_folder_info == template_info))
    {
        return;
    }

    BASE_TreeMenuInfo new_continent_menu_info = (*template_folder_info);
    BASE_TreeMenuInfo new_area_menu_info = (*template_info);
    new_continent_menu_info.tree_group = BASE_TreeMenuInfo::kGroupType_GuildDomination_Info;
    new_area_menu_info.tree_group = BASE_TreeMenuInfo::kGroupType_GuildDomination_Info;
    BYTE current_continent = 0;
#ifdef _NA_008334_20150608_SONNENSCHEIN
    BYTE second_continent_tree_count = 0;   // 세리엔트대륙 항목 수
#endif //_NA_008334_20150608_SONNENSCHEIN

    int reward_info_size = reward_info().GetRewardOptionInfo().size();
    for (int info_index = 0; info_index < reward_info_size; ++info_index)
    {
        REWARD_OPTION& reward_option = reward_info().GetRewardOptionInfo()[info_index];
        const sMAPINFO* map_info = MapInfoParser::Instance()->FindMapInfo(reward_option.MapCode);
        if (map_info == NULL)
        {
            continue;
        }

        if (current_continent != map_info->ContienetNumber)
        {
            // 대륙 폴더 넣기
            current_continent = (BYTE)map_info->ContienetNumber;
            new_continent_menu_info.menu_depth[BASE_TreeMenuInfo::kFirstSubMenuIndex] = current_continent;
            if (current_continent == 1)
            {
                // 브라키온 대륙
                new_continent_menu_info.string_code = 51010;
            }
            else if (current_continent == 2)
            {
                // 세리엔트 대륙
                new_continent_menu_info.string_code = 51011;
            }
            TreeMenuInfoParser::Instance()->Insert(new_continent_menu_info);
        }

        // 점령지역 넣기
        new_area_menu_info.menu_depth[BASE_TreeMenuInfo::kFirstSubMenuIndex] = current_continent;
        new_area_menu_info.menu_depth[BASE_TreeMenuInfo::kSecondSubMenuIndex] = (BYTE)(info_index + 1);
        new_area_menu_info.string_code = map_info->dwNCode;
#ifdef _NA_008334_20150608_SONNENSCHEIN
        new_area_menu_info.user_data = info_index;
#else
        new_area_menu_info.user_data = (DWORD)(&reward_option);
#endif //_NA_008334_20150608_SONNENSCHEIN
        TreeMenuInfoParser::Instance()->Insert(new_area_menu_info);

#ifdef _NA_008334_20150608_SONNENSCHEIN
        if (current_continent == 2)
        {
            ++second_continent_tree_count;   // 세리엔트대륙 항목 수
        }
#endif //_NA_008334_20150608_SONNENSCHEIN
    }

    
#ifdef _NA_008334_20150608_SONNENSCHEIN
    // 소넨샤인 트리에 넣기
    if (GuildSystem::GetGuildSystemData().get_sonnenschein_phase())
    {
        if (second_continent_tree_count == 0)
        {
            new_continent_menu_info.menu_depth[BASE_TreeMenuInfo::kFirstSubMenuIndex] = 2;
            // 세리엔트 대륙
            new_continent_menu_info.string_code = 51011;
            TreeMenuInfoParser::Instance()->Insert(new_continent_menu_info);
        }

        new_area_menu_info.menu_depth[BASE_TreeMenuInfo::kFirstSubMenuIndex] = 2;
        new_area_menu_info.menu_depth[BASE_TreeMenuInfo::kSecondSubMenuIndex] = second_continent_tree_count + 1;
        new_area_menu_info.string_code = ZoneMarkRewardInfo::SONNENSCHEIN_CLEAR_STRINGCODE_START +
            (GuildSystem::GetGuildSystemData().get_sonnenschein_phase() - 1);
        new_area_menu_info.user_data = ZoneMarkRewardInfo::SONNENSCHEIN_INDEX_KEY;
        TreeMenuInfoParser::Instance()->Insert(new_area_menu_info);
    }
#endif //_NA_008334_20150608_SONNENSCHEIN
}

void uiGuildDominationAreaInfo::RefreshTab()
{
    CCtrlButtonCheckWZ* check_button = NULL;
    for (int position = kControl_CheckBT_C001; position < (kControl_CheckBT_C004 + 1); ++position)
    {
        check_button = 
            static_cast<CCtrlButtonCheckWZ*>(GetControlWZ(GetControlID_byPosition(position)));
        if (check_button == NULL)
        {
            continue;
        }
        check_button->SetCheckState((current_list_index() == position) ? (true) : (false));
    }
}

void uiGuildDominationAreaInfo::RenderDominationAreaRewardInfo( DWORD reward_index )
{
    if (reward_info().GetRewardCount() > reward_index
#ifdef _NA_008334_20150608_SONNENSCHEIN
        || (reward_index == ZoneMarkRewardInfo::SONNENSCHEIN_INDEX_KEY)
#endif //_NA_008334_20150608_SONNENSCHEIN
        )
    {
        reward_info().RenderTooltip(reward_index, false, GetDialogKey());
    }
}

void uiGuildDominationAreaInfo::Callback_DominationAreaInfoMenuRender(CCtrlListWZ* list_control, 
                                                                      CCtrlListWZ::CListSellData* sell_data, 
                                                                      int sell_index, 
                                                                      RECT& render_area )
{
    TreeMenuGroup::Callback_TreeRender(list_control, sell_data, sell_index, render_area);

    if (list_control == NULL || sell_data == NULL || sell_data->user_data == NULL) 
    {
        return;
    }

    TreeMenuGroup::TreeObject* tree_object = static_cast<TreeMenuGroup::TreeObject*>(sell_data->user_data);
    if (tree_object == NULL)
    {
        return;
    }

    int over_sell = list_control->GetOverSel();
    if ((over_sell == sell_index) && 
        (tree_object->info->menu_type == BASE_TreeMenuInfo::kMenuType_None))
    {
        // 툴팁출력
#ifdef _NA_008334_20150608_SONNENSCHEIN
        DWORD reward_index = tree_object->info->user_data;
     
        uiGuildDominationAreaInfo* ui_guild_domination_area_info = 
            GET_CAST_DIALOG(uiGuildDominationAreaInfo, uiGuildDominationAreaInfo::kDialog_ID);
        if (ui_guild_domination_area_info != NULL)
        {
            ui_guild_domination_area_info->RenderDominationAreaRewardInfo(reward_index);
        }
#else
        DWORD reward_index = tree_object->info->menu_depth[BASE_TreeMenuInfo::kSecondSubMenuIndex];
        if (reward_index != 0)
        {
            uiGuildDominationAreaInfo* ui_guild_domination_area_info = 
                GET_CAST_DIALOG(uiGuildDominationAreaInfo, uiGuildDominationAreaInfo::kDialog_ID);
            if (ui_guild_domination_area_info != NULL)
            {
                --reward_index;
                ui_guild_domination_area_info->RenderDominationAreaRewardInfo(reward_index);
            }
        }
#endif //_NA_008334_20150608_SONNENSCHEIN
    }
}

#endif //_NA_003923_20120222_GUILD_RENEWAL_UI_RENEWAL