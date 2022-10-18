#include "stdafx.h"
#include "DBQueryProxyNode.h"
#include "QueryObjects/DBHandler.h"


//==================================================================================================

namespace nsQuery {
;

//==================================================================================================

void MissionUpdateNode::UpdateAndCompareData()
{
    if (record_changed_ != this->eRecordChange_Default) {
        return;
    };
    
    const bool new_insert = (commited_data_.code != latest_data_.code);
    if (new_insert) {
        record_changed_ = this->eRecordChange_NewInsert;
        return;
    }
    bool changed = commited_data_.state != latest_data_.state ||
                   commited_data_.point != latest_data_.point ||
                   commited_data_.hidden != latest_data_.hidden;

    if (changed) {
        record_changed_ = this->eRecordChange_Changed;
        return;
    }
    ; // no changed
    return;
}

// G->D update stream
void DBQueryProxyNode::CharacterNode::StoreMission(const MSG_DG_CHARINFO_QUEST_CMD& msg_cmd)
{
    BOOST_STATIC_ASSERT(sizeof(msg_cmd.m_pMissionStream) ==
                        sizeof(MISSIONPART) * MAX_MISSION_NUM &&
                        _countof(mission_table_.records_) == MAX_MISSION_NUM);
    //
    ;{  // clear latest information
        MissionUpdateNode* update_it = mission_table_.records_,
            * const update_end = &mission_table_.records_[_countof(mission_table_.records_)];
        for ( ; update_it != update_end; ++update_it) {
            ZeroMemory(&update_it->latest_data_, sizeof(update_it->latest_data_));
        }
    };
    // specification of the msg_cmd.m_pMissionStream by f101130.2L
    //              [00]     [01]     [02]   ...   [31]
    // f101130.2L MISSION1|MISSION2|MISSION3|...|MISSION32|
    const MISSIONPART* slot_it = reinterpret_cast<const MISSIONPART*>(msg_cmd.m_pMissionStream),
                     * slot_end = reinterpret_cast<const MISSIONPART*>(\
                        &msg_cmd.m_pMissionStream[sizeof(msg_cmd.m_pMissionStream)]);
    for (int index = 0; slot_it != slot_end; ++index, ++slot_it)
    {
        const uint8_t mission_code = static_cast<uint8_t>(index + 1);
        MissionUpdateNode* const update_node = &mission_table_.records_[index];
        if (nsQuery::MissionResultRecord* const latest_info = &update_node->latest_data_)
        {
            latest_info->code = mission_code;
            latest_info->state = slot_it->State;
            latest_info->point = slot_it->Point;
            latest_info->hidden = slot_it->Hidden;
            update_node->UpdateAndCompareData();
        };
    }
}

// D->G send stream
void DBQueryProxyNode::CharacterNode::LoadMission(MSG_DG_CHARINFO_QUEST_CMD* msg_cmd) const
{
    // rules : reference 'StoreMission'
    BOOST_STATIC_ASSERT(sizeof(msg_cmd->m_pMissionStream) ==
                        sizeof(MISSIONPART) * MAX_MISSION_NUM &&
                        _countof(mission_table_.records_) == MAX_MISSION_NUM);
    // specification of the msg_cmd->m_pMissionStream by f101130.2L
    //              [00]     [01]     [02]   ...   [31]
    // f101130.2L MISSION1|MISSION2|MISSION3|...|MISSION32|
    //
    ZeroMemory(msg_cmd->m_pMissionStream, sizeof(msg_cmd->m_pMissionStream));
    //
    MISSIONPART* dest_it = reinterpret_cast<MISSIONPART*>(msg_cmd->m_pMissionStream);
    for (const MissionUpdateNode* record_it = mission_table_.records_;
         record_it != &mission_table_.records_[_countof(mission_table_.records_)];
         ++record_it, ++dest_it)
    {
        const nsQuery::MissionResultRecord& latest_info = record_it->latest_data_;
        if (latest_info.code == 0) {
            // the destination position of 'MISSIONPART' is already cleared.
            continue;
        }
        const int mission_code = static_cast<int>(record_it - mission_table_.records_) + 1;
        assert(mission_code == latest_info.code);__TOUCHED(mission_code);
        dest_it->State = latest_info.state;
        dest_it->Point = latest_info.point;
        dest_it->Hidden = latest_info.hidden;
    }
}

bool DBQueryProxyNode::CharacterNode::DBResult(const Query_MissionInfo_Select& query_result)
{
    // 1st load
    const int number_of_rows = query_result.GetResultRowNum();
    const Query_MissionInfo_Select::sQueryResult* record_it = query_result.result_records_,
        *record_end = &query_result.result_records_[number_of_rows];
    for ( ; record_it != record_end; ++record_it)
    {
        if (record_it->code <= 0) {
            assert(!"unexpected");
            continue;
        }
        const int offset = record_it->code - 1;
        MissionUpdateNode* const update_node = &mission_table_.records_[offset];
        update_node->latest_data_ = *record_it;
        update_node->commited_data_ = update_node->latest_data_;
    }
    //
    return true;
}

bool DBQueryProxyNode::CharacterNode::DBResult(const Query_MissionInfo_Insert& query_result)
{
    const Query_MissionInfo_Insert::sQueryParam& parameters = query_result.parameters_[0];
    const int offset = parameters.code - 1;
    if (FlowControl::FCAssert(offset >= 0 && offset < _countof(mission_table_.records_)) == false) {
        // make to a disable record anymore, to do check the query object making step
        return false;
    }
    MissionUpdateNode* update_node = &mission_table_.records_[offset];
    ASSERT(update_node->query_ == &query_result &&
           update_node->record_changed_ == update_node->eRecordChange_NewInsert);
    update_node->record_changed_ = update_node->eRecordChange_Default;
    update_node->query_ = NULL; // external delete control
    update_node->commited_data_ = parameters;
    //
    return true;
}

bool DBQueryProxyNode::CharacterNode::DBResult(const Query_MissionInfo_Update& query_result)
{
    const Query_MissionInfo_Update::sQueryParam& parameters = query_result.parameters_[0];
    const int offset = parameters.code - 1;
    if (FlowControl::FCAssert(offset >= 0 && offset < _countof(mission_table_.records_)) == false) {
        // make to a disable record anymore, to do check the query object making step
        return false;
    }
    MissionUpdateNode* update_node = &mission_table_.records_[offset];

    ASSERT(update_node->query_ == &query_result &&
           update_node->record_changed_ == update_node->eRecordChange_Changed);
    update_node->record_changed_ = update_node->eRecordChange_Default;
    update_node->query_ = NULL; // external delete control
    update_node->commited_data_ = parameters;
    //
    return true;
}

bool DBQueryProxyNode::CharacterNode::UpdateMissionInfo()
{
    const bool loaded_mission = (shared_.state & this->eState_Loaded_MissionInfo) != 0;
    if (loaded_mission == false) {
        return true;
    };
    DBUser* const db_user = db_character_->GetParentDBUser();
    if (db_user == NULL)
    {
        return true;    // °­Á¦..transaction success..
    }
    const USERGUID user_guid = db_user->GetUserGuid();
    const CHARGUID char_guid = db_character_->GetCharGuid();
    // CHANGES: f110628.1L, the load balance updating implemented. updater='db_user->DBQuery'
    const int number_of_pending_queries = \
        g_DBHandler.GetNumberOfDBConnectionQueuingQueries(db_user);
    if (number_of_pending_queries > g_DBHandler.kMaxNumberOfSpecializedConcurrentBusyUpdateQueries) {
        return false; // pendings
    };
    //
    bool changed_events = false;
    STRING_SAFER_QUERY64 query_buffer;
    //
    MissionUpdateNode* update_node_it = mission_table_.records_,
        * const update_node_end = &mission_table_.records_[_countof(mission_table_.records_)];
    for ( ; update_node_it != update_node_end; ++update_node_it)
    {
        if (update_node_it->query_ != NULL) {
            changed_events = true;
            continue; //in transaction
        }
        update_node_it->UpdateAndCompareData();
        if (update_node_it->record_changed_ == update_node_it->eRecordChange_Default) {
            continue;
        }
        //
        switch (update_node_it->record_changed_)
        {
        case update_node_it->eRecordChange_Changed: // update
            {
                query_buffer.MakeString(_T("{?=call S_MissionInfo_Update (?,?,?,?,?)}"));
                Query_MissionInfo_Update* const query = Query_MissionInfo_Update::ALLOC();
                query->SetQuery(query_buffer);
                query->SetUserKey(user_guid);
                query->set_char_guid(char_guid);
                // setup parameters
                if (Query_MissionInfo_Update::sQueryParam* parameters = &query->parameters_[0])
                {
                    static_cast<MissionResultRecord&>(*parameters) = update_node_it->latest_data_;
                    parameters->char_guid_ = char_guid;
                }
                //
                update_node_it->query_ = query;
            };
            break;
        case update_node_it->eRecordChange_NewInsert: // insert
            {
                query_buffer.MakeString(_T("{?=call S_MissionInfo_Insert (?,?,?,?,?)}"));
                Query_MissionInfo_Insert* const query = Query_MissionInfo_Insert::ALLOC();
                query->SetQuery(query_buffer);
                query->SetUserKey(user_guid);
                query->set_char_guid(char_guid);
                // setup parameters
                if (Query_MissionInfo_Insert::sQueryParam* parameters = &query->parameters_[0])
                {
                    static_cast<MissionResultRecord&>(*parameters) = update_node_it->latest_data_;
                    parameters->char_guid_ = char_guid;
                }
                //
                update_node_it->query_ = query;
                //
            };
            break;
        }
        if (changed_events == false && update_node_it->query_) {
            changed_events = true;
        }
        if (update_node_it->query_)
        {
            db_user->DBQuery(DG_CHARINFO, DG_CHARINFO_CHARLISTREQ_INTER_MISSION_DBR,
                             update_node_it->query_);
        }
    };
    if (changed_events) {
        return false;
    }

    return true;
}

//==================================================================================================

}; //end of namespace


