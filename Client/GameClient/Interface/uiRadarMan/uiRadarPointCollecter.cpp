#include "SunClientPrecompiledHeader.h"
#include "uiRadarPointCollecter.h"
#include "uiRadarMan.h"
#include "uiPartyMan/uiPartyMan.h"
#include "uiQuestMan/uiQuestMan.h"
#include "uiQuestListDlg/uiQuestListDlg.h"
#include "uiDominationTowerMan/uiDominationTowerMan.h"
#include "GameFramework.h"
#include "SceneBase.h"
#include "Map.h"
#include "ObjectManager.h"
#include "HeroActionInput.h"
#include "Hero.h"
#include "Player.h"
#include "NPC.h"
#include "Monster.h"
#include "MapObject.h"
#include "GameFunc.h"
#include "GlobalFunc.h"
#include "HeadMeshInfoParser.h"
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
  #include "BattleGroundFlagInfoParser.h"
  #include "ChaosSystem/ChaosSystemFunction.h"
#endif //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND

const float kMonsterDistanceInstanceDungeon = 5.0f;

void uiRadarPointCollecter::Initialize()
{
    BOOST_STATIC_ASSERT(uiRadarPointCollecter::kMaxNaviCount == MAX_NAVI_CNT);

    //메모리풀 초기화
    radar_point_memorypool.Initialize(100);

    //컨테이너 초기화
    radar_point_container_.clear();
    npc_quest_accept_point_container_.clear();
    npc_quest_reward_point_container_.clear();
}

void uiRadarPointCollecter::ClearAll()
{
    //할당된 메모리 해제
    RADAR_POINT_DEQUE_ITR radar_point_itr = radar_point_container_.begin();
    RADAR_POINT_DEQUE_ITR radar_point_itr_end = radar_point_container_.end();
    while (radar_point_itr != radar_point_itr_end)
    {
        radar_point_memorypool.Free((*radar_point_itr));
        ++radar_point_itr;
    }

    //컨테이너 초기화
    radar_point_container_.clear();
    npc_quest_accept_point_container_.clear();
    npc_quest_reward_point_container_.clear();
}

