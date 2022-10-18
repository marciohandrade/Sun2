#ifndef __PROGRAMCOMMON_ITEM_ATTR_CALCULATOR_H
#define __PROGRAMCOMMON_ITEM_ATTR_CALCULATOR_H
#pragma once

#if defined(_NA_002289_20110322_EQUIPMENTS_CALCULATOR_CHANGES_FOR_IMPROVEMENT)
#include "./ItemAttrCalculatorEP2.h"
#else

//==================================================================================================
#include <SetItemOptionInfoParser.h>

class Attributes;
class PlayerAttributes;
class SCSlot;
class SCItemSlot;
class SCSlotContainer;
struct BASE_ITEMINFO;
class SocketOptionAttributes;

//==================================================================================================
//  <ItemAttrCalculator>
//      Item�� �����ؼ� ��ȭ�ϴ� Attr�� Attributes�� update�Ѵ�.
class ItemAttrCalculator
{
public:
    static const int kVersion = 1;
    //
    ItemAttrCalculator(PlayerAttributes& attributes,
                       SCSlotContainer& equip_container,
                       bool auto_update = true);
    virtual ~ItemAttrCalculator();

    void Clear();
    void UpdateAll(BOOL is_pc_room); // ������ ������ ���

    // Ư�� �����ۿ� ���ؼ� ���
    void Equip(const SCSlot& IN rSlot, BOOL is_pc_room, BOOL use_update_ex, BOOL& OUT dummy_ref);
    void UnEquip(const SCSlot* pSlot, BOOL is_pc_room, BOOL use_update_ex, BOOL& OUT dummy_ref);

    // (f100812.2L) add an interface to support external module can access add rule filter
    static void ModifyAttrKindAndCalculatedValue(uint8_t value_type, //eVALUE_TYPE
                                                 uint8_t* attr_kind, //eATTR_KIND
                                                 int * value);
    //
    void MinusOptionAttribute(eATTR_TYPE attr_type, BYTE ValueType, int Value);
    void PlusOptionAttribute(eATTR_TYPE attr_type, BYTE ValueType, int Value);

    void SetCalcTypeSetItemCount(BOOL bCalcTypeSetItemCount);
private:
    //��Ʈ ������ �ɼ��� �����Ų��.
    void ApplySetItemOption(const SCSlot* slot, bool is_equip);
    void ApplySetItemOption(BYTE bySetCode, POSTYPE pos, eSETITEM_OPTION_LEVEL Level, bool is_equip);
    void ApplySetItemFullOption(BYTE bySetCode, bool is_equip);
    BYTE GetSetItemChangeType(BYTE number_of_items, bool is_equip);
    void ApplySpecialSetItemOption(BYTE bySetCode, bool is_equip);

public:
    // true : �������, false : �κ��丮�� ������� ����
    void SetCalcOption(BOOL bIsEquipContainerCalc = true);
    bool IsEquipCalc() const;
private:
    bool _CalculateRoot(const SCItemSlot& IN item_slot, const BASE_ITEMINFO* IN item_info,
                        bool add_op, BOOL is_pc_room, BOOL use_update_ex);
    bool _CheckCalcAttr(const SCItemSlot& IN item_slot, const BASE_ITEMINFO* IN item_info); // STEP 1
    bool _CalcGenericAttr1st(const SCItemSlot& IN item_slot, const BASE_ITEMINFO* IN item_info, bool add_op);
    void _ForeachAttrByRank(const SCItemSlot& IN item_slot, bool is_add);
    void _ForeachAttrBySocket(const SCItemSlot& IN item_slot, bool is_add);
    //
    //void _ForeachAddAttrByRank(const SCItemSlot& IN item_slot);
    //void _ForeachAddAttrBySocket(const SCItemSlot& IN item_slot);
    bool _CalcAttrWeapon(const SCItemSlot& IN item_slot, const BASE_ITEMINFO* IN item_info, bool is_add);
    bool _CalcAttrArmor(const SCItemSlot& IN item_slot, const BASE_ITEMINFO* IN item_info, bool is_add);
    bool _CalcAttrAccessary(const SCItemSlot& IN item_slot, const BASE_ITEMINFO* IN item_info, bool add_op);
    bool _CalcGenericAttr2nd(const SCItemSlot& IN item_slot, const BASE_ITEMINFO* IN item_info,
                             bool is_add, BOOL* is_pc_room); // STEP 4 (last)

