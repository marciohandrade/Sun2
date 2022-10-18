#include "stdafx.h"
#include "Ability.h"
#include "ChaosStatus.h"


VOID ChaosStatus::Start()
{
    const Character* const attacker = GetAttacker();
    if (attacker == NULL)
    {
        return;
    }
    Character* const owner = GetOwner();
    if (owner == NULL)
    {
        return;
    }
    
    AI_MSG_RUNAWAY ai_msg;
	ai_msg.dwTargetKey = attacker->GetObjectKey();
	ai_msg.dwRunAwayTime = GetApplicationTime();
	owner->SendAIMessage(&ai_msg, sizeof(ai_msg));
}