void uiRadarPointCollecter::CollectRadarPoint()
{
    ClearAll();

    if (g_pHero == NULL)
    {
        return;
    }

    if (g_pMap == NULL)
    {
        return;
    }

    uiRadarMan* ui_radar_manager = GET_DIALOG_MANAGER(uiRadarMan, UIMAN_RADAR);
    if (ui_radar_manager == NULL)
    {
        return;
    }

    uiPartyMan* ui_party_manager = GET_DIALOG_MANAGER(uiPartyMan, UIMAN_PARTY);
    if (ui_party_manager == NULL)
    {
        return;
    }

    for(int navi_index = 0; navi_index < kMaxNaviCount; ++navi_index)
    {
        quest_navi_info_[navi_index].Clear();
    }

    WzVector point_position;
    Object* object_ptr;
    STLX_STRING object_name;
    eOBJECT_TYPE object_type;
    eRADAR_POINT_COLOR radar_point_color;

    // 오브젝트 매니저 순회
    Object_Map_Itr object_map_itr = g_ObjectManager.GetBegin();
    Object_Map_Itr object_map_itr_end = g_ObjectManager.GetEnd();
    while (object_map_itr != object_map_itr_end)
    {
        object_ptr = static_cast<Object*>(object_map_itr->second);
        ++object_map_itr;

        if (object_ptr == NULL)
        {
            continue;
        }

        object_type = object_ptr->GetSCObject().GetObjectType();
        switch (object_type)
        {
        case PLAYER_OBJECT:
            {
                // 파티원은 나중에 수집하여 추가
                if ((g_pHero->GetObjectKey() != object_ptr->GetObjectKey()) && 
                    (ui_party_manager->GetPartyMemberInfo(object_ptr->GetObjectKey()) != NULL))
                {
                    continue;
                }
                radar_point_color = GetPointColorPlayer(static_cast<Player*>(object_ptr));
                object_name = object_ptr->GetName();
            }
            break;
        case LOTTO_NPC_OBJECT:
        case NPC_OBJECT:
            {
                radar_point_color = GetPointColorNpc(static_cast<NPC*>(object_ptr));
                object_name = object_ptr->GetName();
            }
            break;
        case SSQMONSTER_OBJECT:
        case MONSTER_OBJECT:
            {
                radar_point_color = GetPointColorMonster(static_cast<Monster*>(object_ptr));
                object_name = object_ptr->GetName();
            }
            break;
        case MAP_OBJECT:
            {   
                if (g_pMap->IsMission() == TRUE)
                {
                    continue;
                }

                radar_point_color = GetPointColorMapObject(static_cast<MapObject*>(object_ptr));
                if (radar_point_color == eRADAR_AIRSHIP_PORTAL)
                {
                    // 비공정포탈
                    object_name = g_InterfaceManager.GetInterfaceString(11476);
                }
                else if (radar_point_color == eRADAR_NORMAL_PORTAL)
                {
                    // 포탈
                    object_name = g_InterfaceManager.GetInterfaceString(1450);
                }
                else if (radar_point_color == eRADAR_MISSION_PORTAL)
                {
                    // 미션포탈
                    object_name = g_InterfaceManager.GetInterfaceString(6830);
                }
                else if (radar_point_color == eRADAR_WAY_POINT)
                {
                    // 웨이포인트
                    object_name = g_InterfaceManager.GetInterfaceString(6831);
                }
                else if (radar_point_color == eRADAR_AIRSHIP)
                {
                    // 피의 성전 타워
                    object_name = g_InterfaceManager.GetInterfaceString(3312);
                }
#ifdef _NA_0_20110327_DISPLAY_COLLECTABLE_OBJECT_IN_RADAR
                else if (radar_point_color == eRADAR_COLLECT_TARGET)
                {
                    object_name = object_ptr->GetName();
                }
#endif //_NA_0_20110327_DISPLAY_COLLECTABLE_OBJECT_IN_RADAR
#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
                else if ((radar_point_color >= eRADAR_BATTLE_GROUND_STRONGPOINT_NORMAL) &&
                    (radar_point_color <= eRADAR_BATTLE_GROUND_STRONGPOINT_BLUE))
                {
                    MapObject* map_object = static_cast<MapObject*>(object_ptr);
                    DWORD name_code = BattleGroundFlagInfoParser::Instance()->GetStrongPointName(
                        g_pMap->GetCurrentFieldID(), map_object->GetObjectKey());
                    object_name = g_InterfaceManager.GetInterfaceString(name_code);
                }
#endif //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
                else
                {
                    continue;
                }
            }
            break;
        default:
            {
                continue;
            }
        }

        if (radar_point_color == eRADAR_NOT_FOUND_OBJECT)
        {
            continue;
        }

        // 좌표 계산 후 포인터 삽입
        point_position = object_ptr->GetVisiblePos();
        point_position = CalculatePointPosition(point_position);

        RADAR_POINT* rardar_point = NewRadarPoint();
        if (rardar_point == NULL)
        {
            break;
        }
        rardar_point->dwObjectKey = object_ptr->GetObjectKey();
        rardar_point->eRadarPointColor = radar_point_color;
        rardar_point->fPosX = point_position.x;
        rardar_point->fPosY = point_position.y;
#ifdef _DH_NPC_GROUP_NAME
        NPC* npc_object_ptr = static_cast<NPC*>(object_ptr);
        if (npc_object_ptr && 
            (object_type == NPC_OBJECT || object_type == MONSTER_OBJECT || object_type == SSQMONSTER_OBJECT))//GName는 몬스터와 NPC만된다. 맵 오브젝트를 하려면 추가작업 필요
        {
            TCHAR temp[INTERFACE_STRING_LENGTH] = {0,};
            BASE_NPCINFO* npc_info = npc_object_ptr->GetNpcInfo();
            if (npc_info && npc_info->group_name != 0)
            {
                g_InterfaceManager.GetInterfaceString(npc_info->group_name, temp);
                Snprintf(rardar_point->szName, MAX_PATH-1, "<%s> %s", temp, object_name.c_str());
            }
            else
            {
                StrnCopy(rardar_point->szName, object_name.c_str(), MAX_PATH-1);
            }
        }
        else
#endif//_DH_NPC_GROUP_NAME
        {
            StrnCopy(rardar_point->szName, object_name.c_str(), MAX_PATH - 1);
        }

        rardar_point->szName[_countof(rardar_point->szName) - 1] = '\0';

        InsertRadarPoint(rardar_point);

        // 순회중 네비 정보 수집
        CollectQuestNaviInfo(rardar_point, object_ptr);
    }

    // 수집된 네비 정보 이용하여 포인트 추가
    CollectQuestNaviPoint();

    // 파티원 포인트 추가
    CollectPartyNaviPoint();

    // 수집된 포인트 병합
    MergeCollectPoint();
}

