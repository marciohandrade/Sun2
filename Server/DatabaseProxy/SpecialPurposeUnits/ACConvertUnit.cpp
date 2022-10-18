#include "stdafx.h"

#if DBPROXY_TYPE_SELECTOR == DBPROXY_TYPE_UNIT
#ifdef C_NA_0_20100520_ACSYSTEM_CHANGED

#include "ACConvertUnit.h"

#include "ACConvertQuery.h"

#include "AreaConquest.h"
#include "VersionControl.h"

#include "MapInfoParser.h"
#include "NPCInfoParser.h"

#include "QuestManagerForUnit.h"


namespace batch_unit {
;

using batch::IBatchUnit;

void
ACConvertUnit::UpdateNode::Init()
{
    ZeroMemory(this, sizeof(*this));
    util::LList::Init(this);
}

ACConvertUnit::ACConvertUnit() 
    : step_(STEP_NONE)
    , total_completed_(false)
    , suspend_query_character_(false)
    , completed_query_character_(false)
{
    const DWORD UserSlotLastKey = 2;
    BOOST_STATIC_ASSERT(kPending_StartKey < UserSlotLastKey);
    select_key_generator_.Create(kPending_StartKey, UserSlotLastKey);

    max_transactions_ = GetPrivateProfileInt("DBPUNIT_DATA",
                                             "MAX_UPDATES_PER_UNITTIME",
                                             300,
                                             "./DBPUnit.ini");
    if (max_transactions_ < 300)
        max_transactions_ = 300;
    key_generator_.Create(kPending_StartKey, max_transactions_);

    ZeroMemory(&arguments_, sizeof(arguments_));
    util::LList::Init(&arguments_.pending_nodes_);
    util::LList::Init(&arguments_.updating_nodes_);
    ZeroMemory(&statistics_, sizeof(statistics_));
}

ACConvertUnit::~ACConvertUnit()
{

}

inline batch::IBatchUnit::eSTEP
ACConvertUnit::CurrentStep() const
{
    return step_;
}

BOOLEAN
ACConvertUnit::BatchInit()
{    
    SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"]\n");

    ZeroMemory(&arguments_, sizeof(arguments_));
    util::LList::Init(&arguments_.pending_nodes_);
    util::LList::Init(&arguments_.updating_nodes_);

    statistics_timer_.SetTimer(5000);
    step_ = this->STEP_PROCESSING;
    //
    InitACScriptData();
    return true;
}

BOOLEAN
ACConvertUnit::BatchProcess()
{
    Get_Character_ACInfo();
    QueryAchievementUpdate();

    bool forced_statistics_logging = false;
    // completion check
    if (completed_query_character_)
    {
        if (util::LList::IsEmpty(&arguments_.pending_nodes_) &&
            util::LList::IsEmpty(&arguments_.updating_nodes_) &&
            this->max_transactions_ == key_generator_.GetSize()) // to fit dual buffering db queue
        {
            total_completed_ = true;
            forced_statistics_logging = true;
        }
    };
    if (forced_statistics_logging || statistics_timer_.IsExpired())
    {
        float interval = static_cast<float>(statistics_timer_.GetInterval());
        ;     interval /= 1000.0f;

        float selecting_ratio = statistics_.solved_selectings_ / interval;
        float updating_ratio = statistics_.solving_updatings_ / interval;

        SUNLOG(eCRITICAL_LOG,
               "|Number of get queried=%u|Number of set queried=%u|",
               statistics_.number_of_get_queried_, statistics_.number_of_set_queried_);

        SUNLOG(eCRITICAL_LOG,
               "|Pendings=%d|Updatings=%d|gets/sec=%2.2f|sets/sec=%2.2f|",
               arguments_.number_of_pending_nodes_, arguments_.number_of_updating_nodes_,
               selecting_ratio, updating_ratio);

        statistics_.solved_selectings_ = 0;
        statistics_.solving_updatings_ = 0;
    }

    BOOLEAN continued = true;
    if (total_completed_)
    {
        continued = false;
        step_ = this->STEP_COMPLETED;
    }

    return continued;
}

