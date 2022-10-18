#include "stdafx.h"
#include "./CommonCharacterFormula.h"

#include <Struct.h>
#include "NumericValues.h"

//==================================================================================================
namespace ns_formula {
;

// internal interfaces
static float CalcHPRatioNum(DWORD partymemberNum);
static float CalcAdditionalHPRatio(DWORD partymemberNum);
//

//==================================================================================================

static ns_formula::SkillAttackPowerBalanceConstants kSkillAttackPowerConstants =
{
    // ratio_of_conversion
#ifdef _NA_008486_20150914_TOTAL_BALANCE
    {
        0.0f, // eCHAR_NONE
        90.0f, // eCHAR_BERSERKER
        90.0f, // eCHAR_DRAGON
        90.0f, // eCHAR_SHADOW
        90.0f, // eCHAR_VALKYRIE
        90.0f, // eCHAR_ELEMENTALIST
        90.0f, // eCHAR_MYSTIC
        90.0f, // eCHAR_HELLROID
        90.0f, // eCHAR_WITCHBLADE
    },
#else
    {
        0.0f, // eCHAR_NONE
        90.0f, // eCHAR_BERSERKER
        80.0f, // eCHAR_DRAGON
        70.0f, // eCHAR_SHADOW
        60.0f, // eCHAR_VALKYRIE
        50.0f, // eCHAR_ELEMENTALIST
        80.0f, // eCHAR_MYSTIC
        90.0f, // eCHAR_HELLROID
        90.0f, // eCHAR_WITCHBLADE
    },
#endif //_NA_008486_20150914_TOTAL_BALANCE
};

static ns_formula::CriticalBalanceConstants kCriticalChanceConstants =
{
#ifdef _NA_008486_20150914_TOTAL_BALANCE
    60.0f, // kNumberatorWeight
#else
    5.0f, // kNumberatorWeight
#endif //_NA_008486_20150914_TOTAL_BALANCE
    100.0f, // kDenominatorWeight
    // base_probabilies
    {
        0.0f, // eCHAR_NONE
        0.8f, // eCHAR_BERSERKER
        1.7f, // eCHAR_DRAGON
        1.2f, // eCHAR_SHADOW
        1.0f, // eCHAR_VALKYRIE
        0.2f, // eCHAR_ELEMENTALIST
        1.5f, // eCHAR_MYSTIC
        0.8f, // eCHAR_HELLROID
        1.0f, // eCHAR_WITCHBLADE
    },
};

static ns_formula::AttackRateConstants kAttackRateConstants =
{
    1.0f,
#ifdef _NA_008486_20150914_TOTAL_BALANCE
    {
        6.0f, // eCHAR_NONE
        6.0f, // eCHAR_BERSERKER
        6.0f, // eCHAR_DRAGON
        6.0f, // eCHAR_SHADOW
        6.0f, // eCHAR_VALKYRIE
        6.0f, // eCHAR_ELEMENTALIST
        6.0f, // eCHAR_MYSTIC
        6.0f, // eCHAR_HELLROID
        6.0f, // eCHAR_WITCHBLADE
    },
#else
    {
        4.0f, // eCHAR_NONE
        4.0f, // eCHAR_BERSERKER
        4.0f, // eCHAR_DRAGON
        4.0f, // eCHAR_SHADOW
        4.0f, // eCHAR_VALKYRIE
        4.0f, // eCHAR_ELEMENTALIST
        4.0f, // eCHAR_MYSTIC
        4.0f, // eCHAR_HELLROID
        4.0f, // eCHAR_WITCHBLADE
    },
#endif //_NA_008486_20150914_TOTAL_BALANCE
};

static ns_formula::AttackAvoidRateConstants kAttackAvoidRateConstants =
{
    0.2f,
#ifdef _NA_008486_20150914_TOTAL_BALANCE
    {
        0.0f, // eCHAR_NONE
        5.0f, // eCHAR_BERSERKER
        5.0f, // eCHAR_DRAGON
        5.0f, // eCHAR_SHADOW
        5.0f, // eCHAR_VALKYRIE
        5.0f, // eCHAR_ELEMENTALIST
        5.0f, // eCHAR_MYSTIC
        5.0f, // eCHAR_HELLROID
        5.0f, // eCHAR_WITCHBLADE
    },
#else
    {
        0.0f, // eCHAR_NONE
        8.0f, // eCHAR_BERSERKER
        8.0f, // eCHAR_DRAGON
        8.0f, // eCHAR_SHADOW
        5.0f, // eCHAR_VALKYRIE
        5.0f, // eCHAR_ELEMENTALIST
        6.0f, // eCHAR_MYSTIC
        8.0f, // eCHAR_HELLROID
        6.0f, // eCHAR_WITCHBLADE
    },
#endif //_NA_008486_20150914_TOTAL_BALANCE
};

extern const double& AttackRateConstants_EquipmentWeight =
    kAttackRateConstants.kEquipmentWeight;
extern const double& AttackAvoidRateConstants_EquipmentWeight =
    kAttackAvoidRateConstants.kEquipmentWeight;

//==================================================================================================

void ns_formula::BalanceConstantsChangerInProgramCommon::GetSkillAttackPower(
    SkillAttackPowerBalanceConstants* const constants)
{
    *constants = ns_formula::kSkillAttackPowerConstants;
}

void ns_formula::BalanceConstantsChangerInProgramCommon::ModifySkillAttackPower(
    const SkillAttackPowerBalanceConstants& constants)
{
    SkillAttackPowerBalanceConstants modified = constants;
    modified.ratio_of_conversion[eCHAR_NONE] = 0.0f;
    // > 0.0f
    if (modified.ratio_of_conversion[eCHAR_BERSERKER] < 0.0f) {
        modified.ratio_of_conversion[eCHAR_BERSERKER] = 90.0f;
    };
    if (modified.ratio_of_conversion[eCHAR_DRAGON] < 0.0f) {
        modified.ratio_of_conversion[eCHAR_DRAGON] = 80.0f;
    };
    if (modified.ratio_of_conversion[eCHAR_SHADOW] < 0.0f) {
        modified.ratio_of_conversion[eCHAR_SHADOW] = 70.0f;
    };
    if (modified.ratio_of_conversion[eCHAR_VALKYRIE] < 0.0f) {
        modified.ratio_of_conversion[eCHAR_VALKYRIE] = 60.0f;
    };
    if (modified.ratio_of_conversion[eCHAR_ELEMENTALIST] < 0.0f) {
        modified.ratio_of_conversion[eCHAR_ELEMENTALIST] = 50.0f;
    };
    // _NA_004965_20120613_NEW_CHARACTER_MYSTIC
    if (modified.ratio_of_conversion[eCHAR_MYSTIC] < 0.0f) 
    {
        modified.ratio_of_conversion[eCHAR_MYSTIC] = 70.0f;
    }
    // _NA_000000_20130812_NEW_CHARACTER_HELLROID
    if (modified.ratio_of_conversion[eCHAR_HELLROID] < 0.0f) 
    {
        modified.ratio_of_conversion[eCHAR_HELLROID] = 90.0f;
    }
    //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    if (modified.ratio_of_conversion[eCHAR_WITCHBLADE] < 0.0f) 
    {
        modified.ratio_of_conversion[eCHAR_WITCHBLADE] = 80.0f;
    }

    ns_formula::kSkillAttackPowerConstants = modified;
    return;
}

void ns_formula::BalanceConstantsChangerInProgramCommon::GetCriticalChance(
    CriticalBalanceConstants* const constants)
{
    *constants = ns_formula::kCriticalChanceConstants;
}

void ns_formula::BalanceConstantsChangerInProgramCommon::ModifyCriticalChance(
    const CriticalBalanceConstants& constants)
{
    CriticalBalanceConstants modified = constants;
    // > 0.0f
    /* NOTE: it's dangerous if the below code was opened.
    if (modified.kNumberatorWeight <= 0.0f) {
        modified.kNumberatorWeight = 3.0f;
    };
    if (modified.kDenominatorWeight <= 0.0f) {
        modified.kDenominatorWeight = 5.0f;
    };
    */
    modified.base_probabilies[eCHAR_NONE] = 0.0f;
    if (modified.base_probabilies[eCHAR_BERSERKER] < 0.0f) {
        modified.base_probabilies[eCHAR_BERSERKER] = 1.7f;
    };
    if (modified.base_probabilies[eCHAR_DRAGON] < 0.0f) {
        modified.base_probabilies[eCHAR_DRAGON] = 3.3f;
    };
    if (modified.base_probabilies[eCHAR_SHADOW] < 0.0f) {
        modified.base_probabilies[eCHAR_SHADOW] = 2.3f;
    };
    if (modified.base_probabilies[eCHAR_VALKYRIE] < 0.0f) {
        modified.base_probabilies[eCHAR_VALKYRIE] = 2.0f;
    };
    if (modified.base_probabilies[eCHAR_ELEMENTALIST] < 0.0f) {
        modified.base_probabilies[eCHAR_ELEMENTALIST] = 0.3f;
    };
    // _NA_004965_20120613_NEW_CHARACTER_MYSTIC
    if (modified.base_probabilies[eCHAR_MYSTIC] < 0.0f) {
        modified.base_probabilies[eCHAR_MYSTIC] = 2.3f;
    };
    // _NA_000000_20130812_NEW_CHARACTER_HELLROID
    if (modified.base_probabilies[eCHAR_HELLROID] < 0.0f)  {
        modified.base_probabilies[eCHAR_HELLROID] = 0.8f;
    };
    //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    if (modified.base_probabilies[eCHAR_WITCHBLADE] < 0.0f)  {
        modified.base_probabilies[eCHAR_WITCHBLADE] = 1.0f;
    };

