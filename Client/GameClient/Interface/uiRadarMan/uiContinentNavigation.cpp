#include "SunClientPrecompiledHeader.h"
#include "uiContinentNavigation.h"
#include "uiContinentDefine.h"
#include "QuestInfoManagerEx.h"
#include "MissionManager_Concrete.h"
#include "uiRadarMan.h"
#include "uiToolTipMan/uiToolTipMan.h"
#include "ItemManager.h"
#include "MapInfoParser.h"
#include "RadarInfoParser.h"
#include "QuestTextInfoParser.h"
#include "NPCInfoParser.h"
#include "ItemInfoParser.h"
#include "Mouse.h"
#include "FTextParser.h"
#include "FTextSentence.h"
#include "FTextParam.h"
#include "Hero.h"

uiContinentNavigation::uiContinentNavigation()
{
    InitializeNaviPoint();
}

uiContinentNavigation::~uiContinentNavigation()
{

}

void uiContinentNavigation::SetNavigationPoint()
{
    InitializeNaviPoint();

    uiQuestListDlg* quest_list_dialog = GET_CAST_DIALOG(uiQuestListDlg, IM_QUEST_MANAGER::QUEST_LIST_DLG);
    if (quest_list_dialog == NULL)
    {
        return;
    }

    uiRadarMan* ui_radar_manager = GET_DIALOG_MANAGER(uiRadarMan, UIMAN_RADAR);
    if (ui_radar_manager == NULL)
    {
        return;
    }

    ContinentMapNumber current_continent_map_number = ui_radar_manager->GetCurrentContinentMapNumber();
    if (current_continent_map_number == kContinentNone)
    {
        return;
    }

    int navigation_start_point = ui_radar_manager->GetCurrnetContinentMapStartPoint();
    int navigation_start_end = ui_radar_manager->GetCurrnetContinentMapStartEnd();
    
    DWORD navigation_quest_code = 0;
    for(int index = 0; index < MAX_NAVI_CNT; ++index)
    {
        navigation_point_[index] = -1;
        navigation_quest_code = quest_list_dialog->GetNaviQCodes(index);
        Quest* quest = g_pQMGR_Real->FindQuest(static_cast<QCODE>(navigation_quest_code));
        if (quest == NULL)
        {
            continue;
        }

        eQUEST_STATE quest_state = quest->GetQState();
        if (quest_state == QS_COMPLETED)
        {
            DWORD reward_map_code = quest->GetRewardMapCode();
            const sMAPINFO* map_info = MapInfoParser::Instance()->FindMapInfo((MAPCODE)reward_map_code);
            if(NULL == map_info)
                continue;

            // 대륙맵넘버 먼저 체크
            if (current_continent_map_number != map_info->ContienetNumber)
            {
                continue;
            }

            if ((navigation_start_point > map_info->wContinentCode) || 
                (navigation_start_end < map_info->wContinentCode))
            {
                assert(!"Invalid Code : map_info->wContinentCode");
                continue;
            }

            int continent_code = map_info->wContinentCode - 1;
            navigation_point_[index] = continent_code;
        }
        else if(QS_NORMAL == quest_state)
        {
            QUEST_CONDITION_KILLMONSTER_INFO* quest_condition_info = 
                (QUEST_CONDITION_KILLMONSTER_INFO*)quest->GetQuestInfo()->FindCompleteCondition(eQUEST_CONDITION_KILLMONSTER);
            if (quest_condition_info == NULL)
            {
                continue;
            }

            QUEST_CONDITION_KILLMONSTER* quest_condition = NULL;
            QUEST_COND_HASH& complete_condition_hash = static_cast<QUEST_COND_HASH&>(quest->GetCompleteConditionHash());
            QUEST_COND_HASH_IT complete_condition_hash_itr = complete_condition_hash.begin(); 
            while (complete_condition_hash_itr != complete_condition_hash.end())
            {
                QuestCondition* complete_condition = (*complete_condition_hash_itr);
                if (eQUEST_CONDITION_KILLMONSTER == complete_condition->GetType())
                {
                    quest_condition = static_cast<QUEST_CONDITION_KILLMONSTER*>(complete_condition);
                    break;
                }
                ++complete_condition_hash_itr;
            }

            if (quest_condition == NULL)
            {
                continue;
            }

            for(int condition_index = 0; condition_index < QUEST_CONDITION_KILLMONSTER::MAX_MONSTER_KIND_NUM; ++condition_index)
            {

                if (quest_condition->GetCurMonsterNum(condition_index) >= quest_condition_info->GetMonsterNum(condition_index))
                {
                    continue;
                }

                DWORD KillMonsterMapCode = quest->GetKillMonsterMapCode(condition_index);
                const sMAPINFO* map_info = MapInfoParser::Instance()->FindMapInfo((MAPCODE)KillMonsterMapCode);
                if (map_info == NULL)
                    continue;

                // 대륙맵넘버 먼저 체크
                if (current_continent_map_number != map_info->ContienetNumber)
                {
                    continue;
                }

                if ((navigation_start_point > map_info->wContinentCode) || 
                    (navigation_start_end < map_info->wContinentCode))
                {
                    assert(!"Invalid Code : map_info->wContinentCode");
                    continue;
                }

                int continent_code = map_info->wContinentCode - 1;
                navigation_point_[index] = continent_code;

                break;
            }
        }
    }
}