BOOLEAN
ACConvertUnit::BatchRelease()
{
    printf("["__FUNCTION__"]\n");
    step_ = this->STEP_RELEASED;
    return false;
}

void
ACConvertUnit::DBResult(DWORD index, QueryResult* query_result)
{
    BOOST_STATIC_ASSERT(sizeof(KeyNode) == sizeof(index));
    KeyNode key_node = { 0, };
    key_node.composed_ = index;

    switch (key_node.query_type_)
    {
    case key_node.QUERY_SELECT_CHARACTERS:
        {
            OnSelectCharacterACInfo(key_node, query_result);
        }
        break;
    case key_node.QUERY_UPDATE_AC:
        {
            OnUpdateCharacterACInfo(key_node, query_result);
        }
        break;
    }
}

void
ACConvertUnit::Query_Achievement_Info(Character_Achievement_Unit& ac_info)
{
    Character_Achievements achievement_info;
    ac_info.achievement_.Serialize(achievement_info, ACSERIALIZE_DBPROXY_LOAD);

    for(int i = 0 ; i < achievement_info.achievement_count_ ; i++)
    {
        Character_Achievement& ac_part_info = achievement_info.achievement_info_[i];
        UpdateNode* update_node = TAllocNew(UpdateNode);
        update_node->Init();
        //update_node->key_ = key_generator_.GetKey(); //assert key_ != 0
        update_node->query_ = Query_Update_Character_ACInfo::ALLOC();
        // alignment processed values
        Query_Update_Character_ACInfo* query = update_node->query_;
        // query { update AC info };
        STRING_SAFER_QUERY128 query_buffer;
        query_buffer.MakeString("{call S_ACSystem_Update(%u, %u, %u, %u, %u, %u, %u, %u, %u, %u, %u)}",
            ac_info.character_guid_, ac_part_info.index_number_, ac_part_info.is_reward_ ? 1 : 0,
            ac_part_info.object_value_[0], ac_part_info.object_value_[1], 
            ac_part_info.object_value_[2], ac_part_info.object_value_[3],
            ac_part_info.object_value_[4], ac_part_info.object_value_[5],
            ac_part_info.object_value_[6], ac_part_info.object_value_[7]);
        query->SetQuery(query_buffer);
        //
        util::LList::AddPrev(update_node, &arguments_.pending_nodes_);
        ++arguments_.number_of_pending_nodes_;
    }
}

bool 
ACConvertUnit::QueryAchievementUpdate()
{
    UpdateNode* it = static_cast<UpdateNode*>(arguments_.pending_nodes_.next);
    for (; it != &arguments_.pending_nodes_;)
    {
        if (key_generator_.IsEmpty()) {
            return false;
        }

        UpdateNode* extracted = it;
        it = util::LList::Delete(it);
        if (arguments_.number_of_pending_nodes_)
            --arguments_.number_of_pending_nodes_;

        extracted->key_ = key_generator_.GetKey();

        Query_Update_Character_ACInfo* const query = extracted->query_;
        query->node_pointer_ = extracted; // passing object;

        util::LList::AddPrev(extracted, &arguments_.updating_nodes_);
        ++arguments_.number_of_updating_nodes_;

        KeyNode key_node = { 0, };
        key_node.query_type_ = key_node.QUERY_UPDATE_AC;
        key_node.node_key_ = static_cast<WORD>(extracted->key_);
        query->SetVoidObject(this);
        query->SetIndex(key_node.composed_);
        //query->SetQuery(extracted->query_);
        static ulong output_db_selector = 0;
        ++output_db_selector;
        SolarDatabaseInDBP* output_db
            = (output_db_selector & 0x1)
            ?   g_DBProxyServer.Get2ndDatabase()
            :   g_DBProxyServer.Get3rdDatabase();
        output_db->DBQuery(query);
    }
    return true;
}

