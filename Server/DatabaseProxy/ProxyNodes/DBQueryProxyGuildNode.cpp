#include "stdafx.h"
#include "./DBQueryProxyGuildNode.h"


#include "Services/GuildManagerInDBP.h"
#include "QueryObjects/GuildServerQuery.h"

namespace nsQuery {
;

void GuildWarehouseUpdateNode::UpdateAndCompareData()
{
    if (record_changed_ != this->eRecordChange_Default) {
        return;
    };
    // NOTE: f110419.2L, prevent unexpected huge sum of money.
    // reference value: 0x8000000000000000ULL
    //               int64
    if (latest_data_.money_ < 0) {
        latest_data_.money_ = 0;
    };
    bool changed = //commited_data_.number_of_slot_tabs_ != latest_data_.number_of_slot_tabs_ ||
                   commited_data_.money_ != latest_data_.money_;
    //
    if (changed) {
        record_changed_ = this->eRecordChange_Changed;
        return;
    }
    ; // no changed

    return;
}

//==================================================================================================


DBQueryProxyGuildNode::DBQueryProxyGuildNode()
{
    lifetime_timeout_.Disabled();
    // NOTE: f101008.2L
    ZeroMemory(pod_field_start_, static_cast<size_t>(pod_field_end_ - pod_field_start_));
}

void DBQueryProxyGuildNode::Init(GUILDGUID guild_guid)
{
    DBQueryProxyNode::Init();
    // NOTE: f101008.2L
    ZeroMemory(pod_field_start_, static_cast<size_t>(pod_field_end_ - pod_field_start_));
    //
    guild_guid_ = guild_guid;
    lifetime_timeout_.Disabled();
}

// call by DBQueryProxyManager
void DBQueryProxyGuildNode::Release()
{
    // db user delete processing...
}

bool DBQueryProxyGuildNode::BeginTransaction(eTransaction transaction)
{
    const bool is_checked = (shared_.transaction & transaction) != 0;
    __TOUCHED(is_checked); // for debug

    // r100928.3L
    switch (transaction)
    {
    case this->eTrans_Select_Warehouse:
    case this->eTrans_Select_WareItems:
        break;
    default:
        ASSERT(!"unexpected transaction value");
        return false;
    }
    //
    shared_.transaction |= transaction;
    //
    return true;
}

bool DBQueryProxyGuildNode::EndTransaction(eTransaction transaction, bool success)
{
    if (!FlowControl::FCAssert(shared_.transaction & transaction)) {
        return false;
    }
    // r100928.4L
    switch (transaction)
    {
    case this->eTrans_Select_Warehouse:
        if (success) {
            shared_.state |= this->eState_Loaded_Warehouse;
        }
        else {
            shared_.state &= ~this->eState_Loaded_Warehouse;
        }
        break;
        //
    case this->eTrans_Select_WareItems :
        if (success) {
            shared_.state |= this->eState_Loaded_WareItems;
        }
        else {
            shared_.state &= ~this->eState_Loaded_WareItems;
        }
        break;
        //
    default:
        ASSERT(!"unexpected transaction value");
        return false;
    }
    // processed
    shared_.transaction &= ~transaction;

    return true;
}

bool DBQueryProxyGuildNode::Update(ChildServerSession* server_session,
                                   USERGUID user_guid, CHARGUID char_guid)
{
    bool changed_events = false;
    bool cleared = false;
    // NOTE: f101118.1L, update routines related to guild of this has only snapshop update
    UpdateArgs update_args = { 0, };
    update_args.server_session = server_session;
    update_args.user_guid = user_guid;
    update_args.char_guid = char_guid;
    assert(server_session != NULL);
    if (user_guid == 0 || char_guid == 0) {
        update_args.user_guid = locker_user_guid_;
        update_args.char_guid = locker_char_guid_;
    }
    //
    cleared = UpdateWarehouse(update_args);
    if (cleared == false) {
        changed_events = true;
    }
    cleared = UpdateGuildLevelItemSlots(update_args);
    if (cleared == false) {
        changed_events = true;
    }
    //
    update_check_ = changed_events;
    if (changed_events) {
        return false;
    }
    return true;
}

bool DBQueryProxyGuildNode::UpdateWarehouse(const UpdateArgs& update_args)
{
    const bool loaded_warehouse = (shared_.state & this->eState_Loaded_Warehouse) != 0;
    if (loaded_warehouse == false) {
        return true;
    }
    //
    bool changed_events = false;

    if (warehouse_update_node_.query_ != NULL) {
        return false; //in transaction
    }
    warehouse_update_node_.UpdateAndCompareData();
    if (warehouse_update_node_.record_changed_ == warehouse_update_node_.eRecordChange_Default) {
        return true;
    }
    //
    switch (warehouse_update_node_.record_changed_)
    {
    case warehouse_update_node_.eRecordChange_Changed: // update
        {
            STRING_SAFER_QUERY64 query_buffer;
            query_buffer.MakeString(\
                _T("{?=call S_GuildWarehouse_Update (?,?)}"));
            GuildWarehouseUpdateQuery* const query = GuildWarehouseUpdateQuery::ALLOC();
            query->SetQuery(query_buffer);
            query->SetUserKey(update_args.user_guid);
            query->set_char_guid(update_args.char_guid);
            // setup parameters
            if (GuildWarehouseUpdateQuery::sQueryParam* parameters = &query->parameters_[0])
            {
                static_cast<GuildWarehouseResultRecord&>(*parameters) = \
                    warehouse_update_node_.latest_data_;
                parameters->guild_guid_ = guild_guid_;
            };
            //
            warehouse_update_node_.query_ = query;
        };
        break;
    }
    if (changed_events == false && warehouse_update_node_.query_) {
        changed_events = true;
    }
    if (warehouse_update_node_.query_)
    {
        update_args.server_session->DBQuery(DG_WAREHOUSE, DG_GUILD_WAREHOUSE_UPDATE_INNER_DBR,
                                            warehouse_update_node_.query_);
    }
    //----------------------
    if (changed_events) {
        return false;
    }

    return true;
}

bool DBQueryProxyGuildNode::UpdateGuildLevelItemSlots(const UpdateArgs& update_args)
{
    const bool loaded_warehouse = (shared_.state & this->eState_Loaded_Warehouse) != 0;
    const bool loaded_wareitems = (shared_.state & this->eState_Loaded_WareItems) != 0;
    if (loaded_warehouse == false || loaded_wareitems == false) {
        return true;
    }
    //
    bool changed_events = false;
    STRING_SAFER_QUERY128 query_buffer;
    ItemSlotUpdateNode* update_nodes[1] = {
        loaded_warehouse ? warehouse_table_.records_ : NULL,
    };
    ItemSlotUpdateNode* update_nodes_end[_countof(update_nodes)] = {
        loaded_warehouse ? &warehouse_table_.records_[_countof(warehouse_table_.records_)] : NULL,
    };
    //
    for (int update_node_index = 0; update_node_index < _countof(update_nodes); ++update_node_index)
    {
        ItemSlotUpdateNode* update_node_it = update_nodes[update_node_index],
                          * const update_node_end = update_nodes_end[update_node_index];
        if (update_node_it == NULL) {
            continue; //not loaded
        };
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
                    query_buffer.MakeString(\
                        _T("{?=call S_GuildWarehouseItemSlotInfo_Update (?,?,?,?,?,?,?,?)}"));
                    Query_ItemSlotInfo_Update* const query = Query_ItemSlotInfo_Update::ALLOC();
                    query->SetQuery(query_buffer);
                    query->SetUserKey(guild_guid_);
                    // setup parameters
                    Query_ItemSlotInfo_Update::sQueryParam* parameters = &query->parameters_[0];
                    static_cast<ItemSlotResultRecord&>(*parameters) = update_node_it->latest_data_;
                    parameters->guid_ = guild_guid_;
                    parameters->slot_type_ = warehouse_table_.kSlotIndex;
                    //
                    update_node_it->query_ = query;
                };
                break;
            }
            if (changed_events == false && update_node_it->query_) {
                changed_events = true;
            }
            if (update_node_it->query_)
            {
                update_args.server_session->DBQuery(\
                    DG_WAREHOUSE, DG_GUILD_WAREHOUSE_ITEMSLOTINFO_UPDATE_INTER_DBR,
                    update_node_it->query_);
            }
        };
    }
    if (changed_events) {
        return false;
    }

    return true;
}