void uiRadarPointCollecter::CollectQuestNaviPoint()
{
    // 오브젝트매니저에 있는 경우
    for (int navi_index = 0; navi_index < kMaxNaviCount; ++navi_index)
    {
        if (quest_navi_info_[navi_index].navi_object_key == 0)
        {
            continue;
        }

        RADAR_POINT* rardar_point = NewRadarPoint();
        if (rardar_point == NULL)
        {
            break;
        }

        rardar_point->dwObjectKey = quest_navi_info_[navi_index].navi_object_key;
        rardar_point->eRadarPointColor = quest_navi_info_[navi_index].navi_point_color;
        rardar_point->fPosX = quest_navi_info_[navi_index].navi_position_x;
        rardar_point->fPosY = quest_navi_info_[navi_index].navi_position_y;
        StrnCopy(rardar_point->szName, quest_navi_info_[navi_index].navi_quest_name.c_str(), MAX_PATH - 1);
        rardar_point->szName[_countof(rardar_point->szName) - 1] = '\0';

        InsertRadarPoint(rardar_point);
    }

    // 오브젝트매니저에 없는 경우(서버로 부터 받은 정보를 기반으로 수집)
    uiQuestMan* ui_quest_manager = GET_DIALOG_MANAGER(uiQuestMan, UIMAN_QUEST);
    if (ui_quest_manager == NULL)
    {
        return;
    }

    uiQuestListDlg* quest_list_dialog = GET_CAST_DIALOG(uiQuestListDlg, IM_QUEST_MANAGER::QUEST_LIST_DLG);
    if (quest_list_dialog == NULL)
    {
        return;
    }

    KILL_MON_QUEST_INFO_4_NAVI* quest_navi_info = NULL;
    Quest* quest = NULL;
    QUEST_CONDITION_KILLMONSTER_INFO* quest_condition_info = NULL;

    int count_of_quest_navi = ui_quest_manager->GetKillMonQuest4NaviCnt();
	int navi_index;
	int navi_info_index;
    for (navi_index = 0; navi_index < count_of_quest_navi; ++navi_index)
    {
        quest_navi_info = ui_quest_manager->GetKillMonQuest4Navi(navi_index);
        if (quest_navi_info == NULL)
        {
            continue;
        }

        // 퀘스트가 없으면 스킵
        quest = g_pQMGR_Real->FindQuest(static_cast<QCODE>(quest_navi_info->dwQuestCode));
        if (quest == NULL)
        {
            continue;
        }

        // 퀘스트가 완료되었으면 스킵
        if (quest->GetQState() == QS_COMPLETED)
        {
            continue;
        }

        // 몬스터사냥 조건이 없으면 스킵
        quest_condition_info = static_cast<QUEST_CONDITION_KILLMONSTER_INFO*>(quest->GetQuestInfo()->FindCompleteCondition(eQUEST_CONDITION_KILLMONSTER));
        if (quest_condition_info == NULL)
        {
            continue;
        }

        for(int navi_info_index = 0; navi_info_index < kMaxNaviCount; ++navi_info_index)
        {
            if(quest_navi_info->dwQuestCode == quest_list_dialog->GetNaviQCodes(navi_info_index))
            {
                break;
            }
        }

        if (navi_info_index >= kMaxNaviCount)
        {
             continue;
        }

        QUEST_CONDITION_KILLMONSTER* quest_condition = NULL;
        QUEST_COND_HASH& quest_condition_hash = quest->GetCompleteConditionHash();
        QUEST_COND_HASH_IT quest_condition_hash_itr = quest_condition_hash.begin(); 
        while (quest_condition_hash_itr != quest_condition_hash.end())
        {
            if ((*quest_condition_hash_itr)->GetType() == eQUEST_CONDITION_KILLMONSTER)
            {
                quest_condition = static_cast<QUEST_CONDITION_KILLMONSTER*>(*quest_condition_hash_itr);
                break;
            }
            ++quest_condition_hash_itr;
        }

        if (quest_condition == NULL)
        {
            continue;
        }
        
        // 조건은 만족했으면 스킵
        for (int condition_index = 0; condition_index < QUEST_CONDITION_KILLMONSTER_INFO::_MAX_CHECK_NUM; ++condition_index)
        {
            if (quest_navi_info->dwMonCode[condition_index] == 0)
            {
                continue;
            }

            if(quest_condition->GetCurMonsterNum(condition_index) < quest_condition_info->GetMonsterNum(condition_index))
            {
                break;
            }
        }
		int condition_index;
        if (condition_index >= QUEST_CONDITION_KILLMONSTER_INFO::_MAX_CHECK_NUM)
        {
            continue;
        }

        // 오브젝트매니저에 있으면 스킵
        Object* object_ptr = g_ObjectManager.GetObject(quest_navi_info_[navi_info_index].navi_object_key);
        if ((object_ptr != NULL) && 
            (object_ptr->IsKindOfObject(MONSTER_OBJECT) == TRUE))
        {
            Monster* monster_ptr = static_cast<Monster*>(object_ptr);
            if (monster_ptr->GetMonsterInfo()->m_MonsterCode == quest_navi_info->dwMonCode[condition_index])
            {
                continue;
            }
        }

        WzVector monster_position;
        monster_position.x = static_cast<float>(quest_navi_info->ptMonPos[condition_index].x);
        monster_position.y = static_cast<float>(quest_navi_info->ptMonPos[condition_index].y);
        monster_position.z = 0.0f;
        monster_position = CalculatePointPosition(monster_position);

        RADAR_POINT* rardar_point = NewRadarPoint();
        if (rardar_point == NULL)
        {
            break;
        }
        rardar_point->dwObjectKey = 0;
        rardar_point->eRadarPointColor = static_cast<eRADAR_POINT_COLOR>(eRADAR_NAVI_POINT_RED + navi_info_index);
        rardar_point->fPosX = monster_position.x;
        rardar_point->fPosY = monster_position.y;
        g_InterfaceManager.GetInterfaceString(quest->GetQuestInfo()->GetQNCode(), rardar_point->szName, MAX_PATH -1);

        InsertRadarPoint(rardar_point);
    }
}

