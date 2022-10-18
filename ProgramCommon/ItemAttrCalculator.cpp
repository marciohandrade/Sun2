#include "stdafx.h"
#include "./ItemAttrCalculator.h"

#if defined(_NA_002289_20110322_EQUIPMENTS_CALCULATOR_CHANGES_FOR_IMPROVEMENT)

__EMPTY_FILE_TAG();

#else //if !defined(_NA_002289_20110322_EQUIPMENTS_CALCULATOR_CHANGES_FOR_IMPROVEMENT)
#include <Const.h>
#include <Struct.h>

#include <Attribute.h>
#include <PlayerAttributes.h>
#include <ItemInfoParser.h>
#include <StrengthPenaltyParser.h>
//
#include <ItemFormula.h>
//
#include <ItemOptionHeader.h>
#include <SCSlotContainer.h>
#include <SCItemSlot.h>
#include <SCItemSlotContainer.h>
//

//==================================================================================================
// @history
//  - ~~~
//  - 09/10/24|waverix|code arrangement, remove warnings

// (NOTE) for set item operation, real equip position marking, for safely boundary
static bool sRealEquipSlotSafeArray[256];

//==================================================================================================
//
ItemAttrCalculator::ItemAttrCalculator(PlayerAttributes& attributes,
                                       SCSlotContainer& equip_container,
                                       bool auto_update)
    : attributes_((Attributes&)attributes), slot_container_(equip_container), auto_update_(auto_update)
    , equip_container_calculation_(true)
    , set_item_count_calculation_type_(false)
    , real_equip_slots_((REAL_EQUIPSLOTS&)sRealEquipSlotSafeArray)
#ifdef _NA_000251_20100727_SOCKET_SYSTEM
    , socket_option_calculator_(attributes.GetSocketOptionAttributes())
#endif // _NA_000251_20100727_SOCKET_SYSTEM
{}

ItemAttrCalculator::~ItemAttrCalculator()
{
}

//==================================================================================================

// STEP 1
inline bool
ItemAttrCalculator::_CheckCalcAttr(const SCItemSlot& IN item_slot, const BASE_ITEMINFO* IN item_info)
{
    __UNUSED(item_info);
    //if (item_slot.IsBlocked()) return;

#ifndef  __NA_001062_20080623_STRENGTH_PENALTY
    // 내구도가 있는지 체크
    if (item_slot.WithDura()) {
        if (!item_slot.GetDura()) {
            return false;
        }
    }
#endif

    // 기간이 만료되면 수치를 적용안한다.
    if (item_slot.IsExpiredTime()) {
        return false;
    }
    if (item_slot.IsExpiredWaitTime()) {
        return false;
    }
    return true;
}

//==================================================================================================

void ItemAttrCalculator::_ForeachAttrByRank(const SCItemSlot& IN item_slot, bool is_add)
{
    const eRANK rank_upperbound = static_cast<eRANK>(item_slot.GetRank() + 1);
    for (eRANK rank = RANK_D; rank < rank_upperbound; rank = static_cast<eRANK>(rank + 1))
    {
        const sRANK_OPTION* rank_option = item_slot.GetRankAttrDesc(rank);
        if (rank_option == NULL) {
            continue;
        };
        eATTR_TYPE rank_attr = item_slot.GetRankAttr(rank);
        BYTE value_type = static_cast<BYTE>(rank_option->m_NumericType);
        int value = item_slot.GetRankAttrValue(rank);
        if (is_add) {
            PlusOptionAttribute(rank_attr, value_type, value);
        }
        else {
            MinusOptionAttribute(rank_attr, value_type, value);
        }
    }
}

void ItemAttrCalculator::_ForeachAttrBySocket(const SCItemSlot& IN item_slot, bool is_add)
{
#ifdef _NA_000251_20100727_SOCKET_SYSTEM
    socket_option_calculator_.Calculate(item_slot, is_add, *this);
#else
    const int upperbound = item_slot.GetSocketNum();
    for (eSOCKET sock = SOCKET_1; sock < upperbound; sock = static_cast<eSOCKET>(sock + 1))
    {
        const sSOCKET_OPTION* sock_option = item_slot.GetSocketAttrDesc(sock);
        if (sock_option == NULL) {
            continue;
        }
        eATTR_TYPE rank_attr = item_slot.GetSocketAttr(sock);
        BYTE value_type = static_cast<BYTE>(sock_option->m_NumericType);
        int value = item_slot.GetSocketAttrValue(sock);

        if (is_add) {
            PlusOptionAttribute(rank_attr, value_type, value);
        }
        else {
            MinusOptionAttribute(rank_attr, value_type, value);
        }
    }
#endif // _NA_000251_20100727_SOCKET_SYSTEM
}

#if SUN_CODE_BACKUP
inline void
ItemAttrCalculator::_ForeachAddAttrByRank(const SCItemSlot& IN item_slot)
{
    //랭크
    //BYTE Index = 0, Value = 0;
    const BYTE upper = item_slot.GetRank() + 1;
    for (BYTE rank = (BYTE)RANK_D; rank < upper; ++rank)
    {
        if (item_slot.GetRankAttrDesc((eRANK)rank)) {
            PlusOptionAttribute(item_slot.GetRankAttr((eRANK)rank),
                                (BYTE)item_slot.GetRankAttrDesc((eRANK)rank)->m_NumericType,
                                item_slot.GetRankAttrValue((eRANK)rank));
        }
    }
}
inline void
ItemAttrCalculator::_ForeachAddAttrBySocket(const SCItemSlot& IN item_slot)
{
    // 소켓 옵션
    const int upper = item_slot.GetSocketNum();
    for (int sock = (int)SOCKET_1; sock < upper; ++sock)
    {
        if (item_slot.GetSocketAttrDesc((eSOCKET)sock)) {
            PlusOptionAttribute(item_slot.GetSocketAttr((eSOCKET)sock),
                                item_slot.GetSocketAttrDesc((eSOCKET)sock)->m_NumericType,
                                item_slot.GetSocketAttrValue((eSOCKET)sock));
        }
    }
};

inline void
ItemAttrCalculator::_ForeachSubAttrByRank(const SCItemSlot& IN item_slot)
{
    //랭크
    //BYTE Index = 0, Value = 0;
    const BYTE upper = item_slot.GetRank() + 1;
    for (BYTE rank = (BYTE)RANK_D; rank < upper; ++rank)
    {
        if (item_slot.GetRankAttrDesc((eRANK)rank)) {
            MinusOptionAttribute(item_slot.GetRankAttr((eRANK)rank),
                                 (BYTE)item_slot.GetRankAttrDesc((eRANK)rank)->m_NumericType,
                                 item_slot.GetRankAttrValue((eRANK)rank));
        }
    }
}

inline void
ItemAttrCalculator::_ForeachSubAttrBySocket(const SCItemSlot& IN item_slot)
{
    // 소켓 옵션
    const int upper = item_slot.GetSocketNum();
    for (int sock = (int)SOCKET_1; sock < upper; ++sock)
    {
        if (item_slot.GetSocketAttrDesc((eSOCKET)sock)) {
            MinusOptionAttribute(item_slot.GetSocketAttr((eSOCKET)sock),
                                 item_slot.GetSocketAttrDesc((eSOCKET)sock)->m_NumericType,
                                 item_slot.GetSocketAttrValue((eSOCKET)sock));
        }
    }
}
#endif //SUN_CODE_BACKUP

//==================================================================================================

// STEP 2
bool ItemAttrCalculator::_CalcGenericAttr1st(const SCItemSlot& IN item_slot,
                                             const BASE_ITEMINFO* IN item_info,
                                             bool add_op)
{
    __UNUSED(item_slot);
    // (f100721.1L)
    static const Attributes::Operation attr_item_array[] = {
        eATTR_PHYSICAL_ATTACK_SUCCESS_RATIO,
    #ifdef _NA001605_EP2_ATTRIBUTES_RENEWAL_
        // CHANGES: f110209.6L, changes formula by planner request
        eATTR_PHYSICAL_ATTACK_BLOCK_BASE_ARMOR_RATIO,
    #else
        eATTR_PHYSICAL_ATTACK_BLOCK_RATIO,
    #endif
        eATTR_ATTACK_SPEED,
        eATTR_MOVE_SPEED
    };
    //
    const int value_array[_countof(attr_item_array)] = {
        item_info->m_wPhyAttRate,
        item_info->m_wPhyAvoid,
        item_info->m_wPhyAttSpeed,
        item_info->m_wSpeed
    };
    const Attributes::Operation::eOper oper_type = add_op ?
        attr_item_array->eOper_Add : attr_item_array->eOper_Sub;
    //
    return attributes_.GroupOperate(oper_type, eATTR_KIND_ITEM,
                                    attr_item_array, value_array, _countof(attr_item_array));
}

