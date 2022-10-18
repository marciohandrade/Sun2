#include "stdafx.h"
#include "./ItemAttrCalculator.h"

#if defined(_NA_002289_20110322_EQUIPMENTS_CALCULATOR_CHANGES_FOR_IMPROVEMENT)

#if !defined(_NA001605_EP2_ATTRIBUTES_RENEWAL_) || \
    !defined(_NA_000251_20100727_SOCKET_SYSTEM) || \
    !defined(_NA_001699_20110104_SOCKET_OPTION_POINT) || \
    !defined(_NA_0_20100517_ITEMSLOTCONTAINER_EQUIPGROUP_DETECTION)
    #error "Impossible Build!! - Reason: Violation Dependency Chain"
#endif

//==================================================================================================
// @history
//  - ~~~
//  - 09/10/24, waverix, code arrangement, remove warnings, Riding system
//  - ~10/08/10, anybody, major changes: worked about special accessory option calculations
//  - ~11/03/22, anybody, major changes: EP2 contents={perk, socket, preview}
//  - 11/03/22, waverix, item attributes calucator refactoring to reduce item dependency chains.
//  -           version control : __NA_001062_20080623_STRENGTH_PENALTY { China(x) }
//              remove '_ForeachAttrByRank'
//  - 11/03/29, waverix, updates renewaled code,
//              remains = { __NA_001062_20080623_STRENGTH_PENALTY, Socket, enable_option with time };

#define PROGRAMCOMMON_ITEM_ATTR_CALCULATOR_INTER_UTIL
#include "./ItemAttrCalculator_Util.hxx"
#undef PROGRAMCOMMON_ITEM_ATTR_CALCULATOR_INTER_UTIL

#include <EnchantParser.h>
#include "ItemFormula.h"
//
#include <SCSlotContainer.h>
#include <SCItemSlotContainer.h>
#ifdef _NA_002935_20110704_ITEM_SCORE
#include <ItemScoreParser.h>
#endif
//==================================================================================================
//==================================================================================================
//==================================================================================================

//                                                  [eVALUE_TYPE]
const eATTR_KIND ItemAttrCalculator::kAttrKindByValueTypes[6] =
{
    eATTR_KIND_ITEM,        // 0: undefined
    eATTR_KIND_ITEM,        // 1: VALUE_TYPE_VALUE // 고정 수치
	eATTR_KIND_ITEM_RATIO,  // 2: VALUE_TYPE_PERCENT_PER_CUR // 현재치 대비 비율
	eATTR_KIND_ITEM_RATIO,  // 3: VALUE_TYPE_PERCENT_PER_MAX // 최대치 대비 비율
    eATTR_KIND_ITEM,        // 4: undefined
	eATTR_KIND_ITEM,        // 5: VALUE_TYPE_RATIO_VALUE // 수치가 비율값인 경우(이동속도율)
};

const ns_formula::EquipSlotsManageInfo::ItemSocketOption
ns_formula::EquipSlotsManageInfo::ItemSocketOption::kDefault =
{
    eATTR_TYPE_INVALID, //eATTR_TYPE attr_option_code;
    eATTR_TYPE_INVALID, //ItemOptionCode item_option_code;
    0, //OptionPoint option_point;
    VALUE_TYPE_VALUE, //eVALUE_TYPE option_type;
    0 //OptionValue option_value;
};



//==================================================================================================
//==================================================================================================
//==================================================================================================
// 2013.01.18. - arycoat 
//              인벤토리에 있는 소지하고 있기만 해도 옵션을 적용하던 함수에서
//              인벤토리에서 사용/중지 가능한 아이템의 옵션도 적용할수 있도록 수정.
bool InvenItemOptionCalculator::ApplyInvenItemOption(
    PlayerAttributes& attributes,
    const SCItemSlot& item_slot, int changed_num,
    bool pc_room_status, bool use_update_ex)
{
    if (changed_num == 0) {
        return false;
    };
    const nsSlot::ItemRuleInfo& item_rule_info = item_slot.GetItemRuleInfo();
    if (item_rule_info.is_valid_item == false) {
        return false;
    };
    const BASE_ITEMINFO* item_info = item_slot.GetItemInfo();
    bool exist_inven_option = item_info->IsExistInvenOptions();
    bool exist_equip_option = item_info->IsExistEquipOptions();
    if ( exist_inven_option == false && exist_equip_option == false ) {
        return false;
    };
    // success ...
    //----------------------------------------------------------------------------------------------
    Attributes::OperationRecord option_records[BASE_ITEMINFO::_MAX_OPTION_NUM * 2] = { 0, };
    Attributes::OperationRecord* record_it = option_records;
    const Attributes::Operation::eOper oper_type \
        = (changed_num > 0) ? Attributes::Operation::eOper_Add
        :                     Attributes::Operation::eOper_Sub;
    //----------------------------------------------------------------------------------------------
    // (NOTE) calculate item option by script
    // (CHANGES) (WAVERIX) changes to more simple code
    const int upperbound_selector = pc_room_status ? 2 : 1; //Normal|PC option groups
    for (int selector = 0; selector < upperbound_selector; ++selector)
    {
        //const BASE_ITEMINFO::OptionEffects* __option_effects = selector == 0 ?
        //    &item_info->m_byOptionExEffect : &item_info->m_byPCOptionExEffect;

        //const BASE_ITEMINFO::OptionEffects& option_effects = *__option_effects;

        /*bool apply = (selector == 0 ? exist_inven_option : exist_equip_option);
        if (apply == false) {
            continue;
        }*/
        const BASE_ITEMINFO::OptionIndexes* __option_indexes = (selector == 0) ?
            &item_info->m_wOptionIndex : &item_info->m_wPCOptionIndex;
        const BASE_ITEMINFO::OptionTypes* __option_types = (selector == 0) ?
            &item_info->m_byOptionType : &item_info->m_byPCOptionType;
        const BASE_ITEMINFO::OptionValues* __option_values = (selector == 0) ?
            &item_info->m_iOptionValue : &item_info->m_iPCOptionValue;
        //
        const BASE_ITEMINFO::OptionIndexes& option_indexes = *__option_indexes;
        const BASE_ITEMINFO::OptionTypes& option_types = *__option_types;
        const BASE_ITEMINFO::OptionValues& option_values = *__option_values;

        for (int op = 0; op < _countof(option_indexes); ++op)
        {
            bool is_valid_values =
                option_indexes[op] != 0 && 
                option_values[op] != 0 &&
                option_indexes[op] < _countof(g_ITEM_OPTION) &&
                option_types[op] < _countof(ItemAttrCalculator::kAttrKindByValueTypes);
            if (is_valid_values == false) {
                continue;
            }
            //                                  notice about 'record_it'
            if (Attributes::OperationRecord* record = record_it++)
            {
                uint8_t attr_kind = static_cast<uint8_t>(\
                    ItemAttrCalculator::kAttrKindByValueTypes[option_types[op]]);
                // change to skill attribute
                attr_kind = attr_kind == eATTR_KIND_ITEM ?       eATTR_KIND_SKILL
                          : attr_kind == eATTR_KIND_ITEM_RATIO ? eATTR_KIND_SKILL_RATIO
                          :                                      attr_kind;

                record->operate = static_cast<uint8_t>(oper_type);
                record->attr_kind = attr_kind;
                record->attr_type = static_cast<uint16_t>(g_ITEM_OPTION[option_indexes[op]]);
                record->attr_value = option_values[op];
            };
        }; //end 'for option in options'
    };
    //
    const int number_of_options = static_cast<int>(record_it - option_records);
    if (number_of_options == 0) {
        return true;
    };
    const int loop_counter = std::abs(changed_num);
    for (int index = 0; index < loop_counter; ++index) {
        attributes.GroupBatchOperate(option_records, number_of_options);
    }    
    //----------------------------------------------------------------------------------------------
    if (use_update_ex) {
        attributes.UpdateEx();
    }
    else {
        attributes.Update();
    }
    return true;
}

//==================================================================================================
//==================================================================================================
//==================================================================================================
//
ItemAttrCalculator::ItemAttrCalculator(PlayerAttributes& attributes,
                                       IEquipmentRestriction* equipment_accessor)
    : attributes_(attributes)
    , socket_attributes_(attributes_.GetSocketOptionAttributes())
    , equip_container_(*equipment_accessor->GetEquipmentSlotContainer())
    , equipment_accessor_(equipment_accessor)
    , slot_cache_info_(equipment_accessor->GetEquipmentManageInfo())
    , pc_room_status_(false)
    , use_update_ex_(false)
    , inter_control_mode_(ns_formula::eInterControl_Default)
{

    WAVERIX_WRAPPER((WAVERIX_LC_CALL1("waverix_log_trace", __FUNCTION__)));
}

ItemAttrCalculator::~ItemAttrCalculator()
{

    WAVERIX_WRAPPER((WAVERIX_LC_CALL1("waverix_log_trace", __FUNCTION__)));
}

void ItemAttrCalculator::Clear()
{
    WAVERIX_WRAPPER((WAVERIX_LC_CALL1("waverix_log_trace", __FUNCTION__)));
    // f100720.3L
    socket_attributes_.Clear();
#ifdef _NA_002935_20110704_ITEM_SCORE
    attributes_.GetSetOptionAttributes().Clear();
#endif
    attributes_.ClearRelatedAttrKind(eATTR_KIND_ITEM);
    attributes_.Update();
}

// NOTE: f100812.2L, add an interface to support external module can access add rule filter
void ItemAttrCalculator::ModifyAttrKindAndCalculatedValue(uint8_t value_type, //eVALUE_TYPE
                                                          uint8_t* attr_kind, //eATTR_KIND
                                                          int * value)
{
    __TOUCHED(value);
    *attr_kind = eATTR_KIND_ITEM;
    //float calced_value = static_cast<float>(*value);

    if (value_type == VALUE_TYPE_PERCENT_PER_MAX ||
        value_type == VALUE_TYPE_PERCENT_PER_CUR)
    {
        *attr_kind = eATTR_KIND_ITEM_RATIO;
    }
    //
    //*value = static_cast<int>(calced_value);
}

//==================================================================================================

ItemAttrCalculator::EquipSlotNode::EquipSlotNode(
    const SCItemSlot& input_slot,
    const BASE_ITEMINFO* input_item_info,
    const nsSlot::ItemRuleInfo& input_item_rule_info)
    : item_slot(input_slot)
    , item_info(input_item_info)
    , item_rule_info(input_item_rule_info)
{
}

bool ItemAttrCalculator::EquipSlotNode::Initialize(
    eDetailedResult* result,
    EquipSlotNode* using_buffer,
    const SCItemSlot* slot)
{
    WAVERIX_WRAPPER((WAVERIX_LC_CALL1("waverix_log_trace", __FUNCTION__)));
    //
    ZeroMemory(using_buffer, sizeof(*using_buffer));
    if (slot == NULL) {
        *result = eResult_CantProcess;
        return false;
    };
    const SCItemSlot& item_slot = (*slot);
    const BASE_ITEMINFO* item_info = item_slot.GetItemInfo();
    if (item_info->m_Code == 0) {
        *result = eResult_CantProcess;
        return false;
    };
    const nsSlot::ItemRuleInfo& item_rule_info = item_slot.GetItemRuleInfo();
    if (item_rule_info.is_valid_item == false) {
        *result = eResult_CantProcess;
        return false;
    };

    // regenerate
    new (using_buffer) EquipSlotNode(item_slot, item_info, item_rule_info);
    // if is_accessory_wings then assert(is_accessory_special)
    const bool default_equips = item_rule_info.is_weapon || item_rule_info.is_armor ||
                                item_rule_info.is_accessory || item_rule_info.is_accessory_special;
    // NOTE: f100406.2L, waverix, fix up missing process on the inventory option effect
    // item which is not equal equip item that need applied option
    ;
    // success process
    if (default_equips == false)
    {
        if (item_info->IsExistEquipOptions() == false) {
            using_buffer->init_result = eResult_NoOptions;
            *result = eResult_NoOptions;
            return true;
        }
        using_buffer->init_result = eResult_OptionsOnly;
        *result = eResult_OptionsOnly;
        return true;
    }
    //
    using_buffer->init_result = eResult_AllProcess;
    *result = eResult_AllProcess;
    return true;
}

