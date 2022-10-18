#include "SunClientPrecompiledHeader.h"
#ifdef _NA_003027_20111013_HONOR_SYSTEM

#include "../ProgramCommon/HonorSystem/HonorSystemPacket.h"
#include "HonorSystemFunction.h"
#include "HonorSystem.h"
#include "GlobalFunc.h"
#include "ObjectManager.h"
#include "Hero.h"
#include "Player.h"
#include "TreeMenuGroup.h"
#include "TreeMenuInfoParser.h"
#include "TextureListInfoHandleManager.h"
#include "TitleListParser.h"
#include "InterfaceManager.h"
#include "uiCharacterStatus/uiCharacterStatusTitleList.h"

namespace HonorSystem{

namespace HonorSystemFunction
{

    void UseTitleNameForHero(WORD title_code)
    {
        if (g_pHero == NULL)
        {
            return;
        }

        if (GENERALPARAM->IsNet() == TRUE)
        {
            PACKET_CG_HONORSYSTEM_TITLE_CHANGE_CMD title_change_cmd(title_code);
            GlobalFunc::SendPacket(CI_GAMESERVERIDX, &title_change_cmd, sizeof(title_change_cmd));
        }
        else
        {
            if (g_pHero != NULL)
            {
                SetTitleName(g_pHero->GetObjectKey(), title_code);
            }
        }
    }

    void SetTitleName(DWORD player_key, WORD title_code)
    {
        Player* player_ptr = static_cast<Player*>(g_ObjectManager.GetObject(player_key));
        if (player_ptr != NULL)
        {
            player_ptr->SetTitleName(title_code);
        }
    }

    void OnChangeTitle(WORD title_code)
    {
        // 타이틀 리스트 UI 갱신
        uiCharacterStatusTitleList* ui_title_list = GET_CAST_DIALOG(uiCharacterStatusTitleList, uiCharacterStatusTitleList::kDialog_ID);
        if (ui_title_list != NULL)
        {
            ui_title_list->OnChangeTitle(title_code);
        }
    }

    void OnRewardTitle(WORD titile_code)
    {
        if (titile_code == 0)
        {
            return;
        }

        TitleListParser::TitleInfo* title_info = TitleListParser::Instance()->GetTitleInfo(titile_code);
        if (title_info == NULL)
        {
            return;
        }

        TCHAR system_message[INTERFACE_STRING_LENGTH] = {'\0', };
        TCHAR title_name[INTERFACE_STRING_LENGTH] = {'\0', };
        g_InterfaceManager.GetInterfaceString(title_info->title_name_code, title_name);
        g_InterfaceManager.GetInterfaceStringFormat(system_message, INTERFACE_STRING_LENGTH - 1, 6767, title_name);
        GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, system_message);
    }

    void OnAddTitle()
    {

    }

    void OnRemoveTitle()
    {

    }

    void RefreshTitleListMenu()
    {
        TreeMenuInfoParser::Instance()->Remove(BASE_TreeMenuInfo::kGroupType_Title_List);

        std::vector<BASE_TreeMenuInfo*> template_info_container;
        TreeMenuInfoParser::Instance()->GetTreeMenu(template_info_container, 
            BASE_TreeMenuInfo::kGroupType_Title_List_Template);

        if (template_info_container.begin() == template_info_container.end())
        {
            return;
        }

        BASE_TreeMenuInfo* template_info = template_info_container.front();
        if (template_info == NULL)
        {
            return;
        }

        BASE_TreeMenuInfo new_menu_info = (*template_info);
        new_menu_info.tree_group = BASE_TreeMenuInfo::kGroupType_Title_List;
        WORD title_code;
        DWORD title_string_code;
        TitleListParser::TitleInfo* title_info;

        HonorSystemData::TitleCodeContainer& title_code_contaioner = HonorSystem::GetHonorSystemData().title_code_contaioner();

        // 칭호없음 메뉴 삽입
        new_menu_info.menu_index = HonorSystem::kNoTitleMenuIndex;
        new_menu_info.string_code = HonorSystem::kNoTitleMenuStringCode;
        TreeMenuInfoParser::Instance()->Insert(new_menu_info);

        HonorSystemData::TitleCodeContainer::iterator begin_itr = title_code_contaioner.begin();
        HonorSystemData::TitleCodeContainer::iterator end_itr = title_code_contaioner.end();
        for ( ; begin_itr != end_itr; ++begin_itr)
        {
            title_code = (*begin_itr);
            if (title_code == 0)
            {
                continue;
            }

            // 메뉴 인덱스는 타이틀코드를 사용
            new_menu_info.menu_index = title_code;

            // 메뉴 스트링코드 세팅
            title_info = TitleListParser::Instance()->GetTitleInfo(title_code);
            if (title_info != NULL &&
                title_info->title_name_code != 0)
            {
                title_string_code = title_info->title_name_code;
            }
            else
            {
                continue;
            }
            
            new_menu_info.string_code = title_string_code;

            // 세팅된 정보 삽입
            TreeMenuInfoParser::Instance()->Insert(new_menu_info);
        }
    }

