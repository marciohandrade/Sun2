#include "stdafx.h"

#define PROGRAMCOMMON_ITEM_ATTR_CALCULATOR_INTER_UTIL
#include "./ItemAttrCalculator_Util.hxx"
#undef PROGRAMCOMMON_ITEM_ATTR_CALCULATOR_INTER_UTIL

#if defined(_NA_002289_20110322_EQUIPMENTS_CALCULATOR_CHANGES_FOR_IMPROVEMENT)
namespace ns_formula {
;
}; //end of namespace
//==================================================================================================

Attributes::OperationRecord* ns_formula::ItemAttrContext::BeginRecord()
{
    assert(working_record == NULL);
    if (static_cast<size_t>(workable_it - begin_it) >=
        static_cast<size_t>(end_it - begin_it))
    {
        assert(!"index out of ranges");
        return NULL;
    };
    Attributes::OperationRecord* empty_record = workable_it;
    ZeroMemory(empty_record, sizeof(*empty_record));
    working_record = empty_record;
    return empty_record;
}

bool ns_formula::ItemAttrContext::CommitRecord()
{
    if (working_record == NULL) {
        assert(!"this record must be same working record");
        return false;
    };
    const size_t index = static_cast<size_t>(working_record - begin_it);
    if (index >= static_cast<size_t>(end_it - begin_it)) {
        assert(!"index out of ranges");
        return false;
    };
    ;{
        ++workable_it;
        working_record = NULL;
    };
    return true;
}

bool ns_formula::ItemAttrContext::RollbackRecord()
{
    if (working_record == NULL) {
        assert(!"this record must be same working record");
        return false;
    };
    const size_t index = static_cast<size_t>(working_record - begin_it);
    if (index >= static_cast<size_t>(end_it - begin_it)) {
        assert(!"index out of ranges");
        return false;
    };
    ;{
        //++number_of_records;
        working_record = NULL;
    };
    return true;
}

//==================================================================================================

ns_formula::ItemAttrContextList::ItemAttrContextList()
{
    ZeroMemory(this, static_cast<size_t>(reinterpret_cast<char*>(contexts) -
                                         reinterpret_cast<char*>(this)));
}

ns_formula::ItemAttrContext* ns_formula::ItemAttrContextList::BeginContext(POSTYPE equip_position)
{
    assert(working_context == NULL);
    if (number_of_contexts >= _countof(contexts)) {
        assert(number_of_contexts < _countof(contexts));
        return NULL;
    };
    if (ItemAttrContext* const context = &contexts[number_of_contexts])
    {
        ZeroMemory(context, sizeof(*context));
        context->equip_position = equip_position;
        context->begin_it = &total_records[number_of_total_records];
        context->workable_it = context->begin_it;
        context->end_it = &total_records[number_of_total_records + kMaxNumberOfContextRecords];
        context->parent = this;
        working_context = context;
    };
    return working_context;
}

bool ns_formula::ItemAttrContextList::CommitContext()
{
    if (working_context == NULL) {
        assert(!"can't find working context");
        return false;
    };
    const size_t context_index = static_cast<size_t>(working_context - contexts);
    if (context_index >= _countof(contexts) || context_index != number_of_contexts) {
        assert(!"index out of ranges");
        return false;
    };
    ;{
        size_t number_of_used_nodes =
            static_cast<size_t>(working_context->workable_it - working_context->begin_it);
        number_of_total_records += number_of_used_nodes;
        assert(number_of_total_records < _countof(total_records));
    };
    ;{
        ++number_of_contexts;
        working_context = NULL;
    };
    return true;
}

bool ns_formula::ItemAttrContextList::RollbackContext()
{
    if (working_context == NULL) {
        assert(!"can't find working context");
        return false;
    };
    const size_t index = static_cast<size_t>(working_context - contexts);
    if (index >= _countof(contexts) || index != number_of_contexts) {
        assert(!"index out of ranges");
        return false;
    };
    ;{
        //++number_of_contexts;
        working_context = NULL;
    };
    return true;
}

// [1, 2] operations per item
bool ns_formula::ItemAttrContextList::RecycleContexts()
{
    assert(working_context == NULL);
    number_of_contexts = 0;
    number_of_total_records = 0;
    return true;
}

//==================================================================================================

ns_formula::ItemAttrCalculatorInterControlAutoVariable::ItemAttrCalculatorInterControlAutoVariable(
    int* ref, eItemAttrCalculatorInterControl mode)
    : ref_field(ref)
    , backup_field(*ref_field)
{
    *ref_field = mode;
}

