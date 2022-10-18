#include "stdafx.h"
#include "BombEffect.h"
#include "SkillInfoParser.h"
#include "CharacterFormula.h"
#include "StatusManager.h"
#include "Skill.h"
#include "AbilityFactory.h"
#include "Ability.h"
#include "StateInfoParser.h"

VOID BombEffect::End()
{
    if (m_pField == NULL) {
        return;
    };

    Character* const owner = GetOwner();
    if (owner == NULL)
    {
        return;
    }
    // TODO: need 'm_pOwner' validation check

    //_NA000000_110210_ENHANCED_TARGET_SEARCH_ALGORITHM
    SkillInfo temp_skill_info;

    const SkillInfoParser* skill_parser = SkillInfoParser::Instance();
    const SkillScriptInfo* base_skill_info = skill_parser->GetSkillInfo(m_SkillCode);
    if (base_skill_info == NULL) {
        return;
    }
    // 주기 데미지 어빌리티가 있는지 체크
    const BASE_ABILITYINFO* ability_interval_damage = \
        base_skill_info->GetAbilityInfo(eABILITY_DAMAGE_PER_TIME);
    // 디버프 어빌리티가 있는지 체크
    const BASE_ABILITYINFO* ability_debuff = \
        base_skill_info->GetAbilityInfo(eABILITY_STATUS_HEAL);

    // 최대 마리수 제한
    const int max_number_of_targets
        = (base_skill_info->m_byMaxTargetNo)
        ?   base_skill_info->m_byMaxTargetNo
        :   MAX_TARGET_NUM;

    ns_object::SearchResult::SingleSearch search_result;
    ;{
        ns_object::SearchOption::SingleSearch search_option(
            max_number_of_targets,//int max_number_of_selected_characters,
            owner,   //Character* input_actor,
            NULL,       //Character* input_main_target_if_exist_it,
            &m_vCurPos, //const WzVector* input_specific_application_center_pos_if_main_target_is_not_used_to_pos,
            //
            0.0f,       //float attack_ranges,
            m_fRadius,  //float ranges_of_application,
            SRF_FOWARD_360);    //eSKILL_AREA_TYPE forms_of_application
        bool success = m_pField->FindTargetsUsingSingleSearch(search_option, &search_result);
        if (success == false) {
            return;
        };
    };
    //
    if (search_result.number_of_selected_chars == 0) {
        return;
    }
    //
    MSGSUB_SKILL_EFFECT_BOMB_BRD msg_brd;
    ;{
        msg_brd.Clear();
        msg_brd.m_dwAttackerKey = owner->GetObjectKey();
        msg_brd.m_SkillCode = m_SkillCode;
    };
    // f100908.1L
    DamageArgs damage_args(owner, eATTACK_TYPE_MELEE);
    ;{
        //damage_args.option_.is_critical_;
        //damage_args.option_.is_mirror_;
        //damage_args.limit_hp_;
        damage_args.attack_regist_kind_ = eATTACK_RESIST_SKILL;
        StateInfoParser* state_parser = StateInfoParser::Instance();
        const BASE_STATEINFO* base_state_info = state_parser->GetStateInfo(GetStateID());
        if (base_state_info) {
            damage_args.sd_apply_ = static_cast<eSD_APPLY>(base_state_info->m_bySDApply);
        }
        //damage_args.damage_ = wResultDamage;
        //damage_args.damage_sd_;
    };

    AbilityFactory* const ability_factory = AbilityFactory::Instance();
    int number_of_hit_targets = 0;
    const int number_of_targets = search_result.number_of_selected_chars;
    for (int i = 0; i < number_of_targets; ++i)
    {
        Character* character = search_result.targets[i];
        // PK 공격이라면 관련처리를 한다.
        owner->ForcedAttack(character);

        // 데미지 처리
        BYTE skill_effects = 0;
        // TODO: f110106.2L, do check a 'skill attack power'
        damage_args.damage_ = ep2::CalcSkillDamage(owner, character,
            eATTACK_TYPE_MELEE, true, 0, &skill_effects,
            eATTACK_TYPE_MELEE, 0, m_fSkillPercentDamage);
        ;{
            // f100908.1L
            damage_args.option_.is_critical_ = (skill_effects & SKILL_EFFECT_CRITICAL) != 0;
        };

        owner->Damaged(&damage_args);

        EFFECT_BOMB_INFO* const effect_info = &msg_brd.m_DamageInfo[number_of_hit_targets];
        //
        if (ability_interval_damage)
        {
            // 타겟에 걸리는 주기 데미지 처리
            Ability* periodic_ability = ability_factory->AllocAbility(
                ability_interval_damage->m_wAbilityID, SKILL_TYPE_ACTIVE);
            periodic_ability->InitDetailed(owner,
                m_SkillCode, &temp_skill_info, 0, ability_interval_damage);
            AbilityStatus* ability_status_for_debug =
                character->GetStatusManager()->AllocAbilityStatus(owner, periodic_ability);
            __TOUCHED(ability_status_for_debug);

            effect_info->m_wStateID = ability_interval_damage->m_wStateID;

            // delete
            {
                AbilityFactory::Instance()->FreeAbility(periodic_ability);
                periodic_ability = NULL;
            }
        }

        // 디버프 처리
        if (ability_debuff)
        {
            Ability* cure_ability = ability_factory->AllocAbility(
                ability_debuff->m_wAbilityID, SKILL_TYPE_ACTIVE);
            cure_ability->InitDetailed(owner, m_SkillCode, &temp_skill_info, 0, ability_debuff);
            AbilityStatus* ability_status_for_debug =
                character->GetStatusManager()->AllocAbilityStatus(owner, cure_ability);
            __TOUCHED(ability_status_for_debug);

            // delete
            {
                AbilityFactory::Instance()->FreeAbility(cure_ability);
                cure_ability = NULL;
            }
        }

        // 패킷에 데미지 정보 셋팅
        effect_info->m_dwTargetKey = character->GetObjectKey();
        effect_info->m_wDamage = damage_args.damage_;
        effect_info->m_dwTargetHP = character->GetHP();

        ++number_of_hit_targets;
    }

    msg_brd.m_byNumberOfTargets = static_cast<uint8_t>(number_of_hit_targets);

    if (number_of_hit_targets) {
        m_pField->SendPacketAroundForSync(m_dwSectorIndex, &msg_brd); // ...
    }
    //
    BaseEffect::End();
}


