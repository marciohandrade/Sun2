#pragma once
#ifndef PROGRAMCOMMON_COMMON_CHARACTER_FORMULA_H
#define PROGRAMCOMMON_COMMON_CHARACTER_FORMULA_H

#include "ItemOptionHeader.h"

//==================================================================================================
// @history
//  - ~10.12.20
//  - 10.12.20, waverix, remove unused section
//
//==================================================================================================

namespace ns_formula {
;

struct SkillAttackPowerBalanceConstants
{
    double ratio_of_conversion[eCHAR_TYPE_MAX];
};

struct CriticalBalanceConstants
{
    double kNumberatorWeight;
    double kDenominatorWeight;
    double base_probabilies[eCHAR_TYPE_MAX];
};

struct AttackRateConstants
{
    double kEquipmentWeight;
    double ratio_of_conversion[eCHAR_TYPE_MAX];
};
extern const double& AttackRateConstants_EquipmentWeight;

struct AttackAvoidRateConstants
{
    double kEquipmentWeight;
    double ratio_of_conversion[eCHAR_TYPE_MAX];
};
extern const double& AttackAvoidRateConstants_EquipmentWeight;

struct BalanceConstantsChangerInProgramCommon
{
    static void GetSkillAttackPower(SkillAttackPowerBalanceConstants* const constants);
    static void ModifySkillAttackPower(const SkillAttackPowerBalanceConstants& constants);
    static void GetCriticalChance(CriticalBalanceConstants* const constants);
    static void ModifyCriticalChance(const CriticalBalanceConstants& constants);
    static void GetAttackRate(AttackRateConstants* const constants);
    static void ModifyAttackRate(const AttackRateConstants& constants);
    static void GetAttackAvoidRate(AttackAvoidRateConstants* const constants);
    static void ModifyAttackAvoidRate(const AttackAvoidRateConstants& constants);
};

//==================================================================================================

// 레벨당 스탯포인트
extern DWORD GetStatPointPerLevel();

// 특정 레벨에서의 스킬포인트
extern DWORD GetSkillPoint(LEVELTYPE level);

// 기본값 계산
extern DWORD CalcHP(const eCHAR_TYPE char_type, const LEVELTYPE level, const short vitality);
extern DWORD CalcMP(const eCHAR_TYPE char_type, const LEVELTYPE level, const short spirit);
extern DWORD CalcSD(eCHAR_TYPE char_type, LEVELTYPE level);
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
extern PLAYERFP CalcFP(const eCHAR_TYPE char_type);
#endif //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
// 공격속도
extern int CalcAttackSpeedRatio(eCHAR_TYPE char_type,
                                DWORD WeaponKind,
                                short dexterity,
                                int iItemAttSpeed,
                                int iSkillAttSpeed);

// 이동속도
#ifdef _NA_008486_20150914_TOTAL_BALANCE
extern int CalcMoveSpeedRatio(int iItemMoveSpeed ,int iSkillMoveSpeed, LEVELTYPE char_level = 0);
#else
extern int CalcMoveSpeedRatio(short dexterity, int iItemMoveSpeed ,int iSkillMoveSpeed);
#endif //_NA_008486_20150914_TOTAL_BALANCE

// 기본 공격력(물리)
extern DWORD CalcMinMeleeAttackPower(
    const eCHAR_TYPE char_type, const short strength, const short dexterity); // 근거리
extern DWORD CalcMaxMeleeAttackPower(
    const eCHAR_TYPE char_type, const short strength, const short dexterity);
extern DWORD CalcMinRangeAttackPower(
    const eCHAR_TYPE char_type, const short strength, const short dexterity); // 원거리
extern DWORD CalcMaxRangeAttackPower(
    const eCHAR_TYPE char_type, const short strength, const short dexterity);

// UNUSED(EP2): old comment, 기본 공격력(마법)
extern DWORD CalcMagicAttackPower(BOOL bMin, short intelligence);

// CHANGES: f110103.4L, changes concept of a CalcMagicAttackPower.
// added by _NA001605_EP2_ATTRIBUTES_RENEWAL_
// 기본 스킬 공격력
extern DWORD CalcSkillAttackPower(const eCHAR_TYPE char_type, const short intelligence);

// critical
extern DWORD CalcPhyCriticalBaseRatio(
    const eCHAR_TYPE char_type, const LEVELTYPE level, const short dexterity);
extern DWORD CalcMagicCriticalBaseRatio(
    const eCHAR_TYPE char_type, const LEVELTYPE level, const short spirit);


// 방어력
extern DWORD CalcPhyBaseDef(const eCHAR_TYPE char_type, const short vitality);
extern DWORD CalcMagicBaseDef(const eCHAR_TYPE char_type, const short spirit);

// 공격성공율
extern DWORD CalcPhysicalAttackRateBase(
    const eCHAR_TYPE char_type, const LEVELTYPE level, const short dexterity);

// 파티원 수에 따른 NPC HP 증가율
extern float CalcHPIncreaseRatioAsParty(DWORD partymemberNum);

// 파티원 수에 따른 NPC 공격력 증가수치
extern int CalcAttackPowerIncreaseAsParty(DWORD partymemberNum, int iAttackPower);

#ifdef _NA_008486_20150914_TOTAL_BALANCE
// 공격회피율
extern DWORD CalcPhysicalAvoidRateBase(
    const eCHAR_TYPE char_type, const LEVELTYPE level, const short vitality);

extern INT CalcDebuffDuration(const short spirit);
#else
// 공격회피율
extern DWORD CalcPhysicalAvoidRateBase(
                                       const eCHAR_TYPE char_type, const LEVELTYPE level, const short dexterity);
#endif //_NA_008486_20150914_TOTAL_BALANCE
/*
// added by _NA001605_EP2_ATTRIBUTES_RENEWAL_
struct DamageEncoder
{
#define USE_DAMAGE_EXTENTION    0
#if defined(_NA001605_EP2_ATTRIBUTES_RENEWAL_) && USE_DAMAGE_EXTENTION
    static const int kMaxDamageExpression = (0x00040000 - 1); // 262144 - 1
    static const int kMaxDamage = 260000;
#else
    static const int kMaxDamageExpression = (0x00010000 - 1); //  65536 - 1
    static const int kMaxDamage =  65000;
#endif
    static int Fix(int origin_damage);
    static DAMAGETYPE Encode(int fixed_damage);
    static int Decode(DAMAGETYPE encoded_damage);
};
*/
}; //end of namespace 'ns_formula'

