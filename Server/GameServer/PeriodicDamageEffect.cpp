#include "stdafx.h"
#include "PeriodicDamageEffect.h"

#include "SkillInfoParser.h"
#include "ApplySplashDamage.h"
#include "StateInfoParser.h"

#include "CharacterFormula.h"
#include <SkillManager.h>
#include <StatusManager.h>

VOID PeriodicDamageEffect::Execute()
{
    if (m_eDamageType == eATTACK_TYPE_SKILL)
    {
        SLOTCODE skill_code = (SLOTCODE)ability_info_->m_iOption2;

        if (ability_info_->m_wAbilityID == eABILITY_AURORA) 
        {
            if (ExecuteAurora() == false) 
            {
                StopStatus(); // off effect
                return;
            }
        }
        else if (UseSkill(skill_code) == FALSE) 
        {
            StopStatus(); // off effect only
            return;
        }
    }
    else
    {
        // 최대 마리수 제한
        SkillScriptInfo *pBaseSkillInfo = SkillInfoParser::Instance()->GetSkillInfo( m_SkillCode );
        int iCountLimit = MAX_TARGET_NUM;
        if( pBaseSkillInfo && pBaseSkillInfo->m_byMaxTargetNo ) {
            iCountLimit = pBaseSkillInfo->m_byMaxTargetNo;
        }

        eSD_APPLY SDApply = eSD_APPLY_EMPTY;

        BASE_STATEINFO* pStateInfo = StateInfoParser::Instance()->GetStateInfo(GetStateID());
        if( pStateInfo ) {
            SDApply = (eSD_APPLY)pStateInfo->m_bySDApply;
        }

        nsSkill::BaseAttackPowerApplyOption apply_option[1] = { 0, };
        if (ability_info_ != NULL)
        {
            apply_option->use_base_attack_apply = true;
            apply_option->base_attack_application_ratio = ability_info_->m_iOption2;
        };

        nsSkill::ApplySplashDamage( m_pField, GetOwner(), m_SkillCode, m_eDamageType, 
            m_wDamage, iCountLimit, m_dwSectorIndex, m_vCurPos, m_fRadius, SDApply,
            apply_option);
    }

	BaseEffect::Execute();
}

BOOL PeriodicDamageEffect::UseSkill(SLOTCODE skill_code)
{
    const SkillScriptInfo* skill_info = SkillInfoParser::Instance()->GetSkillInfo(skill_code);
    if (skill_info == NULL) {
        return false;
    }

    Character* owner = GetOwner();
    if (owner == NULL)
        return FALSE;

    if (owner->_ChkNeedMP(skill_info->m_wMPSpend) == false) {
        return FALSE;
    }

    const WzVector* item_pos = NULL;
    ;{
        if (skill_info->m_byTarget == SKILL_TARGET_AREA)
            item_pos = &m_vCurPos;
        else if(skill_info->m_byTarget == SKILL_TARGET_ME)
            item_pos = owner->GetPosPtr();
    };

    SkillInfo skill_info_data(owner, skill_code, 0);
    skill_info_data.SetArgsObjectInfo(owner->GetObjectKey(), item_pos, item_pos, item_pos);
    skill_info_data.SetArgsSkillExt(skill_info_data.kDefaultSequence,
        skill_info_data.kDefaultSkillEffect,
        skill_info_data.kDefaultSkillDelay,
        SKILL_FACTOR_EFFECT,
        skill_info_data.kDefaultPropensity,
        skill_info_data.kDefaultEvent);

    SkillManager* skill_manager = owner->GetSkillManager();
    return skill_manager->RegisterSkill(SKILL_TYPE_ACTIVE, &skill_info_data);
}

bool PeriodicDamageEffect::ExecuteAurora()
{
    Character* const owner = GetOwner();
    if (owner == NULL) {
        return false;
    }

    StatusManager* const status_manager = owner->GetStatusManager();
    eCHAR_STATE_TYPE state_id = (eCHAR_STATE_TYPE)ability_info_->m_wStateID;
    if (status_manager == NULL || status_manager->FindStatus(state_id) == NULL) {
        return false;
    }

    SLOTCODE skill_code = (SLOTCODE)ability_info_->m_iOption2;
    if (UseSkill(skill_code) == FALSE)
    {
        if (status_manager) {
            status_manager->Remove(state_id); // remove status
        }
        return false;
    }

    return true;
}
