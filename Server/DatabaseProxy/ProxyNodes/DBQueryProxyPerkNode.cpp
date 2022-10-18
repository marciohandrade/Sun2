#include "stdafx.h"
#include "DBQueryProxyNode.h"
#include <Protocol_DG.h>

//
#include <SkillSystem/PerkInfoParser.h>

#include "QueryObjects/DBHandler.h"

namespace nsQuery {
;

void PerkUpdateNode::ChangeData(const PerkData& inputed_data)
{
    // PerkResultRecord <-- PerkData
    latest_data_.group_code_ = inputed_data.perk_id_.group_;
    latest_data_.level_ = inputed_data.perk_id_.level_;
    latest_data_.link_code_ = inputed_data.link_code_;
    latest_data_.exp_ = inputed_data.exp_;
}

void PerkUpdateNode::UpdateAndCompareData()
{
    if (record_changed_ != this->eRecordChange_Default) {
        return;
    };
    //
    const bool key_changed = (commited_data_.group_code_ != latest_data_.group_code_);
    if (key_changed) {
        if (latest_data_.group_code_ == 0) {
            record_changed_ = this->eRecordChange_NeedDelete;
            return;
        }
        commited_data_.group_code_ = latest_data_.group_code_;
    }
    const bool changed = (commited_data_.level_ != latest_data_.level_ ||
                          commited_data_.link_code_ != latest_data_.link_code_ ||
                          commited_data_.exp_ != latest_data_.exp_);
    if (key_changed) {
        record_changed_ = this->eRecordChange_NewInsert;
        return;
    }
    if (changed) {
        record_changed_ = this->eRecordChange_Changed;
    }
    // no changed
}

bool DBQueryProxyNode::CharacterNode::SetupDefaultPerkInfo()
{
    const PerkUpdateTable& default_table = _GetDefaultPerkInfoTable();
    assert(perk_table_.empty());
    perk_table_ = default_table;
    return true;
}

//static
const DBQueryProxyNode::CharacterNode::PerkUpdateTable&
DBQueryProxyNode::CharacterNode::_GetDefaultPerkInfoTable()
{
    static PerkUpdateTable* default_perk_table_static_ = NULL;
    static struct AutoClosure {
        AutoClosure(PerkUpdateTable** static_table) : static_table_(static_table) {
        }
        ~AutoClosure() {
            SAFE_DELETE(*static_table_);
        }
        PerkUpdateTable** static_table_;
    } auto_closure(&default_perk_table_static_);
    //
    if (default_perk_table_static_ != NULL) {
        return *default_perk_table_static_;
    }
    // setup default perk information
    default_perk_table_static_ = new PerkUpdateTable;
    PerkUpdateTable& default_table = *default_perk_table_static_; // alias
    //
    PerkInfoParser* const perk_parser = PerkInfoParser::Instance();
    const PerkInfoParser::GroupTable& perk_group_table = perk_parser->GetPerkGroupTable();
    //
    PerkData perk_data;
    FOREACH_CONTAINER(const PerkInfoParser::GroupTable::value_type& group_node, perk_group_table,
                      PerkInfoParser::GroupTable)
    {
        const PerkGroup* perk_group = &group_node.second;
        if (perk_group->item_code_) {
            continue;
        }
        PerkUpdateNode* const node = &default_table[perk_group->group_code_];
        ZeroMemory(node, sizeof(*node));
        const PerkInfo* base_perk_info = perk_group->perk_info_array_[0];
        ZeroMemory(&perk_data, sizeof(perk_data));
        perk_data.perk_id_ = base_perk_info->perk_id_;
        ;//perk_data.link_code_ = 0;
        ;//perk_data.exp_ = 0;
        node->ChangeData(perk_data);
    }
    ;{
        const PerkDataList::eGroupCodeActiveSlot* it = PerkDataList::kGroupCodeActiveSlots;
        for (int i = 0; i < _countof(PerkDataList::kGroupCodeActiveSlots); ++i, ++it)
        {
            SLOTCODE group_code_for_active_link = static_cast<SLOTCODE>(*it);
            PerkUpdateNode* const node = &default_table[group_code_for_active_link];
            ZeroMemory(node, sizeof(*node));
            ZeroMemory(&perk_data, sizeof(perk_data));
            perk_data.perk_id_.group_ = group_code_for_active_link;
            ;//perk_data.perk_id_.level_ = 0;
            ;//perk_data.link_code_ = 0;
            ;//perk_data.exp_ = 0;
            node->ChangeData(perk_data);
        }
    };
    return *default_perk_table_static_;
}

// G->D update stream
void DBQueryProxyNode::CharacterNode::StorePerk(const PerkDataList& perk_data_list)
{
    PerkInfoParser* const perk_parser = PerkInfoParser::Instance();
    const PerkData* src_it = perk_data_list.perk_data_;
    for (int i = 0; i < perk_data_list.number_of_nodes_; ++i, ++src_it)
    {
        const PerkID& perk_id = src_it->perk_id_;
        const bool is_active_slot = (perk_id.group_ >= PerkDataList::eActiveGroup01);
        const PerkGroup* perk_group = is_active_slot == false ? perk_parser->FindPerkGroup(perk_id)
                                                              : NULL;
        if (is_active_slot == false &&
            FlowControl::FCAssert(perk_group != NULL) == false)
        {
            continue;
        }
        PerkUpdateTable::iterator it = perk_table_.find(perk_id.group_);
        PerkUpdateNode* node = NULL;
        if (bool is_found = (it != perk_table_.end())) {
            node = &it->second;
        }
        else
        {   // NOTE: an automatic new insertion operation
            //SUNLOG(eCRITICAL_LOG, _T("|[%s]|Msg=can't find a perk group(%d)|"),
            //       __FUNCTION__, perk_id.group_);
            node = &perk_table_[perk_id.group_];
            ZeroMemory(node, sizeof(*node));
            node->ChangeData(*src_it);
        };
        //
        node->ChangeData(*src_it);
    }
}

// D->G send stream
void DBQueryProxyNode::CharacterNode::LoadPerk(PerkDataList* perk_data_list) const
{
    ZeroMemory(perk_data_list, sizeof(*perk_data_list));
    //
    PerkData* dest_it = perk_data_list->perk_data_,
            * const dest_end = &perk_data_list->perk_data_[_countof(perk_data_list->perk_data_)];
    //
    const PerkUpdateTable::size_type number_of_records = perk_table_.size();
    if (FlowControl::FCAssert(number_of_records <=
                              _countof(perk_data_list->perk_data_)) == false)
    {
        return;
    }

    FOREACH_CONTAINER(const PerkUpdateTable::value_type& node, perk_table_, PerkUpdateTable)
    {
        if (dest_it == dest_end) {
            break;
        }
        const PerkUpdateNode& update_node = node.second;
        const PerkResultRecord& latest_info = update_node.latest_data_;
        if (latest_info.group_code_ == 0) {
            continue; // delete reserved
        }
        // PerkData <-- PerkResultRecord
        dest_it->perk_id_.group_ = static_cast<SLOTCODE>(latest_info.group_code_);
        dest_it->perk_id_.level_ = static_cast<LEVELTYPE>(latest_info.level_);
        dest_it->link_code_ = static_cast<uint8_t>(latest_info.link_code_);
        dest_it->exp_ = latest_info.exp_;
        //
        ++dest_it;
    }
    //
    perk_data_list->number_of_nodes_ = \
        static_cast<int>(dest_it - perk_data_list->perk_data_);
}

bool DBQueryProxyNode::CharacterNode::ProcessPerkOperation(const PerkControl& req_info,
                                                           PerkControl* header, PerkControlData* data)
{
    *header = req_info;
    data->Clear(); // in game server operation
    //
    switch (req_info.request)
    {
    case req_info.eReq_InsertS2S:
#if SUN_CODE_BACKUP
        {   // NOTE: change to an auto insertion mechanism
            const PerkID perk_id(req_info.args.insert_s2s.perk_uid);
            const PerkInfo* perk_info = PerkInfoParser::Instance()->FindPerkInfo(perk_id);
            if (FlowControl::FCAssert(perk_info != NULL) == false) {
                break;
            }
            PerkUpdateTable::iterator it = perk_table_.find(perk_id.group_);
            const bool new_insert = (it == perk_table_.end());
            if (new_insert == false) {
                header->result = header->eRet_Failed;
                // add operation result code related to a fail transaction
                header->result_code = header->eRetCode_Unknown;
                return true;
            }
            PerkUpdateNode* node = &perk_table_[perk_id.group_];
            ZeroMemory(node, sizeof(*node));
            //
            if (perk_info->item_code_ == 0) {
                header->result = header->eRet_Failed;
                // add operation result code related to a not cash type
                header->result_code = header->eRetCode_Unknown;
                return true;
            };
            PerkData perk_data = { 0, };
            perk_data.perk_id_ = perk_info->perk_id_;
            node->ChangeData(perk_data);
            return true;
        }
#endif //SUN_CODE_BACKUP
        break;
    case req_info.eReq_DeleteS2S:
        {
            const PerkID perk_id(req_info.args.delete_s2s.perk_uid);
            const PerkGroup* perk_group = PerkInfoParser::Instance()->FindPerkGroup(perk_id);
            if (FlowControl::FCAssert(perk_group != NULL) == false) {
                break;
            }
            PerkUpdateTable::iterator it = perk_table_.find(perk_id.group_);
            const bool cant_found = (it == perk_table_.end());
            if (cant_found) {
                header->result = header->eRet_Failed;
                // NOTE: it may be needed additional operation result code related to a fails
                header->result_code = header->eRetCode_Unknown;
                return true;
            }
            //
            if (perk_group->item_code_ == 0) {
                header->result = header->eRet_Failed;
                // NOTE: it may be needed additional operation result code related to a fails
                header->result_code = header->eRetCode_Unknown;
                return true;
            };
            // (NOTE) this operation is an event after calling a status update related to unregister
            PerkUpdateNode* const node = &it->second;
            node->latest_data_.group_code_ = 0;
            //node->record_changed_ = node->eRecordChange_NeedDelete;
            return true;
        }
        break;
    }
    // operator processing command returns fails.
    header->result = header->eRet_Failed;
    header->result_code = header->eRetCode_Unknown;
    return false;
}


bool DBQueryProxyNode::CharacterNode::DBResult(const Query_Perk_Select& query_result)
{
    // 1st load
    if (FlowControl::FCAssert(perk_table_.empty()) == false) {
        return false;
    }
    
    const int number_of_rows = query_result.GetResultRowNum();
    for (int i = 0; i < number_of_rows; ++i)
    {
        const Query_Perk_Select::sQueryResult& record = query_result.result_records_[i];
        PerkUpdateNode* const node = &perk_table_[record.group_code_];
        ZeroMemory(node, sizeof(*node));
        node->commited_data_ = record;
        node->latest_data_ = record;
    }
    // (WARNING) need modify data of the database if a default perk is inserted.
    if (number_of_rows == 0) {
        SetupDefaultPerkInfo();
    };
    return true;
}

bool DBQueryProxyNode::CharacterNode::DBResult(const Query_Perk_Insert& query_result)
{
    const Query_Perk_Insert::sQueryParam& parameters = query_result.parameters_[0];
    PerkUpdateTable::iterator it = perk_table_.find(parameters.group_code_);
    if (!FlowControl::FCAssert(it != perk_table_.end())) {
        return false;
    }
    //
    PerkUpdateNode* const node = &it->second;
    ASSERT(node->query_ == &query_result &&
           node->record_changed_ == node->eRecordChange_NewInsert);
    node->record_changed_ = node->eRecordChange_Default;
    node->query_ = NULL; // external delete control
    node->commited_data_ = parameters;
    //
    return true;
}

bool DBQueryProxyNode::CharacterNode::DBResult(const Query_Perk_Delete& query_result)
{
    const Query_Perk_Delete::sQueryParam& parameters = query_result.parameters_[0];
    PerkUpdateTable::iterator it = perk_table_.find(parameters.group_code_);
    if (!FlowControl::FCAssert(it != perk_table_.end())) {
        return false;
    }
    //
    PerkUpdateNode* const node = &it->second;
    ASSERT(node->query_ == &query_result &&
           node->record_changed_ == node->eRecordChange_NeedDelete);
    // NOTE: it's possible that a record is inserted in deletion transaction
    if (node->latest_data_.group_code_ == 0)
    {
        perk_table_.erase(it);
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

bool DBQueryProxyNode::CharacterNode::DBResult(const Query_Perk_Update& query_result)
{
    const Query_Perk_Update::sQueryParam& parameters = query_result.parameters_[0];
    PerkUpdateTable::iterator it = perk_table_.find(parameters.group_code_);
    if (!FlowControl::FCAssert(it != perk_table_.end())) {
        return false;
    }
    //
    PerkUpdateNode* const node = &it->second;
    ASSERT(node->query_ == &query_result &&
           node->record_changed_ == node->eRecordChange_Changed);
    node->record_changed_ = node->eRecordChange_Default;
    node->query_ = NULL; // external delete control
    node->commited_data_ = parameters;
    //
    return true;
}

bool DBQueryProxyNode::CharacterNode::UpdatePerk()
{
    DBUser* const db_user = db_character_->GetParentDBUser();
    if (db_user == NULL)
    {
        return true;    // °­Á¦..transaction success..
    }
    const USERGUID user_guid = db_user->GetUserGuid();
    const CHARGUID char_guid = db_character_->GetCharGuid();
    if ((shared_.state & this->eState_Loaded_PerkInfo) == 0) {
        return true;
    }
    // CHANGES: f110628.1L, the load balance updating implemented. updater='db_user->DBQuery'
    const int number_of_pending_queries = \
        g_DBHandler.GetNumberOfDBConnectionQueuingQueries(db_user);
    if (number_of_pending_queries > g_DBHandler.kMaxNumberOfSpecializedConcurrentBusyUpdateQueries) {
        return false; // pendings
    };
    //
    bool changed_events = false;
    STRING_SAFER_QUERY64 query_buffer;
    FOREACH_CONTAINER(PerkUpdateTable::value_type& update_node, perk_table_, PerkUpdateTable)
    {
        PerkUpdateNode* const node = &update_node.second;
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
        const ulong group_code = update_node.first;
        switch (node->record_changed_)
        {
        /*case node->eRecordChange_ExpChanged: // update exp event
            {
                query_buffer.MakeString(_T("{?=call S_Perk_Update_ExpEvent (?,?)}"));
                Query_Perk_Update_ExpEvent* const query = Query_Perk_Update_ExpEvent::ALLOC();
                query->SetQuery(query_buffer);
                query->SetUserKey(user_guid);
                query->set_char_guid(char_guid);
                // setup parameters
                Query_Perk_Update_ExpEvent::sQueryParam* parameters = &query->parameters_[0];
                static_cast<PerkResultRecord&>(*parameters) = node->latest_data_;
                parameters->char_guid_ = char_guid;
                //
                query_base = query;
            };
            break;*/
        case node->eRecordChange_Changed: // update
            {
                query_buffer.MakeString(_T("{?=call S_Perk_Update (?,?,?,?,?)}"));
                Query_Perk_Update* const query = Query_Perk_Update::ALLOC();
                query->SetQuery(query_buffer);
                query->SetUserKey(user_guid);
                query->set_char_guid(char_guid);
                // setup parameters
                Query_Perk_Update::sQueryParam* parameters = &query->parameters_[0];
                static_cast<PerkResultRecord&>(*parameters) = node->latest_data_;
                parameters->char_guid_ = char_guid;
                parameters->group_code_ = group_code;
                //
                node->query_ = query;
            };
            break;
        case node->eRecordChange_NewInsert: // insert
            {
                query_buffer.MakeString(_T("{?=call S_Perk_Insert (?,?,?,?,?)}"));
                Query_Perk_Insert* const query = Query_Perk_Insert::ALLOC();
                query->SetQuery(query_buffer);
                query->SetUserKey(user_guid);
                query->set_char_guid(char_guid);
                // setup parameters
                Query_Perk_Insert::sQueryParam* parameters = &query->parameters_[0];
                static_cast<PerkResultRecord&>(*parameters) = node->latest_data_;
                parameters->char_guid_ = char_guid;
                parameters->group_code_ = group_code;
                //
                node->query_ = query;
            };
            break;
        case node->eRecordChange_NeedDelete: // delete
            {
                query_buffer.MakeString(_T("{?=call S_Perk_Delete (?,?)}"));
                Query_Perk_Delete* const query = Query_Perk_Delete::ALLOC();
                query->SetQuery(query_buffer);
                query->SetUserKey(user_guid);
                query->set_char_guid(char_guid);
                // setup parameters
                Query_Perk_Delete::sQueryParam* parameters = &query->parameters_[0];
                static_cast<PerkResultRecord&>(*parameters) = node->latest_data_;
                // (WARNING) a value of node->latest_data_.perk_id_.group_ is zero.
                parameters->char_guid_ = char_guid;
                parameters->group_code_ = group_code;
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
            //ChildServerSession* const server_session = db_user->GetServerSession();
            db_user->DBQuery(DG_CHARINFO, DG_CHARINFO_CHARLISTREQ_INTER_PERK_DBR,
                             node->query_);
            //perk_node->BeginTransaction(kProxyCommand.eCmd_Perk_Load);
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

