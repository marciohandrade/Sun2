#include "StdAfx.h"
#include "DragonTransAbility.h"
#include "AbilityStatus.h"
#include "StatusManager.h"


bool DragonTransAbility::Execute(Character* target, BYTE* msg, WORD& msg_size)
{
    if (BaseStatusAbility::Execute(target, msg, msg_size) == false)
    {
        return false;
    }

    Character* const attacker = GetAttacker();
    if (attacker == NULL)
    {
        return false;
    }
        
    MSG_CG_STATUS_START_DRAGON_FORMATION_BRD brd_msg;
    brd_msg.m_dwObjKey = attacker->GetObjectKey();
    brd_msg.m_SkillCode = GetSkillCode();
    brd_msg.m_wStatusCode = GetStateID();

    SKILL_RESULT_CODE* const result_msg = (SKILL_RESULT_CODE*)msg;
    brd_msg.m_iStatusTime = result_msg->m_iStatusTime;

    attacker->SendPacketAround(&brd_msg, sizeof(brd_msg));

    return false;
}