ns_formula::ItemAttrCalculatorInterControlAutoVariable::~ItemAttrCalculatorInterControlAutoVariable()
{
    *ref_field = backup_field;
}

//==================================================================================================

void ns_formula::SocketSearchBoard::Clear()
{
    ZeroMemory(this, static_cast<size_t>(reinterpret_cast<char*>(nodes) - 
                                         reinterpret_cast<char*>(this)));
    for (node_type* it = nodes; it != &nodes[_countof(nodes)]; ++it)
    {
        *it = node_type::kDefault;
    }
}

bool ns_formula::SocketSearchBoard::SearchSocketOptionPoint(
    const SCItemSlot& item_slot, ns_formula::SocketSearchBoard* search_board)
{
    WAVERIX_WRAPPER((WAVERIX_LC_CALL1("waverix_log_trace", __FUNCTION__)));
    typedef SocketSearchBoard::node_type result_node_t;
    //
    search_board->Clear(); // nodes not cleared;
    //
    const int number_of_sockets = item_slot.GetSocketNum();
    for (eSOCKET socket = SOCKET_1;
         socket < number_of_sockets;
         socket = static_cast<eSOCKET>(socket+1))
    {
        // 소켓 옵션 정보가 유효한지 검사한다.
        const sSOCKET_OPTION* const socket_option = item_slot.GetSocketOption(socket);
        if (socket_option == NULL) {
            continue;
        }

        const sSOCKET_OPTION::OptionPointInfo* point_it = NULL;
        const int number_of_points = socket_option->GetPointArrayInfo(&point_it);
        if (number_of_points == 0) {
            assert(!"exist invalid option");
            continue;
        };
        // 소켓 옵션의 각 옵션 포인트에 대해
        for (int point_index = 0; point_index < number_of_points; ++point_index, ++point_it)
        {
            if (point_it->option_point == 0) {
                assert(!"exist invalid option");
                continue;
            };
            // 옵션 정보가 유효한지 검사한다.
            const ZardOptionInfo* const zard_option = point_it->zard_option;
            if (zard_option == NULL || 
                zard_option->attr_option_code == eATTR_TYPE_INVALID || 
                zard_option->item_option_code == 0)
            {
                assert(!"exist invalid option");
                continue;
            }
            const eATTR_TYPE attr_type = zard_option->attr_option_code;
            if (attr_type >= eATTR_MAX) {
                assert(!"exist invalid option");
                continue;
            };
            eVALUE_TYPE value_type = point_it->zard_option->GetOptionType();
            if (value_type == 0 ||
                value_type >= _countof(ItemAttrCalculator::kAttrKindByValueTypes))
            {
                assert(!"exist invalid option");
                continue;
            };

            // 중복 옵션인지 검사해서 중복이면 포인트 수치를 합친다.
            result_node_t& result_node = search_board->nodes[attr_type];
            if (result_node.attr_option_code == eATTR_TYPE_INVALID)
            {
                result_node.attr_option_code = attr_type;
                result_node.item_option_code = zard_option->item_option_code;
                result_node.option_point = point_it->option_point;
                result_node.option_type = value_type;
                //result_node.option_value = 0;
                if (SocketSearchBoard::RegistNode* regist_node = \
                        &search_board->registered_nodes[search_board->number_of_registered])
                {
                    regist_node->link_node = &result_node;
                    regist_node->option_info = zard_option;
                    ++search_board->number_of_registered;
                }
            }
            else
            {
                result_node.option_point += point_it->option_point;
            }
            //
        };
    };
    //
    return (search_board->number_of_registered != 0);
}

//==================================================================================================
// NOTE: f110111.2L, magical defense values are used to increase it of all elements
// in the Episode II.
void ns_formula::InterAttackPowerCalculator::AddRatioWeight_Limited(float ratio)
{
    const float apply_rates = (ratio / 100.0f);
    float min_power = static_cast<float>(sum_of_min_power);
    float max_power = static_cast<float>(sum_of_max_power);
    const float correction_value = (ratio >= 0.0f) ? +0.5f : -0.5f;

    min_power += (apply_rates * min_power + correction_value);
    max_power += (apply_rates * max_power + correction_value);

    sum_of_min_power = static_cast<DWORD>(min_power);
    sum_of_max_power = static_cast<DWORD>(max_power);
}

