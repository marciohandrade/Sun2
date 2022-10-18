#include "StdAfx.h"
#include "WeaponMasteryAbility.h"
#include "SkillAttrCalculator.h"


bool WeaponMasteryAbility::Execute(Character* /*target*/, BYTE* /*msg*/, WORD& /*msg_size*/)
{
    Character* const attacker = GetAttacker();
    if (attacker == NULL)
    {
        return false;
    }

    SkillAttrCalculator calc(attacker->GetAttr());

    // 공격력 증가
    const BASE_ABILITYINFO* const ability_info = GetBaseAbilityInfo();
    const int add_attack_power = ability_info->m_iParam[0];
    if (add_attack_power != 0)
    {
        calc.AddAttr(eATTR_OPTION_PHYSICAL_ATTACK_POWER, VALUE_TYPE_VALUE, add_attack_power);
    }
    
    // 공격 속도 증가
    const int add_attack_speed = ability_info->m_iParam[1];
    if (add_attack_speed != 0)
    {
        calc.AddAttr(eATTR_ATTACK_SPEED, VALUE_TYPE_VALUE, add_attack_speed);
    }
    
    return false;
}

void WeaponMasteryAbility::Release(Character* /*target*/)
{
    Character* const attacker = GetAttacker();
    if (attacker == NULL)
    {
        return;
    }

    SkillAttrCalculator calc(attacker->GetAttr());

    // 공격력 감소
    const BASE_ABILITYINFO* const ability_info = GetBaseAbilityInfo();
    const int del_attack_power = ability_info->m_iParam[0];
    if (del_attack_power != 0)
    {
        calc.DelAttr(eATTR_OPTION_PHYSICAL_ATTACK_POWER, VALUE_TYPE_VALUE, del_attack_power);
    }
    
    // 공격 속도 감소
    const int del_attack_speed = ability_info->m_iParam[1];
    if (del_attack_speed != 0)
    {
        calc.DelAttr(eATTR_ATTACK_SPEED, VALUE_TYPE_VALUE, del_attack_speed);
    }
}