#include "stdafx.h"
#include "./ItemManager.h"
#include "./ItemManager_Util.h"
//
#include <ItemInfoParser.h>
#include <ItemTypeList.h>

#include "Player.h"
#include "InventorySlotContainer.h"

//==================================================================================================

// implemented by _NA000023_090323_ENHANCED_INVENTORY_SLOT_FUNCTIONALITY_

namespace nsSlot {
;
//--------------------------------------------------------------------------------------------------
// f100903.4L, the SortStateNode structure support two phase search data caching.
typedef SCSlotContainer::SLOT_LINEAR_LIST LinearList;

struct SortStateNode
{
    //
    uint8_t check_cache_; // use eNodeCheckState
    bool duplicable_;
    SLOTCODE item_code_;
    const SCSlotContainer::SlotNode* slot_node_;
};

struct SortStateTable
{
    enum eNodeCheckState {
        NotChecked   = 0x00,
        Empty        = 0x01,
        LockedState  = 0x02,
        Skipped      = 0x04,
    };
    //
    void Clear();
    void ChangeMode(SearchOffset::eSearchMode change_mode);
    //
    int current_mode_; // use SearchOffset::eSearchMode
    bool option_check_empty_;
    SortStateNode states_[ItemQuery::kMaxNumberOfPosSlots];
    SearchOffset iterators_[ItemQuery::kMaxNumberOfPosSlots];
};

inline void SortStateTable::Clear() {
    // NOTE, dangerous routine, however a nessessary index node is realigned each query.
    const size_t size = static_cast<size_t>(\
        reinterpret_cast<char*>(iterators_) - reinterpret_cast<char*>(this));
    ZeroMemory(this, size);
}

void SortStateTable::ChangeMode(SearchOffset::eSearchMode change_mode)
{
    assert(change_mode >= 0 && change_mode < SearchOffset::eSearch_Mode_Counts);
    current_mode_ = change_mode;
    switch (current_mode_)
    {
    case SearchOffset::eSearch_1st_FindOverlappingSlot:
        option_check_empty_ = false;
        break;
    case SearchOffset::eSearch_2nd_FindEmptySlot:
        option_check_empty_ = true;
        break;
    case SearchOffset::eSearch_3rd_FindOverlappingSlot:
        option_check_empty_ = false;
        break;
    case SearchOffset::eSearch_4th_FindEmptySlot:
        option_check_empty_ = true;
        break;
    case SearchOffset::eSearch_5th_FindOverlappingSlot:
        option_check_empty_ = false;
        break;
    case SearchOffset::eSearch_6th_FindEmptySlot:
        option_check_empty_ = true;
        break;
    }
}

static SortStateTable sort_state_table;

//--------------------------------------------------------------------------------------------------

static bool insert_slot_iterator_skip_cache_static[ItemQuery::kMaxNumberOfPosSlots];
static SCSlotContainer::SLOT_LINEAR_LIST::const_iterator insert_slot_iterators_static[\
    ItemQuery::kMaxNumberOfPosSlots];
static SCSlotContainer::SLOT_LINEAR_LIST::const_iterator insert_slot_end_of_iterators_static[\
    ItemQuery::kMaxNumberOfPosSlots];

// If the result is true, the item_info of a QueryInfo::Node is aligned reference.
// If the result is false, can't find item info.
bool ItemQuery::InterAlignItemInfo(ItemQuery::QueryInfo::Node* query_node,
                                   ItemInfoParser* item_parser,
                                   int query_node_index, bool sorting_insert)
{
    if (query_node->item_info == NULL)
    {
        if (query_node->item_slot) {
            query_node->item_info = query_node->item_slot->GetItemInfo();
        }
        else if (query_node->item_code) {
            query_node->item_info = item_parser->GetItemInfo(query_node->item_code);
        }
    }
    //
    const BASE_ITEMINFO* item_info = query_node->item_info;
    if (item_info == NULL || item_info->m_Code == 0) {
        return false;
    }

    const uint16_t item_type = item_info->m_wType;
    static const uint16_t kFunctionMaterials[] = {
        eITEMTYPE_ENCHANT, eITEMTYPE_CRYSTAL, eITEMTYPE_SOCKET,
        eITEMTYPE_SOCKETMATERIAL, eITEMTYPE_ACCESSORYMATERIAL, eITEMTYPE_SACCESSORYMATERIAL,
        eITEMTYPE_SOCKET_ZARD,
    };
    BOOST_STATIC_ASSERT(_countof(kFunctionMaterials) == 7);
    struct LocalUtil {
        static bool IsFunctionMaterials(const uint16_t* it, size_t size, uint16_t item_type) {
            const uint16_t* const end = it + size;
            for (; it != end; ++it) {
                if (*it == item_type) {
                    return true;
                }
            }
            return false;
        }
    };
    // select insert group
    //
    // set default
    query_node->item_insert_rule = nsSlot::InventoryConfig::eInsertRule_Normal;
    if (sorting_insert == false) {
        return true;
    }

    if (false) { // dummy
    }
    else if (item_type == eITEMTYPE_QUEST) {
        query_node->item_insert_rule = nsSlot::InventoryConfig::eInsertRule_Quest;
    }
    else if (item_info->m_wACCode != 0) {
        query_node->item_insert_rule = nsSlot::InventoryConfig::eInsertRule_AC;
    }
    else if (item_type == eITEMTYPE_WASTE || item_info->IsCanUseWaste()) {
        query_node->item_insert_rule = nsSlot::InventoryConfig::eInsertRule_Waste;
    }
    else if (SCItemSlot::CanEquip(item_info)) {
        query_node->item_insert_rule = nsSlot::InventoryConfig::eInsertRule_Equip;
    }
    else if (LocalUtil::IsFunctionMaterials(kFunctionMaterials, _countof(kFunctionMaterials), \
                                            item_type))
    {
        query_node->item_insert_rule = nsSlot::InventoryConfig::eInsertRule_Material;
    }
    // get insert rule
    if (slot_index_ == SI_INVENTORY)
    {
        CInventorySlotContainer* inventory = static_cast<CInventorySlotContainer*>(item_container_);
        // f100904.4L, replace an old data structure that has one phase searching algorithm.
        nsSlot::SearchOffset* search_offset = &sort_state_table.iterators_[query_node_index];
        inventory->GetConfiguredSlotList(\
            static_cast<nsSlot::InventoryConfig::eInsertRule>(query_node->item_insert_rule),
            search_offset);
    }

    return true;
}

// if current operation is queried by the inverse_select query, call the below method.
bool ItemQuery::PrefetchQueryInfoForInverseSelect()
{
    bool success = true;
    // CHANGES, f100903.4L
    sort_state_table.Clear();
    //
    ZeroMemory(insert_slot_iterator_skip_cache_static, \
               sizeof(insert_slot_iterator_skip_cache_static));
    ItemInfoParser* const item_parser = ItemInfoParser::Instance();
    QueryInfo::Node* query_it = query_info_.nodes;
    for (int query_index = 0;
         query_index < query_info_.number_of_queries;
         ++query_index, ++query_it)
    {
        success = InterAlignItemInfo(query_it, item_parser, query_index, true);
        if (success == false) {
            break;
        }
    }
    assert(success != false);
    return success;
}

bool ItemQuery::SetupDefaults(eItemQueryType query_type,
                              ItemQuery* item_query, Player* player, SLOTIDX slot_index)
{
    // boundary rule check section
    BOOST_STATIC_ASSERT(sizeof(POSTYPE) == 1 &&
                        _countof(item_query->query_info_.nodes) == UCHAR_MAX);
    BOOST_STATIC_ASSERT(_countof(item_query->query_info_.nodes) ==
                        _countof(item_query->result_info_.nodes));

    // input argument test
    if (item_query == NULL) {
        return false;
    }
    item_query->Clear(); // prevent query call on uncleared state by invalid inputs
    //                    eSlotContainerIndex
    if (player == NULL || slot_index >= SI_MAX) {
    }
    // query type test
    if ((query_type > eItemQuery_None &&
         query_type < eItemQuery_Max) == false)
    {
        return false;
    }
    switch (slot_index)
    {
    case SI_INVENTORY:
    case SI_EQUIPMENT:
    case SI_WAREHOUSE:
    case SI_GUILDWAREHOUSE:
        break;
    default:
        return false;
    }
    //
    item_query->query_type_ = query_type;
    item_query->player_ = player;
    item_query->slot_index_ = slot_index;
    item_query->item_manager_ = player->GetItemManager();
    item_query->item_container_ = item_query->item_manager_->GetItemSlotContainer(slot_index);

    return true;
}
//--------------------------------------------------
// target:
//  - eItemQuery_Select,
//  - eItemQuery_InverseSelect,
//  - eItemQuery_Obtain,
//  - eItemQuery_Delete,
// select one query data = { ItemCode, ItemSlot }
// if you want multi items query, use AddArg_ItemCode recursively
int ItemQuery::AddArg_ItemCode(SLOTCODE item_code, int number_of_items)
{
    if (query_info_.number_of_queries >= _countof(query_info_.nodes) ||
        (number_of_items <= 0 || number_of_items > USHRT_MAX))
    {
        return -1;
    }
    if (item_code == 0) {
        return -1;
    }
    if (QueryInfo::Node* info_node = &query_info_.nodes[query_info_.number_of_queries])
    { // alignment
        info_node->position = INVALID_POSTYPE_VALUE;
        //info_node->item_insert_rule
        info_node->item_code = item_code;
        info_node->number_of_items = number_of_items;
        info_node->item_slot = NULL;

        query_info_.sum_of_queried_items += number_of_items;
    };
    //
    int registered_index = query_info_.number_of_queries;
    ++query_info_.number_of_queries;
    return registered_index;
}

int ItemQuery::AddArg_ItemSlot(const SCItemSlot& item_slot, int number_of_items,
                               bool non_constant_slot)
{
    if (query_info_.number_of_queries >= _countof(query_info_.nodes) ||
        (number_of_items <= 0 || number_of_items > USHRT_MAX))
    {
        return -1;
    }
    SLOTCODE item_code = item_slot.GetItemCode();
    if (item_code == 0) {
        return -1;
    }
    { // alignment
        QueryInfo::Node* info_node = &query_info_.nodes[query_info_.number_of_queries];
        info_node->position = INVALID_POSTYPE_VALUE;
        //info_node->item_insert_rule
        info_node->item_code = item_code;
        info_node->number_of_items = number_of_items;
        info_node->item_slot = &item_slot;
        info_node->item_info = item_slot.GetItemInfo();

        info_node->setting_guide = non_constant_slot == false ?
                                    info_node->eSettingGuid_ConstantSlot :
                                    info_node->eSettingGuid_NonConstantSlot;

        query_info_.sum_of_queried_items += number_of_items;
    };
    //
    int registered_index = query_info_.number_of_queries;
    ++query_info_.number_of_queries;
    return registered_index;
}

int ItemQuery::AddArg_Custom(POSTYPE number_of_kinds,
                             const SLOTCODE* item_code_it, const POSTYPE* item_num_it)
{
    if (number_of_kinds == 0 ||
        query_info_.number_of_queries + number_of_kinds >= _countof(query_info_.nodes))
    {
        return -1;
    }
    QueryInfo::Node* dest_node = &query_info_.nodes[query_info_.number_of_queries];
    int sum_of_queried_items = 0;
    int index = 0;
    // CHANGES, f100831.1L, add a missing increase operation.
    for (; index < int(number_of_kinds); ++index, ++dest_node)
    {
        if (*item_num_it == 0 || *item_code_it == 0) {
            break;
        }
        dest_node->position = INVALID_POSTYPE_VALUE;
        dest_node->item_code = *item_code_it++;
        dest_node->number_of_items = *item_num_it++;
        dest_node->item_slot = NULL;

        sum_of_queried_items += dest_node->number_of_items;
    }
    //
    if (index != number_of_kinds)
    {
        dest_node = &query_info_.nodes[query_info_.number_of_queries];
        const size_t cleared_size = \
            reinterpret_cast<char*>(&query_info_.nodes[_countof(query_info_.nodes)]) -
            reinterpret_cast<char*>(dest_node);
        ZeroMemory(dest_node, cleared_size);
        return -1;
    }
    query_info_.sum_of_queried_items += sum_of_queried_items;

    // to compatible interface, return last element index
    int registered_last_index = query_info_.number_of_queries + index - 1;
    query_info_.number_of_queries += index;
    return registered_last_index;
}

// for Debug routine
bool ItemQuery::CheckResultValidatation_Custom(POSTYPE number_of_kinds,
                                               const SLOTCODE* item_code_array,
                                               const POSTYPE* item_num_array) const
{
    __UNUSED((number_of_kinds, item_code_array));
    // for iteration
    //const ItemQuery::QueryInfo& query_info = delete_query.query_info();
    const ItemQuery::QueryInfo::Node* query_node = query_info_.nodes;
    // for debug section
    const POSTYPE* source_num_it = item_num_array;
    for (int index = 0; index < query_info_.number_of_queries;
         ++index, ++query_node, ++source_num_it)
    {
        if (!FlowControl::FCAssert(query_node->sum_of_items == *source_num_it)) {
            return false;
        }
    }
    return true;
}

// (f100525.4L) set up values of a LOCKED_INFO and lock slots automatically.
// (WARNING) the result value must be checked with number_of_locked_info
// after a calling of a 'Query'
// assert(result_info.number_of_results <= number_of_locked_info)
bool ItemQuery::SetupLockedInfoWithLock(POSTYPE* const number_of_used_locked_info,
                                        const int number_of_locked_info,
                                        LOCKED_INFO* const locked_info,
                                        POSTYPE number_of_kinds,
                                        const DWORD* datetime_array)
{
    *number_of_used_locked_info = 0;
    if (number_of_locked_info < result_info_.number_of_results ||
        number_of_locked_info < number_of_kinds)
    {
        return false;
    }
    const ItemQuery::ResultInfo::Node* query_result_it = result_info_.nodes,
                                     * query_result_end = \
                                            &result_info_.nodes[_countof(result_info_.nodes)];
    //
    const DWORD* datetime_it = datetime_array;
    LOCKED_INFO* locked_info_it = locked_info;
    for (int countdown = result_info_.number_of_results;
         countdown && query_result_it != query_result_end;
         ++query_result_it)
    {
        if (query_result_it->container_item_code == 0 ||
            query_result_it->number_of_selected_items == 0)
        {
            continue;
        }
        ZeroMemory(locked_info_it, sizeof(*locked_info_it));
        locked_info_it->m_code = query_result_it->container_item_code;
        locked_info_it->m_SlotIndex = this->slot_index_;
        locked_info_it->m_pos = query_result_it->position;
        locked_info_it->m_num = query_result_it->number_of_selected_items;
        //
        const SCItemSlot& query_item_slot = *query_result_it->query_item_slot;
        const nsSlot::ItemRuleInfo& item_rule_info = query_item_slot.GetItemRuleInfo();
        locked_info_it->m_Enchant = query_item_slot.GetEnchant();
        locked_info_it->m_Rank = query_item_slot.GetRank();
        locked_info_it->m_ItemType = nsSlot::ItemTypeChanger::GetKindOfItemType(query_item_slot);
        if (datetime_it) {
            locked_info_it->m_DateTime = *datetime_it;
            ++datetime_it;
        }
        query_result_it->container_item_slot->SetLock(true);

        WAVERIX_WRAPPER((WAVERIX_LC_CALL3("waverix_logitemquery",
            countdown, locked_info_it->m_pos, locked_info_it->m_code)));

        ++locked_info_it;
        ++(*number_of_used_locked_info);
        --countdown;
    }
    return true;
}

// (f100607.5L) add the logging function after a result process completed.
bool ItemQuery::WriteToGameLogBasedOnResult(Player* player, int log_type)
{
    const ItemQuery::ResultInfo::Node* query_result_it = result_info_.nodes,
                                     * query_result_end = \
                                            &result_info_.nodes[_countof(result_info_.nodes)];
    //
    for (int countdown = result_info_.number_of_results;
         countdown && query_result_it != query_result_end;
         ++query_result_it)
    {
        if (query_result_it->container_item_code == 0 ||
            query_result_it->number_of_selected_items == 0)
        {
            continue;
        }
        GAMELOG->LogItem(log_type, player,
                         query_result_it->query_item_slot, 0,
                         query_result_it->number_of_selected_items);
    }
    return true;
}

bool ItemQuery::MakeSerializedStream(INVENTORY_TOTAL_INFO* inven_total_info) const
{
    // (NOTE) don't support temp inventory since 0902 version
    const ItemQuery::ResultInfo::Node* query_result_it = result_info_.nodes,
                                     * query_result_end = \
                                            &result_info_.nodes[_countof(result_info_.nodes)];
    //
    INVENTORY_TOTAL_INFO::SLOT_TYPE* serialize_result_it = inven_total_info->m_Slot;
    for (int countdown = result_info_.number_of_results;
         countdown && query_result_it != query_result_end;
         ++query_result_it)
    {
        if (query_result_it->container_item_code == 0 ||
            query_result_it->number_of_selected_items == 0)
        {
            continue;
        }
        ZeroMemory(serialize_result_it, sizeof(*serialize_result_it));
        serialize_result_it->m_Pos = query_result_it->position;
        query_result_it->container_item_slot->CopyOut(serialize_result_it->m_Stream);

        WAVERIX_WRAPPER((WAVERIX_LC_CALL3("waverix_logitemquery",
            countdown, serialize_result_it->m_Pos, serialize_result_it->m_Stream.Part.wCode)));

        ++serialize_result_it;
        --countdown;
    }
    inven_total_info->m_InvenCount = static_cast<BYTE>(result_info_.number_of_results);
    inven_total_info->m_TmpInvenCount = 0;
    return true;
}

bool ItemQuery::MakeSerializedStream(JUST_INVENTORY_TOTAL_INFO* inven_total_info) const
{
    const ItemQuery::ResultInfo::Node* query_result_it = result_info_.nodes,
                                     * query_result_end = \
                                            &result_info_.nodes[_countof(result_info_.nodes)];
    //
    JUST_INVENTORY_TOTAL_INFO::SLOT_TYPE* serialize_result_it = inven_total_info->m_Slot;    
    for (int countdown = result_info_.number_of_results;
         countdown && query_result_it != query_result_end;
         ++query_result_it)
    {
        if (query_result_it->container_item_code == 0 ||
            query_result_it->number_of_selected_items == 0)
        {
            continue;
        }
        ZeroMemory(serialize_result_it, sizeof(*serialize_result_it));
        serialize_result_it->m_Pos = query_result_it->position;
        query_result_it->container_item_slot->CopyOut(serialize_result_it->m_Stream);
        ++serialize_result_it;
        --countdown;
    }
    inven_total_info->m_Count = static_cast<BYTE>(result_info_.number_of_results);
    return true;
}

bool ItemQuery::MakeSerializedStream(INVEN_DELETE_TOTAL_INFO* inven_delete_total_info) const
{
    const ItemQuery::ResultInfo::Node* query_result_it = result_info_.nodes,
                                     * query_result_end = \
                                            &result_info_.nodes[_countof(result_info_.nodes)];
    //
    INVEN_DELETE_TOTAL_INFO::SLOT_TYPE* serialize_result_it = inven_delete_total_info->m_Slot;    
    for (int countdown = result_info_.number_of_results;
         countdown && query_result_it != query_result_end;
         ++query_result_it)
    {
        if (query_result_it->container_item_code == 0 ||
            query_result_it->number_of_selected_items == 0)
        {
            continue;
        }
        serialize_result_it->m_Pos = query_result_it->position;
        serialize_result_it->m_Num = static_cast<POSTYPE>(query_result_it->number_of_selected_items);
        ++serialize_result_it;
        --countdown;
    }
    inven_delete_total_info->m_Count = static_cast<BYTE>(result_info_.number_of_results);
    return true;
}

bool ItemQuery::MakeSerializedStream(int number_of_kinds,
                                     POS_INFO* pos_info_array,
                                     POSTYPE* upperbound) const
{
    // ... delete reserved
    const ItemQuery::ResultInfo::Node* query_result_it = result_info_.nodes,
                                     * query_result_end = \
                                            &result_info_.nodes[_countof(result_info_.nodes)];
    //
    for (int countdown = result_info_.number_of_results, countdown_for_kinds = number_of_kinds;
         countdown && countdown_for_kinds && query_result_it != query_result_end;
         ++query_result_it)
    {
        if (query_result_it->container_item_code == 0 ||
            query_result_it->number_of_selected_items == 0)
        {
            continue;
        }
        pos_info_array->m_pos = query_result_it->position;
        pos_info_array->m_num = static_cast<POSTYPE>(query_result_it->number_of_selected_items);
        ++pos_info_array;
        --countdown;
        --countdown_for_kinds;
    }
    *upperbound = static_cast<POSTYPE>(result_info_.number_of_results);
    return true;
}

}; //end of namespace


