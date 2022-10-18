#include "stdafx.h"
#include "Ability.h"

#include <PublicMath.h>
#include <SkillAttrCalculator.h>
#include <StateInfoParser.h>
#include <SkillInfoParser.h>

#include "AbilityStatus.h"
#include "Skill.h"
#include "StateInfoParser.h"
#include "StatusManager.h"
#include "CharacterFormula.h"
#ifdef _NA_007667_20141001_WITCHBLADE_SKILL
#include "Summoned.h"
#endif //_NA_007667_20141001_WITCHBLADE_SKILL


AbilityStatus::AbilityStatus() : 
    is_apply_(false), 
    ability_value_type_(0), 
    ability_value_(0), 
    sum_value_(0), 
    skill_code_(0), 
    ability_id_(eABILITY_INVALID), 
    ability_index_(0), 
    attr_id_(eATTR_TYPE_INVALID), 
    totem_radius_(0), 
    ability_event_code_(0), 
    attacker_(NULL), 
    ability_info_(NULL), 
    m_iOption1(0), 
    m_iOption2(0)
{
}

AbilityStatus::~AbilityStatus()
{
}

void AbilityStatus::Init(Character* owner, Character* attacker, Ability* ability)
{
    is_apply_ = false;
    attacker_ = attacker;

    const Skill* const skill = ability->GetSKill();
    if (skill != NULL)
    {
        const SkillInfo* const skill_info = skill->GetSkillInfo();
        RegisterSkillOption(skill_info->skill_option_);
    }
    else
    {
        static const SkillOption skill_option = { 0, };
        RegisterSkillOption(skill_option);
    }

    const BASE_ABILITYINFO* const ability_info = ability->GetBaseAbilityInfo();

    skill_code_ = ability->GetSkillCode();
    ability_id_ = (eABILITY)ability_info->m_wAbilityID;
    ability_index_ = ability_info->m_byIndex;
    attr_id_ = ability->GetAttrID();

    const eCHAR_STATE_TYPE state_id = ability->GetStateID();
    int apply_time = ability_info->m_iParam[2];

    if (skill != NULL && skill->GetSkillInfo()->skill_factor_ == SKILL_FACTOR_EFFECT)
    {
        // 이펙트에 의한 스킬이면 버프증가/디버프감소 효과를 적용하지 않는다.
    }
    else
    {
        const int apply_policy = GetDurationControlWithRelation(
            owner, attacker, state_id, ability_id_, apply_time);
        if (apply_policy < 0)
        {
            //apply_policy가 -1이면 abnormal, weaken 등, 디버프계열
            //자신에게 걸리는 디버프 효과 약화 (지속시간 감소)
            int changed = owner->GetAttr().GetDecreaseSkillDuration() * 1000;
            changed = apply_time + changed;
            apply_time = max(0, changed);

#ifdef _NA_008486_20150914_TOTAL_BALANCE
            if (attacker != NULL)
            {
                //공격자의 디버프효과 강화 (지속시간 증가)
                INT add_time = attacker->GetAttr().GetDebuffDuration(); //밀리세컨드단위이므로 1000 안곱해도됨
                apply_time += add_time;
            }
#endif //_NA_008486_20150914_TOTAL_BALANCE        
        }
        else if (apply_policy > 0)
        {
            int changed = owner->GetAttr().GetIncreaseSkillDuration() * 1000;
            apply_time += changed;
        }

        // 연계 스킬 사용으로 인한 추가 상태 지속시간을 계산한다.
        if (attacker != NULL && skill != NULL)
        {
            const int combo_apply_time = 
                ns_formula::ability::CalcComboApplyTime(*attacker, *skill, state_id);
            if (combo_apply_time != 0)
            {
                apply_time += combo_apply_time;
            }
        }
    }

    BaseStatus::Init(owner, static_cast<WORD>(state_id), apply_time, ability_info->m_iParam[3]);

    m_iOption1 = ability_info->m_iOption1;
    m_iOption2 = ability_info->m_iOption2;
    ability_value_type_ = ability_info->m_iParam[0];
    ability_value_ = ability_info->m_iParam[1];
    sum_value_ = 0;
    totem_radius_ = 0;
    ability_info_ = ability_info;

    if (attacker_ && 
        attacker->IsEqualObjectKind(TOTEMNPC_OBJECT) &&
        attacker->IsTotemSkillAreaType())
    {
        const SkillScriptInfo* const skill_script_info = 
            SkillInfoParser::Instance()->GetSkillInfo(skill_code_);
        if (skill_script_info != NULL)
        {
            totem_radius_ = skill_script_info->m_wSkillArea / 10.f;
        }
    }
    ability_event_code_ = ability->GetEventCode();
}