//==================================================================================================

namespace ns_formula { namespace internal {
;

#ifdef _NA_008486_20150914_TOTAL_BALANCE
int CalcMoveSpeedRatio(int item_move_speed, int skill_move_speed, LEVELTYPE char_level = 0);
#else
int CalcMoveSpeedRatio(short dexterity, int item_move_speed, int skill_move_speed);
#endif //_NA_008486_20150914_TOTAL_BALANCE

int CalcAttackSpeedRatio(eCHAR_TYPE char_type, DWORD weapon_kind,
    short dexterity, int item_attr_speed, int skill_attr_speed);

}}; //end of namespace

//==================================================================================================

// NOTE: logic dispersion for hack prevention facility
#ifdef _NA_008486_20150914_TOTAL_BALANCE
__forceinline int ns_formula::internal::CalcMoveSpeedRatio(
                            int item_move_speed, int skill_move_speed, LEVELTYPE char_level /*= 0*/)
{
#ifdef _NA_008486_20151105_MODIFY_MOVESPEED_FORMULA
    return (100 + (int)(char_level*0.08f)+item_move_speed + skill_move_speed);
#else
    return (100 + item_move_speed + skill_move_speed);
#endif //_NA_008486_20151105_MODIFY_MOVESPEED_FORMULA
}
#else //_NA_008486_20150914_TOTAL_BALANCE
__forceinline int ns_formula::internal::CalcMoveSpeedRatio(
    short dexterity, int item_move_speed, int skill_move_speed)
{
    return (100 + dexterity / 15 + item_move_speed + skill_move_speed);
}
#endif //_NA_008486_20150914_TOTAL_BALANCE