void uiContinentNavigation::RenderNavigationPoint()
{
    uiRadarMan* ui_radar_manager = GET_DIALOG_MANAGER(uiRadarMan, UIMAN_RADAR);
    if (ui_radar_manager == NULL)
    {
        return;
    }

    int mouse_pos_x = Mouse::Instance()->GetMouseX();
    int mouse_pos_y = Mouse::Instance()->GetMouseY();
    int continent_code = -1;

    RECT* point_image_rect = NULL;
    for(int index = 0; index < MAX_NAVI_CNT; ++index)
    {
        continent_code = navigation_point_[index];

        if (navigation_point_[index] < 0)
        {
            continue;
        }

        CControlWZ* point_control = ui_radar_manager->GetCurrentContinentMapPointControl(continent_code);
        if (point_control == NULL)
        {
            continue;
        }

        float control_width = static_cast<float>(point_control->GetSizeRect().left);
        float control_height = static_cast<float>(point_control->GetSizeRect().top);
#ifdef _NA_004644_20110506_QUEST_NAVIGATION
        point_image_rect = RadarInfoParser::Instance()->GetRadarImgInfo(eRADAR_NAVI_CIRCLE_RED + index);

        g_pSunRenderer->RenderTexture(RadarInfoParser::Instance()->GetTexture(),
            control_width - static_cast<float>(point_image_rect->right/2) + 10,
            control_height - static_cast<float>(point_image_rect->bottom/2),
            static_cast<float>(point_image_rect->right),
            static_cast<float>(point_image_rect->bottom),
            static_cast<float>(point_image_rect->left),
            static_cast<float>(point_image_rect->top),
            static_cast<float>(point_image_rect->right),
            static_cast<float>(point_image_rect->bottom),
            NULL,
            0.0f, 
            RTO_DEFAULT);

        if ((mouse_pos_x > control_width + 10 - (point_image_rect->right/2)) && 
            (mouse_pos_x < control_width - (point_image_rect->right/2) + point_image_rect->right) && 
            (mouse_pos_y > control_height - (point_image_rect->bottom/2)) && 
            (mouse_pos_y < control_height - (point_image_rect->bottom/2) + point_image_rect->bottom))
#else
        point_image_rect = RadarInfoParser::Instance()->GetRadarImgInfo(eRADAR_NAVI_ARROW_RED + index);

        g_pSunRenderer->RenderTexture( RadarInfoParser::Instance()->GetTexture(),
            control_width,
            control_height - static_cast<float>(point_image_rect->bottom/2),
            static_cast<float>(point_image_rect->right),
            static_cast<float>(point_image_rect->bottom),
            static_cast<float>(point_image_rect->left),
            static_cast<float>(point_image_rect->top),
            static_cast<float>(point_image_rect->right),
            static_cast<float>(point_image_rect->bottom),
            NULL,
            0.0f, 
            RTO_DEFAULT,
            WZ_PI);

        if ((mouse_pos_x > control_width) && 
            (mouse_pos_x < control_width + point_image_rect->right) && 
            (mouse_pos_y > control_height - (point_image_rect->bottom/2)) && 
            (mouse_pos_y < control_height - (point_image_rect->bottom/2) + point_image_rect->bottom))
#endif//_NA_004644_20110506_QUEST_NAVIGATION
        {
            RenderNavigationTooltip(index);
        }
    }

#ifdef _NA_004644_20110506_QUEST_NAVIGATION
    for(int index = 0; index < MAX_NAVI_CNT; ++index)
    {
        continent_code = navigation_point_[index];

        if (navigation_point_[index] < 0)
        {
            continue;
        }

        CControlWZ* point_control = ui_radar_manager->GetCurrentContinentMapPointControl(continent_code);
        if (point_control == NULL)
        {
            continue;
        }

        float control_width = static_cast<float>(point_control->GetSizeRect().left) - 10.f;
        float control_height = static_cast<float>(point_control->GetSizeRect().top);

        point_image_rect = RadarInfoParser::Instance()->GetRadarImgInfo(eRADAR_NAVI_ARROW_RED + index);

        const float degree[4] = {(250.f * WZ_PI)/180.f, (205.f * WZ_PI)/180.f, (160.f * WZ_PI)/180.f,(115.f * WZ_PI)/180.f};
        const float move_x[4] = {-5.f, 5.f, 16.f, +25.f};
        const float move_y[4] = {-2.f, -8.f, -8.f, -2.f};

        g_pSunRenderer->RenderTexture( RadarInfoParser::Instance()->GetTexture(),
            (control_width + move_x[index]),
            (control_height + move_y[index] - static_cast<float>(point_image_rect->bottom/2)),
            static_cast<float>(point_image_rect->right),
            static_cast<float>(point_image_rect->bottom),
            static_cast<float>(point_image_rect->left),
            static_cast<float>(point_image_rect->top),
            static_cast<float>(point_image_rect->right),
            static_cast<float>(point_image_rect->bottom),
            NULL,
            0.0f, 
            RTO_DEFAULT,
            degree[index]);
    }
#endif//_NA_004644_20110506_QUEST_NAVIGATION
}