void uiRadarPointCollecter::CollectPartyNaviPoint()
{
    uiPartyMan* ui_party_manager = GET_DIALOG_MANAGER(uiPartyMan, UIMAN_PARTY);
    if (ui_party_manager == NULL)
    {
        return;
    }

    if (g_pHero->IsParty() == FALSE)
    {
        return;	
    }

    PartyNaviInfo* party_navi_info = ui_party_manager->GetPartyNaviInfo();
    if (party_navi_info == NULL)
    {
        return;
    }

    STLX_VECTOR<uicPartyMemberInfoPlus>& party_info_list = ui_party_manager->GetPartyListRef();
    STLX_VECTOR<uicPartyMemberInfoPlus>::iterator party_info_itr = party_info_list.begin();
    STLX_VECTOR<uicPartyMemberInfoPlus>::iterator party_info_itr_end = party_info_list.end();
    NAVINODE navi_node;
    DWORD member_object_key;
    bool is_dead;
    bool return_value;
    Object* party_member_object;
    WzVector navi_position;

    for ( ;party_info_itr != party_info_itr_end; ++party_info_itr)
    {
        member_object_key = party_info_itr->Get().m_dwMemberKey;
        if (g_pHero->GetObjectKey() == member_object_key)
        {
            continue;
        }

        is_dead = party_info_itr->IsDead();
        return_value = party_navi_info->GetNaviInfo(member_object_key, &navi_node);
        party_member_object = g_ObjectManager.GetObject(member_object_key);

        if (party_member_object != NULL)
        {
            navi_position = party_member_object->GetVisiblePos();
        }
        else if ((return_value == true) && 
            (g_pMap->GetCurrentFieldID() == navi_node.fieldCode))
        {
            navi_position.x = navi_node.posX;
            navi_position.y = navi_node.posY;
            navi_position.z = navi_node.posZ;
        }
        else
        {
            continue;
        }

        navi_position = CalculatePointPosition(navi_position);

        RADAR_POINT* rardar_point = NewRadarPoint();
        if (rardar_point == NULL)
        {
            break;
        }
        rardar_point->dwObjectKey = member_object_key;
        rardar_point->eRadarPointColor = (is_dead == true) ? eRADAR_TEAM_PLAYER_DEAD_POINT : eRADAR_PC_POINT;
        rardar_point->fPosX = navi_position.x;
        rardar_point->fPosY = navi_position.y;
        StrnCopy(rardar_point->szName, party_info_itr->Get().m_szCharName, MAX_PATH - 1);
        rardar_point->szName[_countof(rardar_point->szName) - 1] = '\0';

        InsertRadarPoint(rardar_point);
    }
}

void uiRadarPointCollecter::CollectQuestNaviInfo(RADAR_POINT* radar_point, Object* object_ptr)
{
    if ((radar_point == NULL) || 
        (object_ptr== NULL))
    {
        return;
    }

    uiQuestListDlg* quest_list_dialog = GET_CAST_DIALOG(uiQuestListDlg, IM_QUEST_MANAGER::QUEST_LIST_DLG);
    if (quest_list_dialog == NULL)
    {
        return;
    }

    DWORD navi_quest_code = 0;
    for(int navi_index = 0; navi_index < kMaxNaviCount; ++navi_index)
    {
        navi_quest_code = quest_list_dialog->GetNaviQCodes(navi_index);
        Quest* quest = g_pQMGR_Real->FindQuest(static_cast<QCODE>(navi_quest_code));
        if (quest == NULL)
        {
            continue;
        }

        eOBJECT_TYPE object_type = object_ptr->GetSCObject().GetObjectType();
        switch(object_type)
        {
        case MONSTER_OBJECT:
            {
                if (quest->GetQState() != QS_NORMAL)
                {
                    break;
                }

                Monster* monster_ptr = static_cast<Monster*>(object_ptr);
                if (monster_ptr->IsShadowSummon() == TRUE)
                {
                    break;
                }

                if (monster_ptr->IsDead() == TRUE)
                {
                    break;
                }

                QUEST_CONDITION_KILLMONSTER_INFO* quest_condition_info = 
                    static_cast<QUEST_CONDITION_KILLMONSTER_INFO*>(quest->GetQuestInfo()->FindCompleteCondition(eQUEST_CONDITION_KILLMONSTER));
                if (quest_condition_info == NULL)
                {
                    break;
                }

                QUEST_CONDITION_KILLMONSTER* quest_condition = NULL;
                QUEST_COND_HASH& quest_condition_hash = quest->GetCompleteConditionHash();
                QUEST_COND_HASH_IT quest_condition_hash_itr = quest_condition_hash.begin(); 
                while (quest_condition_hash_itr != quest_condition_hash.end())
                {
                    if ((*quest_condition_hash_itr)->GetType() == eQUEST_CONDITION_KILLMONSTER)
                    {
                        quest_condition = static_cast<QUEST_CONDITION_KILLMONSTER*>(*quest_condition_hash_itr);
                        break;
                    }
                    ++quest_condition_hash_itr;
                }


                for(int condition_index = 0; condition_index < QUEST_CONDITION_KILLMONSTER::MAX_MONSTER_KIND_NUM; ++condition_index)
                {

                    if ((monster_ptr->GetMonsterInfo()->m_MonsterCode == quest_condition_info->GetMonsterCode(condition_index)) &&
                        (quest_condition->GetCurMonsterNum(condition_index) < quest_condition_info->GetMonsterNum(condition_index)))
                    {
                        // 이미 수집된 정보가 있으면 가까운것으로 갱신
                        bool is_near = true;
                        if (quest_navi_info_[navi_index].navi_object_key != 0)
                        {
                            Object* temp_object = g_ObjectManager.GetObject(quest_navi_info_[navi_index].navi_object_key);
                            WzVector temp_length = (temp_object->GetPosition() - g_pHero->GetPosition());
                            WzVector current_length = (object_ptr->GetPosition() - g_pHero->GetPosition());
                            if (VectorLength2(&temp_length) <= VectorLength2(&current_length))
                            {
                                is_near = false;
                            }
                        }

                        if (is_near == true)
                        {
                            // 갱신
                            quest_navi_info_[navi_index].navi_point_color = static_cast<eRADAR_POINT_COLOR>(eRADAR_NAVI_POINT_RED + navi_index);
                            quest_navi_info_[navi_index].navi_position_x = radar_point->fPosX;
                            quest_navi_info_[navi_index].navi_position_y = radar_point->fPosY;
                            quest_navi_info_[navi_index].navi_object_key = radar_point->dwObjectKey;
                            quest_navi_info_[navi_index].navi_quest_name = g_InterfaceManager.GetInterfaceString(quest->GetQuestInfo()->GetQNCode());
                            break;
                        }
                    }
                }
            }
            break;
        case LOTTO_NPC_OBJECT:
        case NPC_OBJECT:
            {
                if (quest_navi_info_[navi_index].navi_object_key != 0)
                {
                    break;
                }

                if (quest->GetQState() != QS_COMPLETED)
                {
                    break;
                }

                NPC* npc_ptr = static_cast<NPC*>(object_ptr);
                if (npc_ptr->GetMonsterInfo()->m_MonsterCode == quest->GetQuestInfo()->GetNPCCode_Reward())
                {
                    quest_navi_info_[navi_index].navi_point_color = static_cast<eRADAR_POINT_COLOR>(eRADAR_NAVI_POINT_RED + navi_index);
                    quest_navi_info_[navi_index].navi_position_x = radar_point->fPosX;
                    quest_navi_info_[navi_index].navi_position_y = radar_point->fPosY;
                    quest_navi_info_[navi_index].navi_object_key = radar_point->dwObjectKey;
                    quest_navi_info_[navi_index].navi_quest_name = g_InterfaceManager.GetInterfaceString(quest->GetQuestInfo()->GetQNCode());
                }
            }
            break;
        default:
            break;
        }
    }
}

