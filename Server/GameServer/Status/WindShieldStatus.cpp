#include "stdafx.h"
#include "WindShieldStatus.h"

#include "SkillInfoParser.h"
#include "StateInfoParser.h"

#include "Ability.h"
#include "MovingPlanner.h"
#include "CharacterFormula.h"

VOID WindShieldStatus::Init( Character *pOwner, Character *pAttacker, Ability *pAbility )
{
	AbilityStatus::Init( pOwner, pAttacker, pAbility );

	const BASE_ABILITYINFO *pBaseAbilityInfo = pAbility->GetBaseAbilityInfo();
	m_eAttackType = (eATTACK_TYPE)pBaseAbilityInfo->m_iOption1;
	m_fAttackRange = pBaseAbilityInfo->m_iParam[0]/10.0f;
	m_wPeriodicDamage = pBaseAbilityInfo->m_iParam[1];
    base_skill_power_application_ratio_ = pBaseAbilityInfo->m_iOption2;

#ifdef _NA_008324_20150603_MODIFY_WINDSHIELD_FORCEATTACK
    owner_player_ = NULL;
    is_force_attack_ = false;
    if (pOwner->GetObjectType() == PLAYER_OBJECT) {
        owner_player_ = static_cast<Player*>(pOwner);
        is_force_attack_ = owner_player_->IsForceAttack();
    }
#endif //_NA_008324_20150603_MODIFY_WINDSHIELD_FORCEATTACK 
}