//==================================================================================================

nsSlot::ItemQueryResult
ItemManager::Query(nsSlot::ItemQuery* item_query)
{
    bool is_valid = true;
    bool is_multi_select = false;
    nsSlot::ItemQueryResult query_result = { NULL, };

    switch (item_query->query_type_)
    {
    case nsSlot::eItemQuery_Select:
        Query_Select(&query_result, item_query);
        break;
    case nsSlot::eItemQuery_InverseSelect:
        //Query_InverseSelect(&query_result, item_query);
        // change selector by inventory sorting configuration
        Query_InverseSelectSort(&query_result, item_query);
        break;
    default:
        is_valid = false;
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"] Unexpected query type|Query=%d|",
               item_query->query_type_);
        break;
    };

    if (is_valid == false)
    {
        ASSERT(!"Impossible reached point");
        __debugbreak();
        query_result.slot = NULL;
    }
    return query_result;
}

//--------------------------------------------------------------------------------------------------
// @description
//  The interface to find targets which are items with number 
//  (WARNING) don't input same item code by AddArg_Item~()
//
// @note
//  If the function succeeds, the return value is true and has partial result based on query array
//  - the partial result has distributed in the result_info_array, \
//    that corresponding to query_info_array.
//  - if item query result is accepted state, \
//    number_of_items value, the pos_info of the result_info_array, is matched to it of \
//    the query_info_array
//  - (pseudo)
//      item_query_ptr->AddArg_ItemCode(item_code1, item_num1); // 1st item index is 0
//      item_query_ptr->AddArg_ItemCode(item_code2, item_num2); // 2nd item index is 1
//      ... (continued)
//
//      if (Query_Select(query_result_ptr, item_query_ptr) != false) {
//          if (item_query_ptr->result_info_array[0].number_of_items == item_num1) {
//              first query is succeeds.
//          }
//          if (item_query_ptr->result_info_array[1].number_of_items == item_num2) {
//              second query is succeeds.
//          }
//          ... (continued)
//      }
//  If the function fails, the return value is false
//  - can't search state
//
bool ItemManager::Query_Select(nsSlot::ItemQueryResult* query_result,
                               nsSlot::ItemQuery* item_query)
{
    using nsSlot::ItemQuery;
    if (item_query->query_info_.number_of_queries == 0)
    { // the number_of_query_items value must be not zero.
        query_result->result_of_select = 0;
        return false;
    }
    //
    SCItemSlotContainer* const item_container = item_query->item_container_;
    const SCSlotContainer::SLOT_LINEAR_LIST& slot_list = item_container->GetSlotList();
    SCSlotContainer::SLOT_LINEAR_LIST::const_iterator it = slot_list.begin(),
                                                      end = slot_list.end();
    //
    int searched_items = 0, sum_of_selected_items = 0;
    //
    const int max_number_of_slots = item_container->GetMaxSlotNum();
    for (int index = 0; index < max_number_of_slots && it != end; ++index, ++it)
    {
        if (item_query->query_info_.number_of_queries == searched_items) {
            // all items found, change condition by calculated result
            break;
        }
        const SCSlotContainer::SlotNode& slot_node = *it;
        SCItemSlot* container_item = static_cast<SCItemSlot*>(slot_node.slot);
        const SLOTCODE container_item_code = container_item->GetItemCode();
        if (container_item_code == 0) {
            // SCItemSlot is empty if code equals zero. that's enough don't have call 'IsEmpty'
            continue;
        }
        if (container_item->IsBlocked() ||
            container_item->IsLocked())// ||
            //container_item->IsUserBlocked())
        {
            continue;
        }
        //
        const int container_item_num = container_item->GetNum();
        ItemQuery::QueryInfo::Node* query_it = item_query->query_info_.nodes;

        for (int search_index = 0;
             search_index < item_query->query_info_.number_of_queries;
             ++search_index, ++query_it)
        {
            if (query_it->item_code != container_item_code) {
                continue;
            }
            if (query_it->number_of_items == query_it->sum_of_items) {
                continue;
            }

            if (ItemQuery::ResultInfo::Node* result_it = &item_query->result_info_.nodes[index])
            {   //alignment result
                int remains_in_container = container_item_num - \
                                           result_it->number_of_selected_items;
                if (remains_in_container == 0) { // all selected
                    continue;
                }
                if (result_it->number_of_selected_items == 0) {
                    ++item_query->result_info_.number_of_results;
                }

                result_it->position = static_cast<POSTYPE>(index);
                result_it->container_item_code = container_item_code;
                result_it->query_item_slot = query_it->item_slot;
                result_it->container_item_slot = container_item;
                result_it->update_guide = query_it->setting_guide;
                result_it->update_guide |= result_it->eUpdateGuide_Update;

                // assert(post_remains >= 1);
                if (query_it->sum_of_items + remains_in_container <= query_it->number_of_items)
                {
                    result_it->number_of_selected_items += remains_in_container;
                    query_it->sum_of_items += remains_in_container;
                    sum_of_selected_items += remains_in_container;
                }
                else //if (sum_of_search_items + remains_in_container > query_it->number_of_items)
                {
                    int increment = query_it->number_of_items - query_it->sum_of_items;
                    result_it->number_of_selected_items += increment;
                    query_it->sum_of_items = query_it->number_of_items;
                    sum_of_selected_items += increment;
                }
            };
            //
            if (query_it->sum_of_items == query_it->number_of_items)
            {
                ++searched_items;
                //break; (consider same item multi-selection)
            }
        }
        // already found state = if (search_index != item_query->query_info_.number_of_queries)
    }
    item_query->result_info_.sum_of_selected_items += sum_of_selected_items;

    // query_result.result_of_contains has only the number of 1st item to support custom logic
    query_result->result_of_select = item_query->result_info_.sum_of_selected_items;
    return true;
};