// NOTE: logic dispersion for hack prevention facility
__forceinline int ns_formula::internal::CalcAttackSpeedRatio(
    eCHAR_TYPE char_type, DWORD weapon_kind,
    short dexterity, int item_attr_speed, int skill_attr_speed)
{
    // CHANGES: f110119.3L, remove a rule after application of the socket system, post result is OK
    //ASSERT (item_attr_speed >= 0);

    // item_attr_speed 0 이면 맨손이란 의미로 해석을 하고. 공속을 1.0으로 맞춰줌
    if (weapon_kind == eWEAPONTYPE_PUNCH || item_attr_speed == 0)
    {
        item_attr_speed = 150;
    }

    if (dexterity < 0)
    {
        dexterity = 0;
    }

    float stat_bonus = 0.0f;
#ifdef _NA_008486_20150914_TOTAL_BALANCE
    switch (char_type)
    {
    case eCHAR_DRAGON:
        stat_bonus = dexterity / 1600.0f;
        break;
    case eCHAR_BERSERKER:
        stat_bonus = dexterity / 1200.0f;
        break;
    case eCHAR_SHADOW:      
        stat_bonus = dexterity / 1800.0f;
        break;
    case eCHAR_VALKYRIE:
        stat_bonus = dexterity / 2000.0f;
        break;
    case eCHAR_ELEMENTALIST: 
        stat_bonus = dexterity / 1400.0f;
        break;
    case eCHAR_MYSTIC: // _NA_004965_20120613_NEW_CHARACTER_MYSTIC  
        stat_bonus = dexterity / 1800.0f;
        break;
    case eCHAR_HELLROID: //_NA_000000_20130812_NEW_CHARACTER_HELLROID
        stat_bonus = dexterity / 1200.0f;
        break;
    case eCHAR_WITCHBLADE: //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
        stat_bonus = dexterity / 1800.0f;
        break;
    }
#else
    switch (char_type)
    {
    case eCHAR_DRAGON:
        stat_bonus = dexterity / 800.0f;
        break;
    case eCHAR_BERSERKER:
        stat_bonus = dexterity / 600.0f;
        break;
    case eCHAR_SHADOW:      
        stat_bonus = dexterity / 900.0f;
        break;
    case eCHAR_VALKYRIE:
        stat_bonus = dexterity / 1000.0f;
        break;
    case eCHAR_ELEMENTALIST: 
        stat_bonus = dexterity / 700.0f;
        break;
    case eCHAR_MYSTIC: // _NA_004965_20120613_NEW_CHARACTER_MYSTIC  
        stat_bonus = dexterity / 900.0f;
        break;
    case eCHAR_HELLROID: //_NA_000000_20130812_NEW_CHARACTER_HELLROID
        stat_bonus = dexterity / 600.0f;
        break;
    case eCHAR_WITCHBLADE: //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
        stat_bonus = dexterity / 900.0f;
        break;
    }
#endif //_NA_008486_20150914_TOTAL_BALANCE

    float weighted_ratio = 1.0f + stat_bonus + skill_attr_speed/100.0f;

#ifdef _NA_008481_20150909_MODIFY_REMOVE_ATTSPEED_300LIMIT
    return (int)(item_attr_speed * weighted_ratio);
#else
    return min(300, (int)(item_attr_speed * weighted_ratio));
#endif //_NA_008481_20150909_MODIFY_REMOVE_ATTSPEED_300LIMIT
}

//==================================================================================================

#endif //PROGRAMCOMMON_COMMON_CHARACTER_FORMULA_H