    void RefreshHonorPointListMenu()
    {
        {
            TreeMenuInfoParser::Instance()->Remove(BASE_TreeMenuInfo::kGroupType_Honor_Info);

            std::vector<BASE_TreeMenuInfo*> template_info_container;
            TreeMenuInfoParser::Instance()->GetTreeMenu(template_info_container, 
                BASE_TreeMenuInfo::kGroupType_Honor_Info_Template);

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


            // 명성 넣기
            HonorSystemData::FamePointInfoContainer& fame_point_info_container = HonorSystem::GetHonorSystemData().fame_point_info_container();

            // 명성 폴더 정보
            BASE_TreeMenuInfo new_folder_menu_info = (*template_folder_info);
            new_folder_menu_info.tree_group = BASE_TreeMenuInfo::kGroupType_Honor_Info;
            new_folder_menu_info.string_code = 47649;
            TreeMenuInfoParser::Instance()->Insert(new_folder_menu_info);

            // 명성 수치 정보
            BASE_TreeMenuInfo new_menu_info = (*template_info);
            new_menu_info.tree_group = BASE_TreeMenuInfo::kGroupType_Honor_Info;
            HonorSystemData::FamePointInfoContainer::iterator fame_begin_itr = fame_point_info_container.begin();
            HonorSystemData::FamePointInfoContainer::iterator fame_end_itr = fame_point_info_container.end();
            for ( ; fame_begin_itr != fame_end_itr; ++fame_begin_itr)
            {
                FamePointInfo& fame_point_info = (fame_begin_itr->second);
                new_menu_info.menu_depth[2] = (BYTE)(fame_point_info.fame_code);
                new_menu_info.string_code = fame_point_info.fame_info->fame_name_code;
                new_menu_info.user_data = (DWORD)(&fame_point_info);
                TreeMenuInfoParser::Instance()->Insert(new_menu_info);
            }

            // 평판 넣기
            HonorSystemData::ReputePointInfoContainer& repute_point_info_container = HonorSystem::GetHonorSystemData().repute_point_info_container();

            // 폴더 넣기
            new_folder_menu_info = (*template_folder_info);
            new_folder_menu_info.tree_group = BASE_TreeMenuInfo::kGroupType_Honor_Info;
            ReputeListParser::ReputeGroupInfoMap& repute_group_info_map = ReputeListParser::Instance()->GetReputeGroupInfoMap();
            ReputeListParser::ReputeGroupInfoMap::iterator repute_group_info_itr = repute_group_info_map.begin();
            ReputeListParser::ReputeGroupInfoMap::iterator repute_group_info_end_itr = repute_group_info_map.end();
            for ( ; repute_group_info_itr != repute_group_info_end_itr; ++repute_group_info_itr)
            {
                ReputeListParser::ReputeGroupInfo* repute_group_info = repute_group_info_itr->second;
                if (repute_group_info == NULL)
                {
                    continue;
                }

                WORD repute_group_code = repute_group_info->repute_group_code;
                DWORD repute_group_name_code = repute_group_info->repute_group_name_code;
                new_folder_menu_info.menu_depth[3] = (BYTE)repute_group_code;
                new_folder_menu_info.string_code = repute_group_name_code;
                TreeMenuInfoParser::Instance()->Insert(new_folder_menu_info);

                // 평판 수치 정보
                new_menu_info = (*template_info);
                new_menu_info.tree_group = BASE_TreeMenuInfo::kGroupType_Honor_Info;
                new_menu_info.menu_depth[3] = (BYTE)repute_group_code;

                STLX_SET<WORD>::iterator repute_code_itr = repute_group_info->repute_code_set.begin();
                STLX_SET<WORD>::iterator repute_code_end_itr = repute_group_info->repute_code_set.end();
                for ( ; repute_code_itr != repute_code_end_itr; ++repute_code_itr)
                {
                    HonorSystemData::ReputePointInfoContainer::iterator found_itr = repute_point_info_container.find((*repute_code_itr));
                    if (found_itr == repute_point_info_container.end())
                    {
                        continue;
                    }

                    ReputePointInfo& repute_point_info = found_itr->second;
                    
                    new_menu_info.menu_depth[2] = (BYTE)repute_point_info.repute_code;
                    new_menu_info.string_code = repute_point_info.repute_info->repute_name_code;
                    new_menu_info.user_data = (DWORD)(&repute_point_info);
                    TreeMenuInfoParser::Instance()->Insert(new_menu_info);
                }
            }
        }
    }

