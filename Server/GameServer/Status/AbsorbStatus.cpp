#include "stdafx.h"
#include "Ability.h"
#include "AbsorbStatus.h"


void AbsorbStatus::AttackAbsorb(DAMAGETYPE damage)
{
#ifdef _NA_008486_20150914_TOTAL_BALANCE
#else
    Character* const owner = GetOwner();
    if (owner == NULL)
    {
        return;
    }

    int calc_value = GetAbilityValue();
    {
        const int value_type = GetAbilityValueType();
        if (value_type == VALUE_TYPE_PERCENT_PER_MAX)
        {
            calc_value = damage * calc_value / 1000;
        }
        else if (value_type == VALUE_TYPE_PERCENT_PER_CUR)
        {
            calc_value = damage * calc_value / 1000;
        }
    }

    const eABILITY ability_id = GetAbilityID();
    if (ability_id == eABILITY_ATTACK_DAMAGE_HP_ABSORPTION)
    {
        owner->OnRecover(calc_value, 0);
    }
    else if (ability_id == eABILITY_ATTACK_DAMAGE_MP_ABSORPTION)
    {
        owner->OnRecover(0, calc_value);
    }
#endif //_NA_008486_20150914_TOTAL_BALANCE
}

void AbsorbStatus::AttackedAbsorb(eATTACK_TYPE /*attack_type*/, DAMAGETYPE damage)
{
    Character* const owner = GetOwner();
    if (owner == NULL)
    {
        return;
    }

    int calc_value = GetAbilityValue();
    {
        const int value_type = GetAbilityValueType();
        if (value_type == VALUE_TYPE_PERCENT_PER_MAX)
        {
            calc_value = damage * calc_value / 1000;
        }
        else if (value_type == VALUE_TYPE_PERCENT_PER_CUR)
        {
            calc_value = damage * calc_value / 1000;
        }
    }
        
    const eABILITY ability_id = GetAbilityID();
    if (ability_id == eABILITY_ATTACKED_DAMAGE_HP_ABSORPTION)
    {
        owner->OnRecover(calc_value, 0);
    }
    else if (ability_id == eABILITY_ATTACKED_DAMAGE_MP_ABSORPTION)
    {
        owner->OnRecover(0, calc_value);
    }
}