void AbilityStatus::Init(
    Character* const owner, 
    const WORD state_id,
    const BASE_ABILITYINFO* base_ability_info, 
    const SLOTCODE skill_code,
    const int application_time, 
    const int period)
{
    static Ability temp_ability;
    temp_ability.InitDetailed(NULL, skill_code, NULL, 0, base_ability_info);
    Init(owner, NULL, &temp_ability);

    BaseStatus::Init(owner, state_id, application_time, period);
}

BOOL AbilityStatus::Update(DWORD cur_time)
{
    if (totem_radius_)
    {
        const Character* const attacker = GetAttacker();
        if (attacker == NULL)
        {
            return FALSE;
        }
        if (attacker->IsDead())
        {
            return FALSE;
        }
        const Character* const owner = GetOwner();
        if (owner == NULL)
        {
            return FALSE;
        }
        const WzVector& owner_pos = *owner->GetPosPtr();
        const WzVector& totem_pos = *attacker->GetPosPtr();
        const WzVector diff_pos = owner_pos - totem_pos;
        const float distance_sq = VectorLength2(&diff_pos);
        if (distance_sq >= totem_radius_ * totem_radius_)
        {
            return FALSE;
        }
    }

    return BaseStatus::Update(cur_time);
}

void AbilityStatus::Start()
{
    BaseStatus::Start();

    if (BaseStatus::IsPeriodicStatus())
    {
        return;
    }
        
    if (attr_id_ == eATTR_CUR_HP)
    {
#ifdef _NA_006702_20130506_ADD_ABILITYTYPE_PUREVALUE_HP
        //스킬공격력 적용여부
        bool is_increase = true; 
        if(GetAbilityID() == eABILITY_CUR_HP_INCREASE)
            is_increase = true;
        else if(GetAbilityID() == eABILITY_VARIATION_PUREVALUE_HP)
            is_increase = false;
        ChangeHP(is_increase, ability_value_type_, ability_value_, m_iOption2);
#else
        ChangeHP(true, ability_value_type_, ability_value_, m_iOption2);
#endif //_NA_006702_20130506_ADD_ABILITYTYPE_PUREVALUE_HP
    }
    else if (attr_id_ == eATTR_CUR_MP)
    {
        ChangeMP(true, ability_value_type_, ability_value_);
    }
    else if (attr_id_ == eATTR_CUR_SD) {
        ChangeSD(true, ability_value_type_, ability_value_);
    }
    else if (attr_id_ != eATTR_TYPE_INVALID)
    {
        Character* const owner = GetOwner();
        if (owner == NULL)
        {
            return;
        }

        SkillAttrCalculator calc(owner->GetAttr());
        calc.AddAttr(attr_id_, ability_value_type_, ability_value_);

        sum_value_ += ability_value_;
        is_apply_ = true;
    }
}

void AbilityStatus::Execute()
{
    if (attr_id_ == eATTR_CUR_HP)
    {
#ifdef _NA_006702_20130506_ADD_ABILITYTYPE_PUREVALUE_HP
        //스킬공격력 적용여부
        bool is_increase = true; 
        if(GetAbilityID() == eABILITY_CUR_HP_INCREASE)
            is_increase = true;
        else if(GetAbilityID() == eABILITY_VARIATION_PUREVALUE_HP)
            is_increase = false;
        ChangeHP(is_increase, ability_value_type_, ability_value_, m_iOption2);
#else
        ChangeHP(true, ability_value_type_, ability_value_, m_iOption2);
#endif //_NA_006702_20130506_ADD_ABILITYTYPE_PUREVALUE_HP
    }
    else if (attr_id_ == eATTR_CUR_MP)
    {
        ChangeMP(true, ability_value_type_, ability_value_);
    }
    else if (attr_id_ == eATTR_CUR_SD) {
        ChangeSD(true, ability_value_type_, ability_value_);
    }
    else if (attr_id_ != eATTR_TYPE_INVALID)
    {
        Character* const owner = GetOwner();
        if (owner == NULL)
        {
            return;
        }

        SkillAttrCalculator calc(owner->GetAttr());
        calc.AddAttr(attr_id_, ability_value_type_, ability_value_);

        sum_value_ += ability_value_;
        is_apply_ = true;
    }

    BaseStatus::Execute();
}


