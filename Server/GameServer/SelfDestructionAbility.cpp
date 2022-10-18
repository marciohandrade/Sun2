#include "StdAfx.h"
#include ".\selfdestructionability.h"
#include "Monster.h"
#include "MovingPlanner.h"
#include "SkillSystem/CSSyncTuner.h"

bool SelfDestructionAbility::CanExecute(
    Character* attacker, Character* target, DWORD main_target_key, WzVector main_target_pos)
{
    if (Ability::CanExecute(attacker, target, main_target_key, main_target_pos) == false)
    {
        return false;
    }
        
    const BASE_ABILITYINFO* const ability_info = GetBaseAbilityInfo();
    if (ability_info == NULL)
    {
        return false;
    }
    const int kill_ratio = ability_info->m_iParam[0] / 10;
    if (kill_ratio == 0)
    {
        return false;
    }
    const int waste_hp_ratio = ability_info->m_iParam[1];
    if (waste_hp_ratio == 0)
    {
        return false;
    }
#if !defined(_NA_000000_20130417_SELFDISTRUCTIONABILITY_MODIFY)
    const DWORD waste_hp = 
        (DWORD)((float)attacker->GetMaxHP() * ((float)waste_hp_ratio / 1000.0f));
    if (attacker->GetHP() <= waste_hp)
    {
        return false;
    }
    
    if (attacker->IsEqualObjectKind(PLAYER_OBJECT) == false)
    {
        return false;
    }
    const bool is_valid_target_kind = 
        (target->IsEqualObjectKind(PLAYER_OBJECT) || target->IsEqualObjectKind(NPC_OBJECT));
    if (is_valid_target_kind == false)
    {
        return false;
    }
    if (target->IsEqualObjectKind(NPC_OBJECT))
    {
        const NPC* const npc = (NPC*)target;
        const int npc_grade = npc->GetBaseInfo()->m_byGrade;
        if (npc_grade == eNPC_BOSS || 
            npc_grade == eNPC_MIDDLEBOSS ||  
            npc_grade == eNPC_LUCKY_MONSTER || 
            npc_grade == eNPC_CRYSTAL_WARP ||
            npc_grade == eNPC_DOMINATION_MAPOBJECT_NPC)
        {
            return false;
        }
    }
    enum { kMaxLevelDiff = 10 };    
    const int level_diff = target->GetLevel() - attacker->GetLevel();
    if (level_diff >= kMaxLevelDiff)
    {
        return false;
    }
#endif

    return true;
}

bool SelfDestructionAbility::Execute(Character* target, BYTE* msg, WORD& msg_size)
{
    if (Ability::Execute(target, msg, msg_size) == false)
    {
        return false;
    }
    
    Character* const attacker = GetAttacker();
    if (attacker == NULL)
    {
        return false;
    }
    GameField* const game_field = attacker->GetField();
    if (game_field == NULL || game_field != target->GetField()) 
    {
        return false;
    }
    if (attacker->IsEqualObjectKind(PLAYER_OBJECT) == false)
    {
        return false;
    }
    const bool is_valid_target_kind = 
        (target->IsEqualObjectKind(PLAYER_OBJECT) || target->IsEqualObjectKind(NPC_OBJECT));
    if (is_valid_target_kind == false)
    {
        return false;
    }

    const BASE_ABILITYINFO* const ability_info = GetBaseAbilityInfo();
    const int kill_ratio = ability_info->m_iParam[0] / 10;
    if (kill_ratio == 0)
    {
        return false;
    }
    const int waste_hp_ratio = ability_info->m_iParam[1];
    if (waste_hp_ratio == 0)
    {
        return false;
    }
    const DWORD waste_hp = 
        (DWORD)((float)attacker->GetMaxHP() * ((float)waste_hp_ratio / 1000.0f));
    if (attacker->GetHP() <= waste_hp)
    {
        return false;
    }
    const DWORD remain_hp = attacker->GetHP() - waste_hp;
    if (remain_hp < 1)
    {
        return false;
    }

    // CHANGES: f110823.3L, prevent forced moved state problem without tile attributes check
    // CHANGES: f111011.4L, fixed calculation to apply C/S strict moving synchronization.
    //1. 먼저 앞으로 위치 이동하고.
    const WzVector& attacker_pos = *attacker->GetPosPtr();
    const WzVector& target_pos = *target->GetPosPtr();
    WzVector distance = target_pos;
    ;        distance -= attacker_pos;
    WzVector direction; // nomalized vector
    VectorNormalize(&direction, &distance);
    WzVector offset = direction;
    ;        offset *= 2.0f;
    WzVector destination = attacker_pos;
    ;        destination += (distance - offset);

    const WORD kFilteredAttr = (PTA_NO_WALK);
    if (game_field->GetMovingPlanner()->PathPlan_FastMoved(
        attacker, destination, kFilteredAttr, &destination) != false)
    {
        if (attacker->IsEqualObjectKind(PLAYER_OBJECT)) 
        {
            static_cast<Player*>(attacker)->GetSyncTuner()->OnFastMoved(destination);
        }
    }
    attacker->SetPos(&destination);
    
    const bool is_success_kill = (random(0, 100) <= kill_ratio);
    if (is_success_kill)
    {
        if (target->IsEqualObjectKind(PLAYER_OBJECT))
        {
#ifdef _NA_006728_20130520_SKILL_BALANCE_MODIFY
            DamageArgs damage_args(attacker, eATTACK_TYPE_MELEE, GetSkillCode());
#else
            DamageArgs damage_args(attacker, eATTACK_TYPE_MELEE);
#endif // _NA_006728_20130520_SKILL_BALANCE_MODIFY
            {
                damage_args.damage_ = target->GetMaxHP();
            }
            target->SetHP(1);
            target->Damaged(&damage_args);

            MSGSUB_SKILL_PERIODIC_DAMAGE_BRD msg_brd;
            {
                msg_brd.Clear();
                msg_brd.m_dwAttackerKey = attacker->GetObjectKey();
                msg_brd.m_SkillCode = GetSkillCode();
                msg_brd.m_byNumberOfTargets = 1;
                DAMAGE_INFO& damage_info = msg_brd.m_DamageInfo[0];
                {
                    damage_info.m_dwTargetKey = target->GetObjectKey();
                    damage_info.m_wDamage = damage_args.damage_;
                    damage_info.m_dwTargetHP = target->GetHP();
                }
            }
            target->SendPacketAroundForSync(&msg_brd);
        }
        else
        {
            game_field->KillNPC(static_cast<Player*>(attacker), static_cast<NPC*>(target), true);
        }
    }

    attacker->SetHP(remain_hp);

    SKILL_RESULT_SELF_DESTRUCTION* const damage_msg = (SKILL_RESULT_SELF_DESTRUCTION*)msg;
    damage_msg->m_dwHP = attacker->GetHP();
    damage_msg->m_dwTargetHP = target->GetHP();
    msg_size = damage_msg->GetSize();

    return true;
}