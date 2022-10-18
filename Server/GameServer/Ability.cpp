#include "stdafx.h"
#include "Ability.h"
#include "StateInfoParser.h"
#include "SkillInfoParser.h"
#include "Battle.h"

#include "GameZone.h"
#include "Skill.h"
#include "CharacterFormula.h"
#include "StatusManager.h"
#include "./MovingPlanner.h"

void Ability::Init(Skill* skill, const BASE_ABILITYINFO* ability_info)
{
    skill_ = skill;
    
    Character* const owner = skill->GetOwner();
    const SLOTCODE skill_code = skill->GetSkillCode();
    SkillInfo* const skill_info = skill->GetSkillInfo();
    const SkillScriptInfo* script_info = skill->GetSkillBaseInfo();
    const BYTE skill_stat_type = script_info->m_bySkillStatType;
    
    InitDetailed(owner, skill_code, skill_info, skill_stat_type, ability_info);
}

void Ability::InitDetailed(
    Character* attacker,
    SLOTCODE skill_code, 
    SkillInfo* skill_info,
    BYTE skill_stat_type,
    const BASE_ABILITYINFO* ability_info)
{
    __UNUSED((skill_info, skill_stat_type));
    skill_code_ = skill_code;
    base_ability_info_ = ability_info;
    attacker_= attacker;
    event_code_ = 0;
}

bool Ability::Execute(Character* target, BYTE* msg, WORD& msg_size)
{
    if (GetAttacker() == NULL)
    {
        return false;
    }
    if (target != NULL && target->IsDeleted())
    {
        return false;
    }
    if (msg != NULL)
    {
        SKILL_RESULT_CODE* const result_msg = (SKILL_RESULT_CODE*)msg;
        result_msg->m_wAbilityIndex = GetIndex();
        msg_size = result_msg->GetSize();
        result_msg->m_wStatusCode = 0;
        result_msg->m_iStatusTime = 0;
#ifdef _NA_007667_20141001_WITCHBLADE_SKILL
        result_msg->overlap_status_count = 0;
#endif //_NA_007667_20141001_WITCHBLADE_SKILL
    }
    return true;
}

bool Ability::IsValidStateID() const
{
    const eCHAR_STATE_TYPE state_type = GetStateID();
    if (state_type != eCHAR_STATE_INVALID)
    {
        return false;
    }
    return true;
}

bool Ability::CanExecute(
    Character* attacker, Character* target, DWORD main_target_key, WzVector /*main_target_pos*/)
{
    if (base_ability_info_ == NULL) 
    {
        return false;
    }

    const eABILITY_RANGE_TYPE range_type = eABILITY_RANGE_TYPE(base_ability_info_->m_byRangeType);
    const DWORD range_type_bitset = (1 << range_type);
    
    const StatusManager* const attacker_status_manager = attacker->GetStatusManager();
    const StatusField& attacker_status_field = attacker_status_manager->Status;
    if (attacker_status_field.IsPassConstraint(attacker, base_ability_info_) == false) 
    {
        return false;
    }
    const DWORD apply_check_canbe_attacked = 
        (1 << SKILL_ABILITY_ENEMY) | 
        (1 << SKILL_ABILITY_TARGETAREA_ENEMY) | 
        (1 << SKILL_ABILITY_MYAREA_ENEMY);
    if ((range_type_bitset & apply_check_canbe_attacked) && !target->CanBeAttacked()) 
    {
        return false;
    }

    const DWORD apply_safety_zone_rule_field = 
        (1 << SKILL_ABILITY_ENEMY) | 
        (1 << SKILL_ABILITY_TARGETAREA_ENEMY) | 
        (1 << SKILL_ABILITY_MYAREA_ENEMY) | 
        (1 << SKILL_ABILITY_PLAYER_ENEMY);
    if (apply_safety_zone_rule_field & range_type_bitset)
    {
        WORD skip_check_tile_attr_for_target = 0;
        if (target->IsEqualObjectKind(MAPNPC_OBJECT)) 
        {
            skip_check_tile_attr_for_target = PTA_NO_WALK;
        }
        if (nsAI::PlanEventObject::AcceptableTarget(
            target, PTA_SAFETY_ZONE, skip_check_tile_attr_for_target) == false)
        {
            return false;
        }
        if ((attacker != target) &&
            (nsAI::PlanEventObject::AcceptableTarget(attacker, PTA_SAFETY_ZONE, 0) == false))
        {
            return false;
        }
    }

    const StatusManager* const target_status_manager = target->GetStatusManager();
    
    bool is_riding_or_spreading = false;
    {
        const StatusField& target_status_field = target_status_manager->Status;
        if (target_status_field.IsRidingRider() || target_status_field.IsSpreadWings())
        {
            is_riding_or_spreading = true;
        }
    }
    if (base_ability_info_->m_wAbilityID == eABILITY_KNOCKBACK ||
        base_ability_info_->m_wAbilityID == eABILITY_KNOCKBACK2)
    {
        if (is_riding_or_spreading) 
        {
            return false;
        }
    }

#ifdef _NA_007128_20140326_MONDSCHIEN_DOMINATION
    //각인상태에서는 아래 어빌리티가 적용되지않는다(각인 실패 방지)
    if ( target_status_manager->FindStatus(eCHAR_STATE_STAMP))
    {
        switch(base_ability_info_->m_wAbilityID)
        {
        case eABILITY_STUN_STATUS:
        case eABILITY_KNOCKBACK:
        case eABILITY_KNOCKBACK2:
        case eABILITY_PULLING:
            return false;
        default:
            break;
        }
    }
#endif //_NA_007128_20140326_MONDSCHIEN_DOMINATION

    // 풀링의 면역조건 검사는 별도로 진행한다.
    // 풀링은 State가 아니기 때문
    if (base_ability_info_->m_wAbilityID == eABILITY_PULLING)
    {
        //풀링에 면역되는 상태들
        if (target_status_manager->FindStatus(eCHAR_STATE_PROTECTION) != NULL)
        {
            return false;
        }
    }

    const eCHAR_STATE_TYPE state_type = GetStateID();
    const BASE_STATEINFO* const state_info = StateInfoParser::Instance()->GetStateInfo(state_type);

    bool check_success_rate = false;
    //적용하려는 State가 있을 경우
    if (state_info != NULL)
    {
        if (!state_info->m_bRidingApply)
        {
            if (is_riding_or_spreading) 
            {
                return false;
            }
        }
        if (state_info->m_byType == eSTATE_TYPE_ABNORMAL || 
            state_info->m_byType == eSTATE_TYPE_WEAKENING)
        {
            // 디버프계열 State 면역 관련
#ifdef _NA_008486_20150914_TOTAL_BALANCE
            if (target_status_manager->FindStatus(eCHAR_STATE_PROTECTION) ||
                target_status_manager->FindStatus(eCHAR_STATE_STAMP) ||
                target_status_manager->isImmunityDamageState(attacker)) 
            {
                return false;
            }
#else
            if (target_status_manager->FindStatus(eCHAR_STATE_PROTECTION) ||
                target_status_manager->FindStatus(eCHAR_STATE_STAMP) ||
                target_status_manager->isImmunityDamageState()) 
            {
                return false;
            }
#endif //_NA_008486_20150914_TOTAL_BALANCE
            check_success_rate = true;
        }
        else if (state_info->m_byType == eSTATE_TYPE_SPECIALITY) 
        {
            if (target_status_manager->FindStatus(eCHAR_STATE_IMMUNITY_DAMAGE)) 
            {
#ifdef _JP_006544_DRAGON_NIGHT_DRAGONTANS_BUG
#else
                return false;
#endif // _JP_006544_DRAGON_NIGHT_DRAGONTANS_BUG
            }
            check_success_rate = true;
        }
        if (IsStatusHit(
            attacker, target, base_ability_info_->m_wSuccessRate, state_type, GetSKill()) == false)
        {
            return false;
        }
    }

    if (check_success_rate == false)
    {
        if (!DrawLots((BYTE)(base_ability_info_->m_wSuccessRate / 10.0f))) 
        {
            return false;
        }
    }

    if (CheckAbilityRange(
        attacker, target, main_target_key, base_ability_info_->m_byRangeType) == false) 
    {
        return false;
    }

    return true;
}