// list = { guild_warehouse, }
// G->D update stream
bool DBQueryProxyGuildNode::StoreItemSlotInfo(SLOTIDX slot_type,
                                              const ITEMSLOTEX_INSERVER* data_list,
                                              const POSTYPE number_of_slots)
{
    ItemSlotVersionChecker version_check;
    bool accept_version = ItemSlotVersionChecker::IsAcceptableItemSlot(slot_type, &version_check);
    if (FlowControl::FCAssert(accept_version) == false) {
        return false;
    }
    ItemSlotUpdateNode* update_node_target = NULL;
    POSTYPE max_number_of_slots = 0;
    switch (slot_type)
    {
    case warehouse_table_.kSlotIndex:
        update_node_target = warehouse_table_.records_;
        max_number_of_slots = warehouse_table_.kMaxNumberOfSlots;
        warehouse_table_._OperateAll(warehouse_table_.eOper_ClearAllToDefault,
                                     warehouse_table_.records_,
                                     warehouse_table_.kMaxNumberOfSlots,
                                     warehouse_table_.kSlotIndex);
        break;
    default:
        __debugbreak();
        return false;
    };
    //
    DBQueryProxyItemSlotNode::StoreItemSlotInfo(slot_type, data_list, number_of_slots,
                                                update_node_target, max_number_of_slots);
    update_check_ = true;
    //
    return true;
}

