#include "stdafx.h"
#include "AutoCastStatus.h"
#ifdef _NA_007667_20141001_WITCHBLADE_SKILL
#include "Ability.h"
#include "SkillManager.h"

VOID AutoCastStatus::Init( Character *pOwner, Character *pAttacker, Ability *pAbility )
{
    AbilityStatus::Init( pOwner, pAttacker, pAbility );

    const BASE_ABILITYINFO *pBaseAbilityInfo = pAbility->GetBaseAbilityInfo();
    m_iOption2 = pBaseAbilityInfo->m_iOption2;
}

void AutoCastStatus::UseSkillByAttack(Character* target)
{
    Character* const owner = GetOwner();
    if (owner == NULL) {
        return;
    }
    const BASE_ABILITYINFO* ability_info = GetAbilityInfo();

    bool is_success = random_percent(ability_info->m_wSuccessRate);
    if (is_success == true) {
        owner->GetSkillManager()->SetAutoCastSkill(m_iOption2, true, target);       
    }

    return;
}
#endif //_NA_007667_20141001_WITCHBLADE_SKILL