void uiContinentNavigation::RenderNavigationTooltip(int navigation_index)
{
    uiToolTipMan* ui_tooltip_manager = GET_DIALOG_MANAGER(uiToolTipMan, UIMAN_TOOLTIP);
    if (ui_tooltip_manager == NULL)
    {
        return;
    }

    uiQuestListDlg* quest_list_dialog = GET_CAST_DIALOG(uiQuestListDlg, IM_QUEST_MANAGER::QUEST_LIST_DLG);
    if (quest_list_dialog == NULL)
    {
        return;
    }

    DWORD navigation_quest_code = quest_list_dialog->GetNaviQCodes(navigation_index);
    Quest* quest = g_pQMGR_Real->FindQuest(static_cast<QCODE>(navigation_quest_code));
    if (quest == NULL)
    {
        return;
    }

    DWORD colors[] = { WzColor_RGBA(254,65,61,255), WzColor_RGBA(110,225,74,255), 
        WzColor_RGBA(32,163,254,255), WzColor_RGBA(155,108,248,255), WzColor_RGBA(230,230,230,255),
        WzColor_RGBA(0,0,0,255)};

    int color_number = navigation_index;
    if (navigation_index < 0 || navigation_index > 3)//퀘스트 네비게이션은 최대 4개 0~3
    {
        /*
        0 = WzColor_RGBA(254,65,61,255), 붉은색
        1 = WzColor_RGBA(110,225,74,255), 파란색
        2 = WzColor_RGBA(32,163,254,255), 연두색
        3 = WzColor_RGBA(155,108,248,255), 보라색
          (0~3 은 퀘스트 네비게이션 색상)
        4 = WzColor_RGBA(230,230,230,255) 밝은 회색 (회색이 나오면 실제는 문제가 있는 것임)
        */
        color_number = 4;
    }

    DWORD sp_tooltip = eSPECIAL_TOOLTIP_NONE;
    sp_tooltip = eSPECIAL_TOOLTIP_NOMAL_ITEM;

    TCHAR tooltip_message[INTERFACE_STRING_LENGTH + 1] = {0, };
    g_InterfaceManager.GetInterfaceString(quest->GetQuestInfo()->GetQNCode(), tooltip_message);
    ui_tooltip_manager->RegisterTooltip(tooltip_message, 
        -1, 
        22, 
        StrToWzID("s210"),
#ifdef _NA_004644_20110506_QUEST_NAVIGATION
        colors[color_number],
#else
        WzColor_RGBA(75, 255, 45, 255),
#endif//_NA_004644_20110506_QUEST_NAVIGATION
        TOOLTIP_BG_WZCOLOR,
        c_dwTOOLTIP_EXTRA_INFO_DT_LEFT,
        sp_tooltip);
    ui_tooltip_manager->AddEmptyLineForTooltip();

    QuestTextInfo* quest_text_info = QuestTextInfoParser::Instance()->GetQuestTextInfo(quest->GetQuestInfo()->GetSCode_Brief());
    if (quest_text_info == NULL)
    {
        return;
    }

    SUN_STRING strTemp = quest_text_info->m_pszText;
    FTextParser ftext_parser;
    FTextSentence ftext_sentence;
    ZeroMemory(tooltip_message, sizeof(tooltip_message));
    if (ftext_parser.Parser(strTemp, ftext_sentence))
    {
        for(int index = 0; index < ftext_sentence.Size(); ++index)
        {
            strcat(tooltip_message, ftext_sentence.GetParam(index).m_StrContents.c_str());
        }
    }
    ui_tooltip_manager->RegisterTooltipLineChange(tooltip_message,
        _tcslen(tooltip_message),
        20,
        StrToWzID("s210"), 
        WzColor_RGBA(213,201,171,255),
        TOOLTIP_BG_WZCOLOR,
        c_dwTOOLTIP_EXTRA_INFO_DT_LEFT);

    //////////////////////////////////////////////////////////////////////////
	ui_tooltip_manager->SetTooltipDialogID( quest_list_dialog->GetDialogKey() );
    AddTooltip_QuestState(quest);

    //////////////////////////////////////////////////////////////////////////
    QuestInfo* quest_info = g_QuestInfoManager.FindQuestInfo(static_cast<QCODE>(navigation_quest_code));
    if (quest_info != NULL)
    {
        BASE_NPCINFO* quest_start_npc = NPCInfoParser::Instance()->GetNPCInfo(quest_info->GetNPCCode_Start());
        BASE_NPCINFO* quest_reward_npc = NPCInfoParser::Instance()->GetNPCInfo(quest_info->GetNPCCode_Reward());

        TCHAR temp_string[INTERFACE_STRING_LENGTH + 1] = {0, };

        if (quest_start_npc != NULL)
        {
            g_InterfaceManager.GetInterfaceString(quest_start_npc->m_NCode, temp_string, MAX_PATH);
            _sntprintf(tooltip_message, INTERFACE_STRING_LENGTH, "%s %s", g_InterfaceManager.GetInterfaceString(1429), temp_string);		
            ui_tooltip_manager->RegisterTooltip(tooltip_message, 
                -1,
                20,
                StrToWzID("s210"),
                WzColor_RGBA(213, 201, 171, 255), 
                TOOLTIP_BG_WZCOLOR, 
                c_dwTOOLTIP_EXTRA_INFO_DT_LEFT);
        }

        if (quest_reward_npc != NULL)
        {
            g_InterfaceManager.GetInterfaceString(quest_reward_npc->m_NCode, temp_string, MAX_PATH);
            _sntprintf(tooltip_message, INTERFACE_STRING_LENGTH, "%s %s", g_InterfaceManager.GetInterfaceString(1430), temp_string);		
            ui_tooltip_manager->RegisterTooltip(tooltip_message, 
                -1,
                20,
                StrToWzID("s210"),
                WzColor_RGBA(213, 201, 171, 255), 
                TOOLTIP_BG_WZCOLOR, 
                c_dwTOOLTIP_EXTRA_INFO_DT_LEFT);
        }
        ui_tooltip_manager->AddEmptyLineForTooltip();
    }

}