    ns_formula::kCriticalChanceConstants = modified;
    return;
}

void ns_formula::BalanceConstantsChangerInProgramCommon::GetAttackRate(
    AttackRateConstants* const constants)
{
    *constants = ns_formula::kAttackRateConstants;
}

void ns_formula::BalanceConstantsChangerInProgramCommon::ModifyAttackRate(
    const AttackRateConstants& constants)
{
    AttackRateConstants modified = constants;
    if (modified.kEquipmentWeight < 0.0f) {
        modified.kEquipmentWeight = 1.0f;
    }
    modified.ratio_of_conversion[eCHAR_NONE] = 0.0f;
    if (modified.ratio_of_conversion[eCHAR_BERSERKER] < 0.0f) {
        modified.ratio_of_conversion[eCHAR_BERSERKER] = 4.0f;
    };
    if (modified.ratio_of_conversion[eCHAR_DRAGON] < 0.0f) {
        modified.ratio_of_conversion[eCHAR_DRAGON] = 4.0f;
    };
    if (modified.ratio_of_conversion[eCHAR_SHADOW] < 0.0f) {
        modified.ratio_of_conversion[eCHAR_SHADOW] = 4.0f;
    };
    if (modified.ratio_of_conversion[eCHAR_VALKYRIE] < 0.0f) {
        modified.ratio_of_conversion[eCHAR_VALKYRIE] = 4.0f;
    };
    if (modified.ratio_of_conversion[eCHAR_ELEMENTALIST] < 0.0f) {
        modified.ratio_of_conversion[eCHAR_ELEMENTALIST] = 4.0f;
    };
    // _NA_004965_20120613_NEW_CHARACTER_MYSTIC
    if (modified.ratio_of_conversion[eCHAR_MYSTIC] < 0.0f) {
        modified.ratio_of_conversion[eCHAR_MYSTIC] = 4.0f;
    };
    //_NA_000000_20130812_NEW_CHARACTER_HELLROID
    if (modified.ratio_of_conversion[eCHAR_HELLROID] < 0.0f) {
        modified.ratio_of_conversion[eCHAR_HELLROID] = 4.0f;
    };
    //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    if (modified.ratio_of_conversion[eCHAR_WITCHBLADE] < 0.0f) {
        modified.ratio_of_conversion[eCHAR_WITCHBLADE] = 4.0f;
    };

    ns_formula::kAttackRateConstants = modified;
    return;
}

void ns_formula::BalanceConstantsChangerInProgramCommon::GetAttackAvoidRate(
    AttackAvoidRateConstants* const constants)
{
    *constants = ns_formula::kAttackAvoidRateConstants;
}