bool ItemAttrCalculator::UpdateEquipSlotsLatestInfo(
    bool activated,
    const EquipSlotNode& info_node,
    ns_formula::EquipSlotsManageInfo::Node* const work_node) const
{
    ZeroMemory(work_node, sizeof(*work_node));
    work_node->position = info_node.position;
    work_node->activated = activated;
    work_node->duration = info_node.duration;
    //
    return true;
}

// static
bool ItemAttrCalculator::LoadPlayerLimits(ns_formula::ItemAttrStats* const player_stats,
                                          const PlayerAttributes& attributes,
                                          LEVELTYPE player_level)
{
    const ns_formula::ItemAttrStats& kIndex = *player_stats; __TOUCHED(kIndex);
    //
    player_stats->level = player_level;
    player_stats->stats[kIndex.eSTAT_STR] = attributes[eATTR_STR];
    player_stats->stats[kIndex.eSTAT_DEX] = attributes[eATTR_DEX];
    player_stats->stats[kIndex.eSTAT_VIT] = attributes[eATTR_VIT];
    player_stats->stats[kIndex.eSTAT_INT] = attributes[eATTR_INT];
    player_stats->stats[kIndex.eSTAT_SPR] = attributes[eATTR_SPR];
    player_stats->stats[kIndex.eSTAT_DEC_LIMITS] = attributes[eATTR_DECREASE_LIMIT_STAT];
    //
    for (AttrValue* it = player_stats->stats;
         it != &player_stats->stats[_countof(player_stats->stats)];
         ++it)
    {
        it->skill_value_.value_ = 0;
        it->skill_ratio_.value_ = 0;
        // CHANGES: f110810.1L, fixed the unexpected option value calculation
        // that occur when a player has stat increasing buffs (skill)
        it->Update();
    }
    //
    return true;
}

//static
bool ItemAttrCalculator::LoadEquipmentLimits(ns_formula::ItemLimitations* const item_limits,
                                             const SCItemSlot& item_slot)
{
    //
    enum eLimitField {
        eLimitDefault = 0,
        eLimitDivine,
        eLimitLimited,
        eLimitCounts
    };
    const int limits_selector = item_slot.IsDivine() ?  eLimitDivine
                              : item_slot.IsLimited() ? eLimitLimited
                              :                         eLimitDefault;
    //
    const BASE_ITEMINFO* const item_info = item_slot.GetItemInfo();
    const ns_formula::ItemLimitations& kIndex = *item_limits;__TOUCHED(kIndex);
    short level = item_info->m_LimitEqLevel;
    ZeroMemory(item_limits, sizeof(*item_limits));
    ;{
        bool is_divine = (limits_selector == eLimitDivine);
        item_limits->limits[kIndex.eSTAT_STR] = is_divine ?
            item_info->m_wDivLimitStr : item_info->m_wLimitStr;
        item_limits->limits[kIndex.eSTAT_DEX] = is_divine ?
            item_info->m_wDivLimitDex : item_info->m_wLimitDex;
        item_limits->limits[kIndex.eSTAT_VIT] = is_divine ?
            item_info->m_wDivLimitVit : item_info->m_wLimitVit;
        item_limits->limits[kIndex.eSTAT_INT] = is_divine ?
            item_info->m_wDivLimitInt : item_info->m_wLimitInt;
        item_limits->limits[kIndex.eSTAT_SPR] = is_divine ?
            item_info->m_wDivLimitSpr : item_info->m_wLimitSpr;
    };

    if (limits_selector == eLimitLimited)
    {
        const float ratio = static_cast<float>(item_slot.GetLimitValueRatio());
        // NOTE: 0.001f = FLT_EPSILON
        const float kCorrection = (ratio < 0) ? (-0.5f - 0.001f) : (0.5f + 0.001f);
        if (level != 0)
        {
            float calc_limit = level + ((level * ratio) / 100.0f) + kCorrection;
            level = static_cast<short>(calc_limit);
        };
        // ranges = [eSTAT_STR:0, eSTAT_SPR:4]
        for (short* it = item_limits->limits;
             it != &item_limits->limits[kIndex.eSTAT_SIMUL_COUNTS - 1];
             ++it)
        {
            const short limit = *it;
            if (limit == 0) {
                continue;
            };
            float calc_limit = limit + ((limit * ratio) / 100.0f) + kCorrection;
            *it = static_cast<short>(calc_limit);
        }
    };

    const bool is_weapon = (item_info->IsWeapon() != false);
    const bool is_armor = (is_weapon == false) && item_info->IsArmor();
#ifdef _NA002823_WINGS_EQUIPMENTS_EXTENTION
    const bool is_wings = (item_info->m_wType == eITEMTYPE_WINGS);
#else
    const bool is_wings = false;
#endif
    // etc... accessories and enabled equip facility (pc room item, autobot, sign item...)

    if (is_weapon || is_armor || is_wings)
    {
        const BYTE enchant = item_slot.GetEnchant();
        const LEVELTYPE item_level = item_info->m_LV;
        float limit_ratio = 0.0f;
        if (EnchantParser* enchant_parser = EnchantParser::Instance()) {
            limit_ratio = enchant_parser->GetLimitRatio(item_level, enchant, is_weapon,
                                                        (limits_selector == eLimitLimited));
        }
        // ranges = [eSTAT_STR:0, eSTAT_SPR:4]
        for (short* it = item_limits->limits;
             it != &item_limits->limits[kIndex.eSTAT_SIMUL_COUNTS - 1];
             ++it)
        {
            const short limit = *it;
            if (limit == 0) {
                continue;
            };
            if (is_weapon) {
                *it = ::ns_formula::CalcLimitStatWeapon(\
                    limit, enchant, RANK_E, item_level, limit_ratio);
            }
            else if (is_armor || is_wings) {
                *it = ::ns_formula::CalcLimitStatArmor(\
                    limit, enchant, RANK_E, item_level, limit_ratio);
            }
        }
    };
    //
    item_limits->level = level;
    //
    return true;
}

//static
bool ItemAttrCalculator::CanEquipmentStatComparison(const ns_formula::ItemAttrStats& player_stats,
                                                    const ns_formula::ItemLimitations& item_limits)
{
    if (player_stats.level < item_limits.level) {
        return false;
    };
#if defined(_NA_002935_20110704_ITEM_SCORE) && !defined(_SERVER)
    //// 클라이언트에서 주변 플레이어 정보를 받을때 스텟 정보는 받지 않기 때문에 스텟은 0으로 초기화 되어 있다.
    //// 하지만 장비 계산 정보는 다른 컨텐츠에서 사용하기 때문에 성공 처리한다.
    // 클라이언트에서 상대방 장비보기 작업 중 다른 플레이어의 스탯에 대한 장비 계산을 해야 할 필요가 생겨서,
    // 히어로가 아닌 스탯 계산에서는 true를 리턴한다.(히어로가 아닌 플레이어는 스탯정보가 0으로 초기화되어 있다.)
    const bool is_other_player = (
        (player_stats.stats[player_stats.eSTAT_STR]).base_value_.value_ == 0 && 
        (player_stats.stats[player_stats.eSTAT_DEX]).base_value_.value_ == 0 &&
        (player_stats.stats[player_stats.eSTAT_VIT]).base_value_.value_ == 0 &&
        (player_stats.stats[player_stats.eSTAT_INT]).base_value_.value_ == 0 &&
        (player_stats.stats[player_stats.eSTAT_SPR]).base_value_.value_ == 0);
    if (is_other_player)
    {
        return true;
    }
#endif //defined(_NA_002935_20110704_ITEM_SCORE) && !defined(_SERVER)
    //
    const int decrease_limits = player_stats.stats[\
        player_stats.eSTAT_DEC_LIMITS].calc_value_.value_;
    //
    if (const AttrValue* limits = &player_stats.stats[player_stats.eSTAT_STR]) {
        if (limits->calc_value_.value_ + decrease_limits <
            item_limits.limits[item_limits.eSTAT_STR])
        {
            return false;
        }
    };
    if (const AttrValue* limits = &player_stats.stats[player_stats.eSTAT_DEX]) {
        if (limits->calc_value_.value_ + decrease_limits <
            item_limits.limits[item_limits.eSTAT_DEX])
        {
            return false;
        }
    };
    if (const AttrValue* limits = &player_stats.stats[player_stats.eSTAT_VIT]) {
        if (limits->calc_value_.value_ + decrease_limits <
            item_limits.limits[item_limits.eSTAT_VIT])
        {
            return false;
        }
    };
    if (const AttrValue* limits = &player_stats.stats[player_stats.eSTAT_INT]) {
        if (limits->calc_value_.value_ + decrease_limits <
            item_limits.limits[item_limits.eSTAT_INT])
        {
            return false;
        }
    };
    if (const AttrValue* limits = &player_stats.stats[player_stats.eSTAT_SPR]) {
        if (limits->calc_value_.value_ + decrease_limits <
            item_limits.limits[item_limits.eSTAT_SPR])
        {
            return false;
        }
    };
    return true;
}

bool ItemAttrCalculator::CanEquipmentValidStats(
    const SCItemSlot& item_slot,
    const PlayerAttributes& player_attr, LEVELTYPE player_level)
{
    ns_formula::ItemLimitations item_limits;
    ns_formula::ItemAttrStats player_stats;
    LoadPlayerLimits(&player_stats, player_attr, player_level);
    //
    if (LoadEquipmentLimits(&item_limits, item_slot) == false) {
        return false;
    }
    if (CanEquipmentStatComparison(player_stats, item_limits) == false) {
        return false;
    }
    return true;
}

// use : info_node.context
bool ItemAttrCalculator::CanEquipmentValidStatsInternal(const EquipSlotNode& info_node)
{
    ns_formula::ItemAttrContext* const context = info_node.context;
    if (context->calculated_stats == false)
    {
        if (LoadEquipmentLimits(&context->limits, info_node.item_slot) == false) {
            return false;
        };
        context->calculated_stats = true;
    };
    //----------------------------------------------------------------------------------------------
    ns_formula::ItemAttrContextList* parent = info_node.context->parent;
    if (parent->calculated_total_stats == false) {
        LoadPlayerLimits(&parent->total_stats, attributes_, player_level_);
        parent->calculated_total_stats = true;
    }
    //parent->total_records // already updated
    if (CanEquipmentStatComparison(parent->total_stats, context->limits) == false) {
        return false;
    }
    return true;
}

bool ItemAttrCalculator::UpdatePlayerSimulationStatsByItemStats(const EquipSlotNode& info_node)
{
    WAVERIX_WRAPPER((WAVERIX_LC_CALL1("waverix_log_trace", __FUNCTION__)));
    // NOTE: apply simulation stat
    ns_formula::ItemAttrStats* const total_stats = &info_node.context->parent->total_stats;
    for (AttrValue* it = total_stats->stats;
         it != &total_stats->stats[_countof(total_stats->stats)];
         ++it)
    {
        it->Update();
    }
    WAVERIX_WRAPPER((WAVERIX_LC_CALL4("waverix_log_trace2",
        "|Player current stats|{Str,Dex,Vit}=",
        total_stats->stats[total_stats->eSTAT_STR].calc_value_.value_,
        total_stats->stats[total_stats->eSTAT_DEX].calc_value_.value_,
        total_stats->stats[total_stats->eSTAT_VIT].calc_value_.value_)));
    WAVERIX_WRAPPER((WAVERIX_LC_CALL4("waverix_log_trace2",
        "|Player current stats|{Int,Spr,DecAll}=",
        total_stats->stats[total_stats->eSTAT_INT].calc_value_.value_,
        total_stats->stats[total_stats->eSTAT_SPR].calc_value_.value_,
        total_stats->stats[total_stats->eSTAT_DEC_LIMITS].calc_value_.value_)));
    return true;
};

