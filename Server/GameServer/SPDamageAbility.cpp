#include "StdAfx.h"
#include "SPDamageAbility.h"
#include "SkillAttrCalculator.h"


bool SPDamageAbility::Execute(Character* /*target*/, BYTE* /*msg*/, WORD& /*msg_size*/)
{
    Character* const attacker = GetAttacker();
    if (attacker == NULL)
    {
        return false;
    }

    SkillAttrCalculator calc(attacker->GetAttr());

    // ���� ���ݷ��� ��ġ��ŭ �÷��ش�.
    const BASE_ABILITYINFO* const ability_info = GetBaseAbilityInfo();
    const int add_value = ability_info->m_iParam[0];
    if (add_value != 0)
    {
        calc.AddAttr(eATTR_OPTION_PHYSICAL_ATTACK_POWER, VALUE_TYPE_VALUE, add_value);
    }
    
    // ���� ���ݷ��� ������ŭ �÷��ش�.
    const int add_ratio = ability_info->m_iParam[1];
    if (add_ratio != 0)
    {
        calc.AddAttr(eATTR_OPTION_PHYSICAL_ATTACK_POWER, VALUE_TYPE_PERCENT_PER_CUR, add_ratio);
    }
    
    return false;
}

void SPDamageAbility::Release(Character* /*target*/)
{
    Character* const attacker = GetAttacker();
    if (attacker == NULL)
    {
        return;
    }

    SkillAttrCalculator calc(attacker->GetAttr());

    // ���� ���ݷ��� ��ġ��ŭ �����ش�.
    const BASE_ABILITYINFO* const ability_info = GetBaseAbilityInfo();
    const int del_value = ability_info->m_iParam[0];
    if (del_value != 0)
    {
        calc.DelAttr(eATTR_OPTION_PHYSICAL_ATTACK_POWER, VALUE_TYPE_VALUE, del_value);
    }

    // ���� ���ݷ��� ������ŭ �����ش�.
    const int del_ratio = ability_info->m_iParam[1];
    if (del_ratio != 0)
    {
        calc.DelAttr(eATTR_OPTION_PHYSICAL_ATTACK_POWER, VALUE_TYPE_PERCENT_PER_CUR, del_ratio);
    }
}