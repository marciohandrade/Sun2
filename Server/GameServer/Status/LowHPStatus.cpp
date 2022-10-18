#include "stdafx.h"
#include "Ability.h"
#include "LowHPStatus.h"
#include "SkillAttrCalculator.h"


void LowHPStatus::Init(Character* owner, Character* attacker, Ability* ability)
{
	AbilityStatus::Init(owner, attacker, ability);

	const BASE_ABILITYINFO* const ability_info = ability->GetBaseAbilityInfo();
	hp_ratio_ = ability_info->m_iOption2 / 1000.f;
}

void LowHPStatus::Start()
{
	UpdateLowHP();
}

void LowHPStatus::End()
{
	hp_ratio_ = 0;
	UpdateLowHP();

	AbilityStatus::End();
}

void LowHPStatus::UpdateLowHP()
{
    Character* const owner = GetOwner();
    if (owner == NULL)
    {
        return;
    }

	const float hp_ratio = (float)owner->GetHP() / owner->GetMaxHP();
    const eATTR_TYPE attr_id = GetAttrID();

	if (IsApply())
	{
		if (owner->IsDead() || hp_ratio > hp_ratio_)
		{
            const int ability_value = GetAbilityValue();

			SkillAttrCalculator calc(owner->GetAttr());
			calc.DelAttr(attr_id, GetAbilityValueType(), ability_value);
			SetApply(false);
            
			owner->SendAttrChange(attr_id, ability_value);
		}
	}
	else
	{
		if (!owner->IsDead() && hp_ratio <= hp_ratio_)
		{
            const int ability_value = GetAbilityValue();

			SkillAttrCalculator calc(owner->GetAttr());
			calc.AddAttr(attr_id, GetAbilityValueType(), ability_value);
			SetApply(true);
            
			owner->SendAttrChange(attr_id, ability_value);
		}
	}
}