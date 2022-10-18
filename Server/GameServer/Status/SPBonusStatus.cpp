#include "stdafx.h"
#include "Ability.h"
#include "SPBonusStatus.h"
#include "SkillAttrCalculator.h"


void SPBonusStatus::Start()
{
    Character* const owner = GetOwner();
    if (owner == NULL)
    {
        return;
    }

    SkillAttrCalculator calc(owner->GetAttr());

    const int add_value = GetAbilityValueType();
    if (add_value != 0)
    {
        calc.AddAttr(eATTR_OPTION_PHYSICAL_ATTACK_POWER, VALUE_TYPE_VALUE, add_value);
    }
    
    const int add_ratio = GetAbilityValue();
    if (add_ratio != 0)
    {
        calc.AddAttr(eATTR_OPTION_PHYSICAL_ATTACK_POWER, VALUE_TYPE_PERCENT_PER_CUR, add_ratio);
    }
}

void SPBonusStatus::End()
{
    Character* const owner = GetOwner();
    if (owner == NULL)
    {
        return;
    }

    SkillAttrCalculator calc(owner->GetAttr());

    const int add_value = GetAbilityValueType();
    if (add_value != 0)
    {
        calc.DelAttr( eATTR_OPTION_PHYSICAL_ATTACK_POWER, VALUE_TYPE_VALUE, add_value);
    }

    const int add_ratio = GetAbilityValue();
    if (add_ratio != 0)
    {
        calc.DelAttr(eATTR_OPTION_PHYSICAL_ATTACK_POWER, VALUE_TYPE_PERCENT_PER_CUR, add_ratio);
    }

    AbilityStatus::End();
}