// __NA_001062_20080623_STRENGTH_PENALTY
void ns_formula::InterAttackPowerCalculator::AddRatioWeight_Strength(float ratio)
{
    float min_power = static_cast<float>(sum_of_min_power);
    float max_power = static_cast<float>(sum_of_max_power);
    const float correction_value = (ratio >= 0.0f) ? +0.5f : -0.5f;

    min_power = (ratio * min_power + correction_value);
    max_power = (ratio * max_power + correction_value);

    sum_of_min_power = static_cast<DWORD>(min_power);
    sum_of_max_power = static_cast<DWORD>(max_power);
}

void ns_formula::InterAttackPowerCalculator::AddRatioWeight_Fate(float ratio)
{
    const float apply_rates = (ratio / 100.0f);
    float min_power = static_cast<float>(sum_of_min_power);
    float max_power = static_cast<float>(sum_of_max_power);
    const float correction_value = (ratio >= 0.0f) ? +0.5f : -0.5f;

    min_power += (apply_rates * min_power + correction_value);
    max_power += (apply_rates * max_power + correction_value);

    sum_of_min_power = static_cast<DWORD>(min_power);
    sum_of_max_power = static_cast<DWORD>(max_power);
}

//==================================================================================================
// NOTE: f110111.2L, magical defense values are used to increase it of all elements
// in the Episode II.
void ns_formula::InterDefensePowerCalculator::AddRatioWeight_Limited(float ratio)
{
    const float apply_rates = (ratio / 100.0f);
    float defense_power = static_cast<float>(sum_of_defense_power);
    const float correction_value = (ratio >= 0.0f) ? +0.5f : -0.5f;

    defense_power += (apply_rates * defense_power + correction_value);

    sum_of_defense_power = static_cast<DWORD>(defense_power);
}

// __NA_001062_20080623_STRENGTH_PENALTY
void ns_formula::InterDefensePowerCalculator::AddRatioWeight_Strength(float ratio)
{
    float defense_power = static_cast<float>(sum_of_defense_power);
    const float correction_value = (ratio >= 0.0f) ? +0.5f : -0.5f;

    defense_power = (ratio * defense_power + correction_value);

    sum_of_defense_power = static_cast<DWORD>(defense_power);
}

void ns_formula::InterDefensePowerCalculator::AddRatioWeight_Fate(float ratio)
{
    const float apply_rates = (ratio / 100.0f);
    float defense_power = static_cast<float>(sum_of_defense_power);
    const float correction_value = (ratio >= 0.0f) ? +0.5f : -0.5f;

    defense_power += (apply_rates * defense_power + correction_value);

    sum_of_defense_power = static_cast<DWORD>(defense_power);
}

//==================================================================================================
//==================================================================================================
//==================================================================================================

ns_formula::EquipSlotsManageInfo::EquipSlotsManageInfo()
{
    ZeroMemory(slots, sizeof(slots));
#ifdef _NA_002935_20110704_ITEM_SCORE
    item_score_info.slot_nodes = slots;
#endif
}

ns_formula::EquipSlotsManageInfo::~EquipSlotsManageInfo()
{
}

void ns_formula::EquipSlotsManageInfo::ClearAll()
{
    ZeroMemory(slots, sizeof(slots));
    ClearEquipCounterInfo();
#if USE_CLIENT_SOCKET_OPTION_TABLE
    // CHANGES: f110530.3L
    item_socket_option_lines_sorted.resize(0);
    item_socket_option_lines.resize(0);
    for (ItemSocketOptionList* it = item_socket_option_list;
         it != &item_socket_option_list[_countof(item_socket_option_list)];
         ++it)
    {
        it->resize(0);
    };
#endif
#ifdef _NA_002935_20110704_ITEM_SCORE
    item_score_info.Clear();
#endif
}

void ns_formula::EquipSlotsManageInfo::ClearCacheInfo()
{
    ZeroMemory(slots, sizeof(slots));
}

void ns_formula::EquipSlotsManageInfo::ClearEquipCounterInfo()
{
    FOREACH_CONTAINER(ItemEquipCounterTable::value_type& node, restrict_table_,
                      ItemEquipCounterTable)
    {
        EquipSlotsManageInfo::CounterNode* counter_node = &node.second;
        ZeroMemory(counter_node, sizeof(*counter_node));
    };
}


bool ns_formula::EquipSlotsManageInfo::IsEquipEventItem(BYTE event_code) const
{
    ItemEquipCounter key_node = { 0, };
    key_node.key_value.event_info.event_code = event_code;
    key_node.key_value.event_info.tag = key_node.eEquipEvent;
    ItemEquipCounterTable::const_iterator it = restrict_table_.find(key_node.key_value.composed);
    if (it != restrict_table_.end())
    {
        const EquipSlotsManageInfo::CounterNode& counter_node = it->second;
        return (counter_node.number_of_contains + counter_node.post_apply_working_value) != 0;
    };

    return false;
}

