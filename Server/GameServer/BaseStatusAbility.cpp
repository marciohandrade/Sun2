#include "StdAfx.h"
#include "BaseStatusAbility.h"
#include "AbilityStatus.h"
#include "StatusManager.h"


bool BaseStatusAbility::CanExecute( Character* attacker, Character* target, DWORD main_target_key, WzVector main_target_pos )
{
    if (Ability::CanExecute(attacker, target, main_target_key, main_target_pos) == false)
        return false;

#ifdef _NA_007620_20140915_STATE_RESISTS
    if (target->IsEqualObjectKind(NPC_OBJECT) == true)
    {
        NPC* npc = static_cast<NPC*>(target);
        BASE_NPCINFO* npc_info = npc->GetBaseInfo();
        STLX_MAP<WORD, RESISTANCE_CONDITION>* state_resistances = npc_info->state_resistances_;
        if (state_resistances != NULL)
        {
            WORD state_id = GetBaseAbilityInfo()->m_wStateID;
            STLX_MAP<WORD, RESISTANCE_CONDITION>::iterator it = \
                (*state_resistances).find(state_id);
            if (it != (*state_resistances).end())
            {
                RESISTANCE_CONDITION& condition = it->second;
                if (condition.m_byRatio == 100)
                    return false;
                if (random(0, 100) <= condition.m_byRatio)
                    return false;
            }
        }
    }
#endif //_NA_007620_20140915_STATE_RESISTS

    return true;
}

bool BaseStatusAbility::Execute(Character* target, BYTE* msg, WORD& msg_size)
{
    if (Ability::Execute(target, msg, msg_size) == false)
    {
        return false;
    }
    
    if (IsValidStateID() == false)
    {
        return false;
    }
    
    Character* const attacker = GetAttacker();
    if (attacker == NULL)
    {
        return false;
    }

#ifdef _NA_007667_20141001_WITCHBLADE_SKILL
    StatusManager* status_manager = target->GetStatusManager();
    eCHAR_STATE_TYPE state_id = GetStateID();
    BaseStatus* status = status_manager->FindStatus(state_id);

    int status_time = 0;
    WORD overlap_count =0;

    //중첩되는 버프
    if (status != NULL && status->IsOverlapStatus() == true) 
    {
        status->StatusOverlap();

        status_time = status->GetApplicationTime();
        overlap_count = status->GetStatusOverlapCount();
    }
    else 
    {
        AbilityStatus* const ability_status = 
            target->GetStatusManager()->AllocAbilityStatus(attacker, this);
        if (ability_status == NULL) {
            return false;
        }

        status_time = ability_status->GetApplicationTime();
        overlap_count = ability_status->GetStatusOverlapCount();
    }

    // 결과메시지
    SKILL_RESULT_CODE* const result_msg = (SKILL_RESULT_CODE*)msg;
    if (result_msg != NULL)
    {
        result_msg->m_wStatusCode = state_id;
        result_msg->m_iStatusTime = status_time;
        result_msg->overlap_status_count = overlap_count;              
    }
#else
    const AbilityStatus* const ability_status = 
        target->GetStatusManager()->AllocAbilityStatus(attacker, this);
    if (ability_status == NULL)
    {
        return false;
    }

    SKILL_RESULT_CODE* const result_msg = (SKILL_RESULT_CODE*)msg;
    if (result_msg != NULL)
    {
        result_msg->m_wStatusCode = ability_status->GetStateID();
        result_msg->m_iStatusTime = ability_status->GetApplicationTime();
    }
#endif //_NA_007667_20141001_WITCHBLADE_SKILL

    return true;
}