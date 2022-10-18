#include "stdafx.h"
#include "DBQueryProxyNode.h"

#include <VersionControl.h>

//==================================================================================================
#pragma warning(push, 4)
namespace nsQuery {
;

//==================================================================================================
//#define BATTLE_RECORD_NODE(val1, val2)
#define BATTLE_RECORD_NODE_LIST(record, player_info) \
    BATTLE_RECORD_NODE((record).pvp_point_, (player_info)->m_iPVPPoint) \
    BATTLE_RECORD_NODE((record).pvp_score_, (player_info)->m_iPVPScore) \
    BATTLE_RECORD_NODE((record).pvp_grade_, (player_info)->m_byPVPGrade) \
    BATTLE_RECORD_NODE((record).pvp_max_series_, (player_info)->m_iPVPMaxSeries) \
    BATTLE_RECORD_NODE((record).pvp_max_kills_, (player_info)->m_iPVPMaxKill) \
    BATTLE_RECORD_NODE((record).pvp_max_die_, (player_info)->m_iPVPMaxDie) \
    BATTLE_RECORD_NODE((record).pvp_number_of_kills_, (player_info)->m_iPVPTotalKill) \
    BATTLE_RECORD_NODE((record).pvp_number_of_dead_, (player_info)->m_iPVPTotalDie) \
    BATTLE_RECORD_NODE((record).pvp_number_of_draws_, (player_info)->m_iPVPTotalDraw) \
    BATTLE_RECORD_NODE((record).hunting_number_of_fails_, (player_info)->m_iCHuntingDisCount) \
    BATTLE_RECORD_NODE((record).hunting_number_of_victories_, (player_info)->m_iCHuntingVCount) \
    BATTLE_RECORD_NODE((record).hunting_number_of_defeats_, (player_info)->m_iCHuntingLCount) \
    BATTLE_RECORD_NODE((record).hunting_number_of_joins_, (player_info)->m_iCHuntingTotCount)

void BattleRecordsUpdateNode::UpdateAndCompareData(const DBCharacter* db_character)
{
    if (record_changed_ != this->eRecordChange_Default) {
        return;
    };

    const BASE_PLAYERINFO* player_info = db_character->GetInfo();
#undef BATTLE_RECORD_NODE
#define BATTLE_RECORD_NODE(val1, val2) \
    ((val1) != (val2)) ||
    //
    bool changed = BATTLE_RECORD_NODE_LIST(commited_data_, player_info) false;

    //bool changed = \
    //    (commited_data_.pvp_point_ != player_info->m_iPVPPoint) ||
    //    (commited_data_.pvp_score_ != player_info->m_iPVPScore) ||
    //    (commited_data_.pvp_grade_ != player_info->m_byPVPGrade) ||
    //    (commited_data_.pvp_max_series_ != player_info->m_iPVPMaxSeries) ||
    //    (commited_data_.pvp_max_kills_ != player_info->m_iPVPMaxKill) ||
    //    (commited_data_.pvp_max_die_ != player_info->m_iPVPMaxDie) ||
    //    (commited_data_.pvp_number_of_kills_ != player_info->m_iPVPTotalKill) ||
    //    (commited_data_.pvp_number_of_dead_ != player_info->m_iPVPTotalDie) ||
    //    (commited_data_.pvp_number_of_draws_ != player_info->m_iPVPTotalDraw) ||
    //    (commited_data_.hunting_number_of_fails_ != player_info->m_iCHuntingDisCount) ||
    //    (commited_data_.hunting_number_of_victories_ != player_info->m_iCHuntingVCount) ||
    //    (commited_data_.hunting_number_of_defeats_ != player_info->m_iCHuntingLCount) ||
    //    (commited_data_.hunting_number_of_joins_ != player_info->m_iCHuntingTotCount);
    ;
    if (changed) {
        record_changed_ = this->eRecordChange_Changed;
        return;
    }
    ; // no changed
    return;
}

//==================================================================================================

// G->D update stream
void DBQueryProxyNode::CharacterNode::StoreBattleRecords()
{
    battle_records_table_.UpdateAndCompareData(db_character_);
}

// D->G send stream
void DBQueryProxyNode::CharacterNode::LoadBattleRecords() const
{
    ; // nothing to do
}

//==================================================================================================

bool DBQueryProxyNode::CharacterNode::DBResult(const Query_BattleRecords_Select& query_result)
{
    // 1st load
    const int number_of_rows = query_result.GetResultRowNum();
    BASE_PLAYERINFO* player_info = db_character_->GetInfo();
    //const Query_BattleRecords_Select::sQueryParam& parameters = query_result.parameters_[0];
    if (number_of_rows != 0)
    {   // setup default information
        const Query_BattleRecords_Select::sQueryResult& record = query_result.result_records_[0];
        BattleRecordsUpdateNode* const update_node = &battle_records_table_;
        BattleRecordsResultRecord& commited_data = update_node->commited_data_;
        commited_data = record;
        //
#pragma warning(push)
#pragma warning(disable: 4244) // added to solve ODBC type and C-type convertion tolerance
#undef BATTLE_RECORD_NODE
#define BATTLE_RECORD_NODE(val1, val2) \
    ((val2) = (val1));
        // update to player_info
        BATTLE_RECORD_NODE_LIST(commited_data, player_info);
#pragma warning(pop)
    }
    else
    {   // setup default information
        BattleRecordsUpdateNode* const update_node = &battle_records_table_;
        ZeroMemory(&update_node->commited_data_, sizeof(update_node->commited_data_));
#undef BATTLE_RECORD_NODE
#define BATTLE_RECORD_NODE(val1, val2) \
    ((val2) = (0));

        BATTLE_RECORD_NODE_LIST(commited_data, player_info);
    }
    //
    return true;
}

bool DBQueryProxyNode::CharacterNode::DBResult(const Query_BattleRecords_Update& query_result)
{
    const Query_BattleRecords_Update::sQueryParam& parameters = query_result.parameters_[0];
    BattleRecordsUpdateNode* const update_node = &battle_records_table_;
    ASSERT(update_node->query_ == &query_result &&
           update_node->record_changed_ == update_node->eRecordChange_Changed);
    update_node->record_changed_ = update_node->eRecordChange_Default;
    update_node->query_ = NULL; // external delete control
    update_node->commited_data_ = parameters;
    //
    return true;

}

bool DBQueryProxyNode::CharacterNode::UpdateBattleRecords()
{
    const bool loaded_battle_records = (shared_.state & this->eState_Loaded_BattleRecords) != 0;
    if (loaded_battle_records == false) {
        return true;
    };
    
    DBUser* const db_user = db_character_->GetParentDBUser();
    if (db_user == NULL)
    {
        return true;    // °­Á¦..transaction success..
    }
    const USERGUID user_guid = db_user->GetUserGuid();
    const CHARGUID char_guid = db_character_->GetCharGuid();
    //
    BattleRecordsUpdateNode* update_node = &battle_records_table_;
    if (update_node->query_ != NULL) {
        return true; //in transaction
    }
    //
    update_node->UpdateAndCompareData(db_character_);
    if (update_node->record_changed_ == update_node->eRecordChange_Default) {
        return true;
    };
    
    STRING_SAFER_QUERY64 query_buffer;
    bool changed_events = false;
    //
    switch (update_node->record_changed_)
    {
    case update_node->eRecordChange_Changed: // update
        {
            query_buffer.MakeString(\
                _T("{?=call S_BattleRecords_Update (?,?,?,?,?,?,?,?,?,?,?,?,?,?)}"));
            Query_BattleRecords_Update* const query = Query_BattleRecords_Update::ALLOC();
            query->SetQuery(query_buffer);
            query->SetUserKey(user_guid);
            query->set_char_guid(char_guid);
            // setup parameters
            Query_BattleRecords_Update::sQueryParam* parameters = &query->parameters_[0];
            BattleRecordsResultRecord& update_record = \
                static_cast<BattleRecordsResultRecord&>(*parameters);
            const BASE_PLAYERINFO* player_info = db_character_->GetInfo();

#undef BATTLE_RECORD_NODE
#define BATTLE_RECORD_NODE(val1, val2) \
    ((val1) = (val2));
            //
            BATTLE_RECORD_NODE_LIST(update_record, player_info);
            //
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
        db_user->DBQuery(DG_CHARINFO, DG_CHARINFO_UPDATE_INTER_BATTLERECORDS_DBR,
                         update_node->query_);
    }
    if (changed_events) {
        return false;
    }

    return true;
}

//==================================================================================================

}; //end of namespace


