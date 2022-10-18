#include "stdafx.h"
#include "StyleSkill.h"

#include <PacketStruct_CG.h>
#include <Battle.h>

#include <ServerOptionParser.h>
#include <ServerOptionParserEx.h>
#include <SkillInfoParser.h>
#include <AIParamParser.h>

#include "CharacterFormula.h"
#include "NPC.h"
#include "GameField.h"
#include "Character.h"
#include "Player.h"
#include "StatusManager.h"
#include "EtcStatus.h"
#include "EtherBulletInfoParser.h"

//==================================================================================================

// internal dice by ratio
// arg: 100% = 1.0f
inline BOOLEAN
StyleSkill::IsOccuredByDraw(float ratio_limit)
{
    DWORD seed = RandomNumberGenerator::GetRandomNumberRange(0, 10000);
    DWORD calc_ratio_limit = static_cast<DWORD>(ratio_limit * 10000.0f);
    return seed <= calc_ratio_limit;
}

void
StyleSkill::SetExecuteInterval()
{
    interval_ = 0;

    if (Player* player = static_cast<Player*>(owner_))
    { // set up interval
        float interval = static_cast<float>(player->GetActionDelay());
        ;     interval *= base_style_info_->m_fThirdDelay;
        interval_ = static_cast<WORD>(interval);
    }

    // 패킷 살~짝 일찍 주자
    if (interval_ > 100) {
        interval_ -= 100;
    }
}

void
StyleSkill::Init(Character* owner,
                 const SkillInfo* skill_info,
                 const ROOT_SKILLINFO* root_skill_info)
{
    SetupDefault(owner, skill_info, root_skill_info);
    // 변수 초기화
    ZeroMemory(&style_, sizeof(style_));
}

BOOL
StyleSkill::StartExecute()
{
    game_field_ = owner_->GetField();
    if (game_field_ == NULL) 
    {
        MSG_CG_SKILL_ACTION_NAK msg_nak;
        msg_nak.m_byErrorCode = RC::RC_SKILL_FIELD_IS_NULL;
        owner_->SendPacket(&msg_nak, sizeof(msg_nak));
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"] GameField is NULL!");
        return false;
    }

    // MainTarget이 유효한지 체크한다.(스킬 사거리 체크)
    if (CheckMainTarget(NULL) == false) {
        is_missed_ = true;
    }
    else {
        CommonExecute();
    }
    ;{ //assign values
        MSGSUB_STYLE_PLAYER_ATTACK_BRD msg_brd;
        msg_brd.Clear();
        msg_brd.dwAttackerKey      = owner_->GetObjectKey();
        msg_brd.byAttackType       = skill_info_data_.attack_sequence_;
    #ifdef _NA003109_STYLE_PRESENTATION_IMPROVEMENT
        msg_brd.style_presentation = skill_info_data_.style_presentation_;
    #endif
        msg_brd.StyleCode          = skill_info_data_.skill_code_;
        msg_brd.dwClientSerial     = skill_info_data_.client_serial_;
        msg_brd.dwPrimaryTargetKey = skill_info_data_.main_target_key_;
        msg_brd.wvCurPos           = skill_info_data_.cur_pos_;
        owner_->SendPacketAroundForSync(&msg_brd);
    };
    return true;
}

