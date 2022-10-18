#include "StdAfx.h"
#include ".\summondeadmonsterability.h"
#include "Skill.h"
#include "SummonManager.h"
#include "Summoned.h"
#include "SummonedNPCs.h"
#include "ObjectFactory.h"
#include "NPCStateManager.h"
#include "DeadState.h"
#include "MovingPlanner.h"


bool SummonDeadMonsterAbility::CanExecute(
    Character* attacker, Character* target, DWORD main_target_key, WzVector main_target_pos)
{
    if (Ability::CanExecute(attacker, target, main_target_key, main_target_pos) == false)
    {
        return false;
    }
    const GameField* const field = target->GetField();
    if (field == NULL) 
    {
        return false;
    }

    if (!target->IsDead() || target->GetDeadType() != eCHAR_DEAD_NORMAL)
    {
        return false;
    }
    if (target->IsEqualObjectKind(MONSTER_OBJECT) == false)
    {
        return false;
    }
        
    const NPC* const target_npc = (NPC*)target;
#ifdef _NA_007124_20140324_SHADOW_MODIFIED_SKILL_SOULCONTROL_SUMMON_GRADE
    if (target_npc->GetBaseInfo()->m_byGrade == 9 || 
        target_npc->GetBaseInfo()->m_byGrade == 7)
#else
    if (target_npc->GetBaseInfo()->m_byGrade == 5 || 
        target_npc->GetBaseInfo()->m_byGrade == 9 || 
        target_npc->GetBaseInfo()->m_byGrade == 7 || 
        target_npc->GetBaseInfo()->m_byGrade == 4)
#endif
    {
        return false;
    }
    
    const SummonedNPCs* const summoned_npcs = 
        SummonManager::Instance()->FindSummonNPCs(attacker->GetObjectKey());
    if (summoned_npcs != NULL)
    {
        if (summoned_npcs->GetNumberOfNPCs() >= GetBaseAbilityInfo()->m_iParam[0])
        {
            return false;
        }
    }

    return true;
}

bool SummonDeadMonsterAbility::Execute(Character* target, BYTE* msg, WORD& msg_size)
{
    if (Ability::Execute(target, msg, msg_size) == false)
    {
        return false;
    }
    
    Character* const summoner = GetAttacker();
    if (summoner == NULL)
    {
        return false;
    }
    GameField* const field = target->GetField();
    if (field == NULL)
    {
        return false;
    }
    const int cur_tile = target->GetPathExplorer()->GetTile();
    if (cur_tile < 0)   
    {
        return false;
    }

    NPC* const target_npc = (NPC*)target;    
    State* const dead_state = target_npc->GetStateManager()->GetCurState();
    if (dead_state == NULL)
    {
        return false;
    }
    if (dead_state->GetStateID() != STATE_ID_DEAD)
    {
        return false;
    }
    target_npc->SetDeadType(eCHAR_DEAD_SUMMON);
    static_cast<DeadState*>(dead_state)->SetNextTimeoutInterval(0);

    nsAI::MovingPlanner* const moving_planner = field->GetMovingPlanner();
    
    WzVector summoner_pos;
    summoner->GetPos(&summoner_pos);

    const SkillScriptInfo* const skill_info = GetSKill()->GetSkillBaseInfo();
    float radius = static_cast<float>(skill_info->m_wSkillRange) / 10.0f;
    radius += summoner->GetSkillRangeBonus();
    const WzVector accept_radius = { radius, radius, 5.0f };
        
    nsGrid::ViewBoxCubic accept_range;
    accept_range.SetInit3D(&summoner_pos, &accept_radius);
    
    WzVector monster_pos;
    for (int pos_trial = 0; pos_trial < 51; ++pos_trial)
    {
        monster_pos = moving_planner->GetRandomPosInTile(cur_tile);
        if (accept_range.IsValidCoordinateXYZ(&monster_pos)) 
        {
            break;
        }
        if (pos_trial >= 50)
        {
            return false;
        }
    }

    SummonedNPCs* summoned_npcs = SummonManager::Instance()->FindSummonNPCs(summoner->GetObjectKey());
    if (summoned_npcs == NULL)
    {
        summoned_npcs = SummonManager::Instance()->AllocSummonNPCs(summoner, true);
        if (summoned_npcs == NULL)
        {
            return false;
        }            
    }

    const MONSTERCODE monster_code = target_npc->GetBaseInfo()->m_MonsterCode;
    Summoned* const summoned = field->CreateSummonDeadMonster(
        monster_code, &monster_pos, summoner, summoned_npcs, GetSkillCode());
    if (summoned == NULL)
    {
        return false;
    }
        
    summoned->SetHP((double)summoned->GetMaxHP() * ((float)GetBaseAbilityInfo()->m_iParam[1] / 1000));
#if !defined(_NA_000000_20140224_SUMMONER_REFACTORING)
    summoner->SetSummonerKey(summoned_npcs->GetSummonerKey());
#endif //_NA_000000_20140224_SUMMONER_REFACTORING

    MSG_CG_SUMMON_CURRENT_HP_BRD brd_msg;
    brd_msg.m_dwSummonedObjKey = summoned->GetObjectKey();
    brd_msg.m_dwMaxHP = summoned->GetMaxHP();
    brd_msg.m_dwHP = summoned->GetHP();
    brd_msg.m_dwSpeedRatio = summoned->GetMoveSpeedRatio();
    summoner->SendPacketAround(&brd_msg, sizeof(MSG_CG_SUMMON_CURRENT_HP_BRD));

    return true;
}