bool
ACConvertUnit::Get_Character_ACInfo()
{
    if (completed_query_character_ != false) {
        return false;
    }

    if (arguments_.total_pages_ != 0 &&
        arguments_.selected_page_ >= arguments_.total_pages_)
    {
        completed_query_character_ = true;
        return false;
    }

    if (suspend_query_character_ != false) {
        return false;
    }

    if (select_key_generator_.IsEmpty() != false) {
        return false; // wait next time
    }

    const DWORD user_slot_index = select_key_generator_.GetKey();

    ++arguments_.selected_page_;

    STRING_SAFER_QUERY128 query_buffer;
    query_buffer.MakeString("{call USP_SUN_GetACStreamInfo(%d, ?)}",
                            arguments_.selected_page_);

    Query_Select_Character_ACInfo* query = Query_Select_Character_ACInfo::ALLOC();
    KeyNode key_node = { 0, };
    key_node.query_type_ = key_node.QUERY_SELECT_CHARACTERS;
    key_node.node_key_ = static_cast<WORD>(user_slot_index);
    query->SetVoidObject(this);
    query->SetIndex(key_node.composed_);
    query->SetQuery(query_buffer);
    query->SetQueriedPage(arguments_.selected_page_);

    SolarDatabaseInDBP* first_db = g_DBProxyServer.Get1stDatabase();
    first_db->DBQuery(query);

    return false;
}

bool
ACConvertUnit::OnUpdateCharacterACInfo(const KeyNode& index, QueryResult* query_result_base)
{
    key_generator_.RestoreKey(index.node_key_);

    ++statistics_.number_of_set_queried_;
    //static int queried = 0;
    //printf("Number of queried=%u\n", ++queried);

    Query_Update_Character_ACInfo* const query_result =
        static_cast<Query_Update_Character_ACInfo*>(query_result_base);

    UpdateNode* update_node = reinterpret_cast<UpdateNode*>(query_result->node_pointer_);
    util::LList::Delete(update_node);

    if (arguments_.number_of_updating_nodes_)
        --arguments_.number_of_updating_nodes_;

    ++statistics_.solving_updatings_;
    //printf("Updated %s\n", query_result->GetQuery());

    TAllocDelete(UpdateNode, update_node);
    Query_Update_Character_ACInfo::FREE(query_result);

    // balance formula
    size_t remains = key_generator_.GetSize();
    suspend_query_character_ = (remains < 10) ||
        (Query_Select_Character_ACInfo::RESULT_ROW_NUM * 2) < arguments_.number_of_pending_nodes_;

    return true;
}

bool
ACConvertUnit::OnSelectCharacterACInfo(const KeyNode& index, QueryResult* query_result_base)
{
    select_key_generator_.RestoreKey(index.node_key_);

    ++statistics_.number_of_get_queried_;
    ++statistics_.solved_selectings_;

    Query_Select_Character_ACInfo* const query_result = static_cast<Query_Select_Character_ACInfo*>(query_result_base);
    const DWORD number_of_rows = query_result->GetResultRowNum();

    if (number_of_rows == 0)
    {
        Query_Select_Character_ACInfo::FREE(query_result);
        // force stop
        completed_query_character_ = true;
        return false;
    }

    // latest update
    arguments_.total_pages_ = query_result->pParam[0].end_page_;

    BYTE ac_stream[MAX_ACSTREAM_SIZE];
    BYTE mission_stream[MAX_MISSIONSTREAM_SIZE];
    BYTE quest_stream[MAX_COMPL_QUESTSTREAM_SIZE];
    BYTE quest_prog[MAX_PROGR_QUESTSTREAM_SIZE];

    DWORD row = 0;
    for (; row < number_of_rows; ++row)
    {
        Query_Select_Character_ACInfo::sQUERY& result = query_result->pResult[row];

        CHARGUID char_guid = result.char_guid_;

        ZeroMemory(ac_stream, sizeof(ac_stream));
        ZeroMemory(mission_stream, sizeof(mission_stream));
        ZeroMemory(quest_stream, sizeof(quest_stream));
        ZeroMemory(quest_prog, sizeof(quest_prog));

        ConvertACInfo(result, ac_stream, mission_stream, quest_stream, quest_prog);
        // side effect : query 'Update'
        ConvertACToAchievement(char_guid, ac_stream, mission_stream, quest_stream, quest_prog);
    }

    Query_Select_Character_ACInfo::FREE(query_result);
    return true;
}