void uiContinentNavigation::RenderHeroLocation()
{

}

void uiContinentNavigation::AddTooltip_QuestState(Quest* quest)
{
    QUEST_COND_HASH& complete_condition_hash = static_cast<QUEST_COND_HASH&>(quest->GetCompleteConditionHash());
    QUEST_COND_HASH_IT complete_condition_hash_itr = complete_condition_hash.begin(); 
    QuestCondition * quest_condition = NULL;
    while (complete_condition_hash_itr != complete_condition_hash.end())
    {
        quest_condition = (*complete_condition_hash_itr);
        if (quest_condition == NULL)
        {
            continue;
        }

        switch (quest_condition->GetType())
        {
        case eQUEST_CONDITION_ITEM:			
            {
                AddTooltipLine_ConditionItem(quest_condition);
            }
            break;
        case eQUEST_CONDITION_MONEY:
            {
                AddTooltipLine_ConditionMoney(quest_condition);
            }
            break;
        case eQUEST_CONDITION_KILLMONSTER:
            {
                AddTooltipLine_ConditionKillMon(quest_condition);
            }
            break;
        case eQUEST_CONDITION_MISSION:
            {
                AddTooltipLine_ConditionMission(quest_condition);
                break;
            }
        case eQUEST_CONDITION_QUEST:
            {
                AddTooltipLine_ConditionQuest(quest_condition);
            }
            break;
        case eQUEST_CONDITION_CLASSITEM:
        case eQUEST_CONDITION_CHARLV:
        case eQUEST_CONDITION_CHARCLASS:
        case eQUEST_CONDITION_REPEAT:
        default:
            break;
        }

        ++complete_condition_hash_itr;
    }
}


