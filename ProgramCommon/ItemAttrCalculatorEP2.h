#ifndef __PROGRAMCOMMON_ITEM_ATTR_CALCULATOR_EP2_H
#define __PROGRAMCOMMON_ITEM_ATTR_CALCULATOR_EP2_H
#pragma once
#if defined(_NA_002289_20110322_EQUIPMENTS_CALCULATOR_CHANGES_FOR_IMPROVEMENT)
//==================================================================================================
#define PROGRAMCOMMON_ITEM_ATTR_CALCULATOR_EP2_TYPES
#include "./ItemAttrCalculator_Types.hxx"
#undef PROGRAMCOMMON_ITEM_ATTR_CALCULATOR_EP2_TYPES

//==================================================================================================
//==================================================================================================
//==================================================================================================
// ItemAttrCalculator
// - Item과 관련해서 변화하는 Attr을 Attributes에 update한다.
class ItemAttrCalculator
{
public:
    static const int kVersion = 2;
    //
    struct EquipSlotNode;
    class SocketOptionCalculator;
    //                                      [eVALUE_TYPE]
    static const eATTR_KIND kAttrKindByValueTypes[6];
    //
    ItemAttrCalculator(PlayerAttributes& attributes,
                       IEquipmentRestriction* equipment_accessor);
    virtual ~ItemAttrCalculator();

    void Clear();
    // special interfaces
    // NOTE: f100812.2L, add an interface to support external module can access add rule filter
    static void ModifyAttrKindAndCalculatedValue(uint8_t value_type, //eVALUE_TYPE
                                                 uint8_t* attr_kind, //eATTR_KIND
                                                 int * value);
    // added interfaces to replace version 1
    // Checking list = { STR, DEX, VIT, INT, SPR, Level }
    static bool CanEquipmentValidStats(const SCItemSlot& item_slot,
                                       const PlayerAttributes& player_attr, LEVELTYPE player_level);

    bool Equip2(const SCItemSlot& IN slot,
                bool is_pc_room, bool use_update_ex, LEVELTYPE player_level);
    // NOTE: f110415.7L, test whether new inserting item enabled equips instead of origin equipments
    bool TestSwitchableEquipments(const SCItemSlot* equip_test_item,
        const POSTYPE to_slot_pos, bool is_pc_room, bool use_update_ex, LEVELTYPE player_level);
    bool Unequip2(const SCItemSlot* slot, POSTYPE slot_pos,
                  bool is_pc_room, bool use_update_ex, LEVELTYPE player_level);
    bool EquipAll(bool is_pc_room, bool use_update_ex, LEVELTYPE player_level);
    // changed item durability, change time expiration, etc...
    bool ChangeItemDurabilityStatus(const SCItemSlot& IN slot, POSTYPE slot_pos,
                                    bool is_pc_room, bool use_update_ex, LEVELTYPE player_level);

private:
    //----------------------------------------------------------------------------------------------
    // private methods
    // arguments: 'hint' = { eItemAttrCalculatorInterControlHint };
    bool CalculateTotalConditionalEquipments(const int hint, const int hint_arg);
    //----------------------------------------------------------------------------------------------
    // calculators based on item partially functionality
    bool CalculateEquipments(const EquipSlotNode& info_node);
    bool CalculateWeapon(const EquipSlotNode& info_node);
    bool CalculateArmor(const EquipSlotNode& info_node);
    bool CalculateAccessory(const EquipSlotNode& info_node);
    bool CalculateOptionList(const EquipSlotNode& info_node);
    //
    bool CalculateOptionNode(const EquipSlotNode& info_node,
        eATTR_TYPE attr_type, eATTR_KIND attr_kind, int attr_value);
    // NOTE: f110906.2L, added 'working_slot_root' to prevent a special option duplicated calculation.
    bool CalculateSetItemOptions(const EquipSlotNode& info_node,
        ns_formula::EquipWorkingSlots* const working_slot_root);
    //
    bool CalculateSocketOptions(const EquipSlotNode& info_node);
#ifdef _NA_006731_20130521_ENCHANT_ADD_OPTION
    bool CalculateEnchantOption(const EquipSlotNode& info_node);
#endif // _NA_006731_20130521_ENCHANT_ADD_OPTION
    //----------------------------------------------------------------------------------------------
    // use : info_node.context
    bool CanEquipmentValidStatsInternal(const EquipSlotNode& info_node);
    static bool CanEquipmentStatComparison(const ns_formula::ItemAttrStats& player_stats,
                                           const ns_formula::ItemLimitations& item_limits);
    static bool LoadPlayerLimits(ns_formula::ItemAttrStats* const player_stats,
                                 const PlayerAttributes& attributes,
                                 LEVELTYPE player_level);
    static bool LoadEquipmentLimits(ns_formula::ItemLimitations* const item_limits,
                                    const SCItemSlot& item_slot);
    // NOTE: f110415.7L, add check interfaces to support swichable test
    static bool CheckExistLimits(const ns_formula::ItemLimitations& item_limits);
    bool UpdateEquipSlotsLatestInfo(bool activated, const EquipSlotNode& info_node,
        ns_formula::EquipSlotsManageInfo::Node* const work_node) const;
    bool SetupDefaultEquipSlotsLatestInfo(ns_formula::EquipSlotsManageInfo::Node* const work_node,
        POSTYPE pos, DURATYPE duration) const;
    bool UpdatePlayerSimulationStatsByItemStats(const EquipSlotNode& info_node);
    //----------------------------------------------------------------------------------------------
    // fields...
    PlayerAttributes& attributes_;
    SocketOptionAttributes& socket_attributes_; //_NA_000251_20100727_SOCKET_SYSTEM
    SCItemSlotContainer& equip_container_;
    IEquipmentRestriction* const equipment_accessor_;
    ns_formula::EquipSlotsManageInfo* const slot_cache_info_;
    //bool auto_update_;
    bool pc_room_status_;
    bool use_update_ex_;
    LEVELTYPE player_level_;
    int inter_control_mode_;
    //
    __DISABLE_COPY(ItemAttrCalculator);
};

