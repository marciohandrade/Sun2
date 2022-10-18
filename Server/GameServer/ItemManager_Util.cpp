#include "stdafx.h"
#include "ItemManager.h"
#include "ItemManager_Util.h"
//
#include <SCItemSlotContainer.h>
#include <PacketStruct_DG.h>

#include <ItemInfoParser.h>
#include <ItemTypeList.h>
//
#include "Player.h"

//==================================================================================================
static SCItemSlot working_item_slots_static_[INVALID_POSTYPE_VALUE];

// (CHANGES) (f100325.2L) added by _NA000023_090323_ENHANCED_INVENTORY_SLOT_FUNCTIONALITY_ \
// and other 'CheckInsertItem's are removed argument initial value
int ItemManager::CheckInsertItem(SLOTIDX slot_index, SLOTCODE item_code, int number_of_items)
{
    using nsSlot::ItemQuery;
    ItemQuery select_suplus_query;
    if (ItemQuery::MakeQueryInverseSelect(&select_suplus_query, player_, slot_index) == false) {
        return 0;
    }
    int registered_index = select_suplus_query.AddArg_ItemCode(item_code, number_of_items);
    if (registered_index < 0) {
        return 0; // assert(registered_index >= 0 && registered_index == 0);
    }
    //
    const nsSlot::ItemQueryResult result = Query(&select_suplus_query);
    // (CHANGES) (f100525.2L) remove a condition check to compatible a custom usage
    //if (result.result_of_inverse_select != number_of_items) {
    //    return 0;
    //}
    //return number_of_items;
    // (SPEC) fail    = { result.result_of_inverse_select != number_of_items }
    //        success = { result.result_of_inverse_select == number_of_items }
    return result.result_of_inverse_select;
}

//==================================================================================================

// The interface to update query
bool ItemManager::ObtainAllKindOfItemByQuery(const nsSlot::ItemQuery& queried_query)
{
    using nsSlot::ItemQuery;
    // prerequisite
    if (!FlowControl::FCAssert(queried_query.query_type_ == nsSlot::eItemQuery_InverseSelect)) {
        return false;
    }
    //
    const ItemQuery::ResultInfo& result_info = queried_query.result_info();
    const int number_of_result = result_info.number_of_results;
    assert(number_of_result != 0);
    //
    static SCItemSlot making_item_static_;
    // up-casted, to avoid ambiguous name conflict
    SCSlotContainer* const item_container = queried_query.item_container_;
    const KindOfItemTag option_tag = queried_query.GetItemTag(); //copy
    const ItemQuery::ResultInfo::Node* result_it = result_info.nodes,
                                     * result_end = &result_info.nodes[_countof(result_info.nodes)];
    for (int countdown = number_of_result; countdown && result_it != result_end; ++result_it)
    {
        if (result_it->container_item_code == 0 ||
            result_it->number_of_selected_items == 0)
        {
            continue;
        }
        --countdown;

        if (const SCItemSlot* queried_item_slot = result_it->query_item_slot)
        { // queried by item slot, item slot attributes are the points to be considered.
            if (result_it->update_guide & result_it->eUpdateGuide_Insert)
            {
                // copy slot to prevent disturb input argument,
                // fix up incorrect event problem that related on the SetNum 
                const bool need_copy = \
                    (result_it->update_guide & result_it->eUpdateGuide_ConstantSlot) != 0;
                if (need_copy) {
                    making_item_static_ = *queried_item_slot;
                }
                SCItemSlot* making_item = need_copy
                                        ?   &making_item_static_
                                        :   const_cast<SCItemSlot*>(queried_item_slot);
                making_item->SetNum(static_cast<POSTYPE>(result_it->number_of_selected_items));
                // f100909.1L
                if (option_tag.value != option_tag.Tag_None) {
                    making_item->SetOptionTag(option_tag);
                }
                item_container->InsertSlot(result_it->position, *making_item);
            }
            else if (result_it->update_guide & result_it->eUpdateGuide_Update)
            {
                // boundary ...
                item_container->UpdateSlot(result_it->position, result_it->number_of_selected_items);
                GAMELOG->LogItem(ITEM_OVERLAPPED_ITEM, player_,
                                 result_it->container_item_slot,
                                 result_it->number_of_selected_items);
            }
        }
        else
        {
            if (result_it->update_guide & result_it->eUpdateGuide_Insert)
            {
                making_item_static_.Clear();
                making_item_static_.SetCode(result_it->container_item_code);
                making_item_static_.SetNum(POSTYPE(result_it->number_of_selected_items));
                // f100909.1L
                if (option_tag.value != option_tag.Tag_None) {
                    making_item_static_.SetOptionTag(option_tag);
                }
                item_container->InsertSlot(result_it->position, making_item_static_);
                // boundary
            }
            else if (result_it->update_guide & result_it->eUpdateGuide_Update)
            {
                // boundary
                item_container->UpdateSlot(result_it->position, result_it->number_of_selected_items);
                GAMELOG->LogItem(ITEM_OVERLAPPED_ITEM, player_,
                                 result_it->container_item_slot,
                                 result_it->number_of_selected_items);
            }
        }
    }

    return true;
}

// CHANGES: f100327.5L, '_Obtain' deleted, and changed replaced mechanism

//==================================================================================================

// (CHANGES) (f100325.2L) added by _NA000023_090323_ENHANCED_INVENTORY_SLOT_FUNCTIONALITY_ \
// and other 'ExistItem's are removed argument initial value
// @returns
//  specification
//  - result == item_num : SUCCEED
//    result == 0 : failed, can't find item
//    result <  item_num : failed, find item, but don't satisfied by required number
//    result >= item_num : success, the current container has grater than the checking number of items
int ItemManager::ExistItem(SLOTIDX slot_index, SLOTCODE item_code, int item_num)
{
    using nsSlot::ItemQuery;
    ItemQuery item_query;
    if (ItemQuery::MakeQuerySelect(&item_query, player_, slot_index) == false) {
        return 0;
    }
    int registered_index = item_query.AddArg_ItemCode(item_code, item_num);
    if (registered_index) {
        return 0;
    }
    const nsSlot::ItemQueryResult result = Query(&item_query);
    // if (result.result_of_select != item_num) --> not satisfied
    return result.result_of_select;
}

//==================================================================================================

void ItemManager::Delete(SCSlotContainer* slot_container,
                         const POS_INFO* IN pos_info_array, POSTYPE IN pos_info_num)
{
    // add boundary checker
    const int max_number_of_slots = slot_container->GetMaxSlotNum();
    for (int i = 0; i < pos_info_num; ++i)
    {
        const POS_INFO* pos_info = &pos_info_array[i];
        const POSTYPE pos = pos_info->m_pos;
        const bool is_valid_state_that_has_in_of_range_and_exist_item = \
            pos < max_number_of_slots && slot_container->IsEmpty(pos) == false;
        if (!FlowControl::FCAssert(is_valid_state_that_has_in_of_range_and_exist_item)) {
            continue;
        }
        const SCItemSlot& container_item = static_cast<SCItemSlot&>(slot_container->GetSlot(pos));
        const int container_item_num = container_item.GetNum();
        const int require_delete_num = pos_info->m_num;
        // changes boundary concept \
        // accept input number can grater than the number of item in the slot
        if (container_item_num <= require_delete_num) {
            slot_container->DeleteSlot(pos, NULL);
        }
        else {
            slot_container->UpdateSlot(pos, -require_delete_num);
        }
    }
}

void ItemManager::Delete(SCSlotContainer* slot_container,
                         const POS_INFO_EX* IN pos_info_array, POSTYPE IN pos_info_num)
{
    // add boundary checker
    const int max_number_of_slots = slot_container->GetMaxSlotNum();
    for (int index = 0; index < int(pos_info_num); ++index)
    {
        const POSTYPE pos = pos_info_array[index].m_pos;
        if (!FlowControl::FCAssert(pos < max_number_of_slots &&
                                   slot_container->IsEmpty(pos) == false))
        {
            continue;
        }
        SCItemSlot& container_item = static_cast<SCItemSlot&>(slot_container->GetSlot(pos));
        const int container_item_num = container_item.GetNum();
        const int require_delete_num = pos_info_array[index].m_num;
        // changes boundary concept \
        // accept input number can grater than the number of item in the slot
        if (container_item_num <= require_delete_num) {
            slot_container->DeleteSlot(pos, NULL);
        }
        else {
            slot_container->UpdateSlot(pos, -require_delete_num);
        }
    }
}