void ns_formula::BalanceConstantsChangerInProgramCommon::ModifyAttackAvoidRate(
    const AttackAvoidRateConstants& constants)
{
    AttackAvoidRateConstants modified = constants;
    if (modified.kEquipmentWeight < 0.0f) {
        modified.kEquipmentWeight = 0.2f;
    }
    modified.ratio_of_conversion[eCHAR_NONE] = 0.0f;
    if (modified.ratio_of_conversion[eCHAR_BERSERKER] < 0.0f) {
        modified.ratio_of_conversion[eCHAR_BERSERKER] = 8.0f;
    };
    if (modified.ratio_of_conversion[eCHAR_DRAGON] < 0.0f) {
        modified.ratio_of_conversion[eCHAR_DRAGON] = 8.0f;
    };
    if (modified.ratio_of_conversion[eCHAR_SHADOW] < 0.0f) {
        modified.ratio_of_conversion[eCHAR_SHADOW] = 8.0f;
    };
    if (modified.ratio_of_conversion[eCHAR_VALKYRIE] < 0.0f) {
        modified.ratio_of_conversion[eCHAR_VALKYRIE] = 3.5f;
    };
    if (modified.ratio_of_conversion[eCHAR_ELEMENTALIST] < 0.0f) {
        modified.ratio_of_conversion[eCHAR_ELEMENTALIST] = 16.0f;
    };
    // _NA_004965_20120613_NEW_CHARACTER_MYSTIC
    if (modified.ratio_of_conversion[eCHAR_MYSTIC] < 0.0f) {
        modified.ratio_of_conversion[eCHAR_MYSTIC] = 8.0f;
    }
    // _NA_000000_20130812_NEW_CHARACTER_HELLROID
    if (modified.ratio_of_conversion[eCHAR_HELLROID] < 0.0f) {
        modified.ratio_of_conversion[eCHAR_HELLROID] = 8.0f;
    }
    //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    if (modified.ratio_of_conversion[eCHAR_WITCHBLADE] < 0.0f) {
        modified.ratio_of_conversion[eCHAR_WITCHBLADE] = 7.0f;
    }

    ns_formula::kAttackAvoidRateConstants = modified;
    return;
}

//==================================================================================================
//==================================================================================================
//==================================================================================================


//==================================================================================================
/*
#if defined(_NA001605_EP2_ATTRIBUTES_RENEWAL_) && USE_DAMAGE_EXTENTION

int ns_formula::DamageEncoder::Fix(int origin_damage)
{
    // 0x0000
    // 2bit : 0, 1, 2, 3
    // gap  : 0  1  4  8
    int fixed_damage = origin_damage;
    if (fixed_damage < 0) {
        fixed_damage = 0;
    }
    else if (fixed_damage > kMaxDamage) {
        fixed_damage = kMaxDamage;
    };
    //
    fixed_damage
        //   4096 - 1
        = (fixed_damage < 0x00001000) ? ((fixed_damage))
        //  16384 - 1
        : (fixed_damage < 0x00004000) ? ((fixed_damage & ~0x1))
        //  65536 - 1
        : (fixed_damage < 0x00010000) ? ((fixed_damage & ~0x3))
        // 262144 - 1
        :                               ((fixed_damage & ~0xF));

    return fixed_damage;
}

DAMAGETYPE ns_formula::DamageEncoder::Encode(int fixed_damage)
{
    assert(fixed_damage >= 0 &&
           fixed_damage <= kMaxDamage);

    int encode_damage
        //   4096 - 1
        = (fixed_damage < 0x00001000) ? ((fixed_damage))
        //  16384 - 1
        : (fixed_damage < 0x00004000) ? ((fixed_damage >> 1) | (0x4000))
        //  65536 - 1
        : (fixed_damage < 0x00010000) ? ((fixed_damage >> 2) | (0x8000))
        // 262144 - 1
        //(damage < 0x00040000) ?       ((fixed_damage >> 4) | (0xC000))
        :                               ((fixed_damage >> 4) | (0xC000));

    return static_cast<DAMAGETYPE>(encode_damage);
}

int ns_formula::DamageEncoder::Decode(DAMAGETYPE encoded_damage)
{
    int decode_damage
        // 262144 - 1
        = ((encoded_damage & 0xC000) == 0xC000) ? ((encoded_damage & ~0xC000) << 4)
        //  65536 - 1
        : ((encoded_damage & 0x8000) == 0x8000) ? ((encoded_damage & ~0x8000) << 2)
        //  16384 - 1
        : ((encoded_damage & 0x4000) == 0x4000) ? ((encoded_damage & ~0x4000) << 1)
        //   4096 - 1
        :                                         ((encoded_damage));
    assert(decode_damage >= 0 &&
           decode_damage <= kMaxDamage);

    return decode_damage;
}

#else //if !(defined(_NA001605_EP2_ATTRIBUTES_RENEWAL_) && USE_DAMAGE_EXTENTION)

int ns_formula::DamageEncoder::Fix(int origin_damage)
{
    int fixed_damage = origin_damage;
    if (fixed_damage < 0) {
        fixed_damage = 0;
    }
    else if (fixed_damage > kMaxDamage) {
        fixed_damage = kMaxDamage;
    }
    return fixed_damage;
}

DAMAGETYPE ns_formula::DamageEncoder::Encode(int fixed_damage)
{
    assert(fixed_damage >= 0 &&
           fixed_damage <= kMaxDamage);
    return static_cast<DAMAGETYPE>(fixed_damage);
}

int ns_formula::DamageEncoder::Decode(DAMAGETYPE encoded_damage)
{
    assert(encoded_damage >= 0 &&
           encoded_damage <= kMaxDamage);
    return encoded_damage;
}

#endif
*/
//==================================================================================================

DWORD ns_formula::GetStatPointPerLevel()
{
    return 5;
}