WzVector uiRadarPointCollecter::CalculatePointPosition(WzVector object_position)
{
    uiRadarMan* ui_radar_manager = GET_DIALOG_MANAGER(uiRadarMan, UIMAN_RADAR);
    if (ui_radar_manager == NULL)
    {
        return object_position;
    }

    uiRadarMapData& radar_map_data = ui_radar_manager->radar_map_data();

    D3DXVECTOR4 transfirm_position(object_position.x, object_position.z, object_position.y, 1.0f);
    WzMatrix& transfirm_matrix = radar_map_data.transform_matrix();
    D3DXVec4Transform(&transfirm_position, &transfirm_position, (D3DXMATRIX*)(&transfirm_matrix));

    object_position.x = (((transfirm_position.x * 0.5f) + 0.5f) * radar_map_data.map_image_width());
    object_position.y = (((transfirm_position.y * (-0.5f)) + 0.5f) * radar_map_data.map_image_height());

    return object_position;
}

void uiRadarPointCollecter::InsertRadarPoint(RADAR_POINT* radar_point)
{
    if (radar_point == NULL)
    {
        return;
    }

    eRADAR_POINT_COLOR radar_point_color = radar_point->eRadarPointColor;

    if ((radar_point_color == eRADAR_NORMAL_QUEST_ACCEPT) || 
        (radar_point_color == eRADAR_DEACTIVE_QUEST_NPC) || 
        (radar_point_color == eRADAR_REPEAT_QUEST_ACCEPT) || 
        (radar_point_color == eRADAR_DAILY_QUEST_ACCEPT) ||
        (radar_point_color == eRADAR_GUILD_QUEST_ACCEPT)
#ifdef _NA_007407_20140710_CHALLENGE_DAILY_QUEST
        || (radar_point_color == eRADAR_CHALLENGE_DAILY_QUEST_ACCEPT)
#endif // _NA_007407_20140710_CHALLENGE_DAILY_QUEST
        )
    {
        npc_quest_accept_point_container_.push_back(radar_point);
        
    }
    else if ((radar_point_color == eRADAR_NORMAL_QUEST_REWARD) ||
        (radar_point_color == eRADAR_REPEAT_QUEST_REWARD) ||
        (radar_point_color == eRADAR_DAILY_QUEST_REWARD) ||
        (radar_point_color == eRADAR_GUILD_QUEST_REWARD)
#ifdef _NA_007407_20140710_CHALLENGE_DAILY_QUEST
        || (radar_point_color == eRADAR_CHALLENGE_DAILY_QUEST_REWARD)
#endif // _NA_007407_20140710_CHALLENGE_DAILY_QUEST
        )
    {

        npc_quest_reward_point_container_.push_back(radar_point);
    }
    else if ((radar_point_color >= eRADAR_NAVI_POINT_YELLOW) && 
        (radar_point_color <= eRADAR_CONTINENT_MAP_HERO_POS))
    {
        radar_point_container_.push_back(radar_point);
    }
    else if ((radar_point_color >= eRADAR_PARTY_POINT_01) && 
        (radar_point_color <= eRADAR_PARTY_POINT_10))
    {
        radar_point_container_.push_back(radar_point);
    }
    else if (radar_point_color == eRADAR_BOSS_MONSTER_POINT)
    {
        radar_point_container_.push_back(radar_point);
    }
    else
    {
        radar_point_container_.push_front(radar_point);
    }
}