#if SUN_CODE_BACKUP
// STEP 2
inline bool
ItemAttrCalculator::_CalcGenericAddAttr1st(const SCItemSlot& IN item_slot, const BASE_ITEMINFO* IN item_info)
{
    __UNUSED(item_slot);
    // 물리 공격 성공률, 회피율
    if (item_info->m_wPhyAttRate)
    attributes_[eATTR_PHYSICAL_ATTACK_SUCCESS_RATIO][eATTR_KIND_ITEM] += item_info->m_wPhyAttRate;
    if (item_info->m_wPhyAvoid)
    attributes_[eATTR_PHYSICAL_ATTACK_BLOCK_RATIO][eATTR_KIND_ITEM]   += item_info->m_wPhyAvoid;

    // 공격, 이동 속도
    if (item_info->m_wPhyAttSpeed)
    attributes_[eATTR_ATTACK_SPEED][eATTR_KIND_ITEM] += item_info->m_wPhyAttSpeed;
    if (item_info->m_wSpeed)
    attributes_[eATTR_MOVE_SPEED][eATTR_KIND_ITEM]   += item_info->m_wSpeed;

    return true;
}

// STEP 2
inline bool
ItemAttrCalculator::_CalcGenericSubAttr1st(const SCItemSlot& IN item_slot, const BASE_ITEMINFO* IN item_info)
{
    __UNUSED(item_slot);
    // 물리 공격 성공률, 회피율
    if (item_info->m_wPhyAttRate)
    attributes_[eATTR_PHYSICAL_ATTACK_SUCCESS_RATIO][eATTR_KIND_ITEM] -= item_info->m_wPhyAttRate;
    if (item_info->m_wPhyAvoid)
    attributes_[eATTR_PHYSICAL_ATTACK_BLOCK_RATIO][eATTR_KIND_ITEM]   -= item_info->m_wPhyAvoid;

    // 공격, 이동 속도
    if (item_info->m_wPhyAttSpeed)
    attributes_[eATTR_ATTACK_SPEED][eATTR_KIND_ITEM] -= item_info->m_wPhyAttSpeed;
    if (item_info->m_wSpeed)
    attributes_[eATTR_MOVE_SPEED][eATTR_KIND_ITEM]   -= item_info->m_wSpeed;

    return true;
}
#endif //SUN_CODE_BACKUP

//==================================================================================================
// NOTE: f110111.2L, magical defense values are used to increase it of all elements
// in the Episode II.
struct sInterDamageOp
{
    enum {
        LAST_MIN_DAMAGE_PHY     = 0,
        LAST_MAX_DAMAGE_PHY     ,
        LAST_MIN_DAMAGE_MAG     ,
        LAST_MAX_DAMAGE_MAG     ,
        MAX_DAMAGES             ,
    };

    sInterDamageOp() {
        ZeroMemory(this, sizeof(*this));
    }

    __forceinline void  SetWeaponValue(DWORD lastMinPhy, DWORD lastMaxPhy,
                                       DWORD lastMinMag, DWORD lastMaxMag)
    {
        LastPhyMinDamage = lastMinPhy;
        LastPhyMaxDamage = lastMaxPhy;
    #if !defined(_NA001605_EP2_ATTRIBUTES_RENEWAL_)
        LastMagicMinDamage = lastMinMag;
        LastMagicMaxDamage = lastMaxMag;
    #endif
    }

    __forceinline void  SetArmorValue(DWORD lastPhyDef, DWORD lastMagDef,
                                      WORD itemPhyDef, WORD itemMagDef)
    {
        LastPhyDef = lastPhyDef;
        ItemPhyDef = itemPhyDef;
    #if !defined(_NA001605_EP2_ATTRIBUTES_RENEWAL_)
        LastMagicDef = lastMagDef;
        ItemMagicDef = itemMagDef;
    #endif
    }

    __forceinline void  AddWeaponValue(DWORD value)
    {
        LastPhyMinDamage += value;
        LastPhyMaxDamage += value;
    #if !defined(_NA001605_EP2_ATTRIBUTES_RENEWAL_)
        LastMagicMinDamage += value;
        LastMagicMaxDamage += value;
    #endif
    }

    __forceinline void  AddWeaponValueByLimited(float ratio)
    {
        if (ratio >= 0)
        {
            LastPhyMinDamage += (int)((ratio / 100.0f) * (float)LastPhyMinDamage + 0.5f);
            LastPhyMaxDamage += (int)((ratio / 100.0f) * (float)LastPhyMaxDamage + 0.5f);
        #if !defined(_NA001605_EP2_ATTRIBUTES_RENEWAL_)
            LastMagicMinDamage += (int)((ratio / 100.0f) * (float)LastMagicMinDamage + 0.5f);
            LastMagicMaxDamage += (int)((ratio / 100.0f) * (float)LastMagicMaxDamage + 0.5f);
        #endif
        }
        else
        {
            LastPhyMinDamage += (int)((ratio / 100.0f) * (float)LastPhyMinDamage - 0.5f);
            LastPhyMaxDamage += (int)((ratio / 100.0f) * (float)LastPhyMaxDamage - 0.5f);
        #if !defined(_NA001605_EP2_ATTRIBUTES_RENEWAL_)
            LastMagicMinDamage += (int)((ratio / 100.0f) * (float)LastMagicMinDamage - 0.5f);
            LastMagicMaxDamage += (int)((ratio / 100.0f) * (float)LastMagicMaxDamage - 0.5f);
        #endif
        }
    }

    __forceinline void  AddArmorValueByLimited(float ratio)
    {
        if (ratio >= 0)
        {
            ItemPhyDef += (WORD)(int)((ratio / 100.0f) * (float)ItemPhyDef + 0.5f);
        #ifdef _NA001605_EP2_ATTRIBUTES_RENEWAL_
        #else
            ItemMagicDef += (WORD)(int)((ratio / 100.0f) * (float)ItemMagicDef + 0.5f);
        #endif
        }
        else
        {
            ItemPhyDef += (WORD)(int)((ratio / 100.0f) * (float)ItemPhyDef - 0.5f);
        #ifdef _NA001605_EP2_ATTRIBUTES_RENEWAL_
        #else
            ItemMagicDef += (WORD)(int)((ratio / 100.0f) * (float)ItemMagicDef - 0.5f);
        #endif
        }
    }

#ifdef __NA_001062_20080623_STRENGTH_PENALTY
    __forceinline void  AddWeaponValueByStrength(float ratio)
    {
        if (ratio >= 0)
        {
            LastPhyMinDamage    = (WORD)((LastPhyMinDamage * ratio) + 0.5f);
            LastPhyMaxDamage    = (WORD)((LastPhyMaxDamage * ratio) + 0.5f);
        #if !defined(_NA001605_EP2_ATTRIBUTES_RENEWAL_)
            LastMagicMinDamage  = (WORD)((LastMagicMinDamage * ratio) + 0.5f);
            LastMagicMaxDamage  = (WORD)((LastMagicMaxDamage * ratio) + 0.5f);
        #endif
        }
        else
        {
            LastPhyMinDamage    = (WORD)((LastPhyMinDamage * ratio) - 0.5f);
            LastPhyMaxDamage    = (WORD)((LastPhyMaxDamage * ratio) - 0.5f);
        #if !defined(_NA001605_EP2_ATTRIBUTES_RENEWAL_)
            LastMagicMinDamage  = (WORD)((LastMagicMinDamage * ratio) - 0.5f);
            LastMagicMaxDamage  = (WORD)((LastMagicMaxDamage * ratio) - 0.5f);
        #endif
        }
    }
    __forceinline void  AddArmorValueByStrength(float ratio)
    {
        if (ratio >= 0)
        {
            LastPhyDef      = (WORD)((LastPhyDef * ratio) + 0.5f);
        #ifdef _NA001605_EP2_ATTRIBUTES_RENEWAL_
        #else
            LastMagicDef    = (WORD)((LastMagicDef * ratio) + 0.5f);
        #endif
        }
        else
        {
            LastPhyDef      = (WORD)((LastPhyDef * ratio) - 0.5f);
        #ifdef _NA001605_EP2_ATTRIBUTES_RENEWAL_
        #else
            LastMagicDef    = (WORD)((LastMagicDef * ratio) - 0.5f);
        #endif
        }
    }
#endif //__NA_001062_20080623_STRENGTH_PENALTY