VOID
SelfDestructionDamageEffect::SetInformation( Character* const /*pAttacker*/, GameField* const pGameField, const SelfDestructionDamage_info_t* const pDamaged_char_info )
{
    m_pField			= pGameField;
	m_eStep				= eStep_Init;
	m_DamagedCharInfo	= *pDamaged_char_info;
	m_ExecutableLifetime.SetTimer(TIMEOUT_LIFE_TIME);
}

BOOL
SelfDestructionDamageEffect::Update( DWORD /*curTick*/ )
{
    //switch( m_eStep )
	//{
	//case eStep_Init:
	//case eStep_Execute:
	//case eStep_Executed:
	//case eStep_Release:
	//	return FALSE;
	//};
    
	if( (eStep_Release == m_eStep) || (GetOwner() == NULL))
		return FALSE;

	if( eStep_Execute == m_eStep )
	{
		Start();
		return TRUE;
	}

	if( eStep_Executed == m_eStep )
	{
		End();
		return FALSE;
	}

	if( m_ExecutableLifetime.IsExpired() )
	{
		return FALSE;
	}

	return TRUE;
	//if( m_bNeedClose || !m_pOwner )
	//	return FALSE;

	//GameField* const pGameField = m_pOwner->GetField();
	//if( !(pGameField && pGameField == m_pField) )
	//	return FALSE;

	//return TRUE;
}

VOID
SelfDestructionDamageEffect::CmdExecute()
{
	if(m_eStep == eStep_Init)
	{
		m_eStep = eStep_Execute;
		//Start();
	}
}

VOID
SelfDestructionDamageEffect::End()
{
	//GameField* const pGameField = m_pOwner->GetField();
	//if( (eStep_Executed != m_eStep) || !(pGameField && (pGameField == m_pField)) )
	//	return;

	//const DWORD curHP = m_pOwner->GetHP();
	//if( !curHP )
	//{
	//	//	특수 처리 해당 Character가 바로 제거되는 상황 (Start에서 HP가 없었던 상황_이미 죽음_)
	//	m_pOwner->StopMoving();
	//	m_pField->DestroyObject( m_pOwner );
	//}

    Character* const owner = GetOwner();
    if (owner == NULL)
    {
        return;
    }

	//	해당 몬스터 제거
	const DWORD curHP = owner->GetHP();
	if( curHP )
	{
		//m_pOwner->SetHP( 1 );
		//m_pOwner->Damaged( 0, eATTACK_TYPE_MELEE, 100, FALSE );

		//	그냥 죽는 거다...
		owner->StopMoving();
		m_pField->DestroyObject( owner );
	}

}