void uiContinentNavigation::AddTooltipLine_ConditionItem(QuestCondition* quest_condition)
{
    QUEST_CONDITION_ITEM* condition_item = static_cast<QUEST_CONDITION_ITEM*>(quest_condition);
    if (condition_item == NULL)
    {
        return;
    }    

    TCHAR tooltip_message[INTERFACE_STRING_LENGTH + 1] = {0, };
    TCHAR item_name[INTERFACE_STRING_LENGTH + 1] = {0, };
    WzColor tooltip_color;

    for (int index = 0; index < condition_item->GetInfo()->GetItemArrayCount(); ++index)
    {
        if (condition_item->GetInfo()->IsNecessaryItem())
        {
            CODETYPE require_item_code = condition_item->GetInfo()->GetItemCode(index);			
            WORD require_number = condition_item->GetInfo()->GetItemNum(index);      

            if (require_item_code == 0) 
            {
                continue;
            }

            BASE_ITEMINFO* itemInfo = ItemInfoParser::Instance()->GetItemInfo(require_item_code);
            if (itemInfo == NULL)
            {
                assert(itemInfo);
                continue;
            }

            WORD item_number = ItemManager::Instance()->GetItemCount(SI_INVENTORY, require_item_code);

            // [어떤 아이템]    min(require_number, item_number) / item_number; 

            g_InterfaceManager.GetInterfaceString(itemInfo->m_NCode, item_name, INTERFACE_STRING_LENGTH);
            Sprintf(tooltip_message, 
                _T("    %s    (%d / %d)"),
                item_name,
                min(require_number, item_number), 
                require_number);

            if (RC::RC_QUEST_SUCCESS == condition_item->CanComplete(g_pHero))
            {
                tooltip_color = WzColor_RGBA(150,150,150,255);
            }
            else
            {
                tooltip_color = WzColor_RGBA(255,255,255,255);
            }  		

            uiToolTipMan* pUIMan = (uiToolTipMan*)g_InterfaceManager.GetUserInterfaceManager(UIMAN_TOOLTIP);
            if(pUIMan)
            {
                pUIMan->RegisterTooltip(tooltip_message, 
                    -1,
                    20,
                    StrToWzID("s210"),
                    tooltip_color, 
                    TOOLTIP_BG_WZCOLOR, 
                    c_dwTOOLTIP_EXTRA_INFO_DT_LEFT);
            }
        }
    }
}