bool ItemManager::Query_InverseSelect(nsSlot::ItemQueryResult* query_result,
                                      nsSlot::ItemQuery* item_query)
{
    using nsSlot::ItemQuery;
    query_result->result_of_inverse_select = 0;
    // the number_of_query_items value must be not zero.
    if (item_query->query_info_.number_of_queries == 0) {
        return false;
    }
    //
    ItemInfoParser* const item_parser = ItemInfoParser::Instance();
    SCItemSlotContainer* const item_container = item_query->item_container_;
    const SCSlotContainer::SLOT_LINEAR_LIST& slot_list = item_container->GetSlotList();
    SCSlotContainer::SLOT_LINEAR_LIST::const_iterator it = slot_list.begin(),
                                                      end = slot_list.end();
    //
    int searched_items = 0, sum_of_selected_items = 0;

    const int max_number_of_slots = item_container->GetMaxSlotNum();
    for (int index = 0; index < max_number_of_slots && it != end; ++index, ++it)
    {
        if (item_query->query_info_.number_of_queries == searched_items) {
            // all items found, change condition by calculated result
            break;
        }
        ItemQuery::QueryInfo::Node* query_it = item_query->query_info_.nodes;

        const SCSlotContainer::SlotNode& slot_node = *it;
        SCItemSlot* container_item = static_cast<SCItemSlot*>(slot_node.slot);
        const SLOTCODE container_item_code = container_item->GetItemCode();
        // (CHANGES) (f100525.4L) prevent a search a locking state on a reserved space
        if (container_item->IsLocked()) { // no-check LaxLock
            continue;
        }

        if (container_item_code == 0)
        {
            ItemQuery::ResultInfo::Node* const result_it = &item_query->result_info_.nodes[index];
            // SCItemSlot is empty if code equals zero. that's enough don't have call 'IsEmpty'
            // The InverseSelect find a surplus space, \
            // like the empty or remains of a overlapped item
            for (int search_index = 0;
                 search_index < item_query->query_info_.number_of_queries;
                 ++search_index, ++query_it)
            {
                if (query_it->number_of_items == query_it->sum_of_items) {
                    continue;
                }
                // the below condition check is needed, because this slot is empty
                if (result_it->container_item_code != 0 &&
                    result_it->container_item_code != query_it->item_code)
                {
                    continue;
                }
                const bool reserved_slot = result_it->container_item_code != 0;
                // (CHANGES) (f100727.2L) the query of a 'InverseSelect' is
                // based on a obtain function directly or indirectly(reserve)
                // this function is to support a long term item transaction like a shop transaction
                if (item_query->find_empty_only_ && reserved_slot) {
                    break;
                }
                if (item_query->InterAlignItemInfo(query_it, item_parser,
                                                   search_index, false) == false)
                {
                    continue;
                }

                // other states are filtered that is checked on start of a server,
                // or must be checked.
                int max_overlap_num = query_it->item_info->m_byDupNum <= 1
                                    ?  1 : query_it->item_info->m_byDupNum;
                if (reserved_slot && max_overlap_num == 1) {
                    break; // already this slot has reserved by another same an item
                }
                // change algorithm whether result_it->number_of_selected_items is zero or not zero.
                // if the value is zero, ok
                // if the value is not zero, must be considered it.
                ;   max_overlap_num -= result_it->number_of_selected_items;
                if (max_overlap_num == 0) {
                    break;
                }
                if (result_it->number_of_selected_items == 0) {
                    ++item_query->result_info_.number_of_results;
                }

                int remains = query_it->number_of_items - query_it->sum_of_items;
                ;{ //alignment result
                    if (max_overlap_num < remains)
                    {
                        result_it->number_of_selected_items += max_overlap_num;
                        query_it->sum_of_items += max_overlap_num;
                        sum_of_selected_items += max_overlap_num;
                    }
                    else
                    {
                        result_it->number_of_selected_items += remains;
                        query_it->sum_of_items += remains;
                        sum_of_selected_items += remains;
                    }
                    result_it->position = static_cast<POSTYPE>(index);
                    result_it->container_item_code = query_it->item_code;
                    // it's possible any same query item can replace result_it->query_item_slot
                    result_it->query_item_slot = query_it->item_slot;
                    result_it->container_item_slot = container_item;
                    result_it->update_guide = query_it->setting_guide;
                    result_it->update_guide |= result_it->eUpdateGuide_Insert;
                };
                if (query_it->sum_of_items == query_it->number_of_items)
                {
                    ++searched_items;
                    //break; // <- to support multi - inverse select
                }
            }
            continue;
        }
        // (CHANGES) (f100525.4L) the query of a 'InverseSelect' is
        // based on a obtain function directly or indirectly(reserve)
        // this function is to support a long term item transaction like a shop transaction
        if (item_query->find_empty_only_) {
            continue;
        }
        // (CHANGES) (f100525.4L) move a lock check block
        // to prevent a search a locking state on a reserved space
        if (container_item->IsBlocked() ||
            container_item->IsUserBlocked()) // no-check LaxLock
        {
            continue;
        }

        ItemQuery::ResultInfo::Node* const result_it = &item_query->result_info_.nodes[index];
        const POSTYPE container_item_num = container_item->GetNum();

        for (int search_index = 0;
             search_index < item_query->query_info_.number_of_queries;
             ++search_index, ++query_it)
        {
            if (query_it->item_code != container_item_code) {
                continue;
            }
            if (query_it->number_of_items == query_it->sum_of_items) {
                continue;
            }
            if (item_query->InterAlignItemInfo(query_it, item_parser,
                                               search_index, false) == false)
            {
                continue;
            }
            // f100909.1L, add a rule about an item tag
            if (item_query->option_tag_.value != container_item->GetOptionTag().value) {
                continue;
            }
            //
            int max_overlap_num = query_it->item_info->m_byDupNum <= 1
                                ?  1 : query_it->item_info->m_byDupNum;
            if ((max_overlap_num == 1 && result_it->container_item_code != 0) ||
                !FlowControl::FCAssert(max_overlap_num >= container_item_num))
            {
                break;
            }
            ;   max_overlap_num -= container_item_num;
            ;   max_overlap_num -= result_it->number_of_selected_items; assert(max_overlap_num >= 0);
            if (max_overlap_num <= 0) {
                break;
            }
            //
            if (result_it->number_of_selected_items == 0) {
                ++item_query->result_info_.number_of_results;
            }

            int remains = query_it->number_of_items - query_it->sum_of_items;
            { //alignment result
                if (max_overlap_num < remains)
                {
                    result_it->number_of_selected_items += max_overlap_num;
                    query_it->sum_of_items += max_overlap_num;
                    sum_of_selected_items += max_overlap_num;
                }
                else
                {
                    result_it->number_of_selected_items += remains;
                    query_it->sum_of_items += remains;
                    sum_of_selected_items += remains;
                }
                result_it->position = static_cast<POSTYPE>(index);
                result_it->container_item_code = query_it->item_code;
                result_it->query_item_slot = query_it->item_slot;
                result_it->container_item_slot = container_item;
                result_it->update_guide = query_it->setting_guide;
                result_it->update_guide |= result_it->eUpdateGuide_Update;
            };
            if (query_it->sum_of_items == query_it->number_of_items)
            {
                ++searched_items;
                //break; // <- to support multi - inverse select
            }
        }
        // already found state = if (search_index != item_query->query_info_.number_of_queries)
    }
    item_query->result_info_.sum_of_selected_items += sum_of_selected_items;
    // query_result.result_of_contains has only the number of 1st item to support custom logic
    query_result->result_of_inverse_select = item_query->result_info_.sum_of_selected_items;
    return true;
}

