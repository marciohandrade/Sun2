#include "stdafx.h"
#include "Ability.h"
#include "BonusDamageStatus.h"

void BonusDamageStatus::Init(Character* owner, Character* attacker, Ability* ability)
{
    AbilityStatus::Init(owner, attacker, ability);

    const BASE_ABILITYINFO* const ability_info = ability->GetBaseAbilityInfo();
    attack_type_ = (eATTACK_TYPE)ability_info->m_iOption1;
}

void BonusDamageStatus::AddDamage(eATTACK_TYPE attack_type, DAMAGETYPE& damage)
{
    if (attack_type_ != eATTACK_TYPE_ALL_OPTION)
    {
        if (attack_type_ == eATTACK_TYPE_PHYSICAL_OPTION)
        {
            if (attack_type != eATTACK_TYPE_MELEE && attack_type != eATTACK_TYPE_RANGE)
            {
                return;
            }
        }
        else if (attack_type_ == eATTACK_TYPE_MAGIC_OPTION)
        {
            if (attack_type == eATTACK_TYPE_MELEE || attack_type == eATTACK_TYPE_RANGE)
            {
                return;
            }
        }
        else
        {
            if (attack_type_ != attack_type)
            {
                return;
            }
        }
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
    damage += calc_value;
}