void 
ACConvertUnit::ConvertACInfo(Query_Select_Character_ACInfo::sQUERY& IN result, 
                             BYTE OUT ac_stream[MAX_ACSTREAM_SIZE],
                             BYTE OUT mission_stream[MAX_MISSIONSTREAM_SIZE],
                             BYTE OUT quest_stream[MAX_COMPL_QUESTSTREAM_SIZE],
                             BYTE OUT quest_prog[MAX_PROGR_QUESTSTREAM_SIZE])
{
    int converted_ac_version = 0;
    if(AreaConquest::ACStreamConverter(MAX_AC_SECTION_NUM,
        result.ac_version_, result.ac_stream_,
        converted_ac_version, ac_stream) == false)
    {
        ASSERT(!"AC stream conversion failed!");
        __asm { int 3 };
    }

    CopyMemory(mission_stream, result.mission_stream_, MAX_MISSIONSTREAM_SIZE);
    // NOTE: ... quest_prog (source == output)...
    int converted_quest_version = 0;
    if( FALSE == VersionControl::QuestConverter(converted_quest_version,
        quest_prog,
        quest_stream,
        result.quest_version_,
        quest_prog, // WARNING: ...
        result.quest_complete_stream_))
    {
        ASSERT(!"AC stream conversion failed!");
        __asm { int 3 };
    }
}

void 
ACConvertUnit::ConvertACToAchievement(CHARGUID char_guid,
                                   BYTE ac_stream[MAX_ACSTREAM_SIZE],
                                   BYTE mission_stream[MAX_MISSIONSTREAM_SIZE],
                                   BYTE quest_stream[MAX_COMPL_QUESTSTREAM_SIZE],
                                   BYTE quest_prog[MAX_PROGR_QUESTSTREAM_SIZE])
{
    ACInfoConvertManager    ac_convert_manager;   
    Character_Achievement_Unit   achievement_info;

    achievement_info.character_guid_ = char_guid;

    ac_convert_manager.Init();
    ac_convert_manager.AlignStream(ac_stream, mission_stream, quest_stream, achievement_info);      

    for( int area_index = 0 ; area_index < MAX_AC_SECTION_NUM ; area_index++)
    {
        AreaConquestInfo* ac_info = ac_convert_manager.GetAreaConquestInfo(area_index + 1);

        for(int condition_index = eACC_COLLECTING_ITEM1; 
            condition_index < eACC_CONDITION_MAX ; condition_index++)
        {
            int condition_value = ac_info->GetConditionValue(condition_index);            
            if(condition_value > 0)
            {
                AddAchievementQuery(achievement_info, area_index + 1, condition_index, condition_value);
            }
        }

        achievement_info.achievement_.AccomplishAchievement();
        //achievement_info.achievement_.InitAccomplishmentRatio();        

        for(BYTE reward_index = 1 ; reward_index <= MAX_AC_REWARD_NUM ; reward_index +=2 )
        {   
            if(ac_info->GetRewardFlag(reward_index))            
            {
                DWORD achievement_map_key = MAKEDWORD(area_index + 50001, (reward_index + 1) * 500);  
                Achievement_SearchInfo* achieveement_searchinfo = 
                            GetAchievementSearch()->GetAccomplishment_SearchInfo(achievement_map_key);

                if(achieveement_searchinfo)
                {
                    achievement_info.achievement_.Force_RewardAchievement(achieveement_searchinfo->parser_info_->index_,
                                                                area_index + 50001 ,true);
                }                
            }
        }
    }       
    
    //changed_ac_queue_.push(achievement_info);
    Query_Achievement_Info(achievement_info);
}

