#include "StdAfx.h"
#include "StunAbility.h"
#include "AbilityStatus.h"
#include "StatusManager.h"


bool StunAbility::Execute(Character* target, BYTE* pMsg, WORD& wMsgSize)
{
    if (!BaseStatusAbility::Execute(target, pMsg, wMsgSize))
        return false;

    // ��Ŷ����ü�� ä���.
    SKILL_RESULT_STUN *pStunMsg = (SKILL_RESULT_STUN*)pMsg;
    wMsgSize = pStunMsg->GetSize();
    target->GetPos(&pStunMsg->m_wvCurPos);

    target->CancelAllSkill();

    return true;
}