    __forceinline void  AddWeaponValueByFate(float ratio)
    {
        if (ratio >= 0)
        {
            LastPhyMinDamage    = (WORD)(LastPhyMinDamage * (ratio / 100.0f) + 0.5f);
            LastPhyMaxDamage    = (WORD)(LastPhyMaxDamage * (ratio / 100.0f) + 0.5f);
        #if !defined(_NA001605_EP2_ATTRIBUTES_RENEWAL_)
            LastMagicMinDamage  = (WORD)(LastMagicMinDamage * (ratio / 100.0f) + 0.5f);
            LastMagicMaxDamage  = (WORD)(LastMagicMaxDamage * (ratio / 100.0f) + 0.5f);
        #endif
        }
        else
        {
            LastPhyMinDamage    = (WORD)(LastPhyMinDamage * (ratio / 100.0f) - 0.5f);
            LastPhyMaxDamage    = (WORD)(LastPhyMaxDamage * (ratio / 100.0f) - 0.5f);
        #if !defined(_NA001605_EP2_ATTRIBUTES_RENEWAL_)
            LastMagicMinDamage  = (WORD)(LastMagicMinDamage * (ratio / 100.0f) - 0.5f);
            LastMagicMaxDamage  = (WORD)(LastMagicMaxDamage * (ratio / 100.0f) - 0.5f);
        #endif
        }
    }
    __forceinline void  AddArmorValueByFate(float ratio)
    {
        if (ratio >= 0)
        {
            ItemPhyDef      = (WORD)(ItemPhyDef * (ratio / 100.0f) + 0.5f);
        #ifdef _NA001605_EP2_ATTRIBUTES_RENEWAL_
        #else
            ItemMagicDef    = (WORD)(ItemMagicDef * (ratio / 100.0f) + 0.5f);
        #endif
        }
        else
        {
            ItemPhyDef      = (WORD)(ItemPhyDef * (ratio / 100.0f) - 0.5f);
        #ifdef _NA001605_EP2_ATTRIBUTES_RENEWAL_
        #else
            ItemMagicDef    = (WORD)(ItemMagicDef * (ratio / 100.0f) - 0.5f);
        #endif
        }
    }
#pragma warning(push)
#pragma warning(disable:4201)
    union {
        struct {
            DWORD   LastPhyMinDamage;
            DWORD   LastPhyMaxDamage;
        #if !defined(_NA001605_EP2_ATTRIBUTES_RENEWAL_)
            DWORD   LastMagicMinDamage;
            DWORD   LastMagicMaxDamage;
        #endif
        };
        struct {
            DWORD   LastPhyDef; //reordering assume align 4
            WORD    ItemPhyDef;
        #if !defined(_NA001605_EP2_ATTRIBUTES_RENEWAL_)
            DWORD   LastMagicDef;
            WORD    ItemMagicDef;
        #endif
        };
    };
#pragma warning(pop)
};

// 무기 공격력
inline bool
ItemAttrCalculator::_CalcAttrWeapon(const SCItemSlot& IN item_slot,
                                    const BASE_ITEMINFO* IN item_info,
                                    bool is_add)
{
    sInterDamageOp op;

    DWORD WeightDamage = CalcAttackPower(item_slot.GetEnchant(), item_info->m_LV);

    if (item_slot.IsDivine())
    {
        op.SetWeaponValue(item_info->m_wDivPhyMinDamage,
                          item_info->m_wDivPhyMaxDamage,
                          item_info->m_wDivMagicMinDamage,
                          item_info->m_wDivMagicMaxDamage);
    }
    else if (item_info->IsElite())
    {
        op.SetWeaponValue(item_info->m_wElitePhyMinDamage,
                          item_info->m_wElitePhyMaxDamage,
                          item_info->m_wEliteMagicMinDamage,
                          item_info->m_wEliteMagicMaxDamage);
    }
    else if (item_info->IsUnique())
    {
        op.SetWeaponValue(item_info->m_wUniPhyMinDamage,
                          item_info->m_wUniPhyMaxDamage,
                          item_info->m_wUniMagicMinDamage,
                          item_info->m_wUniMagicMaxDamage);
    }
    else
    {
        op.SetWeaponValue(item_info->m_wMinDamage,
                          item_info->m_wMaxDamage,
                          item_info->m_wMagicAttMinDamgage,
                          item_info->m_wMagicAttMaxDamgage);
    }

    if (item_slot.IsLimited()) {
        op.AddWeaponValueByLimited((float)item_slot.GetDamageRatio());
    }
    else if (item_slot.IsFateItem()) {
        op.AddWeaponValueByFate((float)item_slot.GetFateAtkDefRatio());
    }

    op.AddWeaponValue(WeightDamage);

#ifdef __NA_001062_20080623_STRENGTH_PENALTY
    ;{
        const DWORD penalty_index = item_info->m_dwStrengthPenaltyIdx;
        if (penalty_index != 0)
        {
            const sSTRENGTH_PENALTY* strength_penalty = \
                StrengthPenaltyParser::Instance()->GetStrengthPenaltyInfo(\
                    penalty_index, item_slot.GetMaxDura(), item_slot.GetDura());
            //
            op.AddWeaponValueByStrength(strength_penalty->m_fStrengthPro);
        }
    };
#endif
    // (f100721.1L)
    static const Attributes::Operation attr_item_array[] = {
        eATTR_BASE_MELEE_MIN_ATTACK_POWER, eATTR_BASE_MELEE_MAX_ATTACK_POWER,
        eATTR_BASE_RANGE_MIN_ATTACK_POWER, eATTR_BASE_RANGE_MAX_ATTACK_POWER,
    #if !defined(_NA001605_EP2_ATTRIBUTES_RENEWAL_)
        eATTR_BASE_MAGICAL_MIN_ATTACK_POWER,
        eATTR_BASE_MAGICAL_MAX_ATTACK_POWER
    #endif
    };
    const int value_array[_countof(attr_item_array)] = {
        op.LastPhyMinDamage, op.LastPhyMaxDamage,
        op.LastPhyMinDamage, op.LastPhyMaxDamage,
    #if !defined(_NA001605_EP2_ATTRIBUTES_RENEWAL_)
        op.LastMagicMinDamage,
        op.LastMagicMaxDamage
    #endif
    };
    const Attributes::Operation::eOper oper_type = \
        is_add ? attr_item_array->eOper_Add : attr_item_array->eOper_Sub;
    attributes_.GroupOperate(oper_type, eATTR_KIND_ITEM,
                             attr_item_array, value_array, _countof(attr_item_array));
    //
    _ForeachAttrByRank(item_slot, is_add);
    _ForeachAttrBySocket(item_slot, is_add);
    //
    return true;
}