//==================================================================================================
RC::eITEM_RESULT
ItemManager::DeleteOneKindOfItem(const SLOTCODE del_item_code, 
                                 const WORD number_of_del_item,
                                 INVEN_DELETE_TOTAL_INFO* OUT inven_delete_total_info)
{
    enum { kPosTypeMax = UCHAR_MAX };

    SLOTCODE item_codes[kPosTypeMax];
    ::ZeroMemory(item_codes, sizeof(item_codes));
    
    POSTYPE item_nums[kPosTypeMax];
    ::ZeroMemory(item_nums, sizeof(item_nums));

    BOOST_STATIC_ASSERT(sizeof(POSTYPE) == sizeof(BYTE));
    BOOST_STATIC_ASSERT(_countof(item_codes) == _countof(item_nums));

    int num_of_kinds = number_of_del_item / kPosTypeMax;
    if (num_of_kinds > _countof(item_codes) || num_of_kinds > _countof(item_nums))
    {
        SUNLOG(
            eCRITICAL_LOG, 
            _T("|%s|number of kinds is overflow.|number of kinds = %d, max kinds = %d|"), 
            __FUNCTION__, 
            num_of_kinds, 
            _countof(item_codes)
            );
        return RC::RC_ITEM_FAILED;
    }

    int remainder = number_of_del_item % kPosTypeMax;
    if (remainder != 0)
    {
        if (num_of_kinds == kPosTypeMax)
        {
            SUNLOG(
                eCRITICAL_LOG, 
                _T("|%s|number of kinds is overflow.|number of kinds = %d, max kinds = %d|"), 
                __FUNCTION__, 
                num_of_kinds, 
                _countof(item_codes)
                );
            return RC::RC_ITEM_FAILED;;
        }
        ++num_of_kinds;
    }

    for (int i = 0; i < num_of_kinds; ++i)
    {
        item_codes[i] = del_item_code;
        if (remainder != 0 && (i == (num_of_kinds - 1)))
        {
            item_nums[i] = (POSTYPE)remainder;
        }
        else
        {
            item_nums[i] = kPosTypeMax;
        }
    }

    return DeleteAllKindOfItem(num_of_kinds, item_codes, item_nums, inven_delete_total_info);;
}

RC::eITEM_RESULT
ItemManager::DeleteAllKindOfItem(POSTYPE number_of_kinds,
                                 const SLOTCODE* IN item_code_array,
                                 const POSTYPE* IN item_num_array, 
                                 INVEN_DELETE_TOTAL_INFO* OUT inven_delete_total_info,
                                 SCItemSlot* const deleted_item_slot)
{
    using nsSlot::ItemQuery;
    ItemQuery delete_query;
    if (ItemQuery::MakeQuerySelect(&delete_query, player_, SI_INVENTORY) == false) {
        return RC::RC_ITEM_INVALIDSTATE; // need check to valid result
    }
    int registered_index = delete_query.AddArg_Custom(number_of_kinds,
                                                      item_code_array, item_num_array);
    if (registered_index < 0) {
        return RC::RC_ITEM_INVALID_VALUE;
    }
    // assert(registered_index == number_of_kinds);
    const nsSlot::ItemQueryResult result = Query(&delete_query);
    const ItemQuery::QueryInfo& query_info = delete_query.query_info();
    if (result.result_of_select != query_info.sum_of_queried_items) {
        return RC::RC_ITEM_NOTEXIST_ITEM;
    }
    // debug routine
    delete_query.CheckResultValidatation_Custom(number_of_kinds, item_code_array, item_num_array);
    //
    delete_query.MakeSerializedStream(inven_delete_total_info);
    // (CHANGES) remove rollback mechanism, \
    // because Query_Delete has an implicit transaction related with atomic.
    // CHANGES, f100831.1L, add a missing delete operation
    // that the above routines have a search and build an information without a delete process.
    ;{  //delete commit process
        SCItemSlotContainer* item_container = delete_query.item_container();
        const int number_of_deleting_items = inven_delete_total_info->m_Count;
        const INVEN_DELETE_TOTAL_INFO::SLOT_TYPE* delete_it = inven_delete_total_info->m_Slot;
        SCItemSlot* deleted_item_slot_it = deleted_item_slot;
        for (int index = 0; index < number_of_deleting_items; ++index, ++delete_it)
        {
            const POSTYPE pos = delete_it->m_Pos;
            const SCItemSlot& container_item = \
                static_cast<SCItemSlot&>(item_container->GetSlot(pos));
            const int container_item_num = container_item.GetNum();
            const int require_delete_num = delete_it->m_Num;

            if (deleted_item_slot_it) {
                deleted_item_slot_it->Copy(item_container->GetSlot(pos));
                ++deleted_item_slot_it;
            }
            // changes boundary concept \
            // accept input number can grater than the number of item in the slot
            if (container_item_num <= require_delete_num) {
                item_container->DeleteSlot(pos, NULL);
            }
            else {
                item_container->UpdateSlot(pos, -require_delete_num);
            }
        } //end 'for'
    };

    return RC::RC_ITEM_SUCCESS;
}

// 인벤 전체 삭제
//(lst1024)(090218) 전체 삭제 못할시 롤백 에서 삭제 가능만 삭제로 변경.
BOOL ItemManager::DeleteAllInvenItem(INVEN_DELETE_TOTAL_INFO* OUT inven_delete_total_info,
                                     bool partial_delete)
{
    __UNUSED(partial_delete);
    static POS_INFO_EX pos_info[INVEN_DELETE_TOTAL_INFO::MAX_SLOT_NUM];

    SCSlotContainer* inventory = GetItemSlotContainer(SI_INVENTORY);
    POSTYPE next_copy_pos = 0;
    inven_delete_total_info->m_Count = 0;

    for (POSTYPE i = 0; (i < inventory->GetMaxSlotNum()) && (0 != inventory->GetSlotNum()); ++i)
    {
        if (!inventory->IsEmpty(i))
        {
            SCItemSlot& scslot = (SCItemSlot&)inventory->GetSlot(i);

            if (INVEN_DELETE_TOTAL_INFO::MAX_SLOT_NUM == next_copy_pos) {
                break;
            }
            if (scslot.IsBlocked() || scslot.IsLocked()) {
                continue;
            }
            // (CHANGES) change to more generic code 'IsPetLocked' & 'IsRiderLocked' ...
            if (scslot.IsUserBlocked()) {
                continue;
            }

            pos_info[next_copy_pos].m_pos = scslot.GetPos();
            pos_info[next_copy_pos].m_num = (POSTYPE)scslot.GetNum();
            scslot.CopyOut(pos_info[next_copy_pos++].ItemStream);
            inventory->DeleteSlot(i, NULL);
        }   
    }

    inven_delete_total_info->m_Count = next_copy_pos;
    for (POSTYPE i = 0; i < next_copy_pos; ++i)
    {
        inven_delete_total_info->m_Slot[i].m_Pos = pos_info[i].m_pos;
        inven_delete_total_info->m_Slot[i].m_Num = pos_info[i].m_num;
    }
    return true;
}

//==================================================================================================