void uiRadarPointCollecter::MergeCollectPoint()
{
    radar_point_container_.insert(radar_point_container_.end(), 
        npc_quest_accept_point_container_.begin(), 
        npc_quest_accept_point_container_.end());

    radar_point_container_.insert(radar_point_container_.end(),
        npc_quest_reward_point_container_.begin(), 
        npc_quest_reward_point_container_.end());
}

eRADAR_POINT_COLOR uiRadarPointCollecter::GetPointColorPlayer(Player* player_ptr)
{
    eRADAR_POINT_COLOR radar_point_color = eRADAR_PC_POINT;
    if (player_ptr == NULL)
    {
        radar_point_color = eRADAR_NOT_FOUND_OBJECT;
        return radar_point_color;
    }

    if (player_ptr->GetObjectKey() == g_pHero->GetObjectKey())
    {
        return radar_point_color;
    }

    uiPartyMan* ui_party_manager = GET_DIALOG_MANAGER(uiPartyMan, UIMAN_PARTY);
    if (ui_party_manager == NULL)
    {
        return radar_point_color;
    }

    bool is_dead = (player_ptr->IsDead() == TRUE);
    bool is_party_member = ui_party_manager->IsPartyMember(player_ptr->GetObjectKey());
    bool is_enemy = (RELATIONKIND::Enemy == GameFramework::GetCurrentScene()->GetRelationKindOfCharacter(player_ptr, g_HeroInput.lsForceAttack()));
    SCENE_TYPE scene_type = GameFramework::GetCurrentScene()->GetType();
    switch (scene_type)
    {
    case SCENE_TYPE_INSTANCE_DUNGEON:
        {
            if (is_dead == true)
            {
                radar_point_color = eRADAR_TEAM_PLAYER_DEAD_POINT;
            }

            bool is_same_floor = (player_ptr->GetFloor() == g_pHero->GetFloor());
            if (is_same_floor == false)
            {
                radar_point_color = eRADAR_NOT_FOUND_OBJECT;
            }
        }
        break;
    default:
        {
            if (is_party_member == true)
            {
                if (is_dead == true)
                {
                    radar_point_color = eRADAR_TEAM_PLAYER_DEAD_POINT;
                }
            }
            else if (is_enemy == true)
            {
                radar_point_color = eRADAR_BOSS_MONSTER_POINT;

                // GM캐릭터의 투명모드는 추가하지 않는다
                bool is_gm_observer_mode = ((player_ptr->IsGM() != FALSE) && 
                    (player_ptr->IsObserverMode() != FALSE));
                // 섀도우캐릭터가 하이드일때는 추가하지 않는다
                bool is_shadow_hide = ((player_ptr->GetClass() == eCHAR_SHADOW) && 
                    (player_ptr->GetHideSkill() == HIDE_SKILL_INFO::ALL_HIDE));
                if ((is_gm_observer_mode == true) ||
                    (is_shadow_hide == true))
                {
                    radar_point_color = eRADAR_NOT_FOUND_OBJECT;
                }
                else
                {
                    if (scene_type == SCENE_TYPE_CHUNTING)
                    {
                        radar_point_color = eRADAR_TIMEATTACK;
                    }

                    if (is_dead == true)
                    {
                        radar_point_color = eRADAR_ENEMY_PLAYER_DEAD_POINT;
                    }                    
                }
            }
            else
            {
                radar_point_color = eRADAR_NOT_FOUND_OBJECT;
            }
        }
    }

    return radar_point_color;
}