int ns_formula::EquipSlotsManageInfo::GetEquipCount(SLOTCODE item_code) const
{
    ItemEquipCounter key_node = { 0, };
    key_node.key_value.code_info.item_code = item_code;
    key_node.key_value.code_info.tag = key_node.eItemCode;
    ItemEquipCounterTable::const_iterator it = restrict_table_.find(key_node.key_value.composed);
    if (it == restrict_table_.end()) {
        return 0;
    };
    const EquipSlotsManageInfo::CounterNode& counter_node = it->second;
    int number_of_same_items =
        (counter_node.number_of_contains + counter_node.post_apply_working_value);
    return number_of_same_items;
}

int ns_formula::EquipSlotsManageInfo::GetEquipTypeCount(eITEMTYPE item_type) const
{
    ItemEquipCounter key_node = { 0, };
    key_node.key_value.type_info.item_type = static_cast<uint16_t>(item_type);
    key_node.key_value.type_info.tag = key_node.eItemType;
    ItemEquipCounterTable::const_iterator it = restrict_table_.find(key_node.key_value.composed);
    if (it == restrict_table_.end()) {
        return 0;
    };
    const EquipSlotsManageInfo::CounterNode& counter_node = it->second;
    int number_of_same_type_items =
        (counter_node.number_of_contains + counter_node.post_apply_working_value);
    return number_of_same_type_items;
}

//그룹별 장착 개수 제한 관리
// (NOTE) it has not the usage case at this point in time (100505)
int ns_formula::EquipSlotsManageInfo::GetEquipGroupCount(BYTE equip_group) const
{
    ItemEquipCounter key_node = { 0, };
    key_node.key_value.group_info.group = equip_group;
    key_node.key_value.group_info.tag = key_node.eEquipGroup;
    ItemEquipCounterTable::const_iterator it = restrict_table_.find(key_node.key_value.composed);
    if (it == restrict_table_.end()) {
        return 0;
    };
    const EquipSlotsManageInfo::CounterNode& counter_node = it->second;
    int number_of_same_group_items =
        (counter_node.number_of_contains + counter_node.post_apply_working_value);
    return number_of_same_group_items;
}

#ifdef _NA_001990_ACSYSTEM_ADD
int ns_formula::EquipSlotsManageInfo::GetEquipACGroupCount(WORD item_acgroup) const
{
    ItemEquipCounter key_node = { 0, };
    key_node.key_value.ac_group_info.ac_group = item_acgroup;
    key_node.key_value.ac_group_info.tag = key_node.eACGroup;
    ItemEquipCounterTable::const_iterator it = restrict_table_.find(key_node.key_value.composed);
    if (it == restrict_table_.end()) {
        return 0;
    };
    const EquipSlotsManageInfo::CounterNode& counter_node = it->second;
    int number_of_same_group_items =
        (counter_node.number_of_contains + counter_node.post_apply_working_value);
    return number_of_same_group_items;
}
#endif //_NA_001990_ACSYSTEM_ADD

//==================================================================================================
// CHANGES: f110422.5L, added filter to check the item relational dependency rules
bool ns_formula::EquipSlotsManageInfo::CanEquipmentsDependency(const SCItemSlot& item_slot) const
{
    const BASE_ITEMINFO* const item_info = item_slot.GetItemInfo();
    // 중복장착 개수 체크(0이면 무제한)
    if (item_info->m_byDupEquipNum)
    {
        const int number_of_same_items = GetEquipCount(item_info->m_Code);
        if (number_of_same_items >= item_info->m_byDupEquipNum) {
            return false;
        }
    };
    // 중복타입 장착 개수 체크
    if (item_info->m_byDupEquipTypeNum)
    {
        const int number_of_same_items =
            GetEquipTypeCount(static_cast<eITEMTYPE>(item_info->m_wType));
        if (number_of_same_items >= 1)
            return false;
    };
    // 아이템 그룹별 개수 제한 체크
    if (item_info->m_byDupEquipGroup)
    {
        const int number_of_same_items = GetEquipGroupCount(item_info->m_byDupEquipGroup);
        if (number_of_same_items >= item_info->m_byDupEquipGroupNum) {
            return false;
        }
    };
    return true;
}

//==================================================================================================