void AbilityStatus::End()
{
    BaseStatus::End();

    // 상태적용이 끝났음을 클라이언트에게 알려준다.
    if (GetStateID() != eCHAR_STATE_INVALID)
    {
        SendStatusDelBRD();
    }

    // Buff 계열의 어빌리티들을 처리한다.
    if (is_apply_ && attr_id_ != eATTR_TYPE_INVALID)
    {
        Character* const owner = GetOwner();
        if (owner == NULL)
        {
            return;
        }

        SkillAttrCalculator calc(owner->GetAttr());
        calc.DelAttr(attr_id_, ability_value_type_, sum_value_);
    }
}

void AbilityStatus::ChangeHP(
    const bool is_increse, const int value_type, const int in_value, const int in_hp_limit)
{
    Character* const owner = GetOwner();
    if (owner == NULL)
    {
        return;
    }

    int result_value = in_value;
    {
        if (value_type == VALUE_TYPE_PERCENT_PER_MAX)
        {
            result_value = (int)owner->GetMaxHP() * in_value / 1000;
        }
        else if (value_type == VALUE_TYPE_PERCENT_PER_CUR)
        {
            result_value = (int)owner->GetHP() * in_value / 1000;
        }
#ifdef _NA_006702_20130506_ADD_ABILITYTYPE_PUREVALUE_HP
#else
        if (is_increse == false)  
        {
            result_value = -result_value;
        }
#endif //_NA_006702_20130506_ADD_ABILITYTYPE_PUREVALUE_HP
    }

    if (is_increse)
    {
        const Character* const skill_user = GetAttacker();
        if (skill_user != NULL)
        {
            // 스킬 비율형 추가 데미지를 회복량 추가 비율으로 계산한다.
#ifdef _NA_006606_VALKYRIES_SUMMONS_HEAL_SKILL_DONOT_APPLY_ADD_SKILL_RATIO
            const Attributes& skill_user_attr = \
                skill_user->IsEqualObjectKind(SUMMON_OBJECT) ? owner->GetAttr() : skill_user->GetAttr();
#else
            const Attributes& skill_user_attr = skill_user->GetAttr();
#endif // _NA_006606_VALKYRIES_SUMMONS_HEAL_SKILL_DONOT_APPLY_ADD_SKILL_RATIO
            const int add_skill_ratio = skill_user_attr.GetSkillPercentDamage();
            if (add_skill_ratio != 0)
            {
                const int calc_skill_ratio = 
                    static_cast<int>(static_cast<float>(in_value) * (static_cast<float>(add_skill_ratio) / 100.0f));
                if (calc_skill_ratio != 0)
                {
                    result_value += calc_skill_ratio;
                }
            }

            // 회복량 증가 어빌리티 적용 여부를 검사한다.
            const int add_value = ns_formula::ability::CalcIncreseHeal(
                IncreseHealAbilityType::kSkill, result_value, *skill_user);
            if (add_value != 0)
            {
                result_value += add_value;
            }
        }
    }

    int hp_limit = in_hp_limit;
    if (owner->IsEqualObjectKind(NPC_OBJECT))
    {
        NPC* const npc = (NPC*)owner;
        if (npc->GetGrade() == eNPC_CRYSTAL_WARP || npc->GetGrade() == eNPC_DOMINATION_MAPOBJECT_NPC)
        {
            hp_limit = 100;
        }
    }

    const int hp_limit_ratio = BOUND_CHECK(0, hp_limit, 100);
    float calc_hp_limit = (float)hp_limit;
    {
        if (value_type == VALUE_TYPE_PERCENT_PER_MAX)
        {
            calc_hp_limit = owner->GetMaxHP() * hp_limit_ratio / 100.f;
        }
        else if (value_type == VALUE_TYPE_PERCENT_PER_CUR)
        {
            calc_hp_limit = owner->GetHP() * hp_limit_ratio / 100.f;
        }
    }
        
    // HP가 감소하는 경우는 데미지를 입는 경우이다.
    if (result_value < 0)
    {
        Character* const attacker = GetAttacker();
        if (attacker == NULL)
        {
            return;
        }
#ifdef _NA_006728_20130520_SKILL_BALANCE_MODIFY
        DamageArgs damage_args(attacker, eATTACK_TYPE_MELEE, GetSkillCode());
#else
        DamageArgs damage_args(attacker, eATTACK_TYPE_MELEE);
#endif // _NA_006728_20130520_SKILL_BALANCE_MODIFY
        {
            damage_args.limit_hp_ = static_cast<int>(calc_hp_limit);
            damage_args.attack_regist_kind_ = eATTACK_RESIST_SKILL;
            const BASE_STATEINFO* const state_info = 
                StateInfoParser::Instance()->GetStateInfo(GetStateID());
            if (state_info != NULL) 
            {
                damage_args.sd_apply_ = (eSD_APPLY)state_info->m_bySDApply;
            }
            damage_args.damage_ = -result_value;
        }
#ifdef _NA_006702_20130506_ADD_ABILITYTYPE_PUREVALUE_HP
        if(GetAbilityID() == eABILITY_VARIATION_PUREVALUE_HP)
            owner->DecreaseHP(damage_args.damage_, damage_args.limit_hp_);
        else if(GetAbilityID() == eABILITY_CUR_HP_INCREASE)
            owner->Damaged(&damage_args);
#else
        owner->Damaged(&damage_args);
#endif //_NA_006702_20130506_ADD_ABILITYTYPE_PUREVALUE_HP
        
        MSGSUB_SKILL_PERIODIC_DAMAGE_BRD msg_brd;
        {
            msg_brd.Clear();
            msg_brd.m_dwAttackerKey = attacker->GetObjectKey();
            msg_brd.m_SkillCode = skill_code_;
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
    }
    else if (result_value > 0)
    {
        // 실제 회복을 시킨다.
        // _NA_0_20091118_GAMEROOM_BATTLE_RESULT
        Character* const attacker = GetAttacker();
        owner->OnRecover(result_value, 0, 0, eRECOVER_TYPE_NORMAL, attacker);
    }
}

void AbilityStatus::ChangeMP(const bool is_increase, const int value_type, const int in_value)
{
    Character* const owner = GetOwner();
    if (owner == NULL)
    {
        return;
    }

    int result_value = in_value;
    {
        if (value_type == VALUE_TYPE_PERCENT_PER_MAX)
        {
            result_value = (int)owner->GetMaxMP() * in_value / 1000;
        }
        else if (value_type == VALUE_TYPE_PERCENT_PER_CUR)
        {
            result_value = (int)owner->GetMP() * in_value / 1000;
        }
        if (is_increase == false)
        {
            result_value = -result_value;
        }
    }
        
    owner->OnRecover(0, result_value);
}

void AbilityStatus::ChangeSD(const bool is_increase, const int value_type, const int in_value)
{
    Character* const owner = GetOwner();
    if (owner == NULL) {
        return;
    }

    int result_value = in_value;
    {
        if (value_type == VALUE_TYPE_PERCENT_PER_MAX) {
            result_value = (int)owner->GetMaxSD() * in_value / 1000;
        }
        else if (value_type == VALUE_TYPE_PERCENT_PER_CUR) {
            result_value = (int)owner->GetSD() * in_value / 1000;
        }

        if (is_increase == false) {
            result_value = -result_value;
        }
    }

    owner->OnRecover(/*HP*/0, /*MP*/0, /*SD*/result_value);
}

bool AbilityStatus::ComparePriority(const Ability* const ability) const
{
    // 민경승 / 상태 버프 우선순위 비교 함수
    // return true --> 현재 걸려있는 버프를 유지한다.
    //return false --> 현재 사용하여 적용시키려 하는 버프로 대체한다.
    // current_ability_value -> 현재 걸려있는 버프 / use_ability_value -> 사용하여 적용시키려 하는 버프
    if (GetExpiredTime() == BASE_EXPIRE_TIME_INFINITY) 
    {
        return true;
    }

    const eATTR_TYPE attr_type = ability->GetAttrID();
    const BASE_ABILITYINFO* const ability_info = ability->GetBaseAbilityInfo();
    if (attr_type == eATTR_TYPE_INVALID)
    {
        const eATTR_EXTRA_OPTION extra_option = ability_info->GetAttrExtraOption();
        if (extra_option == eATTR_EXTRA_OPTION_INVALID) 
        {
            return false;
        }
    }

    const int value_type = ability_info->m_iParam[0];
    if (value_type != ability_value_type_)
    {
        return false;
    }
    
#ifdef _NA_006686_20130502_REMAKE_BUFF_PRIORITY
    // [1] 두 버프의 value값을 비교한다
    const double current_ability_value = ability_value_;
    const double new_use_ability_value = ability_info->m_iParam[1];

    return (abs(current_ability_value) > abs(new_use_ability_value));
#else
    double sum_value1 = ability_value_;
    {
        const DWORD expire_time = GetExpiredTime();
        const DWORD current_tick = GetTickCount();
        const DWORD remain_tick = (expire_time > current_tick) ? (expire_time - current_tick) : 0;
        if (GetApplicationTime() != 0)
        {
            const int period_time = GetPeriodTime();
            if (period_time != 0)
            {
                sum_value1 *= remain_tick / period_time;
            }
            else
            {
                sum_value1 *= remain_tick;
            }
        }
    }
    
    double sum_value2 = 0.0f;
    {
        const int in_value = ability_info->m_iParam[1];
        sum_value2 = in_value;
        
        const int apply_time = ability_info->m_iParam[2];
        if (apply_time != 0)
        {
            const int period = ability_info->m_iParam[3];
            if (period != 0)
            {
                sum_value2 *= apply_time / period;
            }
            else
            {
                sum_value2 *= apply_time;
            }
        }
    }

    return abs(sum_value1) > abs(sum_value2);
#endif //_NA_006686_20130502_REMAKE_BUFF_PRIORITY
}


// (CHANGES) (f100309.1L) changes interface specification regard with relation.
// if result == 0 then no apply
// if result <  0 then target be applied bad status, calculate decrease skill duration option
// if result >  0 then target be applied good status, calculate increase skill duration option
int AbilityStatus::GetDurationControlWithRelation(
    const Character* const owner, 
    Character* attacker,
    const eCHAR_STATE_TYPE in_char_state,
    const eABILITY ability_id,
    const int application_time) const
{
    enum { kNoApplication = 0 };
    enum { kApplyDecrease = -1 };
    enum { kApplyIncrease = +1 };

    eCHAR_STATE_TYPE char_state = in_char_state;
    if (char_state == eCHAR_STATE_INVALID) 
    {
        char_state = static_cast<eCHAR_STATE_TYPE>(GetStateID());
    }
    
#ifdef _NA_007667_20141001_WITCHBLADE_SKILL
    //고정형 소환수의 스킬은 버프/디버프효과 제외
    if (attacker != NULL && attacker->GetSummonType() == eSUMMON_IMMOVABLE)
    {
        return kNoApplication;
    }
#endif //_NA_007667_20141001_WITCHBLADE_SKILL

    const BASE_STATEINFO* const state_info = StateInfoParser::Instance()->GetStateInfo(char_state);
    if (state_info == NULL) 
    {
        if (application_time == 0)
        {
            return kNoApplication;
        }
        return kApplyIncrease; //return kNoApplication;
    };

    // CHANGES: f110607.2L, prevent the infinity time expiration value modification problem.
    if (application_time < 0) 
    {
        return kNoApplication;
    };
    
    const eSTATE_TYPE state_category = static_cast<eSTATE_TYPE>(state_info->m_byType);
    if (state_category == eSTATE_TYPE_SPECIALITY) 
    {
        return kNoApplication;
    }
    
    const bool default_bad_status = 
        state_category == eSTATE_TYPE_ABNORMAL || state_category == eSTATE_TYPE_WEAKENING;
    if (owner == attacker) // self application
    {
        if (default_bad_status) 
        {
            return kApplyDecrease;
        }
        return kApplyIncrease;
    }

    // assertion (invoker != target)
    // special exception by hard coding about 'divine emblem' based on requested by planner
    if (ability_id == eABILITY_BONUS_DAMAGE_PER_ATTACK) 
    {
        return kNoApplication;
    }
    
    if (default_bad_status) 
    {
        return kApplyDecrease;
    }

    return kApplyIncrease;
}