#include "stdafx.h"
#include "./PerkDataContainer.h"
#include <SkillSystem/PerkInfoParser.h>

#ifdef _NA002217_100728_EP2_PERK_SYSTEM_

#include <SCPerkSlot.h>
#include <SCPerkSlotContainer.h>

//==================================================================================================

struct PerkContents_IncompleteCode_NeedRecheck {
    static const bool value = false;
};

namespace nsSkill {
;

//==================================================================================================

PerkDataContainer::PerkDataContainer()
{
    ZeroMemory(&inter_, sizeof(inter_));
    ZeroMemory(&server_data_, sizeof(server_data_));
};

PerkDataContainer::~PerkDataContainer()
{
    perk_table_.clear();
}

void PerkDataContainer::Clear()
{
    perk_table_.clear();
    ZeroMemory(&inter_, sizeof(inter_));
    ZeroMemory(&server_data_, sizeof(server_data_));
}

// the result_index have a range eActiveGroup01 with eActiveGroup10.
// if a result is fails, the result is 0.
// the plan rule is below.
//  a default perk can select multiple to regist to active slot.
/*
bool PerkDataContainer::AllocFromFreeActiveSlot(PerkID* alloced_active_id,
                                                const PerkDataNode* perk_data_node)
{
    alloced_active_id->uid_ = 0;
    //
    for (int index = 0; index < _countof(inter_.active_slots_); ++index)
    {
        const PerkDataNode*& node = inter_.active_slots_[index];
        if (node != NULL) {
            continue;
        }
        node = perk_data_node;
        alloced_active_id->group_ = PerkDataList::GetActiveGroupCodeByIndex(index);
        ++inter_.number_of_aligned_actives_;
        return true;
    }
    return false;
}
*/
bool PerkDataContainer::AllocActiveSlot(const PerkID& active_slot_id,
                                        const PerkDataNode* perk_data_node)
{
    if (perk_data_node == NULL) {
        return false;
    }
    const size_t slot_index = PerkDataList::GetIndexByActiveGroupCode(active_slot_id.group_);
    if (slot_index == _countof(inter_.active_slots_)) {
        return false;
    }
    const PerkDataNode** it = &inter_.active_slots_[slot_index];
    const bool is_empty = (*it == NULL);
    *it = perk_data_node;
    if (is_empty) {
        ++inter_.number_of_aligned_actives_;
    }
    return true;
}

bool PerkDataContainer::FreeActiveSlot(const PerkID& alloced_active_id,
                                       const PerkDataNode* test_node)
{
    if (alloced_active_id.group_ < PerkDataList::eActiveGroup01 ||
        alloced_active_id.group_ > PerkDataList::eActiveGroup10)
    {
        return false;
    }
    BOOST_STATIC_ASSERT(_countof(inter_.active_slots_) == \
                        _countof(PerkDataList::kGroupCodeActiveSlots));
    const size_t slot_index = PerkDataList::GetIndexByActiveGroupCode(alloced_active_id.group_);
    if (slot_index == _countof(inter_.active_slots_)) {
        return false;
    }
    // ... test object?
    if (test_node) {
        assert(inter_.active_slots_[slot_index] == test_node);
    }
    inter_.active_slots_[slot_index] = NULL;
    if (FlowControl::FCAssert(inter_.number_of_aligned_actives_)) {
        --inter_.number_of_aligned_actives_;
    }
    return true;
}

bool PerkDataContainer::IsActivatedSlot(PerkID* one_of_selected_activated_slot_id,
                                        const PerkDataNode* perk_data_node) const
{
    one_of_selected_activated_slot_id->uid_ = 0;
    //
    for (int index = 0; index < _countof(inter_.active_slots_); ++index)
    {
        const PerkDataNode* node = inter_.active_slots_[index];
        if (node == NULL) {
            continue;
        }
        if (node != perk_data_node) {
            continue;
        }
        // found
        one_of_selected_activated_slot_id->group_ = PerkDataList::GetActiveGroupCodeByIndex(index);
        return true;
    }
    return false;
}

// the result_index have a range 0 with kNumberOfCashSlots.
// if a result is fails, the result is kNumberOfCashSlots.
// the major premise is below.
//  you should be find a same group perk before calling a this method.
//  a cash perk using a perk item must be have the unique group code,
// because of the key of a perk table is managed by a group code.
int PerkDataContainer::AllocFromFreeCashSlot(const PerkDataNode* perk_cash_node)
{
    const int kFailValue = _countof(inter_.cash_slots_);
    if (perk_cash_node == NULL) {
        return kFailValue;
    }
    //
    int empty_index = kFailValue;
    for (const PerkDataNode** it = inter_.cash_slots_;
         it != &inter_.cash_slots_[_countof(inter_.cash_slots_)];
         ++it)
    {
        if (*it == perk_cash_node) {
            return kFailValue;
        }
        if (*it != NULL) {
            continue;
        }
        if (empty_index != kFailValue) {
            continue;
        }
        empty_index = static_cast<int>(it - inter_.cash_slots_);
    };
    //
    if (empty_index != kFailValue)
    {
        inter_.cash_slots_[empty_index] = perk_cash_node;
        ++inter_.number_of_aligned_cashes_;
    }
    return empty_index;
}

bool PerkDataContainer::FreeCashSlot(const PerkDataNode* free_cash_node)
{
    if (free_cash_node == NULL) {
        return false;
    };
    for (const PerkDataNode** it = inter_.cash_slots_;
         it != &inter_.cash_slots_[_countof(inter_.cash_slots_)];
         ++it)
    {
        if (*it != free_cash_node) {
            continue;
        }
        *it = NULL;
        if (FlowControl::FCAssert(inter_.number_of_aligned_cashes_)) {
            --inter_.number_of_aligned_cashes_;
        }
        return true;        
    }
    return false;
}

// NOTE, if the result is fails,
// a result value is a number of cash slot array (inter_.cash_slots_)
int PerkDataContainer::GetIndexInCashSlot(const PerkDataNode* perk_data_node) const
{
    const int kCantFind = _countof(inter_.cash_slots_);
    //
    if (perk_data_node == NULL) {
        return kCantFind;
    };
    for (const PerkDataNode* const * it = inter_.cash_slots_;
         it != &inter_.cash_slots_[_countof(inter_.cash_slots_)];
         ++it)
    {
        if (*it != perk_data_node) {
            continue;
        }
        const int find_index = static_cast<int>(it - inter_.cash_slots_);
        return find_index;
    }
    return kCantFind;
}

// NOTE: the perk system experience rule, add only battle experience value.
// CHANGES: f101108.3L, add a function related to level control by GM commands
bool PerkDataContainer::ModifyExpOrLevel(int changed_value, bool is_level, TouchList* result_list)
{
    if (kClientLogic) {
        __UNUSED((changed_value, is_level, result_list));
        return false;
    }
#ifdef _SERVER
    if (changed_value == 0) {
        return false;
    };
    //
    ZeroMemory(result_list, sizeof(*result_list));
    int64_t changed_exps[_countof(inter_.active_slots_)];
    //
    const PerkDataNode** const active_end = \
        &inter_.active_slots_[_countof(inter_.active_slots_)];
    //
    if (is_level)
    {   // special logic by GM command
        PerkDataNode** active_it = const_cast<PerkDataNode**>(inter_.active_slots_);
        for (int index = 0; active_it != active_end; ++index, ++active_it)
        {
            const PerkInfo* perk_info = NULL;
            if (*active_it == NULL || (perk_info = (*active_it)->perk_info_) == NULL) {
                continue;
            };
            const PerkDataNode* const active_perk_node = *active_it;
            //
            TouchList::ChangedInfo* const changed_info = &result_list->changed_infos[index];
            changed_info->prev_exp = active_perk_node->exp_;
            //
            int min_val = min(0, changed_value);
            const int max_val = max(0, changed_value);
            //
            int64_t* const exp_value = &changed_exps[index];
            for (bool leveldown = (max_val == 0); min_val < max_val; ++min_val)
            {
                const PerkInfo* perk_rotated = leveldown ?
                    perk_info->prev_perk_ : perk_info->next_perk_;
                if (perk_rotated == NULL) {
                    *exp_value = perk_info->exp_value_;
                    break;
                }
                *exp_value = perk_rotated->exp_value_;
                perk_info = perk_rotated;
            };
            changed_info->perk_group = perk_info->perk_id_.group_;
            changed_info->next_exp = *exp_value;
        }
    }
    else
    {
        for (int64_t* exps_it = changed_exps;
             exps_it != &changed_exps[_countof(inter_.active_slots_)];
             ++exps_it)
        {
            *exps_it = changed_value;
        }
    }

    PerkDataNode** active_it = const_cast<PerkDataNode**>(inter_.active_slots_);
    for (int index = 0; active_it != active_end; ++index, ++active_it)
    {
        uint8_t* const touch_it = &result_list->touches[index];
        if (*touch_it != result_list->None) {
            continue;
        };
        //
        *touch_it = result_list->Skipped;
        //
        PerkDataNode* active_node = *active_it;
        if (active_node == NULL) {
            continue;
        }
        const PerkInfo* perk_info = active_node->perk_info_;
        if (bool cash_perk = (perk_info->item_code_ != 0)) {
            continue;
        }
        const int64_t& changed_exp = changed_exps[index];
        if (is_level)
        {
            active_node->exp_ = changed_exp;
        }
        else
        {
            if (changed_exp > 0LL && perk_info->max_level_) {
                continue;
            };
            TouchList::ChangedInfo* const changed_info = &result_list->changed_infos[index];
            if (int64_t(active_node->exp_) + changed_exp <= 0LL) {
                active_node->exp_ = 0;
                changed_info->next_exp = 0;
            }
            else {
                active_node->exp_ += changed_exp;
                changed_info->perk_group = perk_info->perk_id_.group_;
                changed_info->next_exp = active_node->exp_ + changed_exp;
            }
        };
        //
        *touch_it = result_list->Selected;
        //
        for (int inter_index = static_cast<int>(active_it - inter_.active_slots_) + 1;
             inter_index < _countof(inter_.active_slots_);
             ++inter_index)
        {
            if (active_node == inter_.active_slots_[inter_index]) {
                result_list->touches[inter_index] = result_list->Skipped;
            }
        }
    }; //end 'for'
    //
#endif
    return true;
}

// (NOTE) if the value of a 'is_add' is false, do batched process subtract attributes
void PerkDataContainer::BatchCalculateAttrActivePerks(bool is_add,
    IPerkControlMessenger* const perk_messenger)
{
    struct AttrBatchProcessor {
        enum { eMode_None, eMode_Touched, eMode_Selected };
        AttrBatchProcessor() {
            ZeroMemory(this, sizeof(*this));
        }
        //
        uint8_t modes[PerkDataList::kNumberOfActiveSlots];
        uint8_t number[PerkDataList::kNumberOfActiveSlots];
        // 5 * (4 + 2 * 3) = 50(max)...
        // real
        //   max_overlaps = 4 -> active = 1 --> 1 * (4 + 1 * 3) = 7
        //   overlaps     = 1 -> active = 4 --> 4 * (4 + 0) = 16
        //   overlaps_avg = 2 -> active = 2 --> 2 * (4 + 1 * 2) = 12
        Attributes::OperationRecord records[
            PerkDataList::kNumberOfActiveSlots *
            (PerkInfo::kNumberOfBenefitOptions + PerkInfo::kNumberOfPenaltyOptions * 3)];
    } batch_process;
    //
    BOOST_STATIC_ASSERT(_countof(batch_process.modes) == _countof(inter_.active_slots_));
    //
    const PerkDataNode** const active_end_it = \
        &inter_.active_slots_[_countof(inter_.active_slots_)];
    const PerkDataNode** base_it = inter_.active_slots_;
    for (int index = 0; base_it != active_end_it; ++index, ++base_it)
    {
        const PerkDataNode* node = *base_it;
        const int mode = batch_process.modes[index];
        if (mode == batch_process.eMode_Touched) {
            continue;
        }
        if (node == NULL) {
            batch_process.modes[index] = batch_process.eMode_Touched;
            continue;
        }
        if (mode != batch_process.eMode_None) {
            continue;
        };
        //
        int number = 0;
        for (const PerkDataNode** check_it = inter_.active_slots_;
             check_it != active_end_it; ++check_it)
        {
            const int interloop_index = static_cast<int>(check_it - inter_.active_slots_);
            const PerkDataNode* checked_node = *check_it;
            if (checked_node == NULL) {
                batch_process.modes[interloop_index] = batch_process.eMode_Touched;
                continue;
            }
            if (checked_node == node) {
                ++number;
                batch_process.modes[interloop_index] = batch_process.eMode_Touched;
                continue;
            }
            //
            //batch_process.modes[interloop_index] = batch_process.eMode_Touched;
        }
        batch_process.modes[index] = batch_process.eMode_Selected;
        batch_process.number[index] = static_cast<uint8_t>(number);
    }; //end 'for'
    //
    // register add or sub operation
    //
    Attributes::OperationRecord* record = batch_process.records;
    base_it = inter_.active_slots_;
    const uint8_t operate = is_add ? static_cast<uint8_t>(Attributes::Operation::eOper_Add) :
                                     static_cast<uint8_t>(Attributes::Operation::eOper_Sub);
    //
    for (int index = 0;
         index < _countof(batch_process.modes) &&
         record < &batch_process.records[_countof(batch_process.records)];
         ++index, ++base_it)
    {
        const PerkDataNode* node = *base_it;
        const int mode = batch_process.modes[index];
        if (mode != batch_process.eMode_Selected) {
            continue;
        }
        //
        int duplicated = batch_process.number[index];
        if (FlowControl::FCAssert(duplicated <= 4) == false) {
            duplicated = 4;
            batch_process.number[index] = 4;
        }

        const PerkInfo* perk_info = node->perk_info_;
        for (int dup_index = 0; dup_index < duplicated; ++dup_index)
        {
            const PerkInfo::OptionNode* benefit_it = perk_info->benefit_option_, \
                * benefit_end = &perk_info->benefit_option_[_countof(perk_info->benefit_option_)];
            for ( ; benefit_it != benefit_end; ++benefit_it)
            {
                if (benefit_it->option_ == eATTR_TYPE_INVALID) {
                    continue;
                }
                record->operate = operate; // add or sub?
                record->attr_kind = benefit_it->attr_kind_;
                record->attr_type = benefit_it->option_;
                record->attr_value = benefit_it->value_;
                ++record;
            }
        };
        //
        if (duplicated == 1) {
            continue;
        }
        //
        const PerkInfo::OptionNode* penalty_it
            = duplicated == 2 ? perk_info->penalty_2x_option_
            : duplicated == 3 ? perk_info->penalty_3x_option_
            : duplicated == 4 ? perk_info->penalty_4x_option_
            :                   NULL;
        const PerkInfo::OptionNode* penalty_end
            = duplicated == 2 ? &perk_info->penalty_2x_option_[_countof(perk_info->penalty_2x_option_)]
            : duplicated == 3 ? &perk_info->penalty_3x_option_[_countof(perk_info->penalty_3x_option_)]
            : duplicated == 4 ? &perk_info->penalty_4x_option_[_countof(perk_info->penalty_4x_option_)]
            :                   NULL;

        for ( ; penalty_it != penalty_end; ++penalty_it)
        {
            if (penalty_it->option_ == eATTR_TYPE_INVALID) {
                continue;
            }
            record->operate = operate; // add or sub?
            record->attr_kind = penalty_it->attr_kind_;
            record->attr_type = penalty_it->option_;
            record->attr_value = penalty_it->value_;
            ++record;
        }
    }

    Attributes* attributes = perk_messenger->GetOwnerAttributes();
    attributes->GroupBatchOperate(batch_process.records,
                                  static_cast<int>(record - batch_process.records));
    //
#if SUN_CODE_BACKUP // NOTE: f110413.2L
    //if (kClientLogic)
    {
        ZeroMemory(&inter_.active_slot_presenter_, sizeof(inter_.active_slot_presenter_));
        ActiveSlotPresenter* const presenter = &inter_.active_slot_presenter_;
        base_it = inter_.active_slots_;
        for (int index = 0; index < _countof(batch_process.modes); ++index, ++base_it)
        {
            const PerkDataNode* node = *base_it;
            if (node == NULL) {
                continue;
            }
            const int mode = batch_process.modes[index];
            if (mode != batch_process.eMode_Selected) {
                continue;
            }
            //
            presenter->number_of_overlaps_[index] = batch_process.number[index];
            presenter->perk_infos_[index] = node->perk_info_;
            if (batch_process.number[index] == 1) {
                continue;
            }
            //
            const PerkDataNode** inter_loop_it = base_it + 1;
            for (int second_index = index + 1;
                 second_index < _countof(batch_process.modes);
                 ++second_index, ++inter_loop_it)
            {
                if (*inter_loop_it == *base_it) {
                    presenter->number_of_overlaps_[second_index] = batch_process.number[index];
                    presenter->perk_infos_[second_index] = node->perk_info_;
                }
            }
        };
    }
#endif //SUN_CODE_BACKUP
}

//==================================================================================================

// D->G or G->C update stream
void PerkDataContainer::StorePerk(const PerkDataList& perk_data_list,
                                  IPerkControlMessenger* perk_messenger)
{
    ASSERT(perk_data_list.number_of_nodes_ <= perk_data_list.kMaxNumberOfNodes &&
           perk_table_.empty());
    perk_table_.clear();
    ZeroMemory(&inter_, sizeof(inter_));
    ZeroMemory(&server_data_, sizeof(server_data_));
    //
    PerkInfoParser* const perk_parser = PerkInfoParser::Instance();
    const PerkData* src_it = perk_data_list.perk_data_,
                  * src_end = perk_data_list.perk_data_ + perk_data_list.number_of_nodes_;
    int active_index = 0;
    int default_index = 0;
    int cash_index = 0;
    for (; src_it != src_end; ++src_it)
    {
        const PerkID& perk_id = src_it->perk_id_;
        const bool is_active_slot = (perk_id.group_ >= PerkDataList::eActiveGroup01);
        const PerkInfo* perk_info = is_active_slot == false ? perk_parser->FindPerkInfo(perk_id)
                                                            : NULL;
        if (is_active_slot == false) {
            if (FlowControl::FCAssert(perk_info != NULL) == false) {
                continue;
            }
        }
        // register & align data
        PerkDataNode* node = &perk_table_[perk_id.group_];
        ZeroMemory(node, sizeof(*node));
        static_cast<PerkData&>(*node) = *src_it;
        node->perk_info_ = perk_info;
        //
        if (is_active_slot) {
            ++active_index;
            continue;
        }
        assert(perk_info);
        if (perk_info->item_code_ == 0) {
            node->pos_in_container_ = perk_info->order_;
            ++default_index;
            continue;
        }
        // cash type control
        if (FlowControl::FCAssert(cash_index < _countof(inter_.cash_slots_)) == false) {
            continue;
        }
        inter_.cash_slots_[cash_index] = node;
        node->pos_in_container_ = PerkDataList::kCashSlotPositions[cash_index];
        inter_.number_of_aligned_cashes_ = ++cash_index;
        //
    }
    // active slot control
    assert(active_index == _countof(PerkDataList::kGroupCodeActiveSlots));
    assert((active_index + default_index + cash_index) == perk_data_list.number_of_nodes_);
    //
    const PerkDataList::eGroupCodeActiveSlot* active_code_it = PerkDataList::kGroupCodeActiveSlots;
    BOOST_STATIC_ASSERT(_countof(PerkDataList::kGroupCodeActiveSlots) ==
                        _countof(inter_.active_slots_));
    for (active_index = 0;
         active_index < _countof(PerkDataList::kGroupCodeActiveSlots);
         ++active_index, ++active_code_it) 
    {
        PerkTable::iterator active_slot_it = perk_table_.find(*active_code_it);
        assert(active_slot_it != perk_table_.end());
        PerkDataNode& active_slot = active_slot_it->second;
        //
        active_slot.pos_in_container_ = PerkDataList::kActiveSlotPositions[active_index];
        if (active_slot.link_code_ == 0) {
            continue;
        }
        PerkTable::iterator perk_slot_it = perk_table_.find(active_slot.link_code_);
        if (!FlowControl::FCAssert(perk_slot_it != perk_table_.end())) {
            active_slot.link_code_ = 0;
            continue;
        }
        inter_.active_slots_[active_index] = &perk_slot_it->second;
        ++inter_.number_of_aligned_actives_;
    }
    //
    if (perk_messenger == NULL) {
        return;
    }
    // for client process
    SCPerkSlotContainer* perk_container = perk_messenger->GetPerkSlotContainer();
    if (perk_container)
    {
        // client section : fill a slot in a perk slot container
        const int number_of_default = default_index;
        const int number_of_cash = cash_index;
        const int number_of_active = active_index;
        perk_container->SetSlotPosByTypes(number_of_default, number_of_cash, number_of_active);
        default_index = perk_container->GetDefaultSlotPos();
        cash_index = perk_container->GetCashSlotPos();
        active_index = perk_container->GetActiveSlotPos();
        //
        SCPerkSlot* base_slot = static_cast<SCPerkSlot*>(perk_container->CreateSlot());
        struct ReleaseResource {
            ReleaseResource(SCPerkSlot* slot) : slot_(slot) {}
            ~ReleaseResource() { SAFE_DELETE(slot_); };
            SCPerkSlot* slot_;
        } release_resource(base_slot);
        //
        FOREACH_CONTAINER(const PerkTable::value_type& node, perk_table_, PerkTable)
        {
            const PerkDataNode& control = node.second;
            //const PerkID& perk_id = control.perk_id_;
            //const bool is_active_slot = (perk_id.group_ >= PerkDataList::eActiveGroup01);
            //const bool is_cash_slot = is_active_slot ? false : (control.perk_info_->item_code_ != 0);
            base_slot->Copy(control);
            //int selected_pos = is_active_slot ? active_index++
            //                 : is_cash_slot   ? cash_index++
            //                 :                  control.perk_info_->order_;
            perk_container->InsertSlot(control.pos_in_container_, *base_slot);
        }
        // use variable 'active_index'
        if (inter_.number_of_aligned_actives_)
        {   // NOTE: link a real activated perk slot
            RC::eSLOT_INSERT_RESULT slot_result;
            const PerkDataNode** it = inter_.active_slots_,
                              ** end = &inter_.active_slots_[_countof(inter_.active_slots_)];
            for (int index = 0; it != end; ++it, ++index, ++active_index)
            {
                const PerkDataNode* node = *it;
                if (node == NULL) {
                    continue;
                }
                // CHANGES: f110228.3L, fixed a redirection routine
                // to valid acquisition real active slot pos.
                POSTYPE active_slot_pos = PerkDataList::kActiveSlotPositions[index];
                SCPerkSlot* active_slot = static_cast<SCPerkSlot*>(\
                    perk_container->GetValidSlotPtr(&slot_result, active_slot_pos));
                const SCPerkSlot* activated_slot = static_cast<SCPerkSlot*>(\
                    perk_container->GetValidSlotPtr(&slot_result, node->pos_in_container_));
                active_slot->LinkSlot(activated_slot);
                perk_container->UpdateSlot(active_slot_pos, 1);
            }
        };
    }; //end if 'exist a slot container'
}

// NOTE: f101026.2L, add a factility for single attribute application on first enter field event
bool PerkDataContainer::ApplyAttributes(IPerkControlMessenger* perk_messenger)
{
    if (perk_messenger == NULL) {
        return false;
    };
    BatchCalculateAttrActivePerks(true, perk_messenger);
    if (IPerkControlEvent* event_notifier = perk_messenger->GetPerkControlEvent()) {
        event_notifier->OnChangeAttributes();
    }
    return true;
}

// G->C send stream
void PerkDataContainer::LoadPerk(PerkDataList* perk_data_list) const
{
//#ifdef _SERVER
    ZeroMemory(perk_data_list, sizeof(*perk_data_list));
    //
    PerkData* dest_it = perk_data_list->perk_data_,
            * const dest_end = &perk_data_list->perk_data_[_countof(perk_data_list->perk_data_)];
    //
    const PerkTable::size_type number_of_records = perk_table_.size();
    if (FlowControl::FCAssert(number_of_records <=
                              _countof(perk_data_list->perk_data_)) == false)
    {
        return;
    }

    FOREACH_CONTAINER(const PerkTable::value_type& node, perk_table_, PerkTable)
    {
        if (dest_it == dest_end) {
            break;
        }
        const PerkDataNode& control = node.second;
        // PerkData <-- PerkDataNode
        *dest_it = control;
        //
        ++dest_it;
    }
    //
    perk_data_list->number_of_nodes_ = static_cast<int>(dest_it - perk_data_list->perk_data_);
//#endif
}

// register a cash perk slot using a cash item
bool PerkDataContainer::RequestPlants(PerkControl* req_info, const POSTYPE from_inven_item_pos,
                                      IPerkControlMessenger* const perk_messenger)
{
    __UNUSED(perk_messenger);
    req_info->Clear();
    req_info->request = req_info->eReq_Plants;
    //
    if (inter_.request_info_.request != PerkControl::eReq_None) {
        req_info->result = req_info->eRet_ItemFailed;
        req_info->result_code = RC::RC_ITEM_COOLTIME_ERROR;
        return false;
    };
    //
    //if (inter_.plants_trans_ != PerkControl::eReq_None) {
    //    req_info->result = req_info->eRet_ItemFailed;
    //    req_info->result_code = RC::RC_ITEM_UNUSABLE_FUNCTION;
    //    return false;
    //};
    if (inter_.number_of_aligned_cashes_ >= PerkDataList::kNumberOfCashSlots) {
        req_info->result = req_info->eRet_ItemFailed;
        req_info->result_code = RC::RC_ITEM_ISNOTEMPTYSLOT;
        return false;
    }
    //
    req_info->args.plants.from_item_pos = from_inven_item_pos;
    //
    //inter_.plants_trans_ = req_info->request;
    inter_.request_info_ = *req_info;
    if (kServerLogic) {
        ZeroMemory(&server_data_, sizeof(server_data_));
    }
    return true;
}

// unregister a registered cash perk so that a cash perk move to item in inventory
bool PerkDataContainer::RequestUproot(PerkControl* req_info,
                                      const PerkID& uprooting_perk_id,
                                      const POSTYPE to_inven_empty_pos,
                                      IPerkControlMessenger* const perk_messenger)
{
    __UNUSED(perk_messenger);
    req_info->Clear();
    req_info->request = req_info->eReq_Uproot;
    //
    if (inter_.request_info_.request != PerkControl::eReq_None) {
        req_info->result = req_info->eRet_ItemFailed;
        req_info->result_code = RC::RC_ITEM_COOLTIME_ERROR;
        return false;
    };
    //
    PerkTable::iterator found = perk_table_.find(uprooting_perk_id.group_);
    if (found == perk_table_.end()) {
        req_info->result = req_info->eRet_SkillFailed;
        req_info->result_code = RC::RC_SKILL_CANT_FIND_INFO;
        return false;
    };
    //
    PerkDataNode* perk_data_node = &found->second;
    const PerkInfo* perk_info = perk_data_node->perk_info_;
    if (perk_info->item_code_ == 0) {
        req_info->result = req_info->eRet_ItemFailed;
        req_info->result_code = RC::RC_ITEM_UNUSABLE_FUNCTION;
        return false;
    }
    
    PerkID alloced_active_id;
    bool is_activated = IsActivatedSlot(&alloced_active_id, perk_data_node);
    //    req_info->result = req_info->eRet_SkillFailed;
    //    req_info->result_code = RC::RC_SKILL_UNROOT_PERK_REQUIRE_INACTIVE_STATE;
    //    return false;
    //}
    //
    req_info->char_guid = inter_.pass_key_;
    req_info->args.uproot.perk_uid = perk_data_node->perk_id_.uid_;
    req_info->args.uproot.to_item_pos = to_inven_empty_pos;
    //
    inter_.request_info_ = *req_info;
    if (kServerLogic) {
        ZeroMemory(&server_data_, sizeof(server_data_));
        server_data_.uproot.uprooting_node = perk_data_node;
        if (is_activated) {
            server_data_.uproot.active_node_if_registered = \
                &perk_table_.find(alloced_active_id.group_)->second;
        }
    }
    return true;
}

// move to an active slot so that change to active
bool PerkDataContainer::RequestRegister(PerkControl* req_info,
                                        const PerkID& from_perk_id, const PerkID& to_perk_id,
                                        IPerkControlMessenger* const perk_messenger)
{
    __UNUSED(perk_messenger);
    req_info->Clear();
    req_info->request = req_info->eReq_Register;
    /*
    if (inter_.number_of_aligned_actives_ >= PerkDataList::kNumberOfActiveSlots) {
        req_info->result = req_info->eRet_ItemFailed;
        req_info->result_code = RC::RC_ITEM_ISNOTEMPTYSLOT;
        return false;
    }
    */
#ifdef _NA_007019_20140107_PERK_CHANGE_SYSTEM
    // 한번에 보내기 위해서 막는다.
#else
    if (inter_.request_info_.request != PerkControl::eReq_None) {
        req_info->result = req_info->eRet_ItemFailed;
        req_info->result_code = RC::RC_ITEM_COOLTIME_ERROR;
        return false;
    };
#endif // _NA_007019_20140107_PERK_CHANGE_SYSTEM
    const int active_array_index = PerkDataList::GetIndexByActiveGroupCode(to_perk_id.group_);
    if (active_array_index == _countof(PerkDataList::kGroupCodeActiveSlots)) {
        req_info->result = req_info->eRet_ItemFailed;
        req_info->result_code = RC::RC_ITEM_INVALIDPOS;
        return false;
    };
    PerkTable::iterator found = perk_table_.find(from_perk_id.group_);
    if (found == perk_table_.end()) {
        req_info->result = req_info->eRet_SkillFailed;
        req_info->result_code = RC::RC_SKILL_CANT_FIND_INFO;
        return false;
    }
    //
    PerkDataNode* perk_data_node = &found->second;
    const bool can_register = PerkDataList::CanRegisterToActiveSlot(perk_data_node->perk_info_,
                                                                    active_array_index);
    if (can_register == false) {
        req_info->result = req_info->eRet_ItemFailed;
        req_info->result_code = RC::RC_ITEM_INVALIDPOS;
        return false;
    }
    /*
    PerkID alloced_active_id;
    if (AllocFromFreeActiveSlot(&alloced_active_id, perk_data_node) == false) {
        req_info->result = req_info->eRet_ItemFailed;
        req_info->result_code = RC::RC_ITEM_ISNOTEMPTYSLOT;
        return false;
    };
    */
    //PerkTable::iterator active_node_it = perk_table_.find(alloced_active_id.group_);
    // assert rule
    //if (FlowControl::FCAssert(active_node_it != perk_table_.end()) == false) {
    //    req_info->result = req_info->eRet_ItemFailed;
    //    req_info->result_code = RC::RC_ITEM_INVALIDSTATE;
    //    return false;
    //}
    //
    req_info->char_guid = inter_.pass_key_;
    req_info->args.regist.active_slot_uid = to_perk_id.uid_;
    req_info->args.regist.perk_uid = from_perk_id.uid_;
    // enter transaction
    //PerkDataNode* active_node = &active_node_it->second;
    inter_.request_info_ = *req_info;
    //
    if (kServerLogic) {
        ZeroMemory(&server_data_, sizeof(server_data_));
        server_data_.regist.registering_node = perk_data_node;
        PerkTable::iterator active_node_it = perk_table_.find(to_perk_id.group_);
        server_data_.regist.active_node = &active_node_it->second;
    }
    //
    return true;
}

// move from an active slot so that change to inactive
bool PerkDataContainer::RequestUnregister(PerkControl* req_info, const PerkID& perk_id,
                                          IPerkControlMessenger* const perk_messenger)
{
    __TOUCHED(perk_messenger);
    req_info->Clear();
    req_info->request = req_info->eReq_Unregister;
    //
#ifdef _NA_007019_20140107_PERK_CHANGE_SYSTEM
    // 한번에 하나만 바꾸겠다는 전제가 깔려있다.
#else
    if (inter_.request_info_.request != PerkControl::eReq_None) {
        req_info->result = req_info->eRet_ItemFailed;
        req_info->result_code = RC::RC_ITEM_COOLTIME_ERROR;
        return false;
    };
#endif // _NA_007019_20140107_PERK_CHANGE_SYSTEM
    //
    PerkTable::iterator found = perk_table_.find(perk_id.group_);
    if (found == perk_table_.end()) {
        req_info->result = req_info->eRet_ItemFailed;
        req_info->result_code = RC::RC_ITEM_INVALIDPOS;
        return false;
    }
    //
    PerkDataNode* active_node = &found->second;
    if (active_node->link_code_ == 0) {
        req_info->result = req_info->eRet_SkillFailed;
        req_info->result_code = RC::RC_SKILL_CANT_FIND_INFO;
        return false;
    }
    //
    PerkTable::iterator perk_node_it = perk_table_.find(active_node->link_code_);
    if (perk_node_it == perk_table_.end()) {
        req_info->result = req_info->eRet_SkillFailed;
        req_info->result_code = RC::RC_SKILL_CANT_FIND_INFO;
        return false;
    }
    PerkDataNode* perk_data_node = &perk_node_it->second;
    req_info->char_guid = inter_.pass_key_;
    req_info->args.unregist.perk_uid = active_node->perk_id_.uid_; //perk_data_node->perk_id_.uid_;
    //
    // enter transaction
    inter_.request_info_ = *req_info;
    //
    if (kServerLogic) {
        ZeroMemory(&server_data_, sizeof(server_data_));
        server_data_.unregist.active_node_for_unregist = active_node;
        server_data_.unregist.unregistering_node = perk_data_node;
    }

    return true;
}

// NOTE, f100902.5L, this is a single side command by server
// if a cash perk containing the expiration date is expired.
// but, the result_code value of a req_info is not established
// because of a reason of deletion may have serveral situations.
bool PerkDataContainer::RequestDeletion(PerkControl* req_info, const PerkID& perk_id,
                                        IPerkControlMessenger* const perk_messenger)
{
    req_info->Clear();
#if !defined(_SERVER)
    __UNUSED((perk_id, perk_messenger));
    return false;
#endif
//
#if defined(_SERVER)
    __UNUSED(perk_messenger);
    req_info->request = req_info->eReq_Delete;
    if (inter_.request_info_.request != PerkControl::eReq_None) {
        req_info->result = req_info->eRet_ItemFailed;
        req_info->result_code = RC::RC_ITEM_COOLTIME_ERROR;
        return false;
    };
    //
    PerkTable::iterator found = perk_table_.find(perk_id.group_);
    if (found == perk_table_.end()) {
        req_info->result = req_info->eRet_ItemFailed;
        req_info->result_code = RC::RC_ITEM_INVALIDPOS;
        return false;
    }
    //
    PerkDataNode* cash_perk_node = &found->second;
    const int cash_slot_index = GetIndexInCashSlot(cash_perk_node);
    if (!FlowControl::FCAssert(cash_slot_index != _countof(inter_.cash_slots_))) {
        req_info->result = req_info->eRet_ItemFailed;
        req_info->result_code = RC::RC_ITEM_INVALIDPOS;
        return false;
    }
    //
    PerkID active_slot_id;
    bool is_activated = IsActivatedSlot(&active_slot_id, cash_perk_node);
    //
    req_info->char_guid = inter_.pass_key_;
    req_info->args.deletion.perk_uid = perk_id.uid_;
    // enter transaction
    inter_.request_info_ = *req_info;
    //
    if (kServerLogic) {
        ZeroMemory(&server_data_, sizeof(server_data_));
        server_data_.deletion.deleting_node = cash_perk_node;
        if (is_activated) {
            // WARNING, an activated cash perk has one slot only.
            PerkTable::iterator active_node_it = perk_table_.find(active_slot_id.group_);
            assert(active_node_it != perk_table_.end());
            server_data_.deletion.active_node_if_registered = &active_node_it->second;
        }
    }

    return true;
#endif //defined(_SERVER)
}

bool PerkDataContainer::ProcessReplyMessage(const PerkControl& reply_header,
                                            PerkControlData* data,
                                            IPerkControlMessenger* perk_messenger)
{
    if (kClientLogic)
    {
        // (NOTE) a message handling logic for client parts is not check a 'request_info_' field
        // of a PerkDataNode, because of to handle server single side command
        if (reply_header.result != reply_header.eRet_Success) {
            this->inter_.request_info_.Clear();
            return false;
        }
        switch (reply_header.request)
        {
        case reply_header.eReq_Register:
            return ProcessRegister(reply_header, data, perk_messenger);
        case reply_header.eReq_Delete:
            return ProcessDelete(reply_header, data, perk_messenger);
        case reply_header.eReq_Plants:
            return ProcessPlants(reply_header, data, perk_messenger);
        case reply_header.eReq_Uproot:
            return ProcessUproot(reply_header, data, perk_messenger);
        case reply_header.eReq_Status:
            assert(!"function not implementation");
            break;
        case reply_header.eReq_Unregister:
            return ProcessUnregister(reply_header, data, perk_messenger);
        }
    };
    if (kServerLogic)
    {
        switch (reply_header.request)
        {
        case reply_header.eReq_Register:
            return ProcessRegister(reply_header, data, perk_messenger);
        case reply_header.eReq_Delete:
            return ProcessDelete(reply_header, data, perk_messenger);
        case reply_header.eReq_Plants:
            return ProcessPlants(reply_header, data, perk_messenger);
        case reply_header.eReq_Uproot:
            return ProcessUproot(reply_header, data, perk_messenger);
        case reply_header.eReq_Unregister:
            return ProcessUnregister(reply_header, data, perk_messenger);
        }
    }
    return false;
}


bool PerkDataContainer::ProcessPlants(const PerkControl& reply_header, PerkControlData* data,
                                      IPerkControlMessenger* perk_messenger)
{
    // (NOTE) a message handling logic for client parts is not check a 'request_info_' field
    // of a PerkDataNode, because of to handle server single side command
    const bool plants_process_acceptable_condition = \
        (reply_header.result == reply_header.eRet_Success) &&
        (data && data->data_type == data->eDataType_Plants);
    if (!FlowControl::FCAssert(plants_process_acceptable_condition == true)) {
        return false;
    }
    //
    PerkInfoParser* perk_parser = PerkInfoParser::Instance();
    const PerkID perk_id(data->plants.perk_uid);
    const PerkInfo* perk_info = perk_parser->FindPerkInfo(perk_id);
    assert(perk_info && perk_info->item_code_ != 0);
    //
    PerkDataNode* node = &perk_table_[perk_id.group_];
    ZeroMemory(node, sizeof(*node));
    node->perk_id_ = perk_id;
    node->exp_ = data->plants.expiration_date; // f101228.2L
    node->perk_info_ = perk_info;
    int cash_slot_index = AllocFromFreeCashSlot(node);
    assert(cash_slot_index != _countof(inter_.cash_slots_));
    node->pos_in_container_ = PerkDataList::kCashSlotPositions[cash_slot_index];
    //
    inter_.request_info_.Clear();
    //
    if (SCPerkSlotContainer* perk_container = perk_messenger->GetPerkSlotContainer())
    {
        SCPerkSlot* cash_perk_slot = static_cast<SCPerkSlot*>(perk_container->CreateSlot());
        cash_perk_slot->Copy(*node);
        perk_container->InsertSlot(node->pos_in_container_, *cash_perk_slot);
        delete cash_perk_slot;
    }
    return true;
}

bool PerkDataContainer::ProcessUproot(const PerkControl& reply_header, PerkControlData* data,
                                      IPerkControlMessenger* perk_messenger)
{
    __TOUCHED(data);
    // (NOTE) a message handling logic for client parts is not check a 'request_info_' field
    // of a PerkDataNode, because of to handle server single side command
    const PerkControl::Req_Uproot& uproot = reply_header.args.uproot;
    if (kClientLogic)
    {
        const PerkID uproot_node_id(uproot.perk_uid);
        PerkTable::iterator uproot_node_found = perk_table_.find(uproot_node_id.group_);
        if (!FlowControl::FCAssert(uproot_node_found != perk_table_.end())) {
            return false;
        }
        PerkDataNode* uproot_node = server_data_.uproot.uprooting_node = &uproot_node_found->second;
        //
        PerkID find_registered_active_id;
        bool is_activated = IsActivatedSlot(&find_registered_active_id, uproot_node);
        PerkDataNode* active_node = is_activated == false ? \
            NULL : &perk_table_.find(find_registered_active_id.group_)->second; //assert
        server_data_.uproot.active_node_if_registered = active_node;
    };
    //
    PerkDataNode* uproot_node = server_data_.uproot.uprooting_node;
    PerkDataNode* active_node = server_data_.uproot.active_node_if_registered;
    //
    const bool is_activated = (active_node != NULL);
    SCPerkSlotContainer* const perk_container = perk_messenger->GetPerkSlotContainer();
    if (is_activated)
    {
        BatchCalculateAttrActivePerks(false, perk_messenger);
        if (perk_container)
        {
            SCPerkSlot& active_slot = \
                static_cast<SCPerkSlot&>(perk_container->GetSlot(active_node->pos_in_container_));
            active_slot.Unlink();
            perk_container->UpdateSlot(active_node->pos_in_container_, -1);
        }
        active_node->link_code_ = 0;
        FreeActiveSlot(active_node->perk_id_, NULL);
    }
    //
    if (perk_container) {
        perk_container->DeleteSlot(uproot_node->pos_in_container_, NULL);
    }
    //
    FreeCashSlot(uproot_node);
    perk_table_.erase(uproot_node->perk_id_.group_);
    // corrupt data : uproot_node
    //
    inter_.request_info_.Clear();
    if (is_activated) {
        BatchCalculateAttrActivePerks(true, perk_messenger);
        if (IPerkControlEvent* event_notifier = perk_messenger->GetPerkControlEvent()) {
            event_notifier->OnChangeAttributes();
        }
    }

    return true;
}

bool PerkDataContainer::ProcessRegister(const PerkControl& reply_header, PerkControlData* data,
                                        IPerkControlMessenger* perk_messenger)
{
    __TOUCHED(data);
    // (NOTE) a message handling logic for client parts is not check a 'request_info_' field
    // of a PerkDataNode, because of to handle server single side command
    const PerkControl::Req_Register& regist = reply_header.args.regist;
    if (kClientLogic)
    {
        const PerkID regist_node_id(regist.perk_uid);
        PerkTable::iterator regist_node_found = perk_table_.find(regist_node_id.group_);
        if (!FlowControl::FCAssert(regist_node_found != perk_table_.end())) {
            return false;
        }
        server_data_.regist.registering_node = &regist_node_found->second;
        //
        const PerkID active_node_id(regist.active_slot_uid);
        PerkTable::iterator active_node_found = perk_table_.find(active_node_id.group_);
        if (!FlowControl::FCAssert(active_node_found != perk_table_.end())) {
            return false;
        }
        server_data_.regist.active_node = &active_node_found->second;
    }
    //
    PerkDataNode* regist_node = server_data_.regist.registering_node;
    PerkDataNode* active_node = server_data_.regist.active_node;
    //
    BatchCalculateAttrActivePerks(false, perk_messenger);
    //
    active_node->link_code_ = regist_node->perk_id_.group_;
    AllocActiveSlot(active_node->perk_id_, regist_node);
    //
    inter_.request_info_.Clear();
    //
    BatchCalculateAttrActivePerks(true, perk_messenger);
    //
    if (SCPerkSlotContainer* perk_container = perk_messenger->GetPerkSlotContainer())
    {
        SCPerkSlot& active_slot = \
            static_cast<SCPerkSlot&>(perk_container->GetSlot(active_node->pos_in_container_));
        const SCPerkSlot& regist_slot = \
            static_cast<SCPerkSlot&>(perk_container->GetSlot(regist_node->pos_in_container_));
        active_slot.LinkSlot(&regist_slot);
        perk_container->UpdateSlot(active_node->pos_in_container_, 1);
    }
    if (IPerkControlEvent* event_notifier = perk_messenger->GetPerkControlEvent()) {
        event_notifier->OnChangeAttributes();
    }

    return true;
}

bool PerkDataContainer::ProcessUnregister(const PerkControl& reply_header, PerkControlData* data,
                                          IPerkControlMessenger* perk_messenger)
{
    __TOUCHED(data);
    // (NOTE) a message handling logic for client parts is not check a 'request_info_' field
    // of a PerkDataNode, because of to handle server single side command
    const PerkControl::Req_Unregister& unregist = reply_header.args.unregist;
    if (kClientLogic)
    {
        const PerkID active_node_id(unregist.perk_uid);
        PerkTable::iterator active_node_found = perk_table_.find(active_node_id.group_);
        if (!FlowControl::FCAssert(active_node_found != perk_table_.end())) {
            return false;
        }
        PerkDataNode* active_node = &active_node_found->second;
        assert(active_node->link_code_);
        const PerkID unregist_node_id(active_node->link_code_);
        PerkTable::iterator unregist_node_found = perk_table_.find(unregist_node_id.group_);
        if (!FlowControl::FCAssert(unregist_node_found != perk_table_.end())) {
            return false;
        }
        //
        server_data_.unregist.active_node_for_unregist = active_node;
        server_data_.unregist.unregistering_node = &unregist_node_found->second;
    }
    //
    PerkDataNode* active_node = server_data_.unregist.active_node_for_unregist;
    PerkDataNode* unregist_node = server_data_.unregist.unregistering_node;
    //
    BatchCalculateAttrActivePerks(false, perk_messenger);
    SCPerkSlotContainer* const perk_container = perk_messenger->GetPerkSlotContainer();
    if (perk_container)
    {
        SCPerkSlot& active_slot = \
            static_cast<SCPerkSlot&>(perk_container->GetSlot(active_node->pos_in_container_));
        active_slot.Unlink();
        perk_container->UpdateSlot(active_node->pos_in_container_, -1);
    }
    //
    active_node->link_code_ = 0;
    FreeActiveSlot(active_node->perk_id_, unregist_node);
    //
    inter_.request_info_.Clear();
    BatchCalculateAttrActivePerks(true, perk_messenger);
    if (IPerkControlEvent* event_notifier = perk_messenger->GetPerkControlEvent()) {
        event_notifier->OnChangeAttributes();
    }

    return true;
}

bool PerkDataContainer::ProcessDelete(const PerkControl& reply_header, PerkControlData* data,
                                      IPerkControlMessenger* perk_messenger)
{
    __TOUCHED(data);
    // (NOTE) a message handling logic for client parts is not check a 'request_info_' field
    // of a PerkDataNode, because of to handle server single side command
    const PerkControl::Req_Delete& deletion = reply_header.args.deletion;
    if (kClientLogic)
    {
        const PerkID delete_node_id(deletion.perk_uid);
        PerkTable::iterator delete_node_found = perk_table_.find(delete_node_id.group_);
        if (!FlowControl::FCAssert(delete_node_found != perk_table_.end())) {
            return false;
        }
        PerkDataNode* deleting_node = \
            server_data_.deletion.deleting_node = &delete_node_found->second;
        //
        PerkID activated_perk_id;
        bool is_activated = IsActivatedSlot(&activated_perk_id, deleting_node);
        PerkDataNode* active_node = is_activated == false \
            ?  NULL : &perk_table_.find(activated_perk_id.group_)->second; //assert
        server_data_.deletion.active_node_if_registered = active_node;
    }
    //
    PerkDataNode* deleting_node = server_data_.deletion.deleting_node;
    PerkDataNode* active_node = server_data_.deletion.active_node_if_registered;
    const bool is_activated = (active_node != NULL);
    if (is_activated)
    {
        BatchCalculateAttrActivePerks(false, perk_messenger);
        if (SCPerkSlotContainer* perk_container = perk_messenger->GetPerkSlotContainer())
        {
            SCPerkSlot& active_slot = \
                static_cast<SCPerkSlot&>(perk_container->GetSlot(active_node->pos_in_container_));
            active_slot.Unlink();
            perk_container->UpdateSlot(active_node->pos_in_container_, -1);
        }
        active_node->link_code_ = 0;
        FreeActiveSlot(active_node->perk_id_, deleting_node);
    };
    if (SCPerkSlotContainer* perk_container = perk_messenger->GetPerkSlotContainer())
    {
        perk_container->DeleteSlot(deleting_node->pos_in_container_, NULL);
    }

    FreeCashSlot(deleting_node);
    perk_table_.erase(deleting_node->perk_id_.group_);
    // corrupt data = deleting_node
    //
    inter_.request_info_.Clear();
    //
    if (is_activated) {
        BatchCalculateAttrActivePerks(true, perk_messenger);
        if (IPerkControlEvent* event_notifier = perk_messenger->GetPerkControlEvent()) {
            event_notifier->OnChangeAttributes();
        }
    }

    return true;
}

bool PerkDataContainer::ProcessEventMessage(const PerkControl& update_header, PerkStatusData* data,
                                            IPerkControlMessenger* perk_messenger)
{
    if (kClientLogic)
    {
        // (NOTE) a message handling logic for client parts is not check a 'request_info_' field
        // of a PerkDataNode, because of to handle server single side command
        switch (update_header.request)
        {
        case update_header.eReq_Status:
            {
                // change to stack buffer to escape critical section.
                PerkStatusData copied_data = *data;
                return ProcessEvent(update_header, &copied_data, perk_messenger);
            };
            break;
        default:
            assert(!"function not implementation");
            break;
        }
    };
    return false;
}

bool PerkDataContainer::ProcessEvent(const PerkControl& reply_header, PerkStatusData* data,
                                     IPerkControlMessenger* perk_messenger)
{
    __TOUCHED(&reply_header);
    // NOTE: level up event handling
    const PerkStatusData::StatusList& status_list = data->status_list;
    if (status_list.number_of_nodes_ == 0) {
        return true;
    };
    bool exist_level_change_event = false;
    // '> 0' : levelup, '< 0' : leveldown
    int8_t level_change_events[_countof(inter_.active_slots_)] = { 0, };
    //
    const PerkStatusData::__Status* status_it = status_list.nodes_,
        * const status_end = status_list.nodes_ + status_list.number_of_nodes_;
    for (int index = 0; status_it != status_end; ++index, ++status_it)
    {
        if (status_it->index_ >= _countof(inter_.active_slots_)) {
            assert(!"unexpected status: index out of ranges");
            continue;
        };
        const PerkDataNode* active_node = inter_.active_slots_[status_it->index_];
        if (active_node == NULL) {
            assert(!"unexpected status: must be exist an active perk");
            continue;
        };
        PerkDataNode* active_node_edit = const_cast<PerkDataNode*>(active_node);
        if (kClientLogic) {
            active_node_edit->exp_ = status_it->exp_;
        };
        if (active_node->perk_info_->max_level_ == false)
        {
            if (EXPTYPE(active_node->exp_) >= active_node->perk_info_->next_level_exp_) {
                // levelup
                ++level_change_events[index];
            }
        };
        if (active_node->perk_id_.level_ != 1 &&
            EXPTYPE(active_node->exp_) < active_node->perk_info_->exp_value_)
        {
            --level_change_events[index];
        }
        if (exist_level_change_event == false && level_change_events[index] != 0) {
            exist_level_change_event = true;
        };
    }; //end 'for'
    //
    if (exist_level_change_event) {
        BatchCalculateAttrActivePerks(false, perk_messenger);
    }
    // NOTE: this algorithm implemented changed level events based on step by step
    // +1 event, +1 event, +1 event
    // -1 event, -1 event, -1 event
    const PerkInfoParser* const perk_parser = PerkInfoParser::Instance();
    SCPerkSlotContainer* const perk_container = perk_messenger->GetPerkSlotContainer();
    //
    status_it = status_list.nodes_;
    for (int index = 0; status_it != status_end; ++index, ++status_it)
    {
        //
        const PerkDataNode* active_node = inter_.active_slots_[status_it->index_];
        if (level_change_events[index])
        {
            const PerkGroup* perk_group = perk_parser->FindPerkGroup(active_node->perk_id_);
            const PerkInfo* perk_info = perk_group->GetPerkInfoByExp(active_node->exp_);
            /*
            const PerkInfo* perk_info = NULL;
            // current level
            // perk_info = perk_group->perk_info_array_[active_node->perk_id_.level_ - 1];
            int modified_index = active_node->perk_id_.level_ - 1; // current;
            ;{
                (level_change_events[index] > 0) ? ++modified_index : --modified_index;
                perk_info = perk_group->perk_info_array_[modified_index];
                assert(modified_index >= 0 && modified_index < perk_group->number_of_perk_infos_ &&
                       perk_info != NULL);
            };
            */
            //
            if (PerkDataNode* active_node_edit = const_cast<PerkDataNode*>(active_node)) {
                active_node_edit->perk_id_ = perk_info->perk_id_;
                active_node_edit->perk_info_ = perk_info;
            };

#ifdef _NA_001990_ACSYSTEM_ADD
            if (level_change_events[index] > 0) // 레벨이 올랐을때..
            {
                if (IPerkControlEvent* event_notifier = perk_messenger->GetPerkControlEvent()) {
                    event_notifier->OnLevelup(perk_info->perk_id_);
                };
            }
#endif //_NA_001990_ACSYSTEM_ADD
        }
        if (perk_container)
        {   // client events
            SCPerkSlot& perk_slot = static_cast<SCPerkSlot&>(\
                perk_container->GetSlot(active_node->pos_in_container_));
            perk_slot.Copy(*active_node);
            perk_container->UpdateSlot(active_node->pos_in_container_, 0);
        }
    }; //end 'for'
    //
    if (exist_level_change_event) {
        BatchCalculateAttrActivePerks(true, perk_messenger);
        if (IPerkControlEvent* event_notifier = perk_messenger->GetPerkControlEvent()) {
            event_notifier->OnChangeAttributes();
            event_notifier->OnLevelup();
        }
    }

    return true;
}


}; //end of namespace

//==================================================================================================
#endif //_NA002217_100728_EP2_PERK_SYSTEM_