void ns_formula::EquipSlotsManageInfo::IncreaseEquipInfo(const SCItemSlot& item_slot)
{
    const BASE_ITEMINFO* item_info = item_slot.GetItemInfo();
    if (item_info->m_Code == 0) {
        return;
    };

    ;{
        ItemEquipCounter key_node = { 0, };
        key_node.key_value.code_info.item_code = item_info->m_Code;
        key_node.key_value.code_info.tag = key_node.eItemCode;
        EquipSlotsManageInfo::CounterNode* counter_node =
            &restrict_table_[key_node.key_value.composed];
        ++counter_node->number_of_contains;
    };
    if (item_info->m_wType)
    {
        ItemEquipCounter key_node = { 0, };
        key_node.key_value.type_info.item_type = item_info->m_wType;
        key_node.key_value.type_info.tag = key_node.eItemType;
        EquipSlotsManageInfo::CounterNode* counter_node =
            &restrict_table_[key_node.key_value.composed];
        ++counter_node->number_of_contains;
    };
    if (item_info->m_byDupEquipGroup)
    {
        ItemEquipCounter key_node = { 0, };
        key_node.key_value.group_info.group = item_info->m_byDupEquipGroup;
        key_node.key_value.group_info.tag = key_node.eEquipGroup;
        EquipSlotsManageInfo::CounterNode* counter_node =
            &restrict_table_[key_node.key_value.composed];
        ++counter_node->number_of_contains;
    }
    if (item_info->m_byEventCode)
    {
        ItemEquipCounter key_node = { 0, };
        key_node.key_value.event_info.event_code = item_info->m_byEventCode;
        key_node.key_value.event_info.tag = key_node.eEquipEvent;
        EquipSlotsManageInfo::CounterNode* counter_node =
            &restrict_table_[key_node.key_value.composed];
        ++counter_node->number_of_contains;
    }
#ifdef _NA_001990_ACSYSTEM_ADD
    if (item_info->m_byACReferenceID)
    {
        ItemEquipCounter key_node = { 0, };
        key_node.key_value.ac_group_info.ac_group = item_info->m_byACReferenceID;
        key_node.key_value.ac_group_info.tag = key_node.eEquipGroup;
        EquipSlotsManageInfo::CounterNode* counter_node =
            &restrict_table_[key_node.key_value.composed];
        ++counter_node->number_of_contains;
    }
#endif //_NA_001990_ACSYSTEM_ADD
}

void ns_formula::EquipSlotsManageInfo::DecreaseEquipInfo(const SCItemSlot& item_slot)
{
    const BASE_ITEMINFO* item_info = item_slot.GetItemInfo();
    if (item_info->m_Code == 0) {
        return;
    };

    ;{
        ItemEquipCounter key_node = { 0, };
        key_node.key_value.code_info.item_code = item_info->m_Code;
        key_node.key_value.code_info.tag = key_node.eItemCode;
        ItemEquipCounterTable::iterator it = restrict_table_.find(key_node.key_value.composed);
        if (it != restrict_table_.end())
        {
            EquipSlotsManageInfo::CounterNode* counter_node = &it->second;
            if (counter_node->number_of_contains) {
                --counter_node->number_of_contains;
            };
            if (counter_node->number_of_contains == 0) {
                restrict_table_.erase(it);
            };
        };
    };
    if (item_info->m_wType)
    {
        ItemEquipCounter key_node = { 0, };
        key_node.key_value.type_info.item_type = item_info->m_wType;
        key_node.key_value.type_info.tag = key_node.eItemType;
        ItemEquipCounterTable::iterator it = restrict_table_.find(key_node.key_value.composed);
        if (it != restrict_table_.end())
        {
            EquipSlotsManageInfo::CounterNode* counter_node = &it->second;
            if (counter_node->number_of_contains) {
                --counter_node->number_of_contains;
            };
            if (counter_node->number_of_contains == 0) {
                restrict_table_.erase(it);
            };
        };
    };
    if (item_info->m_byDupEquipGroup)
    {
        ItemEquipCounter key_node = { 0, };
        key_node.key_value.group_info.group = item_info->m_byDupEquipGroup;
        key_node.key_value.group_info.tag = key_node.eEquipGroup;
        ItemEquipCounterTable::iterator it = restrict_table_.find(key_node.key_value.composed);
        if (it != restrict_table_.end())
        {
            EquipSlotsManageInfo::CounterNode* counter_node = &it->second;
            if (counter_node->number_of_contains) {
                --counter_node->number_of_contains;
            };
            if (counter_node->number_of_contains == 0) {
                restrict_table_.erase(it);
            };
        };
    }
    if (item_info->m_byEventCode)
    {
        ItemEquipCounter key_node = { 0, };
        key_node.key_value.event_info.event_code = item_info->m_byEventCode;
        key_node.key_value.event_info.tag = key_node.eEquipEvent;
        ItemEquipCounterTable::iterator it = restrict_table_.find(key_node.key_value.composed);
        if (it != restrict_table_.end())
        {
            EquipSlotsManageInfo::CounterNode* counter_node = &it->second;
            if (counter_node->number_of_contains) {
                --counter_node->number_of_contains;
            };
            if (counter_node->number_of_contains == 0) {
                restrict_table_.erase(it);
            };
        };
    }
#ifdef _NA_001990_ACSYSTEM_ADD
    if (item_info->m_byACReferenceID)
    {
        ItemEquipCounter key_node = { 0, };
        key_node.key_value.ac_group_info.ac_group = item_info->m_byACReferenceID;
        key_node.key_value.ac_group_info.tag = key_node.eACGroup;
        ItemEquipCounterTable::iterator it = restrict_table_.find(key_node.key_value.composed);
        if (it != restrict_table_.end())
        {
            EquipSlotsManageInfo::CounterNode* counter_node = &it->second;
            if (counter_node->number_of_contains) {
                --counter_node->number_of_contains;
            };
            if (counter_node->number_of_contains == 0) {
                restrict_table_.erase(it);
            };
        };
    }
#endif //_NA_001990_ACSYSTEM_ADD
}