DWORD ns_formula::GetSkillPoint(LEVELTYPE level)
{
#ifdef _NA_0_20100901_SKILLTREE_SYSTEM
    uint temp = level % CHARACTER_SKILL_LEVEL_RATIO;
    return ((temp == 0) && (level != 0)) ? 1 : 0;
#else

    __UNUSED(level); // current (100221)
    // i.    1~50LV시 사용 공식 = (레벨/10)+1 (소수는 내림처리한다.)
    // ii.  51~135LV시 사용 공식 = (레벨/15)+3 (소수는 내림처리한다.)
    // iii. 136~200LV시 사용 공식 = (레벨/20)+5 (소수는 내림처리한다.)

    /*
    if (level<=50)
    {
        return level/10 + 1;
    }
    else if (level<=135)
    {
        return level/15 + 3;
    }
    else if (level<=200)
    {
        return level/20 + 5;
    }
    */

    // 레벨업시 무조건 스킬포인트 1씩 획득하는 것으로 수정됨
    return 1;
#endif
}

DWORD ns_formula::CalcHP(const eCHAR_TYPE char_type, const LEVELTYPE level, const short in_vitality)
{
    short vitality = in_vitality;
    if (vitality < 0)
    {
        vitality = 0;
    }
#ifdef _NA001605_EP2_ATTRIBUTES_RENEWAL_
    switch (char_type)
    {
    case eCHAR_BERSERKER:
        return (DWORD)(200 + (level - 1) * 39.0f + (vitality - 27) * 11.0f);
    case eCHAR_DRAGON:
        return (DWORD)(180 + (level - 1) * 26.0f + (vitality - 20) * 9.0f);
    case eCHAR_SHADOW:
        return (DWORD)(160 + (level - 1) * 25.0f + (vitality - 16) * 9.0f);
    case eCHAR_VALKYRIE:
        return (DWORD)(160 + (level - 1) * 25.0f + (vitality - 15) * 9.0f);
    case eCHAR_ELEMENTALIST:
        return (DWORD)(150 + (level - 1) * 23.0f + (vitality - 13) * 7.0f);
    case eCHAR_MYSTIC: // _NA_004965_20120613_NEW_CHARACTER_MYSTIC
        return (DWORD)(160 + (level - 1) * 25.0f + (vitality - 15) * 9.0f);
    case eCHAR_HELLROID: // _NA_000000_20130812_NEW_CHARACTER_HELLROID
        return (DWORD)(200 + (level - 1) * 39.0f + (vitality - 27) * 12.0f);
    case eCHAR_WITCHBLADE: // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
        return (DWORD)(180 + (level - 1) * 25.0f + (vitality - 24) * 9.0f);
    }
#else
    switch (char_type)
    {
    case eCHAR_BERSERKER:
        return (DWORD)(200 + (level - 1) * 30.0f + (vitality - 27) * 10.0f);
    case eCHAR_DRAGON:
        return (DWORD)(180 + (level - 1) * 22.0f + (vitality - 20) * 7.0f);
    case eCHAR_SHADOW:
        return (DWORD)(160 + (level - 1) * 19.0f + (vitality - 15) * 7.0f);
    case eCHAR_VALKYRIE:
        return (DWORD)(160 + (level - 1) * 19.0f + (vitality - 15) * 5.0f);
    case eCHAR_ELEMENTALIST:
        return (DWORD)(150 + (level - 1) * 18.0f + (vitality - 13) * 5.0f);
    case eCHAR_MYSTIC: // _NA_004965_20120613_NEW_CHARACTER_MYSTIC
        return (DWORD)(160 + (level - 1) * 19.0f + (vitality - 15) * 7.0f);
#ifdef _NA_000000_20130812_NEW_CHARACTER_HELLROID
    case eCHAR_HELLROID:
        return (DWORD)(200 + (level - 1) * 39.0f + (vitality - 27) * 12.0f);
#endif //_NA_000000_20130812_NEW_CHARACTER_HELLROID
#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    case eCHAR_WITCHBLADE:
        return (DWORD)(180 + (level - 1) * 25.0f + (vitality - 24) * 9.0f);
#endif //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
    }
#endif
    SASSERT(false, "Wrong Char_Type in CalcHP()");
    return 1;
}

DWORD ns_formula::CalcMP(const eCHAR_TYPE char_type, const LEVELTYPE level, const short in_spirit)
{
    short spirit = in_spirit;
    if (spirit < 0)
    {
        spirit = 0;
    }

    switch (char_type)
    {
    case eCHAR_BERSERKER:
        return (DWORD)( 20 + (level - 1) *  5.0f + (spirit-10) * 2.0f);
    case eCHAR_DRAGON:
        return (DWORD)( 25 + (level - 1) *  7.0f + (spirit-12) * 4.0f);
    case eCHAR_SHADOW:
        return (DWORD)( 40 + (level - 1) *  8.5f + (spirit-10) * 6.0f);
    case eCHAR_VALKYRIE:
        return (DWORD)( 32 + (level - 1) *  8.0f + (spirit-19) * 5.0f);
    case eCHAR_ELEMENTALIST:
        return (DWORD)(100 + (level - 1) * 25.0f + spirit * 6.0f);
    case eCHAR_MYSTIC: // _NA_004965_20120613_NEW_CHARACTER_MYSTIC
        return (DWORD)(40 + (level - 1) *  8.5f + (spirit - 10) * 6.0f);
    case eCHAR_HELLROID: //_NA_000000_20130812_NEW_CHARACTER_HELLROID
        return (DWORD)(100 + (level - 1) *  20.0f + (spirit * 6.0f));
    case eCHAR_WITCHBLADE: //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
        return (DWORD)(30 + (level - 1) *  7.0f + ((spirit-10) * 4.0f));
    }

    SASSERT(false, "Wrong Char_Type in CalcMP()");
    return 1;
}

// 각 케릭터 SD Point 공식

DWORD ns_formula::CalcSD(eCHAR_TYPE char_type, LEVELTYPE level)
{
    __UNUSED(char_type);

    float calc = static_cast<float>(level);
#ifdef _NA_004010_20120102_1201_SD_FORMULA
    calc = 200.0f + pow(calc, 3) / (calc * 2);
#else
    calc = 200.0f + (pow(calc, 3) / SDINCREASE_RATIO / calc);
#endif
    if (calc < 0)
    {
        return 0;
    }
    return static_cast<DWORD>(util::LL_Round(calc));
}

