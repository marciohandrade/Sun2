#include "StdAfx.h"
#include "ResurrectionAbility.h"


bool ResurrectionAbility::Execute(Character* target, BYTE*, WORD&)
{
    Character* const attacker = GetAttacker();
    if (attacker == NULL)
    {
        return false;
    }

    if ((attacker == target) && (attacker->IsEqualObjectKind(PLAYER_OBJECT) == false))
    {
        return false;
    }
    
    const BASE_ABILITYINFO* const ability_info = GetBaseAbilityInfo();
    const float recover_exp = ability_info->m_iOption2 / 1000.0f;
    const float recover_hp = ability_info->m_iParam[0] / 1000.0f;
    const float recover_mp = ability_info->m_iParam[1] / 1000.0f;

    if (!target->OnResurrection(recover_exp, recover_hp, recover_mp, attacker->GetPlayerOwner()))
    {
        return false;
    }
    
    return false;
}