//==================================================================================================
// NOTE: f110530.3L
namespace ns_formula {
;
EquipSlotsManageInfo::ItemScoreInfo::ItemScoreInfo() : 
    slot_nodes(NULL), socket_score(0), set_option_score(0), total_score(0)
{
    ::ZeroMemory(each_item_score, sizeof(each_item_score));
}

void EquipSlotsManageInfo::ItemScoreInfo::Clear()
{
    socket_score = 0;
    set_option_score = 0;
    total_score = 0;
    ::ZeroMemory(each_item_score, sizeof(each_item_score));
}

int EquipSlotsManageInfo::ItemScoreInfo::GetScore(const POSTYPE equip_position) const
{
    enum { kMaxWeight = 99999 };
    if (equip_position >= _countof(each_item_score))
    {
        return 0;
    }
    assert(slot_nodes != NULL);
    const Node& slot_node = *(slot_nodes + equip_position);
    if (slot_node.activated == false)
    {
        return 0;
    }
    const Weight item_score = each_item_score[equip_position];
    if (item_score < 0)
    {
        return 0;
    }
    if (item_score > kMaxWeight)
    {
        return kMaxWeight;
    }
    return static_cast<int>(item_score + 0.5f);
}

int EquipSlotsManageInfo::ItemScoreInfo::GetTotalScore() const
{
    enum { kMaxWeight = 99999 };
    if (total_score < 0)
    {
        return 0;
    }
    if (total_score > kMaxWeight)
    {
        return kMaxWeight;
    }
    return static_cast<int>(total_score + 0.5f);
}

void EquipSlotsManageInfo::ItemScoreInfo::UpdateTotalScore()
{
    total_score = 0;
    assert(slot_nodes != NULL);
    for (int i = 0; i < _countof(each_item_score); ++i)
    {
        const Node& slot_node = *(slot_nodes + i);
        if (slot_node.activated == false)
        {
            continue;
        }
        total_score += each_item_score[i];
    }
    total_score += socket_score;
    total_score += set_option_score;
}

struct SocketOptionMappingTableUtil
{
    typedef ns_formula::EquipSlotsManageInfo::ItemSocketOption ItemSocketOption;
    typedef ns_formula::EquipSlotsManageInfo::ItemSocketOptionList ItemSocketOptionList;
    typedef ns_formula::EquipSlotsManageInfo::ItemSocketOptionLine ItemSocketOptionLine;
    typedef ns_formula::EquipSlotsManageInfo::ItemSocketOptionLineList ItemSocketOptionLineList;
    typedef ns_formula::EquipSlotsManageInfo::ItemSocketOptionLineListSorted
        ItemSocketOptionLineListSorted;
    //
    SocketOptionMappingTableUtil(ItemSocketOptionLineList* const line_list,
                                 ItemSocketOptionLineListSorted* const sorted_line_list);
    ItemSocketOptionLine* FindOptionLine(eATTR_TYPE attr_type);
    bool AddItemSocketOptionList(POSTYPE equip_pos, const ItemSocketOptionList& item_socket_options);
    bool SortBySumOfPoints();
    static bool DescendingOrderComparator(
        const ItemSocketOptionLine* lhs, const ItemSocketOptionLine* rhs);
    //
    ItemSocketOptionLineList* const line_list_;
    ItemSocketOptionLineListSorted* const sorted_line_list_;
    //
    __DISABLE_COPY(SocketOptionMappingTableUtil);
};

}; //end of namespace