inline bool
ItemAttrCalculator::_CalcAttrArmor(const SCItemSlot& IN item_slot,
                                   const BASE_ITEMINFO* IN item_info,
                                   bool is_add)
{
    // Divine 아이템 처리! 방어력 상승된 값이 기본 PhyDef 이 된다.
    sInterDamageOp op;
    //
    if (item_slot.IsDivine()) {
        op.SetArmorValue(0, 0, item_info->m_wDivPhyDef, item_info->m_wDivMagicDef);
    }
    else if (item_info->IsElite()) {
        op.SetArmorValue(0, 0, item_info->m_wElitePhyDef, item_info->m_wEliteMagicDef);
    }
    else if (item_info->IsUnique()) {
        op.SetArmorValue(0, 0, item_info->m_wUniPhyDef, item_info->m_wUniMagicDef);
    }
    else {
        op.SetArmorValue(0, 0, item_info->m_wPhyDef, item_info->m_wMagicDef);
    };
    //
    if (item_slot.IsLimited()) {
        op.AddArmorValueByLimited((float)item_slot.GetDamageRatio());
    }
    else if (item_slot.IsFateItem()) {
        op.AddArmorValueByFate((float)item_slot.GetFateAtkDefRatio());
    }

    BYTE enchant = item_slot.GetEnchant();
    LEVELTYPE level = item_info->m_LV;
    op.LastPhyDef = CalcPhyDef(op.ItemPhyDef, enchant, level);
#if !defined(_NA001605_EP2_ATTRIBUTES_RENEWAL_)
    op.LastMagicDef = CalcPhyDef(op.ItemMagicDef, enchant, level);
#endif

#ifdef __NA_001062_20080623_STRENGTH_PENALTY
    ;{
        const DWORD penalty_index = item_info->m_dwStrengthPenaltyIdx;
        if (penalty_index != 0)
        {
            const sSTRENGTH_PENALTY* strength_penalty = \
                StrengthPenaltyParser::Instance()->GetStrengthPenaltyInfo(\
                    penalty_index, item_slot.GetMaxDura(), item_slot.GetDura());
            //
            op.AddArmorValueByStrength(strength_penalty->m_fStrengthPro);
        }
    };
#endif
    // (f100721.1L)
    static const Attributes::Operation attr_item_array[] = {
        eATTR_BASE_MELEE_DEFENSE_POWER,
        eATTR_BASE_RANGE_DEFENSE_POWER,
    #if !defined(_NA001605_EP2_ATTRIBUTES_RENEWAL_)
        eATTR_BASE_MAGICAL_DEFENSE_POWER,
    #endif
    };
    const int value_array[_countof(attr_item_array)] = {
        op.LastPhyDef, op.LastPhyDef
    #if !defined(_NA001605_EP2_ATTRIBUTES_RENEWAL_)
        , op.LastMagicDef
    #endif
    };
    const Attributes::Operation::eOper oper_type = \
        is_add ? attr_item_array->eOper_Add : attr_item_array->eOper_Sub;
    attributes_.GroupOperate(oper_type, eATTR_KIND_ITEM,
                             attr_item_array, value_array, _countof(attr_item_array));
    //
    _ForeachAttrByRank(item_slot, is_add);
    if (item_info->IsArmor()) {
        _ForeachAttrBySocket(item_slot, is_add);
    }

    return true;
}

bool ItemAttrCalculator::_CalcAttrAccessary(const SCItemSlot& IN item_slot,
                                            const BASE_ITEMINFO* IN item_info,
                                            bool add_op)
{
    __UNUSED(item_info);
    _ForeachAttrByRank(item_slot, add_op);
    return true;
}

#if SUN_CODE_BACKUP
// STEP 3 CASE 3
inline bool
ItemAttrCalculator::_CalcAddAttrAccessary(const SCItemSlot& IN item_slot, const BASE_ITEMINFO* IN item_info)
{
    __UNUSED(item_info);
    _ForeachAddAttrByRank(item_slot);

    return true;
}

// STEP 3 CASE 3
inline bool
ItemAttrCalculator::_CalcSubAttrAccessary(const SCItemSlot& IN item_slot, const BASE_ITEMINFO* IN item_info)
{
    __UNUSED(item_info);
    _ForeachSubAttrByRank(item_slot);

    return true;
}
#endif //SUN_CODE_BACKUP

#if SUN_CODE_BACKUP
// STEP 4 (last)
inline bool
ItemAttrCalculator::_CalcGenericAddAttr2nd(const SCItemSlot& IN item_slot, const BASE_ITEMINFO* IN item_info,
                                           BOOL* is_pc_room, BOOL* pbIsSetItemFull)
{
    return _CalcGenericAttr2nd(item_slot, item_info, true, is_pc_room, pbIsSetItemFull);
}

// STEP 4 (last)
inline bool
ItemAttrCalculator::_CalcGenericSubAttr2nd(const SCItemSlot& IN item_slot, const BASE_ITEMINFO* IN item_info,
                                           BOOL* is_pc_room, BOOL* pbIsSetItemFull)
{
    return _CalcGenericAttr2nd(item_slot, item_info, false, is_pc_room, pbIsSetItemFull);
}
#endif //SUN_CODE_BACKUP

bool
ItemAttrCalculator::_CalcGenericAttr2nd(const SCItemSlot& IN item_slot,
                                        const BASE_ITEMINFO* IN item_info,
                                        bool is_add, BOOL* is_pc_room)
{
    // (NOTE) extra-stone option
    // (CHANGES) (f100406.2L) (WAVERIX) add filter condition \
    // because of inventory effect item is not equip but can apply option
    const nsSlot::ItemRuleInfo& item_rule_info = item_slot.GetItemRuleInfo();
    if (item_rule_info.IsEnableExtraOption())
    {
        if (int n_option_attr_index = item_slot.GetNOptionAttrIndex())
        {
            const sEXTRASTONE_OPT_INFO* extra_option = item_slot.GetNOptionAttrDesc();
            if (extra_option)
            {
                const eATTR_TYPE attr_type = item_slot.GetNOptionAttr();
                const int attr_value = item_slot.GetNOptionAttrValue();
                if (is_add) {
                    PlusOptionAttribute(attr_type, extra_option->m_byValueType, attr_value);
                }
                else {
                    MinusOptionAttribute(attr_type, extra_option->m_byValueType, attr_value);
                }
            }
        }
    } //end extra option part

    // (NOTE) calculate item option by script
    // (CHANGES) (WAVERIX) changes to more simple code
    BOOST_STATIC_ASSERT(0 < BASE_ITEMINFO::_MAX_OPTION_NUM &&
                            BASE_ITEMINFO::_MAX_OPTION_NUM <= sizeof(DWORD64));
    typedef util::SEQ_BYTE_MASK<eITEM_EFFECT_ON_EQUIP | eITEM_EFFECT_ON_INVEN, \
                          BASE_ITEMINFO::_MAX_OPTION_NUM>   OPTION_EFFECT_MASK;

    int upperbound_selector = (*is_pc_room) ? 2 : 1; //Normal|PC option groups
    for (int selector = 0; selector < upperbound_selector; ++selector)
    {
        const BASE_ITEMINFO::OptionEffects* __option_effects = selector == 0 ?
            &item_info->m_byOptionExEffect : &item_info->m_byPCOptionExEffect;

        const BASE_ITEMINFO::OptionEffects& option_effects = *__option_effects;

        bool apply = ARR_CHECK_IS_CONTAIN(OPTION_EFFECT_MASK::MASK, option_effects);
        if (apply == false) {
            continue;
        }
        const BASE_ITEMINFO::OptionIndexes* __option_indexes = selector == 0 ?
            &item_info->m_wOptionIndex : &item_info->m_wPCOptionIndex;
        const BASE_ITEMINFO::OptionTypes* __option_types = selector == 0 ?
            &item_info->m_byOptionType : &item_info->m_byPCOptionType;
        const BASE_ITEMINFO::OptionValues* __option_values = selector == 0 ?
            &item_info->m_iOptionValue : &item_info->m_iPCOptionValue;
        //
        const BASE_ITEMINFO::OptionIndexes& option_indexes = *__option_indexes;
        const BASE_ITEMINFO::OptionTypes& option_types = *__option_types;
        const BASE_ITEMINFO::OptionValues& option_values = *__option_values;

        for (int op = 0; op < _countof(option_indexes); ++op)
        {
            bool is_valid_values = option_indexes[op] != 0 && 
                                   option_values[op] != 0 &&
                                   option_indexes[op] < _countof(g_ITEM_OPTION);
            if (is_valid_values == false) {
                continue;
            }
            const eATTR_TYPE attr_type = g_ITEM_OPTION[option_indexes[op]];
            if (is_add) {
                PlusOptionAttribute(attr_type, option_types[op], option_values[op]);
            }
            else {
                MinusOptionAttribute(attr_type, option_types[op], option_values[op]);
            }
        }
    }

    if (IsEquipCalc())
    {
        real_equip_slots_[item_slot.GetPos()] = is_add;
        ApplySetItemOption(&item_slot, is_add);
    }

    return true;
}

