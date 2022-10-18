#include "stdafx.h"
#include "Ability.h"
#include "WoundStatus.h"
#include "StatusManager.h"
#include "CharacterFormula.h"
#include "SkillInfoParser.h"
#include "StateInfoParser.h"


void WoundStatus::Init(Character* in_owner, Character* in_attacker, Ability* ability)
{
	AbilityStatus::Init(in_owner, in_attacker, ability);

	const BASE_ABILITYINFO* const base_ability_info = ability->GetBaseAbilityInfo();
	attack_type_ = (eATTACK_TYPE)base_ability_info->m_iOption1;
	period_damage_ = base_ability_info->m_iParam[1];
	
    Character* const attacker = GetAttacker();
    if (attacker == NULL)
    {
        return;
    }
    Character* const owner = GetOwner();
    if (owner == NULL)
    {
        return;
    }

    int skill_attack_power = period_damage_;

#ifdef _NA001605_EP2_ATTRIBUTES_RENEWAL_
    DAMAGETYPE calc_damage = ep2::DamagePerCycleCalculator::Simulate(
        attacker, 
        owner,
        attacker->GetWeaponBaseAttType(),
        attack_type_, 
        base_ability_info->m_iOption2,
        skill_attack_power, 
        0
    );
#else
    const SkillScriptInfo* const skill_script_info = 
        SkillInfoParser::Instance()->GetSkillInfo(GetSkillCode());
    if (skill_script_info == NULL)
    {
        return;
    }
    DAMAGETYPE calc_damage = ep1::CalcSkillDamage(
        attacker, 
        owner,
        attack_type_, 
        skill_attack_power,
        0, 
        0,
        NULL,
        skill_script_info->m_bySkillStatType, 
        FALSE
    );
#endif

    int add_damage = CalcAddDamage(*ability, *attacker);
    if (add_damage != 0)
    {
        calc_damage += add_damage;
    }

	period_damage_ = calc_damage;
}

void WoundStatus::Execute()
{
    Character* const attacker = GetAttacker();
    if (attacker == NULL || attacker->GetField() == NULL)
    {
        return;
    }
    Character* const owner = GetOwner();
    if (owner == NULL || owner->CanBeAttacked() == false)
    {
        return;
    }
#ifdef _NA_006728_20130520_SKILL_BALANCE_MODIFY
    DamageArgs damage_args(attacker, attack_type_, GetSkillCode());
#else
    DamageArgs damage_args(attacker, attack_type_);
#endif _NA_006728_20130520_SKILL_BALANCE_MODIFY
    {
#ifdef _NA_008486_20150914_TOTAL_BALANCE
        damage_args.attack_regist_kind_ = eATTACK_RESIST_SKILL_DOT_DAMAGE;
#else
        damage_args.attack_regist_kind_ = eATTACK_RESIST_SKILL;
#endif //_NA_008486_20150914_TOTAL_BALANCE
        const BASE_STATEINFO* const state_info = StateInfoParser::Instance()->GetStateInfo(GetStateID());
        if (state_info != NULL) 
        {
            damage_args.sd_apply_ = static_cast<eSD_APPLY>(state_info->m_bySDApply);
        }
        damage_args.damage_ = period_damage_;
    }
    owner->Damaged(&damage_args);

    MSGSUB_SKILL_PERIODIC_DAMAGE_BRD msg_brd;
    {
        msg_brd.Clear();
        msg_brd.m_dwAttackerKey = attacker->GetObjectKey();
        msg_brd.m_SkillCode = GetSkillCode();
        msg_brd.m_byNumberOfTargets = 1;
        DAMAGE_INFO* const damage_info = &msg_brd.m_DamageInfo[0];
        if (damage_info != NULL)
        {
            damage_info->m_dwTargetKey = owner->GetObjectKey();
            damage_info->m_wDamage = damage_args.damage_;
            damage_info->m_dwTargetHP = owner->GetHP();
        }
    }
    owner->SendPacketAroundForSync(&msg_brd);

    AbilityStatus::Execute();
}

int WoundStatus::CalcAddDamage(const Ability& ability, const Character& attacker) const
{
    int add_damage = 0;
    const int skill_damage = GetAbilityInfo()->m_iParam[1];

    const Skill* const skill = ability.GetSKill();
    if (skill != NULL)
    {
        // 스킬 데미지 증가 상태에 따른 추가 데미지를 계산한다.
        const int increse_skill_damage = 
            ns_formula::ability::CalcIncreseSkillDamage(attacker, *skill, skill_damage);
        if (increse_skill_damage != 0)
        {
            add_damage += increse_skill_damage;
        }

        // 콤보 사용에 따른 추가 데미지를 계산한다.
        const int combo_damage = 
            ns_formula::ability::CalcComboDamage(attacker, *skill, skill_damage);
        if (combo_damage != 0)
        {
            add_damage += combo_damage;
        }
    }

    // 저주 증가 스킬 사용에 따른 추가 데미지를 계산한다.
    eSKILL attack_skill_class = eSKILL_INVALID;
    switch (GetStateID())
    {
    case eCHAR_STATE_PAIN:
        attack_skill_class = eSKILL_PAIN;
        break;
    case eCHAR_STATE_FIRE_WOUND2:
        attack_skill_class = eSKILL_DARK_FIRE;
        break;
    case eCHAR_STATE_POISON2:
        attack_skill_class = eSKILL_ENCHANT_POISON;
        break;
    }
    if (attack_skill_class != eSKILL_INVALID)
    {
        const int curse_damage = 
            ns_formula::ability::CalcIncreseCurse(attack_skill_class, skill_damage, attacker);
        if (curse_damage != 0)
        {
            add_damage += curse_damage;
        }
    }

    return add_damage;
}