void
StyleSkill::EndExecute()
{
    game_field_ = owner_->GetField();
    if (game_field_ == false) 
    {
        MSG_CG_SKILL_ACTION_NAK msg_nak;
        msg_nak.m_byErrorCode = RC::RC_SKILL_FIELD_IS_NULL;
        owner_->SendPacket(&msg_nak, sizeof(msg_nak));
        SUNLOG(eCRITICAL_LOG, "["__FUNCTION__"] GameField is NULL!");
        return;
    }

    if (is_missed_)   
    { // Miss 처리를 하기 위해서 결과 패킷은 보낸다.
        Broadcast_Style_Result();
        Skill::EndExecute();
        return;
    }

    MSGSUB_STYLE_PLAYER_ATTACK_RESULT_BRD msg_brd;
    msg_brd.Clear();
    msg_brd.dwClientSerial = skill_info_data_.client_serial_;
    msg_brd.dwAttackerKey  = owner_->GetObjectKey();
    //
    style_.player_pos = *owner_->GetPosPtr();

    ns_object::SearchOption::SingleSearch::result_type search_result;
    if (FindTargets(&search_result) == false) {
        return;
    }
    const int number_of_targets = search_result.number_of_selected_chars;
    // 타겟별로 데미지와 이펙트를 적용한다.
    Character* main_target = NULL;
    for (int i = 0; i < number_of_targets; ++i)
    {
        Character* target = search_result.targets[i];
        // NOTE: change filters
        if (target == NULL) {
            continue;
        }
        const DWORD target_object_key = target->GetObjectKey();
        if (target_object_key == skill_info_data_.main_target_key_) {
            main_target = target;
        }

        if (target->IsDead() ||
            owner_->IsFriend(target) != USER_RELATION_ENEMY)
        {
            continue;
        }
        // PK 공격이라면 관련처리를 한다.
        owner_->ForcedAttack(target);
        if (target->CanBeAttacked() == false) {
            continue;
        }
        // 스타일 이펙트 및 타겟위치 셋팅
        skill_info_data_.skill_effect_ = style_.common_effect;
        style_.target_pos = *target->GetPosPtr();
        style_.thrust_pos = style_.target_pos;

        // 데미지를 계산한다.
        if (CalcDamage(target))
        {   // 만약 어빌리티를 처리하는 중에 자기자신이 죽어서 스킬이 초기화될 경우!
            //(절대 발생하면 안되지만 예외처리!)
            if (owner_ == NULL)
            {
                SUNLOG(eCRITICAL_LOG, "[StyleSkill::CalcDamage] SkillCode[%d] owner_ is NULL!", \
                       GetSkillCode());
                return;
            }

            // 공격이 성공했을 경우
            target->Attacked();

            //_NA_0_20100211_SD_EFFECT_MODIFY
            SendAIMessage(target, style_.damage);

            // 이펙트 효과 적용
            ApplyEffect(target);
        }

        //__NA001390_090915_RIDING_SYSTEM__
        StatusManager* status_manager = target->GetStatusManager();
        BOOLEAN target_riding = status_manager->Status.IsRidingRider();

        // (CHANGES) 넉백등의 동작을 일으키는 상황만 제거하는 방향으로 수정한다.
        //_NA001385_20090924_DOMINATION_STAMP_STYLE_SKILL_BUG
        const BOOLEAN target_stamp = (status_manager->FindStatus(eCHAR_STATE_STAMP) == NULL) ? 0 : 1;
        if (target_riding || target_stamp) 
        {
            skill_info_data_.skill_effect_ &= ~(SKILL_EFFECT_KNOCKBACK |
                                                SKILL_EFFECT_KNOCKDOWN |
                                                SKILL_EFFECT_STUN);
        };
        ;{ //align msg
            STYLE_ATTACK_RESULT_INFO& attack_info = msg_brd.AttackInfo[number_of_hit_targets_];
            attack_info.dwTargetKey = target_object_key;
            //_NA_0_20100211_SD_EFFECT_MODIFY
            attack_info.wDamage     = style_.damage;
            attack_info.dwTargetHP  = target->GetHP();
            attack_info.wvCurPos    = style_.target_pos;
            attack_info.wvDestPos   = style_.thrust_pos;
            attack_info.byEffect    = skill_info_data_.skill_effect_;
            attack_info.byEtherComboCount = ether_combo_count_;      
#ifdef _NA_006638_20120415_ADD_SHIELD_INFO_IN_TARGET_INFO
            attack_info.target_sd = target->GetSD();
#endif // _NA_006638_20120415_ADD_SHIELD_INFO_IN_TARGET_INFO
        }

        ++number_of_hit_targets_;

        if (base_style_info_->m_wMaxTargetNo &&
            number_of_hit_targets_ >= base_style_info_->m_wMaxTargetNo)
        {
            break;
        }
    }

    // 캐릭터에 걸어준 속성등을 해제한다.
    CommonRelease();
    // 공격하면 MP, HP가 회복되는 캐릭터가 있다.
    //_NA_0_20100211_SD_EFFECT_MODIFY
    owner_->OnAttack(main_target, skill_info_data_.skill_code_, style_.main_target_damage);

    msg_brd.byNumberOfTargets = static_cast<uint8_t>(number_of_hit_targets_);
    owner_->SendPacketAroundForSync(&msg_brd);
    Skill::EndExecute();
}