bool Ability::CheckAbilityRange(
    Character* const attacker, 
    Character* const target, 
    const DWORD main_target_key, 
    const BYTE ability_range_type) const
{
    if (ability_range_type == SKILL_ABILITY_CORPSE_RESURRECTION ||
        ability_range_type == SKILL_ABILITY_CORPSE_FRIEND ||
        ability_range_type == SKILL_ABILITY_MYAREA_CORPSE_ENEMY ||
        ability_range_type == SKILL_ABILITY_CORPSE_ENEMY)
    {
        if (!target->IsDead()) 
        {
            return false;
        }
    }
    else if (target->IsDead())
    {
        return false;
    }
    
    if (ability_range_type == SKILL_ABILITY_FIELD) 
    {
        return false;
    }
    
    switch (ability_range_type)
    {
    case SKILL_ABILITY_FRIEND:
    case SKILL_ABILITY_TARGETAREA_FRIEND:   
    case SKILL_ABILITY_MYAREA_FRIEND:
    case SKILL_ABILITY_CORPSE_FRIEND:       
        {
            if (attacker->IsFriend(target) != USER_RELATION_FRIEND) 
            {       
                return false;
            }
        }
        break;
    case SKILL_ABILITY_ENEMY:
    case SKILL_ABILITY_TARGETAREA_ENEMY:
    case SKILL_ABILITY_MYAREA_ENEMY:    
    case SKILL_ABILITY_MYAREA_CORPSE_ENEMY: 
    case SKILL_ABILITY_CORPSE_ENEMY:    
        {
            if (attacker->IsFriend(target) != USER_RELATION_ENEMY) 
            {
                return false;
            }
        }
        break;
    case SKILL_ABILITY_CORPSE_RESURRECTION: 
        {
            if (!attacker->CanResurrection(target)) 
            {
                return false;
            }
        }
        break;
    case SKILL_ABILITY_ME:
    case SKILL_ABILITY_FIELD:
    case SKILL_ABILITY_SUMMON:
    case SKILL_ABILITY_SUMMONED_MONSTER:
        break;
    case SKILL_ABILITY_PLAYER_ENEMY:
        if (target->IsEqualObjectKind(PLAYER_OBJECT) == 0) 
        {
            return false;
        }
        break;
    default:
        break;
    }

    if (ability_range_type == SKILL_ABILITY_FRIEND || ability_range_type == SKILL_ABILITY_ENEMY)
    {
        if (main_target_key != target->GetObjectKey()) 
        {
            return false;
        }
    }
    else if (ability_range_type == SKILL_ABILITY_ME)
    {
        if (attacker->GetObjectKey() != target->GetObjectKey()) 
        {
            return false;
        }
    }

    return true;
}