ns_formula::SocketOptionMappingTableUtil::SocketOptionMappingTableUtil(
    ItemSocketOptionLineList* const line_list,
    ItemSocketOptionLineListSorted* const sorted_line_list)
    : line_list_(line_list)
    , sorted_line_list_(sorted_line_list)
{
    line_list_->resize(0);
    sorted_line_list_->resize(0);
};

ns_formula::SocketOptionMappingTableUtil::ItemSocketOptionLine*
ns_formula::SocketOptionMappingTableUtil::FindOptionLine(eATTR_TYPE attr_type)
{
    FOREACH_CONTAINER(ItemSocketOptionLineList::value_type& node, (*line_list_),
                      ItemSocketOptionLineList)
    {
        if (node.attr_type == attr_type) {
            return &node;
        };
    };
    return NULL;
}

bool ns_formula::SocketOptionMappingTableUtil::AddItemSocketOptionList(
    POSTYPE equip_pos, const ItemSocketOptionList& item_socket_options)
{
    FOREACH_CONTAINER(const ItemSocketOptionList::value_type& node, item_socket_options,
                      ItemSocketOptionList)
    {
        const ItemSocketOption& option_type = node;
        ItemSocketOptionLine* working_line = FindOptionLine(option_type.attr_option_code);
        if (working_line == NULL)
        {
            line_list_->resize(line_list_->size() + 1);
            working_line = &line_list_->back();
        };
        working_line->attr_type = option_type.attr_option_code;
        working_line->options[equip_pos] = &option_type;
        working_line->sum_of_points += option_type.option_point;
    };

    return true;
}

bool ns_formula::SocketOptionMappingTableUtil::DescendingOrderComparator(
    const ItemSocketOptionLine* lhs, const ItemSocketOptionLine* rhs)
{
    return (lhs->sum_of_points > rhs->sum_of_points);
}

bool ns_formula::SocketOptionMappingTableUtil::SortBySumOfPoints()
{
    if (line_list_->empty()) {
        return true;
    };
    FOREACH_CONTAINER(const ItemSocketOptionLineList::value_type& node, (*line_list_),
                      ItemSocketOptionLineList)
    {
        sorted_line_list_->push_back(&node);
    };
    std::sort(sorted_line_list_->begin(), sorted_line_list_->end(), &DescendingOrderComparator);
    return true;
}

#if USE_CLIENT_SOCKET_OPTION_TABLE
bool ns_formula::EquipSlotsManageInfo::CalculateSocketOptionMappingTable()
{
    SocketOptionMappingTableUtil inter_util(
        &item_socket_option_lines, &item_socket_option_lines_sorted);
    //
    for (const ItemSocketOptionList* it = item_socket_option_list;
         it != &item_socket_option_list[_countof(item_socket_option_list)];
         ++it)
    {
        inter_util.AddItemSocketOptionList(static_cast<POSTYPE>(it - item_socket_option_list), *it);
    };

    return inter_util.SortBySumOfPoints();
}
#endif //USE_CLIENT_SOCKET_OPTION_TABLE
//==================================================================================================
BOOST_STATIC_ASSERT(sizeof(EquipSlotsManageInfoInterController) ==
                    sizeof(EquipSlotsManageInfo));