// NOTE: f110415.7L, add check interfaces to support switchable test
bool ItemAttrCalculator::CheckExistLimits(const ns_formula::ItemLimitations& item_limits)
{
    if (item_limits.level) {
        return true;
    };

    for (const short* limit_it = item_limits.limits;
         limit_it != &item_limits.limits[_countof(item_limits.limits)];
         ++limit_it)
    {
        // CHANGES: f110521.3L, fixed checking of value existence
        if (*limit_it) {
            return true;
        };
    };
    return false;
}

//==================================================================================================
//==================================================================================================
//==================================================================================================

bool ItemAttrCalculator::Equip2(
    const SCItemSlot& IN slot,
    bool is_pc_room, bool use_update_ex, LEVELTYPE player_level)
{
    WAVERIX_WRAPPER((WAVERIX_LC_CALL1("waverix_log_trace", __FUNCTION__)));
    __TOUCHED((&slot));
    //----------------------------------------------------------------------------------------------
    // NOTE: f110420.3L, changes logic to support nested update when the result of unit calculation
    // needs nested updates.
    ;{
        pc_room_status_ = is_pc_room;
        use_update_ex_ = use_update_ex;
        player_level_ = player_level;
    };
    ;{
        slot_cache_info_->ClearEquipCounterInfo();
    };
    bool result = CalculateTotalConditionalEquipments(eInterControlHint_EquipOne, slot.GetPos());
    //----------------------------------------------------------------------------------------------
    //
    if (use_update_ex) {
        attributes_.UpdateEx();
    }
    else {
        attributes_.Update();
    };
    //----------------------------------------------------------------------------------------------
    return result;
}

bool ItemAttrCalculator::Unequip2(
    const SCItemSlot* slot, POSTYPE slot_pos,
    bool is_pc_room, bool use_update_ex, LEVELTYPE player_level)
{
    WAVERIX_WRAPPER((WAVERIX_LC_CALL1("waverix_log_trace", __FUNCTION__)));
    __TOUCHED((slot));
    //
    ;{
        pc_room_status_ = is_pc_room;
        use_update_ex_ = use_update_ex;
        player_level_ = player_level;
    };
    ;{
        // clear process
        Clear(); // total stacked calculation
        //
        slot_cache_info_->ClearAll();
    };
    bool result = CalculateTotalConditionalEquipments(eInterControlHint_Unequip, slot_pos);
    //----------------------------------------------------------------------------------------------
    //
    if (use_update_ex) {
        attributes_.UpdateEx();
    }
    else {
        attributes_.Update();
    };
    //
    return result;
}

//--------------------------------------------------------------------------------------------------

bool ItemAttrCalculator::EquipAll(bool is_pc_room, bool use_update_ex, LEVELTYPE player_level)
{
    WAVERIX_WRAPPER((WAVERIX_LC_CALL1("waverix_log_trace", __FUNCTION__)));
    //
    ;{
        pc_room_status_ = is_pc_room;
        use_update_ex_ = use_update_ex;
        player_level_ = player_level;
    };
    ;{
        // clear process
        Clear(); // total stacked calculation
        //
        slot_cache_info_->ClearAll();
    };

    bool result = CalculateTotalConditionalEquipments(eInterControlHint_TotalEquips, 0);
    //----------------------------------------------------------------------------------------------
    //
    if (use_update_ex) {
        attributes_.UpdateEx();
    }
    else {
        attributes_.Update();
    };
    //----------------------------------------------------------------------------------------------
    return result;
}

// changed item durability, change time expiration, etc...
bool ItemAttrCalculator::ChangeItemDurabilityStatus(
    const SCItemSlot& IN slot, POSTYPE slot_pos,
    bool is_pc_room, bool use_update_ex, LEVELTYPE player_level)
{
    WAVERIX_WRAPPER((WAVERIX_LC_CALL1("waverix_log_trace", __FUNCTION__)));
    //
    ;{
        pc_room_status_ = is_pc_room;
        use_update_ex_ = use_update_ex;
        player_level_ = player_level;
    };

    const bool duration_type = (slot.WithDura() != false);
    if (FlowControl::FCAssert(duration_type) == false) {
        return false;
    }
    const ns_formula::EquipSlotsManageInfo::Node& last_updated_info = \
        slot_cache_info_->slots[slot_pos];
    const DURATYPE duration = slot.GetDura();

    enum {
        eProcess_NoChanges = 0,
        eProcess_Activation,
        eProcess_EquipAll,
        eProcess_ChangeOption,
    };
    int what_to_do = eProcess_EquipAll;
    ;{
        if (last_updated_info.duration == duration)
        {
            if (last_updated_info.duration == 0) {
                assert(last_updated_info.activated == false);
            };
            what_to_do = eProcess_NoChanges;
        }
        else if (last_updated_info.duration)
        {
            // change duration
            if (duration == 0) {
                // disable (total calculation)
                what_to_do = eProcess_EquipAll;
            }
            else {
            #ifdef __NA_001062_20080623_STRENGTH_PENALTY
                // change attack power or defense power ...
                what_to_do = eProcess_ChangeOption;
            #else
                what_to_do = eProcess_NoChanges;
            #endif
            }
        }
        else if (duration)
        {
            if (last_updated_info.duration == 0) {
                what_to_do = eProcess_Activation;
            }
            else {
            #ifdef __NA_001062_20080623_STRENGTH_PENALTY
                // change attack power or defense power ...
                what_to_do = eProcess_ChangeOption;
            #else
                what_to_do = eProcess_NoChanges;
            #endif
            }
        }
    };

    if (what_to_do == eProcess_NoChanges) {
        // update latest info.
        if (last_updated_info.duration != duration) {
            const_cast<ns_formula::EquipSlotsManageInfo::Node*>(\
                &last_updated_info)->duration = duration;
        }
        return false;
    };
    //
    switch (what_to_do)
    {
    case eProcess_Activation:
        ;{
            ;{
                pc_room_status_ = is_pc_room;
                use_update_ex_ = use_update_ex;
                player_level_ = player_level;
            };
            bool result = CalculateTotalConditionalEquipments(
                eInterControlHint_ChangedToActivates, slot_pos);
            //----------------------------------------------------------------------------------------------
            //
            if (use_update_ex) {
                attributes_.UpdateEx();
            }
            else {
                attributes_.Update();
            };
            return result;
        };
    case eProcess_EquipAll:
        return this->EquipAll(is_pc_room, use_update_ex, player_level);
    };
#if !defined(__NA_001062_20080623_STRENGTH_PENALTY)
    // update latest info.
    if (last_updated_info.duration != duration) {
        const_cast<ns_formula::EquipSlotsManageInfo::Node*>(\
            &last_updated_info)->duration = duration;
    }
    return false;
#else //if defined(__NA_001062_20080623_STRENGTH_PENALTY)
    //
    if (what_to_do != eProcess_ChangeOption)
    {
        assert(what_to_do == eProcess_ChangeOption);
        return false;
    }
    //----------------------------------------------------------------------------------------------
    const DURATYPE max_duration = slot.GetMaxDura();
    if (const BASE_ITEMINFO* item_info = slot.GetItemInfo())
    {
        const DWORD penalty_index = item_info->m_dwStrengthPenaltyIdx;
        if (penalty_index == 0)
        {
            // update latest info.
            if (last_updated_info.duration != duration) {
                const_cast<ns_formula::EquipSlotsManageInfo::Node*>(\
                    &last_updated_info)->duration = duration;
            }
            return false;
        };
        //
        StrengthPenaltyParser* const penalty_parser = StrengthPenaltyParser::Instance();
        const sSTRENGTH_PENALTY* strength_penalty_prev = \
            penalty_parser->GetStrengthPenaltyInfo(\
                penalty_index, max_duration, last_updated_info.duration);
        const sSTRENGTH_PENALTY* strength_penalty_curr = \
            penalty_parser->GetStrengthPenaltyInfo(\
                penalty_index, max_duration, duration);
        if (strength_penalty_prev == strength_penalty_curr) {
            return false;
        };
    };

    //----------------------------------------------------------------------------------------------
    // activated -> activated
    assert(duration != 0 && last_updated_info.duration != 0);
    //
    ns_formula::EquipSlotNodeBuffer info_node_buffer;
    EquipSlotNode* const info_node = info_node_buffer.GetInstance();
    //
    EquipSlotNode::eDetailedResult init_result;
    if (EquipSlotNode::Initialize(
            &init_result,
            info_node,
            &slot) == false)
    {
        return false;
    };
    //
    ;{  // configure operator
        info_node->current_operate = Attributes::Operation::eOper_Sub;
        info_node->position = slot_pos;
        info_node->duration = last_updated_info.duration; // subtract previous options
        info_node->max_duration = max_duration;
    };
    // skip.
    //slot_cache_info_->IncreaseEquipInfo(slot);
    ns_formula::EquipSlotsManageInfo::Node* const work_node = \
        &slot_cache_info_->slots[info_node->position];
    //
    ns_formula::ItemAttrContextList contexts;
    //
    ns_formula::ItemLimitations* prev_limits = NULL;
    for (int loop_counter = 0; loop_counter < 2; ++loop_counter)
    {
        if (loop_counter == 0)
        {   // 1st
            UpdateEquipSlotsLatestInfo(false, *info_node, work_node);
            info_node->context = contexts.BeginContext(info_node->position);
        }
        else
        {   // 2nd
            info_node->current_operate = Attributes::Operation::eOper_Add;
            info_node->duration = duration; // 'duration' is changed value
            UpdateEquipSlotsLatestInfo(false, *info_node, work_node);
            info_node->context = contexts.BeginContext(info_node->position);
            info_node->context->calculated_stats = true;
            info_node->context->limits = *prev_limits;
        };
        //
        if (CanEquipmentValidStatsInternal(*info_node) == false)
        {
            contexts.RollbackContext();
        }
        else if (init_result == info_node->eResult_NoOptions)
        {
            work_node->activated = true;
            //-------------------------------------------
            contexts.RollbackContext();
        }
        else
        {
            // (init_result == info_node->eResult_OptionsOnly) ||
            // (init_result == info_node->eResult_AllProcess)
            work_node->activated = true;
            //-------------------------------------------
            CalculateEquipments(*info_node);
            //-------------------------------------------
            contexts.CommitContext();
        }
        //------------------------------------------------------------------------------------------
        UpdateEquipSlotsLatestInfo(work_node->activated, *info_node, work_node);
        //------------------------------------------------------------------------------------------
        if (loop_counter == 0) {
            assert(info_node->context->calculated_stats);
            prev_limits = &info_node->context->limits;
        }
    };
    if (contexts.number_of_total_records) {
        attributes_.GroupBatchOperate(contexts.total_records, contexts.number_of_total_records);
    };

    //----------------------------------------------------------------------------------------------
    //
    if (use_update_ex) {
        attributes_.UpdateEx();
    }
    else {
        attributes_.Update();
    };
    //----------------------------------------------------------------------------------------------
    //
    return true;
#endif //__NA_001062_20080623_STRENGTH_PENALTY
}

