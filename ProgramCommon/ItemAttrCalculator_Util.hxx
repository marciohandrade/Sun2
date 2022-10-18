#ifndef PROGRAMCOMMON_ITEM_ATTR_CALCULATOR_INTER_UTIL
    #error "Can't direct include this file"
#endif
#if defined(_NA_002289_20110322_EQUIPMENTS_CALCULATOR_CHANGES_FOR_IMPROVEMENT)

#include "./ItemAttrCalculator.h"

#include <PlayerAttributes.h>
#include <ItemInfoParser.h>
#include <StrengthPenaltyParser.h>
//
#include <ItemFormula.h>
// SolarSlot
#include <ItemOptionHeader.h>
#include <SCItemSlot.h>
//

//==================================================================================================
//==================================================================================================
//==================================================================================================

namespace ns_formula {
;

//==================================================================================================
struct ItemAttrStats;
struct ItemLimitations;
struct ItemAttrContext;
struct ItemAttrContextList;

//==================================================================================================

struct ItemAttrStatsBase
{
    enum {
        eSTAT_STR, // link eATTR_STR
        eSTAT_DEX, // link eATTR_DEX
        eSTAT_VIT, // link eATTR_VIT
        eSTAT_INT, // link eATTR_INT
        eSTAT_SPR, // link eATTR_SPR
        //eSTAT_ALL_STATS,  // link eATTR_INCREASE_STAT_POINT
        eSTAT_DEC_LIMITS, // link eATTR_DECREASE_LIMIT_STAT
        //
        eSTAT_SIMUL_COUNTS
    };
};

struct ItemAttrStats : ItemAttrStatsBase
{
    LEVELTYPE level;
    AttrValue stats[eSTAT_SIMUL_COUNTS];
};

struct ItemLimitations : ItemAttrStatsBase
{
    LEVELTYPE level;
    short limits[eSTAT_SIMUL_COUNTS];
};

//==================================================================================================

struct ItemAttrContext
{
    // partial record operations in the working context
    Attributes::OperationRecord* BeginRecord();
    bool CommitRecord();
    bool RollbackRecord();
    //
    POSTYPE equip_position;
    bool calculated_stats;
    Attributes::OperationRecord* working_record;
    //
    Attributes::OperationRecord* begin_it;
    Attributes::OperationRecord* workable_it;
    Attributes::OperationRecord* end_it;
    //
    ItemAttrContextList* parent;
    //
    ItemLimitations limits;
};

struct ItemAttrContextList
{
    // kMaxNumberOfContexts :
    //  normal operation has 1 (add) operation
    //  = Equip => Add:1
    //  decreasing operation have 2 (sub, add) operations
    //  = change status (duration changes)
    //    duration(=0) => same as EquipAll (equipped, but inactivated)
    //  full operation have maximum 24 operations
    //    Unequip => Flush -> EquipAll
    //    change status (time expired) => same as Unequip
    static const int kMaxNumberOfContexts = MAX_EQUIPMENT_SLOT_NUM;
    // Weapon = { Attack power, ... }
    // Armor = { Defense power, Block ratio, ... }
    static const int kMaxNumberOfDefaultWeaponArmorOptions = 10;
    static const int kMaxNumberOfDefaultOptions = (BASE_ITEMINFO::_MAX_OPTION_NUM * 2);
    static const int kMaxNumberOfExtraOptions =
        3 + // TODO: extra options, need analysis
        20; // paddings
    // MAX_SETITEM_OPTION_LEVEL (per item) * MAX_SETITEM_ACTIVE_EQUIP_OPTION (per parts)
    // + MAX_SETITEM_FULL_OPTION (full option)
    // + MAX_SETITEM_SPECIAL_OPTION (activates based on full option)
    static const int kMaxNumberOfSetItemOptions =
        (MAX_SETITEM_OPTION_LEVEL * MAX_SETITEM_ACTIVE_EQUIP_OPTION) +
        (MAX_SETITEM_FULL_OPTION) +
        (MAX_SETITEM_SPECIAL_OPTION);
    // basically, this operation have 2 operations like previous substration and post addition
    static const int kMaxNumberOfSocketOptions =
        (2) *
        (SOCKET_MAX * sSOCKET_OPTION::OptionPointInfo::kMaxListSize);

    static const int kMaxNumberOfContextRecords =
        (kMaxNumberOfDefaultWeaponArmorOptions +
         kMaxNumberOfDefaultOptions +
         kMaxNumberOfExtraOptions +
         kMaxNumberOfSetItemOptions +
         kMaxNumberOfSocketOptions);