// 최소 공격력(근거리)
DWORD ns_formula::CalcMinMeleeAttackPower(
    const eCHAR_TYPE char_type, const short in_strength, const short in_dexterity)
{
    short dexterity = in_dexterity;
    if (dexterity < 0)
    {
        dexterity = 0;
    }
    short strength = in_strength;
    if (strength < 0)
    {
        strength = 0;
    }
    
#ifdef _NA_008486_20150914_TOTAL_BALANCE
    switch (char_type)
    {
    case eCHAR_DRAGON:
        return (DWORD)(strength / 5);
    case eCHAR_BERSERKER:
        return (DWORD)(strength / 6);
    case eCHAR_SHADOW:
        return (DWORD)(strength / 8);
    case eCHAR_VALKYRIE:
        return (DWORD)(strength / 8);
    case eCHAR_ELEMENTALIST:
        return (DWORD)(strength / 9);
    case eCHAR_MYSTIC: // _NA_004965_20120613_NEW_CHARACTER_MYSTIC
        return (DWORD)(strength / 9);
    case eCHAR_HELLROID: //_NA_000000_20130812_NEW_CHARACTER_HELLROID
        return (DWORD)(strength / 8);
    case eCHAR_WITCHBLADE: //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
        return (DWORD)(strength / 8);
    }
#else
    switch (char_type)
    {
    case eCHAR_DRAGON:
        return (DWORD)(strength / 5);
    case eCHAR_BERSERKER:
        return (DWORD)(strength / 6);
    case eCHAR_SHADOW:
        return (DWORD)(strength / 9 + dexterity / 12);
    case eCHAR_VALKYRIE:
        return (DWORD)(strength / 8 + dexterity / 10);
    case eCHAR_ELEMENTALIST:
        return (DWORD)(strength / 9);
    case eCHAR_MYSTIC: // _NA_004965_20120613_NEW_CHARACTER_MYSTIC
        return (DWORD)(strength / 9 + dexterity / 12);
    case eCHAR_HELLROID: //_NA_000000_20130812_NEW_CHARACTER_HELLROID
        return (DWORD)(strength / 8);
    case eCHAR_WITCHBLADE: //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
        return (DWORD)(strength / 8 + dexterity / 12);
    }
#endif //_NA_008486_20150914_TOTAL_BALANCE

    SASSERT(false, "Wrong Char_Type in CalcMP()");
    return 1;
}

#ifdef _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
extern PLAYERFP CalcFP( const eCHAR_TYPE char_type )
{
    switch (char_type)
    {
    case eCHAR_WITCHBLADE:
        return 10;
    }

    return 0;
}
#endif //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
// 최소 공격력(원거리)
DWORD ns_formula::CalcMinRangeAttackPower(
    const eCHAR_TYPE char_type, const short in_strength, const short in_dexterity)
{
    short dexterity = in_dexterity;
    if (dexterity < 0)
    {
        dexterity = 0;
    }

    short strength = in_strength;
    if (strength < 0)
    {
        strength = 0;
    }

    switch (char_type)
    {
    case eCHAR_DRAGON:
    case eCHAR_BERSERKER:
    case eCHAR_ELEMENTALIST:
        return CalcMinMeleeAttackPower(char_type, strength, dexterity);
    case eCHAR_SHADOW:
        return (DWORD)(strength / 8);
    case eCHAR_VALKYRIE:
#ifdef _NA_008486_20150914_TOTAL_BALANCE
        return (DWORD)(strength / 8);
#else
        return (DWORD)(strength / 8 + dexterity / 10);
#endif //_NA_008486_20150914_TOTAL_BALANCE
    case eCHAR_MYSTIC: // _NA_004965_20120613_NEW_CHARACTER_MYSTIC
        return (DWORD)(strength / 8);
    case eCHAR_HELLROID: // _NA_000000_20130812_NEW_CHARACTER_HELLROID
        return (DWORD)(strength / 8);
    case eCHAR_WITCHBLADE: // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
        return (DWORD)(strength / 8);
        
    }
    return 1;
}

// 최대 공격력(근거리)
DWORD ns_formula::CalcMaxMeleeAttackPower(
    const eCHAR_TYPE char_type, const short in_strength, const short in_dexterity)
{
    short dexterity = in_dexterity;
    if (dexterity < 0)
    {
        dexterity = 0;
    }

    short strength = in_strength;
    if (strength < 0)
    {
        strength = 0;
    }

#ifdef _NA_008486_20150914_TOTAL_BALANCE
    switch (char_type)
    {
    case eCHAR_DRAGON:
        return (DWORD)(strength / 3);
    case eCHAR_BERSERKER:
        return (DWORD)(strength / 2);
    case eCHAR_SHADOW:
        return (DWORD)(strength / 4);
    case eCHAR_VALKYRIE:
        return (DWORD)(strength / 4);
    case eCHAR_ELEMENTALIST:
        return (DWORD)(strength / 4);
    case eCHAR_MYSTIC: // _NA_004965_20120613_NEW_CHARACTER_MYSTIC
        return (DWORD)(strength / 6);
    case eCHAR_HELLROID: // _NA_000000_20130812_NEW_CHARACTER_HELLROID
        return (DWORD)(strength / 3);
    case eCHAR_WITCHBLADE: // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
        return (DWORD)(strength / 4);

    }
#else
    switch (char_type)
    {
    case eCHAR_DRAGON:
        return (DWORD)(strength / 3);
    case eCHAR_BERSERKER:
        return (DWORD)(strength / 2);
    case eCHAR_SHADOW:
        return (DWORD)(strength / 6 + dexterity / 8);
    case eCHAR_VALKYRIE:
        return (DWORD)(strength / 4 + dexterity / 5);
    case eCHAR_ELEMENTALIST:
        return (DWORD)(strength / 4);
    case eCHAR_MYSTIC: // _NA_004965_20120613_NEW_CHARACTER_MYSTIC
        return (DWORD)(strength / 6 + dexterity / 8);
    case eCHAR_HELLROID: // _NA_000000_20130812_NEW_CHARACTER_HELLROID
        return (DWORD)(strength / 3);
    case eCHAR_WITCHBLADE: // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
        return (DWORD)(strength / 4 + dexterity / 7);

    }
#endif //_NA_008486_20150914_TOTAL_BALANCE

    SASSERT(false, "Wrong Char_Type in CalcMaxMeleeAttackPower()");
    return 1;
}