eRADAR_POINT_COLOR uiRadarPointCollecter::GetPointColorNpc(NPC* npc_ptr)
{
    eRADAR_POINT_COLOR radar_point_color = eRADAR_NOT_FOUND_OBJECT;
    if (npc_ptr == NULL)
    {
        return radar_point_color;
    }

    EXTRA_NPCINFO* extra_npc_info = npc_ptr->GetExtraInfo();
    if (extra_npc_info == NULL)
    {
        return radar_point_color;
    }

    int npc_emotion = npc_ptr->GetEmoticonIcon();
    switch (npc_emotion)
    {
    case HEAD_MESH_DOCUMENT_ACTIVE:
        {
            if (npc_ptr->GetMaxQuestRepeat() == 1)
            {
                radar_point_color = eRADAR_NORMAL_QUEST_REWARD;
            }
#ifdef _NA_003027_20111013_HONOR_SYSTEM
            else if (npc_ptr->IsHaveGuildQuest() == true)
            {
                radar_point_color = eRADAR_GUILD_QUEST_REWARD;
            }
#ifdef _NA_007407_20140710_CHALLENGE_DAILY_QUEST
            else if (npc_ptr->IsHaveChallengeDailyQuest() == true)
            {
                radar_point_color = eRADAR_CHALLENGE_DAILY_QUEST_REWARD;
            }
#endif // _NA_007407_20140710_CHALLENGE_DAILY_QUEST
            else if (npc_ptr->IsHaveDailyQuest() == true)
            {
                radar_point_color = eRADAR_DAILY_QUEST_REWARD;
            }
#endif //_NA_003027_20111013_HONOR_SYSTEM
            else
            {
                radar_point_color = eRADAR_REPEAT_QUEST_REWARD;
            }
        }
        break;
    case HEAD_MESH_SCROLL_ACTIVE:
        {
            if (npc_ptr->GetMaxQuestRepeat() == 1)
            {
                radar_point_color = eRADAR_NORMAL_QUEST_ACCEPT;
            }
#ifdef _NA_003027_20111013_HONOR_SYSTEM
            else if (npc_ptr->IsHaveGuildQuest() == true)
            {
                radar_point_color = eRADAR_GUILD_QUEST_ACCEPT;
            }
#ifdef _NA_007407_20140710_CHALLENGE_DAILY_QUEST
            else if (npc_ptr->IsHaveChallengeDailyQuest() == true)
            {
                radar_point_color = eRADAR_CHALLENGE_DAILY_QUEST_ACCEPT;
            }
#endif // _NA_007407_20140710_CHALLENGE_DAILY_QUEST
            else if (npc_ptr->IsHaveDailyQuest() == true)
            {
                radar_point_color = eRADAR_DAILY_QUEST_ACCEPT;
            }
#endif //_NA_003027_20111013_HONOR_SYSTEM
            else
            {
                radar_point_color = eRADAR_REPEAT_QUEST_ACCEPT;
            }
        }
        break;
    case HEAD_MESH_SCROLL_DEACTIVE:
        {
            radar_point_color = eRADAR_DEACTIVE_QUEST_NPC;
        }
        break;
#ifdef _NA_004921_20120628_ADD_QUEST_NPC_EMOTICON_FOR_QUEST_TYPE
    case HEAD_MESH_DOCUMENT_DAILY:
        {
            radar_point_color = eRADAR_DAILY_QUEST_REWARD;
        }
        break;
    case HEAD_MESH_DOCUMENT_GUILD:
        {
            radar_point_color = eRADAR_GUILD_QUEST_REWARD;
        }
        break;
    case HEAD_MESH_DOCUMENT_REPEAT:
        {
            radar_point_color = eRADAR_REPEAT_QUEST_REWARD;
        }
        break;
    case HEAD_MESH_SCROLL_DAILY:
        {
            radar_point_color = eRADAR_DAILY_QUEST_ACCEPT;
        }
        break;
    case HEAD_MESH_SCROLL_GUILD:
        {
            radar_point_color = eRADAR_GUILD_QUEST_ACCEPT;
        }
        break;
    case HEAD_MESH_SCROLL_REPEAT:
        {
            radar_point_color = eRADAR_REPEAT_QUEST_ACCEPT;
        }
        break;
#endif //_NA_004921_20120628_ADD_QUEST_NPC_EMOTICON_FOR_QUEST_TYPE
    default:
        {
            if (g_pHero->IsShowFullMap() == TRUE)
            {
                if (extra_npc_info->m_eDisplayType & EXTRA_NPCINFO::eDISPLAY_FULLMAP)
                {
                    radar_point_color = static_cast<eRADAR_POINT_COLOR>(extra_npc_info->m_iNPC_IconID);
                }
            }
            else
            {
                if (extra_npc_info->m_eDisplayType & EXTRA_NPCINFO::eDISPLAY_MINIMAP)
                {
                    radar_point_color = static_cast<eRADAR_POINT_COLOR>(extra_npc_info->m_iNPC_IconID);
                }
            }

            if ((extra_npc_info->m_eNPCTYPE == eNPC_TYPE_PROTECTS) && 
                (npc_ptr->IsDead() == TRUE))
            {
                radar_point_color = eRADAR_MONSTER_DEAD_POINT;
            }
        }
    }

    return radar_point_color;
}