bool ItemAttrCalculator::_CalculateRoot(const SCItemSlot& IN item_slot,
                                        const BASE_ITEMINFO* IN item_info,
                                        bool add_op,
                                        BOOL is_pc_room, BOOL use_update_ex)
{
    if (!_CheckCalcAttr(item_slot, item_info)) {
        return false;
    }
    const nsSlot::ItemRuleInfo& item_rule_info = item_slot.GetItemRuleInfo();
    //
    _CalcGenericAttr1st(item_slot, item_info, add_op);

    if (false)
        ; // no operation
    else if (item_rule_info.is_weapon) {
        _CalcAttrWeapon(item_slot, item_info, add_op);
    }
    else if (item_rule_info.is_armor) {
        _CalcAttrArmor(item_slot, item_info, add_op);
    }
    else if (item_rule_info.is_accessory || item_rule_info.is_accessory_special) {
        _CalcAttrAccessary(item_slot, item_info, add_op);
    }

    _CalcGenericAttr2nd(item_slot, item_info, add_op, &is_pc_room);

    if (auto_update_)
    {
        if (use_update_ex) {
            attributes_.UpdateEx();
        }
        else {
            attributes_.Update();
        }
    }
    return true;
}

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//  <Equip>
void
ItemAttrCalculator::Equip(const SCSlot& IN slot,
                          BOOL is_pc_room,
                          BOOL use_update_ex,
                          BOOL& OUT dummy_ref)
{
    __UNUSED(dummy_ref);
    const SCItemSlot& item_slot = static_cast<const SCItemSlot&>(slot);
    const BASE_ITEMINFO* item_info = item_slot.GetItemInfo();
    //assert(item_info != 0);
    // (NOTE) Equipitem이면서 equip연산이 필요하지 않는 아이템(간판...)등이 존재한다.
    const nsSlot::ItemRuleInfo& item_rule_info = item_slot.GetItemRuleInfo();
    bool need_apply = item_rule_info.is_valid_item &&
                     (item_rule_info.is_weapon || item_rule_info.is_armor ||
                      item_rule_info.is_accessory || item_rule_info.is_accessory_special);
    if (need_apply == false)
    {
        // (CHANGES) (f100406.2L) (WAVERIX) fix up missing process on the inventory option effect \
        // item which is not equal equip item that need applied option
        typedef util::SEQ_BYTE_MASK<eITEM_EFFECT_ON_INVEN, BASE_ITEMINFO::_MAX_OPTION_NUM> \
            OPTION_EFFECT_INVEN_MASK;

        const bool is_inven_effect_item = \
            ARR_CHECK_IS_CONTAIN(OPTION_EFFECT_INVEN_MASK::MASK, item_info->m_byOptionExEffect) ||
            ARR_CHECK_IS_CONTAIN(OPTION_EFFECT_INVEN_MASK::MASK, item_info->m_byPCOptionExEffect);
        if (is_inven_effect_item == false) {
            return;
        }
    }
    // (f100721.1L)
    _CalculateRoot(item_slot, item_info, true, is_pc_room, use_update_ex);
    //
#if SUN_CODE_BACKUP
    if (!_CheckCalcAttr(item_slot, item_info))
        return;

    _CalcGenericAddAttr1st(item_slot, item_info);

    if (false)
        ; // no operation
    else if (item_rule_info.is_weapon)
        _CalcAddAttrWeapon(item_slot, item_info);
    else if (item_rule_info.is_armor)
        _CalcAddAttrArmor(item_slot, item_info);
    else if (item_rule_info.is_accessory || item_rule_info.is_accessory_special)
        _CalcAddAttrAccessary(item_slot, item_info);

    _CalcGenericAddAttr2nd(item_slot, item_info, &is_pc_room, &dummy_ref);

    if (auto_update_) {
        if (use_update_ex)
            attributes_.UpdateEx();
        else
            attributes_.Update();
    }
#endif //SUN_CODE_BACKUP
}

//==================================================================================================
//  <UnEquip>
//      비활성화 아이템은 Equip를 하지 않으므로 UnEquip을 호출하지 않도록 하자!!!
void
ItemAttrCalculator::UnEquip(const SCSlot* slot,
                            BOOL is_pc_room,
                            BOOL use_update_ex,
                            BOOL& OUT dummy_ref)
{
    __UNUSED(dummy_ref);
    if (slot == NULL) {
        return;
    }
    const SCItemSlot& item_slot = static_cast<const SCItemSlot&>(*slot);
    const BASE_ITEMINFO* item_info = item_slot.GetItemInfo();
    //assert(item_info != 0);
    // (NOTE) Equipitem이면서 equip연산이 필요하지 않는 아이템(간판...)등이 존재한다.
    const nsSlot::ItemRuleInfo& item_rule_info = item_slot.GetItemRuleInfo();
    bool need_apply = item_rule_info.is_valid_item &&
                     (item_rule_info.is_weapon || item_rule_info.is_armor ||
                      item_rule_info.is_accessory || item_rule_info.is_accessory_special);
    if (need_apply == false)
    {
        // (CHANGES) (f100520.4L) (WAVERIX) critical, add a missing logic to match an equip operation
        // (CHANGES) (f100406.2L) (WAVERIX) fix up missing process on the inventory option effect \
        // item which is not equal equip item that need applied option
        typedef util::SEQ_BYTE_MASK<eITEM_EFFECT_ON_INVEN, BASE_ITEMINFO::_MAX_OPTION_NUM> \
            OPTION_EFFECT_INVEN_MASK;

        const bool is_inven_effect_item =
            ARR_CHECK_IS_CONTAIN(OPTION_EFFECT_INVEN_MASK::MASK, item_info->m_byOptionExEffect) ||
            ARR_CHECK_IS_CONTAIN(OPTION_EFFECT_INVEN_MASK::MASK, item_info->m_byPCOptionExEffect);
        if (is_inven_effect_item == false) {
            return;
        }
    }
    // (f100721.1L)
    _CalculateRoot(item_slot, item_info, false, is_pc_room, use_update_ex);
    //
#if SUN_CODE_BACKUP
    if (!_CheckCalcAttr(item_slot, item_info))
        return;

    _CalcGenericSubAttr1st(item_slot, item_info);

    if (false)
        ; //no operation
    else if (item_rule_info.is_weapon)
        _CalcSubAttrWeapon(item_slot, item_info);
    else if (item_rule_info.is_armor)
        _CalcSubAttrArmor(item_slot, item_info);
    else if (item_rule_info.is_accessory || item_rule_info.is_accessory_special)
        _CalcSubAttrAccessary(item_slot, item_info);

    _CalcGenericSubAttr2nd(item_slot, item_info, &is_pc_room, &dummy_ref);

    if (auto_update_) {
        if (use_update_ex)
            attributes_.UpdateEx();
        else
            attributes_.Update();
    }
#endif //SUN_CODE_BACKUP
}



// 현재 세트 아이템 옵션을 바꿔야 하는가?
// 리턴값 : 0이면 바꾸지 않는다.
//          1이면 현재 새로 장착, 탈착 된 아이템 값만 바꾼다.
//          2이면 현재 세트 아이템 그룹의 모든 아이템이 바뀌어야 한다.
BYTE
ItemAttrCalculator::GetSetItemChangeType(BYTE number_of_items, bool is_equip)
{
    // 풀 옵션일때 어떻게 할 것인지 생각해 보자.
    if (is_equip)
    {
        switch(number_of_items)
        {
        case 1:                 return 0;
            // 장착시
        case 2: case 4: case 6: return 2;
        default:                return 1;
        }
    }
    else
    {
        switch(number_of_items)
        {
        case 0:                 return 0;
            // 뺄시..
        case 1: case 3: case 5: return 2;
        default:                return 1;
        }
    }

    // Unreached
    return 0;
}


void
ItemAttrCalculator::Clear()
{
    // (f100720.3L)
    attributes_.ClearRelatedAttrKind(eATTR_KIND_ITEM);
    /*
    for (int attr_type = 0; attr_type < eATTR_MAX; ++attr_type)
    {
        if (!attributes_.IsNULL((eATTR_TYPE)attr_type))
        {
            AttrValue& attrs = attributes_[(eATTR_TYPE)attr_type];
            attrs[eATTR_KIND_ITEM] = 0;
            attrs[eATTR_KIND_ITEM_RATIO] = 0;
        }
    }
    */
    if (auto_update_)
    {
        attributes_.Update();
    }

    set_item_count_calculation_type_ = false;
    ZeroMemory(real_equip_slots_, sizeof(real_equip_slots_));
#ifdef _NA_000251_20100727_SOCKET_SYSTEM
    socket_option_calculator_.Clear();
#endif // _NA_000251_20100727_SOCKET_SYSTEM
}

