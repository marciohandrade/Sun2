#include "StdAfx.h"
#include "DamageAbility.h"
#include "Skill.h"
#include "CharacterFormula.h"
#include "EtherBulletInfoParser.h"
#include "StatusManager.h"
#include "AbilityStatus.h"
#include "StateInfoParser.h"

void DamageAbility::InitDetailed(Character* attacker,
                                 SLOTCODE skill_code, SkillInfo* skill_info,
                                 BYTE skill_stat_type,
                                 const BASE_ABILITYINFO* ability_info)
{
    Ability::InitDetailed(attacker, skill_code, skill_info, skill_stat_type, ability_info);
    skill_stat_type_ = skill_stat_type;
    if (skill_info) {
        skill_effect_notify_link_ = &skill_info->skill_effect_;
    }
    else {
        skill_effect_notify_link_ = NULL;
    }
}

bool DamageAbility::Execute(Character* target, BYTE* msg, WORD& msg_size)
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

    const eATTACK_TYPE attack_type = (eATTACK_TYPE)ability_info->m_iOption1;
#ifdef _NA_006728_20130520_SKILL_BALANCE_MODIFY
    DamageArgs damage_args(attacker, attack_type, GetSkillCode());
#else
    DamageArgs damage_args(attacker, attack_type);
#endif // _NA_006728_20130520_SKILL_BALANCE_MODIFY

    // 스킬 공격력, 스킬 %추가 데미지를 계산한다.
    int skill_attack_power = ability_info->m_iParam[0];
    float skill_added_ratio_damage = (float)(ability_info->m_iParam[1] / 1000.0f);
    
    skill_attack_power = CalcAddDamage(skill_attack_power, attacker);
#ifdef _NA001605_EP2_ATTRIBUTES_RENEWAL_    
    damage_args.damage_ = ep2::CalcSkillDamage(
        attacker, 
        target,
        attacker->GetWeaponBaseAttType(),
        true, 
        0, 
        skill_effect_notify_link_,
        attack_type, 
        skill_attack_power, 
        skill_added_ratio_damage
    );
#else
    damage_args.damage_ = ep1::CalcSkillDamage(
        attacker, 
        target, 
        attack_type,
        skill_attack_power, 
        skill_added_ratio_damage, 
        0,
        skill_effect_notify_link_, 
        skill_stat_type_
    );
#endif
    
    damage_args.option_.is_critical_ = ((*skill_effect_notify_link_ & SKILL_EFFECT_CRITICAL) != 0);
    damage_args.attack_regist_kind_ = eATTACK_RESIST_SKILL;
    damage_args.sd_apply_ = eSD_APPLY_DO;
    
    target->Damaged(&damage_args);
 
    const Skill* const skill = GetSKill();
    if (skill != NULL)
    {
        if (skill->GetAppliedEtherBulletOption())
        {
            tagEtherBulletInfo * bullet_info = skill->GetAppliedEtherBulletInfo();
            if (bullet_info != NULL)
            {
                *skill_effect_notify_link_ |= bullet_info->m_dwEffect;
            }
        }
    }

    // 패킷구조체를 채운다.
    SKILL_RESULT_DAMAGE* const damage_msg = (SKILL_RESULT_DAMAGE*)msg;
    if (damage_msg != NULL)
    {
        //_NA_0_20100211_SD_EFFECT_MODIFY
        damage_msg->m_wDamage = damage_args.damage_;
        damage_msg->m_dwTargetHP = target->GetHP();
#ifdef _NA_006638_20120415_ADD_SHIELD_INFO_IN_TARGET_INFO
        damage_msg->target_sd = target->GetSD();
#endif // _NA_006638_20120415_ADD_SHIELD_INFO_IN_TARGET_INFO

        if (skill_effect_notify_link_ != NULL)
        {
            damage_msg->m_byEffect = *skill_effect_notify_link_;
        }
        else
        {
            damage_msg->m_byEffect = 0;
        }
            
        msg_size = damage_msg->GetSize();
    }

    return true;
}

int DamageAbility::CalcAddDamage(const int skill_attack_power, Character* const attacker) const
{
    if (attacker == NULL)
    {
        return 0;
    }

    const Skill* const skill = GetSKill();
    if (skill == NULL)
    {
        return 0;
    }
    int calc_power = skill->GetAddedAttackPowerPerStatus();
    calc_power += skill_attack_power;

    // 스킬 데미지 증가 상태에 따른 추가 데미지를 계산한다.
    const int increse_skill_damage = 
        ns_formula::ability::CalcIncreseSkillDamage(*attacker, *skill, skill_attack_power);
    if (increse_skill_damage != 0)
    {
        calc_power += increse_skill_damage;
    }
    
    // 콤보 사용에 따른 추가 데미지를 계산한다.
    const int combo_add_damage = 
        ns_formula::ability::CalcComboDamage(*attacker, *skill, skill_attack_power);
    if (combo_add_damage != 0)
    {
        calc_power += combo_add_damage;
    }

    // 투기 사용에 따른 추가 데미지를 계산한다.
    const int fighting_energy_damage = 
        ns_formula::ability::CalcFightingEnergy(*attacker, *skill, skill_attack_power);
    if (fighting_energy_damage != 0)
    {
        calc_power += fighting_energy_damage;
    }

    // 저주 증가 스킬 사용에 따른 추가 데미지를 계산한다.
    const eSKILL skill_class_code = skill->GetSkillClassCode();
    const int curse_add_damage = 
        ns_formula::ability::CalcIncreseCurse(skill_class_code, skill_attack_power, *attacker);
    if (curse_add_damage != 0)
    {
        calc_power += curse_add_damage;
    }
    
    return calc_power;
}