// 최대 공격력(원거리)
DWORD ns_formula::CalcMaxRangeAttackPower(
    const eCHAR_TYPE char_type, const short in_strength, const short in_dexterity)
{
    short dexterity = in_dexterity;
    if (dexterity < 0)
    {
        dexterity = 0;
    }

    short strength = in_strength;
    if (strength < 0)
    {
        strength = 0;
    }

    switch (char_type)
    {
    case eCHAR_DRAGON:
    case eCHAR_BERSERKER:
    case eCHAR_ELEMENTALIST:
        return CalcMaxMeleeAttackPower(char_type, strength, dexterity);
    case eCHAR_SHADOW:
        return (DWORD)(strength / 4);
    case eCHAR_VALKYRIE:
#ifdef _NA_008486_20150914_TOTAL_BALANCE
        return (DWORD)(strength / 4);
#else
        return (DWORD)(strength / 4 + dexterity / 5);
#endif //_NA_008486_20150914_TOTAL_BALANCE
    case eCHAR_MYSTIC: // _NA_004965_20120613_NEW_CHARACTER_MYSTIC
        return (DWORD)(strength / 4);
    case eCHAR_HELLROID: // _NA_000000_20130812_NEW_CHARACTER_HELLROID
        return (DWORD)(strength / 3);
    case eCHAR_WITCHBLADE: // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
        return (DWORD)(strength / 4);
        
    }
    SASSERT(false, "Wrong Char_Type in CalcMaxRangeAttackPower()");
    return 1;
}

// EP2:unused, old comment, 기본 공격력(마법)
DWORD ns_formula::CalcMagicAttackPower(BOOL bMin, short intelligence)
{
    if (intelligence < 0)
    {
        intelligence = 0;
    }

    if (bMin)   return intelligence / 3;
    else        return intelligence / 2;
}

// CHANGES: f110103.4L, changes concept of a CalcMagicAttackPower.
// added by _NA001605_EP2_ATTRIBUTES_RENEWAL_
// 기본 스킬 공격력
DWORD CalcSkillAttackPower(const eCHAR_TYPE char_type, const short in_intelligence)
{
    // _NA001605_EP2_ATTRIBUTES_RENEWAL_
    short intelligence = in_intelligence;
    if (intelligence < 0) 
    {
        intelligence = 0; // NOTE: set dummy penalty value if an invalid attributes value.
    }
    if (char_type < eCHAR_BERSERKER || char_type >= eCHAR_TYPE_MAX) 
    {
        return 0;
    }
    double ratio_of_intelligence_to_skill_attack_power = \
        ns_formula::kSkillAttackPowerConstants.ratio_of_conversion[char_type];
    //
    // formula
    //                         ratio_of_intelligence_to_skill_attack_power
    // BaseSkillAttackPower = _____________________________________________ * intelligence
    //                                            40
#ifdef _NA_008486_20150914_TOTAL_BALANCE
    double skill_attack_power = ratio_of_intelligence_to_skill_attack_power / 40 * intelligence;
#else
    double skill_attack_power = ratio_of_intelligence_to_skill_attack_power / 100 * intelligence;
#endif //_NA_008486_20150914_TOTAL_BALANCE
    return util::LL_Round(skill_attack_power);
}

DWORD ns_formula::CalcPhyBaseDef(const eCHAR_TYPE char_type, const short in_vitality)
{
    short vitality = in_vitality;
    if (vitality < 0)
    {
        vitality = 0;
    }

    switch (char_type)
    {
    case eCHAR_DRAGON:
        return (DWORD)(vitality / 5 + 29);
    case eCHAR_BERSERKER:
        return (DWORD)(vitality / 5 + 29);
    case eCHAR_SHADOW:
        return (DWORD)(vitality / 7 + 29);
    case eCHAR_VALKYRIE:
        return (DWORD)(vitality / 6 + 29);
    case eCHAR_ELEMENTALIST:
        return (DWORD)(vitality / 10 +29);
    case eCHAR_MYSTIC: // _NA_004965_20120613_NEW_CHARACTER_MYSTIC
        return (DWORD)(vitality / 7 + 29);
    case eCHAR_HELLROID: // _NA_000000_20130812_NEW_CHARACTER_HELLROID
        return (DWORD)((vitality / 5) + 29);
    case eCHAR_WITCHBLADE: // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
        return (DWORD)((vitality / 7) + 29);
        
    }

    SASSERT(false, "Wrong Char_Type in CalcPhyDef()");
    return 1;
}

// EP2:unused, 각 속성 별로 분리해야 한다.
DWORD ns_formula::CalcMagicBaseDef(const eCHAR_TYPE char_type, const short in_spirit)
{
    short spirit = in_spirit;
    if (spirit < 0)
    {
        spirit = 0;
    }

    switch (char_type)
    {
    case eCHAR_DRAGON:
        return (DWORD)(spirit / 9);
    case eCHAR_BERSERKER:
        return (DWORD)(spirit / 9);
    case eCHAR_SHADOW:
        return (DWORD)(spirit / 8);
    case eCHAR_VALKYRIE:
        return (DWORD)(spirit / 8);
    case eCHAR_ELEMENTALIST:
        return (DWORD)(spirit / 7);
    case eCHAR_MYSTIC: // _NA_004965_20120613_NEW_CHARACTER_MYSTIC
        return (DWORD)(spirit / 8);
    case eCHAR_HELLROID: // _NA_000000_20130812_NEW_CHARACTER_HELLROID
        return (DWORD)(spirit / 8);
    case eCHAR_WITCHBLADE: // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
        return (DWORD)(spirit / 8);
        
    }

    SASSERT(false, "Wrong Char_Type in CalcMagicDef()");
    return 1;
}

#ifdef _NA_008486_20150914_TOTAL_BALANCE
int ns_formula::CalcMoveSpeedRatio(int item_move_speed, int skill_move_speed, 
                                   LEVELTYPE char_level)
{
    return internal::CalcMoveSpeedRatio(item_move_speed, skill_move_speed, char_level);
}
#else
int ns_formula::CalcMoveSpeedRatio(short dexterity, int item_move_speed, int skill_move_speed)
{
    return internal::CalcMoveSpeedRatio(dexterity, item_move_speed, skill_move_speed);
}
#endif //_NA_008486_20150914_TOTAL_BALANCE