// D->G send stream
bool DBQueryProxyGuildNode::LoadItemSlotInfo(SLOTIDX slot_type,
                                             ITEMSLOTEX_INSERVER* data_list,
                                             POSTYPE* result_number_of_slots) const
{
    ItemSlotVersionChecker version_check;
    bool accept_version = ItemSlotVersionChecker::IsAcceptableItemSlot(slot_type, &version_check);
    if (FlowControl::FCAssert(accept_version) == false) {
        return false;
    }
    const ItemSlotUpdateNode* update_node_target = NULL;
    POSTYPE max_number_of_slots = 0;
    switch (slot_type)
    {
    case warehouse_table_.kSlotIndex:
        update_node_target = warehouse_table_.records_;
        max_number_of_slots = warehouse_table_.kMaxNumberOfSlots;
        break;
    default:
        __debugbreak();
        return false;
    };
    //
    DBQueryProxyItemSlotNode::LoadItemSlotInfo(slot_type, data_list, result_number_of_slots,
                                               update_node_target, max_number_of_slots);
    //
    return true;
}

//
//----------------------------------------------------------------------------------------------

bool DBQueryProxyGuildNode::OnSetGuildWarehouseBaseInfo(
    const GuildWarehouseSelectQuery& query_result)
{
    const GuildWarehouseSelectQuery::sQueryResult* record = query_result.result_records_;
    warehouse_update_node_.latest_data_.money_ = record->money_;
    number_of_slot_tabs_ = static_cast<uint8_t>(record->number_of_slot_tabs_);

    return true;
}

bool DBQueryProxyGuildNode::DBResult(const GuildWarehouseUpdateQuery& query_result)
{
    const GuildWarehouseUpdateQuery::sQueryParam& parameters = query_result.parameters_[0];
    GuildWarehouseUpdateNode* const update_node = &warehouse_update_node_;
    ASSERT(update_node->query_ == &query_result &&
           update_node->record_changed_ == update_node->eRecordChange_Changed);
    update_node->record_changed_ = update_node->eRecordChange_Default;
    update_node->query_ = NULL; // external delete control
    //
    if (query_result.IsError()) {
        return false;
    }
    //
    update_node->commited_data_ = parameters;
    //
    return true;
}

bool DBQueryProxyGuildNode::DBResult(const Query_ItemSlotInfo_Select& query_result)
{
    // 1st load
    const Query_ItemSlotInfo_Select::sQueryParam& parameters = query_result.parameters_[0];
    const SLOTIDX slot_type = parameters.slot_type_;
    ItemSlotVersionChecker version_check;
    bool accept_version = ItemSlotVersionChecker::IsAcceptableItemSlot(slot_type, &version_check);
    if (FlowControl::FCAssert(accept_version) == false) {
        return false;
    }
    ItemSlotUpdateNode* update_node_target = NULL;
    POSTYPE max_number_of_slots = 0;
    switch (slot_type)
    {
    case warehouse_table_.kSlotIndex:
        update_node_target = warehouse_table_.records_;
        max_number_of_slots = warehouse_table_.kMaxNumberOfSlots;
        warehouse_table_._OperateAll(warehouse_table_.eOper_ClearAllToDefault |
                                     warehouse_table_.eOper_CopyAllLatestDataToCommitedData,
                                     warehouse_table_.records_,
                                     warehouse_table_.kMaxNumberOfSlots,
                                     warehouse_table_.kSlotIndex);
        break;
    default:
        __debugbreak();
        return false;
    };
    //
    DBQueryProxyItemSlotNode::DBResult_Select(query_result,
                                              update_node_target, max_number_of_slots);

    return true;
}

bool DBQueryProxyGuildNode::DBResult(const Query_ItemSlotInfo_Update& query_result)
{
    const Query_ItemSlotInfo_Update::sQueryParam& parameters = query_result.parameters_[0];
    const SLOTIDX slot_type = parameters.slot_type_;
    ItemSlotVersionChecker version_check;
    bool accept_version = ItemSlotVersionChecker::IsAcceptableItemSlot(slot_type, &version_check);
    if (FlowControl::FCAssert(accept_version) == false) {
        return false;
    }
    ItemSlotUpdateNode* update_node_target = NULL;
    POSTYPE max_number_of_slots = 0;
    switch (slot_type)
    {
    case warehouse_table_.kSlotIndex:
        update_node_target = warehouse_table_.records_;
        max_number_of_slots = warehouse_table_.kMaxNumberOfSlots;
        break;
    default:
        __debugbreak();
        return false;
    };
    if (FlowControl::FCAssert(parameters.pos_ <= max_number_of_slots) == false) {
        return false;
    }

    DBQueryProxyItemSlotNode::DBResult_Update(query_result,
                                              update_node_target, max_number_of_slots);

    return true;
}

}; //end of namespace

