#include "stdafx.h"
#include "ItemManager.h"

#include "ItemTypeList.h"

#include "SCItemSlot.h"
#include "SCItemSlotContainer.h"

//==================================================================================================
// NOTE: f101013.1L, added shared logic
namespace nsSlot {
;

struct CompositeShared {
    enum eOperator {
        eOperator_None = 0,
        eOperator_MaterialItemCheck,
        eOperator_DeleteMaterials,
        eOperator_Counts
    };
    static bool IsEqualItemTypeRule(const SCItemSlot& item_slot, const ItemType* item_type);
#ifdef _NA_007435_20140718_MODIFY_ITEMCOMPOSE_MATERIAL_CONDITION
    static bool CheckItemTypeRule(const SCItemSlot& item_slot, const ItemType* item_type);
#endif //_NA_007435_20140718_MODIFY_ITEMCOMPOSE_MATERIAL_CONDITION
};

bool CompositeShared::IsEqualItemTypeRule(const SCItemSlot& item_slot, const ItemType* item_type)
{
    if (item_slot.GetEnchant() != item_type->GetEnchant()) {
        return false;
    };
    if (item_slot.GetRank() != item_type->GetRank()) {
        return false;
    };
    if (nsSlot::ItemTypeChanger::IsEqualItemType(item_slot, item_type) == false) {
        return false;
    }
    return true;
}
#ifdef _NA_007435_20140718_MODIFY_ITEMCOMPOSE_MATERIAL_CONDITION
bool CompositeShared::CheckItemTypeRule(const SCItemSlot &item_slot, const ItemType *item_type)
{
    // 해당 아이템타입의 인챈트값 이상이 되어야 통과
    if (item_slot.GetEnchant() < item_type->GetEnchant()) {
        return false;
    };
    if (item_slot.GetRank() != item_type->GetRank()) {
        return false;
    };
    if (nsSlot::ItemTypeChanger::IsEqualItemType(item_slot, item_type) == false) {
        return false;
    }
    return true;
}
#endif _NA_007435_20140718_MODIFY_ITEMCOMPOSE_MATERIAL_CONDITION
}; //end of namespace

//==================================================================================================
//==================================================================================================
// CHANGES: f100524.3L, waverix, move to a code file related to large size code blocks
// CHANGES: f110406.6L, waverix, change functors to interface implementation objects
//  known functors currently (f110406) = {
//      DeleteItem, DeleteComposeMaterialItem, EmptyItemCheck,
//      ComposeMaterialItemCheck, OverlappedItemCheck
//  };

