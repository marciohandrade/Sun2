#include "stdafx.h"
#include "DBQueryProxyNode.h"

#include <VersionControl.h>

//==================================================================================================
#pragma warning(push, 4)
namespace nsQuery {
;

//==================================================================================================

void QuestUpdateNode::UpdateAndCompareData()
{
    if (record_changed_ != this->eRecordChange_Default) {
        return;
    };

    if (commited_data_.quest_version_ != latest_data_.quest_version_)
    {
        record_changed_ = this->eRecordChange_Changed;
        return;
    };
    if (::memcmp(commited_data_.quest_stream_progress_, latest_data_.quest_stream_progress_,
                 sizeof(commited_data_.quest_stream_progress_)) != 0)
    {
        record_changed_ = this->eRecordChange_Changed;
        return;
    };
    if (::memcmp(commited_data_.quest_stream_complete_, latest_data_.quest_stream_complete_,
                 sizeof(commited_data_.quest_stream_complete_)) != 0)
    {
        record_changed_ = this->eRecordChange_Changed;
        return;
    };

    ; // no changed
    return;
}

//==================================================================================================

// G->D update stream
void DBQueryProxyNode::CharacterNode::StoreQuest(const MSG_DG_CHARINFO_QUEST_CMD& quest_block)
{
    //QuestUpdateNode* const update_node = &quest_table_;
    QuestResultRecord* const record = &quest_table_.latest_data_;
    BOOST_STATIC_ASSERT(sizeof(quest_block.m_pProgrQStream) ==
                        sizeof(record->quest_stream_progress_) &&
                        sizeof(quest_block.m_pProgrQStream) == MAX_PROGR_QUESTSTREAM_SIZE);
    BOOST_STATIC_ASSERT(sizeof(quest_block.m_pComplQStream) ==
                        sizeof(record->quest_stream_complete_) &&
                        sizeof(quest_block.m_pComplQStream) == MAX_COMPL_QUESTSTREAM_SIZE);
    //ZeroMemory(&quest_table_.latest_data_, sizeof(quest_table_.latest_data_));
    
    record->quest_version_ = QUESTPART_PROGR::QUESTPART_PROGR_VERSION;
    CopyMemory(record->quest_stream_progress_, quest_block.m_pProgrQStream,
               sizeof(record->quest_stream_progress_));
    CopyMemory(record->quest_stream_complete_, quest_block.m_pComplQStream,
               sizeof(record->quest_stream_complete_));
    quest_table_.UpdateAndCompareData();
}

// D->G send stream
void DBQueryProxyNode::CharacterNode::LoadQuest(MSG_DG_CHARINFO_QUEST_CMD* quest_block) const
{
    const QuestResultRecord* const record = &quest_table_.latest_data_;
    CopyMemory(quest_block->m_pProgrQStream, record->quest_stream_progress_,
               sizeof(quest_block->m_pProgrQStream));
    CopyMemory(quest_block->m_pComplQStream, record->quest_stream_complete_,
               sizeof(quest_block->m_pComplQStream));
}

//==================================================================================================

bool DBQueryProxyNode::CharacterNode::DBResult(const Query_Quest_Select& query_result)
{
    // 1st load
    const int number_of_rows = query_result.GetResultRowNum();
    //const Query_Quest_Select::sQueryParam& parameters = query_result.parameters_[0];
    if (number_of_rows != 0)
    {   // setup default information
        const Query_Quest_Select::sQueryResult& record = query_result.result_records_[0];
        QuestUpdateNode* const update_node = &quest_table_;
        QuestResultRecord* const latest_data = &update_node->latest_data_;
        ZeroMemory(latest_data, sizeof(*latest_data));
        FlowControl::FCAssert(\
            VersionControl::QuestConverter(latest_data->quest_version_,
                                           latest_data->quest_stream_progress_,
                                           latest_data->quest_stream_complete_,
                                           record.quest_version_,
                                           record.quest_stream_progress_,
                                           record.quest_stream_complete_));
        update_node->commited_data_ = *latest_data;
    }
    else
    {   // setup default information
        QuestUpdateNode* const update_node = &quest_table_;
        ZeroMemory(&update_node->latest_data_, sizeof(update_node->latest_data_));
        update_node->latest_data_.quest_version_ = QUESTPART_PROGR::QUESTPART_PROGR_VERSION;
        update_node->commited_data_ = update_node->latest_data_;
    }
    //
    return true;
}

bool DBQueryProxyNode::CharacterNode::DBResult(const Query_Quest_Update& query_result)
{
    const Query_Quest_Update::sQueryParam& parameters = query_result.parameters_[0];
    QuestUpdateNode* const update_node = &quest_table_;
    ASSERT(update_node->query_ == &query_result &&
           update_node->record_changed_ == update_node->eRecordChange_Changed);
    update_node->record_changed_ = update_node->eRecordChange_Default;
    update_node->query_ = NULL; // external delete control
    update_node->commited_data_ = parameters;
    //
    return true;

}

bool DBQueryProxyNode::CharacterNode::UpdateQuest()
{
    const bool loaded_quest = (shared_.state & this->eState_Loaded_QuestInfo) != 0;
    if (loaded_quest == false) {
        return true;
    };
    //
    DBUser* const db_user = db_character_->GetParentDBUser();
    if (db_user == NULL)
    {
        return true;    // °­Á¦..transaction success..
    }
    const USERGUID user_guid = db_user->GetUserGuid();
    const CHARGUID char_guid = db_character_->GetCharGuid();
    //
    QuestUpdateNode* update_node = &quest_table_;
    if (update_node->query_ != NULL) {
        return false; //in transaction
    }
    update_node->UpdateAndCompareData();
    if (update_node->record_changed_ == update_node->eRecordChange_Default) {
        return true;
    };
    //
    STRING_SAFER_QUERY64 query_buffer;
    bool changed_events = false;
    //
    switch (update_node->record_changed_)
    {
    case update_node->eRecordChange_Changed: // update
        {
            query_buffer.MakeString(_T("{?=call S_Quest_Update (?,?,?,?)}"));
            Query_Quest_Update* const query = Query_Quest_Update::ALLOC();
            query->SetQuery(query_buffer);
            query->SetUserKey(user_guid);
            query->set_char_guid(char_guid);
            // setup parameters
            Query_Quest_Update::sQueryParam* parameters = &query->parameters_[0];
            static_cast<QuestResultRecord&>(*parameters) = update_node->latest_data_;
            parameters->char_guid_ = char_guid;
            //
            update_node->query_ = query;
        };
        break;
    }
    if (changed_events == false && update_node->query_) {
        changed_events = true;
    }
    if (update_node->query_)
    {
        db_user->DBQuery(DG_CHARINFO, DG_CHARINFO_UPDATE_INTER_QUEST_DBR,
                         update_node->query_);
    }
    if (changed_events) {
        return false;
    }

    return true;
}
//==================================================================================================

}; //end of namespace