BOOL
StyleSkill::CheckMainTarget(BattleResult* const detailed_result)
{
    BattleResult battle_result = { 0, };
    BattleResult* result_handle = detailed_result ? detailed_result : &battle_result;
    result_handle->result_code = RC::RC_BATTLE_SUCCESS;
    //
    if (game_field_ == NULL)
    {
        SUNLOG(eCRITICAL_LOG, "[StyleSkill::CheckMainTarget] GameField is NULL!");
        result_handle->result_code = RC::RC_BATTLE_INVALID_MAINTARGET;
        return false;
    }

    Character* main_target = game_field_->FindCharacter(skill_info_data_.main_target_key_);
    if (main_target == NULL) {
        result_handle->result_code = RC::RC_BATTLE_INVALID_MAINTARGET;
        return false;
    }
    // 메인타겟 좌표를 설정한다.
    skill_info_data_.main_target_pos_ = *main_target->GetPosPtr();
    // 관통 스타일인 경우 메인타겟 좌표를 메인타겟을 관통한 뒤편 좌표로 다시 구한다.
    if (base_style_info_->m_byAttRangeform == SRF_PIERCE)
    {
        WzVector& vDiff = skill_info_data_.main_target_pos_ - skill_info_data_.cur_pos_;
        VectorNormalize(&vDiff, &vDiff);
        skill_info_data_.main_target_pos_ += vDiff * base_style_info_->m_fStyleArea;
    }
    else
    { // 메인타겟에 대해 스킬 사거리를 체크한다.
        BOOLEAN skill_range_check = \
            !!owner_->CheckAttackRange(main_target, skill_info_data_.main_target_pos_);
        if (skill_range_check == false) {
            result_handle->result_code = RC::RC_BATTLE_OUT_OF_RANGE;
            return false;
        }
    }

    return true;
}

void
StyleSkill::CommonExecute()
{
    // 스킬 구사중 맞지 않도록 해야 하는 경우 비젼을 꺼준다.
    BYTE attribution = GetAttribution();
    if (attribution & SKILL_ATTRIBUTION_INVISIBLE)
    {
        StatusManager* status_manager = owner_->GetStatusManager();
        status_manager->AllocStatus(eCHAR_STATE_ETC_DISABLE_VISION);
    }
    // 일단 녹백은 타겟 NPC 전체에 같은 확률로 적용하기 위해 미리 계산
    if (base_style_info_->m_fKnockBackRate != 0.0f &&
        IsOccuredByDraw(base_style_info_->m_fKnockBackRate))
    {
        style_.common_effect |= SKILL_EFFECT_KNOCKBACK;
    }
}

void
StyleSkill::CommonRelease()
{
    // 플레이어가 스킬 구사중 몬스터의 시야에서 사라지는 스킬일 경우 Vision 속성을 켜준다.
    BYTE attribution = GetAttribution();
    if (attribution & SKILL_ATTRIBUTION_INVISIBLE)
    {
        StatusManager* status_manager = owner_->GetStatusManager();
        status_manager->Remove(eCHAR_STATE_ETC_DISABLE_VISION);
    }
}

bool StyleSkill::FindTargets(ns_object::SearchOption::SingleSearch::result_type* const search_result)
{
    //_NA000000_110210_ENHANCED_TARGET_SEARCH_ALGORITHM
    search_result->number_of_selected_chars = 0;
    assert(game_field_ != NULL);

    Character* main_target = game_field_->FindCharacter(skill_info_data_.main_target_key_);

    const eSKILL_AREA_TYPE forms_of_application =
        static_cast<eSKILL_AREA_TYPE>(base_style_info_->m_byAttRangeform);

    int max_number_of_target_select = base_style_info_->m_wMaxTargetNo;
    if (max_number_of_target_select == 1 ||
        base_style_info_->m_byAttRangeform == SRF_FOWARD_ONE)
    {   // select one
        max_number_of_target_select = 1;
    };
    const WzVector* main_target_pos = (main_target == NULL) ?
        &skill_info_data_.main_target_pos_ : NULL;

    // result->targets sequence
    //  [MainTarget] | [OtherEnemy] | ...
    ;{
        ns_object::SearchOption::SingleSearch search_option(
            max_number_of_target_select,//int max_number_of_selected_characters,
            owner_, //Character* input_actor,
            main_target, //Character* input_main_target_if_exist_it,
            main_target_pos, //const WzVector* input_specific_application_center_pos_if_main_target_is_not_used_to_pos,
            //
            owner_->GetAttackRange(), //float attack_ranges,
            base_style_info_->m_fStyleArea, //float ranges_of_application,
            forms_of_application); //eSKILL_AREA_TYPE forms_of_application
        bool success = game_field_->FindTargetsUsingSingleSearch(search_option, search_result);
        if (success == false) {
            return false;
        };
    };
    return true;    
}