//==================================================================================================
// NOTE: f110415.7L, test whether new inserting item enabled equips instead of origin equipments
bool ItemAttrCalculator::TestSwitchableEquipments(
    const SCItemSlot* equip_test_item, const POSTYPE to_slot_pos,
    bool is_pc_room, bool use_update_ex, LEVELTYPE player_level)
{
    WAVERIX_WRAPPER((WAVERIX_LC_CALL1("waverix_log_trace", __FUNCTION__)));
    //
    if (bool out_of_ranges = (to_slot_pos >= eEQUIPCONTAINER_MAX)) {
        return false;
    };
    //
    if (equip_test_item->CanEquip() == false) {
        return false;
    };
    //
    ns_formula::EquipSlotNodeBuffer info_node_buffer_insert_test;
    EquipSlotNode* const info_node_insert_test = info_node_buffer_insert_test.GetInstance();
    //
    EquipSlotNode::eDetailedResult init_result_insert_test;
    if (EquipSlotNode::Initialize(
            &init_result_insert_test,
            info_node_insert_test,
            equip_test_item) == false)
    {
        return false;
    };

    ItemLimitations inven_item_limits;
    if (LoadEquipmentLimits(&inven_item_limits, *equip_test_item) == false) {
        return false;
    };
    if (CheckExistLimits(inven_item_limits) == false) {
        // no-check ok.
        return true;
    };
    //----------------------------------------------------------------------------------------------
    // ns_formula::eInterControl_LimitsTestOnly support default attributes calculation skipping
    // in CalculateOptionNode wrapping with REGISTER_ATTR_NODE
    ns_formula::ItemAttrCalculatorInterControlAutoVariable auto_trans(
        &inter_control_mode_, ns_formula::eInterControl_LimitsTestOnly);
    // NOTE: f110415.7L, this algorithm use pre-calculating limitations.
    // AutoDependencyRevert is used to rollback temporary calculation result
    // that is used to support equips simulation without un-equips operation.
    struct AutoDependencyRevert {
        AutoDependencyRevert(ns_formula::EquipSlotsManageInfo* slot_cache_info)
            : slot_cache_info_(\
                static_cast<ns_formula::EquipSlotsManageInfoInterController*>(slot_cache_info))
            , modified_slot(NULL)
        {}
        ~AutoDependencyRevert() {
            if (modified_slot != NULL) {
                slot_cache_info_->IncreaseEquipInfoSimulation(*modified_slot);
            };
        }
        ns_formula::EquipSlotsManageInfoInterController* slot_cache_info_;
        const SCItemSlot* modified_slot;
    } auto_revert(slot_cache_info_);
    //----------------------------------------------------------------------------------------------
    ;{
        pc_room_status_ = is_pc_room;
        use_update_ex_ = use_update_ex;
        player_level_ = player_level;
    };
    //
    ns_formula::EquipSlotNodeBuffer info_node_buffer[MAX_EQUIPMENT_SLOT_NUM];
    ns_formula::EquipWorkingSlots equip_slots = { 0, };

    int number_of_item_contains = 0;
    ;{
        ns_formula::EquipWorkingSlots::Node* dest_it = equip_slots.slots;
        ns_formula::EquipSlotsManageInfo::Node* work_node_it = slot_cache_info_->slots;

        typedef SCItemSlotContainer::SLOT_LINEAR_LIST SLOT_LINEAR_LIST;
        const SLOT_LINEAR_LIST& slot_list = equip_container_.GetSlotList();
        SLOT_LINEAR_LIST::const_iterator it = slot_list.begin(),
                                         end = slot_list.end();
        for (POSTYPE pos = 0; it != end; ++pos, ++dest_it, ++work_node_it, ++it)
        {
            const SCItemSlot* item_slot = static_cast<SCItemSlot*>(it->slot);
            const SLOTCODE item_code = item_slot->GetItemCode();
            if (item_code == 0) {
                continue;
            };
            //
            // NOTE: f110415.7L, this operation has only test. this calling can only be called
            // under already calculated attributes. so that this don't check external conditions.
            // no check = { slot_cache_info_->CheckEquipDefaultValidationWithoutStats }
            // no update = { UpdateEquipSlotsLatestInfo }
            const bool is_target_slot = (to_slot_pos == pos);
            if (is_target_slot)
            {
                // the reason of the activated check faster calling than decrease dependencies have
                // dependencies even though a checking item is inactivated.
                auto_revert.slot_cache_info_->DecreaseEquipInfoSimulation(*item_slot);
                auto_revert.modified_slot = item_slot;
                // the validation of this condition passed because new insert item test only
                if (equipment_accessor_->CheckEquipDefaultValidationWithoutStats(
                        pos, *equip_test_item) == false)
                {
                    return false;
                }
                continue;
            };
            if (work_node_it->activated == false) {
                continue;
            };
            //
            EquipSlotNode* info_node = info_node_buffer[pos].GetInstance();
            EquipSlotNode::eDetailedResult init_result;
            //
            if (EquipSlotNode::Initialize(
                    &init_result,
                    info_node,
                    item_slot) == false)
            {
                continue;
            }
            ;{  // configure operator
                info_node->current_operate = Attributes::Operation::eOper_Add;
            };
            ;{  // setup info_node
                info_node->position = pos;
                if (item_slot->WithDura())
                {
                    info_node->duration = item_slot->GetDura();
                    info_node->max_duration = item_slot->GetMaxDura();
                };
                // NOTE: f110415.7L, this calling has only stat limitation check.
                // no update to external cache information.
                //UpdateEquipSlotsLatestInfo(false, *info_node, work_node_it);
            };
            //
            dest_it->item_slot = item_slot;
            dest_it->info_node = info_node;
            dest_it->work_node = work_node_it;
            ++number_of_item_contains;
        }
    };
    //
    // all equipments 'can equip' tests and apply options
    // except remove reservation item by 'to_slot_pos'
    ns_formula::ItemAttrContextList contexts;
    //
    int number_of_total_activates = 0;
    while (bool loop_until_has_not_calculate_anymore =
            (number_of_item_contains > number_of_total_activates))
    {
        int number_of_activates = 0;
        ns_formula::EquipWorkingSlots::Node* equip_it = equip_slots.slots,
            * const equip_end = &equip_slots.slots[_countof(equip_slots.slots)];
        for (; equip_it != equip_end; ++equip_it)
        {
            EquipSlotNode* const info_node = equip_it->info_node;
            if (equip_it->equipped || info_node == NULL) {
                continue;
            }

            bool activated = false;
            //
            info_node->context = contexts.BeginContext(info_node->position);
            if (CanEquipmentValidStatsInternal(*info_node) == false)
            {
                contexts.RollbackContext();
            }
            else if (info_node->init_result == info_node->eResult_NoOptions)
            {
                activated = true;
                //-------------------------------------------
                contexts.RollbackContext();
            }
            else if (info_node->init_result == info_node->eResult_OptionsOnly)
            {
                activated = true;
                //-------------------------------------------
                CalculateOptionList(*info_node);
                //-------------------------------------------
                UpdatePlayerSimulationStatsByItemStats(*info_node);
                //
                contexts.CommitContext();
            }
            else
            {
                assert(info_node->init_result == info_node->eResult_AllProcess);
                activated = true;
                // equip process
                //-------------------------------------------
                //-------------------------------------------
                // NOTE: f110415.7L, this calling has only stat limitation check.
                //CalculateEquipments(*info_node);
                //-------------------------------------------
                CalculateOptionList(*info_node);
                //-------------------------------------------
                if (info_node->item_info->set_group_code) {
                    CalculateSetItemOptions(*info_node, &equip_slots);
                };
                //-------------------------------------------
                // NOTE: f110415.7L, this calling has only stat limitation check.
                // the socket options has not limitations.
                //CalculateSocketOptions(*info_node);
                //-------------------------------------------
                UpdatePlayerSimulationStatsByItemStats(*info_node);
                //
                contexts.CommitContext();
            };
            //--------------------------------------------------------------------------------------
            //UpdateEquipSlotsLatestInfo(work_node->activated, *info_node, work_node);
            //--------------------------------------------------------------------------------------
            if (activated) {
                equip_it->equipped = true;
                ++number_of_activates;
            }
            // NOTE: f110415.7L, this calling has only stat limitation check.
            // don't call attributes_.GroupBatchOperate. it is real calculator.
            //
            // NOTE: 'RecycleContexts' method is used in all equip process
            //contexts.RecycleContexts();
            //--------------------------------------------------------------------------------------
        } //end 'foreach item_slot in equip_container'
        //
        number_of_total_activates += number_of_activates;
        if (number_of_activates == 0) {
            break;
        };
    };
    //----------------------------------------------------------------------------------------------
    // current state: at this point, all items that related to limitations calculated 
    // except the slot of target position that will be inserted.
    // final test: whether the insertion test item enables really insertable considering limitations
    ;{
        ns_formula::ItemAttrContext* context = contexts.BeginContext(to_slot_pos);
        context->calculated_stats = true;
        context->limits = inven_item_limits; // use pre-calculated info.
        //
        info_node_insert_test->context = context;
        if (CanEquipmentValidStatsInternal(*info_node_insert_test) == false) {
            return false;
        };
        // WARNING: don't use a context closure because contexts don't be managed any longer.
        // but, if the contexts that is collection of context has operations since at this point, 
        // use it like 'contexts.CommitContext' or 'contexts.RollbackContext'.
    };
    //----------------------------------------------------------------------------------------------
    return true;
}

//==================================================================================================
//==================================================================================================