namespace ns_functor {
;

//==================================================================================================

ns_functor::DeleteItem::DeleteItem(CODETYPE code, int max_num)
    : max_number_(max_num), item_code_(code), item_world_serial_(0)
{
}

bool ns_functor::DeleteItem::operator() (
    SCItemSlotContainer* slot_container, const SCItemSlot& item_slot) const
{
    if (item_slot.IsBlocked() || item_slot.IsLocked()) {
        return false;
    };
    if (item_code_ == item_slot.GetItemCode())
    {
        if (max_number_ < item_slot.GetNum())
        {
            slot_container->UpdateSlot(item_slot.GetPos(), -max_number_);
            return true;
        }

        item_world_serial_ = item_slot.GetWorldSerial();

        max_number_ -= item_slot.GetNum();
        slot_container->DeleteSlot(item_slot.GetPos(), NULL);

        if (0 == max_number_) {
            return true;
        }
    }

    return false;
}

ns_functor::DeleteComposeMaterialItem::DeleteComposeMaterialItem(
    SLOTCODE code, DWORD item_type_index, int max_num)
    : max_number_(max_num),
      item_code_(code),
      item_type_index_(item_type_index),
      item_world_serial_(0)
{
}

#ifdef _NA_007435_20140718_MODIFY_ITEMCOMPOSE_MATERIAL_CONDITION
bool ns_functor::DeleteComposeMaterialItem::operator() (SCItemSlotContainer* slot_container, 
                                                        const SCItemSlot& item_slot, 
                                                        INVEN_DELETE_TOTAL_INFO* OUT delete_item_info) const
{
    // TODO: f101013.1L, copy & paste logic generated from ComposeMaterialItemCheck
    // you should be make more enhanced logic using inventory query processor, but... tiresome.

    // (CHANGES) (100128) (WAVERIX) Pet이후의 Lock요소 고려 대상을 추가했다.
    if (item_slot.IsBlocked() || item_slot.IsLocked() || item_slot.IsUserBlocked()) {
        return false;
    }

    const SLOTCODE item_code_in_slot = item_slot.GetItemCode();
    if (item_code_ != item_code_in_slot) {
        return false;
    };
    const POSTYPE processing_position = item_slot.GetPos();
    const nsSlot::ItemRuleInfo& item_rule_info = item_slot.GetItemRuleInfo();
    if (item_rule_info.is_can_overlap)
    {
        bool is_success = false;
        if (item_type_index_ != 0) {
            return false;
        }
        //
        const POSTYPE number_of_items = item_slot.GetNum();
        int deleted_count = 0;
        if (max_number_ < number_of_items) { // f101013.1L, diff
            deleted_count = max_number_;
            slot_container->UpdateSlot(processing_position, -max_number_);
            is_success = true;
        }
        else
        {
            deleted_count = number_of_items;
            item_world_serial_ = item_slot.GetWorldSerial();
            max_number_ -= number_of_items;
            slot_container->DeleteSlot(processing_position, NULL);

            if (max_number_ == 0) {
                is_success = true;
            }
        }
        int item_count = delete_item_info->m_Count;
        delete_item_info->m_Slot[item_count].m_Pos = processing_position;
        delete_item_info->m_Slot[item_count].m_Num = deleted_count;
        delete_item_info->m_Count++;

        return is_success;
    }
    // non-overlapping item processing
    // NOTE: f101013.1L, this rules describe composite rules for etherealized items.
    // the custom usage of 'IGType' or 'item_type_index_' has
    // item type number of the 'ItemTypeDropRatio' script or empty only.
    // the changed rule added a filter related to an etherealized item
    // if (_NA000109_100427_ITEM_CUSTOMIZING_CONTENTS_COMPOSITE_RULE_)
    //      item_type_index_ (script data) { 0,1,2,3,4,5 }
    //      KindOfItemType::value =          { 0,1,2,3,4 }
    //      the zero value of item_type_index_ has all skip rule
    // else
    //      KindOfItemType::value = { 0 }
    //------------------------------------------------------------------------------
    //                                                 contains 5 = { X,1,2,3,4,5 }
    const bool use_item_type_index = (item_type_index_ > KindOfItemType::Counts);
    if (use_item_type_index)
    {
        const ItemType* item_type = ItemTypeList::Instance()->FindItemType(item_type_index_);
        if (item_type == NULL) {
            SUNLOG(eCRITICAL_LOG, _T("[ItemTypeList]Not Exist Index(%u)\n"), item_type_index_);
            return false;
        }

        bool is_satisfy = nsSlot::CompositeShared::CheckItemTypeRule(item_slot, item_type);
        if (is_satisfy)
        {
            bool is_success = false;
            const POSTYPE number_of_items = item_slot.GetNum();
            int deleted_count = 0;
            if (max_number_ < number_of_items) { // f101013.1L, diff
                deleted_count = max_number_;
                slot_container->UpdateSlot(processing_position, -max_number_);
                is_success = true;
            }
            else
            {
                deleted_count = number_of_items;
                item_world_serial_ = item_slot.GetWorldSerial();
                max_number_ -= number_of_items;
                slot_container->DeleteSlot(processing_position, NULL);

                if (max_number_ == 0) {
                    is_success = true;
                }
            }
            int item_count = delete_item_info->m_Count;
            delete_item_info->m_Slot[item_count].m_Pos = processing_position;
            delete_item_info->m_Slot[item_count].m_Num = deleted_count;
            delete_item_info->m_Count++;

            return is_success;
        }
        return is_satisfy;
    };
    // assert(use_item_type_index == false);
    //
    const bool all_skip_except_etherealized_items = (item_type_index_ == 0);
    if (all_skip_except_etherealized_items) // all skip
    {
        bool is_success = false;
        if (item_rule_info.is_etheria || item_rule_info.is_shell) {
            return false;
        }
        const POSTYPE number_of_items = item_slot.GetNum();
        int deleted_count = 0;
        if (max_number_ < number_of_items) { // f101013.1L, diff
            deleted_count = max_number_;
            slot_container->UpdateSlot(processing_position, -max_number_);
            is_success = true;
        }
        else
        {
            deleted_count = number_of_items;
            item_world_serial_ = item_slot.GetWorldSerial();
            max_number_ -= number_of_items;
            slot_container->DeleteSlot(processing_position, NULL);

            if (max_number_ == 0) {
                is_success = true;
            }
        }
        int item_count = delete_item_info->m_Count;
        delete_item_info->m_Slot[item_count].m_Pos = processing_position;
        delete_item_info->m_Slot[item_count].m_Num = deleted_count;
        delete_item_info->m_Count++;

        return is_success;
    };
    //
    const KindOfItemType kind_of_item_type = { // modify index
        static_cast<KindOfItemType::value_type>(item_type_index_ - 1) };

        const KindOfItemType::value_type current_kind_of_item_type = \
            nsSlot::ItemTypeChanger::GetKindOfItemType(item_slot);
        if (kind_of_item_type.value == current_kind_of_item_type)
        {
            bool is_success = false;
            const POSTYPE number_of_items = item_slot.GetNum();
            int deleted_count = 0;
            if (max_number_ < number_of_items) { // f101013.1L, diff
                deleted_count = max_number_;
                slot_container->UpdateSlot(processing_position, -max_number_);
                is_success = true;
            }
            else
            {
                max_number_ = number_of_items;
                item_world_serial_ = item_slot.GetWorldSerial();
                max_number_ -= number_of_items;
                slot_container->DeleteSlot(processing_position, NULL);
              
                if (max_number_ == 0) {
                    is_success = true;
                }
            }

            int item_count = delete_item_info->m_Count;
            delete_item_info->m_Slot[item_count].m_Pos = processing_position;
            delete_item_info->m_Slot[item_count].m_Num = deleted_count;
            delete_item_info->m_Count++;
            return is_success;
        }
        return false;
}
#else
bool ns_functor::DeleteComposeMaterialItem::operator() (
    SCItemSlotContainer* slot_container, const SCItemSlot& item_slot) const
{
    // TODO: f101013.1L, copy & paste logic generated from ComposeMaterialItemCheck
    // you should be make more enhanced logic using inventory query processor, but... tiresome.

    // (CHANGES) (100128) (WAVERIX) Pet이후의 Lock요소 고려 대상을 추가했다.
    if (item_slot.IsBlocked() || item_slot.IsLocked() || item_slot.IsUserBlocked()) {
        return false;
    }

    const SLOTCODE item_code_in_slot = item_slot.GetItemCode();
    if (item_code_ != item_code_in_slot) {
        return false;
    };
    const POSTYPE processing_position = item_slot.GetPos();
    const nsSlot::ItemRuleInfo& item_rule_info = item_slot.GetItemRuleInfo();
    if (item_rule_info.is_can_overlap)
    {
        if (item_type_index_ != 0) {
            return false;
        }
        //
        const POSTYPE number_of_items = item_slot.GetNum();
        if (max_number_ < number_of_items) { // f101013.1L, diff
            slot_container->UpdateSlot(processing_position, -max_number_);
            return true;
        }
        item_world_serial_ = item_slot.GetWorldSerial();
        max_number_ -= number_of_items;
        slot_container->DeleteSlot(processing_position, NULL);

        if (max_number_ == 0) {
            return true;
        }
        return false;
    }
    // non-overlapping item processing
    // NOTE: f101013.1L, this rules describe composite rules for etherealized items.
    // the custom usage of 'IGType' or 'item_type_index_' has
    // item type number of the 'ItemTypeDropRatio' script or empty only.
    // the changed rule added a filter related to an etherealized item
    // if (_NA000109_100427_ITEM_CUSTOMIZING_CONTENTS_COMPOSITE_RULE_)
    //      item_type_index_ (script data) { 0,1,2,3,4,5 }
    //      KindOfItemType::value =          { 0,1,2,3,4 }
    //      the zero value of item_type_index_ has all skip rule
    // else
    //      KindOfItemType::value = { 0 }
    //------------------------------------------------------------------------------
    //                                                 contains 5 = { X,1,2,3,4,5 }
    const bool use_item_type_index = (item_type_index_ > KindOfItemType::Counts);
    if (use_item_type_index)
    {
        const ItemType* item_type = ItemTypeList::Instance()->FindItemType(item_type_index_);
        if (item_type == NULL) {
            SUNLOG(eCRITICAL_LOG, _T("[ItemTypeList]Not Exist Index(%u)\n"), item_type_index_);
            return false;
        }

        bool success = nsSlot::CompositeShared::IsEqualItemTypeRule(item_slot, item_type);
        if (success)
        {
            const POSTYPE number_of_items = item_slot.GetNum();
            if (max_number_ < number_of_items) { // f101013.1L, diff
                slot_container->UpdateSlot(processing_position, -max_number_);
                return true;
            }
            item_world_serial_ = item_slot.GetWorldSerial();
            max_number_ -= number_of_items;
            slot_container->DeleteSlot(processing_position, NULL);

            if (max_number_ == 0) {
                return true;
            }
            return false;
        }
        return success;
    };
    // assert(use_item_type_index == false);
    //
    const bool all_skip_except_etherealized_items = (item_type_index_ == 0);
    if (all_skip_except_etherealized_items) // all skip
    {
        if (item_rule_info.is_etheria || item_rule_info.is_shell) {
            return false;
        }
        const POSTYPE number_of_items = item_slot.GetNum();
        if (max_number_ < number_of_items) { // f101013.1L, diff
            slot_container->UpdateSlot(processing_position, -max_number_);
            return true;
        }
        item_world_serial_ = item_slot.GetWorldSerial();
        max_number_ -= number_of_items;
        slot_container->DeleteSlot(processing_position, NULL);

        if (max_number_ == 0) {
            return true;
        }
        return false;
    };
    //
    const KindOfItemType kind_of_item_type = { // modify index
        static_cast<KindOfItemType::value_type>(item_type_index_ - 1) };

        const KindOfItemType::value_type current_kind_of_item_type = \
            nsSlot::ItemTypeChanger::GetKindOfItemType(item_slot);
        if (kind_of_item_type.value == current_kind_of_item_type)
        {
            const POSTYPE number_of_items = item_slot.GetNum();
            if (max_number_ < number_of_items) { // f101013.1L, diff
                slot_container->UpdateSlot(processing_position, -max_number_);
                return true;
            }
            item_world_serial_ = item_slot.GetWorldSerial();
            max_number_ -= number_of_items;
            slot_container->DeleteSlot(processing_position, NULL);

            if (max_number_ == 0) {
                return true;
            }
            return false;
        }
        return false;
}
#endif

ns_functor::EmptyItemCheck::EmptyItemCheck(CODETYPE code, int max_num)
    : max_number_(max_num), item_code_(code)
{
}

bool ns_functor::EmptyItemCheck::operator()(
    SCItemSlotContainer* slot_container, const SCItemSlot& item_slot) const
{
    if (item_slot.IsBlocked() || item_slot.IsLocked()) {
        return false;
    };
    if (item_code_ == item_slot.GetItemCode())
    {
        if (max_number_ <= item_slot.GetNum()) {
            return true;
        }
        max_number_ -= item_slot.GetNum();
    }

    return false;
}

ns_functor::ComposeMaterialItemCheck::ComposeMaterialItemCheck(
    SLOTCODE code, DWORD item_type_index, int max_num)
    : max_number_(max_num), item_code_(code), item_type_index_(item_type_index)
{
}

bool ns_functor::ComposeMaterialItemCheck::operator()(
    SCItemSlotContainer* slot_container, const SCItemSlot& item_slot) const
{
    // (CHANGES) (100128) (WAVERIX) Pet이후의 Lock요소 고려 대상을 추가했다.
    if (item_slot.IsBlocked() || item_slot.IsLocked() || item_slot.IsUserBlocked()) {
        return false;
    }

    const SLOTCODE item_code_in_slot = item_slot.GetItemCode();
    if (item_code_ != item_code_in_slot) {
        return false;
    };

    const nsSlot::ItemRuleInfo& item_rule_info = item_slot.GetItemRuleInfo();
    if (item_rule_info.is_can_overlap)
    {
        if (item_type_index_ != 0) {
            return false;
        }
        //
        const POSTYPE number_of_items = item_slot.GetNum();
        if (max_number_ <= number_of_items) {
            return true;
        }
        max_number_ -= number_of_items;
        return false;
    }
    // non-overlapping item processing
    // NOTE: f101013.1L, this rules describe composite rules for etherealized items.
    // the custom usage of 'IGType' or 'item_type_index_' has
    // item type number of the 'ItemTypeDropRatio' script or empty only.
    // the changed rule added a filter related to an etherealized item
    // if (_NA000109_100427_ITEM_CUSTOMIZING_CONTENTS_COMPOSITE_RULE_)
    //      item_type_index_ (script data) { 0,1,2,3,4,5 }
    //      KindOfItemType::value =          { 0,1,2,3,4 }
    //      the zero value of item_type_index_ has all skip rule
    // else
    //      KindOfItemType::value = { 0 }
    //------------------------------------------------------------------------------
    //                                                 contains 5 = { X,1,2,3,4,5 }
    const bool use_item_type_index = (item_type_index_ > KindOfItemType::Counts);
    if (use_item_type_index)
    {
        const ItemType* item_type = ItemTypeList::Instance()->FindItemType(item_type_index_);
        if (item_type == NULL) {
            SUNLOG(eCRITICAL_LOG, _T("[ItemTypeList]Not Exist Index(%u)\n"), item_type_index_);
            return false;
        }
#ifdef _NA_007435_20140718_MODIFY_ITEMCOMPOSE_MATERIAL_CONDITION
        bool success = nsSlot::CompositeShared::CheckItemTypeRule(item_slot, item_type);
#else
        bool success = nsSlot::CompositeShared::IsEqualItemTypeRule(item_slot, item_type);
#endif //_NA_007435_20140718_MODIFY_ITEMCOMPOSE_MATERIAL_CONDITION
        if (success)
        {
            const POSTYPE number_of_items = item_slot.GetNum();
            if (max_number_ <= number_of_items) {
                return true;
            }
            max_number_ -= number_of_items;
            return false;
        }
        return success;
    };
    // assert(use_item_type_index == false);
    //
    const bool all_skip_except_etherealized_items = (item_type_index_ == 0);
    if (all_skip_except_etherealized_items) // all skip
    {
        if (item_rule_info.is_etheria || item_rule_info.is_shell) {
            return false;
        }
        const POSTYPE number_of_items = item_slot.GetNum();
        if (max_number_ <= number_of_items) {
            return true;
        }
        max_number_ -= number_of_items;
        return false;
    };
    //
    const KindOfItemType kind_of_item_type = { // modify index
        static_cast<KindOfItemType::value_type>(item_type_index_ - 1) };

    const KindOfItemType::value_type current_kind_of_item_type = \
        nsSlot::ItemTypeChanger::GetKindOfItemType(item_slot);
    if (kind_of_item_type.value == current_kind_of_item_type)
    {
        const POSTYPE number_of_items = item_slot.GetNum();
        if (max_number_ <= number_of_items) {
            return true;
        }
        max_number_ -= number_of_items;
        return false;
    }
    return false;
}

ns_functor::OverlappedItemCheck::OverlappedItemCheck(CODETYPE code, int num)
    : max_number_(num), item_code_(code)
{
}

bool ns_functor::OverlappedItemCheck::operator()(
    SCItemSlotContainer* slot_container, const SCItemSlot& item_slot) const
{
    if (item_slot.IsBlocked() || item_slot.IsLocked()) {
        return false;
    };
    if (item_code_ == item_slot.GetItemCode())
    {
        if (item_slot.IsOverlap())
        {
            if (max_number_ + item_slot.GetNum() <= item_slot.GetMaxNum()) {
                return true;
            }

            max_number_ = item_slot.GetMaxNum() - item_slot.GetNum();
            if (max_number_ == 0) {
                return true;
            }
        }
    }
    return false;
}

}; //end of namespace 'ns_functor'

//==================================================================================================
//==================================================================================================
//==================================================================================================
