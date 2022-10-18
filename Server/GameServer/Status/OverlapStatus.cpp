#include "stdafx.h"
#include "OverlapStatus.h"
#include "Ability.h"

void OverlapStatus::Init( Character* owner, Character* attacker, Ability* ability )
{
    AbilityStatus::Init(owner, attacker, ability);

    const BASE_ABILITYINFO* base_ability_info = ability->GetBaseAbilityInfo();
    if (base_ability_info != NULL)
    {
        max_overlap_count_ = base_ability_info->m_iOption2;
        ++current_overlap_count_;   // ±âº»°ª 1
    }
    
}

bool OverlapStatus::StatusOverlap()
{
    if (current_overlap_count_ < max_overlap_count_) 
    {
        ++current_overlap_count_;
        ResetStatus();
        return true;
    }
    return false;
}