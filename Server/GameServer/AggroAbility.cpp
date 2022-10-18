#include "StdAfx.h"
#include "AggroAbility.h"
#include "Battle.h"

#include "Skill.h"


// NOTE: f110621.4L, exported this method to support the aggressive distirbution consistency
void AggroAbility::Init(Skill* skill, const BASE_ABILITYINFO* base_ability_info)
{
    Ability::Init(skill, base_ability_info);
    touched_counting_aggressive_dispersion_ = false;
    number_of_targets_for_aggressive_dispersion_ = 0;
}

bool AggroAbility::Execute(Character* target, BYTE* pMsg, WORD& wMsgSize)
{
    if (Ability::Execute(target, pMsg, wMsgSize) == false)
    {
        return false;
    }
    
    Character* const attacker = GetAttacker();
    if (attacker == NULL)
    {
        return false;
    }

    const BASE_ABILITYINFO* const ability_info = GetBaseAbilityInfo();

    // 어그로 포인트 증가인 경우
    if (GetAbilityID() == eABILITY_AGGROPOINT_INCREASE)
    {
#ifdef _NA_0_20100819_AGGRO_RENEWAL
        if (ability_info->m_iOption1 != AGGRO_TYPE_DISPERSION)
        {
            target->AddBattlePoint(attacker, BATTLE_POINT_ETC, ability_info->m_iParam[1]);
            return true;
        };
        assert(ability_info->m_iOption1 == AGGRO_TYPE_DISPERSION);
        // CHANGES: f110621.4L, fixed divide by 0 exception and
        // support the aggressive distirbution consistency
        // old-comments: 타겟마다 전체 타겟수를 구해야 하므로 비용이 크다.
        // 스킬에서 어빌리티 수행하는 구조를 크게 깨지 않기위해 일단 이렇게 가자~
        if (touched_counting_aggressive_dispersion_ == false)
        {
            touched_counting_aggressive_dispersion_ = true;
            Skill* skill = GetSKill();
            uint8_t number_of_targets = skill->FindTargetCount(SKILL_TARGET_ENEMY);
            if (number_of_targets == 0) {
                number_of_targets = 1;
            };
            number_of_targets_for_aggressive_dispersion_ = number_of_targets;
        };
        //
        target->AddBattlePoint(attacker, BATTLE_POINT_ETC,
            ability_info->m_iParam[1] / number_of_targets_for_aggressive_dispersion_);
        //
        return true;
        //------------------------------------------------------------------------------------------
#else //if !defined(_NA_0_20100819_AGGRO_RENEWAL)
        target->AddBattlePoint(attacker, BATTLE_POINT_ETC, ability_info->m_iParam[1]);
#endif
    }
    // NOTE: f110621.4L, currently, this condition does not processed.
    // 어그로 끌기(타겟변경)인 경우
    else if (GetAbilityID() == eABILITY_TARGET_CHANGE)
    {
        if (DrawLots(ability_info->m_iParam[1] / 10))
        {
            target->SetTargetChar(attacker);
            target->ChangeState(STATE_ID_TRACK);
        }
    }

    return true;
}