DWORD ns_formula::CalcPhyCriticalBaseRatio(
    const eCHAR_TYPE char_type, const LEVELTYPE level, const short in_dexterity)
{
    // CHANGES: f110103.2L, change critical occurrence probability
#ifdef _NA001605_EP2_ATTRIBUTES_RENEWAL_
    //
    __UNUSED(level);
    if (char_type < eCHAR_BERSERKER || char_type >= eCHAR_TYPE_MAX) 
    {
        return 0;
    }
    // change boundary
    short dexterity = in_dexterity;
    if (dexterity <= 0) 
    {
        return 0;
    };
    typedef BalanceConstantsChangerInProgramCommon Constants;
    double default_critical_probability = \
        ns_formula::kCriticalChanceConstants.base_probabilies[char_type];

    double dex = static_cast<double>(dexterity);
    const double kNumberatorWeight = ns_formula::kCriticalChanceConstants.kNumberatorWeight;
    const double kDenominatorWeight = ns_formula::kCriticalChanceConstants.kDenominatorWeight;
    // formula
    //                       (Dex * kNumberatorWeight)
    //  critical chance = ______________________________ + default_critical_probability
    //                    sqrt(Dex) * kDenominatorWeight
    double result = (dex * kNumberatorWeight) /
                    (sqrt(dex) * kDenominatorWeight);
    ;      result += default_critical_probability;

    return util::LL_Round(result);
#else
    float phyValue = 0;

    if (dexterity < 0) 
    {
        dexterity = 0;
    }

    switch (char_type)
    {
    case eCHAR_DRAGON:   
        phyValue = 2.0f;    
        break;
    case eCHAR_BERSERKER:   
        phyValue = 2.0f;    
        break;
    case eCHAR_SHADOW:   
        phyValue = 3.0f;    
        break;
    case eCHAR_VALKYRIE:   
        phyValue = 3.0f;    
        break;
    case eCHAR_ELEMENTALIST:   
        phyValue = 4.0f;    
        break;
    case eCHAR_MYSTIC: // _NA_004965_20120613_NEW_CHARACTER_MYSTIC
        phyValue = 3.0f;    
        break;
    case eCHAR_HELLROID: // _NA_000000_20130812_NEW_CHARACTER_HELLROID
        phyValue = 3.0f;
        break;
    case eCHAR_WITCHBLADE: // _NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
        phyValue = 3.0f;
        break;

    default:
        return 0;
    }

    return (DWORD)(level / 4 + dexterity / phyValue);
#endif
}

DWORD ns_formula::CalcMagicCriticalBaseRatio(
    const eCHAR_TYPE char_type, const LEVELTYPE level, const short in_spirit)
{
    DWORD phyValue = 0;

    short spirit = in_spirit;
    if (spirit < 0) 
    {
        spirit = 0;
    }

    switch (char_type)
    {
    case eCHAR_DRAGON:   
        phyValue = 2;   
        break;
    case eCHAR_BERSERKER:   
        phyValue = 2;   
        break;
    case eCHAR_SHADOW:   
        phyValue = 2;   
        break;
    case eCHAR_VALKYRIE:   
        phyValue = 2;   
        break;
    case eCHAR_ELEMENTALIST:   
        phyValue = 2;   
        break;
    case eCHAR_MYSTIC: // _NA_004965_20120613_NEW_CHARACTER_MYSTIC
        phyValue = 2;   
        break;
    case eCHAR_HELLROID: //_NA_000000_20130812_NEW_CHARACTER_HELLROID
        phyValue = 2;
        break;
    case eCHAR_WITCHBLADE: //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
        phyValue = 2;
        break;
        
    default:
        return 0;
    }

    return (level / 4 + spirit / phyValue);
}

// 공격 성공률
DWORD ns_formula::CalcPhysicalAttackRateBase(
    const eCHAR_TYPE char_type, const LEVELTYPE level, const short in_dexterity)
{
#ifdef _NA001605_EP2_ATTRIBUTES_RENEWAL_
    __UNUSED(level);
    if (char_type < eCHAR_BERSERKER || char_type >= eCHAR_TYPE_MAX) 
    {
        return 0;
    }
    short dexterity = in_dexterity;
    if (dexterity < 0) 
    {
        dexterity = 0;
    };
    // [Items...] + [Constant] * [Dex]
    // [Constant] =   0.0004   *  100 -> 4%
    // percentage base = 1000 * 4.0(x10000) * [Dex] / 10
    double calc = static_cast<double>(dexterity);
    ;      calc *= ns_formula::kAttackRateConstants.ratio_of_conversion[char_type];
    ;      calc /= 100.0f; // make to percentage
    return util::LL_Round(calc);
#else
    float rateValue = 0;

    short dexterity = in_dexterity;
    if (dexterity < 0)
    {
        dexterity = 0;
    }

    switch (char_type)
    {
    case eCHAR_DRAGON       : rateValue = 2.0f; break;
    case eCHAR_BERSERKER    : rateValue = 1.0f; break;
    case eCHAR_SHADOW       : rateValue = 3.0f; break;
    case eCHAR_VALKYRIE     : rateValue = 5.0f; break;
    case eCHAR_ELEMENTALIST : rateValue = 1.0f; break;
    case eCHAR_MYSTIC       : rateValue = 3.0f; break; // _NA_004965_20120613_NEW_CHARACTER_MYSTIC
    case eCHAR_HELLROID     : rateValue = 3.0f; break; //_NA_000000_20130812_NEW_CHARACTER_HELLROID
    case eCHAR_WITCHBLADE   : rateValue = 3.0f; break; //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE
        
    default:
        {
            SASSERT(false, "Wrong Char_Type in CalcPhysicalAttackRateBase()");
            rateValue = 1.0f;
        }
        break;
    }

    return (DWORD)(50 + level * 10 + dexterity * rateValue);
#endif
}

int ns_formula::CalcAttackSpeedRatio(eCHAR_TYPE char_type, DWORD weapon_kind,
                                     short dexterity, int item_attr_speed, int skill_attr_speed)
{
    return internal::CalcAttackSpeedRatio(char_type, weapon_kind,
        dexterity, item_attr_speed, skill_attr_speed);
}