    static const int kNumberOfTotalRecords =
        (kMaxNumberOfContexts * kMaxNumberOfContextRecords);
    //
    ItemAttrContextList();
    //
    ItemAttrContext* BeginContext(POSTYPE equip_position);
    bool CommitContext();
    bool RollbackContext();
    // to support single context updates
    bool RecycleContexts(); // [1, 2] operations per item
    //
    size_t number_of_contexts;
    size_t number_of_total_records;
    ItemAttrContext* working_context;
    bool calculated_total_stats;
    ItemAttrStats total_stats;
    ItemAttrContext contexts[kMaxNumberOfContexts];
    Attributes::OperationRecord total_records[kNumberOfTotalRecords];
};

//==================================================================================================
// ItemAttrCalculator internal mode control
enum eItemAttrCalculatorInterControl
{
    eInterControl_Default,
    eInterControl_LimitsTestOnly,
};

struct ItemAttrCalculatorInterControlAutoVariable
{
    ItemAttrCalculatorInterControlAutoVariable(int* ref, eItemAttrCalculatorInterControl mode);
    ~ItemAttrCalculatorInterControlAutoVariable();
    int* const ref_field;
    int backup_field;
private:
    void operator=(const ItemAttrCalculatorInterControlAutoVariable&);
};

enum eItemAttrCalculatorInterControlHint
{
    eInterControlHint_NoOperation,
    eInterControlHint_EquipOne,
    eInterControlHint_TotalEquips,
    eInterControlHint_Unequip,
    eInterControlHint_ChangedToActivates,
    eInterControlHint_LimitsTestOnly,
};

//==================================================================================================

struct EquipSlotNodeBuffer
{
    ItemAttrCalculator::EquipSlotNode* GetInstance() {
        return reinterpret_cast<ItemAttrCalculator::EquipSlotNode*>(buffer);
    }
    char buffer[sizeof(ItemAttrCalculator::EquipSlotNode)];
};

//==================================================================================================
struct ItemEquipCounter
{
    enum { eItemCode, eItemType, eEquipGroup, eEquipEvent, eACGroup };
    union KeyType
    {
        typedef EquipSlotsManageInfo::ItemEquipCounterKeyType_t key_type;
        key_type composed;
        struct {
            SLOTCODE item_code;
            uint8_t __padding;
            uint8_t tag;
        } code_info;
        struct {
            uint16_t item_type; //eITEMTYPE
            uint8_t __padding;
            uint8_t tag;
        } type_info;
        struct {
            uint16_t group; //eITEMTYPE
            uint8_t __padding;
            uint8_t tag;
        } group_info;
        struct {
            uint8_t event_code;
            uint8_t __padding[2];
            uint8_t tag;
        } event_info;
        struct {
            WORD ac_group;
            uint8_t __padding;
            uint8_t tag;
        } ac_group_info;
    };
    KeyType key_value;
};
BOOST_STATIC_ASSERT(sizeof(ItemEquipCounter::KeyType) == sizeof(ulong));

//==================================================================================================

struct EquipSlotsManageInfoInterController : public EquipSlotsManageInfo
{
    void IncreaseEquipInfoSimulation(const SCItemSlot& item_slot);
    void DecreaseEquipInfoSimulation(const SCItemSlot& item_slot);
#if SUN_CODE_BACKUP
    // NOTE: f110424.2L, this mediator that additionally support the IEquipmentRestriction object
    // process item duplication dependency rule conditions
    // with CheckEquipDefaultValidationWithoutStats calling in the object
    bool CheckEquipDefaultValidationWithoutStats(
        IEquipmentRestriction* restriction,
        POSTYPE at_pos, const SCItemSlot& IN check_slot);
#endif
};

//==================================================================================================

struct SocketSearchBoard
{
    typedef ItemAttrCalculator::SocketOptionCalculator::CalculationResult node_type;
    struct RegistNode {
        node_type* link_node;
        const ZardOptionInfo* option_info;
    };
    //
    void Clear();
    static bool SearchSocketOptionPoint(
        const SCItemSlot& item_slot, SocketSearchBoard* search_board);
    //
    int number_of_registered;
    RegistNode registered_nodes[eATTR_MAX];
    node_type nodes[eATTR_MAX];
};

//==================================================================================================
// NOTE: f110111.2L, magical defense values are used to increase it of all elements
// in the Episode II.
struct InterAttackPowerCalculator
{
    void SetDefault(DWORD min_power, DWORD max_power);
    void AddWeightValue(DWORD weight_value);
    void AddRatioWeight_Limited(float ratio);
    // __NA_001062_20080623_STRENGTH_PENALTY
    void AddRatioWeight_Strength(float ratio);
    void AddRatioWeight_Fate(float ratio);

    DWORD sum_of_min_power;
    DWORD sum_of_max_power;
};

inline void InterAttackPowerCalculator::SetDefault(DWORD min_power, DWORD max_power)
{
    sum_of_min_power = min_power;
    sum_of_max_power = max_power;
};

inline void InterAttackPowerCalculator::AddWeightValue(DWORD weight_value)
{
    sum_of_min_power += weight_value;
    sum_of_max_power += weight_value;
};

//==================================================================================================
// NOTE: f110111.2L, magical defense values are used to increase it of all elements
// in the Episode II.
struct InterDefensePowerCalculator
{
    void SetDefault(DWORD defense_power);
    void AddWeightValue(DWORD weight_value);
    void AddRatioWeight_Limited(float ratio);
    // __NA_001062_20080623_STRENGTH_PENALTY
    void AddRatioWeight_Strength(float ratio);
    void AddRatioWeight_Fate(float ratio);
    //
    DWORD sum_of_defense_power;
};

inline void InterDefensePowerCalculator::SetDefault(DWORD defense_power)
{
    sum_of_defense_power = defense_power;
};

inline void InterDefensePowerCalculator::AddWeightValue(DWORD weight_value)
{
    sum_of_defense_power += weight_value;
};

//==================================================================================================
// temporary buffer for calculating
struct EquipWorkingSlots
{
    struct Node
    {
        bool equipped;
        // CHANGES: f110906.2L, f110905.3L, prevent the special accessory
        // overlapped attribute calcuation problem when the full option event is occurred.
        bool applied_special_fullset_option;
        const SCItemSlot* item_slot;
        ItemAttrCalculator::EquipSlotNode* info_node;
        ns_formula::EquipSlotsManageInfo::Node* work_node;
    };
    Node slots[MAX_EQUIPMENT_SLOT_NUM];
};

//==================================================================================================

}; //end of namespace

//==================================================================================================
//==================================================================================================
//==================================================================================================
#endif //_NA_002289_20110322_EQUIPMENTS_CALCULATOR_CHANGES_FOR_IMPROVEMENT