void
ItemAttrCalculator::UpdateAll(BOOL is_pc_room)
{
    Clear();

    POSTYPE size = slot_container_.GetMaxSlotNum();
    for (POSTYPE posIdx = 0; posIdx < size; ++posIdx)
    {
        if (!slot_container_.IsEmpty(posIdx))
        {
            const SCSlot& pSlot = slot_container_.GetSlot(posIdx);
            BOOL bIsFull = false;
            Equip(pSlot, is_pc_room, false, bIsFull);
        }   
    }
}

// (f100812.2L) add an interface to support external module can access add rule filter
void ItemAttrCalculator::ModifyAttrKindAndCalculatedValue(uint8_t value_type, //eVALUE_TYPE
                                                          uint8_t* attr_kind, //eATTR_KIND
                                                          int * value)
{
    *attr_kind = eATTR_KIND_ITEM;
    float calced_value = static_cast<float>(*value);

    if (value_type == VALUE_TYPE_PERCENT_PER_MAX ||
        value_type == VALUE_TYPE_PERCENT_PER_CUR)
    {
        *attr_kind = eATTR_KIND_ITEM_RATIO;
    }
    //
    *value = static_cast<int>(calced_value);
}

void
ItemAttrCalculator::MinusOptionAttribute(eATTR_TYPE attr_type, BYTE value_type, int value)
{
    if (value == 0)
        return;

    if (!(eATTR_TYPE_INVALID < attr_type && attr_type < eATTR_MAX))
        return;

    eATTR_KIND attr_kind = eATTR_KIND_ITEM;
    if (value_type == VALUE_TYPE_PERCENT_PER_MAX || value_type == VALUE_TYPE_PERCENT_PER_CUR)
    {
        attr_kind = eATTR_KIND_ITEM_RATIO;
    }
    static const Attributes::Operation attr_array_for_all_stats[] = {
        eATTR_STR, eATTR_DEX, eATTR_VIT, eATTR_INT, eATTR_SPR
    };
    switch (attr_type)
    {
    case eATTR_INCREASE_STAT_POINT:
        {
            // (f100722.2L)
            int value_array[_countof(attr_array_for_all_stats)] = {
                value, value, value, value, value
            };
            //
            attributes_.GroupOperate(attr_array_for_all_stats->eOper_Sub, attr_kind,
                                     attr_array_for_all_stats, value_array,
                                     _countof(attr_array_for_all_stats));
        #if SUN_CODE_BACKUP
            // 랭크 반지
            attributes_[eATTR_STR][attr_kind] -= value;
            attributes_[eATTR_DEX][attr_kind] -= value;
            attributes_[eATTR_VIT][attr_kind] -= value;
            attributes_[eATTR_INT][attr_kind] -= value;
            attributes_[eATTR_SPR][attr_kind] -= value;
        #endif
        }
        break;
    default:
        {
            attributes_[attr_type][attr_kind] -= value;
        }
        break;
    }
}

void
ItemAttrCalculator::PlusOptionAttribute(eATTR_TYPE attr_type, BYTE value_type, int value)
{
    if (value == 0)
        return;

    if (!(eATTR_TYPE_INVALID < attr_type && attr_type < eATTR_MAX))
        return;

    eATTR_KIND attr_kind = eATTR_KIND_ITEM;
    if (value_type == VALUE_TYPE_PERCENT_PER_MAX || value_type == VALUE_TYPE_PERCENT_PER_CUR)
    {
        attr_kind = eATTR_KIND_ITEM_RATIO;
    }
    static const Attributes::Operation attr_array_for_all_stats[] = {
        eATTR_STR, eATTR_DEX, eATTR_VIT, eATTR_INT, eATTR_SPR
    };

    switch (attr_type)
    {
    case eATTR_INCREASE_STAT_POINT:
        {
            // (f100722.2L)
            int value_array[_countof(attr_array_for_all_stats)] = {
                value, value, value, value, value
            };
            //
            attributes_.GroupOperate(attr_array_for_all_stats->eOper_Add, attr_kind,
                                     attr_array_for_all_stats, value_array,
                                     _countof(attr_array_for_all_stats));
        #if SUN_CODE_BACKUP
            // 랭크 반지
            attributes_[eATTR_STR][attr_kind] += value;
            attributes_[eATTR_DEX][attr_kind] += value;
            attributes_[eATTR_VIT][attr_kind] += value;
            attributes_[eATTR_INT][attr_kind] += value;
            attributes_[eATTR_SPR][attr_kind] += value;
        #endif
        }
        break;
    default:
        {
            attributes_[attr_type][attr_kind] += value;
        }
        break;
    }
}

// 이 함수는 반드시 장착 후에 호출되어야 한다.
// 내부에서 계산된 값은 현재 장착된 아이템에 대해여만 계산한다.
void
ItemAttrCalculator::ApplySetItemOption(const SCSlot* pSlot, bool is_equip)
{
    if (!pSlot)
        return;

    const SCItemSlot& item_slot = static_cast<const SCItemSlot&>(*pSlot);
    SCItemSlotContainer& item_container = static_cast<SCItemSlotContainer&>(slot_container_);
    assert(item_container.GetSlotIdx() == SI_EQUIPMENT);

    const BASE_ITEMINFO* item_info = item_slot.GetItemInfo();

    BYTE set_item_code = item_info->m_bySetType;    
    POSTYPE equip_pos = item_slot.GetPos();

    // replaced by _SOLAR_SLOT_INTEGRITY_CONTAINER_PROCESS_
    //현재 장착되어 있는 아이템중 해당 타입의 세트 아이템을 가지고 온다.
    // (CHANGES) find related set item, to support special accessory multi-link
    SCItemSlot* temp_set_item_slot_array[MAX_INVENTORY_SLOT_NUM];
    SCItemSlotContainer::SameSetItemSlotsResult find_result = \
        item_container.GetSameSetItemSlots(temp_set_item_slot_array,
                                           _countof(temp_set_item_slot_array),
                                           item_info, is_equip);
    // compatible set
    if (find_result.number_of_same_set_items == 0) {
        return;
    }
    BYTE temp_set_item_count = find_result.number_of_same_set_items;
    set_item_code = find_result.set_item_code; // replacing

    SCSlot* set_item_slot_array[MAX_INVENTORY_SLOT_NUM];
    BYTE real_set_item_count = 0;

    if (set_item_count_calculation_type_ && IsEquipCalc())
    {
        // 아이템 개수를 내부 증가에 의한 방법으로 계산시
        for (BYTE byIdx = 0; byIdx < temp_set_item_count; byIdx++) {
            POSTYPE Pos = temp_set_item_slot_array[byIdx]->GetPos();
            if (real_equip_slots_[Pos]) {
                set_item_slot_array[real_set_item_count] = temp_set_item_slot_array[byIdx];
                real_set_item_count++;
            }
        }
    }
    else
    {
        for (int i = 0; i < temp_set_item_count; i++) {
            set_item_slot_array[i] = temp_set_item_slot_array[i];
        }
        real_set_item_count = temp_set_item_count;
    }

    SetItemOptionInfoParser* const set_item_parser = SetItemOptionInfoParser::Instance();
    eSETITEM_OPTION_LEVEL CurEquipLevel = set_item_parser->GetSetItemOptionLevel(real_set_item_count);

    //1. 현재 장/탈착이 풀 옵션으로의 ? 또는 풀 옵션에서는 변화인가?
    bool is_full_option = false;
    if (is_equip) {
        is_full_option = set_item_parser->IsFullOption(set_item_code, real_set_item_count) != false;
    }
    else {
    
        BYTE byAddCount = 0;
        // 스페셜 아이템이면.. 세트아이템 이면서 풀 옵션에 영향을 주지 말아야 한다.
        if (item_info->m_bySetOptionType == item_info->eSetItem_Special) {
            byAddCount = 0;
        }
        else {
            byAddCount = 1;
        }
        is_full_option = set_item_parser->IsFullOption(set_item_code,
                                                       real_set_item_count + byAddCount) != false;
    
    }

    //2. 풀 세트아이템 옵션을 더하던지 뺀다.

    if (is_full_option && item_info->m_bySetOptionType == item_info->eSetItem_Active) {
        ApplySetItemFullOption(set_item_code, is_equip);
    }

    //////////////////////////////////////////////////////////////////////////
    //3. 일반 세트아이템 옵션을 적용시킨다.


    // 스페셜아이템이면 등급처리 하지 않는다.
    BYTE byChangeType = 0;
    if (item_info->m_bySetOptionType == item_info->eSetItem_Special) {
        byChangeType = 0;
    }
    else {
        byChangeType = GetSetItemChangeType(real_set_item_count, is_equip);
    }

    //4. 등급의 변화가 없다면...
    if (byChangeType == 1)
    {
        // 현재 장착된 아이템은 새로 장착된 아이템 이므로 현재 등급 까지의 모든 옵션을 적용시킨다.
        if (CurEquipLevel > SETITEM_OPTION_LEVEL_NONE)
        {
            for (BYTE Level = 1 ; Level <=  CurEquipLevel ; Level++) {
                ApplySetItemOption(set_item_code, equip_pos, (eSETITEM_OPTION_LEVEL)Level, is_equip);
            }
        }
    }
    //2. 등급에 변화가 있을시.
    else if (byChangeType == 2)
    {
        eSETITEM_OPTION_LEVEL OptionLevel = SETITEM_OPTION_LEVEL_NONE;
        if (is_equip) {
            OptionLevel = CurEquipLevel;
        }
        else {
            OptionLevel = (eSETITEM_OPTION_LEVEL)(CurEquipLevel + 1);       //탈착시에는 이전 등급의 옵션을 제거해야 하므로 + 1한다.
        }
        //3. 현재 장/탈착된 아이템은 현재 등급 까지의 모든 옵션을 적용시킨다.
        for (BYTE Level = 1 ; Level <= OptionLevel ; Level++) {
            ApplySetItemOption(set_item_code, equip_pos, (eSETITEM_OPTION_LEVEL)Level, is_equip);
        }
        //4. 현재 장착된 해당 세트 등급의 모든 아이템에 현재 등급 옵션을 적용 시킨다.(현재 장착된 아이템 제외)
        //BYTE bySetOptionResult = 0;
        for (BYTE byIdx = 0; byIdx < real_set_item_count; byIdx++)
        {
            POSTYPE Pos = set_item_slot_array[byIdx]->GetPos();
            if (equip_pos != Pos) {
                ApplySetItemOption(set_item_code, Pos, OptionLevel, is_equip);
            }
        }
    }

    //////////////////////////////////////////////////////////////////////////


    // 현재 풀 옵션이고, pos에 해당 아이템이 장착되어 있다면.
    if (is_full_option)
    {
        const SETITEM_OPTION_INFO* pSetItemOption = \
            SetItemOptionInfoParser::Instance()->Find(set_item_code);
        if (!pSetItemOption) {
            return;
        }
        //1. 현재 더하거나 빼는게 스페셜 아이템이면.
        if (item_info->m_Code == pSetItemOption->m_SpecialEquipOptionItem.m_wItemCode)
        {
            ApplySpecialSetItemOption(set_item_code, is_equip);       
        }
        else
        {
            // 함께 세트 효과가 발동하는 아이템이 장착되어 있는지 확인
            const SCItemSlot& special_item = item_container.GetItemOfType(\
                pSetItemOption->m_SpecialEquipOptionItem.m_EqPos,
                pSetItemOption->m_SpecialEquipOptionItem.m_wItemCode);
            if (special_item.GetItemCode() != 0 && 
                special_item.GetItemCode() == pSetItemOption->m_SpecialEquipOptionItem.m_wItemCode &&
                real_equip_slots_[pSetItemOption->m_SpecialEquipOptionItem.m_EqPos]) {
                ApplySpecialSetItemOption(set_item_code, is_equip);
            }
        }
    }


    return;
}