// arguments: 'hint' = { eItemAttrCalculatorInterControlHint };
bool ItemAttrCalculator::CalculateTotalConditionalEquipments(const int hint, const int hint_arg)
{
    __TOUCHED((hint_arg));
    WAVERIX_WRAPPER((WAVERIX_LC_CALL1("waverix_log_trace", __FUNCTION__)));
    //
    ns_formula::EquipSlotNodeBuffer info_node_buffer[MAX_EQUIPMENT_SLOT_NUM];
    ns_formula::EquipWorkingSlots equip_slots = { 0, };

    typedef SCItemSlotContainer::SLOT_LINEAR_LIST SLOT_LINEAR_LIST;
    int number_of_item_contains = 0;
    ;{
        ns_formula::EquipWorkingSlots::Node* dest_it = equip_slots.slots;
        ns_formula::EquipSlotsManageInfo::Node* work_node_it = slot_cache_info_->slots;

        const SLOT_LINEAR_LIST& slot_list = equip_container_.GetSlotList();
        SLOT_LINEAR_LIST::const_iterator it = slot_list.begin(),
                                         end = slot_list.end();
        for (POSTYPE pos = 0; it != end; ++pos, ++dest_it, ++work_node_it, ++it)
        {
            const SCItemSlot* item_slot = static_cast<SCItemSlot*>(it->slot);
            const SLOTCODE item_code = item_slot->GetItemCode();
            if (item_code == 0) {
                continue;
            };
            //
            ;;;; if ((hint == eInterControlHint_EquipOne))
            {
                // NOTE: restrict_table_ of the slot_cache_info_ has cleared a reference counter.
                // ref: 'ClearEquipCounterInfo()'
                if (equipment_accessor_->CheckEquipDefaultValidationWithoutStats(
                        pos, *item_slot) == false)
                {
                    slot_cache_info_->IncreaseEquipInfo(*item_slot);
                    continue;
                }
                slot_cache_info_->IncreaseEquipInfo(*item_slot);
                if (work_node_it->activated) {
                    continue;
                };
            }
            else if ((hint == eInterControlHint_TotalEquips) ||
                     (hint == eInterControlHint_Unequip))
            {
                if (equipment_accessor_->CheckEquipDefaultValidationWithoutStats(
                        pos, *item_slot) == false)
                {
                    // NOTE: this process means already exist in container
                    slot_cache_info_->IncreaseEquipInfo(*item_slot);
                    continue;
                }
                slot_cache_info_->IncreaseEquipInfo(*item_slot);
            }
            else if ((hint == eInterControlHint_ChangedToActivates))
            {
                // NOTE: when change durability, this case is selected.
                // changes inactivated slots to activated slots.
                // the case of changed durability event has no-list control
                // because of recalculation in single transaction.
                if (work_node_it->activated) {
                    continue;
                };
                if (equipment_accessor_->CheckEquipDefaultValidationWithoutStats(
                        pos, *item_slot) == false)
                {
                    continue;
                }
            };
            //
            EquipSlotNode* info_node = info_node_buffer[pos].GetInstance();
            EquipSlotNode::eDetailedResult init_result;
            //
            if (EquipSlotNode::Initialize(
                    &init_result,
                    info_node,
                    item_slot) == false)
            {
                continue;
            }
            ;{  // configure operator
                info_node->current_operate = Attributes::Operation::eOper_Add;
            };
            ;{  // setup info_node
                info_node->position = pos;
                if (item_slot->WithDura())
                {
                    info_node->duration = item_slot->GetDura();
                    info_node->max_duration = item_slot->GetMaxDura();
                };
                //
                UpdateEquipSlotsLatestInfo(false, *info_node, work_node_it);
            };
            //
            dest_it->item_slot = item_slot;
            dest_it->info_node = info_node;
            dest_it->work_node = work_node_it;
            ++number_of_item_contains;
        }
    };
    //
    // all equipments 'can equip' tests and apply options
    ns_formula::ItemAttrContextList contexts;
    //
    int number_of_total_activates = 0;
    float set_option_value = 0.0f;

	while (bool loop_until_has_not_calculate_anymore =
            (number_of_item_contains > number_of_total_activates))
    {
        int number_of_activates = 0;
        ns_formula::EquipWorkingSlots::Node* equip_it = equip_slots.slots,
            * const equip_end = &equip_slots.slots[_countof(equip_slots.slots)];

		
        for (; equip_it != equip_end; ++equip_it)
        {
            EquipSlotNode* const info_node = equip_it->info_node;
            if (equip_it->equipped || info_node == NULL) {
                continue;
            }

			
            ns_formula::EquipSlotsManageInfo::Node* const work_node = equip_it->work_node;
            //
            info_node->context = contexts.BeginContext(info_node->position);
            if (CanEquipmentValidStatsInternal(*info_node) == false)
            {
                contexts.RollbackContext();
            }
            else if (info_node->init_result == info_node->eResult_NoOptions)
            {
                work_node->activated = true;
                //-------------------------------------------
                contexts.RollbackContext();
            }
            else if (info_node->init_result == info_node->eResult_OptionsOnly)
            {
                work_node->activated = true;
                //-------------------------------------------
                CalculateOptionList(*info_node);
                //-------------------------------------------
                UpdatePlayerSimulationStatsByItemStats(*info_node);
                //
                contexts.CommitContext();
            }
            else
            {
                assert(info_node->init_result == info_node->eResult_AllProcess);
                work_node->activated = true;
                // equip process
                //-------------------------------------------
                //-------------------------------------------
                CalculateEquipments(*info_node);
                //-------------------------------------------
                CalculateOptionList(*info_node);
                //-------------------------------------------
#ifdef _NA_006731_20130521_ENCHANT_ADD_OPTION
                CalculateEnchantOption(*info_node);
#endif // _NA_006731_20130521_ENCHANT_ADD_OPTION
                if (info_node->item_info->set_group_code ) {
                    CalculateSetItemOptions(*info_node, &equip_slots);
                };
                //-------------------------------------------
                // socket
                CalculateSocketOptions(*info_node);
                //-------------------------------------------
                UpdatePlayerSimulationStatsByItemStats(*info_node);
                //
                contexts.CommitContext();
            };
            //--------------------------------------------------------------------------------------
            UpdateEquipSlotsLatestInfo(work_node->activated, *info_node, work_node);
            //--------------------------------------------------------------------------------------
            if (work_node->activated) {
                equip_it->equipped = true;
                ++number_of_activates;
#ifdef _NA_002935_20110704_ITEM_SCORE
                // 개별 장비 평가 점수를 계산한다.
                ns_formula::EquipSlotsManageInfo::ItemScoreInfo& item_score_info = 
                    slot_cache_info_->item_score_info;
                if (work_node->position < _countof(item_score_info.each_item_score))
                {
                    item_score_info.each_item_score[work_node->position] = 
                        ItemScoreParser::Instance()->CalculateSingleScore(
                            *(equip_it->item_slot), pc_room_status_);
                }
#endif
            }
            //--------------------------------------------------------------------------------------
        } //end 'foreach item_slot in equip_container'
        //
#ifdef _NA_002935_20110704_ITEM_SCORE
        if (number_of_activates > 0)
        {
            // 소켓과 세트 옵션 관련 장비 평가 점수를 계산한다.
            ns_formula::EquipSlotsManageInfo::ItemScoreInfo& item_score_info = 
                slot_cache_info_->item_score_info;
            ItemScoreParser::Instance()->CalculateTotalEquipScore(
                attributes_, item_score_info.socket_score, set_option_value);
        }
#endif
        //
        number_of_total_activates += number_of_activates;
        if (number_of_activates == 0) {
            break;
        };
    };

	//최종 장비 평점 계산은 밑에서 한다
	slot_cache_info_->item_score_info.set_option_score = set_option_value;
	slot_cache_info_->item_score_info.UpdateTotalScore();
    //----------------------------------------------------------------------------------------------
    if (contexts.number_of_total_records)
    {
        attributes_.GroupBatchOperate(contexts.total_records, contexts.number_of_total_records);
    #if USE_CLIENT_SOCKET_OPTION_TABLE
        // NOTE: f110530.3L

		
        this->slot_cache_info_->CalculateSocketOptionMappingTable();
    #endif
    };
    //----------------------------------------------------------------------------------------------
    //
    return true;
}

//==================================================================================================
//==================================================================================================

#define REGISTER_ATTR_NODE(_attr_type_, _attr_kind_, _attr_value_) \
    CalculateOptionNode(info_node, _attr_type_, _attr_kind_, _attr_value_)

bool ItemAttrCalculator::CalculateOptionNode(const EquipSlotNode& info_node,
    eATTR_TYPE attr_type, eATTR_KIND attr_kind, int attr_value)
{
    if (attr_value == 0) {
        return false;
    }
    if (bool out_range = !(eATTR_TYPE_INVALID < attr_type && attr_type < eATTR_MAX)) {
        return false;
    }

    ns_formula::ItemAttrContext& records = *info_node.context;
    // CHANGES: f110630.2L, fixed specific stat increasing option calculation problem
    //----------------------------------------------------------------------------------------------
    if (const bool default_attr = (attr_type != eATTR_INCREASE_STAT_POINT) &&
                                  (attr_type != eATTR_DECREASE_LIMIT_STAT) &&
                                  (attr_type >  eATTR_SPR))
    {
        // NOTE: f110415.7L, if this operation is a test transaction
        // whether new inserting item enabled equips instead of origin equipments, this means
        // the below process that related to default attributes calcuation no longer have process.
        if (inter_control_mode_ != ns_formula::eInterControl_LimitsTestOnly)
        {
            if (Attributes::OperationRecord* record = records.BeginRecord())
            {
                // assert(attr_value != 0);
                record->operate = info_node.current_operate;
                record->attr_kind = static_cast<uint8_t>(attr_kind);
                record->attr_type = static_cast<uint16_t>(attr_type);
                record->attr_value = attr_value;
                records.CommitRecord();
            };
        };
        return true;
    }
    //----------------------------------------------------------------------------------------------
    else if (FlowControl::FCAssert(attr_kind == eATTR_KIND_ITEM) == false)
    {
        return false;
    }
    //----------------------------------------------------------------------------------------------
    else if (attr_type == eATTR_DECREASE_LIMIT_STAT)
    {
        if (inter_control_mode_ != ns_formula::eInterControl_LimitsTestOnly)
        {
            if (Attributes::OperationRecord* record = records.BeginRecord())
            {
                // assert(attr_value != 0);
                record->operate = info_node.current_operate;
                record->attr_kind = static_cast<uint8_t>(attr_kind);
                record->attr_type = static_cast<uint16_t>(eATTR_DECREASE_LIMIT_STAT);
                record->attr_value = attr_value;
                records.CommitRecord();
            };
        };
        if (ns_formula::ItemAttrStats* const player_stats = &records.parent->total_stats)
        {
            bool add_op = (info_node.current_operate == Attributes::Operation::eOper_Add);
            if (AttrValue* dest = &player_stats->stats[player_stats->eSTAT_DEC_LIMITS]) {
                dest->item_value_.value_ += (add_op ? attr_value : -attr_value);
            };
        };
        return true;
    }
    //----------------------------------------------------------------------------------------------
    else if ((attr_type == eATTR_STR) ||
             (attr_type == eATTR_DEX) ||
             (attr_type == eATTR_VIT) ||
             (attr_type == eATTR_INT) ||
             (attr_type == eATTR_SPR))
    {
        if (inter_control_mode_ != ns_formula::eInterControl_LimitsTestOnly)
        {
            if (Attributes::OperationRecord* record = records.BeginRecord())
            {
                // assert(attr_value != 0);
                record->operate = info_node.current_operate;
                record->attr_kind = static_cast<uint8_t>(attr_kind);
                record->attr_type = static_cast<uint16_t>(attr_type);
                record->attr_value = attr_value;
                records.CommitRecord();
            };
        };
        if (ns_formula::ItemAttrStats* const player_stats = &records.parent->total_stats)
        {
            BOOST_STATIC_ASSERT((player_stats->eSTAT_STR == eATTR_STR - 1) &&
                                (player_stats->eSTAT_DEX == eATTR_DEX - 1) &&
                                (player_stats->eSTAT_VIT == eATTR_VIT - 1) &&
                                (player_stats->eSTAT_INT == eATTR_INT - 1) &&
                                (player_stats->eSTAT_SPR == eATTR_SPR - 1));

            bool add_op = (info_node.current_operate == Attributes::Operation::eOper_Add);
            if (AttrValue* dest = &player_stats->stats[attr_type - 1]) {
                dest->item_value_.value_ += (add_op ? attr_value : -attr_value);
            };
        };
    }
    //----------------------------------------------------------------------------------------------
    else if (attr_type == eATTR_INCREASE_STAT_POINT)
    {
        if (inter_control_mode_ != ns_formula::eInterControl_LimitsTestOnly)
        {
            if (Attributes::OperationRecord* record = records.BeginRecord())
            {
                // assert(attr_value != 0);
                record->operate = info_node.current_operate;
                record->attr_kind = static_cast<uint8_t>(attr_kind);
                record->attr_type = static_cast<uint16_t>(eATTR_STR);
                record->attr_value = attr_value;
                records.CommitRecord();
            };
            if (Attributes::OperationRecord* record = records.BeginRecord())
            {
                // assert(attr_value != 0);
                record->operate = info_node.current_operate;
                record->attr_kind = static_cast<uint8_t>(attr_kind);
                record->attr_type = static_cast<uint16_t>(eATTR_DEX);
                record->attr_value = attr_value;
                records.CommitRecord();
            };
            if (Attributes::OperationRecord* record = records.BeginRecord())
            {
                // assert(attr_value != 0);
                record->operate = info_node.current_operate;
                record->attr_kind = static_cast<uint8_t>(attr_kind);
                record->attr_type = static_cast<uint16_t>(eATTR_VIT);
                record->attr_value = attr_value;
                records.CommitRecord();
            };
            if (Attributes::OperationRecord* record = records.BeginRecord())
            {
                // assert(attr_value != 0);
                record->operate = info_node.current_operate;
                record->attr_kind = static_cast<uint8_t>(attr_kind);
                record->attr_type = static_cast<uint16_t>(eATTR_INT);
                record->attr_value = attr_value;
                records.CommitRecord();
            };
            if (Attributes::OperationRecord* record = records.BeginRecord())
            {
                // assert(attr_value != 0);
                record->operate = info_node.current_operate;
                record->attr_kind = static_cast<uint8_t>(attr_kind);
                record->attr_type = static_cast<uint16_t>(eATTR_SPR);
                record->attr_value = attr_value;
                records.CommitRecord();
            };
        };
        if (ns_formula::ItemAttrStats* const player_stats = &records.parent->total_stats)
        {
            bool add_op = (info_node.current_operate == Attributes::Operation::eOper_Add);
            AttrValue* it = player_stats->stats; //without eSTAT_SIMUL_COUNTS
            for (; it != &player_stats->stats[_countof(player_stats->stats) - 1]; ++it) {
                it->item_value_.value_ += (add_op ? attr_value : -attr_value);
            };
        };
    }
    else {
        ASSERT(!"unexpected status");
    };
    //----------------------------------------------------------------------------------------------
    return true;
}


