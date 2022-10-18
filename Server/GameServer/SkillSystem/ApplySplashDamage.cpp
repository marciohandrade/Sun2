#include "stdafx.h"
#include "ApplySplashDamage.h"
#include "MovingPlanner.h"
#include "CharacterFormula.h"

namespace nsSkill {
;

void ApplySplashDamage(GameField* game_field, Character* attacker,
                       SLOTCODE skill_code, eATTACK_TYPE attack_type, 
                       DAMAGETYPE damage, int max_number_of_target_select,
                       DWORD sector_index, WzVector &vDestPos,
                       float fRadius, eSD_APPLY SDApply,
                       const BaseAttackPowerApplyOption* input_apply_option)
{
    if (game_field == NULL || attacker == NULL) {
        return;
    }

    //_NA000000_110210_ENHANCED_TARGET_SEARCH_ALGORITHM
    ns_object::SearchResult::SingleSearch search_result;
    ;{
        ns_object::SearchOption::SingleSearch search_option(
            max_number_of_target_select,//int max_number_of_selected_characters,
            attacker,   //Character* input_actor,
            NULL,       //Character* input_main_target_if_exist_it,
            &vDestPos,  //const WzVector* input_specific_application_center_pos_if_main_target_is_not_used_to_pos,
            //
            0.0f,       //float attack_ranges,
            fRadius,    //float ranges_of_application,
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
    DamageArgs damage_args(attacker, attack_type);
    ;{
        //damage_args.option_.is_critical_;
        //damage_args.option_.is_mirror_;
        //damage_args.limit_hp_;
        damage_args.attack_regist_kind_ = eATTACK_RESIST_SKILL;
        damage_args.sd_apply_ = SDApply;
        //damage_args.damage_;
        //damage_args.damage_sd_;
    };
    //
    const eATTACK_TYPE base_attack_type = attacker->GetWeaponBaseAttType();
    BaseAttackPowerApplyOption apply_option = { 0, };
    if (input_apply_option) {
        apply_option = *input_apply_option;
    }
    //
    MSGSUB_SKILL_PERIODIC_DAMAGE_BRD msg_brd;
    ;{
        msg_brd.Clear();
        msg_brd.m_dwAttackerKey = attacker->GetObjectKey();
        msg_brd.m_SkillCode = skill_code;
    };

    for (int i = 0; i < number_of_targets; ++i)
    {
        Character* const target = search_result.targets[i];
        // PK 공격이라면 관련처리를 한다.
        attacker->ForcedAttack(target);
        ;{  // f100908.1L
            damage_args.damage_ = damage;
            // CHANGES: f110217.5L, changes periodic damage formula
            // CHANGES: f110304.6L, fixed exponentially growth damage problem
            // (Base + Element damage) * [apply ratio (iOption2)] + [skill attack power]
            if (apply_option.use_base_attack_apply)
            {
                damage_args.damage_ = ep2::DamagePerCycleCalculator::Simulate(
                    attacker, target,
                    base_attack_type,
                    attack_type, apply_option.base_attack_application_ratio,
                    damage, 0);
            };
        };
        // 데미지 처리
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
                ai_message_attacked.dwAttackerKey = attacker->GetObjectKey();
                ai_message_attacked.wDamage = damage_args.damage_;
                target->SendAIMessage(&ai_message_attacked, sizeof(ai_message_attacked));
            }

            ++number_of_hit_targets;
        }
    }

    msg_brd.m_byNumberOfTargets = static_cast<uint8_t>(number_of_hit_targets);

    if (number_of_hit_targets) {
        game_field->SendPacketAroundForSync(sector_index, &msg_brd);
    }
}

}; //end of namespace
