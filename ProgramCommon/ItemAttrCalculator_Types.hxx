#pragma once
#if !defined(PROGRAMCOMMON_ITEM_ATTR_CALCULATOR_EP2_TYPES)
    #error "can't direct include this file"
#endif
#if defined(_NA_002289_20110322_EQUIPMENTS_CALCULATOR_CHANGES_FOR_IMPROVEMENT)
//==================================================================================================
#include "SCSlotStruct.h"
#include "SetItemOptionInfoParser.h"
#include "PlayerAttributes.h"

class Attributes;
class PlayerAttributes;
class SCSlot;
class SCItemSlot;
class SCSlotContainer;
class SCItemSlotContainer;
struct BASE_ITEMINFO;
class SocketOptionAttributes;

//==================================================================================================
namespace ns_formula {
;

struct ItemAttrStats;
struct ItemLimitations;
struct ItemAttrContext;
struct EquipSlotsManageInfoInterController;
struct EquipWorkingSlots;

//==================================================================================================
// NOTE: f110530.4L, ItemSocketOptionSameAttrList specification (grid mapping)
// ---------------------------------------------------------------------------
// sum_of_points = 'WEAPON + ARMOR + ... + SHIRTS' in the same attribute type.
// attr_type#nth = the orders are expressed in descending sum_of_points order.
// ---------------------------------------------------------------------------
//                              check 'NULL'                 check 'NULL'
// ItemOptionNode* = item_socket_option_lines_sorted[row]->options[equip_pos];
// ---------------------------------------------------------------------------
//               | SumOfLine(sort)  | Weapon | Armor | ... | Shirt |
// attr_type#1st |  sum_of_points   |   *    |   *   | ... |   *   |
// attr_type#2nd |  sum_of_points   |   *    |   *   | ... |   *   |
//       .       |        .         |   *    |   *   | ... |   *   |
#if !defined(_SERVER) && 0
    #define USE_CLIENT_SOCKET_OPTION_TABLE  (1)
#else
    #define USE_CLIENT_SOCKET_OPTION_TABLE  (0)
#endif
//==================================================================================================

struct EquipSlotsManageInfo
{
    typedef ulong ItemEquipCounterKeyType_t;
    //
    struct Node
    {
        POSTYPE position;
        bool activated;
        DURATYPE duration;
    };
    //
    struct ItemSocketOption
    {
        typedef BYTE ItemOptionCode;
        typedef int OptionPoint;
        typedef int OptionValue;
            
        eATTR_TYPE attr_option_code;
        ItemOptionCode item_option_code;
        OptionPoint option_point;
        eVALUE_TYPE option_type;
        OptionValue option_value;

        static const ItemSocketOption kDefault;
    };
    //
    static const size_t kNumberOfItemSocketOptionLineColumns = eEQUIPCONTAINER_SHIRTS + 1;
    struct ItemSocketOptionLine
    {
        eATTR_TYPE attr_type;
        int sum_of_points; // <- 'WEAPON + ARMOR + ... + SHIRTS' in the same attribute type.
        const ItemSocketOption* options[kNumberOfItemSocketOptionLineColumns];
    };
    //
    struct ItemScoreInfo
    {
        typedef float Weight;

        const Node* slot_nodes;
        Weight socket_score; // 소켓 평가 점수
        Weight set_option_score; // 세트 옵션 평가 점수
        Weight total_score; // 장비 전체 평가 점수
        Weight each_item_score[MAX_EQUIPMENT_SLOT_NUM]; // 개별 아이템 평가 점수
        
        ItemScoreInfo();
        void Clear();
        int GetScore(const POSTYPE equip_position) const;
        int GetTotalScore() const;
        void UpdateTotalScore();

    private:
        __DISABLE_COPY(ItemScoreInfo);
    };
    //
    struct CounterNode
    {
        CounterNode() : number_of_contains(0), post_apply_working_value(0) {}
        int8_t number_of_contains;
        int8_t post_apply_working_value; // this is only used by internal controller
    };
    // NOTE: f110530.3L, this types are used for the client socket option presentation.
    typedef STLX_VECTOR<ItemSocketOption> ItemSocketOptionList;
    typedef STLX_VECTOR<ItemSocketOptionLine> ItemSocketOptionLineList;
    typedef STLX_VECTOR<const ItemSocketOptionLine*> ItemSocketOptionLineListSorted;
    //
    typedef STLX_MAP<ItemEquipCounterKeyType_t, CounterNode> ItemEquipCounterTable;
    //----------------------------------------------------------------------------------------------
    //----------------------------------------------------------------------------------------------
    EquipSlotsManageInfo();
    ~EquipSlotsManageInfo();
    //
    void ClearAll();
    void ClearCacheInfo();
    void ClearEquipCounterInfo();
    //
    bool IsEquipEventItem(BYTE event_code) const;
    // 장착 개수 제한 관리
    int GetEquipCount(SLOTCODE item_code) const;
    int GetEquipTypeCount(eITEMTYPE item_type) const;
    //그룹별 장착 개수 제한 관리
    // (NOTE) it has not the usage case at this point on time (100505)
    int GetEquipGroupCount(BYTE equip_group) const;
#ifdef _NA_001990_ACSYSTEM_ADD
    int GetEquipACGroupCount(WORD ac_group) const;
#endif //
    // CHANGES: f110422.5L, added filter to check the item relational dependency rules
    bool CanEquipmentsDependency(const SCItemSlot& item_slot) const;
    //
    void IncreaseEquipInfo(const SCItemSlot& item_slot);
    void DecreaseEquipInfo(const SCItemSlot& item_slot);
#if USE_CLIENT_SOCKET_OPTION_TABLE
    // NOTE: f110530.3L
    bool CalculateSocketOptionMappingTable();
#endif
    //----------------------------------------------------------------------------------------------
    // data fields...
    ItemEquipCounterTable restrict_table_;
    Node slots[MAX_EQUIPMENT_SLOT_NUM];
#if USE_CLIENT_SOCKET_OPTION_TABLE
    ItemSocketOptionLineListSorted item_socket_option_lines_sorted;
    ItemSocketOptionLineList item_socket_option_lines;
    ItemSocketOptionList item_socket_option_list[kNumberOfItemSocketOptionLineColumns];
#endif
#ifdef _NA_002935_20110704_ITEM_SCORE
    ItemScoreInfo item_score_info;
#endif
    //
    __DISABLE_COPY(EquipSlotsManageInfo);
};

}; //end of namespace

//==================================================================================================

struct IEquipmentRestriction
{
    virtual SCItemSlotContainer* GetEquipmentSlotContainer() = 0;
    virtual ns_formula::EquipSlotsManageInfo* GetEquipmentManageInfo() = 0;
    // check contents level enabled equip condition (no check stats+level)
    virtual bool CheckEquipDefaultValidationWithoutStats(
        POSTYPE at_pos, const SCItemSlot& IN check_slot) = 0;
};

//==================================================================================================

struct InvenItemOptionCalculator
{
    static bool ApplyInvenItemOption(PlayerAttributes& attributes,
                                     const SCItemSlot& item_slot, int changed_num,
                                     bool pc_room_status, bool use_update_ex);
};

//==================================================================================================
// inlining methods
//



//==================================================================================================
//==================================================================================================
//==================================================================================================

#endif //_NA_002289_20110322_EQUIPMENTS_CALCULATOR_CHANGES_FOR_IMPROVEMENT