// The interface to find empty slots or remains of stacked slots \
// which are considered items with number and inventory sorted insert configuration
bool ItemManager::Query_InverseSelectSort(nsSlot::ItemQueryResult* query_result,
                                          nsSlot::ItemQuery* item_query)
{
    using namespace nsSlot;
    if (item_query->query_info_.number_of_queries == 0)
    { // the number_of_query_items value must be not zero.
        query_result->result_of_inverse_select = 0;
        return false;
    }
    if (item_query->PrefetchQueryInfoForInverseSelect() == false)
    {
        query_result->result_of_inverse_select = 0;
        return false;
    }
    //
    bool success = false;
    for (int mode = 0; mode < nsSlot::SearchOffset::eSearch_Mode_Counts; ++mode)
    {
        sort_state_table.ChangeMode(static_cast<nsSlot::SearchOffset::eSearchMode>(mode));
        success = Query_InverseSelectSortInternal(query_result, item_query);
        if (success == false) {
            return success;
        }
        if (item_query->query_info_.sum_of_queried_items == query_result->result_of_inverse_select) {
            success = true; // any logic don't use the result, but support the correct result.
            break;
        }
    };

    return success;
};

// CHANGES, f100903.4L, the algorithm that a duplicable item must find an exist slot first has
// six phase depth complexity. so that a below method is split from a Query_InverseSelectSort
// to support a mode control like not-empty, empty, and that. considering a tab sort priority.
bool ItemManager::Query_InverseSelectSortInternal(nsSlot::ItemQueryResult* query_result,
                                                  nsSlot::ItemQuery* item_query)
{
    // this method is depend on Query_InverseSelectSort.
    using namespace nsSlot;
    //
    SCItemSlotContainer* const item_container = item_query->item_container_;
    ItemQuery::QueryInfo::Node* query_it = item_query->query_info_.nodes;
    //
    const int current_search_mode = sort_state_table.current_mode_;
    int searched_items = 0, sum_of_selected_items = 0;

    const int max_number_of_slots = item_container->GetMaxSlotNum();
    for (int search_index = 0;
         search_index < item_query->query_info_.number_of_queries;
         ++search_index, ++query_it)
    {
        if (query_it->number_of_items == query_it->sum_of_items) {
            continue;
        }
        // f100904.4L, is not overlapped item                    find overlapping slot
        if (query_it->item_info->m_byDupNum <= 1 && sort_state_table.option_check_empty_ == false) {
            continue;
        }
        //
        LinearList::const_iterator& it = \
            sort_state_table.iterators_[search_index].search_it[current_search_mode];
        LinearList::const_iterator& end = \
            sort_state_table.iterators_[search_index].search_end[current_search_mode];
        // prevent an index of out ranges problem because the third group do not always have counts
        if (it == end) {
            continue;
        }
        //
        for (int index = it->pos;
             index < ItemQuery::kMaxNumberOfPosSlots && it != end;
             // fix up an access violation about the end iterator
             index = ((++it) != end) ? it->pos : ItemQuery::kMaxNumberOfPosSlots)
        {
            if (item_query->query_info_.number_of_queries == searched_items) {
                // all items found, change condition by calculated result
                break;
            }
            //
            SortStateNode* const sort_state_node = &sort_state_table.states_[index];
            if (sort_state_node->check_cache_ & (sort_state_table.Skipped |
                                                 sort_state_table.LockedState))
            {
                continue;
            }
            //
            if (sort_state_node->slot_node_ == NULL) {
                sort_state_node->slot_node_ = &(*it);
                sort_state_node->item_code_ = static_cast<SCItemSlot*>(
                    sort_state_node->slot_node_->slot)->GetItemCode();
            }
            //
            const SCSlotContainer::SlotNode& slot_node = *sort_state_node->slot_node_;
            SCItemSlot* container_item = static_cast<SCItemSlot*>(slot_node.slot);
            const SLOTCODE container_item_code = sort_state_node->item_code_;
            // (CHANGES) (f100525.4L) prevent a search a locking state on a reserved space
            if (container_item->IsLocked()) { // no-check LaxLock
                sort_state_node->check_cache_ |= sort_state_table.LockedState;
                continue;
            }
            //
            const bool empty_slot = (container_item_code == 0);
            if (empty_slot) {
                sort_state_node->check_cache_ |= sort_state_table.Empty;
            }
            //
            if (empty_slot && sort_state_table.option_check_empty_)
            {
                ItemQuery::ResultInfo::Node* const result_it = &item_query->result_info_.nodes[index];
                // SCItemSlot is empty if code equals zero. that's enough don't have call 'IsEmpty'
                // The InverseSelect find a surplus space, \
                // like the empty or remains of a overlapped item
                // the below condition check is needed, because this slot is empty
                if (result_it->container_item_code != 0 &&
                    result_it->container_item_code != query_it->item_code)
                {
                    continue;
                }
                const bool reserved_slot = result_it->container_item_code != 0;
                // (CHANGES) (f100727.2L) the query of a 'InverseSelect' is
                // based on a obtain function directly or indirectly(reserve)
                // this function is to support a long term item transaction like a shop transaction
                if (item_query->find_empty_only_ && reserved_slot) {
                    continue;
                }
                // other states are filtered that is checked on start of a server,
                // or must be checked.
                int max_overlap_num = query_it->item_info->m_byDupNum <= 1
                                    ?  1 : query_it->item_info->m_byDupNum;
                if (reserved_slot && max_overlap_num == 1)
                {
                    sort_state_node->check_cache_ |= sort_state_table.Skipped;
                    continue; // already this slot has reserved by another same an item
                }
                // change algorithm whether result_it->number_of_selected_items is zero or not zero.
                // if the value is zero, ok
                // if the value is not zero, must be considered it.
                ;   max_overlap_num -= result_it->number_of_selected_items;
                if (max_overlap_num == 0)
                {
                    sort_state_node->check_cache_ |= sort_state_table.Skipped;
                    continue;
                }
                if (result_it->number_of_selected_items == 0) {
                    ++item_query->result_info_.number_of_results;
                }

                int remains = query_it->number_of_items - query_it->sum_of_items;
                { //alignment result
                    if (max_overlap_num < remains)
                    {
                        result_it->number_of_selected_items += max_overlap_num;
                        query_it->sum_of_items += max_overlap_num;
                        sum_of_selected_items += max_overlap_num;
                    }
                    else
                    {
                        result_it->number_of_selected_items += remains;
                        query_it->sum_of_items += remains;
                        sum_of_selected_items += remains;
                    }
                    result_it->position = static_cast<POSTYPE>(index);
                    result_it->container_item_code = query_it->item_code;
                    // it's possible any same query item can replace result_it->query_item_slot
                    result_it->query_item_slot = query_it->item_slot;
                    result_it->container_item_slot = container_item;
                    result_it->update_guide = query_it->setting_guide;
                    result_it->update_guide |= result_it->eUpdateGuide_Insert;
                };
                if (query_it->sum_of_items == query_it->number_of_items)
                {
                    ++searched_items;
                    break;
                }
                continue;
            }
            // (CHANGES) (f100525.4L) the query of a 'InverseSelect' is
            // based on a obtain function directly or indirectly(reserve)
            // this function is to support a long term item transaction like a shop transaction
            if (item_query->find_empty_only_) {
                continue;
            }
            // f100904.4L
            if (sort_state_table.option_check_empty_) {
                continue;
            }
            // (CHANGES) (f100525.4L) move a lock check block
            // to prevent a search a locking state on a reserved space
            if (container_item->IsBlocked() ||
                container_item->IsUserBlocked()) // no-check LaxLock
            {
                sort_state_node->check_cache_ |= sort_state_table.LockedState;
                continue;
            }

            ItemQuery::ResultInfo::Node* const result_it = &item_query->result_info_.nodes[index];
            const POSTYPE container_item_num = container_item->GetNum();

            if (query_it->item_code != container_item_code) {
                continue;
            }
            if (query_it->number_of_items == query_it->sum_of_items) {
                continue;
            }
            // f100909.1L, add a rule about an item tag
            if (item_query->option_tag_.value != container_item->GetOptionTag().value) {
                continue;
            }
            //
            int max_overlap_num = query_it->item_info->m_byDupNum <= 1
                                ?  1 : query_it->item_info->m_byDupNum;
            if ((max_overlap_num == 1 && result_it->container_item_code != 0) ||
                !FlowControl::FCAssert(max_overlap_num >= container_item_num))
            {
                sort_state_node->check_cache_ |= sort_state_table.Skipped;
                continue;
            }
            ;   max_overlap_num -= container_item_num;
            ;   max_overlap_num -= result_it->number_of_selected_items; assert(max_overlap_num >= 0);
            if (max_overlap_num <= 0)
            {
                sort_state_node->check_cache_ |= sort_state_table.Skipped;
                continue;
            }
            if (result_it->number_of_selected_items == 0) {
                ++item_query->result_info_.number_of_results;
            }

            int remains = query_it->number_of_items - query_it->sum_of_items;
            { //alignment result
                if (max_overlap_num < remains)
                {
                    result_it->number_of_selected_items += max_overlap_num;
                    query_it->sum_of_items += max_overlap_num;
                    sum_of_selected_items += max_overlap_num;
                }
                else
                {
                    result_it->number_of_selected_items += remains;
                    query_it->sum_of_items += remains;
                    sum_of_selected_items += remains;
                }
                result_it->position = static_cast<POSTYPE>(index);
                result_it->container_item_code = query_it->item_code;
                result_it->query_item_slot = query_it->item_slot;
                result_it->container_item_slot = container_item;
                result_it->update_guide = query_it->setting_guide;
                result_it->update_guide |= result_it->eUpdateGuide_Update;
            };
            if (query_it->sum_of_items == query_it->number_of_items)
            {
                ++searched_items;
                break;
            }
        }
        // already found state = if (search_index != item_query->query_info_.number_of_queries)
    }
    item_query->result_info_.sum_of_selected_items += sum_of_selected_items;
    // query_result.result_of_contains has only the number of 1st item to support custom logic
    query_result->result_of_inverse_select = item_query->result_info_.sum_of_selected_items;
    return true;
}

//==================================================================================================