BOOL
StyleSkill::CalcDamage(Character* pTarget)
{
    const eATTACK_TYPE attack_type_weapon = owner_->GetWeaponBaseAttType();
    { // calculate default, 'damage'
#ifdef _NA001605_EP2_ATTRIBUTES_RENEWAL_
        style_.damage = ep2::CalcNormalDamage(owner_, pTarget,
            attack_type_weapon, true, base_style_info_->m_wCriticalBonus,
            &skill_info_data_.skill_effect_);
#else
        const eATTACK_TYPE attack_type_magic = owner_->GetWeaponMagicalAttType();

        style_.damage = ep1::CalcNormalDamage(owner_, pTarget,
                                 attack_type_weapon,
                                 attack_type_magic,
                                 base_style_info_->m_wCriticalBonus,
                                 &skill_info_data_.skill_effect_,
                                 base_style_info_->m_fDefenseIgnore);
#endif
    };
    ;{ // calculate 2nd
        // formula = normal_damage * (타수에 따른 추가 데미지 비율) + (타수에 따른 순수 추가 데미지)
        float probability = (1 + base_style_info_->m_fDamagePercent[2]);

        const int addDamage = base_style_info_->m_iAddDamage[2];
        //_NA_0_20100211_SD_EFFECT_MODIFY
        style_.damage = static_cast<DAMAGETYPE>(style_.damage * probability) + addDamage;

    };

    if (is_applied_ether_bullet_option_ && applied_ether_bullet_info_) {
        assert(SAFE_NUMERIC_TYPECAST(DWORD, applied_ether_bullet_info_->m_dwEffect, BYTE));
        skill_info_data_.skill_effect_ |= applied_ether_bullet_info_->m_dwEffect; // warning, type overflow
    }
    const DWORD target_key = pTarget->GetObjectKey();
    //
    if (BOOLEAN need_apply_splash_damage = skill_info_data_.main_target_key_ != target_key)
    {   // calculate splash damage
        // formula : 발키리 에테르 스타일인 경우 스플래쉬 데미지 30%만 적용
        //           일반 스타일 스플래쉬 데미지 50% 적용
        static const SERVER_COMMON_OPTION& kServerOption = \
            ServerOptionParserEx::Instance()->GetServerOption();
        const float splash_damage_ratio = \
            base_style_info_->m_SkillClassCode == eSTYLE_ETHER_NORMAL
            ?   kServerOption.m_fETHER_SPLASH_DAMAGE_RATIO
            :   kServerOption.m_fSTYLE_SPLASH_DAMAGE_RATIO;
        //_NA_0_20100211_SD_EFFECT_MODIFY
        style_.damage = static_cast<DAMAGETYPE>(style_.damage * splash_damage_ratio);

    }
    // apply damage to target
    //  remove '__BUG_2572_SPIRIT_SLUG_BUG' (0817)
    // f100908.1L, f110104.3L, change attack type 'eATTACK_TYPE_MELEE'
#ifdef _NA_006728_20130520_SKILL_BALANCE_MODIFY
    DamageArgs damage_args(owner_, attack_type_weapon, GetSkillCode());
#else
    DamageArgs damage_args(owner_, attack_type_weapon);
#endif // _NA_006728_20130520_SKILL_BALANCE_MODIFY
    ;{
        damage_args.option_.is_critical_ = !!(skill_info_data_.skill_effect_ & SKILL_EFFECT_CRITICAL);
        //damage_args.option_.is_mirror_;
        //damage_args.limit_hp_;
        //damage_args.attack_regist_kind_;
        damage_args.sd_apply_ = eSD_APPLY_DO;
        //_NA_0_20100211_SD_EFFECT_MODIFY
        damage_args.damage_ = style_.damage;
        //damage_args.damage_sd_;
    };
    ;{
        SkillExecutionTransaction transaction; // CHANGES: f110303.3L
        pTarget->Damaged(&damage_args);
    };
    style_.damage = damage_args.damage_;
    // MainTarget인 경우
    if (skill_info_data_.main_target_key_ == target_key) {
        style_.main_target_damage = style_.damage;
    }

    return style_.damage != 0;
}

