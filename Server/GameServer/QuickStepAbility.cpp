#include "stdafx.h"
#include "QuickStepAbility.h"
#include "SkillInfoParser.h"
#include "Skill.h"
#include "MovingPlanner.h"
#include "SkillSystem/CSSyncTuner.h"

void QuickStepAbility::InitDetailed(
    Character* attacker,
    SLOTCODE skill_code, 
    SkillInfo* skill_info,
    BYTE skill_stat_type,
    const BASE_ABILITYINFO* base_ability_info)
{
    Ability::InitDetailed(attacker, skill_code, skill_info, skill_stat_type, base_ability_info);

    const Skill* const skill = GetSKill();
    if (skill == NULL) 
    {
        executable_condition_ = false;
        ZeroMemory(&available_range_box_, sizeof(available_range_box_));
        return;
    }
    
    const WzVector* const center_pos = attacker->GetPosPtr();

    const eABILITY_RANGE_TYPE range_type = 
        static_cast<eABILITY_RANGE_TYPE>(base_ability_info->m_byRangeType);
    if (range_type == SKILL_ABILITY_ME) 
    {
        const SkillScriptInfo* const skill_script_info = skill->GetSkillBaseInfo();
        float range = static_cast<float>(skill_script_info->m_wSkillRange) / 10.0f;
        range += 5.0f; // fault-tolerant magic value

        const WzVector range_offset = { range, range, 0 };
        nsGrid::ViewBoxCubic fault_tolerant_range;
        fault_tolerant_range.SetInit2D(center_pos, &range_offset);

        executable_condition_ = 
            fault_tolerant_range.IsValidCoordinateXY(&skill_info->main_target_pos_);
    }
    else
    {
        executable_condition_ = true;
    }

    // 실행시점에 공격자의 위치 허용 범위로 재설정
    const WzVector center_fault_tolerant_range_offset = { 5.0f, 5.0f, 0.0f };
    available_range_box_.SetInit2D(center_pos, &center_fault_tolerant_range_offset);
}

bool QuickStepAbility::Execute(Character* target, BYTE* msg, WORD& msg_size)
{
    if (Ability::Execute(target, msg, msg_size) == false)
    {
        return false;
    }    
    
    GameField* const game_field = target->GetField();
    if (game_field == NULL)
    {
        return false;
    }
        
    if (executable_condition_ == false)
    {
        return false;
    }

    Character* const attacker = GetAttacker();
    if (attacker == NULL)
    {
        return false;
    }     
    
    const eABILITY_RANGE_TYPE range_type = 
        static_cast<eABILITY_RANGE_TYPE>(GetBaseAbilityInfo()->m_byRangeType);
    WzVector target_pos; 
    WzVector teleport_pos;

    const bool is_player = target->IsEqualObjectKind(PLAYER_OBJECT);
    const bool is_enemy = (attacker->IsFriend(target) == USER_RELATION_ENEMY);
    if (target != attacker && is_player && is_enemy) 
    {
        game_field->GetRandomPos(teleport_pos);
        target->GetPos(&target_pos);

        if (FastMove(target, teleport_pos) == false)
        {
            return false;
        }
    }
    else if (range_type == SKILL_ABILITY_ME && target == attacker) 
    {
        target->GetPos(&target_pos);

        if (available_range_box_.IsValidCoordinateXY(&target_pos) == false)
        {
            return false;
        }
            
        const SkillInfo* const skill_info = GetSKill()->GetSkillInfo();
        teleport_pos = skill_info->main_target_pos_;

        if (FastMove(target, teleport_pos) == false)
        {
            return false;
        }            
    }
    else 
    {
        ASSERT(!"Unexpected teleport condition");
        return false;
    }

    SKILL_RESULT_POSITION* const position_msg = reinterpret_cast<SKILL_RESULT_POSITION*>(msg);
    position_msg->m_wvCurPos = target_pos;
    position_msg->m_wvDestPos = teleport_pos;
    msg_size = position_msg->GetSize();

    return true;
}

bool QuickStepAbility::FastMove(Character* target, WzVector destination)
{
    const WORD kFilteredAttr = (PTA_NO_WALK);
    GameField* const game_field = target->GetField();
    if (game_field->GetMovingPlanner()->PathPlan_FastMoved(
        target, destination, kFilteredAttr, &destination) != false)
    {
        if (target->IsEqualObjectKind(PLAYER_OBJECT)) 
        {
            static_cast<Player*>(target)->GetSyncTuner()->OnFastMoved(destination);
        }
    }
    target->SetPos(&destination);

    return true;
}