void 
ACConvertUnit::AddAchievementQuery(Character_Achievement_Unit& achievement_info, int area_index, 
                                   int contition_index, int condition_value)
{
    if( condition_value <= 0)
    {
        return;
    }

    switch(GetObjectType(contition_index))
    {
    case ACHIEVEMENT_OBJECT_ITEM: //아이템 수집
        AddAchieveItemQuery(achievement_info, area_index, contition_index, condition_value);        
        break;
    case ACHIEVEMENT_OBJECT_NPC: //몬스터 사냥
        AddAchieveNPCQuery(achievement_info, area_index, contition_index, condition_value);
        break;
    case ACHIEVEMENT_OBJECT_NPC_AC: //지역 몬스터 사냥
        AddAchieveACNPCQuery(achievement_info, area_index, contition_index, condition_value);
        break;
    case ACHIEVEMENT_OBJECT_MISSION: //미션, 저탑
        AddAchieveMissionQuery(achievement_info, area_index, contition_index, condition_value);
        break;
    default:
        break;
    }  
}

void
ACConvertUnit::AddAchieveMissionQuery(Character_Achievement_Unit& achievement_info, int area_index, 
                                    int contition_index, int condition_value)
{
    AreaConquestInfoParser* ac_script = AreaConquestInfoParser::Instance();
    BASE_AreaConquestInfo*  base_ac_script = ac_script->GetAreaConquestInfo(area_index);

    MAPCODE map_code = 0;
    //저탑 
    if(contition_index == eACC_MISSION_CURSE_TOWER)
    {
        //저탑
        map_code = base_ac_script->m_wCT_MC; 

        Achievement_SearchInfo* search_info = NULL;

        GetAchievementSearch()->SetFirst();

        while(GetAchievementSearch()->GetNext(&search_info))
        {
            if(search_info->parser_info_->obejcts_[0].object_code_ == map_code)
            {
                if(search_info->parser_info_->location_code_ == static_cast<WORD>(area_index + 50000))
                {
                    achievement_info.achievement_.SetAchievementValue(static_cast<WORD>(area_index + 50000),
                        search_info->parser_info_->index_, 
                        search_info->index_,
                        condition_value,
                        ACHIEVEMENT_VALUE_SET);
                }                
            }
        }
    }   
    //타락한 사원
    else if(contition_index == eACC_MISSION_CORRUPT_TEMPLE)
    {
        //타락한사원은 없지
        //map_code = base_ac_script->m_wLT_MC;
        //SUNLOG(eCRITICAL_LOG, "Not Found map type %u",contition_index);
        //return;
    }
    
    //메인 미션
    else if(contition_index == eACC_MISSION_MAIN || contition_index == eACC_QUEST_NORMAL ||
        (contition_index >= eACC_MISSION_SUB1 && contition_index <= eACC_MISSION_SUB4))
    {
        /*
        MapInfoParser* const map_info_parser = MapInfoParser::Instance();        
        MapGroup* map_group = map_info_parser->GetMapGroup(base_ac_script->m_byMissionMapKind);
        if (map_group == NULL) 
        {
            SUNLOG(eCRITICAL_LOG, "Not Found map Group %u\n",base_ac_script->m_byMissionMapKind);
            return;
        }

        BYTE map_index = 1;

        if(contition_index >= eACC_MISSION_SUB1 && contition_index <= eACC_MISSION_SUB4)
        {
            map_index = contition_index - eACC_MISSION_MAIN - 2;
        }

        const sMAPINFO* map_info = map_group->GetMapInfoIndex(map_index);
        if (map_info == NULL) 
        {
            SUNLOG(eCRITICAL_LOG, "Not Found map info %u %u", area_index, contition_index);
            return;
        }

        map_code = map_info->MapCode;    

        Achievement_SearchInfo* achieveement_searchinfo = 
            GetAchievementSearch()->Getchievement_SearchInfo(map_code);

        achievement_info.achievement_.SetAchievementValue(static_cast<WORD>(area_index + 50000),
            achieveement_searchinfo->parser_info_->index_, 
            achieveement_searchinfo->index_,
            condition_value,
            ACHIEVEMENT_VALUE_SET);
            */
    }
    
    else
    {
        SUNLOG(eCRITICAL_LOG, "Not Found map type %u",contition_index);
        return;
    }      
}

void
ACConvertUnit::AddAchieveACNPCQuery(Character_Achievement_Unit& achievement_info, int area_index, 
                                    int contition_index, int condition_value)
{
 
    achievement_info.SetAchievementValue(100+area_index, condition_value);
}