bool ItemAttrCalculator::CalculateEquipments(const EquipSlotNode& info_node)
{
    WAVERIX_WRAPPER((WAVERIX_LC_CALL1("waverix_log_trace", __FUNCTION__)));
    //
    // DONE: check time expiration, duration is zero, ... 'f110331.5L'
    const BASE_ITEMINFO* const item_info = info_node.item_info;
    const nsSlot::ItemRuleInfo& item_rule_info = info_node.item_rule_info;

    //----------------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------------

    REGISTER_ATTR_NODE(eATTR_PHYSICAL_ATTACK_SUCCESS_RATIO, eATTR_KIND_ITEM,
                       item_info->m_wPhyAttRate);
    REGISTER_ATTR_NODE(eATTR_PHYSICAL_ATTACK_BLOCK_BASE_ARMOR_RATIO, eATTR_KIND_ITEM,
                       item_info->m_wPhyAvoid);
    REGISTER_ATTR_NODE(eATTR_ATTACK_SPEED, eATTR_KIND_ITEM,
                       item_info->m_wPhyAttSpeed);
    REGISTER_ATTR_NODE(eATTR_MOVE_SPEED, eATTR_KIND_ITEM,
                       item_info->m_wSpeed);

    //----------------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------------
    ;;;; if (item_rule_info.is_weapon) {
        CalculateWeapon(info_node);
    }
    else if (item_rule_info.is_armor) {
        CalculateArmor(info_node);
    }
#ifdef _NA002823_WINGS_EQUIPMENTS_EXTENTION
    else if (item_rule_info.is_accessory_wings) {
        CalculateArmor(info_node);
    }
#endif
    else if (item_rule_info.is_accessory || item_rule_info.is_accessory_special) {
        CalculateAccessory(info_node);
    }

    return true;
}

//==================================================================================================

bool ItemAttrCalculator::CalculateWeapon(const EquipSlotNode& info_node)
{
    WAVERIX_WRAPPER((WAVERIX_LC_CALL1("waverix_log_trace", __FUNCTION__)));
    //
    ns_formula::InterAttackPowerCalculator calc_slot = { 0, };
    const BASE_ITEMINFO* item_info = info_node.item_info;
    //
    if (info_node.item_slot.IsDivine()) {
        calc_slot.SetDefault(item_info->m_wDivPhyMinDamage, item_info->m_wDivPhyMaxDamage);
    }
    else if (info_node.item_info->IsElite()) {
        calc_slot.SetDefault(item_info->m_wElitePhyMinDamage, item_info->m_wElitePhyMaxDamage);
    }
    else if (info_node.item_info->IsUnique()) {
        calc_slot.SetDefault(item_info->m_wUniPhyMinDamage, item_info->m_wUniPhyMaxDamage);
    }
    else {
        calc_slot.SetDefault(item_info->m_wMinDamage, item_info->m_wMaxDamage);
    }
    //
    const SCItemSlot& item_slot = info_node.item_slot;
    if (item_slot.IsLimited()) {
        calc_slot.AddRatioWeight_Limited(static_cast<float>(item_slot.GetDamageRatio()));
    }
    else if (item_slot.IsFateItem()) {
        calc_slot.AddRatioWeight_Fate(static_cast<float>(item_slot.GetFateAtkDefRatio()));
    }
    //
    ;{
        BYTE enchant = item_slot.GetEnchant();
        DWORD weights_power = ::ns_formula::CalcAttackPower(enchant, item_info->m_LV);
        calc_slot.AddWeightValue(weights_power);
    };
#ifdef _NA_008124_20150313_AWAKENING_SYSTEM
    {
        BYTE awakening = item_slot.GetAwakening();
        DWORD weights_power = ::ns_formula::CalcAwakeningWeapon(item_slot.GetCode(), awakening);
        calc_slot.AddWeightValue(weights_power);
    }
#endif // _NA_008124_20150313_AWAKENING_SYSTEM

#ifdef __NA_001062_20080623_STRENGTH_PENALTY
    ;{
        const DWORD penalty_index = item_info->m_dwStrengthPenaltyIdx;
        if (penalty_index != 0)
        {
            const sSTRENGTH_PENALTY* strength_penalty = \
                StrengthPenaltyParser::Instance()->GetStrengthPenaltyInfo(\
                    penalty_index, info_node.max_duration, info_node.duration);
            //
            calc_slot.AddRatioWeight_Strength(strength_penalty->m_fStrengthPro);
        }
    };
#endif

    //----------------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------------

    REGISTER_ATTR_NODE(eATTR_BASE_MELEE_MIN_ATTACK_POWER, eATTR_KIND_ITEM,
                       calc_slot.sum_of_min_power);
    REGISTER_ATTR_NODE(eATTR_BASE_MELEE_MAX_ATTACK_POWER, eATTR_KIND_ITEM,
                       calc_slot.sum_of_max_power);
    REGISTER_ATTR_NODE(eATTR_BASE_RANGE_MIN_ATTACK_POWER, eATTR_KIND_ITEM,
                       calc_slot.sum_of_min_power);
    REGISTER_ATTR_NODE(eATTR_BASE_RANGE_MAX_ATTACK_POWER, eATTR_KIND_ITEM,
                       calc_slot.sum_of_max_power);

    return true;
}

bool ItemAttrCalculator::CalculateArmor(const EquipSlotNode& info_node)
{
    WAVERIX_WRAPPER((WAVERIX_LC_CALL1("waverix_log_trace", __FUNCTION__)));
    //
    ns_formula::InterDefensePowerCalculator calc_slot = { 0, };
    const BASE_ITEMINFO* item_info = info_node.item_info;
    //
    if (info_node.item_slot.IsDivine()) {
        calc_slot.SetDefault(item_info->m_wDivPhyDef);
    }
    else if (info_node.item_info->IsElite()) {
        calc_slot.SetDefault(item_info->m_wElitePhyDef);
    }
    else if (info_node.item_info->IsUnique()) {
        calc_slot.SetDefault(item_info->m_wUniPhyDef);
    }
    else {
        calc_slot.SetDefault(item_info->m_wPhyDef);
    };
    
    const SCItemSlot& item_slot = info_node.item_slot;
    const BYTE enchant = item_slot.GetEnchant();
    calc_slot.sum_of_defense_power =
        ::ns_formula::CalcPhyDef(calc_slot.sum_of_defense_power, enchant, item_info->m_LV);

#ifdef _NA_008124_20150313_AWAKENING_SYSTEM
    const BYTE awakening = item_slot.GetAwakening();
    calc_slot.sum_of_defense_power +=
        ::ns_formula::CalcAwakeningArmor(item_slot.GetCode(), awakening);
#endif // _NA_008124_20150313_AWAKENING_SYSTEM

    // CHANGES: f110831.3L, do not update anymore if the selected defense power is zero
    if (calc_slot.sum_of_defense_power == 0) {
        return true;
    };
    //
    if (item_slot.IsLimited()) {
        calc_slot.AddRatioWeight_Limited(static_cast<float>(item_slot.GetDamageRatio()));
    }
    else if (item_slot.IsFateItem()) {
        calc_slot.AddRatioWeight_Fate(static_cast<float>(item_slot.GetFateAtkDefRatio()));
    }

#ifdef __NA_001062_20080623_STRENGTH_PENALTY
    ;{
        const DWORD penalty_index = item_info->m_dwStrengthPenaltyIdx;
        if (penalty_index != 0)
        {
            const sSTRENGTH_PENALTY* strength_penalty = \
                StrengthPenaltyParser::Instance()->GetStrengthPenaltyInfo(\
                    penalty_index, info_node.max_duration, info_node.duration);
            //
            calc_slot.AddRatioWeight_Strength(strength_penalty->m_fStrengthPro);
        }
    };
#endif

    //----------------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------------

    REGISTER_ATTR_NODE(eATTR_BASE_MELEE_DEFENSE_POWER, eATTR_KIND_ITEM,
                       calc_slot.sum_of_defense_power);
    REGISTER_ATTR_NODE(eATTR_BASE_RANGE_DEFENSE_POWER, eATTR_KIND_ITEM,
                       calc_slot.sum_of_defense_power);

    return true;
}

bool ItemAttrCalculator::CalculateAccessory(const EquipSlotNode& info_node)
{
    WAVERIX_WRAPPER((WAVERIX_LC_CALL1("waverix_log_trace", __FUNCTION__)));
    //
    ns_formula::ItemAttrContext& records = *info_node.context;
    __UNUSED((info_node, records));
    return true;
}


bool ItemAttrCalculator::CalculateOptionList(const EquipSlotNode& info_node)
{
    WAVERIX_WRAPPER((WAVERIX_LC_CALL1("waverix_log_trace", __FUNCTION__)));
    //
    const SCItemSlot& item_slot = info_node.item_slot;
    const nsSlot::ItemRuleInfo& item_rule_info = info_node.item_rule_info;
    const BASE_ITEMINFO* item_info = info_node.item_info;

    // NOTE: extra-stone option
    // CHANGES: f100406.2L, waverix, add filter condition
    //  because inventory effect item is not equip but can apply option
    if (item_rule_info.IsEnableExtraOption())
    {
        if (int n_option_attr_index = item_slot.GetNOptionAttrIndex())
        {
            const sEXTRASTONE_OPT_INFO* extra_option = item_slot.GetNOptionAttrDesc();
            if (extra_option && extra_option->m_byValueType < _countof(kAttrKindByValueTypes))
            {
                REGISTER_ATTR_NODE(item_slot.GetNOptionAttr(),
                                   kAttrKindByValueTypes[extra_option->m_byValueType],
                                   item_slot.GetNOptionAttrValue());
            }
        }
    } //end extra option part

    // (NOTE) calculate item option by script
    // (CHANGES) (WAVERIX) changes to more simple code
    const int upperbound_selector = pc_room_status_ ? 2 : 1; //Normal|PC option groups
    for (int selector = 0; selector < upperbound_selector; ++selector)
    {
        const BASE_ITEMINFO::OptionEffects* __option_effects = selector == 0 ?
            &item_info->m_byOptionExEffect : &item_info->m_byPCOptionExEffect;

        const BASE_ITEMINFO::OptionEffects& option_effects = *__option_effects;

        bool apply = ARR_CHECK_IS_CONTAIN(BASE_ITEMINFO::OPTION_EFFECT_EQUIP_MASK::MASK,
                                          option_effects);
        if (apply == false) {
            continue;
        }
        const BASE_ITEMINFO::OptionIndexes* __option_indexes = (selector == 0) ?
            &item_info->m_wOptionIndex : &item_info->m_wPCOptionIndex;
        const BASE_ITEMINFO::OptionTypes* __option_types = (selector == 0) ?
            &item_info->m_byOptionType : &item_info->m_byPCOptionType;
        const BASE_ITEMINFO::OptionValues* __option_values = (selector == 0) ?
            &item_info->m_iOptionValue : &item_info->m_iPCOptionValue;
        //
        const BASE_ITEMINFO::OptionIndexes& option_indexes = *__option_indexes;
        const BASE_ITEMINFO::OptionTypes& option_types = *__option_types;
        const BASE_ITEMINFO::OptionValues& option_values = *__option_values;

        for (int op = 0; op < _countof(option_indexes); ++op)
        {
            bool is_valid_values = option_indexes[op] != 0 && 
                                   option_values[op] != 0 &&
                                   option_indexes[op] < _countof(g_ITEM_OPTION) &&
                                   option_types[op] < _countof(kAttrKindByValueTypes);
            if (is_valid_values == false) {
                continue;
            }
            REGISTER_ATTR_NODE(g_ITEM_OPTION[option_indexes[op]],
                               kAttrKindByValueTypes[option_types[op]],
                               option_values[op]);
        }
    }

    return true;
}

