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

    // ��׷� ����Ʈ ������ ���
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
        // old-comments: Ÿ�ٸ��� ��ü Ÿ�ټ��� ���ؾ� �ϹǷ� ����� ũ��.
        // ��ų���� �����Ƽ �����ϴ� ������ ũ�� ���� �ʱ����� �ϴ� �̷��� ����~
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
    // ��׷� ����(Ÿ�ٺ���)�� ���
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