RC::eITEM_RESULT
ItemManager::Obtain(SLOTCODE item_code, int item_num,
                    INVENTORY_TOTAL_INFO* OUT inven_total_info)
{
    using nsSlot::ItemQuery;

    inven_total_info->m_InvenCount = 0;
    inven_total_info->m_TmpInvenCount = 0;
    if (ValidState() == false) {
        return RC::RC_ITEM_INVALIDSTATE;
    }
    if (item_code == 0 || item_num == 0)
    {
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"] failed!|Code=%d|Number=%d|", item_code, item_num);
        return RC::RC_ITEM_INVALIDSTATE;
    }

    ItemQuery select_suplus_query;
    if (ItemQuery::MakeQueryInverseSelect(&select_suplus_query, player_, SI_INVENTORY) == false) {
        return RC::RC_ITEM_INVALIDSTATE;
    }
    int registered_index = select_suplus_query.AddArg_ItemCode(item_code, item_num);
    if (registered_index < 0) {
        return RC::RC_ITEM_INVALIDSTATE;
    }
    //
    const nsSlot::ItemQueryResult result = Query(&select_suplus_query);
    if (result.result_of_inverse_select != item_num) {
        return RC::RC_ITEM_HAVENOTSPACE;
    }
    if (ObtainAllKindOfItemByQuery(select_suplus_query) == false) {
        return RC::RC_ITEM_INVALIDSTATE;
    }
    select_suplus_query.MakeSerializedStream(inven_total_info);
    return RC::RC_ITEM_SUCCESS;
}

RC::eITEM_RESULT
ItemManager::Obtain(const SCItemSlot& IN required_item_slot, int item_num,
                    INVENTORY_TOTAL_INFO* OUT inven_total_info)
{
    using nsSlot::ItemQuery;

    inven_total_info->m_InvenCount = 0;
    inven_total_info->m_TmpInvenCount = 0;
    if (ValidState() == false) {
        return RC::RC_ITEM_INVALIDSTATE;
    }
    const SLOTCODE item_code = required_item_slot.GetItemCode();
    if (item_code == 0 || item_num == 0)
    {
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"] failed!|Code=%d|Number=%d|", item_code, item_num);
        return RC::RC_ITEM_INVALIDSTATE;
    }

    ItemQuery select_suplus_query;
    if (ItemQuery::MakeQueryInverseSelect(&select_suplus_query, player_, SI_INVENTORY) == false) {
        return RC::RC_ITEM_INVALIDSTATE;
    }
    int registered_index = select_suplus_query.AddArg_ItemSlot(required_item_slot, item_num, false);
    if (registered_index) {
        return RC::RC_ITEM_INVALIDSTATE;
    }
    //
    const nsSlot::ItemQueryResult result = Query(&select_suplus_query);
    if (result.result_of_inverse_select != item_num) {
        return RC::RC_ITEM_HAVENOTSPACE;
    }
    if (ObtainAllKindOfItemByQuery(select_suplus_query) == false) {
        return RC::RC_ITEM_INVALIDSTATE;
    }
    select_suplus_query.MakeSerializedStream(inven_total_info);
    return RC::RC_ITEM_SUCCESS;
}

RC::eITEM_RESULT
ItemManager::Obtain(const SCItemSlot& IN required_item_slot, int item_num,
                    JUST_INVENTORY_TOTAL_INFO* OUT inven_total_info)
{
    using nsSlot::ItemQuery;

    if (ValidState() == false) {
        return RC::RC_ITEM_INVALIDSTATE;
    }
    const SLOTCODE item_code = required_item_slot.GetItemCode();
    if (item_code == 0 || item_num == 0)
    {
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"] failed!|Code=%d|Number=%d|", item_code, item_num);
        return RC::RC_ITEM_INVALIDSTATE;
    }

    ItemQuery select_suplus_query;
    if (ItemQuery::MakeQueryInverseSelect(&select_suplus_query, player_, SI_INVENTORY) == false) {
        return RC::RC_ITEM_INVALIDSTATE;
    }
    int registered_index = select_suplus_query.AddArg_ItemSlot(required_item_slot, item_num, false);
    if (registered_index) {
        return RC::RC_ITEM_INVALIDSTATE;
    }
    //
    const nsSlot::ItemQueryResult result = Query(&select_suplus_query);
    if (result.result_of_inverse_select != item_num) {
        return RC::RC_ITEM_HAVENOTSPACE;
    }
    if (ObtainAllKindOfItemByQuery(select_suplus_query) == false) {
        return RC::RC_ITEM_INVALIDSTATE;
    }
    select_suplus_query.MakeSerializedStream(inven_total_info);
    return RC::RC_ITEM_SUCCESS;
}

RC::eITEM_RESULT
ItemManager::ObtainAllKindOfItemShared(nsSlot::ItemQuery* const obtain_query,
                                       POSTYPE number_of_kinds,
                                       const SLOTCODE* item_code_array,
                                       const POSTYPE* item_num_array,
                                       const BYTE* enchant_array,
                                       const BYTE* item_type_array,
                                       const BYTE* rank_array,
                                       const DWORD* datetime_array, 
                                       const WORD* zard_option_array)
{
    const SLOTCODE* const item_code_array_begin = item_code_array;
    const POSTYPE* const item_num_array_begin = item_num_array;
    //
    int index = 0;
    for (SCItemSlot* slot_it = working_item_slots_static_;
         index < number_of_kinds;
         ++index, ++slot_it, ++item_code_array, ++item_num_array)
    {
        if (*item_code_array == 0 || *item_num_array == 0) {
            return RC::RC_ITEM_INVALIDSTATE;
        }
        slot_it->Clear();
        slot_it->SetCode(*item_code_array);
        const nsSlot::ItemRuleInfo& item_rule_info = slot_it->GetItemRuleInfo();
        if (item_rule_info.is_valid_item == 0) {
            return RC::RC_ITEM_INVALIDSTATE;
        }
        // add options
        if (enchant_array && item_rule_info.IsEnableEnchant()) {
            slot_it->SetEnchant(*enchant_array);
        }

    #ifdef _NA_000251_20100727_SOCKET_SYSTEM
        // EP2 에서는 랭크 정보를 소켓 개수로 사용한다.
        if (rank_array != NULL)
        {
            const BYTE number_of_socket = *rank_array;
            slot_it->CreateSocketItem(number_of_socket, false);
        }

        // 쟈드 아이템일 경우 쟈드 옵션을 설정한다.
        if (zard_option_array && slot_it->IsZardItem())
        {
            const SOCKETINDEX zard_option_index = *zard_option_array;
            slot_it->SetExtendData(zard_option_index);
        }
    #else
        if (rank_array && item_rule_info.IsEnableRank()) {
            slot_it->SetRank(static_cast<eRANK>(*rank_array));
        }
    #endif // _NA_000251_20100727_SOCKET_SYSTEM

        if (datetime_array && item_rule_info.IsEnableTimer()) {
            slot_it->SetDateTime(*datetime_array);
        }
        if (item_type_array) {
            nsSlot::ItemTypeChanger::ChangeItemByType(slot_it, *item_type_array);
        }
        int registered_index = obtain_query->AddArg_ItemSlot(*slot_it, *item_num_array, true);
        if (registered_index < 0) {
            return RC::RC_ITEM_INVALIDSTATE;
        }
        if (enchant_array) {
            ++enchant_array;
        }
        if (rank_array) {
            ++rank_array;
        }
        if (datetime_array) {
            ++datetime_array;
        }
        if (item_type_array) {
            ++item_type_array;
        }
        if (zard_option_array) {
            ++zard_option_array;
        }
    }
    // current corrupted variables = { item_code_array, item_num_array,
    //                                 enchant_array, divine_array,
    //                                 rank_array, -- EP2:unused
    //                                 datetime_array,
    //                                 zard_option_array -- EP:added };

    const nsSlot::ItemQueryResult result = Query(obtain_query);
    // RESOLVED(WAVERIX) need more thinking about result handling
    const nsSlot::ItemQuery::QueryInfo& query_info = obtain_query->query_info();
    if (result.result_of_select != query_info.sum_of_queried_items) {
        return RC::RC_ITEM_NOTEXIST_ITEM;
    }
    // for debug routing
    obtain_query->CheckResultValidatation_Custom(number_of_kinds,
                                                 item_code_array_begin, item_num_array_begin);
    if (ObtainAllKindOfItemByQuery(*obtain_query) == false) {
        return RC::RC_ITEM_INVALIDSTATE;
    }

    return RC::RC_ITEM_SUCCESS;
}