// NOTE: f110906.2L, added 'working_slot_root' to prevent a special option duplicated calculation.
bool ItemAttrCalculator::CalculateSetItemOptions(const EquipSlotNode& info_node,
    ns_formula::EquipWorkingSlots* const working_slot_root)
{
    WAVERIX_WRAPPER((WAVERIX_LC_CALL1("waverix_log_trace", __FUNCTION__)));
    //
    WORD set_item_group = info_node.item_info->set_group_code;
    if (set_item_group == 0) {
        assert(!"need precedence check");
        return false;
    };
    //
    SetItemOptionInfoParser* const set_item_parser = SetItemOptionInfoParser::Instance();
    const SETITEM_OPTION_INFO* const set_item_option_info =
        set_item_parser->Find(set_item_group);

    // replaced by _SOLAR_SLOT_INTEGRITY_CONTAINER_PROCESS_
    //현재 장착되어 있는 아이템중 해당 타입의 세트 아이템을 가지고 온다.
    // (CHANGES) find related set item, to support special accessory multi-link
    SCItemSlot* temp_set_item_slot_array[MAX_INVENTORY_SLOT_NUM];
    SCItemSlotContainer::SameSetItemSlotsResult find_result = \
        equip_container_.GetSameSetItemSlots(temp_set_item_slot_array,
                                             _countof(temp_set_item_slot_array),
                                             info_node.item_info, true);
    // compatible set
    if (find_result.number_of_same_set_items == 0) {
        return true;
    }

	SCItemSlot* temp_set_item_slot_array2[MAX_INVENTORY_SLOT_NUM]= { 0, };
	SCItemSlotContainer::SameSetItemSlotsResult find_result_Total = \
		equip_container_.GetSameSetItemTotalSlots(temp_set_item_slot_array2,
		_countof(temp_set_item_slot_array2),
		info_node.item_info, true);


    int number_of_same_set_items = find_result.number_of_same_set_items;
    set_item_group = find_result.set_item_code; // replacing
    // activated_set_items is sequential order
    ns_formula::EquipSlotsManageInfo::Node* activated_set_items[MAX_EQUIPMENT_SLOT_NUM] = { 0, };

    ;{
        ns_formula::EquipSlotsManageInfo::Node** dest_it = activated_set_items;
        SCItemSlot** it = temp_set_item_slot_array,
                  ** end = temp_set_item_slot_array + number_of_same_set_items;
        for ( ; it != end; ++it)
        {
            int pos = (*it)->GetPos();
            if (ns_formula::EquipSlotsManageInfo::Node* node = &slot_cache_info_->slots[pos])
            {
                if (node->activated == false) {
                    continue;
                }
                *dest_it++ = node;
            };
        }
        // CHANGES: f110419.4L, modify numbers to current activated set items
        number_of_same_set_items = static_cast<int>(dest_it - activated_set_items);
    };
    //----------------------------------------------------------------------------------------------
    const BASE_ITEMINFO* const item_info = info_node.item_info;
    bool is_special_accessory_process = \
        (item_info->m_bySetOptionType == item_info->eSetItem_Special);
    //----------------------------------------------------------------------------------------------
    BOOST_STATIC_ASSERT(SETITEM_OPTION_LEVEL_NONE == 0);
    const eSETITEM_OPTION_LEVEL changed_level = (number_of_same_set_items <= 0)
        ?   SETITEM_OPTION_LEVEL_NONE
        :   set_item_parser->GetSetItemOptionLevel(number_of_same_set_items);
    if (changed_level == SETITEM_OPTION_LEVEL_NONE) {
        return true;
    };
    const eSETITEM_OPTION_LEVEL prev_level = ((number_of_same_set_items - 1) <= 0)
        ?   SETITEM_OPTION_LEVEL_NONE
        :   set_item_parser->GetSetItemOptionLevel(number_of_same_set_items - 1);
    //
    const bool changed_option_level = (prev_level != changed_level);
    //----------------------------------------------------------------------------------------------
    // NOTE: incremental update routine, the new inserted item to apply set option has not
    // pre-historical update options so that basically set options of the item apply until latest grade
    if (is_special_accessory_process == false)
    {
        for (int level = SETITEM_OPTION_LEVEL_FIRST;
             changed_option_level ? (level < changed_level) : (level <= changed_level);
             ++level)
        {
            WAVERIX_WRAPPER((WAVERIX_LC_CALL3("waverix_log_trace2",
                "|Apply Single|{Pos, Level}=", info_node.position, level)));
            const SET_ITEM_OPTION* set_item_option =
                set_item_parser->GetCurrentSetItemOption(set_item_option_info,
                info_node.position, static_cast<eSETITEM_OPTION_LEVEL>(level));
            if (set_item_option == NULL) {
                continue;
            };

            if (set_item_option->m_byOptionKind == 0 ||
                set_item_option->m_byOptionKind >= _countof(g_ITEM_OPTION) ||
                set_item_option->m_nOption == 0 ||
                set_item_option->m_byUseType == 0 ||
                set_item_option->m_byUseType >= _countof(kAttrKindByValueTypes))
            {
                continue;
            }
#ifdef _NA_002935_20110704_ITEM_SCORE
            attributes_.GetSetOptionAttributes().AddAttr(
                g_ITEM_OPTION[set_item_option->m_byOptionKind], 
                kAttrKindByValueTypes[set_item_option->m_byUseType], 
                set_item_option->m_nOption);
#endif // _NA_002935_20110704_ITEM_SCORE
            REGISTER_ATTR_NODE(g_ITEM_OPTION[set_item_option->m_byOptionKind],
                               kAttrKindByValueTypes[set_item_option->m_byUseType],
                               set_item_option->m_nOption);
        };
    };
    //----------------------------------------------------------------------------------------------
    // apply option effects for all same set items (including inserted now slot)
    if ((changed_option_level) && (is_special_accessory_process == false))
    {
        // s
        ns_formula::EquipSlotsManageInfo::Node** it = activated_set_items,
            ** end = &activated_set_items[_countof(activated_set_items)];
        for ( ; it != end; ++it)
        {
            // activated_set_items is sequential order
            if (*it == NULL) {
                break;
            }
            WAVERIX_WRAPPER((WAVERIX_LC_CALL3("waverix_log_trace2",
                "|Apply grade option|{Pos, Level}=", (*it)->position, changed_level)));
            const SET_ITEM_OPTION* set_item_option =
                set_item_parser->GetCurrentSetItemOption(set_item_option_info,
                    (*it)->position,
                    static_cast<eSETITEM_OPTION_LEVEL>(changed_level));
            if (set_item_option == NULL) {
                continue;
            };
            //
            if (set_item_option->m_byOptionKind == 0 ||
                set_item_option->m_byOptionKind >= _countof(g_ITEM_OPTION) ||
                set_item_option->m_nOption == 0 ||
                set_item_option->m_byUseType == 0 ||
                set_item_option->m_byUseType >= _countof(kAttrKindByValueTypes))
            {
                continue;
            };
#ifdef _NA_002935_20110704_ITEM_SCORE
            attributes_.GetSetOptionAttributes().AddAttr(
                g_ITEM_OPTION[set_item_option->m_byOptionKind],
                kAttrKindByValueTypes[set_item_option->m_byUseType],
                set_item_option->m_nOption);
#endif // _NA_002935_20110704_ITEM_SCORE
            REGISTER_ATTR_NODE(g_ITEM_OPTION[set_item_option->m_byOptionKind],
                               kAttrKindByValueTypes[set_item_option->m_byUseType],
                               set_item_option->m_nOption);
        };
    };
    //
    //----------------------------------------------------------------------------------------------
    // full option
    const bool is_full_option = set_item_parser->IsFullOptionCount(
        set_item_option_info, number_of_same_set_items);
    //
    if ((is_full_option) && (is_special_accessory_process == false))
    {
        SetOptionAttributes& setoption_attrs = attributes_.GetSetOptionAttributes();

        const SET_ITEM_OPTION* it = set_item_option_info->m_FullOption,
            * const end = &set_item_option_info->m_FullOption[\
                _countof(set_item_option_info->m_FullOption)];

#ifdef _NA_000000_20160204_MODIFY_SETOPTION_APPLIED_LIST
        if (setoption_attrs.IsAppliedThisSetOption(set_item_group) == false)
        {
            // 세트옵션 미적용상태일 경우에만 적용시킨다
            setoption_attrs.SetSetGroupCode(set_item_group);
            for ( ; it != end; ++it)
            {
                WAVERIX_WRAPPER((WAVERIX_LC_CALL1("waverix_log_trace2",
                    "|Apply full option|{}=")));
                bool is_valid_attr_info = it->m_byOptionKind != 0 &&
                    it->m_byOptionKind < _countof(g_ITEM_OPTION) &&
                    it->m_nOption != 0 &&
                    it->m_byUseType != 0 &&
                    it->m_byUseType < _countof(kAttrKindByValueTypes);
                if (is_valid_attr_info == false) {
                    continue;
                };

                int value_double = static_cast<int>(find_result.number_of_same_set_items) -
                    static_cast<int>(find_result_Total.number_of_same_set_items);

                //풀셋의 경우 중복 적용되는 경우가 있음.
                //이를 방지함 (특히 악세사리)
                //셋트 option의 최대치는 몇일까?

                //_NA_008664_20160129_MODIFY_SETITEMOPTION_CONDITION
                // 세트옵션 적용조건 변경
                //if(value_double == 0 && attributes_.GetSetOptionAttributes().GetAttrValue(g_ITEM_OPTION[it->m_byOptionKind]).fix_value < it->m_nOption)
                attributes_.GetSetOptionAttributes().AddAttr(
                    g_ITEM_OPTION[it->m_byOptionKind], 
                    kAttrKindByValueTypes[it->m_byUseType],
                    it->m_nOption);

                REGISTER_ATTR_NODE(g_ITEM_OPTION[it->m_byOptionKind],
                                   kAttrKindByValueTypes[it->m_byUseType],
                                   it->m_nOption);
            };
        }
#else
        for ( ; it != end; ++it)
        {
            WAVERIX_WRAPPER((WAVERIX_LC_CALL1("waverix_log_trace2",
                "|Apply full option|{}=")));
            bool is_valid_attr_info = it->m_byOptionKind != 0 &&
                it->m_byOptionKind < _countof(g_ITEM_OPTION) &&
                it->m_nOption != 0 &&
                it->m_byUseType != 0 &&
                it->m_byUseType < _countof(kAttrKindByValueTypes);
            if (is_valid_attr_info == false) {
                continue;
            };

#ifdef _NA_002935_20110704_ITEM_SCORE
#ifdef _NA_006655_20130422_BUG_ACCESSORY_SETOPTION
#else //_NA_006655_20130422_BUG_ACCESSORY_SETOPTION
            attributes_.GetSetOptionAttributes().AddAttr(
                g_ITEM_OPTION[it->m_byOptionKind], 
                kAttrKindByValueTypes[it->m_byUseType],
                it->m_nOption);
#endif //_NA_006655_20130422_BUG_ACCESSORY_SETOPTION

            int value_double = static_cast<int>(find_result.number_of_same_set_items) -
                static_cast<int>(find_result_Total.number_of_same_set_items);

            //풀셋의 경우 중복 적용되는 경우가 있음.
            //이를 방지함 (특히 악세사리)
            //셋트 option의 최대치는 몇일까?

#ifdef _NA_006655_20130422_BUG_ACCESSORY_SETOPTION
            //_NA_008664_20160129_MODIFY_SETITEMOPTION_CONDITION
            // 세트옵션 적용조건 변경
            //if(value_double == 0 && attributes_.GetSetOptionAttributes().GetAttrValue(g_ITEM_OPTION[it->m_byOptionKind]).fix_value < it->m_nOption)

            if(value_double == 0)
            {
                attributes_.GetSetOptionAttributes().AddAttr(
                    g_ITEM_OPTION[it->m_byOptionKind], 
                    kAttrKindByValueTypes[it->m_byUseType],
                    it->m_nOption);

                REGISTER_ATTR_NODE(g_ITEM_OPTION[it->m_byOptionKind],
                                   kAttrKindByValueTypes[it->m_byUseType],
                                   it->m_nOption);
            }
#else
            //_NA_008664_20160129_MODIFY_SETITEMOPTION_CONDITION
            // 세트옵션 적용조건 변경
            //if(value_double == 0 && attributes_.GetSetOptionAttributes().GetAttrValue(g_ITEM_OPTION[it->m_byOptionKind]).fix_value < it->m_nOption)
            if(value_double == -1)
            {	
                attributes_.GetSetOptionAttributes().AddAttr(
                    g_ITEM_OPTION[it->m_byOptionKind], 
                    kAttrKindByValueTypes[it->m_byUseType],
                    it->m_nOption * -1);
            }

            REGISTER_ATTR_NODE(g_ITEM_OPTION[it->m_byOptionKind],
                               kAttrKindByValueTypes[it->m_byUseType],
                               it->m_nOption);
#endif //_NA_006655_20130422_BUG_ACCESSORY_SETOPTION
#endif // _NA_002935_20110704_ITEM_SCORE
        };
#endif //_NA_000000_20160204_MODIFY_SETOPTION_APPLIED_LIST
        
        // CHANGES: f110623.4L, fixed the special accessory item unexpected ordering
        // calulation problem that doesn't applied additional special options
        // when a full set item equipped.
        is_special_accessory_process = true;
    };
    //
    //----------------------------------------------------------------------------------------------
    // special accessory option
    if ((is_full_option) && (is_special_accessory_process))
    {
        bool apply_special_accessory_full_set_options = false;
        const SPECIAL_EQUIP_ACTIVE_OPTION& special_option =
            set_item_option_info->m_SpecialEquipOptionItem;
        // search same item group to regard multi-equippable cases.
        //if (special_option.m_wItemCode == item_info->m_Code)
        //    apply_special_accessory_full_set_options = true;
        //--------------------------------------------------------------------------
        // NOTE: if special accessory is already equipped,
        // the actor that is the equip calulation item has not the main agent
        // for full option calculating.
        // NOTE: action, the changes related to sun:r58153~ implemented by jaykang
        // process a item that is matched with the finding item in same type ranges.
        const SCItemSlot& matched_special_item = equip_container_.GetItemOfType(\
            special_option.m_EqPos, special_option.m_wItemCode);
        SLOTCODE item_code = matched_special_item.GetItemCode();
        if (item_code != 0 && item_code == special_option.m_wItemCode)
        {
            POSTYPE pos = matched_special_item.GetPos();
            ns_formula::EquipSlotsManageInfo::Node* node = &slot_cache_info_->slots[pos];
            if (node->activated &&
                working_slot_root->slots[pos].applied_special_fullset_option == false)
            {
                apply_special_accessory_full_set_options = true;
                working_slot_root->slots[pos].applied_special_fullset_option = true;
            };
        };

        if (apply_special_accessory_full_set_options)
        {
            WAVERIX_WRAPPER((WAVERIX_LC_CALL3("waverix_log_trace2",
                "|Apply special option|{Pos, Code}=",
                special_option.m_EqPos, special_option.m_wItemCode)));
            const SET_ITEM_OPTION* it = special_option.m_ActiveOption,
                * const end = &special_option.m_ActiveOption[\
                    _countof(special_option.m_ActiveOption)];
            for ( ; it != end; ++it)
            {
                bool is_valid_attr_info = it->m_byOptionKind != 0 &&
                                          it->m_byOptionKind < _countof(g_ITEM_OPTION) &&
                                          it->m_nOption != 0 &&
                                          it->m_byUseType != 0 &&
                                          it->m_byUseType < _countof(kAttrKindByValueTypes);
                if (is_valid_attr_info == false) {
                    continue;
                };
#ifdef _NA_002935_20110704_ITEM_SCORE
                attributes_.GetSetOptionAttributes().AddAttr(
                    g_ITEM_OPTION[it->m_byOptionKind],
                    kAttrKindByValueTypes[it->m_byUseType],
                    it->m_nOption);
#endif // _NA_002935_20110704_ITEM_SCORE
                REGISTER_ATTR_NODE(g_ITEM_OPTION[it->m_byOptionKind],
                                   kAttrKindByValueTypes[it->m_byUseType],
                                   it->m_nOption);
            };

        }
    }
    //----------------------------------------------------------------------------------------------

    return true;
}

