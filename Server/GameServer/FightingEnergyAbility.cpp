#include "StdAfx.h"
#include "FightingEnergyAbility.h"
#include "Skill.h"

bool FightingEnergyAbility::IsValidStateID() const
{
    if (GetAbilityID() == eABILITY_FIGHTING_ENERGY_NUM_INCREASE && 
        GetStateID() == eCHAR_STATE_INVALID)
    {
        return false;
    }
    return true;
}

bool FightingEnergyAbility::Execute(Character* target, BYTE* msg, WORD& msg_size)
{
    if (Ability::Execute(target, msg, msg_size) == false)
    {
        return false;
    }
    
    Character* const attacker = GetAttacker();
    if (attacker == NULL)
    {
        return false;
    }

    const BASE_ABILITYINFO* const ability_info = GetBaseAbilityInfo();

    // 투기 개수 증가
    if (GetAbilityID() == eABILITY_FIGHTING_ENERGY_NUM_INCREASE)
    {
        if (attacker->GetFightingEnergy() >= MAX_FIGHTING_ENERGY_NUM)
        {
            return false;
        }
        
        const int increse_count = ability_info->m_iParam[1];
        if (increse_count == 0)
        {
            return false;
        }
        attacker->AddFightingEnergy(increse_count);

        if (msg != NULL)
        {
            SKILL_RESULT_FIGHTING_ENERGY* const fighting_energy_msg = 
                (SKILL_RESULT_FIGHTING_ENERGY*)msg;
            fighting_energy_msg->m_wStatusCode = GetStateID();
            fighting_energy_msg->m_iStatusTime = -1;
        }
    }
    // 투기 데미지 추가
    else if (GetAbilityID() == eABILITY_BONUS_DAMAGE_PER_FIGHTING_ENERGY)
    {
        attacker->SetUsedFightingEnergySize(0);
        
        short decrese_count = static_cast<short>(ability_info->m_iOption2);
        const bool is_use_all = (decrese_count == 0);
        if (is_use_all)
        {
            const short fighting_energy_size = attacker->GetFightingEnergy();
            if (fighting_energy_size == 0)
            {
                return false;
            }
            decrese_count = fighting_energy_size;
        }
        if (attacker->GetFightingEnergy() < decrese_count)
        {
            return false;
        }
        
        attacker->DelFightingEnergy(decrese_count);
        attacker->SetUsedFightingEnergySize(decrese_count);
    }

    if (msg != NULL)
    {
        SKILL_RESULT_FIGHTING_ENERGY* const fighting_energy_msg = 
            (SKILL_RESULT_FIGHTING_ENERGY*)msg;
        fighting_energy_msg->m_sFightingEnergyCount = attacker->GetFightingEnergy();
        msg_size = fighting_energy_msg->GetSize();
    }

    return true;
}