void uiContinentNavigation::AddTooltipLine_ConditionMoney(QuestCondition* quest_condition)
{
    if (g_pHero == NULL)
    {
        return;
    }

    QUEST_CONDITION_MONEY* condition_money = static_cast<QUEST_CONDITION_MONEY*>(quest_condition);
    if (condition_money == NULL)
    {
        return;
    }

    MONEY require_money = condition_money->GetInfo()->GetNeedMoney();

    TCHAR tooltip_message[INTERFACE_STRING_LENGTH + 1] = {0, };
    TCHAR temp_string[INTERFACE_STRING_LENGTH + 1] = {0, };
    TCHAR money_string[INTERFACE_STRING_LENGTH + 1] = {0, };
    WzColor tooltip_color;

    // 9049 필요 하임 : %I64u
    g_InterfaceManager.GetInterfaceString(9049, money_string, INTERFACE_STRING_LENGTH);
    Sprintf(temp_string, _T("    "));
    strcat(temp_string, money_string);
    Sprintf(tooltip_message, temp_string, require_money);

    if (g_pHero->GetMoney() >= require_money)
    {
        tooltip_color = WzColor_RGBA(150,150,150,255);
    }
    else
    {
        tooltip_color = WzColor_RGBA(255,255,255,255);
    }

    uiToolTipMan* pUIMan = (uiToolTipMan*)g_InterfaceManager.GetUserInterfaceManager(UIMAN_TOOLTIP);
    if(pUIMan)
    {
        pUIMan->RegisterTooltip(tooltip_message, 
            -1,
            20,
            StrToWzID("s210"),
            tooltip_color, 
            TOOLTIP_BG_WZCOLOR, 
            c_dwTOOLTIP_EXTRA_INFO_DT_LEFT);
    }
}

void uiContinentNavigation::AddTooltipLine_ConditionKillMon(QuestCondition* quest_condition)
{
    if (g_pHero == NULL)
    {
        return;
    }

    QUEST_CONDITION_KILLMONSTER* condition_kill_monster = static_cast<QUEST_CONDITION_KILLMONSTER*>(quest_condition);
    if (condition_kill_monster == NULL)
    {
        return;
    }

    TCHAR tooltip_message[INTERFACE_STRING_LENGTH + 1] = {0, };
    TCHAR monster_name[INTERFACE_STRING_LENGTH + 1] = {0, };
    WzColor tooltip_color;

    for ( int index = 0; index < QUEST_CONDITION_KILLMONSTER::MAX_MONSTER_KIND_NUM; ++index)
    {
        DWORD monster_code = condition_kill_monster->GetInfo()->GetMonsterCode(index);
        if (monster_code == 0)
        {
            break;
        }

        WORD require_kill_number = condition_kill_monster->GetInfo()->GetMonsterNum(index);
        WORD current_kill_number = condition_kill_monster->GetCurMonsterNum(index);

        if (RC::RC_QUEST_SUCCESS == condition_kill_monster->CanComplete(g_pHero))
        {
            tooltip_color = WzColor_RGBA(150,150,150,255);
        }
        else
        {
            tooltip_color = WzColor_RGBA(255,255,255,255);
        }  

        // [어떤 몬스터] (%d/%d)
        BASE_NPCINFO* npc_info = NPCInfoParser::Instance()->GetNPCInfo(monster_code);

        if (npc_info != NULL)
        {
            g_InterfaceManager.GetInterfaceString(npc_info->m_NCode, monster_name, INTERFACE_STRING_LENGTH);

            Sprintf(tooltip_message, 
                _T("    %s    ( %d / %d )"), 
                monster_name, 
                min(current_kill_number,require_kill_number), 
                require_kill_number );

            uiToolTipMan* pUIMan = (uiToolTipMan*)g_InterfaceManager.GetUserInterfaceManager(UIMAN_TOOLTIP);
            if(pUIMan)
            {
                pUIMan->RegisterTooltip(tooltip_message, 
                    -1,
                    20,
                    StrToWzID("s210"),
                    tooltip_color, 
                    TOOLTIP_BG_WZCOLOR, 
                    c_dwTOOLTIP_EXTRA_INFO_DT_LEFT);
            }
        }

    }    
}

