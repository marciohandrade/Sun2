#include "stdafx.h"
#include "DBQueryProxyNode.h"
#include <Protocol_DG.h>
#include "QueryObjects/DBHandler.h"

//

namespace nsQuery {
;

void CharStateUpdateNode::ChangeData(const STATE_DETAIL_INFO& inputed_data)
{
    // CharStateResultRecord <-- STATE_DETAIL_INFO
    latest_data_.state_code_ = inputed_data.m_wStateCode;
    latest_data_.skill_code_ = inputed_data.m_SkillCode;
    latest_data_.ability_index_with_option_ = inputed_data.m_byAbilityIndex;
    latest_data_.remained_time_ = inputed_data.m_iRemainTime;
    latest_data_.interval_ = inputed_data.m_wInterval;
}

void CharStateUpdateNode::UpdateAndCompareData()
{
    if (record_changed_ != this->eRecordChange_Default) {
        return;
    };
    //
    const bool key_changed = (commited_data_.state_code_ != latest_data_.state_code_);
    if (key_changed) {
        if (latest_data_.state_code_ == 0) {
            record_changed_ = this->eRecordChange_NeedDelete;
            return;
        }
        commited_data_.state_code_ = latest_data_.state_code_;
    }
    const bool changed = (commited_data_.skill_code_ != latest_data_.skill_code_ ||
                          commited_data_.ability_index_with_option_ != \
                            latest_data_.ability_index_with_option_ ||
                          commited_data_.remained_time_ != latest_data_.remained_time_ ||
                          commited_data_.interval_ != latest_data_.interval_);
    if (key_changed) {
        record_changed_ = this->eRecordChange_NewInsert;
        return;
    }
    if (changed) {
        record_changed_ = this->eRecordChange_Changed;
    }
    // no changed
}

// G->D update stream
void DBQueryProxyNode::CharacterNode::StoreCharStates(const STATE_DETAIL_TOTAL_INFO& data_list)
{
    struct FilteredState {
        enum eMode {
            None = 0, Touched,
        };
        struct Node {
            eMode table_check_;
            STATE_DETAIL_INFO detail_info_;
        };
        //
        FilteredState() {
            ZeroMemory(this, sizeof(*this));
            BOOST_STATIC_ASSERT((boost::is_same<STATE_DETAIL_TOTAL_INFO::SLOT_TYPE,
                                                STATE_DETAIL_INFO>::value));
        }
        //
        size_t number_of_states_;
        Node states_[MAX_DBSAVE_STATEINFO_NUM];
    } filter_states_;
    //
    static uint32_t touch_sequence_s = 0;
    const uint32_t touch_sequnce = ++touch_sequence_s;
    //
    const STATE_DETAIL_TOTAL_INFO::SLOT_TYPE* slot_it = data_list.m_Slot,
                                            * const slot_end = &data_list.m_Slot[data_list.m_Count];
    FilteredState::Node* filter_it = filter_states_.states_;
    size_t process_num = 0;
    for ( ; process_num < _countof(filter_states_.states_) && slot_it != slot_end; ++slot_it)
    {
        const eCHAR_STATE_TYPE char_state_id = static_cast<eCHAR_STATE_TYPE>(slot_it->m_wStateCode);
        switch (char_state_id)
        {
        case eCHAR_STATE_INVALID:
        case eCHAR_STATE_TRANSFORMATION:
            continue;
        };
        //
        filter_it->detail_info_ = *slot_it;
        ++filter_it;
        ++process_num;
    };
    ;{
        assert(process_num <= _countof(filter_states_.states_));
        filter_states_.number_of_states_ = process_num;
    };
    //
    process_num = 0;
    const FilteredState::Node* const filter_end = \
        &filter_states_.states_[filter_states_.number_of_states_];
    for (filter_it = filter_states_.states_; filter_it != filter_end; ++filter_it)
    {
        const STATE_DETAIL_TOTAL_INFO::SLOT_TYPE* detail_info = &filter_it->detail_info_;
        const uint16_t state_id = detail_info->m_wStateCode;
        CharStateTable::iterator it = char_state_table_.find(state_id);
        CharStateUpdateNode* node = NULL;
        if (bool is_found = (it != char_state_table_.end())) {
            node = &it->second;
        }
        else
        {   // NOTE: an automatic new insertion operation
            node = &char_state_table_[state_id];
            ZeroMemory(node, sizeof(*node));
        };
        //++process_num;
        node->store_touch_ = touch_sequnce;
        //
        node->ChangeData(*detail_info);
    };
    //const size_t number_of_nodes = char_state_table_.size();
    //if (number_of_nodes != process_num)
    {   // exist that need to delete a item
        FOREACH_CONTAINER(CharStateTable::value_type& node, char_state_table_, CharStateTable)
        {
            CharStateUpdateNode& update_node = node.second;
            if (update_node.store_touch_ != touch_sequnce) {
                //update_node.record_changed_ = update_node.eRecordChange_NeedDelete;
                // change to 'eRecordChange_NeedDelete' in Update~ when a state_code is 0.
                update_node.latest_data_.state_code_ = 0;
            }
        };
    }
}

// D->G send stream
void DBQueryProxyNode::CharacterNode::LoadCharStates(STATE_DETAIL_TOTAL_INFO* data_list) const
{
    data_list->m_Count = 0;
    STATE_DETAIL_TOTAL_INFO::SLOT_TYPE* slot_it = data_list->m_Slot;
    int number = 0;
    //
    FOREACH_CONTAINER(const CharStateTable::value_type& node, char_state_table_, CharStateTable)
    {
        if (number >= MAX_DBSAVE_STATEINFO_NUM) {
            break;
        }
        const CharStateUpdateNode& update_node = node.second;
        const CharStateResultRecord& latest_info = update_node.latest_data_;
        if (latest_info.state_code_ == 0) {
            continue; // delete reserved
        }
        // STATE_DETAIL_TOTAL_INFO::SLOT_TYPE <-- CharStateResultRecord
        slot_it->m_wStateCode = latest_info.state_code_;
        slot_it->m_SkillCode = static_cast<SLOTCODE>(latest_info.skill_code_);
        slot_it->m_byAbilityIndex = static_cast<uint8_t>(latest_info.ability_index_with_option_);
        slot_it->m_iRemainTime = latest_info.remained_time_;
        slot_it->m_wInterval = latest_info.interval_;
        //
        ++slot_it;
        ++number;
    }
    //
    BOOST_STATIC_ASSERT((UCHAR_MAX + 1) > MAX_DBSAVE_STATEINFO_NUM);
    data_list->m_Count = static_cast<uint8_t>(slot_it - data_list->m_Slot);
    //
}

bool DBQueryProxyNode::CharacterNode::DBResult(const Query_Char_State_Select& query_result)
{
    // 1st load
    if (FlowControl::FCAssert(char_state_table_.empty()) == false) {
        return false;
    }
    
    const int number_of_rows = query_result.GetResultRowNum();
    for (int i = 0; i < number_of_rows; ++i)
    {
        const Query_Char_State_Select::sQueryResult& record = query_result.result_records_[i];
        CharStateUpdateNode* const node = &char_state_table_[record.state_code_];
        ZeroMemory(node, sizeof(*node));
        node->commited_data_ = record;
        node->latest_data_ = record;
    }
    //
    return true;
}

bool DBQueryProxyNode::CharacterNode::DBResult(const Query_Char_State_Insert& query_result)
{
    const Query_Char_State_Insert::sQueryParam& parameters = query_result.parameters_[0];
    CharStateTable::iterator it = char_state_table_.find(parameters.state_code_);
    if (!FlowControl::FCAssert(it != char_state_table_.end())) {
        return false;
    }
    //
    CharStateUpdateNode* const node = &it->second;
    ASSERT(node->query_ == &query_result &&
           node->record_changed_ == node->eRecordChange_NewInsert);
    node->record_changed_ = node->eRecordChange_Default;
    node->query_ = NULL; // external delete control
    node->commited_data_ = parameters;
    //
    return true;
}

bool DBQueryProxyNode::CharacterNode::DBResult(const Query_Char_State_Delete& query_result)
{
    const Query_Char_State_Delete::sQueryParam& parameters = query_result.parameters_[0];
    CharStateTable::iterator it = char_state_table_.find(parameters.state_code_);
    if (!FlowControl::FCAssert(it != char_state_table_.end())) {
        return false;
    }
    //
    CharStateUpdateNode* const node = &it->second;
    ASSERT(node->query_ == &query_result &&
           node->record_changed_ == node->eRecordChange_NeedDelete);

    // NOTE: it's possible that a record is inserted in deletion transaction
    if (node->latest_data_.state_code_ == 0)
    {
        char_state_table_.erase(it);
    }
    else
    {
        // inserted in deletion transaction
        node->record_changed_ = node->eRecordChange_NewInsert;;
        node->query_ = NULL; // external delete control
    }
    //
    return true;
}

bool DBQueryProxyNode::CharacterNode::DBResult(const Query_Char_State_Update& query_result)
{
    const Query_Char_State_Update::sQueryParam& parameters = query_result.parameters_[0];
    CharStateTable::iterator it = char_state_table_.find(parameters.state_code_);
    if (!FlowControl::FCAssert(it != char_state_table_.end())) {
        return false;
    }
    //
    CharStateUpdateNode* const node = &it->second;
    ASSERT(node->query_ == &query_result &&
           node->record_changed_ == node->eRecordChange_Changed);
    node->record_changed_ = node->eRecordChange_Default;
    node->query_ = NULL; // external delete control
    node->commited_data_ = parameters;
    //
    return true;
}

bool DBQueryProxyNode::CharacterNode::UpdateCharStates()
{
    if ((shared_.state & this->eState_Loaded_CharStateInfo) == 0) {
        return true;
    }
    DBUser* const db_user = db_character_->GetParentDBUser();
    if (db_user == NULL)
    {
        return true;    // °­Á¦..transaction success..
    }
    // CHANGES: f110628.1L, the load balance updating implemented. updater='db_user->DBQuery'
    const int number_of_pending_queries = \
        g_DBHandler.GetNumberOfDBConnectionQueuingQueries(db_user);
    if (number_of_pending_queries > g_DBHandler.kMaxNumberOfSpecializedConcurrentBusyUpdateQueries) {
        return false; // pendings
    };
    const USERGUID user_guid = db_user->GetUserGuid();
    const CHARGUID char_guid = db_character_->GetCharGuid();
    //
    bool changed_events = false;
    STRING_SAFER_QUERY64 query_buffer;
    FOREACH_CONTAINER(CharStateTable::value_type& update_node, char_state_table_, CharStateTable)
    {
        CharStateUpdateNode* const node = &update_node.second;
        if (node->query_ != NULL) {
            changed_events = true;
            continue; //in transaction
        };
        //
        node->UpdateAndCompareData();
        if (node->record_changed_ == node->eRecordChange_Default) {
            continue;
        };
        //
        const uint16_t char_state = update_node.first;
        switch (node->record_changed_)
        {
        case node->eRecordChange_Changed: // update
            {
                query_buffer.MakeString(_T("{?=call S_Char_State_Update (?,?,?,?,?,?)}"));
                Query_Char_State_Update* const query = Query_Char_State_Update::ALLOC();
                query->SetQuery(query_buffer);
                query->SetUserKey(user_guid);
                query->set_char_guid(char_guid);
                // setup parameters
                Query_Char_State_Update::sQueryParam* parameters = &query->parameters_[0];
                static_cast<CharStateResultRecord&>(*parameters) = node->latest_data_;
                parameters->char_guid_ = char_guid;
                parameters->state_code_ = char_state;
                //
                node->query_ = query;
            };
            break;
        case node->eRecordChange_NewInsert: // insert
            {
                query_buffer.MakeString(_T("{?=call S_Char_State_Insert (?,?,?,?,?,?)}"));
                Query_Char_State_Insert* const query = Query_Char_State_Insert::ALLOC();
                query->SetQuery(query_buffer);
                query->SetUserKey(user_guid);
                query->set_char_guid(char_guid);
                // setup parameters
                Query_Char_State_Insert::sQueryParam* parameters = &query->parameters_[0];
                static_cast<CharStateResultRecord&>(*parameters) = node->latest_data_;
                parameters->char_guid_ = char_guid;
                parameters->state_code_ = char_state;
                //
                node->query_ = query;
            };
            break;
        case node->eRecordChange_NeedDelete: // delete
            {
                query_buffer.MakeString(_T("{?=call S_Char_State_Delete (?,?)}"));
                Query_Char_State_Delete* const query = Query_Char_State_Delete::ALLOC();
                query->SetQuery(query_buffer);
                query->SetUserKey(user_guid);
                query->set_char_guid(char_guid);
                // setup parameters
                Query_Char_State_Delete::sQueryParam* parameters = &query->parameters_[0];
                static_cast<CharStateResultRecord&>(*parameters) = node->latest_data_;
                // (WARNING) a value of node->latest_data_.perk_id_.group_ is zero.
                parameters->char_guid_ = char_guid;
                parameters->state_code_ = char_state;
                //
                node->query_ = query;
            };
            break;
        }; //end switch
        //
        if (changed_events == false && node->query_) {
            changed_events = true;
        }
        if (node->query_)
        {
            db_user->DBQuery(DG_CHARINFO, DG_CHARINFO_CHARLISTREQ_INTER_CHAR_STATE_DBR,
                             node->query_);
            //perk_node->BeginTransaction(kProxyCommand.eCmd_Char_State_Load);
        }
        //
    }
    //
    if (changed_events) {
        return false;
    }
    return true;
}

//==================================================================================================

}; //end of namespace