#ifdef _NA_0_20110613_GAME_SHOP_RENEWAL
// NOTE: f111006.3L, changes the method specification.
// this method support single item purchase action for GameShop service
// ps : 게임샵에서만 사용한다.
RC::eITEM_RESULT ItemManager::ObtainAtEmptySlot(
    const KindOfItemTag item_tag, 
    const SLOTCODE item_code, 
    const POSTYPE item_num,
    const BYTE enchant_count,
    const BYTE item_type,
    const BYTE socket_count,
    const DWORD delete_time,
    INVENTORY_TOTAL_INFO* OUT inven_total_info)
{
    inven_total_info->m_InvenCount = 0;
    inven_total_info->m_TmpInvenCount = 0;

//     if (ValidState() == false) {
//         return RC::RC_ITEM_INVALIDSTATE;
//     }

    if ((player_ == NULL) || (player_->GetField() == NULL))
        return RC::RC_ITEM_INVALIDSTATE;

    SCSlotContainer* inventory = GetItemSlotContainer(SI_INVENTORY);
    //                          (Ref100330.7L)
    if (inventory->GetEmptySlotNum() < 1) {
        return RC::RC_ITEM_HAVENOTSPACE;
    }

    nsSlot::ItemQuery select_suplus_query;
    if (!nsSlot::ItemQuery::MakeQueryInverseSelect(&select_suplus_query, player_, SI_INVENTORY)) {
        return RC::RC_ITEM_INVALIDSTATE;
    };
    // CHANGES: f111006.3L, added the independent slot acquisition mode
    // to support re-generated serial issuing.
    select_suplus_query.ChangeToFindEmptySlotOnlyMode();
    if (item_tag.value != KindOfItemTag::Tag_None)
    {
        select_suplus_query.ChangeToAcquireItemTagOption(item_tag);
    }
    
    RC::eITEM_RESULT item_result = ObtainAllKindOfItemShared(&select_suplus_query,
                                                            1,
                                                            &item_code, 
                                                            &item_num,
                                                            &enchant_count,
                                                            &item_type,
                                                            &socket_count,
                                                            &delete_time,
                                                            NULL);
    if (item_result != RC::RC_ITEM_SUCCESS) {
        return item_result;
    }

    select_suplus_query.MakeSerializedStream(inven_total_info);
    return RC::RC_ITEM_SUCCESS;
}

#endif //_NA_0_20110613_GAME_SHOP_RENEWAL

// 1. 여러종류의 아이템을 개수 별로
// 2. 옵션(인첸트,디바인,랭크포함)을 포함
// 3. 인벤토리,보조창고에 얻을 수 있다
#ifdef _NA_006801_20130705_RENEWAL_RANKINGSYSTEM
RC::eITEM_RESULT
ItemManager::ObtainAllKindOfItem(POSTYPE number_of_kinds,
                                 const SLOTCODE* IN item_code_array,
                                 const POSTYPE* IN item_num_array, 
                                 const BYTE* IN enchant_array,
                                 const BYTE* IN item_type_array,
                                 const BYTE* IN rank_array, 
                                 INVENTORY_TOTAL_INFO* OUT inven_total_info,
                                 const DWORD* IN datetime_array /*= NULL*/)
#else
RC::eITEM_RESULT
ItemManager::ObtainAllKindOfItem(POSTYPE number_of_kinds,
                                 const SLOTCODE* IN item_code_array,
                                 const POSTYPE* IN item_num_array, 
                                 const BYTE* IN enchant_array,
                                 const BYTE* IN item_type_array,
                                 const BYTE* IN rank_array, 
                                 INVENTORY_TOTAL_INFO* OUT inven_total_info)
#endif //_NA_006801_20130705_RENEWAL_RANKINGSYSTEM
{
    inven_total_info->m_InvenCount = 0;
    inven_total_info->m_TmpInvenCount = 0;
    if (ValidState() == false) {
        return RC::RC_ITEM_INVALIDSTATE;
    }
    SCSlotContainer* inventory = GetItemSlotContainer(SI_INVENTORY);
    //                          (Ref100330.7L)
    if (number_of_kinds == 0 || inventory->GetEmptySlotNum() < number_of_kinds) {
        return RC::RC_ITEM_HAVENOTSPACE;
    }

    nsSlot::ItemQuery select_suplus_query;
    if (!nsSlot::ItemQuery::MakeQueryInverseSelect(&select_suplus_query, player_, SI_INVENTORY)) {
        return RC::RC_ITEM_INVALIDSTATE;
    }

#ifdef _NA_006801_20130705_RENEWAL_RANKINGSYSTEM

    RC::eITEM_RESULT item_result = ObtainAllKindOfItemShared(&select_suplus_query,
                                                             number_of_kinds,
                                                             item_code_array, item_num_array,
                                                             enchant_array,
                                                             item_type_array,
                                                             rank_array, datetime_array, NULL);
#else
    RC::eITEM_RESULT item_result = ObtainAllKindOfItemShared(&select_suplus_query,
                                                             number_of_kinds,
                                                             item_code_array, item_num_array,
                                                             enchant_array,
                                                             item_type_array,
                                                             rank_array, NULL, NULL);
#endif //_NA_006801_20130705_RENEWAL_RANKINGSYSTEM

    if (item_result != RC::RC_ITEM_SUCCESS) {
        return item_result;
    }

    select_suplus_query.MakeSerializedStream(inven_total_info);
    return RC::RC_ITEM_SUCCESS;
}

RC::eITEM_RESULT
ItemManager::ObtainAllKindOfItem(POSTYPE number_of_kinds,
                                 const SLOTCODE* IN item_code_array,
                                 const POSTYPE* IN item_num_array, 
                                 JUST_INVENTORY_TOTAL_INFO* OUT inven_total_info)
{
    using nsSlot::ItemQuery;

    inven_total_info->m_Count = 0;
    if (ValidState() == false) {
        return RC::RC_ITEM_INVALIDSTATE;
    }
    SCSlotContainer* inventory = GetItemSlotContainer(SI_INVENTORY);
    //                          (Ref100330.7L)
    if (number_of_kinds == 0 || inventory->GetEmptySlotNum() < number_of_kinds) {
        return RC::RC_ITEM_HAVENOTSPACE;
    }

    ItemQuery select_suplus_query;
    if (!nsSlot::ItemQuery::MakeQueryInverseSelect(&select_suplus_query, player_, SI_INVENTORY)) {
        return RC::RC_ITEM_INVALIDSTATE;
    }
    RC::eITEM_RESULT item_result = ObtainAllKindOfItemShared(&select_suplus_query,
                                                             number_of_kinds,
                                                             item_code_array, item_num_array,
                                                             NULL, NULL, NULL, NULL, NULL);
    if (item_result != RC::RC_ITEM_SUCCESS) {
        return item_result;
    }

    select_suplus_query.MakeSerializedStream(inven_total_info);
    return RC::RC_ITEM_SUCCESS;
}

// 1. 여러종류의 아이템을 개수 별로
// 2. 인벤토리,보조창고에 얻을 수 있다
RC::eITEM_RESULT
ItemManager::ObtainAllKindOfItem(POSTYPE number_of_kinds,
                                 const SLOTCODE* IN item_code_array,
                                 const POSTYPE* IN item_num_array, 
                                 INVENTORY_TOTAL_INFO* OUT inven_total_info)
{
    inven_total_info->m_InvenCount = 0;
    inven_total_info->m_TmpInvenCount = 0;
    if (ValidState() == false) {
        return RC::RC_ITEM_INVALIDSTATE;
    }
    SCSlotContainer* inventory = GetItemSlotContainer(SI_INVENTORY);
    //                          (Ref100330.7L)
    if (number_of_kinds == 0 || inventory->GetEmptySlotNum() < number_of_kinds) {
        return RC::RC_ITEM_HAVENOTSPACE;
    }

    nsSlot::ItemQuery select_suplus_query;
    if (!nsSlot::ItemQuery::MakeQueryInverseSelect(&select_suplus_query, player_, SI_INVENTORY)) {
        return RC::RC_ITEM_INVALIDSTATE;
    }
    RC::eITEM_RESULT item_result = ObtainAllKindOfItemShared(&select_suplus_query,
                                                             number_of_kinds,
                                                             item_code_array, item_num_array,
                                                             NULL, NULL, NULL, NULL, NULL);
    if (item_result != RC::RC_ITEM_SUCCESS) {
        return item_result;
    }

    select_suplus_query.MakeSerializedStream(inven_total_info);
    return RC::RC_ITEM_SUCCESS;
}

