#include "StdAfx.h"
#include "ExhaustAbility.h"

// EP2:unused, _NA001605_EP2_ATTRIBUTES_RENEWAL_
bool ExhaustAbility::Execute(Character* target, BYTE* pMsg, WORD& wMsgSize)
{
    Ability::Execute(target, pMsg, wMsgSize);

    SKILL_RESULT_EXHAUST *pExhaustMsg = (SKILL_RESULT_EXHAUST*)pMsg;

    const BASE_ABILITYINFO* const ability_info = GetBaseAbilityInfo();
    int iValueType = ability_info->m_iParam[0];
    int iValue = ability_info->m_iParam[1];

    // HP 혹은 MP를 지정 수치만큼만 남기고 모두 소진시킨다.
    if (GetAbilityID() == eABILITY_EXHAUST_HP)
    {
        if (iValueType == VALUE_TYPE_PERCENT_PER_MAX) {
            iValue = target->GetMaxHP() * iValue / 1000;
        }
        else if (iValueType == VALUE_TYPE_PERCENT_PER_CUR) {
            iValue = target->GetHP() * iValue / 1000;
        }

        if (iValue <= 0) {
            iValue = 1;
        }
        target->SetHP(iValue);
    }
    else if (GetAbilityID() == eABILITY_EXHAUST_MP)
    {
        if (iValueType == VALUE_TYPE_PERCENT_PER_MAX) {
            iValue = target->GetMaxMP() * iValue / 1000;
        }
        else if (iValueType == VALUE_TYPE_PERCENT_PER_CUR) {
            iValue = target->GetMP() * iValue / 1000;
        }
        target->SetMP(iValue);
    }

    // 패킷구조체를 채운다.
    if (pExhaustMsg)
    {
        pExhaustMsg->m_dwTargetHP = target->GetHP();
        pExhaustMsg->m_dwTargetMP = target->GetMP();
        wMsgSize = pExhaustMsg->GetSize();
    }

    return true;
}






