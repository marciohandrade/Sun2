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
        
    // ���⿡ �ش� ���¸� Ŭ������ ������ �ʴ� ��츦 �ִ´�.
    // 73�� �߰� ������ �̴�.

    // 313 [eCHAR_STATE_CHANGE_ATTR] �� AbilityStatus�� ���� 
    // �ڱ� �ڽ��� �Ӽ� ���� ���� ��Ų��.

    // �߰� �������� ��� �÷��̾� 
    // �Ӽ���(Parameter1 �� ��[73][eATTR_ADD_ARMOR_UNARMOR_DAMAGE]) ��
    // Parameter2 ��ŭ ������Ų��.

    if (GetBaseAbilityInfo()->m_iParam[0] == 73)
    {
        // 0�̸� Ŭ������ �ش� �����Ƽ �޼����� ������ �ʴ´�.
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