#include "stdafx.h"
#include "BatchProcessor.h"

#if DBPROXY_TYPE_SELECTOR == DBPROXY_TYPE_UNIT

#include <MapInfoParser.h>
#include "QuestManagerForUnit.h"
//#include <QuestScriptLoader.h>
//#include <QuestManager.h>
//#include <QuestInfoManager.h>

#include "VersionControl.h"

#define BATCHUNIT_REGISTERING_MARK
#include "./ACAchievementUpdateUnit.hxx"
#undef BATCHUNIT_REGISTERING_MARK

//==================================================================================================
// implements

namespace batch_unit {

using batch::IBatchUnit;

void
ACAchievementUpdateUnit::UpdateNode::Init()
{
    ZeroMemory(this, sizeof(*this));
    util::LList::Init(this);
}

ACAchievementUpdateUnit::ACAchievementUpdateUnit()
    : step_(STEP_NONE)
    , total_completed_(false)
    , suspend_query_character_(false)
    , completed_query_character_(false)
{
    const DWORD UserSlotLastKey = 10;
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

ACAchievementUpdateUnit::~ACAchievementUpdateUnit()
{
}

inline IBatchUnit::eSTEP
ACAchievementUpdateUnit::CurrentStep() const
{
    return step_;
}

BOOLEAN
ACAchievementUpdateUnit::BatchInit()
{
    SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"]\n");

    ZeroMemory(&arguments_, sizeof(arguments_));
    util::LList::Init(&arguments_.pending_nodes_);
    util::LList::Init(&arguments_.updating_nodes_);

    timer_.SetTimer(300);
    statistics_timer_.SetTimer(5000);
    step_ = this->STEP_PROCESSING;
    return true;
}

BOOLEAN
ACAchievementUpdateUnit::BatchProcess()
{
    if (timer_.IsExpired() == false)
        return true;

    //printf("["__FUNCTION__"]\n");

    SelectCharList();
    UpdateACInfo();

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
ACAchievementUpdateUnit::BatchRelease()
{
    printf("["__FUNCTION__"]\n");
    //for (PendingNode* it = pending_nodes_;
    //     it != &pending_nodes_[_countof(pending_nodes_)];
    //     ++it)
    //{
    //    SAFE_DELETE(it->character_);
    //    ZeroMemory(it, sizeof(*it));
    //}
    step_ = this->STEP_RELEASED;
    return false;
}

void
ACAchievementUpdateUnit::DBResult(DWORD index, QueryResult* query_result)
{
    BOOST_STATIC_ASSERT(sizeof(KeyNode) == sizeof(index));
    KeyNode key_node = { 0, };
    key_node.composed_ = index;

    switch (key_node.query_type_)
    {
    case key_node.QUERY_SELECT_CHARACTERS:
        {
            SelectedCharList(key_node, query_result);
        }
        break;
    case key_node.QUERY_UPDATE_AC:
        {
            UpdatedACInfo(key_node, query_result);
        }
        break;
    }
}

}; //end of namespace 'batch_unit'
//
//
//==================================================================================================
//
//
namespace batch_unit {
;

BOOLEAN
ACAchievementUpdateUnit::SelectCharList()
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
    query_buffer.MakeString("{call USP_GlobalSun_GetACStreamInfo(%d, ?)}",
                            arguments_.selected_page_);