//==================================================================================================

// 2006/12/10
// 새로 도입되는 obtain관련 util함수!!!
// 한가지 종류의 아이템 코드 + 아이템 개수 + 유료화 아이템의 날짜 제한
// if success, ReturnValue == item_num
// if failed, ReturnValue < item_num : 넣은 개수 값 return
// 한가지의 아이템을 넣을 곳을 찾아서 잠금 처리를 한다.
int ItemManager::ReservedSpace(SCSlotContainer* slot_container, 
                               const SLOTCODE item_code IN, const POSTYPE item_num IN, 
                               POSTYPE number_of_locked_info IN,
                               LOCKED_INFO* locked_info INOUT,
                               POSTYPE* number_of_used_locked_info INOUT)
{
    ASSERT(0 != item_num && 0 != item_code);
    ASSERT(NULL != slot_container);
    if (0 == item_num || 0 == item_code) {
        return 0;
    }
    if (NULL == slot_container) {
        return 0;
    }
    if (0 == number_of_locked_info || !locked_info || !number_of_used_locked_info) {
        return 0;
    }
    static SCItemSlot referenced_item;
    referenced_item.Clear();
    referenced_item.SetCode(item_code);

    int insufficient_remains = item_num;
    // NOTE: remove overlapping check because difficult level of item tagging info control is hard.
    // the version of 1004 is defined as a rule of find empty slot
/*
    if (SCItemSlot::IsOverlap(item_code))
    {
        // 기존의 겹치는 아이템을 찾는다.
        for (POSTYPE pos = 0; pos < slot_container->GetMaxSlotNum(); ++pos)
        {
            if (slot_container->IsLocked(pos)) continue;

            if (!slot_container->IsEmpty(pos))
            {
                SCItemSlot& container_item = (SCItemSlot&)slot_container->GetSlot(pos);
                if (container_item.GetItemCode() != referenced_item.GetItemCode()) {
                    continue;
                }
                // NOTE: f100909.1L, it is a different item \
                // even if the item code of a tagged item is equal to it of a not tagged item.
                if (container_item.GetOptionTag().value != referenced_item.GetOptionTag().value) {
                    continue;
                };
                if (container_item.GetNum() < container_item.GetMaxNum())
                {
                    if (container_item.GetNum() == 0)
                    {
                        SUNLOG(eCRITICAL_LOG, "[Error]:item_num=0!!");
                        container_item.SetNum(1);
                    }
                    // 잠금
                    container_item.SetLock(true);

                    // 넣는 개수만큼
                    locked_info[*number_of_used_locked_info].m_SlotIndex = slot_container->GetSlotIdx();  
                    locked_info[*number_of_used_locked_info].m_pos = pos; 
                    locked_info[*number_of_used_locked_info].m_code = container_item.GetItemCode();
                    if (insufficient_remains + container_item.GetNum() <= container_item.GetMaxNum())
                    {
                        locked_info[*number_of_used_locked_info].m_num = insufficient_remains;
                    }
                    else
                    {
                        locked_info[*number_of_used_locked_info].m_num = \
                            (container_item.GetMaxNum() - container_item.GetNum());
                    }
                    ++(*number_of_used_locked_info);

                    insufficient_remains -= (container_item.GetMaxNum() - container_item.GetNum());

                    if (insufficient_remains <= 0) {
                        return item_num;
                    }
                    if (number_of_locked_info <= *number_of_used_locked_info) {
                        return item_num - insufficient_remains;
                    }
                }
            }
        }
    }
*/
    for (POSTYPE pos = 0; pos < slot_container->GetMaxSlotNum(); ++pos)
    {
        // 잠금 처리를 위해 필요
        if (slot_container->IsLocked(pos)) {
            continue;
        }
        if (slot_container->IsEmpty(pos))
        {
            // 빈공간
            // 잠금
            slot_container->SetLock(pos, true);

            locked_info[*number_of_used_locked_info].m_SlotIndex = slot_container->GetSlotIdx();  
            locked_info[*number_of_used_locked_info].m_pos = pos;
            locked_info[*number_of_used_locked_info].m_code = referenced_item.GetItemCode();
            if (insufficient_remains > referenced_item.GetMaxNum())
            {
                locked_info[*number_of_used_locked_info].m_num = referenced_item.GetMaxNum();
            }
            else
            {
                locked_info[*number_of_used_locked_info].m_num = insufficient_remains;
            }
            ++(*number_of_used_locked_info);

            insufficient_remains -= referenced_item.GetMaxNum();
            if (insufficient_remains <= 0) {
                return item_num;
            }
            if (number_of_locked_info <= *number_of_used_locked_info) {
                return item_num - insufficient_remains;
            }
        }
    }

    return item_num - insufficient_remains;
}

// 여러가지 종류의 아이템 코드 + 아이템 개수 + 유료화 아이템의 날짜 제한
// 가정 : 유료화 날짜 혹은 옵션 정보 둘중 하나 사용함
// 성공이면, 락된 슬롯 정보 넘겨줌
// 실패면 락건거 롤백함
RC::eITEM_RESULT
ItemManager::ReserveSpaceAllKindOfItem(POSTYPE number_of_kinds, 
                                       const SLOTCODE* item_code_array IN,
                                       const POSTYPE* item_num_array IN,
                                       const DWORD* datetime_array IN,
                                       const BYTE* enchant_array IN,
                                       const BYTE* item_type_array IN, // (f100525.1L)
                                       const BYTE* rank_array IN,
                                       POSTYPE number_of_locked_info IN,
                                       LOCKED_INFO* locked_info OUT,
                                       POSTYPE* number_of_used_locked_info OUT)
{
    if (ValidState() == false) {
        return RC::RC_ITEM_INVALIDSTATE;
    }
    SCSlotContainer* inventory = GetItemSlotContainer(SI_INVENTORY);
    if (number_of_kinds == 0 ||
        inventory->GetEmptySlotNum() < number_of_kinds || // (Ref100330.7L)
        number_of_locked_info < number_of_kinds)
    {
        return RC::RC_ITEM_HAVENOTSPACE;
    }
    //
    nsSlot::ItemQuery select_suplus_query;
    if (!nsSlot::ItemQuery::MakeQueryInverseSelect(&select_suplus_query, player_, SI_INVENTORY)) {
        return RC::RC_ITEM_INVALIDSTATE;
    }
    select_suplus_query.ChangeToFindEmptySlotOnlyMode(); // (f100526.4L)
    //
    const SLOTCODE* const item_code_array_begin = item_code_array;
    const POSTYPE* const item_num_array_begin = item_num_array;
    const DWORD* const datetime_array_begin = datetime_array;
    //
    int index = 0;
    for (SCItemSlot* slot_it = working_item_slots_static_;
         index < number_of_kinds;
         ++index, ++slot_it, ++item_code_array, ++item_num_array)
    {
        if (*item_code_array == 0 || *item_num_array == 0) {
            return RC::RC_ITEM_INVALIDSTATE;
        }
        //
        slot_it->Clear();
        slot_it->SetCode(*item_code_array);
        const nsSlot::ItemRuleInfo& item_rule_info = slot_it->GetItemRuleInfo();
        if (item_rule_info.is_valid_item == 0) {
            return RC::RC_ITEM_INVALIDSTATE;
        }
        // add options
        if (enchant_array && item_rule_info.IsEnableEnchant()) {
            slot_it->SetEnchant(*enchant_array);
        }

    #ifndef _NA_000251_20100727_SOCKET_SYSTEM
        if (rank_array && item_rule_info.IsEnableRank()) {
            slot_it->SetRank(static_cast<eRANK>(*rank_array));
        }
    #endif // _NA_000251_20100727_SOCKET_SYSTEM

        if (datetime_array && item_rule_info.IsEnableTimer()) {
            slot_it->SetDateTime(*datetime_array);
        }
        if (item_type_array) {
            nsSlot::ItemTypeChanger::ChangeItemByType(slot_it, *item_type_array);
        }
        int registered_index = select_suplus_query.AddArg_ItemSlot(*slot_it, *item_num_array, true);
        if (registered_index < 0) {
            return RC::RC_ITEM_INVALIDSTATE;
        }
        if (enchant_array) {
            ++enchant_array;
        }
        if (rank_array) {
            ++rank_array;
        }
        if (datetime_array) {
            ++datetime_array;
        }
        if (item_type_array) {
            ++item_type_array;
        }
    }; //end 'for'
    //
    // current corrupted variables = { item_code_array, item_num_array,
    //                                 enchant_array, divine_array,
    //                                 rank_array, -- EP2:unused
    //                                 datetime_array };
    const nsSlot::ItemQueryResult result = Query(&select_suplus_query);
    // TODO(WAVERIX) need more thinking about result handling
    const nsSlot::ItemQuery::QueryInfo& query_info = select_suplus_query.query_info();
    if (result.result_of_select != query_info.sum_of_queried_items) {
        return RC::RC_ITEM_NOTEXIST_ITEM;
    }
    // for debug routing
    select_suplus_query.CheckResultValidatation_Custom(number_of_kinds,
                                                       item_code_array_begin, item_num_array_begin);
    //
    if (locked_info != NULL) {
        // CHANGES, f100902.2L, fix up usase of index out of range pointer.
        bool success = select_suplus_query.SetupLockedInfoWithLock(number_of_used_locked_info,
                                                                   number_of_locked_info,
                                                                   locked_info,
                                                                   number_of_kinds,
                                                                   datetime_array_begin);
        if (success == false) {
            return RC::RC_ITEM_HAVENOTSPACE;
        }
    };
    return RC::RC_ITEM_SUCCESS;
}