void
ACConvertUnit::AddAchieveNPCQuery(Character_Achievement_Unit& achievement_info, int area_index, 
                                   int contition_index, int condition_value)
{            
    DWORD AC_Key = MAKEDWORD(area_index, contition_index);    
    NPC_MAP::iterator iter_npc = npcInfo_.find(AC_Key);

    if(iter_npc == npcInfo_.end())
    {   
        //SUNLOG(eCRITICAL_LOG, "Not Found npcinfo %u, %u, %u\n",AC_Key, 
            //area_index, contition_index);
        //ASSERT(!"Not Found npcinfo");
        // __asm { int 3 };          
    }
    else
    {  
        BASE_NPCINFO* npc_info;
        npc_info = iter_npc->second;

        achievement_info.SetAchievementValue(npc_info->m_MonsterCode, condition_value);       
    }
}

void
ACConvertUnit::AddAchieveItemQuery(Character_Achievement_Unit& achievement_info, int area_index, 
                                   int contition_index, int condition_value)
{       
    if(contition_index == eACC_COLLECTING_HIDDEN_ITEM) return;
    /*if((eACC_COLLECTING_ITEM7 <= contition_index && contition_index<= eACC_COLLECTING_HIDDEN_ITEM) || 
        (eACC_TREASURE_KEY_FLAG6 <= contition_index && contition_index<= eACC_TREASURE_KEY_FLAG8) ||
        (eACC_TREASURE_SPECIALTY_FLAG1 <= contition_index && contition_index<= eACC_TREASURE_SPECIALTY_FLAG8) ||
        (eACC_TREASURE_SPECIALTY_FLAG7 <= contition_index && contition_index<= eACC_TREASURE_SPECIALTY_FLAG8))
        return;*/

    DWORD AC_Key = MAKEDWORD(area_index, contition_index);    
    ITEM_MAP::iterator iter_item = iteminfo_.find(AC_Key);

    if(iter_item == iteminfo_.end())
    {   
        //SUNLOG(eCRITICAL_LOG, "Not Found ItemInfo %u, %u, %u\n",AC_Key, 
          //  area_index, contition_index);
        //ASSERT(!"Not Found ItemInfo");
        // __asm { int 3 };           
    }
    else
    {
        BASE_ITEMINFO* item_info;
        item_info = iter_item->second; 

        achievement_info.SetAchievementValue(item_info->m_Code, condition_value);
    }            

}

ACHIEVEMENT_OBJECT_TYPE 
ACConvertUnit::GetObjectType(int contition_index)
{
    if ( 0 <= contition_index && contition_index <= eACC_TREASURE_BOX_FLAG)
        return ACHIEVEMENT_OBJECT_ITEM;
    else if ( contition_index == eACC_HUNTING_KILL_CAPTAIN)
        return ACHIEVEMENT_OBJECT_NPC_AC;
    else if ( eACC_HUNTING_KILL_LUCKYMONSTER1 <= contition_index && 
                                                contition_index <= eACC_HUNTING_KILL_HIDDENBOSS2)
        return ACHIEVEMENT_OBJECT_NPC;
    else if ( eACC_QUEST_NORMAL <= contition_index && 
                                                  contition_index <= eACC_MISSION_CORRUPT_TEMPLE)
        return ACHIEVEMENT_OBJECT_MISSION;


    return ACHIEVEMENT_OBJECT_NOT_DEFINE;
}