void ns_formula::EquipSlotsManageInfoInterController::IncreaseEquipInfoSimulation(
    const SCItemSlot& item_slot)
{
    const BASE_ITEMINFO* item_info = item_slot.GetItemInfo();
    if (item_info->m_Code == 0) {
        return;
    };

    ;{
        ItemEquipCounter key_node = { 0, };
        key_node.key_value.code_info.item_code = item_info->m_Code;
        key_node.key_value.code_info.tag = key_node.eItemCode;
        EquipSlotsManageInfo::CounterNode* counter_node =
            &restrict_table_[key_node.key_value.composed];
        ++counter_node->post_apply_working_value;
    };
    if (item_info->m_wType)
    {
        ItemEquipCounter key_node = { 0, };
        key_node.key_value.type_info.item_type = item_info->m_wType;
        key_node.key_value.type_info.tag = key_node.eItemType;
        EquipSlotsManageInfo::CounterNode* counter_node =
            &restrict_table_[key_node.key_value.composed];
        ++counter_node->post_apply_working_value;
    };
    if (item_info->m_byDupEquipGroup)
    {
        ItemEquipCounter key_node = { 0, };
        key_node.key_value.group_info.group = item_info->m_byDupEquipGroup;
        key_node.key_value.group_info.tag = key_node.eEquipGroup;
        EquipSlotsManageInfo::CounterNode* counter_node =
            &restrict_table_[key_node.key_value.composed];
        ++counter_node->post_apply_working_value;
    }
    if (item_info->m_byEventCode)
    {
        ItemEquipCounter key_node = { 0, };
        key_node.key_value.event_info.event_code = item_info->m_byEventCode;
        key_node.key_value.event_info.tag = key_node.eEquipEvent;
        EquipSlotsManageInfo::CounterNode* counter_node =
            &restrict_table_[key_node.key_value.composed];
        ++counter_node->post_apply_working_value;
    }
}

void ns_formula::EquipSlotsManageInfoInterController::DecreaseEquipInfoSimulation(
    const SCItemSlot& item_slot)
{
    const BASE_ITEMINFO* item_info = item_slot.GetItemInfo();
    if (item_info->m_Code == 0) {
        return;
    };

    ;{
        ItemEquipCounter key_node = { 0, };
        key_node.key_value.code_info.item_code = item_info->m_Code;
        key_node.key_value.code_info.tag = key_node.eItemCode;
        ItemEquipCounterTable::iterator it = restrict_table_.find(key_node.key_value.composed);
        if (it != restrict_table_.end())
        {
            EquipSlotsManageInfo::CounterNode* counter_node = &it->second;
            --counter_node->post_apply_working_value;
        };
    };
    if (item_info->m_wType)
    {
        ItemEquipCounter key_node = { 0, };
        key_node.key_value.type_info.item_type = item_info->m_wType;
        key_node.key_value.type_info.tag = key_node.eItemType;
        ItemEquipCounterTable::iterator it = restrict_table_.find(key_node.key_value.composed);
        if (it != restrict_table_.end())
        {
            EquipSlotsManageInfo::CounterNode* counter_node = &it->second;
            --counter_node->post_apply_working_value;
        };
    };
    if (item_info->m_byDupEquipGroup)
    {
        ItemEquipCounter key_node = { 0, };
        key_node.key_value.group_info.group = item_info->m_byDupEquipGroup;
        key_node.key_value.group_info.tag = key_node.eEquipGroup;
        ItemEquipCounterTable::iterator it = restrict_table_.find(key_node.key_value.composed);
        if (it != restrict_table_.end())
        {
            EquipSlotsManageInfo::CounterNode* counter_node = &it->second;
            --counter_node->post_apply_working_value;
        };
    }
    if (item_info->m_byEventCode)
    {
        ItemEquipCounter key_node = { 0, };
        key_node.key_value.event_info.event_code = item_info->m_byEventCode;
        key_node.key_value.event_info.tag = key_node.eEquipEvent;
        ItemEquipCounterTable::iterator it = restrict_table_.find(key_node.key_value.composed);
        if (it != restrict_table_.end())
        {
            EquipSlotsManageInfo::CounterNode* counter_node = &it->second;
            --counter_node->post_apply_working_value;
        };
    }
}

// CHANGES: f110429.2L, removed. using external equipment limitation conditions can check
// whether this item enable equips.
#if SUN_CODE_BACKUP
// NOTE: f110424.2L, this mediator that additionally support the IEquipmentRestriction object
// process item duplication dependency rule conditions
// with CheckEquipDefaultValidationWithoutStats calling in the object
bool ns_formula::EquipSlotsManageInfoInterController::CheckEquipDefaultValidationWithoutStats(
    IEquipmentRestriction* restriction,
    POSTYPE at_pos, const SCItemSlot& IN check_slot)
{
    if (restriction->CheckEquipDefaultValidationWithoutStats(at_pos, check_slot) == false) {
        return false;
    };
    if (this->CanEquipmentsDependency(check_slot) == false) {
        return false;
    };

    return true;
}
#endif //
//==================================================================================================

//==================================================================================================
#endif //_NA_002289_20110322_EQUIPMENTS_CALCULATOR_CHANGES_FOR_IMPROVEMENT