void
StyleSkill::ApplyEffect(Character* pTarget)
{
    StatusManager* status_manager = pTarget->GetStatusManager();
    BOOL knockdown_execute = false;
    BOOLEAN target_riding = status_manager->Status.IsRidingRider();
    // 관통
    if (base_style_info_->m_fPierceRate != 0.0f &&
        IsOccuredByDraw(base_style_info_->m_fPierceRate))
    {
        skill_info_data_.skill_effect_ |= SKILL_EFFECT_PIERCE;
    }
    // 스턴
    if (target_riding == false &&
        base_style_info_->m_fStunRate != 0.0f &&
        IsOccuredByDraw(base_style_info_->m_fStunRate))
    {
        skill_info_data_.skill_effect_ |= SKILL_EFFECT_STUN;
        status_manager->AllocStatus(eCHAR_STATE_STYLE_STUN, base_style_info_->m_wStunTime, 0);
    }
    // 다운
    if (target_riding == false &&
        base_style_info_->m_fDownRate != 0.0f &&
        IsOccuredByDraw(base_style_info_->m_fDownRate))
    {
        skill_info_data_.skill_effect_ |= SKILL_EFFECT_KNOCKDOWN;
        knockdown_execute = true;
    }

    static const AI_COMMON_INFO& kAI_Common = AIParamParser::Instance()->GetCommonInfo();

    // 밀려야 하는 경우의 처리(밀린 후 다운 될 것인가의 정보도 포함)
    if (target_riding == false && (skill_info_data_.skill_effect_ & SKILL_EFFECT_KNOCKBACK))
    {
        WzVector wvNormalVectorP2M;
        WzVector& diffVec = style_.target_pos - style_.player_pos;
        VectorNormalize(&wvNormalVectorP2M, &diffVec);

        wvNormalVectorP2M *= base_style_info_->m_fKnockBackRange;
        if (pTarget->ExecuteThrust(true, &wvNormalVectorP2M, style_.thrust_pos,
                                   base_style_info_->m_fKnockBackRange,
                                   knockdown_execute))
        {
            const ALLOC_STATUS_PAIR result_pair = \
                status_manager->AllocStatus(eCHAR_STATE_STYLE_THRUST, kAI_Common.m_wTHRUST_TIME);
            // 다운은 Thrust 상태 내부에서 처리하도록 한다.
            if (result_pair.first && result_pair.second)
            {
                EtcStatus* etc_status = static_cast<EtcStatus*>(result_pair.second);
                etc_status->SetDownTime(kAI_Common.m_wKNOCKDOWN_TIME);
            }
            knockdown_execute = false;
        }
        else
        {
            // 지형 밀리기가 실패한 경우!!
            skill_info_data_.skill_effect_ &= ~SKILL_EFFECT_KNOCKBACK;
        }
    }

    // 밀리지는 않고 다운만 되는 경우
    if (target_riding == false && knockdown_execute) {
        status_manager->AllocStatus(eCHAR_STATE_STYLE_DOWN, kAI_Common.m_wKNOCKDOWN_TIME, 0);
    }
}

void
StyleSkill::Broadcast_Style_Result()
{
    ;{
        MSGSUB_STYLE_PLAYER_ATTACK_RESULT_BRD msg_brd;
        msg_brd.Clear();
        msg_brd.dwClientSerial    = skill_info_data_.client_serial_;
        msg_brd.dwAttackerKey     = owner_->GetObjectKey();
        msg_brd.byNumberOfTargets = static_cast<uint8_t>(number_of_hit_targets_);
        owner_->SendPacketAroundForSync(&msg_brd);
    };
}