eRADAR_POINT_COLOR uiRadarPointCollecter::GetPointColorMonster(Monster* monster_ptr)
{
    eRADAR_POINT_COLOR radar_point_color = eRADAR_NOT_FOUND_OBJECT;
    if (monster_ptr == NULL)
    {
        return radar_point_color;
    }

    if (g_pHero != NULL)
    {
        float offset_z_value = abs(g_pHero->GetPosition().z - monster_ptr->GetPosition().z);
        if (offset_z_value > kMonsterDistanceInstanceDungeon)
        {
            return radar_point_color;
        }
    }

    eNPC_GRADE npc_grade = static_cast<eNPC_GRADE>(monster_ptr->GetMonsterInfo()->m_byGrade);
    if (monster_ptr->IsDead() == TRUE)
    {
        switch (npc_grade)
        {
        case eNPC_MIDDLEBOSS:
        case eNPC_BOSS:
        case eNPC_LUCKY_MONSTER:
            {
                radar_point_color = eRADAR_BOSS_MONSTER_DEAD_POINT;
            }
            break;
        case eNPC_DOMINATION_MAPOBJECT_NPC:
            {
                radar_point_color = eRADAR_BLAST;
            }
            break;
        default:
            {
                radar_point_color = eRADAR_MONSTER_DEAD_POINT;
            }
        }
    }
    else
    {
        Object* parent_object = g_ObjectManager.GetObject(monster_ptr->GetParentId());
        if (parent_object != NULL)
        {
            bool is_enemy = (RELATIONKIND::Enemy == GameFramework::GetCurrentScene()->GetRelationKindOfCharacter(monster_ptr, g_HeroInput.lsForceAttack()));
            if (is_enemy == true)
            {
                radar_point_color = eRADAR_MONSTER_POINT;
            }
            else
            {
                radar_point_color = eRADAR_PARTY_POINT;
            }
        }
        else
        {
            switch (npc_grade)
            {
            case eNPC_MIDDLEBOSS:
            case eNPC_BOSS:
            case eNPC_LUCKY_MONSTER:
                {
                    radar_point_color = eRADAR_BOSS_MONSTER_POINT;
                }
                break;
            case eNPC_CRYSTAL_WARP:
                {
                    if ((g_pHero->GetGuildRenderInfo().m_GuildGuid != INVALID_GUILDGUID) && 
                        (g_pHero->GetGuildRenderInfo().m_GuildGuid == monster_ptr->GetMasterGuildID()))	
                    {
                        radar_point_color = eRADAR_CRYSTAL_WARP;
                    }   
                    else
                    {
                        radar_point_color = eRADAR_MONSTER_POINT;
                    }
                }
                break;
            case eNPC_DOMINATION_MAPOBJECT_NPC:
                {
                    radar_point_color = eRADAR_NOT_FOUND_OBJECT;
                }
                break;
            case eNPC_FRIEND_MAPOBJECT_NPC:
            case eNPC_FRIEND_MONSTER:
                {
                    radar_point_color = eRADAR_PC_POINT;
                }
                break;
            default:
                {
                    radar_point_color = eRADAR_MONSTER_POINT;
                }
            }
        }
    }

    return radar_point_color;
}

eRADAR_POINT_COLOR uiRadarPointCollecter::GetPointColorMapObject(MapObject* map_object_ptr)
{
    eRADAR_POINT_COLOR radar_point_color = eRADAR_POINT_COUNT;
    if (map_object_ptr->GetShow() == FALSE)
    {
        return radar_point_color;
    }

    if ((map_object_ptr->GetBasePart() == NULL) || 
        (map_object_ptr->GetBasePart()->GetWzUnit() == NULL))
    {
        return radar_point_color;
    }

#ifdef _NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND
    // 전장 거점은 채집키로 발동하기때문에 GetCollectionID() 체크보다 앞에 있어야 한다.
    if (map_object_ptr->IsBattleGroundStrongPointObject() == true)
    {
        int color_index = ChaosSystem::Function::GetStrongPointRadarColor(map_object_ptr);
        return static_cast<eRADAR_POINT_COLOR>(eRADAR_BATTLE_GROUND_STRONGPOINT_NORMAL + color_index);
    }
#endif //_NA_005026_20120527_CHAOS_ZONE_BATTLE_GROUND

#ifdef _NA_0_20110327_DISPLAY_COLLECTABLE_OBJECT_IN_RADAR
    if( map_object_ptr->GetCollectionID() != INVALID_WZID )
    {
        return eRADAR_COLLECT_TARGET;
    }
#endif

    DWORD object_type_id =  map_object_ptr->GetBasePart()->GetWzUnit()->GetType();
    if (object_type_id == StrToWzID("pTG1"))
    {
        radar_point_color = eRADAR_AIRSHIP_PORTAL;
    }
    else if(object_type_id == StrToWzID("pT01"))
    {
        radar_point_color = eRADAR_NORMAL_PORTAL;
    }
    else if((object_type_id == StrToWzID("pT05")) || 
        (object_type_id == StrToWzID("pT06")))
    {
        radar_point_color = eRADAR_MISSION_PORTAL;
    }
    else if(object_type_id == StrToWzID("wpnt"))
    {
        radar_point_color = eRADAR_WAY_POINT;
    }
    else if (map_object_ptr->IsDominationObject() == TRUE)
    {
        uiDominationTowerMan* ui_domination_manager = GET_DIALOG_MANAGER(uiDominationTowerMan, UIMAN_DOMINATIONWAR_TOWER);
        if ((ui_domination_manager != NULL) && 
            (ui_domination_manager->IsShowDominationObject() == true))
        {
            radar_point_color = eRADAR_AIRSHIP;
        }
    }

    return radar_point_color;
}

RADAR_POINT* uiRadarPointCollecter::NewRadarPoint()
{
    RADAR_POINT* radar_point = (RADAR_POINT*)(radar_point_memorypool.Alloc());
    
#ifdef _DEV_VER
    if (radar_point == NULL)
    {
        GlobalFunc::OutputMsg_Error(eOUTPUT_MSG_CHAT, "Failed - uiRadarPointCollecter::NewRadarPoint()");
    }    
#endif //_DEV_VER

    return radar_point;
}