//
bool ItemAttrCalculator::CalculateSocketOptions(const EquipSlotNode& info_node)
{
    WAVERIX_WRAPPER((WAVERIX_LC_CALL1("waverix_log_trace", __FUNCTION__)));
    // 포인트 방식 계산에서는 장착 중인 모든 아이템의 옵션 포인트합을 기준으로 실제 옵션값으로 변환해야 한다.
    // 아이템의 착용 상태가 변경될 때 다음과 같이 계산한다.
    // 1. 착용 상태 변경 전에 적용된 옵션값을 제거한다.
    // 2. 착용 상태 변경 후의 옵션 포인트를 계산해서 옵션값으로 변환한 후 적용한다.

    // 장착 또는 해제하려는 아이템만의 옵션 포인트를 계산한다.
    using ns_formula::SocketSearchBoard;
    SocketSearchBoard search_board;
    if (search_board.SearchSocketOptionPoint(info_node.item_slot, &search_board) == false) {
        return false;
    };
#if USE_CLIENT_SOCKET_OPTION_TABLE
    // CHANGES: f110530.3L
    assert(info_node.position < _countof(this->slot_cache_info_->item_socket_option_list));
    EquipSlotsManageInfo::ItemSocketOptionList* const option_list =
        &this->slot_cache_info_->item_socket_option_list[info_node.position];
    option_list->resize(0);
    option_list->reserve(search_board.number_of_registered);
#endif
    //
    const SocketSearchBoard::RegistNode* regist_it = search_board.registered_nodes,
        * regist_end = &search_board.registered_nodes[search_board.number_of_registered];
    for ( ; regist_it != regist_end; ++regist_it)
    {
        SocketSearchBoard::node_type* result_node = regist_it->link_node;
        //
        const eATTR_TYPE attr_type = result_node->attr_option_code;
        assert(attr_type > eATTR_TYPE_INVALID && attr_type < eATTR_MAX);
        // 옵션값을 계산한다.
        //CalculateItemOptionValue(is_add, item_calc_result_pair.second, item_calculator);

        // 이전 계산 결과를 제거한다.
        SocketOptionAttributes::AttrValue& prev_attr_value = 
            socket_attributes_.GetAttrValueByMutable(attr_type);

        if (prev_attr_value.calc_value != 0)
        {
            ;{  // configure operator
                assert(info_node.current_operate == Attributes::Operation::eOper_Add);
                info_node.current_operate = Attributes::Operation::eOper_Sub;
            };
            REGISTER_ATTR_NODE(attr_type,
                               kAttrKindByValueTypes[result_node->option_type],
                               prev_attr_value.calc_value);
            ;{  // configure operator
                info_node.current_operate = Attributes::Operation::eOper_Add;
            };
        };
        // update point
        prev_attr_value.point += result_node->option_point;
        prev_attr_value.calc_value = 0;
        //
        const ZardOptionInfo* zard_option = regist_it->option_info;
        if (const ZardOptionInfo::OptionValueInfo* zard_option_value = \
                zard_option->Find(prev_attr_value.point))
        {
            prev_attr_value.calc_value = zard_option_value->value;
        }

        // 새로운 계산 결과를 적용한다.
        if (prev_attr_value.calc_value != 0)
        {
            REGISTER_ATTR_NODE(attr_type,
                               kAttrKindByValueTypes[result_node->option_type],
                               prev_attr_value.calc_value);
        #if USE_CLIENT_SOCKET_OPTION_TABLE
            // CHANGES: f110530.3L
            typedef EquipSlotsManageInfo::ItemSocketOption CastType;
            const SocketSearchBoard::node_type* node = regist_it->link_node;
            option_list->push_back(*node);
        #endif
        }
    };

    return true;
}


// static
bool ItemAttrCalculator::SocketOptionCalculator::CalculateItemOptionPoint(
    const SCItemSlot& item_slot, CalculationResultMap& OUT item_calc_result_list)
{
    WAVERIX_WRAPPER((WAVERIX_LC_CALL1("waverix_log_trace", __FUNCTION__)));
    // 포인트 방식 계산에서는 장착 중인 모든 아이템의 옵션 포인트합을 기준으로 실제 옵션값으로 변환해야 한다.
    // 아이템의 착용 상태가 변경될 때 다음과 같이 계산한다.
    // 1. 착용 상태 변경 전에 적용된 옵션값을 제거한다.
    // 2. 착용 상태 변경 후의 옵션 포인트를 계산해서 옵션값으로 변환한 후 적용한다.
    item_calc_result_list.resize(0);

    // 장착 또는 해제하려는 아이템만의 옵션 포인트를 계산한다.
    using ns_formula::SocketSearchBoard;
    SocketSearchBoard search_board;
    if (search_board.SearchSocketOptionPoint(item_slot, &search_board) == false) {
        return false;
    }
    //
    item_calc_result_list.reserve(search_board.number_of_registered);
    //
    const SocketSearchBoard::RegistNode* regist_it = search_board.registered_nodes,
        * regist_end = &search_board.registered_nodes[search_board.number_of_registered];
    for ( ; regist_it != regist_end; ++regist_it)
    {
        typedef ItemAttrCalculator::SocketOptionCalculator::CalculationResult CastType;

        const SocketSearchBoard::node_type* node = regist_it->link_node;
        item_calc_result_list.push_back(CalculationResultMap::value_type(\
            node->attr_option_code, static_cast<const CastType&>(*node)));
    }
    return true;
}

#ifdef _NA_006731_20130521_ENCHANT_ADD_OPTION
bool ItemAttrCalculator::CalculateEnchantOption(const EquipSlotNode& info_node)
{
    const SCItemSlot& item_slot = info_node.item_slot;
    const nsSlot::ItemRuleInfo& item_rule_info = info_node.item_rule_info;
    const BASE_ITEMINFO* item_info = info_node.item_info;

    const bool is_weapon = item_info->IsWeapon(); // 0:무기, 1:방어구
    const LEVELTYPE item_level = item_info->m_LV; // 아이템 등급
    const uint8_t enchant_grade = item_slot.GetEnchant();

    // wing item..제외.
    if (item_info->IsSpecialAccessory())
    {
        return false;
    }

    const EnchantParser* const enchant_parser = EnchantParser::Instance();
    const MATERIALS_FOR_LEVEL* const enchant_info = \
        enchant_parser->GetEnchantInfo(is_weapon, item_level, enchant_grade);
    if (enchant_info == NULL)
    {
        return false;
    }

    for (int op = 0; op < _countof(enchant_info->add_option_kind_); ++op)
    {
        bool is_valid_values = enchant_info->add_option_kind_[op] != 0 && 
            enchant_info->add_option_value_[op] != 0 &&
            enchant_info->add_option_kind_[op] < _countof(g_ITEM_OPTION) &&
            enchant_info->add_option_type_[op] < _countof(kAttrKindByValueTypes);
        if (is_valid_values == false) {
            continue;
        }

        REGISTER_ATTR_NODE(g_ITEM_OPTION[enchant_info->add_option_kind_[op]],
                           kAttrKindByValueTypes[enchant_info->add_option_type_[op]],
                           enchant_info->add_option_value_[op]);
    }

    return true;
}
#endif // _NA_006731_20130521_ENCHANT_ADD_OPTION

//==================================================================================================
//==================================================================================================
//==================================================================================================
//==================================================================================================
//==================================================================================================

#endif //_NA_002289_20110322_EQUIPMENTS_CALCULATOR_CHANGES_FOR_IMPROVEMENT