VOID
SelfDestructionDamageEffect::Start()
{
    Character* const owner = GetOwner();
    if (owner == NULL)
    {
        return;
    }

    GameField* const game_field = owner->GetField();
    if ((eStep_Executed == m_eStep) ||
        !(game_field && (game_field == m_pField)))
    {
        return;
    }
    m_eStep = eStep_Executed;
    //_NA000000_110210_ENHANCED_TARGET_SEARCH_ALGORITHM
    const SkillInfoParser* skill_parser = SkillInfoParser::Instance();
    const SkillScriptInfo* const base_skill_info = skill_parser->GetSkillInfo(m_SkillCode);
    if (base_skill_info == NULL) {
        return;
    }

    ns_object::SearchResult::SingleSearch search_result;
    ;{
        ns_object::SearchOption::SingleSearch search_option(
            MAX_TARGET_NUM,//int max_number_of_selected_characters,
            owner,   //Character* input_actor,
            NULL,       //Character* input_main_target_if_exist_it,
            &m_vCurPos, //const WzVector* input_specific_application_center_pos_if_main_target_is_not_used_to_pos,
            //
            0.0f,       //float attack_ranges,
            m_fRadius,    //float ranges_of_application,
            SRF_FOWARD_360);    //eSKILL_AREA_TYPE forms_of_application
        bool success = game_field->FindTargetsUsingSingleSearch(search_option, &search_result);
        if (success == false) {
            return;
        };
    };
    if (search_result.number_of_selected_chars == 0) {
        return;
    }
    //
    MSGSUB_SKILL_SELF_DESTRUCTION_BRD msg_brd;
    ;{
        msg_brd.Clear();
        msg_brd.m_dwAttackerKey = owner->GetObjectKey();
        msg_brd.m_SkillCode = m_SkillCode;
    };

    StateInfoParser* const state_parser = StateInfoParser::Instance();
    int number_of_hit_targets = 0;
    const int number_of_targets = search_result.number_of_selected_chars;
    for (int i = 0; i < number_of_targets; ++i)
    {
        Character* const character = search_result.targets[i];
        //  PK 공격이라면 관련처리를 한다.
        owner->ForcedAttack(character);

        // f100908.1L
        DamageArgs damage_args(owner, eATTACK_TYPE_MELEE);
        ;{
            //damage_args.option_.is_critical_;
            //damage_args.option_.is_mirror_;
            //damage_args.limit_hp_;
            damage_args.attack_regist_kind_ = eATTACK_RESIST_SKILL;
            const BASE_STATEINFO* base_stateinfo = state_parser->GetStateInfo(GetStateID());
            if (base_stateinfo) {
                damage_args.sd_apply_ = static_cast<eSD_APPLY>(base_stateinfo->m_bySDApply);
            }
            //damage_args.damage_;
            //damage_args.damage_sd_;
        };

        //  데미지 처리  
        BYTE skill_effects = 0;
        if (m_DamagedCharInfo.m_DamageOpt == eDamageOpt_Ratio)
        {
            // TODO: f110106.2L, do check a 'skill attack power'
            damage_args.damage_ = ep2::CalcSkillDamage(owner, character,
                eATTACK_TYPE_MELEE, true, 0, &skill_effects,
                eATTACK_TYPE_MELEE, 0, static_cast<float>(m_DamagedCharInfo.m_Damage / 1000.f));
        }
        else //eDamageOpt_Value
        {
            damage_args.damage_ = m_DamagedCharInfo.m_Damage;
        }

        // f100908.1L
        ;{
            //damage_args.option_.is_normal_;
            damage_args.option_.is_critical_ = (skill_effects & SKILL_EFFECT_CRITICAL) != 0;
            //damage_args.option_.is_mirror_;
            //damage_args.target_;
            //damage_args.limit_hp_;
            //damage_args.attack_regist_kind_;
            //damage_args.sd_apply_;
            //damage_args.damage_;
            //damage_args.damage_sd_;
        };

        owner->Damaged(&damage_args);

        if (DAMAGE_INFO* damage_info = &msg_brd.m_DamageInfo[number_of_hit_targets])
        {
            damage_info->m_dwTargetKey = character->GetObjectKey();
            damage_info->m_wDamage   = damage_args.damage_;
            damage_info->m_dwTargetHP = character->GetHP();
        }
        //
        ++number_of_hit_targets;
    }

    msg_brd.m_byNumberOfTargets = static_cast<uint8_t>(number_of_hit_targets);

    if (number_of_hit_targets) {
        m_pField->SendPacketAroundForSync(owner, &msg_brd);
    };

    BaseEffect::End();
}






