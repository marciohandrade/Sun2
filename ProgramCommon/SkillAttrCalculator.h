#ifndef __PROGRAMCOMMON_SKILL_ATTR_CALCULATOR_H
#define __PROGRAMCOMMON_SKILL_ATTR_CALCULATOR_H
#pragma once

#include <StructInPacket.h>
#include <Attribute.h>

class SCItemSlot;
class Player;

//==================================================================================================
namespace nsSkill {
;
// NOTE: f110808.5L, adrenaline configuration
struct AdrenalineConfiguration
{
#define ADRENALINE_ATTR_OFFSET(_attr_type_) eOFFSET_##_attr_type_
    enum {
        ADRENALINE_ATTR_OFFSET(eATTR_BASE_MELEE_MIN_ATTACK_POWER),
        ADRENALINE_ATTR_OFFSET(eATTR_BASE_MELEE_MAX_ATTACK_POWER),
        ADRENALINE_ATTR_OFFSET(eATTR_BASE_RANGE_MIN_ATTACK_POWER),
        ADRENALINE_ATTR_OFFSET(eATTR_BASE_RANGE_MAX_ATTACK_POWER),
    //#if !defined(_NA001605_EP2_ATTRIBUTES_RENEWAL_)
    //    eATTR_BASE_MAGICAL_MIN_ATTACK_POWER, eATTR_BASE_MAGICAL_MAX_ATTACK_POWER,
    //    eATTR_BASE_MAGICAL_DEFENSE_POWER,
    //#endif
        //
        ADRENALINE_ATTR_OFFSET(eATTR_BASE_MELEE_DEFENSE_POWER),
        ADRENALINE_ATTR_OFFSET(eATTR_BASE_RANGE_DEFENSE_POWER),
        //
        ADRENALINE_ATTR_OFFSET(eATTR_MOVE_SPEED),
        ADRENALINE_ATTR_OFFSET(eATTR_ATTACK_SPEED),
        ADRENALINE_ATTR_OFFSET(eATTR_PHYSICAL_ATTACK_SUCCESS_RATIO),
        ADRENALINE_ATTR_OFFSET(eATTR_MAX_HP),
        //
        MaxCounts
    };
#undef ADRENALINE_ATTR_OFFSET
    //
    static const Attributes::Operation kSkillAttrTypeArray[MaxCounts];
    int skill_attr_values[MaxCounts];
};
//
}; //end of namespace
//==================================================================================================

class SkillAttrCalculator
{
public:
    SkillAttrCalculator(Attributes& attributes) : attributes_(attributes) {}
    virtual ~SkillAttrCalculator() {}
    // (f100812.2L) add an interface to support external module can access add rule filter
    static bool IsPassedAddAttr(eATTR_TYPE attr_type);
    static void ModifyAttrKindAndCalculatedValue(uint8_t value_type, //eVALUE_TYPE
                                                 uint8_t* attr_kind, //eATTR_KIND
                                                 int * value);
    //                                    eVALUE_TYPE
    float AddAttr(eATTR_TYPE attr_type, int value_type, int value, bool skip_update = false);
    float DelAttr(eATTR_TYPE attr_type, int value_type, int value, bool skip_update = false);
#ifdef _NA_008220_20150414_BUG_ETHERBULLET_ATTR
    // 에테르탄환 뎀증률 갱신을 위한 함수
    // 이 함수는 Attr 설계에 매우 부적절한 함수
    // 특정 목적에 의해서만 사용되도록 한다.

    //특정 Attr의 수치를 0으로 세팅한다
    float ClearAttr(eATTR_TYPE attr_type, int value_type, bool skip_update = false);
#endif //_NA_008220_20150414_BUG_ETHERBULLET_ATTR
    void  Clear();

