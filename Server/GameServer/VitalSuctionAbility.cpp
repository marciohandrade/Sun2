#include "StdAfx.h"
#include "VitalSuctionAbility.h"
#include "StateInfoParser.h"
#include "StatusManager.h"
#include "Battle.h"
#include "CharacterFormula.h"


VitalSuctionAbility::VitalSuctionAbility() : 
    suction_count_(0), total_suction_hp_(0)
{
}

VitalSuctionAbility::~VitalSuctionAbility()
{
}

void VitalSuctionAbility::Init(Skill* skill, const BASE_ABILITYINFO* ability_info)
{
    Ability::Init(skill, ability_info);

    suction_count_ = 0;
    total_suction_hp_ = 0;
    suction_hp_ = 0;
}

bool VitalSuctionAbility::CanExecute(
    Character* attacker, Character* target, DWORD main_target_key, WzVector /*main_target_pos*/)
{
    const BASE_ABILITYINFO* const ability_info = GetBaseAbilityInfo();
    if (ability_info == NULL)
    {
        return false;
    }
    if (target->IsEqualObjectKind(MONSTER_OBJECT) == false)
    {
        return false;
    }
            
    bool is_success_rate = false;
    {
        const BASE_STATEINFO* const state_info = StateInfoParser::Instance()->GetStateInfo(GetStateID());
        if (state_info != NULL)
        {
            const StatusManager* const target_status_manager = target->GetStatusManager();
            if (state_info->m_byType == eSTATE_TYPE_ABNORMAL || 
                state_info->m_byType == eSTATE_TYPE_WEAKENING)
            {
                is_success_rate = true;
#ifdef _NA_008486_20150914_TOTAL_BALANCE
                if (IsStatusHit(attacker, target, ability_info->m_wSuccessRate, GetStateID(), GetSKill()) == false ||
                    target_status_manager->FindStatus(eCHAR_STATE_PROTECTION) ||
                    target_status_manager->isImmunityDamageState(attacker)) {
                        return false;
                }
#else
                if (IsStatusHit(attacker, target, ability_info->m_wSuccessRate, GetStateID(), GetSKill()) == false ||
                    target_status_manager->FindStatus(eCHAR_STATE_PROTECTION) ||
                    target_status_manager->isImmunityDamageState()) {
                        return false;
                }
#endif //_NA_008486_20150914_TOTAL_BALANCE
            }
            else if (state_info->m_byType == eSTATE_TYPE_SPECIALITY) 
            {
                is_success_rate = true;
#ifdef _NA_008486_20150914_TOTAL_BALANCE
                if (target_status_manager->isImmunityDamageState(attacker)) {
                    return false;
                }
#else
                if (target_status_manager->isImmunityDamageState()) {
                    return false;
                }
#endif //_NA_008486_20150914_TOTAL_BALANCE
            }
        }
        if (is_success_rate == false)
        {
            if (!DrawLots((BYTE)(ability_info->m_wSuccessRate / 10)))
            {
                return false;
            }
        }
    }
    
    if (CheckAbilityRange(attacker, target, main_target_key, ability_info->m_byRangeType) == false)
    {
        return false;
    }

    if (suction_count_ > ability_info->m_iParam[1])
    {
        return false;
    }
        
    if (target->GetDeadType() == eCHAR_DEAD_SUCTION)
    {
        return false;
    }

    const DWORD max_suction_hp = ability_info->m_iParam[0];
    suction_hp_ = (double)target->GetMaxHP() * ((double)GetBaseAbilityInfo()->m_iOption2 / 1000.0f);

    if (total_suction_hp_ + suction_hp_ > max_suction_hp) {
        suction_hp_ = max_suction_hp - total_suction_hp_;
    }

    total_suction_hp_ += suction_hp_;
    ++suction_count_;

    return true;
}
