#include "StdAfx.h"
#include ".\enchantpoisonstatus.h"
#include "StatusManager.h"
#include "PacketStruct_CG.h"
#include "SkillInfoParser.h"
#include "AbilityFactory.h"
#include "Ability.h"
#include "Skill.h"

EnchantPoisonStatus::EnchantPoisonStatus()
{
}

EnchantPoisonStatus::~EnchantPoisonStatus()
{
}

void EnchantPoisonStatus::ExecuteSkill(Character* target)
{
    if (obj_util::IsValid(target) == false)
    {
        return;
    }
    
    Character* const owner = GetOwner();
    if (owner == NULL)
    {
        return;
    }

    StatusManager* const tartget_status_manager = target->GetStatusManager();
    if (tartget_status_manager->FindStatus(eCHAR_STATE_POISON2) != NULL)
    {
        return;
    }
        
    const SLOTCODE skill_code = GetSkillCode();
    const SkillScriptInfo* const skill_script_info = 
        SkillInfoParser::Instance()->GetSkillInfo(skill_code);
    if (skill_script_info == NULL)
    {
        return;
    }
        
    const CODETYPE effect_ability_id = GetAbilityInfo()->m_iOption1;
    const BASE_ABILITYINFO* const effect_ability_info = 
        skill_script_info->GetAbilityInfo(effect_ability_id);
    if (effect_ability_info == NULL)
    {
        return;
    }
        
    Ability* ability = 
        AbilityFactory::Instance()->AllocAbility(effect_ability_info->m_wAbilityID, SKILL_TYPE_ACTIVE);
    if (ability == NULL)
    {
        return;
    }

    SkillInfo temp_skill_info;
    ability->InitDetailed(owner, skill_code, &temp_skill_info, 0, effect_ability_info);
    WzVector target_pos;
    target->GetPos(&target_pos);
    if (ability->CanExecute(owner, target, target->GetObjectKey(), target_pos) == false)
    {
        AbilityFactory::Instance()->FreeAbility(ability);
        return;
    }

    AbilityStatus* const ability_status = tartget_status_manager->AllocAbilityStatus(owner, ability);
    if (ability_status == NULL)
    {
        AbilityFactory::Instance()->FreeAbility( ability );
        return;
    }
    ability_status->SendStatusAddBRD();

    // delete
    {
        AbilityFactory::Instance()->FreeAbility(ability);
        ability = NULL;
    }
}