    int  ApplyDragonFormation(BOOL bApply, SLOTCODE skill_code, DWORD cur_hp = 0);
    // NOTE: f110808.5L, adrenaline configuration
    static bool GetCurrentAdrenalineConfiguration(
        nsSkill::AdrenalineConfiguration* const adrenaline_data,
        SLOTCODE skill_code, const Attributes& summoner_attr);
#if SUN_CODE_RENEWAL // CHANGES: f110808.5L, prevent the overlapped attribute calculation
    bool ApplyAdrenaline(bool applied, SLOTCODE skill_code,
        const nsSkill::AdrenalineConfiguration& adrenaline_data);
#else
    bool ApplyAdrenaline(BOOL bApply, SLOTCODE skill_code, const Attributes& summoner_attr);
#endif
    // (arguments)
    // total_apply_x8bits : 0   - add apply rider option
    //                      1   - sub apply rider option
    //                      2~4 - add apply parts option
    //                      5~7 - sub apply parts option
    bool ApplyRidingRider(bool is_apply, BOOL is_pc_room,
                          const SCItemSlot& rider_item,
                          const nsSlot::RiderUpdateBits& total_apply_x8bits);
private:
    // attr_type filtering은 호출 전에 수행할 것 + eATTR_INCREASE_STAT_POINT은 호출하지 말 것
    // (WARNING) no-update function
    inline void _CalcAttrValue(bool is_add, AttrValue& attr_value, int value_type, int value);
    bool _CanApplyRiderOption(eATTR_TYPE attr_type) const;
private:
    bool IsRecoverAttr(eATTR_TYPE attr_type); // Add를 한 다음에 다시 Del(복귀)를 하는 속성인가

protected:
    Attributes& attributes_;
    //
    __DISABLE_COPY(SkillAttrCalculator);
};

// (f100812.2L) add an interface to support external module can access add rule filter
inline bool SkillAttrCalculator::IsPassedAddAttr(eATTR_TYPE attr_type)
{
    // 공격력, 방어력은 Option 수치를 변경할 뿐 Base 수치를 변경하지는 않는다!
    if (attr_type == eATTR_BASE_MELEE_MIN_ATTACK_POWER ||
        attr_type == eATTR_BASE_RANGE_MIN_ATTACK_POWER ||
    #if !defined(_NA001605_EP2_ATTRIBUTES_RENEWAL_)
        attr_type == eATTR_BASE_MAGICAL_MIN_ATTACK_POWER ||
        attr_type == eATTR_BASE_MAGICAL_DEFENSE_POWER ||
    #endif
        attr_type == eATTR_BASE_MELEE_DEFENSE_POWER ||
        attr_type == eATTR_BASE_RANGE_DEFENSE_POWER)
    {
        return false;
    }
    return true;
}

// (CHANGES) (WAVERIX) move to header
inline bool
SkillAttrCalculator::IsRecoverAttr(eATTR_TYPE attr_type)
{
    if (attr_type == eATTR_CUR_HP || attr_type == eATTR_CUR_MP)
    {
        return false;
    }
    else
    {
        return true;
    }
}

// (NOTE) Stat의 변화는 장비의 장착 관련 문제에 지독하게 의존되어 있다. (SetOption포함)
// 따라서 문제공간을 최대한 줄이기 위해서는 Stat의 변화는 없도록 하는 쪽이 절대 안전하다.
// 기획과 협의할 필요 있지만, Rider|Parts계열이 Stat의 변화를 일으키지 않는 쪽은 동의하리라 생각한다.
inline bool
SkillAttrCalculator::_CanApplyRiderOption(eATTR_TYPE attr_type) const
{
    switch (attr_type)
    {
    case eATTR_STR:  // 힘추가
    case eATTR_DEX:  // 민첩추가
    case eATTR_INT:  // 지력추가
    case eATTR_VIT:  // 체력추가
    case eATTR_SPR:  // 정신력추가
    case eATTR_INCREASE_STAT_POINT:
        return false;
    }
    return true;
}

#endif //__PROGRAMCOMMON_SKILL_ATTR_CALCULATOR_H
