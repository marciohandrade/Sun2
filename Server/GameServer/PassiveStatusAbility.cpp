#include "StdAfx.h"
#include "PassiveStatusAbility.h"
#include "SkillAttrCalculator.h"


bool PassiveStatusAbility::Execute(Character* /*target*/, BYTE* /*msg*/, WORD& /*msg_size*/)
{
    Character* const attacker = GetAttacker();
    if (attacker == NULL)
    {
        return false;
    }

    const eATTR_TYPE attr_id = GetAttrID();
    if (attr_id == eATTR_TYPE_INVALID)
    {
        return false;
    }

    const BASE_ABILITYINFO* const ability_info = GetBaseAbilityInfo();
    const int add_value = ability_info->m_iParam[1];
    if (add_value == 0)
    {
        return false;
    }

    SkillAttrCalculator calc(attacker->GetAttr());
    calc.AddAttr(attr_id, ability_info->m_iParam[0], add_value);

    return false;
}

void PassiveStatusAbility::Release(Character* /*target*/)
{
    Character* const attacker = GetAttacker();
    if (attacker == NULL)
    {
        return;
    }

    const eATTR_TYPE attr_id = GetAttrID();
    if (attr_id == eATTR_TYPE_INVALID)
    {
        return;
    }

    const BASE_ABILITYINFO* const ability_info = GetBaseAbilityInfo();
    const int del_value = ability_info->m_iParam[1];
    if (del_value == 0)
    {
        return;
    }

    SkillAttrCalculator calc(attacker->GetAttr());
    calc.DelAttr(attr_id, ability_info->m_iParam[0], del_value);
}