void
ItemAttrCalculator::ApplySpecialSetItemOption(BYTE bySetCode, bool is_equip)
{
    const SETITEM_OPTION_INFO* pSetItemOption = \
        SetItemOptionInfoParser::Instance()->Find(bySetCode);
    if (!pSetItemOption) {
        return;
    }
    // _countof(g_ITEM_OPTION)는 배열이므로, 51개가 있다면... 값은 52로 나올것이다.
    // 따라서 "=" 를 해준다.
    //
    // (WAVERIX) (NOTE) 위의 주석: 첨삭하지 않으면 오해의 여지가 있어서... _countof는 배열의 개수를
    // 계산하는 유틸, 따라서 배열 크기가 51이면, 51개가 나온다.
    // 단지 g_ITEM_OPTION의 경우는 eATTR_TYPE_INVALID값을 추가해 두었기 때문에 하기와 같은 연산이
    // 가능해지는 것이다.
    for (int i = 0; i < MAX_SETITEM_SPECIAL_OPTION; i++)
    {
        const SET_ITEM_OPTION& special_option = \
            pSetItemOption->m_SpecialEquipOptionItem.m_ActiveOption[i];

        int option_index =  special_option.m_byOptionKind;
        if (option_index == 0) {
            continue;
        }
        if (option_index >= _countof(g_ITEM_OPTION)) {
            continue;
        }
        //이 아이템 옵션을 현재 속성에 적용한다.
        const eATTR_TYPE attr_type = g_ITEM_OPTION[option_index];
        if (is_equip) {
            PlusOptionAttribute(attr_type, special_option.m_byUseType, special_option.m_nOption);
        }
        else {
            MinusOptionAttribute(attr_type, special_option.m_byUseType, special_option.m_nOption);
        }
    }
}


//set_item_count 만큼 해당 능력치를 적용 시킨다.
void
ItemAttrCalculator::ApplySetItemOption(BYTE bySetCode, POSTYPE pos,
                                       eSETITEM_OPTION_LEVEL Level, bool is_equip)
{
    const SET_ITEM_OPTION* pSetItemOption =
        SetItemOptionInfoParser::Instance()->GetSetItemOption(bySetCode, pos, Level);
    if (!pSetItemOption) {
        return;
    }
    // _countof(g_ITEM_OPTION)는 배열이므로, 51개가 있다면... 값은 52로 나올것이다.
    // 따라서 "=" 를 해준다.
    //
    // (WAVERIX) (NOTE) 위의 주석: 첨삭하지 않으면 오해의 여지가 있어서... _countof는 배열의 개수를
    // 계산하는 유틸, 따라서 배열 크기가 51이면, 51개가 나온다.
    // 단지 g_ITEM_OPTION의 경우는 eATTR_TYPE_INVALID값을 추가해 두었기 때문에 하기와 같은 연산이
    // 가능해지는 것이다.
    if (pSetItemOption->m_byOptionKind >= _countof(g_ITEM_OPTION))
        return;

    //이 아이템 옵션을 현재 속성에 적용한다.
    const eATTR_TYPE attr_type = g_ITEM_OPTION[ pSetItemOption->m_byOptionKind ];

    if (is_equip) {
        PlusOptionAttribute(attr_type, pSetItemOption->m_byUseType, pSetItemOption->m_nOption);
    }
    else {
        MinusOptionAttribute(attr_type, pSetItemOption->m_byUseType, pSetItemOption->m_nOption);
    }
#if defined(_DEBUG) && defined(_SERVER)
    SUNLOG(eDEV_LOG,
           "[ItemAttrCalculator::ApplySetItemOption] "
           "Equip = %d, SetCode = %d, Pos = %d, Level = %d, AttrType  =%d, Option = %d",
            is_equip, bySetCode, pos, Level, attr_type, pSetItemOption->m_nOption);
#endif
}

//세트 아이템 풀 옵션을 적용 시킨다.
void
ItemAttrCalculator::ApplySetItemFullOption(BYTE bySetCode, bool is_equip)
{
    SET_ITEM_OPTION* pSetItemOptionArray[MAX_SETITEM_FULL_OPTION];
    int upperbound =
        SetItemOptionInfoParser::Instance()->GetSetItemFullOption(bySetCode, pSetItemOptionArray);
    if (upperbound == 0) {
        return;
    }
    for (int i = 0; i < upperbound; ++i)
    {
        // _countof(g_ITEM_OPTION)는 배열이므로, 51개가 있다면... 값은 52로 나올것이다.
        // 따라서 "=" 를 해준다.
        //
        // (WAVERIX) (NOTE) 위의 주석: 첨삭하지 않으면 오해의 여지가 있어서... _countof는 배열의 개수를
        // 계산하는 유틸, 따라서 배열 크기가 51이면, 51개가 나온다.
        // 단지 g_ITEM_OPTION의 경우는 eATTR_TYPE_INVALID값을 추가해 두었기 때문에 하기와 같은 연산이
        // 가능해지는 것이다.
        const SET_ITEM_OPTION* set_item_option = pSetItemOptionArray[i];
        if (set_item_option->m_byOptionKind >= _countof(g_ITEM_OPTION)) {
            return;
        }
        //이 아이템 옵션을 현재 속성에 적용한다.
        const eATTR_TYPE attr_type = g_ITEM_OPTION[ set_item_option->m_byOptionKind ];

        if (is_equip) {
            PlusOptionAttribute(attr_type, set_item_option->m_byUseType, set_item_option->m_nOption);
        }
        else {
            MinusOptionAttribute(attr_type, set_item_option->m_byUseType, set_item_option->m_nOption);
        }
    }
}