void 
ACConvertUnit::InitACScriptData()
{    
    NPCInfoParser::Instance()->SetFirst();
    while (BASE_NPCINFO* temp_npcinfo = const_cast<BASE_NPCINFO*>(NPCInfoParser::Instance()->GetNext()))
    {        
        if((temp_npcinfo->m_wACCode > 0 && 
            temp_npcinfo->m_byACReferenceID > 0) &&
            temp_npcinfo->m_byACReferenceID != eACC_HUNTING_KILL_CAPTAIN)
        {
            DWORD key = MAKEDWORD(temp_npcinfo->m_wACCode, temp_npcinfo->m_byACReferenceID);                        
            
            if(npcInfo_.find(key) != npcInfo_.end())
            {   
                BASE_NPCINFO* duplication_npcinfo = npcInfo_.find(key)->second;

                char duplication_msg[512];
                sprintf( duplication_msg, "npcinfo (accode, acreference) duplication [(%u,%u,%u) (%u,%u,%u)]",
                    temp_npcinfo->m_MonsterCode, temp_npcinfo->m_wACCode, temp_npcinfo->m_byACReferenceID,
                    duplication_npcinfo->m_MonsterCode, duplication_npcinfo->m_wACCode, duplication_npcinfo->m_byACReferenceID );                

                SUNLOG(eCRITICAL_LOG, "%s\n",duplication_msg);
                ASSERT(!duplication_msg);
                __asm { int 3 };                
            }
            else
            {
                npcInfo_.insert(NPC_MAP::value_type(key, temp_npcinfo));
            }
        }
    }

    ItemInfoParser::Instance()->SetFirst();
    while(BASE_ITEMINFO* temp_iteminfo = ItemInfoParser::Instance()->GetNext())
    {     

        if(temp_iteminfo->m_wACCode > 0 || 
            temp_iteminfo->m_byACReferenceID > 0)
        {
            DWORD key = MAKEDWORD(temp_iteminfo->m_wACCode, temp_iteminfo->m_byACReferenceID);            
            
            
            if(iteminfo_.find(key) != iteminfo_.end())
            {   
                BASE_ITEMINFO* duplication_iteminfo = iteminfo_.find(key)->second;

                char duplication_msg[512];
                sprintf( duplication_msg, "iteminfo (accode, acreference) duplication [(%u,%u,%u) (%u,%u,%u)]",
                    temp_iteminfo->m_Code, temp_iteminfo->m_wACCode, temp_iteminfo->m_byACReferenceID,
                    duplication_iteminfo->m_Code, duplication_iteminfo->m_wACCode, duplication_iteminfo->m_byACReferenceID );                

                SUNLOG(eCRITICAL_LOG, "%s\n",duplication_msg);
                ASSERT(!duplication_msg);
                __asm { int 3 };
            }
            else
            {
                iteminfo_.insert(ITEM_MAP::value_type(key, temp_iteminfo));
            }
        }
    }

}

void ACConvertUnit::ACInfoConvertManager::Init()
{
    static AreaConquestInfo ac_infos[MAX_AC_SECTION_NUM];

    ACInfoConvertManager::AreaConquest::Init();    

    AreaConquestInfoParser* const ac_parser = AreaConquestInfoParser::Instance();
    for (int section = 0; section < MAX_AC_SECTION_NUM; ++section)
    {
        BASE_AreaConquestInfo* script_info = ac_parser->GetAreaConquestInfo(section + 1);

        if (script_info == NULL) {
            continue;
        }

        ac_infos[section].Init();
        ac_infos[section].SetAllCondition(script_info);
        SetAreaConquestInfo(section + 1, &ac_infos[section]);
    }

}

void ACConvertUnit::ACInfoConvertManager::AlignStream(BYTE ac_stream[MAX_ACSTREAM_SIZE],
                                                      BYTE mission_stream[MAX_MISSIONSTREAM_SIZE],
                                                      BYTE quest_complete_stream[MAX_COMPL_QUESTSTREAM_SIZE],
                                                      Character_Achievement_Unit& achievement_info)
{
    Serialize(ac_stream, MAX_ACSTREAM_SIZE, ACSERIALIZE_SAVE);    
    mission_manager_.SerializeStream(mission_stream, MAX_MISSIONSTREAM_SIZE, SERIALIZE_STORE);
    quest_manager_static_.SerializeComplStream(quest_complete_stream, 
        MAX_COMPL_QUESTSTREAM_SIZE, QSERIALIZE_SAVE);

    for (int section = 0; section < MAX_AC_SECTION_NUM; ++section)
    {
        AreaConquestInfo* ac_info = GetAreaConquestInfo(section + 1);
        _AlignMissionInfo(ac_info, achievement_info);
        _AlignQuestInfo(ac_info, achievement_info);
        ac_info->CalculateCurrentAchievePoint();
        int achieve_point = util::LL_Round(ac_info->GetCurrentAchievePoint() * 100.0f);
        achieve_point = 0;
    }
}