    Query_GetACStreamInfo* query = Query_GetACStreamInfo::ALLOC();
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

BOOLEAN
ACAchievementUpdateUnit::SelectedCharList(const KeyNode& index, QueryResult* query_result_base)
{
    select_key_generator_.RestoreKey(index.node_key_);

    ++statistics_.number_of_get_queried_;
    ++statistics_.solved_selectings_;

    Query_GetACStreamInfo* const query_result = static_cast<Query_GetACStreamInfo*>(query_result_base);
    const DWORD number_of_rows = query_result->GetResultRowNum();

    if (number_of_rows == 0)
    {
        Query_GetACStreamInfo::FREE(query_result);
        // force stop
        completed_query_character_ = true;
        return false;
    }

    // latest update
    arguments_.total_pages_ = query_result->pParam[0].end_page_;

    static AreaConquestForACUnit ac;
    static BYTE ac_stream_static[MAX_ACSTREAM_SIZE];
    ZeroMemory(ac_stream_static, sizeof(ac_stream_static));

    static BYTE mission_stream_static[MAX_MISSIONSTREAM_SIZE];
    ZeroMemory(mission_stream_static, sizeof(mission_stream_static));

    static BYTE quest_stream_static[MAX_COMPL_QUESTSTREAM_SIZE];
    ZeroMemory(quest_stream_static, sizeof(quest_stream_static));

    static BYTE quest_prog_static_dummy[MAX_PROGR_QUESTSTREAM_SIZE];
    ZeroMemory(quest_prog_static_dummy, sizeof(quest_prog_static_dummy));

    DWORD row = 0;
    for (; row < number_of_rows; ++row)
    {
        Query_GetACStreamInfo::sQUERY& result = query_result->pResult[row];
        int converted_ac_version = 0;
        if(AreaConquest::ACStreamConverter(MAX_AC_SECTION_NUM,
                                           result.ac_version_, result.ac_stream_,
                                           converted_ac_version, ac_stream_static) == false)
        {
            ASSERT(!"AC stream conversion failed!");
            __asm { int 3 };
        }

        CopyMemory(mission_stream_static, result.mission_stream_, MAX_MISSIONSTREAM_SIZE);

        int converted_quest_version = 0;
        if( FALSE == VersionControl::QuestConverter(converted_quest_version,
                                                    quest_prog_static_dummy,
                                                    quest_stream_static,
                                                    result.quest_version_,
                                                    quest_prog_static_dummy,
                                                    result.quest_complete_stream_))
        {
            ASSERT(!"AC stream conversion failed!");
            __asm { int 3 };
        }

        ac.Init();
        ac.AlignStream(ac_stream_static, mission_stream_static, quest_stream_static);

        //update_node->state_ = update_node->STATE_QUERYING //unused field

        //
        int need_update_check = 0;
        int achieve_points[MAX_AC_SECTION_NUM] = { 0, };
        for (int section = 0; section < MAX_AC_SECTION_NUM; ++section)
        {
            int achieve_point = ac.GetCurrentAchievePointForUpdate(section + 1);
            //printf("|Character=%d|AchievePoint=%d|\n", result.char_guid_, achieve_point);
            achieve_points[section] = achieve_point;
            need_update_check += achieve_point;
        };

        if (need_update_check == 0) 
        {
            //2010-05-18 문상현 AC 달성률이 모두 0 이면 write 하지 않았으나 혼돈이 예상되어 무조건  write 하도록한다.
            //continue; //no update info
        }

        //if (need_update_check != 0)
        UpdateNode* update_node = TAllocNew(UpdateNode);
        update_node->Init();
        //update_node->key_ = key_generator_.GetKey(); //assert key_ != 0
        update_node->query_ = Query_SetACStreamInfo::ALLOC();
        // alignment processed values

        Query_SetACStreamInfo* query = update_node->query_;
        Query_SetACStreamInfo::sPARAM* arg = &query->pParam[0];
        arg->server_guid_ = result.server_guid_;
        arg->char_guid_ = result.char_guid_;
        arg->area1_ = achieve_points[0];
        arg->area2_ = achieve_points[1];
        arg->area3_ = achieve_points[2];
        arg->area4_ = achieve_points[3];
        arg->area5_ = achieve_points[4];
        arg->area6_ = achieve_points[5];

        // debug
        for (int section = 0; section < MAX_AC_SECTION_NUM; ++section) {
            /*if (achieve_points[section] != 0) {
                SUNLOG(eCRITICAL_LOG,
                       "|UPDATES|Character=%d|Area=%d|AchievePoint=%4d(%2.2f)|",
                       result.char_guid_, section + 1,
                       achieve_points[section],
                       static_cast<float>(achieve_points[section]) / 100.0f);
            }*/
        }

        //
        util::LList::AddPrev(update_node, &arguments_.pending_nodes_);
        ++arguments_.number_of_pending_nodes_;
    }

    Query_GetACStreamInfo::FREE(query_result);
    return true;
}

BOOLEAN
ACAchievementUpdateUnit::UpdateACInfo()
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