//==================================================================================================

RC::eITEM_RESULT
ItemManager::ReservedExtendDateItem(const SLOTCODE* extension_item_code IN,
                                    SLOTIDX slot_index, POSTYPE at_pos,
                                    LOCKED_INFO* locked_info OUT,
                                    POSTYPE* number_of_used_locked_info OUT)
{
    if (!ValidPos(slot_index, at_pos, true)) {
        return RC::RC_ITEM_INVALIDPOS;
    }
    if (NULL == locked_info || NULL == number_of_used_locked_info) {
        return RC::RC_ITEM_FAILED;
    }
    SCSlotContainer* slot_container = GetItemSlotContainer(slot_index);
    if (!slot_container) {
        return RC::RC_ITEM_INVALIDPOS;
    }

    SCItemSlot& expired_item_slot = (SCItemSlot&)slot_container->GetSlot(at_pos);
    const BASE_ITEMINFO* expired_equip_item_info = expired_item_slot.GetItemInfo();
    if (!expired_equip_item_info) {
        return RC::RC_ITEM_FAILED;
    }
    // 정액제(대기) 타입인지 체크
    if (!expired_equip_item_info->CanPeriodExtend()) {
        return RC::RC_ITEM_CANNOT_EXTEND_DATE;
    }
    // 기간이 다된 아이템만 기간 연장을 할 수 있다.
    if (!expired_item_slot.IsExpiredWaitTime()) {
        return RC::RC_ITEM_CANNOT_EXTEND_DATE;
    }
    //                                      (WAVERIX) (091018)
    // 무기 연장인지 방어구 연장인지 체크 + check Rider|Parts
    const BASE_ITEMINFO* extension_item_info = \
        ItemInfoParser::Instance()->GetItemInfo(extension_item_code[0]);
    if (!extension_item_info) {
        return RC::RC_ITEM_FAILED;
    }
    if (extension_item_info->m_wType == eITEMTYPE_EXTEND_DATE_WEAPON)
    {
        if (expired_equip_item_info->m_wEqPos != eEQUIPCONTAINER_WEAPON) {
            return RC::RC_ITEM_INVALID_TYPE;
        }
    }
    else if (extension_item_info->m_wType == eITEMTYPE_EXTEND_DATE_ARMOR)
    {
        if (expired_equip_item_info->m_wEqPos < eEQUIPCONTAINER_ARMOR ||
            expired_equip_item_info->m_wEqPos > eEQUIPCONTAINER_SACCESSORY3)
        {
            return RC::RC_ITEM_INVALID_TYPE;
        }
    }
    else if (extension_item_info->m_wType == eITEMTYPE_EXTEND_DATE_RIDER)
    {
        BOOLEAN is_accept = false;
        if (slot_index == SI_INVENTORY)
        {
            if (expired_equip_item_info->IsRiderItem())
            {
                is_accept = true;
            }
            else if (expired_equip_item_info->IsRiderPartsItem())
            {
                if (expired_equip_item_info->m_wEqPos >= eRIDER_PARTS_POS::StartIndex &&
                    expired_equip_item_info->m_wEqPos < eRIDER_PARTS_POS::EndIndex)
                {
                    is_accept = true;
                }
            }
        }

        if (is_accept == false) {
            return RC::RC_ITEM_INVALID_TYPE;
        }
    }

    // Lock
    slot_container->SetLock(at_pos, true);

    locked_info[0].m_SlotIndex = slot_index;
    locked_info[0].m_pos = at_pos;
    locked_info[0].m_code = expired_item_slot.GetItemCode();
    locked_info[0].m_num = 1;
    *number_of_used_locked_info = 1;

    return RC::RC_ITEM_SUCCESS;
}