void uiContinentNavigation::AddTooltipLine_ConditionMission(QuestCondition* quest_condition)
{
    QUEST_CONDITION_MISSION* condition_mission = static_cast<QUEST_CONDITION_MISSION*>(quest_condition);
    if (condition_mission == NULL)
    {
        return;
    }

    TCHAR tooltip_message[INTERFACE_STRING_LENGTH + 1] = {0, };
    TCHAR mission_name[INTERFACE_STRING_LENGTH + 1] = {0, };
    WzColor tooltip_color;

    for ( int index = 0; index < QUEST_CONDITION_MISSION_INFO::_MAX_CHECK_NUM; ++index)
    {
        MAPCODE mission_code = condition_mission->GetInfo()->GetMissionCode(index);
        if (mission_code == 0)
        {
            continue;
        }

        WORD require_mission_point = condition_mission->GetInfo()->GetMissionNum(index);
        WORD current_mission_point = g_pMission_Concrete->GetMissionPoint(static_cast<BYTE>(mission_code));

        if (RC::RC_QUEST_SUCCESS == condition_mission->CanComplete(g_pHero))
        {
            tooltip_color = WzColor_RGBA(150,150,150,255);
        }
        else
        {
            tooltip_color = WzColor_RGBA(255,255,255,255);
        }  

        // [어떤 미션] (%d/%d)

        const sMAPINFO * map_info = MapInfoParser::Instance()->FindMapInfo(mission_code);
        if (map_info != NULL)
        {
            g_InterfaceManager.GetInterfaceString(map_info->dwNCode, mission_name, INTERFACE_STRING_LENGTH);

            Sprintf(tooltip_message, 
                _T("    %s    ( %d / %d )"), 
                mission_name, 
                min(current_mission_point,require_mission_point), 
                require_mission_point );

            uiToolTipMan* pUIMan = (uiToolTipMan*)g_InterfaceManager.GetUserInterfaceManager(UIMAN_TOOLTIP);
            if(pUIMan)
            {
                pUIMan->RegisterTooltip(tooltip_message, 
                    -1,
                    20,
                    StrToWzID("s210"),
                    tooltip_color, 
                    TOOLTIP_BG_WZCOLOR, 
                    c_dwTOOLTIP_EXTRA_INFO_DT_LEFT);

            }	
        }
    }    		
}

void uiContinentNavigation::AddTooltipLine_ConditionQuest(QuestCondition* quest_condition)
{
    QUEST_CONDITION_QUEST* condition_quest = static_cast<QUEST_CONDITION_QUEST*>(quest_condition);
    if (condition_quest == NULL)
    {
        return;
    }    

    TCHAR tooltip_message[INTERFACE_STRING_LENGTH + 1] = {0, };
    TCHAR quest_name[INTERFACE_STRING_LENGTH + 1] = {0, };
    TCHAR success_string[INTERFACE_STRING_LENGTH + 1] = {0, };
    WzColor tooltip_color;

    for (int index = 0; index < condition_quest->GetInfo()->GetQuestArrayCount(); ++index)
    {
        QCODE quest_code = condition_quest->GetInfo()->GetQuestCode(index);
        if (quest_code == 0)
        {
            break;
        }

        Quest* quest_real = g_pQMGR_Real->FindQuest(quest_code);
        Quest* quest_temp = g_pQMGR_Temp->FindQuest(quest_code);
        Quest* current_quest = NULL;
        if (quest_real != NULL)
        {
            current_quest = quest_real;
        }
        else if(quest_temp != NULL)
        {
            current_quest = quest_temp;
        }

        if(current_quest != NULL)
        {
            QUESTPART_COMPL* quest_complete_num = g_pQMGR_Real->FindComplQuest(quest_code);
            if ((quest_complete_num != NULL) && 
                (quest_complete_num->m_Num > 0))
            {
                tooltip_color = WzColor_RGBA(150,150,150,255);
                //9051	(완료)
                g_InterfaceManager.GetInterfaceString(9051, success_string);
            }
            else
            {
                tooltip_color = WzColor_RGBA(255,255,255,255);
            }  

            g_InterfaceManager.GetInterfaceString(current_quest->GetQuestInfo()->GetQNCode(), quest_name, INTERFACE_STRING_LENGTH);

            Sprintf(tooltip_message, 
                _T("    %s%s"), 
                quest_name,
                success_string);

            uiToolTipMan* pUIMan = (uiToolTipMan*)g_InterfaceManager.GetUserInterfaceManager(UIMAN_TOOLTIP);
            if(pUIMan)
            {
                pUIMan->RegisterTooltip(tooltip_message, 
                    -1,
                    20,
                    StrToWzID("s210"),
                    tooltip_color, 
                    TOOLTIP_BG_WZCOLOR, 
                    c_dwTOOLTIP_EXTRA_INFO_DT_LEFT);
            }
        }
    }    
}