        Query_SetACStreamInfo* const query = extracted->query_;
        query->node_pointer_ = extracted; // passing object;

        util::LList::AddPrev(extracted, &arguments_.updating_nodes_);
        ++arguments_.number_of_updating_nodes_;

        // query { update AC info };
        const Query_SetACStreamInfo::sPARAM* arg = &query->pParam[0];

        STRING_SAFER_QUERY128 query_buffer;
        query_buffer.MakeString(
            "{call USP_GlobalSun_SetACStreamInfo(%d, %d, %d, %d, %d, %d, %d, %d)}",
            arg->server_guid_, arg->char_guid_,
            arg->area1_, arg->area2_, arg->area3_, arg->area4_, arg->area5_, arg->area6_);

        KeyNode key_node = { 0, };
        key_node.query_type_ = key_node.QUERY_UPDATE_AC;
        key_node.node_key_ = static_cast<WORD>(extracted->key_);
        query->SetVoidObject(this);
        query->SetIndex(key_node.composed_);
        query->SetQuery(query_buffer);

        SolarDatabaseInDBP* second_db = g_DBProxyServer.Get2ndDatabase();
        second_db->DBQuery(query);
        /*
        static AreaConquestForACUnit ac;
        ac.Init();

        //BYTE m_pACStream[MAX_ACSTREAM_SIZE];
        ac.AlignStream(extracted->character_);
        int need_update_check = 0;
        int achieve_points[MAX_AC_SECTION_NUM] = { 0, };
        const char* character_name = extracted->character_->GetCharName();
        for (int section = 0; section < MAX_AC_SECTION_NUM; ++section)
        {
            int achieve_point = ac.GetCurrentAchievePointForUpdate(section + 1);
            printf("|Character=%s|AchievePoint=%d|\n", character_name, achieve_point);
            achieve_points[section] = achieve_point;
            need_update_check += achieve_point;
        };

        if (need_update_check != 0)
        {
            for (int section = 0; section < MAX_AC_SECTION_NUM; ++section) {
                SUNLOG(eCRITICAL_LOG, "|UPDATES|Character=%s|Area=%d|AchievePoint=%4d(%2.2f)|",
                       character_name, section + 1,
                       achieve_points[section],
                       static_cast<float>(achieve_points[section]) / 100.0f);
            }
        }
        */
    }

    return true;
}