RC::eITEM_RESULT
ItemManager::ExtendDateOfItem(POSTYPE number_of_kinds,
                              const SLOTCODE* extension_item_code IN,
                              const DWORD* pDateTime IN,
                              SLOTIDX slot_index,
                              POSTYPE expired_slot_pos)
{
    // 기간연장 아이템은 1개만 처리한다.
    if (number_of_kinds != 1) {
        return RC::RC_ITEM_FAILED;
    }
    if (!ValidPos(slot_index, expired_slot_pos, true)) {
        return RC::RC_ITEM_INVALIDPOS;
    }
    SCSlotContainer* slot_container = GetItemSlotContainer(slot_index);
    if (!slot_container) {
        return RC::RC_ITEM_INVALIDPOS;
    }
    SCItemSlot& expired_item_slot = (SCItemSlot&)slot_container->GetSlot(expired_slot_pos);
    const BASE_ITEMINFO* expired_equip_item_info = expired_item_slot.GetItemInfo();
    if (!expired_equip_item_info) {
        return RC::RC_ITEM_FAILED;
    }
    if (expired_item_slot.CanUseMixture(MIXTURE_PERIOD_EXTEND) == false) {
        return RC::RC_ITEM_IMPOSSIBLE_PERIOD_EXTEND;
    }
    // 정액제(대기) 타입인지 체크
    if (!expired_equip_item_info->CanPeriodExtend()) {
        return RC::RC_ITEM_CANNOT_EXTEND_DATE;
    }
    // 기간이 다된 아이템만 기간 연장을 할 수 있다.
    if (!expired_item_slot.IsExpiredWaitTime()) {
        return RC::RC_ITEM_CANNOT_EXTEND_DATE;
    }
    // 무기 연장인지 방어구 연장인지 체크
    const BASE_ITEMINFO* extension_item_info = \
        ItemInfoParser::Instance()->GetItemInfo(extension_item_code[0]);
    if (!extension_item_info) {
        return RC::RC_ITEM_FAILED;
    }
    if (extension_item_info->m_wType == eITEMTYPE_EXTEND_DATE_WEAPON)
    {
        if (expired_equip_item_info->m_wEqPos != eEQUIPCONTAINER_WEAPON) {
            return RC::RC_ITEM_INVALID_TYPE;
        }
    }
    else if (extension_item_info->m_wType == eITEMTYPE_EXTEND_DATE_ARMOR)
    {
        if (expired_equip_item_info->m_wEqPos < eEQUIPCONTAINER_ARMOR ||
            expired_equip_item_info->m_wEqPos > eEQUIPCONTAINER_SACCESSORY3)
        {
            return RC::RC_ITEM_INVALID_TYPE;
        }
    }
    else if (extension_item_info->m_wType == eITEMTYPE_EXTEND_DATE_RIDER)
    {
        BOOLEAN is_accept = false;
        if (slot_index == SI_INVENTORY)
        {
            if (expired_equip_item_info->IsRiderItem())
            {
                is_accept = true;
            }
            else if (expired_equip_item_info->IsRiderPartsItem())
            {
                if (eRIDER_PARTS_POS::StartIndex <= expired_equip_item_info->m_wEqPos &&
                    expired_equip_item_info->m_wEqPos < eRIDER_PARTS_POS::EndIndex)
                {
                    is_accept = true;
                }
            }
        }

        if (is_accept == false) {
            return RC::RC_ITEM_INVALID_TYPE;                            
        }
    }

    // 기간을 연장한다.
    expired_item_slot.SetDateTime(pDateTime[0]);
    expired_item_slot.SetExpiredWaitTime(false);

    // 아이템 슬롯정보 변화를 알린다.
    MSG_CG_ITEM_CHANGE_ITEM_INFO_CMD cmdMsg;
    cmdMsg.m_SlotIndex = slot_index;
    cmdMsg.m_SlotPos = expired_item_slot.GetPos();
    expired_item_slot.CopyOut(cmdMsg.m_ItemStream);
    player_->SendPacket(&cmdMsg, sizeof(cmdMsg));

    return RC::RC_ITEM_SUCCESS;
}

// 잠금처리된 슬롯 해제
void ItemManager::RollbackReservedAllKindOfItem(LOCKED_INFO* locked_info IN,
                                                POSTYPE number_of_used_locked_info IN)
{
    for (POSTYPE f = 0; f < number_of_used_locked_info; ++f)
    {
        SCSlotContainer* slot_container = GetItemSlotContainer(locked_info[f].m_SlotIndex);

        ASSERT(slot_container->IsLocked(locked_info[f].m_pos));
        slot_container->SetLock(locked_info[f].m_pos, false);
    }
}

//==================================================================================================
#if !defined(_NA_0_20110613_GAME_SHOP_RENEWAL)
// (CHANGES) (f100525.4L) change the name,
// and the inven_total_info value will be a possibility of equal to null by the content of
// a _NA000023_090323_ENHANCED_INVENTORY_SLOT_FUNCTIONALITY_
// 유료화 아이템 날짜용
RC::eITEM_RESULT
ItemManager::ObtainAllKindOfItemForShop(POSTYPE number_of_kinds,
                                        const KindOfItemTag all_apply_option_tag,
                                        const SLOTCODE* item_code_array IN,
                                        const POSTYPE* item_num_array IN,
                                        const DWORD* datetime_array IN, 
                                        const BYTE* enchant_array IN,
                                        const BYTE* item_type_array IN,
                                        const BYTE* rank_array IN,
                                        JUST_INVENTORY_TOTAL_INFO* inven_total_info OUT)
{
    inven_total_info->m_Count = 0;
    if (ValidState() == false) {
        return RC::RC_ITEM_INVALIDSTATE;
    }
    SCSlotContainer* inventory = GetItemSlotContainer(SI_INVENTORY);
    //                          (Ref100330.7L)
    if (number_of_kinds == 0 || inventory->GetEmptySlotNum() < number_of_kinds) {
        return RC::RC_ITEM_HAVENOTSPACE;
    }

    nsSlot::ItemQuery select_suplus_query;
    if (!nsSlot::ItemQuery::MakeQueryInverseSelect(&select_suplus_query, player_, SI_INVENTORY)) {
        return RC::RC_ITEM_INVALIDSTATE;
    }
    select_suplus_query.ChangeToFindEmptySlotOnlyMode(); // f100909.1L, ref=f100526.4L
    select_suplus_query.ChangeToAcquireItemTagOption(all_apply_option_tag);
    //
    RC::eITEM_RESULT item_result = ObtainAllKindOfItemShared(&select_suplus_query,
                                                             number_of_kinds,
                                                             item_code_array, item_num_array,
                                                             enchant_array,
                                                             item_type_array,
                                                             rank_array,
                                                             datetime_array, NULL);
    if (item_result != RC::RC_ITEM_SUCCESS) {
        return item_result;
    };
    if (inven_total_info != NULL) {
        select_suplus_query.MakeSerializedStream(inven_total_info);
    };
    return RC::RC_ITEM_SUCCESS;
}
#endif //if !defined(_NA_0_20110613_GAME_SHOP_RENEWAL)
//==================================================================================================
// 주의 (가정) : 유효기간 있는 아이템은 겹칠 수 없다.
// 즉, 겹치는 아이템은 유효기간이 없다. <- (NOTE) exist rule in ItemInfoParser
void ItemManager::ObtainReservedAllKindOfItem(const KindOfItemTag all_apply_option_tag,
                                              LOCKED_INFO* locked_info IN,
                                              POSTYPE number_of_locked_info IN,
                                              JUST_INVENTORY_TOTAL_INFO* inven_total_info OUT)
{

    SCSlotContainer* inventory = GetItemSlotContainer(SI_INVENTORY);
    POSTYPE pos = 0;
    inven_total_info->m_Count = 0;

    for (POSTYPE i = 0; i < number_of_locked_info; ++i)
    {
        pos = locked_info[i].m_pos;
        SCItemSlot& rCurSlot = (SCItemSlot&)inventory->GetSlot(pos);
        ASSERT(rCurSlot.IsLocked());
        inventory->SetLock(pos, false);
        if (inventory->IsEmpty(pos))
        {
            static SCItemSlot making_item;
            making_item.Clear();
            making_item.SetCode(locked_info[i].m_code);
            making_item.SetNum(locked_info[i].m_num);
            // f100909.1L
            if (all_apply_option_tag.value != all_apply_option_tag.Tag_None) {
                making_item.SetOptionTag(all_apply_option_tag);
            }
            switch (making_item.GetItemInfo()->m_ChargeSubType)
            {
            case eCHARGE_SUB_FIXED_AMOUNT:
            case eCHARGE_SUB_FIXED_AMOUNT_EX:
            case eCHARGE_SUB_FIXED_QUANTITY:
            case eCHARGE_SUB_COORDINATE_EX:
            case eCHARGE_SUB_RIDER_WITH_PART:
            case eCHARGE_SUB_RIDER_WITH_PART_NOTIMER:
            case eCHARGE_SUB_FIXED_AMOUNT_EX2:
                {
                    making_item.SetDateTime(locked_info[i].m_DateTime);
                }
                break;
            case eCHARGE_SUB_FIXED_AMOUNT_WAIT:
                {
                    making_item.SetEnchant(locked_info[i].m_Enchant);
                #ifndef _NA_000251_20100727_SOCKET_SYSTEM
                    making_item.SetRank(RANK_E);
                    for (BYTE rank_index = RANK_E; rank_index < locked_info[i].m_Rank; ++rank_index)
                    {
                        ItemManager::ItemRankUp(&making_item);
                    }
                #endif // _NA_000251_20100727_SOCKET_SYSTEM
                    making_item.SetDateTime(locked_info[i].m_DateTime);
                    nsSlot::ItemTypeChanger::ChangeItemByType(&making_item,
                                                              locked_info[i].m_ItemType);
                }
                break;
            case eCHARGE_SUB_FATE:
                {
                    making_item.SetEnchant(locked_info[i].m_Enchant);
                    making_item.SetDateTime(locked_info[i].m_DateTime);
                    nsSlot::ItemTypeChanger::ChangeItemByType(&making_item,
                                                              locked_info[i].m_ItemType);
                }
                break;
            default:
                {
                    making_item.SetEnchant(locked_info[i].m_Enchant);
                #ifndef _NA_000251_20100727_SOCKET_SYSTEM
                    making_item.SetRank(RANK_E);
                    for (BYTE rank_index = RANK_E; rank_index < locked_info[i].m_Rank; ++rank_index)
                    {
                        ItemManager::ItemRankUp(&making_item);
                    }
                #endif // _NA_000251_20100727_SOCKET_SYSTEM
                    nsSlot::ItemTypeChanger::ChangeItemByType(&making_item,
                                                              locked_info[i].m_ItemType);
                }
                break;
            }

            inventory->InsertSlot(pos, making_item);
        }
        else
        {
            inventory->UpdateSlot(rCurSlot.GetPos(), locked_info[i].m_num);
            GAMELOG->LogItem(ITEM_OVERLAPPED_ITEM, player_, &rCurSlot, 0, locked_info[i].m_num);
        }

        inven_total_info->m_Slot[inven_total_info->m_Count].m_Pos = pos;
        rCurSlot.CopyOut(inven_total_info->m_Slot[inven_total_info->m_Count].m_Stream);
        ++inven_total_info->m_Count;
    }
}