    void OnChangeFamePoint(WORD fame_code, DWORD changed_point)
    {
        HonorSystem::FamePointInfo* fame_point_info = HonorSystem::GetHonorSystemData().GetFamePointInfo(fame_code);
        if (fame_point_info == NULL)
        {
            return;
        }

        DWORD current_point = fame_point_info->fame_point_value;
        int difference_point = changed_point - current_point;
        if (difference_point < 1)
        {
            return;
        }

        TCHAR system_message[INTERFACE_STRING_LENGTH] = {'\0', };
        TCHAR fame_name_string[INTERFACE_STRING_LENGTH] = {'\0', };
        TCHAR temp_string[INTERFACE_STRING_LENGTH] = {'\0', };
        g_InterfaceManager.GetInterfaceString(fame_point_info->fame_info->fame_name_code, fame_name_string);
        Snprintf(temp_string, INTERFACE_STRING_LENGTH - 1, "%d", (changed_point - current_point));
        g_InterfaceManager.GetInterfaceStringFormat(system_message, INTERFACE_STRING_LENGTH - 1, 6512, fame_name_string, temp_string);
        GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, system_message);

        WORD before_rating = FameListParser::GetFameRating(fame_point_info->fame_info, fame_point_info->fame_point_value);
        WORD after_rating = FameListParser::GetFameRating(fame_point_info->fame_info, changed_point);
        if (before_rating < after_rating)
        {
            g_InterfaceManager.GetInterfaceString(6501 + after_rating, temp_string);
            g_InterfaceManager.GetInterfaceStringFormat(system_message, INTERFACE_STRING_LENGTH - 1, 6768, fame_name_string, temp_string);
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, system_message);

            // 명성 등급이 올라갈 때, 이펙트 출력
            if (g_pHero != NULL)
            {
                DWORD effect_id = fame_point_info->fame_info->fame_effectcode[after_rating - 1];
                GlobalFunc::CreateEffectBeUpToOption(effect_id, g_pHero->GetPosition());
            }
        }
    }

    void OnChangeReputePoint(WORD repute_code, DWORD changed_point)
    {
        HonorSystem::ReputePointInfo* repute_point_info = HonorSystem::GetHonorSystemData().GetReputePointInfo(repute_code);
        if (repute_point_info == NULL)
        {
            return;
        }

        DWORD current_point = repute_point_info->repute_point_value;
        int difference_point = changed_point - current_point;
        if (difference_point < 1)
        {
            return;
        }

        TCHAR system_message[INTERFACE_STRING_LENGTH] = {'\0', };
        TCHAR repute_name_string[INTERFACE_STRING_LENGTH] = {'\0', };
        TCHAR temp_string[INTERFACE_STRING_LENGTH] = {'\0', };
        g_InterfaceManager.GetInterfaceString(repute_point_info->repute_info->repute_name_code, repute_name_string);
        Snprintf(temp_string, INTERFACE_STRING_LENGTH - 1, "%d", (changed_point - current_point));
        g_InterfaceManager.GetInterfaceStringFormat(system_message, INTERFACE_STRING_LENGTH - 1, 6513, repute_name_string, temp_string);
        GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, system_message);

        WORD before_rating = ReputeListParser::GetReputeRating(repute_point_info->repute_info, repute_point_info->repute_point_value);
        WORD after_rating = ReputeListParser::GetReputeRating(repute_point_info->repute_info, changed_point);
        if (before_rating < after_rating)
        {
            g_InterfaceManager.GetInterfaceString(6508 + after_rating, temp_string);
            g_InterfaceManager.GetInterfaceStringFormat(system_message, INTERFACE_STRING_LENGTH - 1, 6769, repute_name_string, temp_string);
            GlobalFunc::OutputMsg_Notice(eOUTPUT_MSG_CHAT, system_message);

            // 평판 등급이 올라갈 때, 이펙트 출력
            HonorSystem::FamePointInfo* fame_point_info = HonorSystem::GetHonorSystemData().GetFamePointInfo(repute_point_info->repute_info->fame_area_code);
            if ((g_pHero != NULL) && 
                (fame_point_info != NULL))
            {
                WORD fame_before_rating = FameListParser::GetFameRating(fame_point_info->fame_info, 
                    fame_point_info->fame_point_value);
                WORD fame_after_rating = FameListParser::GetFameRating(fame_point_info->fame_info, 
                    (fame_point_info->fame_point_value + repute_point_info->repute_info->reward_fame_point[after_rating - 1]));
                // 명성이 올라갈 수 있는 상황이면 이펙트 출력하지 않고
                if (fame_before_rating == fame_after_rating)
                {
                    
                    DWORD effect_id = repute_point_info->repute_info->repute_effectcode[after_rating - 1];
                    GlobalFunc::CreateEffectBeUpToOption(effect_id, g_pHero->GetPosition());
                }
            }
        }
    }

};//HonorSystemFunction

};//namespace HonorSystem

#endif //_NA_003027_20111013_HONOR_SYSTEM