//==================================================================================================
// implemented by 
struct ItemAttrCalculator::EquipSlotNode
{
public:
    enum eDetailedResult {
        // can't find item information
        eResult_CantProcess,
        // default equips = { weapon, armor, accessories, }
        eResult_AllProcess,
        // not default equips, no options
        eResult_NoOptions,
        // not default equips, but exist options
        eResult_OptionsOnly,
        eResult_Counts
    };
    static bool Initialize(
        eDetailedResult* result,
        EquipSlotNode* using_buffer,
        const SCItemSlot* slot);
private:
    EquipSlotNode(
        const SCItemSlot& input_slot,
        const BASE_ITEMINFO* input_item_info,
        const nsSlot::ItemRuleInfo& input_item_rule_info);
    //
    mutable bool validated;
    uint8_t init_result;
    ns_formula::ItemAttrContext* context;
    // working node infomation
    const SCItemSlot& item_slot;
    const BASE_ITEMINFO* item_info;
    const nsSlot::ItemRuleInfo& item_rule_info;
    // temporary storage
    // if duration is changed, ... need prev-original data
    POSTYPE position;
    DURATYPE duration;
    DURATYPE max_duration;
    mutable uint8_t current_operate;
    //
    friend class ItemAttrCalculator;
    friend class ItemAttrCalculator::SocketOptionCalculator;
    //
    __DISABLE_COPY(EquipSlotNode);
};

//==================================================================================================
// implemented by _NA_000251_20100727_SOCKET_SYSTEM
//----------------------------------------------------------------------------------------------
// 소켓 옵션 계산기
class ItemAttrCalculator::SocketOptionCalculator
{
public:
    // 옵션 계산 결과
    typedef ns_formula::EquipSlotsManageInfo::ItemSocketOption CalculationResult;
#if SUN_CODE_BACKUP
    struct CalculationResult
    {
        typedef BYTE ItemOptionCode;
        typedef int OptionPoint;
        typedef int OptionValue;
            
        eATTR_TYPE attr_option_code;
        ItemOptionCode item_option_code;
        OptionPoint option_point;
        eVALUE_TYPE option_type;
        OptionValue option_value;

        static const CalculationResult kDefault;
    };
#endif
    typedef std::pair<eATTR_TYPE, CalculationResult> CalculationResultNodeType;
    typedef STLX_VECTOR<CalculationResultNodeType> CalculationResultMap;
    //typedef STLX_MAP<eATTR_TYPE, CalculationResult> CalculationResultMap;
    //
public:
    // 아이템이 가진 옵션 포인트를 계산한다.
    static bool CalculateItemOptionPoint(
        const SCItemSlot& item_slot, CalculationResultMap& OUT item_calc_result_map);

};

//==================================================================================================
// inlining methods
//



//==================================================================================================
//==================================================================================================
//==================================================================================================

#endif //_NA_002289_20110322_EQUIPMENTS_CALCULATOR_CHANGES_FOR_IMPROVEMENT
#endif //__PROGRAMCOMMON_ITEM_ATTR_CALCULATOR_EP2_H