// HP 배율수
static float ns_formula::CalcHPRatioNum(DWORD partymemberNum)
{
#ifdef _NA001605_EP2_ATTRIBUTES_RENEWAL_
    if (partymemberNum == 0)    return 0;
    else                        return 0.032f; // 0.4f -> 0.032f
#else
    if (partymemberNum == 0)    return 0;
    else                        return 0.7f;
#endif
}

// HP 추가비율
static float ns_formula::CalcAdditionalHPRatio(DWORD partymemberNum)
{
#ifdef _NA001605_EP2_ATTRIBUTES_RENEWAL_
    const float kAdditionalRatioOfRatio = 0.072f; // 0.09f -> 0.072f
    float additional_ratio = 0.0f;
    if (partymemberNum != 0) {
        additional_ratio = (partymemberNum - 1) * kAdditionalRatioOfRatio;
    }
    return additional_ratio;
#else
    float fAdditionalRatio = 0.0f;
    const float kAdditionalRatioOfRatio = 0.10f;

    if (partymemberNum == 0) {
        fAdditionalRatio = 0;
    }
    else if (partymemberNum <= 5) {
        fAdditionalRatio = (partymemberNum - 1) * kAdditionalRatioOfRatio;
    }
    else {
        fAdditionalRatio = CalcAdditionalHPRatio(partymemberNum - 1) +
                           (partymemberNum - 4) * kAdditionalRatioOfRatio;
    }

    return fAdditionalRatio;
#endif
}


// 파티원 수에 따른 NPC HP 증가율
float ns_formula::CalcHPIncreaseRatioAsParty(DWORD partymemberNum)
{
#ifdef _NA001605_EP2_ATTRIBUTES_RENEWAL_
    float result_ratio = 1.0f;
    ;     result_ratio += (partymemberNum - 1) * CalcHPRatioNum(partymemberNum);
    ;     result_ratio += CalcAdditionalHPRatio(partymemberNum);
    return result_ratio;
#else
    float fResultRatio = 1 +
                         (float)(partymemberNum - 1) * CalcHPRatioNum(partymemberNum) +
                         CalcAdditionalHPRatio(partymemberNum);

    return fResultRatio;
#endif
}

// 파티원 수에 따른 NPC 공격력 증가수치
int ns_formula::CalcAttackPowerIncreaseAsParty(DWORD partymemberNum, int iAttackPower)
{
    int iIncreaseAttackPower = 0;
    iIncreaseAttackPower = (int)(GetBonusAttackPower(partymemberNum) +
        iAttackPower * (partymemberNum - 1) * GetBonusAttPowerRatio(partymemberNum));

    return iIncreaseAttackPower;
}

// 공격회피율(물리)
#ifdef _NA_008486_20150914_TOTAL_BALANCE//---------------------------------------------------
DWORD ns_formula::CalcPhysicalAvoidRateBase(
    const eCHAR_TYPE char_type, const LEVELTYPE level, const short in_vitality)
{
    __UNUSED(level);
    if (char_type < eCHAR_BERSERKER || char_type >= eCHAR_TYPE_MAX) 
    {
        return 0;
    }

    short vitality = in_vitality;
    if (vitality < 0) 
    {
        vitality = 0;
    };
    // [Items...] + [Constant] * [vital]
    // [Constant] =   0.0008   *  100 -> 4%
    // percentage base = 1000 * 8.0(x10000) * [Dex] / 10
    double calc = static_cast<double>(vitality);
    ;      calc *= ns_formula::kAttackAvoidRateConstants.ratio_of_conversion[char_type];
    ;      calc /= 100.0f; // make to percentage
    return util::LL_Round(calc);
}

// 디버프 지속시간
INT ns_formula::CalcDebuffDuration(const short in_spirit)
{
    short spirit = in_spirit;
    if (spirit < 0) {
        spirit = 0;
    }

    //전직업 공통
    //정신력 1당 0.0012초(2밀리세컨드)
    double calc = static_cast<double>(spirit);
    calc *= 1.2f;

    return util::LL_Round(calc);
}
#else //--------------------------------------------------------------------------------------------

DWORD ns_formula::CalcPhysicalAvoidRateBase(
    const eCHAR_TYPE char_type, const LEVELTYPE level, const short in_dexterity)
{
#ifdef _NA001605_EP2_ATTRIBUTES_RENEWAL_
    __UNUSED(level);
    if (char_type < eCHAR_BERSERKER || char_type >= eCHAR_TYPE_MAX) 
    {
        return 0;
    }

    short dexterity = in_dexterity;
    if (dexterity < 0) 
    {
        dexterity = 0;
    };
    // [Items...] + [Constant] * [Dex]
    // [Constant] =   0.0008   *  100 -> 4%
    // percentage base = 1000 * 8.0(x10000) * [Dex] / 10
    double calc = static_cast<double>(dexterity);
    ;      calc *= ns_formula::kAttackAvoidRateConstants.ratio_of_conversion[char_type];
    ;      calc /= 100.0f; // make to percentage
    return util::LL_Round(calc);
#else
    float fRateValue = 1;

    short dexterity = in_dexterity;
    if (dexterity < 0)
    {
        dexterity = 0;
    }

    switch (char_type)
    {
    case eCHAR_DRAGON       : fRateValue = 1.0f; break;
    case eCHAR_BERSERKER    : fRateValue = 1.0f; break;
    case eCHAR_SHADOW       : fRateValue = 1.0f; break;
    case eCHAR_VALKYRIE     : fRateValue = 1.0f; break;
    case eCHAR_ELEMENTALIST : fRateValue = 1.0f; break;
    case eCHAR_MYSTIC       : fRateValue = 1.0f; break; // _NA_004965_20120613_NEW_CHARACTER_MYSTIC
    case eCHAR_HELLROID     : fRateValue = 1.0f; break; //_NA_000000_20130812_NEW_CHARACTER_HELLROID
    case eCHAR_WITCHBLADE   : fRateValue = 1.0f; break; //_NA_007514_20140828_NEW_CHARACTER_WITCHBLADE

    default:
        {
            SASSERT(false, "Wrong Char_Type in CalcPhysicalAvoidRateBase()");
            fRateValue = 1.0f;
        }
        break;
    }

    return (DWORD)(level + dexterity);
#endif
}
#endif //_NA_008486_20150914_TOTAL_BALANCE
}; //end of namespace 'ns_formula'
//==================================================================================================
