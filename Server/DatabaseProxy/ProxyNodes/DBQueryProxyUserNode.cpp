#include "stdafx.h"
#include "DBQueryProxyNode.h"

//==================================================================================================
// implemented by _NA000023_090323_ENHANCED_INVENTORY_SLOT_FUNCTIONALITY_DB_
//==================================================================================================

namespace nsQuery {
;

DBQueryProxyNode::UserNode::UserNode()
{
    db_user_ = NULL;
    lifetime_timeout_.Disabled();
    // NOTE: f101008.2L
    ZeroMemory(pod_field_start_, static_cast<size_t>(pod_field_end_ - pod_field_start_));
}

//DBQueryProxyNode::UserNode::~UserNode()
//{
//}

void DBQueryProxyNode::UserNode::Init(DBUser* db_user)
{
    if (db_user_ != NULL && this->loaded() == false)
    {
        SUNLOG(eCRITICAL_LOG,
               _T("|["__FUNCTION__"]|Msg=release pending node, but has no data|")
               _T("|UserGuid=%d|State=0x%08X|Transaction=0x%08X|Request=0x%08X|"),
               db_user_->GetUserGuid(), shared_.state, shared_.transaction, shared_.request);
    }
    //
    DBQueryProxyNode::Init();
    db_user_ = db_user;
    lifetime_timeout_.Disabled();
    // NOTE: f101008.2L
    ZeroMemory(pod_field_start_, static_cast<size_t>(pod_field_end_ - pod_field_start_));
}

// call by DBQueryProxyManager
void DBQueryProxyNode::UserNode::RequestConnect(const MSG_AD_CHARINFO_CHARLISTREQ_SYN* recv_msg,
                                                ChildServerSession* server_session)
{
    shared_.request |= this->eReq_ConnectPending;
    request_info_.registered_tick = ::GetTickCount();
    request_info_.session = server_session;
    request_info_.message = *recv_msg;
}

void DBQueryProxyNode::UserNode::CancelRequest()
{
    shared_.request &= ~this->eReq_ConnectPending;
    ZeroMemory(&request_info_, sizeof(request_info_));
}


// call by DBQueryProxyManager
void DBQueryProxyNode::UserNode::Release()
{
    // db user delete processing...
}

bool DBQueryProxyNode::UserNode::BeginTransaction(eTransaction transaction)
{
    const bool is_checked = (shared_.transaction & transaction) != 0;
    __TOUCHED(is_checked); // for debug

    BOOST_STATIC_ASSERT(\
        (UserNode::eTrans_Update_Warehouse  == UserNode::eReq_Update_Warehouse));
    // r100928.3L
    switch (transaction)
    {
    case this->eTrans_Select_CharInfo :
    case this->eTrans_Select_InvenInfo:
    case this->eTrans_Select_PetList:
    case this->eTrans_Select_EquipInfo:
    case this->eTrans_Select_Warehouse:
    case this->eTrans_Select_WareItems:
        break;
    //
    //case this->eTrans_Update_CharInfo :
    //case this->eTrans_Update_InvenInfo:
    //case this->eTrans_Update_CharState:
    case this->eTrans_Update_Warehouse:
        // (f100603.5L) add a clear routine by a latest data application mechanism
        shared_.request &= ~transaction;
        break;
    //
    case this->eTrans_Create_Character:
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

bool DBQueryProxyNode::UserNode::EndTransaction(eTransaction transaction, bool success)
{
    if (!FlowControl::FCAssert(shared_.transaction & transaction)) {
        return false;
    }
    // r100928.4L
    switch (transaction)
    {
    case this->eTrans_Select_CharInfo :
        if (success) {
            shared_.state |= this->eState_Loaded_CharInfo;
        }
        else {
            shared_.state &= ~this->eState_Loaded_CharInfo;
        }
        break;
    case this->eTrans_Select_InvenInfo:
        if (success) {
            shared_.state |= this->eState_Loaded_InvenInfo;
        }
        else {
            shared_.state &= ~this->eState_Loaded_InvenInfo;
        }
        break;
    case this->eTrans_Select_PetList:
        if (success) {
            shared_.state |= this->eState_Loaded_PetList;
        }
        else {
            shared_.state &= ~this->eState_Loaded_PetList;
        }
        break;
    case this->eTrans_Select_EquipInfo:
        if (success) {
            shared_.state |= this->eState_Loaded_EquipInfo;
        }
        else {
            shared_.state &= ~this->eState_Loaded_EquipInfo;
        }
        break;
    case this->eTrans_Select_Warehouse:
        if (success) {
            shared_.state |= this->eState_Loaded_Warehouse;
        }
        else {
            shared_.state &= ~this->eState_Loaded_Warehouse;
        }
        break;
    case this->eTrans_Select_WareItems:
        if (success) {
            shared_.state |= this->eState_Loaded_WareItems;
        }
        else {
            shared_.state &= ~this->eState_Loaded_WareItems;
        }
        break;
        //
    //case this->eTrans_Update_CharInfo :
    //case this->eTrans_Update_InvenInfo:
    //case this->eTrans_Update_CharState:
    case this->eTrans_Update_Warehouse:
        break;
    case this->eTrans_Create_Character:
        break;
        //
    default:
        ASSERT(!"unexpected transaction value");
        return false;
    }
    // processed
    shared_.transaction &= ~transaction;
    shared_.loaded = ((shared_.state & this->eState_All_Character_Loaded) ==
                      this->eState_All_Character_Loaded);
    return true;
}

bool DBQueryProxyNode::UserNode::Update()
{
    bool changed_events = false;
    bool cleared = false;
    cleared = UpdateUserLevelItemSlots();
    if (cleared == false) {
        changed_events = true;
    }
    if (changed_events) {
        return false;
    }
    return true;
}


bool DBQueryProxyNode::UserNode::UpdateUserLevelItemSlots()
{
    const bool loaded_warehouse = (shared_.state & this->eState_Loaded_WareItems) != 0;
    if (loaded_warehouse == false) {
        return true;
    }
    //
    const USERGUID user_guid = db_user_->GetUserGuid();
    //
    bool changed_events = false;
    STRING_SAFER_QUERY64 query_buffer;
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
                        _T("{?=call S_WarehouseItemSlotInfo_Update (?,?,?,?,?,?,?,?)}"));
                    Query_ItemSlotInfo_Update* const query = Query_ItemSlotInfo_Update::ALLOC();
                    query->SetQuery(query_buffer);
                    query->SetUserKey(user_guid);
                    // setup parameters
                    Query_ItemSlotInfo_Update::sQueryParam* parameters = &query->parameters_[0];
                    static_cast<ItemSlotResultRecord&>(*parameters) = update_node_it->latest_data_;
                    parameters->guid_ = user_guid;
                    parameters->slot_type_ = WarehouseSlotUpdateTable::kSlotIndex;
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
                db_user_->DBQuery(DG_WAREHOUSE, DG_WAREHOUSE_ITEMSLOTINFO_UPDATE_INTER_DBR,
                                  update_node_it->query_);
            }
        };
    }
    if (changed_events) {
        return false;
    }

    return true;
}

// list = { warehouse, }
// G->D update stream
bool DBQueryProxyNode::UserNode::StoreItemSlotInfo(SLOTIDX slot_type,
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
    //
    return true;
}

// D->G send stream
bool DBQueryProxyNode::UserNode::LoadItemSlotInfo(SLOTIDX slot_type,
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
bool DBQueryProxyNode::UserNode::DBResult(const Query_ItemSlotInfo_Select& query_result)
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

bool DBQueryProxyNode::UserNode::DBResult(const Query_ItemSlotInfo_Update& query_result)
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


}; //end of namespace 'nsQuery'

//==================================================================================================