    //bool _CalcGenericAddAttr1st(const SCItemSlot& IN item_slot, const BASE_ITEMINFO* IN item_info); // STEP 2
    //bool _CalcAddAttrWeapon(const SCItemSlot& IN item_slot, const BASE_ITEMINFO* IN item_info); // STEP 3 CASE 1
    //bool _CalcAddAttrArmor(const SCItemSlot& IN item_slot, const BASE_ITEMINFO* IN item_info); // STEP 3 CASE 2
    //bool _CalcAddAttrAccessary(const SCItemSlot& IN item_slot, const BASE_ITEMINFO* IN item_info); // STEP 3 CASE 3
    //bool _CalcGenericAddAttr2nd(const SCItemSlot& IN item_slot, const BASE_ITEMINFO* IN item_info,
    //                            BOOL* pbPCBang, BOOL* pbIsSetItemFull); // STEP 4 (last)

    //void _ForeachSubAttrByRank(const SCItemSlot& IN item_slot);
    //void _ForeachSubAttrBySocket(const SCItemSlot& IN item_slot);
    //bool _CalcGenericSubAttr1st(const SCItemSlot& IN item_slot, const BASE_ITEMINFO* IN item_info); // STEP 2
    //bool _CalcSubAttrWeapon(const SCItemSlot& IN item_slot, const BASE_ITEMINFO* IN item_info); // STEP 3 CASE 1
    //bool _CalcSubAttrArmor(const SCItemSlot& IN item_slot, const BASE_ITEMINFO* IN item_info); // STEP 3 CASE 2
    //bool _CalcSubAttrAccessary(const SCItemSlot& IN item_slot, const BASE_ITEMINFO* IN item_info); // STEP 3 CASE 3
    //bool _CalcGenericSubAttr2nd(const SCItemSlot& IN item_slot, const BASE_ITEMINFO* IN item_info,
    //                            BOOL* pbPCBang, BOOL* pbIsSetItemFull); // STEP 4 (last)

protected:
    Attributes& attributes_; 
    SCSlotContainer& slot_container_;
    bool auto_update_;
    bool equip_container_calculation_;
    bool set_item_count_calculation_type_; //0 : �����̳ʷ� ������ ���, 1: ������ ������ ���� ������ ���� ������� ���
    typedef bool  REAL_EQUIPSLOTS[MAX_EQUIPMENT_SLOT_NUM];
    REAL_EQUIPSLOTS& real_equip_slots_;

    __DISABLE_COPY(ItemAttrCalculator);

#ifdef _NA_000251_20100727_SOCKET_SYSTEM
public:
    // ���� �ɼ� ����
    class SocketOptionCalculator
    {
    public:
        // �ɼ� ��� ���
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

            CalculationResult() : 
                attr_option_code(eATTR_TYPE_INVALID), 
                item_option_code(eATTR_TYPE_INVALID), 
                option_point(0), 
                option_type(VALUE_TYPE_VALUE), 
                option_value(0)
            {
            }
        };
        typedef STLX_HASH_MAP<eATTR_TYPE, CalculationResult> CalculationResultMap;
    public:
        SocketOptionCalculator(SocketOptionAttributes& socket_option_attributes);

        // �ʱ�ȭ�Ѵ�.
        void Clear();
        
        // ���� �ɼ��� ����Ѵ�.
        void Calculate(
            const SCItemSlot& item_slot, const bool is_add, ItemAttrCalculator& OUT item_calculator);

        // �������� ���� �ɼ� ����Ʈ�� ����Ѵ�.
        static bool CalculateItemOptionPoint(
            const SCItemSlot& item_slot, CalculationResultMap& OUT item_calc_result_map);
    private:
        // �������� ���� �ɼ��� ����Ʈ ������� ������ ���� �ɼǰ��� ����Ѵ�.
        bool CalculateItemOptionValue(
            const bool is_add, 
            const CalculationResult& item_calc_result, 
            ItemAttrCalculator& OUT item_calculator);
    private:
        SocketOptionAttributes& socket_option_attributes_;
        __DISABLE_COPY(SocketOptionCalculator);
    };
private:
    SocketOptionCalculator socket_option_calculator_; // ���� �ɼ� ����
#endif // _NA_000251_20100727_SOCKET_SYSTEM
};


inline void
ItemAttrCalculator::SetCalcTypeSetItemCount(BOOL bCalcTypeSetItemCount) {
    set_item_count_calculation_type_ = bCalcTypeSetItemCount != false;
}

// true : �������, false : �κ��丮�� ������� ����
inline void
ItemAttrCalculator::SetCalcOption(BOOL bIsEquipContainerCalc) {
    equip_container_calculation_ = bIsEquipContainerCalc != false;
}

inline bool
ItemAttrCalculator::IsEquipCalc() const {
    return equip_container_calculation_ != false;
}
#endif //if !defined(_NA_002289_20110322_EQUIPMENTS_CALCULATOR_CHANGES_FOR_IMPROVEMENT)
#endif //__PROGRAMCOMMON_ITEM_ATTR_CALCULATOR_H