VOID WindShieldStatus::Execute()
{
    // CHANGES: f110307.3L, prevent busy loop execution on fail result because of execution timer skipping
    struct LastExecuter {
        LastExecuter(WindShieldStatus* status) : wind_shield_status(status) {}
        ~LastExecuter() {
            wind_shield_status->AbilityStatus::Execute();
        }
        WindShieldStatus* const wind_shield_status;
    } last_executer(this);

    Character* const owner = GetOwner();
    if (owner == NULL)
    {
        return;
    }

    //_NA000000_110210_ENHANCED_TARGET_SEARCH_ALGORITHM
    GameField* game_field = owner->GetField();
    if (game_field == NULL) {
        return;
    }

#ifdef _NA_008324_20150603_MODIFY_WINDSHIELD_FORCEATTACK
    const BOOLEAN bApplyGeneralForcedAtkRule = !(owner->Policyment.FIELD & owner->Policyment.PCC_IN_SSQ_FIELD);
    if (bApplyGeneralForcedAtkRule == TRUE && is_force_attack_ == true)
    {
        if (owner_player_ != NULL) {
            owner_player_->SetForceAttack(true);
        }
    }
#endif //_NA_008324_20150603_MODIFY_WINDSHIELD_FORCEATTACK

    // 최대 마리수 제한
    const SkillInfoParser* skill_parser = SkillInfoParser::Instance();
    const SkillScriptInfo* base_skill_info = skill_parser->GetSkillInfo(GetSkillCode());
    assert(base_skill_info);

    const int max_number_of_target_select = base_skill_info->m_byMaxTargetNo;

    // 시전자가 안전지대 타일에 위치해 있으면 무효
    // (CHANGES) (f100528.5L) fix up an incorrect condition check.
    // the fail result of a 'GetTileEvent' means that a player is not standing on a valid tile.
    // (NOTE) I think that chances of success are very high like a 99.0%
    const nsAI::TileEvent* tile_event = NULL;
    if (owner->PlanEvent->GetTileEvent(&tile_event) == false) {
        return;
    }
    if ((tile_event->Attr & PTA_SAFETY_ZONE)) {
        return;
    }

    ns_object::SearchResult::SingleSearch search_result;
    ;{
        ns_object::SearchOption::SingleSearch search_option(
            max_number_of_target_select,//int max_number_of_selected_characters,
            owner,   //Character* input_actor,
            NULL,       //Character* input_main_target_if_exist_it,
            owner->GetPosPtr(), //const WzVector* input_specific_application_center_pos_if_main_target_is_not_used_to_pos,
            //
            0.0f,       //float attack_ranges,
            m_fAttackRange,    //float ranges_of_application,
            SRF_FOWARD_360);    //eSKILL_AREA_TYPE forms_of_application
        bool success = game_field->FindTargetsUsingSingleSearch(search_option, &search_result);
        if (success == false) {
            return;
        };
    };
    //
    const int number_of_targets = search_result.number_of_selected_chars;
    if (number_of_targets == 0) {
        return;
    }
    int number_of_hit_targets = 0;
    // f100908.1L
#ifdef _NA_006728_20130520_SKILL_BALANCE_MODIFY
    DamageArgs damage_args(owner, m_eAttackType, GetSkillCode());
#else
    DamageArgs damage_args(owner, m_eAttackType);
#endif // _NA_006728_20130520_SKILL_BALANCE_MODIFY
    ;{
        //damage_args.option_.is_critical_;
        //damage_args.option_.is_mirror_;
        //damage_args.limit_hp_;
#ifdef _NA_008486_20150914_TOTAL_BALANCE
        damage_args.attack_regist_kind_ = eATTACK_RESIST_SKILL_DOT_DAMAGE;
#else
        damage_args.attack_regist_kind_ = eATTACK_RESIST_SKILL;
#endif //_NA_008486_20150914_TOTAL_BALANCE
        //레인지(주변적들에게 지속적 데미지)
        const BASE_STATEINFO* base_stateinfo = StateInfoParser::Instance()->GetStateInfo(GetStateID());
        if (base_stateinfo) {
            damage_args.sd_apply_ = static_cast<eSD_APPLY>(base_stateinfo->m_bySDApply);
        }
        damage_args.damage_ = m_wPeriodicDamage;
        //damage_args.damage_sd_;
    };
    // CHANGES: f110217.5L
    const eATTACK_TYPE base_attack_type = owner->GetWeaponBaseAttType();
    //
    MSGSUB_SKILL_PERIODIC_DAMAGE_BRD msg_brd;
    msg_brd.Clear();
    const Character* const attacker = GetAttacker();
    if (attacker != NULL) 
    {
        msg_brd.m_dwAttackerKey = attacker->GetObjectKey();
    }
    msg_brd.m_SkillCode = GetSkillCode();

    for (int i = 0; i < number_of_targets; ++i)
    {
        Character* const target = search_result.targets[i];
        // CHANGES: f110217.5L, changes periodic damage formula
        // CHANGES: f110304.6L, fixed exponentially growth damage problem
        // (Base + Element damage) * [apply ratio (iOption2)] + [skill attack power]
        damage_args.damage_ = ep2::DamagePerCycleCalculator::Simulate(
            owner, target,
            base_attack_type,
            m_eAttackType, base_skill_power_application_ratio_,
            m_wPeriodicDamage, 0);
        //
        target->Damaged(&damage_args);
        if (DAMAGE_INFO* damage_info = &msg_brd.m_DamageInfo[number_of_hit_targets])
        {
            damage_info->m_dwTargetKey = target->GetObjectKey();
            damage_info->m_wDamage = damage_args.damage_;
            damage_info->m_dwTargetHP = target->GetHP();

            // 공격당한 대상이 NPC이면 AIMessage 전송
            if (target->IsEqualObjectKind(NPC_OBJECT))
            {
                AI_MSG_ATTACKED ai_message_attacked;
                ai_message_attacked.dwAttackerKey = owner->GetObjectKey();
                ai_message_attacked.wDamage = damage_args.damage_;
                target->SendAIMessage(&ai_message_attacked, sizeof(ai_message_attacked));
            }

            ++number_of_hit_targets;
        }
    };

    msg_brd.m_byNumberOfTargets = static_cast<uint8_t>(number_of_hit_targets);

    if (number_of_targets) {
        owner->SendPacketAroundForSync(&msg_brd);
    }

#ifdef _NA_008324_20150603_MODIFY_WINDSHIELD_FORCEATTACK
    if (owner_player_ != NULL) {
        owner_player_->SetForceAttack(false);
    }
#endif //_NA_008324_20150603_MODIFY_WINDSHIELD_FORCEATTACK
}



















