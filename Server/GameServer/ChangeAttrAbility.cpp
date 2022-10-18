#include "StdAfx.h"
#include ".\changeattrability.h"
#include "StatusManager.h"
#include "AbilityStatus.h"


bool ChangeAttrAbility::CanExecute(
    Character* attacker, Character* target, DWORD main_target_key, WzVector main_target_pos)
{
    if (Ability::CanExecute(attacker, target, main_target_key, main_target_pos) == false)
    {
        return false;
    }
    
    const BASE_ABILITYINFO* const ability_info = GetBaseAbilityInfo();
    const int target_state = ability_info->m_iOption2;
    if (ability_info->m_byRangeType == SKILL_ABILITY_ME)
    {
        if (target_state != 0)
        {
            return false;
        }
    }
    else if (ability_info->m_byRangeType != SKILL_ABILITY_ENEMY)
    {
        return false;
    }

    const int attacker_state = ability_info->m_iOption1;
    if (attacker_state != 0)
    {
        if (attacker->GetStatusManager()->FindStatus((eCHAR_STATE_TYPE)attacker_state) == NULL)
        {
            return false;
        }            
    }
    if (target_state != 0)
    {
        if (target->GetStatusManager()->FindStatus((eCHAR_STATE_TYPE)target_state) == NULL)
        {
            return false;
        }
    }

    return true;
}

bool ChangeAttrAbility::Execute(Character* target, BYTE* msg, WORD& msg_size)
{
    if (IsValidStateID() == false)
    {
        return false;
    }

    Character* const attacker = GetAttacker();
    if (attacker == NULL)
    {
        return false;
    }
        
    const AbilityStatus* const ability_status = 
        attacker->GetStatusManager()->AllocAbilityStatus(attacker, this);
    if (ability_status == NULL)
    {
        return false;
    }
        
    // 여기에 해당 상태를 클라한테 보내지 않는 경우를 넣는다.
    // 73은 추가 데미지 이다.

    // 313 [eCHAR_STATE_CHANGE_ATTR] 는 AbilityStatus를 통해 
    // 자기 자신의 속성 값을 변경 시킨다.

    // 추가 데미지의 경우 플레이어 
    // 속성값(Parameter1 의 값[73][eATTR_ADD_ARMOR_UNARMOR_DAMAGE]) 을
    // Parameter2 만큼 증가시킨다.

    if (GetBaseAbilityInfo()->m_iParam[0] == 73)
    {
        // 0이면 클라한테 해당 어빌리티 메세지를 보내지 않는다.
        msg_size = 0;
    }
    else
    {
        if (Ability::Execute(target, msg, msg_size) == false)
        {
            return false;
        }
        
        SKILL_RESULT_CODE* const result_msg = (SKILL_RESULT_CODE*)msg;
        if (result_msg != NULL)
        {
            result_msg->m_wStatusCode = ability_status->GetStateID();
            result_msg->m_iStatusTime = ability_status->GetApplicationTime();
        }
    }

    return true;
}

bool ChangeAttrAbility::IsValidStateID() const
{
    switch (GetStateID())
    {
    case eCHAR_STATE_CHANGE_ATTR:
        return true;
    default:
        return false;
    }
}