//==================================================================================================

//장비를 모두 벗었나?
BOOL ItemManager::IsUnEquipAll()
{
    SCSlotContainer* equip_container = (SCSlotContainer*)GetItemSlotContainer(SI_EQUIPMENT);
    if (NULL == equip_container) {
        return false;
    }
    if (0 == equip_container->GetSlotNum()) {
        return true;
    }
    return false;
}

//==================================================================================================

ItemStatSwitch::CodeListCache ItemStatSwitch::matching_table_static_;
ItemStatSwitch* ItemStatSwitch::pool_free_root_;
const std::size_t ItemStatSwitch::pool_size_static_ = 512;


ItemStatSwitch::ItemStatSwitch(SCItemSlot* target_item_slot, SCItemSlot* use_item_slot,
                               SCItemSlotContainer* slot_container) 
                               : target_item_slot_(target_item_slot),
                                 use_item_slot_(use_item_slot),
                                 slot_container_(slot_container)
{
    ASSERT(target_item_slot_ && use_item_slot_ && slot_container_);
    Search_ItemCode_();
}

ItemStatSwitch::~ItemStatSwitch()
{
}

bool ItemStatSwitch::CreateSameItem(SLOTCODE item_code)
{
    if (item_code == 0) {
        return false;
    }
    making_item_slot_.Clear();
    if (making_item_slot_.CopyFromSrcForStatChange(item_code, *target_item_slot_) == 0) {
        return false;
    }
    return true;
}

bool ItemStatSwitch::GetItemCodeList(MSG_CG_ITEM_STAT_CHANGE_USE_ACK* recv_msg)
{
    const BASE_ITEMINFO* item_info = target_item_slot_->GetItemInfo();
    if (!item_info)
    {
        SUNLOG(eCRITICAL_LOG, "[ItemStatSwitch::GetItemCodeList] Not Exist BaseItemInfo:[%d]", target_item_slot_->GetItemCode());
        return false;
    }

    ItemCodeList* item_code_list = \
        FindItemCodeList(item_info->m_NCode, item_info->m_wWICode, item_info->m_wVICode);
    if (!item_code_list)
    {
        SUNLOG(eCRITICAL_LOG, "[ItemStatSwitch::GetItemCodeList] pCodeList[%u/%u/%u] is NULL!", 
               item_info->m_NCode, item_info->m_wWICode, item_info->m_wVICode);
        return false;
    }

    if (item_code_list->size() > MAX_CHANGE_ITEMSTAT_SLOT_NUM)
    {
        SUNLOG(eCRITICAL_LOG,
               "[ItemStatSwitch::GetItemCodeList] pCodeList[%u/%u/%u] size[%d] is over!", 
               item_info->m_NCode, item_info->m_wWICode, item_info->m_wVICode,
               item_code_list->size());
        return false;
    }

    recv_msg->m_ItemCount = 0;

    ItemCodeList::iterator it, end(item_code_list->end());
    for (it = item_code_list->begin(); it != end ; ++it)
    {
        recv_msg->m_ItemCodes[recv_msg->m_ItemCount] = *it;
        recv_msg->m_ItemCount++;
    }

    return true;
}

void* ItemStatSwitch::operator new(std::size_t size)
{
    if (size != sizeof(ItemStatSwitch)) {
        return ::operator new(size);
    }
    ItemStatSwitch* p = pool_free_root_;
    if (p)
    {
        pool_free_root_ = p->item_stat_switch_next_link_;
    }
    else
    {
        ItemStatSwitch* pNewBlock = static_cast<ItemStatSwitch*>(
            ::operator new(pool_size_static_*sizeof(ItemStatSwitch)));

        for (std::size_t i = 0; i < pool_size_static_ - 1; ++i) {
            pNewBlock[i].item_stat_switch_next_link_ = &pNewBlock[i+1];
        }
        pNewBlock[pool_size_static_ - 1].item_stat_switch_next_link_ = 0;
        p = pNewBlock;
        pool_free_root_ = &pNewBlock[1];
    }

    return p;
}

void ItemStatSwitch::operator delete(void* pos, std::size_t size)
{
    if (pos == 0) return;

    if (size != sizeof(ItemStatSwitch))
    {
        ::operator delete(pos);
        return;
    }

    ItemStatSwitch* temp = static_cast<ItemStatSwitch*>(pos);
    temp->item_stat_switch_next_link_ = pool_free_root_;
    pool_free_root_ = temp;
}

void ItemStatSwitch::Search_ItemCode_()
{
    const BASE_ITEMINFO* target_item_info = target_item_slot_->GetItemInfo();
    if (!target_item_info)
    {
        SUNLOG(eCRITICAL_LOG, "ItemStatSwitch::Search_ItemCode_] Not Exist BaseItemInfo:[%d]",
               target_item_slot_->GetItemCode());
        return;
    }

    ItemCodeList* item_code_list = FindItemCodeList(target_item_info->m_NCode,
                                                    target_item_info->m_wWICode,
                                                    target_item_info->m_wVICode);
    if (item_code_list) {
        return;
    }
    ItemCodeListInfo item_code_list_info(target_item_info->m_NCode,
                                         target_item_info->m_wWICode,
                                         target_item_info->m_wVICode);

    ItemInfoParser* const item_parser = ItemInfoParser::Instance();
    item_parser->SetFirst();
    const BASE_ITEMINFO* search_item_info = item_parser->GetNext();
    for (; search_item_info != NULL; search_item_info = item_parser->GetNext())
    {
        if (target_item_info->m_wWICode == search_item_info->m_wWICode &&
            target_item_info->m_wVICode == search_item_info->m_wVICode &&
            target_item_info->m_NCode == search_item_info->m_NCode)
        {
            item_code_list_info.item_code_list_.push_back(search_item_info->m_Code);
        }
    }
    matching_table_static_.push_back(item_code_list_info);
    return;
}

ItemStatSwitch::ItemCodeList*
ItemStatSwitch::FindItemCodeList(CODETYPE n_code, CODETYPE wi_code, CODETYPE vi_code)
{
    CodeListCache::iterator it, end(matching_table_static_.end());
    for (it = matching_table_static_.begin(); it != end ; ++it)
    {
        ItemCodeListInfo& item_code_list_info = *it;
        if (item_code_list_info.n_code_ == n_code &&
            item_code_list_info.wi_code_ == wi_code &&
            item_code_list_info.vi_code_ == vi_code)
        {
            return &item_code_list_info.item_code_list_;
        }
    }
    return NULL;
}