BOOLEAN
ACAchievementUpdateUnit::UpdatedACInfo(const KeyNode& index, QueryResult* query_result_base)
{
    key_generator_.RestoreKey(index.node_key_);

    ++statistics_.number_of_set_queried_;
    //static int queried = 0;
    //printf("Number of queried=%u\n", ++queried);

    Query_SetACStreamInfo* const query_result = static_cast<Query_SetACStreamInfo*>(query_result_base);

    UpdateNode* update_node = reinterpret_cast<UpdateNode*>(query_result->node_pointer_);
    util::LList::Delete(update_node);

    if (arguments_.number_of_updating_nodes_)
        --arguments_.number_of_updating_nodes_;

    ++statistics_.solving_updatings_;
    //printf("Updated %s\n", query_result->GetQuery());

    TAllocDelete(UpdateNode, update_node);
    Query_SetACStreamInfo::FREE(query_result);

    // balance formula
    size_t remains = key_generator_.GetSize();
    suspend_query_character_ = remains < 10 ||
        (MAX_GET_ACSTREAM_INFO_ROWS_PER_PAGE * 7) < arguments_.number_of_pending_nodes_;

    return true;
}

}; //end of namespace 'batch_unit'
//
//
//==================================================================================================
//
//
namespace batch_unit {
;

//QuestScriptLoader quest_script_parser_global;
//QuestInfoManager quest_info_manager_global_;

//class QuestManagerForUnit : public QuestManager
//{
//public:
//    QuestManagerForUnit( QuestInfoManager* pmgr ) : QuestManager(pmgr) {}
//
//    virtual QuestCondition * CreateQuestCondition( QuestConditionInfo * pInfo ) { return 0; }
//    virtual QuestAction * CreateQuestAction( QuestActionInfo * pInfo ) { return 0; }
//    virtual void DestroyQuestCondition( QuestCondition * pCondition ) { return; }
//    virtual void DestroyQuestAction( QuestAction * pAction ) { return; }
//
//    // 퀘스트 완료, 실패에 따른 이벤트 알림
//    virtual void OnEvent( eQUEST_EVENT evt, QUEST_MSG * pMsg, Quest * pQuest ) { return; }
//};
//
//QuestManagerForUnit quest_manager_static_(&quest_info_manager_global_);


AreaConquestForACUnit::AreaConquestForACUnit()
{
}

AreaConquestForACUnit::~AreaConquestForACUnit()
{
}

void
AreaConquestForACUnit::Init()
{
    static AreaConquestInfo ac_infos[MAX_AC_SECTION_NUM];

    AreaConquest::Init();

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

    mission_manager_.Clear();
}

void
AreaConquestForACUnit::AlignStream(BYTE ac_stream[MAX_ACSTREAM_SIZE],
                                   BYTE mission_stream[MAX_MISSIONSTREAM_SIZE],
                                   BYTE quest_complete_stream[MAX_COMPL_QUESTSTREAM_SIZE])
{
    Serialize(ac_stream, MAX_ACSTREAM_SIZE, ACSERIALIZE_SAVE);
    // no version control (10/01/14)
    mission_manager_.SerializeStream(mission_stream, MAX_MISSIONSTREAM_SIZE, SERIALIZE_STORE);
    quest_manager_static_.SerializeComplStream(quest_complete_stream, \
        MAX_COMPL_QUESTSTREAM_SIZE, QSERIALIZE_SAVE);
}

int
AreaConquestForACUnit::GetCurrentAchievePointForUpdate(int area_index)
{
    AreaConquestInfo* ac_info = GetAreaConquestInfo(area_index);
    if (ac_info == NULL) {
        return 0;
    }
    _AlignMissionInfo(area_index, ac_info);
    _AlignQuestInfo(area_index, ac_info);

    ac_info->CalculateCurrentAchievePoint();
    int achieve_point = util::LL_Round(ac_info->GetCurrentAchievePoint() * 100.0f);
    return achieve_point;
}

void
AreaConquestForACUnit::_AlignMissionInfo(int area_index, AreaConquestInfo* ac_info)
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
        point = mission_manager_.GetMissionPoint(static_cast<BYTE>(map_info->MapCode));
        if (point != 0) {
            ac_info->SetConditionValue(eACC_MISSION_MAIN, point);
        }
    }
    //서브 미션, __NA_1288_AC_RENEWAL_FOR_TIME_LINE
    for (int i = 1; i < MAX_AC_SUB_MISSION_NUM; ++i)
    {
        map_info = map_group->GetMapInfoIndex(i);
        if (map_info == NULL) {
            continue;
        }
        point = mission_manager_.GetMissionPoint(static_cast<BYTE>(map_info->MapCode));
        if (point != 0) {
            ac_info->SetConditionValue(eACC_MISSION_SUB1 - 1 + i, point);
        }
    }
    // 저탑/타락한 사원등은 이미 AC스트림에 반영되어 있다.
}


void
AreaConquestForACUnit::_AlignQuestInfo(int area_index, AreaConquestInfo* ac_info)
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
            switch(pInfo->GetQType())
            {
            case eQTYPE_GENERAL:
                ac_info->SetConditionValue(eACC_QUEST_NORMAL,
                                           ac_info->GetConditionValue(eACC_QUEST_NORMAL) + 1);
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


}; //end of namespace 'batch_unit'
//
//==================================================================================================
#endif //DBPROXY_TYPE_SELECTOR == DBPROXY_TYPE_UNIT