#ifdef _NA_000251_20100727_SOCKET_SYSTEM
ItemAttrCalculator::SocketOptionCalculator::SocketOptionCalculator( 
    SocketOptionAttributes& socket_option_attributes) : socket_option_attributes_(socket_option_attributes)
{
}

void ItemAttrCalculator::SocketOptionCalculator::Clear()
{
    socket_option_attributes_.Clear();
}

bool ItemAttrCalculator::SocketOptionCalculator::CalculateItemOptionPoint(
    const SCItemSlot& item_slot, CalculationResultMap& OUT item_calc_result_map)
{
    item_calc_result_map.clear();

    const int number_of_socket = item_slot.GetSocketNum();
    for (eSOCKET socket = SOCKET_1; socket < number_of_socket; socket = static_cast<eSOCKET>(socket+1))
    {
        // 소켓 옵션 정보가 유효한지 검사한다.
        const sSOCKET_OPTION* const socket_option = item_slot.GetSocketOption(socket);
        if (socket_option == NULL)
        {
            continue;
        }

        // 소켓 옵션의 각 옵션 포인트에 대해
        for (sSOCKET_OPTION::OptionPointInfo::ListSizeType i = 0; 
            i < socket_option->option_point_list_size; ++i)
        {
            // 포인트 정보가 유효한지 검사한다.
            const sSOCKET_OPTION::OptionPointInfo* const option_point_info = 
                socket_option->GetPointInfo(i);
            if (option_point_info == NULL)
            {
                continue;
            }

            // 옵션 정보가 유효한지 검사한다.
            const ZardOptionInfo* const zard_option = option_point_info->zard_option;
            if (zard_option == NULL || 
                zard_option->attr_option_code == eATTR_TYPE_INVALID || 
                zard_option->item_option_code == eATTR_TYPE_INVALID)
            {
                continue;
            }

            // 중복 옵션인지 검사해서 중복이면 포인트 수치를 합친다.
            CalculationResult& item_calc_result = item_calc_result_map[zard_option->attr_option_code];
            if (item_calc_result.attr_option_code == eATTR_TYPE_INVALID)
            {
                item_calc_result.attr_option_code = zard_option->attr_option_code;
                item_calc_result.item_option_code = zard_option->item_option_code;
                item_calc_result.option_point = option_point_info->option_point;
            }
            else
            {
                item_calc_result.option_point += option_point_info->option_point;
            }
        }
    }
    if (item_calc_result_map.empty())
    {
        return false;
    }
    return true;
}

bool ItemAttrCalculator::SocketOptionCalculator::CalculateItemOptionValue(
    const bool is_add, const CalculationResult& item_calc_result, ItemAttrCalculator& OUT item_calculator)
{   
    // 아이템 포인트 계산 결과가 0이면 변경시킬 값이 없다.
    if (item_calc_result.option_point == 0)
    {
        return false;
    }

    // 유효한 옵션인지 검사한다.
    if (item_calc_result.attr_option_code == eATTR_TYPE_INVALID || 
        item_calc_result.item_option_code == eATTR_TYPE_INVALID)
    {
        return false;
    }
    const ZardOptionInfo* const zard_option = 
        (SocketOptionParser::Instance())->FindZardOption(item_calc_result.item_option_code);
    if (zard_option == NULL)
    {
        return false;
    }

    // 옵션 타입을 얻는다.
    const BYTE option_value_type = static_cast<BYTE>(zard_option->GetOptionType());

    // 이전 계산 결과를 제거한다.
    SocketOptionAttributes::AttrValue& prev_attr_value = 
        socket_option_attributes_.GetAttrValueByMutable(item_calc_result.attr_option_code);
    if (prev_attr_value.calc_value != 0)
    {
        item_calculator.MinusOptionAttribute(
            item_calc_result.attr_option_code, 
            option_value_type, 
            prev_attr_value.calc_value);
    }

    // 이전 계산 결과를 갱신한다.
    {
        if (is_add)
        {
            prev_attr_value.point += item_calc_result.option_point;
        }
        else
        {
            prev_attr_value.point -= item_calc_result.option_point;
        }
        const ZardOptionInfo::OptionValueInfo* const zard_option_value = 
            zard_option->Find(prev_attr_value.point);
        if (zard_option_value != NULL)
        {
            prev_attr_value.calc_value = zard_option_value->value;
        }
        else
        {
            prev_attr_value.calc_value = 0;
        }
    }

    // 새로운 계산 결과를 적용한다.
    if (prev_attr_value.calc_value != 0)
    {
        item_calculator.PlusOptionAttribute(
            item_calc_result.attr_option_code, 
            option_value_type, 
            prev_attr_value.calc_value);
    }
//#define SOCKET_OPTION_POINT_CALC_DEBUG // 계산결과 디버깅
#ifdef SOCKET_OPTION_POINT_CALC_DEBUG
    SUNLOG(eCRITICAL_LOG, 
           _T("|OptionPointCalcDebug|IsAdd = %d|ItemOptionCode = %d, ItemOptionPoint = %d|UpdatedAttrPoint = %d, UpdatedAttrCalcValue = %d|"), 
           (is_add ? 1 : 0), 
           item_calc_result.item_option_code, 
           item_calc_result.option_point, 
           prev_attr_value.point, 
           prev_attr_value.calc_value);
#endif // SOCKET_OPTION_POINT_CALC_DEBUG
    return true;
}

void ItemAttrCalculator::SocketOptionCalculator::Calculate(
    const SCItemSlot& item_slot, const bool is_add, ItemAttrCalculator& OUT item_calculator)
{
#ifdef _NA_001699_20110104_SOCKET_OPTION_POINT
    // 포인트 방식 계산에서는 장착 중인 모든 아이템의 옵션 포인트합을 기준으로 실제 옵션값으로 변환해야 한다.
    // 아이템의 착용 상태가 변경될 때 다음과 같이 계산한다.
    // 1. 착용 상태 변경 전에 적용된 옵션값을 제거한다.
    // 2. 착용 상태 변경 후의 옵션 포인트를 계산해서 옵션값으로 변환한 후 적용한다.
    
    // 장착 또는 해제하려는 아이템만의 옵션 포인트를 계산한다.
    CalculationResultMap item_calc_result_map;
    if (CalculateItemOptionPoint(item_slot, item_calc_result_map) == false)
    {
        return;
    }

    // 아이템의 옵션 포인트 계산 결과에 대해
    FOREACH_CONTAINER(
        const CalculationResultMap::value_type& item_calc_result_pair, 
        item_calc_result_map, 
        CalculationResultMap)
    {
        // 옵션값을 계산한다.
        CalculateItemOptionValue(is_add, item_calc_result_pair.second, item_calculator);
    }
#else // _NA_001699_20110104_SOCKET_OPTION_POINT
    const int number_of_socket = item_slot.GetSocketNum();
    for (eSOCKET socket = SOCKET_1; 
        socket < number_of_socket; 
        socket = static_cast<eSOCKET>(socket+1))
    {
        const sSOCKET_OPTION* const socket_option = item_slot.GetSocketOption(socket);
        if (socket_option == NULL)
        {
            continue;
        }

        if (is_add)
        {
            item_calculator.PlusOptionAttribute(
                socket_option->attr_option_kind, 
                socket_option->option_type, 
                socket_option->option_value);
        }
        else
        {
            item_calculator.MinusOptionAttribute(
                socket_option->attr_option_kind, 
                socket_option->option_type, 
                socket_option->option_value);
        }
    }
#endif // _NA_001699_20110104_SOCKET_OPTION_POINT
}
#endif // _NA_000251_20100727_SOCKET_SYSTEM
#endif //if !defined(_NA_002289_20110322_EQUIPMENTS_CALCULATOR_CHANGES_FOR_IMPROVEMENT)