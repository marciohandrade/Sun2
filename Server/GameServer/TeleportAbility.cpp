#include "stdafx.h"
#include "TeleportAbility.h"
#include "SkillInfoParser.h"
#include "Skill.h"


void TeleportAbility::InitDetailed(
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

        const WzVector range_offset = { range, range, 10 };
        nsGrid::ViewBoxCubic fault_tolerant_range;
        fault_tolerant_range.SetInit3D(center_pos, &range_offset);

        executable_condition_ = 
            fault_tolerant_range.IsValidCoordinateXYZ(&skill_info->main_target_pos_);
    }
    else
    {
        executable_condition_ = true;
    }

    // 실행시점에 공격자의 위치 허용 범위로 재설정
    const WzVector center_fault_tolerant_range_offset = { 5.0f, 5.0f, 10.0f };
    available_range_box_.SetInit3D(center_pos, &center_fault_tolerant_range_offset);
}

bool TeleportAbility::Execute(Character* target, BYTE* msg, WORD& msg_size)
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
        // 텔레포트를 타인이 걸었고, 타겟이 플레이어이며, 타겟과 스킬사용자가 적이면 스킬 '포지션 체인지' 사용된다
#ifdef _NA_006937_20131030_ABILITY_AND_STATE_CHANGE_CRITICAL
        //헬로이드 스킬 '포지션 체인지'
        WzVector attacker_pos;

        target->GetPos(&target_pos);
        attacker->GetPos(&attacker_pos);

        if (game_field->IsMovableCoordinates(&target_pos) == false ||
            game_field->IsMovableCoordinates(&attacker_pos) == false) {
                return false;
        }

        target->StopMoving();
        attacker->StopMoving();

        // target을 텔레포트시킨다
        if (game_field->TeleportObject(target, &attacker_pos) == false) { 
            //
        }
        //사용자를 텔레포트시킨다.
        if (game_field->TeleportObject(attacker, &target_pos) == false) {
            //
        }

#else
        // 스킬 '퍼지'의 사용로직, 이 스킬은 이미 사라진 스킬이다.
        game_field->GetRandomPos(teleport_pos);
        target->GetPos(&target_pos);

        if (game_field->TeleportObject(target, &teleport_pos) == false)
        {
            return false;
        }
#endif //_NA_006937_20131030_ABILITY_AND_STATE_CHANGE_CRITICAL

        SKILL_RESULT_POSITION* const position_msg = reinterpret_cast<SKILL_RESULT_POSITION*>(msg);
        position_msg->m_wvCurPos = attacker_pos;
        position_msg->m_wvDestPos = target_pos;
        msg_size = position_msg->GetSize();
    }
    else if (range_type == SKILL_ABILITY_ME && target == attacker) 
    {
        // 텔레포트를 스스로 자신에게 사용했을 경우
        target->GetPos(&target_pos);

        if (available_range_box_.IsValidCoordinateXYZ(&target_pos) == false)
        {
            return false;
        }
            
        const SkillInfo* const skill_info = GetSKill()->GetSkillInfo();
        teleport_pos = skill_info->main_target_pos_;

        if (game_field->TeleportObject(target, &teleport_pos) == false)
        {
            return false;
        }    

        SKILL_RESULT_POSITION* const position_msg = reinterpret_cast<SKILL_RESULT_POSITION*>(msg);
        position_msg->m_wvCurPos = target_pos;
        position_msg->m_wvDestPos = teleport_pos;
        msg_size = position_msg->GetSize();
    }
    else 
    {
        ASSERT(!"Unexpected teleport condition");
        return false;
    }
    return true;
}