void
ACConvertUnit::ACInfoConvertManager::_AlignMissionInfo(AreaConquestInfo* ac_info, Character_Achievement_Unit& achievement_info)
{
    // injection other info.
    // #1 Mission
    MapInfoParser* const map_info_parser = MapInfoParser::Instance();
    const BASE_AreaConquestInfo* script_info = ac_info->GetBaseACInfo();
    MapGroup* map_group = map_info_parser->GetMapGroup(script_info->m_byMissionMapKind);
    if (map_group == NULL) {
        return;
    }

    WORD point = 0;
    //메인 미션
    const sMAPINFO* map_info = map_group->GetMapInfoIndex(0);
    if (map_info != NULL) {
        point = mission_manager_.GetMissionPoint(static_cast<BYTE>(map_info->MapCode + 1));
        if (point != 0) {
            ac_info->SetConditionValue(eACC_MISSION_MAIN, point);

            achievement_info.SetAchievementValue(map_info->MapCode, 
                ac_info->GetConditionValue(eACC_MISSION_MAIN));
        }
    }
    //서브 미션, __NA_1288_AC_RENEWAL_FOR_TIME_LINE
    for (int i = 1; i < MAX_AC_SUB_MISSION_NUM; ++i)
    {
        map_info = map_group->GetMapInfoIndex(i);
        if (map_info == NULL) {
            continue;
        }
        point = mission_manager_.GetMissionPoint(static_cast<BYTE>(map_info->MapCode + 1));
        if (point != 0) {
            ac_info->SetConditionValue(eACC_MISSION_SUB1 - 1 + i, point);

            achievement_info.SetAchievementValue(map_info->MapCode, 
                ac_info->GetConditionValue(eACC_MISSION_SUB1 - 1 + i));          
        }
    }
    // 저탑/타락한 사원등은 이미 AC스트림에 반영되어 있다.
}


void
ACConvertUnit::ACInfoConvertManager::_AlignQuestInfo(AreaConquestInfo* ac_info, Character_Achievement_Unit& achievement_info)
{
    const BASE_AreaConquestInfo* ac_script_info = ac_info->GetBaseACInfo();
    for(QCODE i=1;i<QUESTPART_COMPL::MAX_QUEST_COMPL_NUM;++i)
    {
        QUESTPART_COMPL* pComplQuest = quest_manager_static_.FindComplQuest(i);
        if (pComplQuest == NULL || pComplQuest->m_Num == 0) {
            continue;
        }
        QuestInfo* pInfo = quest_info_manager_global_.FindQuestInfo(i);
        if (pInfo == NULL) {
            continue;
        }
        if (ac_script_info->m_wIndex == pInfo->GetACCode())
        {
            switch(pInfo->GetQtype())
            {
            case eQTYPE_GENERAL:    
                {                    
                    ac_info->SetConditionValue(eACC_QUEST_NORMAL,
                        ac_info->GetConditionValue(eACC_QUEST_NORMAL) + 1);  

                    const BASE_AreaConquestInfo* script_info = ac_info->GetBaseACInfo();

                    achievement_info.SetAchievementValue(script_info->m_byMissionMapKind + 200, 
                        ac_info->GetConditionValue(eACC_QUEST_NORMAL));                             
                }
                break;
            case eQTYPE_ELITE:
                ac_info->SetConditionValue(eACC_QUEST_EVENT, \
                    ac_info->GetConditionValue(eACC_QUEST_EVENT) + 1);
                break;
            case eQTYPE_MISSION:
                ac_info->SetConditionValue(eACC_QUEST_MISSION, \
                    ac_info->GetConditionValue(eACC_QUEST_MISSION) + 1);
                break;
            case eQTYPE_HIDDEN:
                ac_info->SetConditionValue(eACC_QUEST_HIDDEN, \
                    ac_info->GetConditionValue(eACC_QUEST_HIDDEN) + 1);
                break;
            }
        }
    }
}

} //end namespace batch_unit 

#endif //C_NA_0_20100520_ACSYSTEM_CHANGED

#endif