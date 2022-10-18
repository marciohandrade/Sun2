#include "StdAfx.h"
#include ".\changeattrstatus.h"
#include "SkillAttrCalculator.h"

ChangeAttrStatus::ChangeAttrStatus()
{
}

ChangeAttrStatus::~ChangeAttrStatus()
{
}

void ChangeAttrStatus::Init(Character* owner, Character* attacker, Ability* ability)
{
    AbilityStatus::Init(owner, attacker, ability);
    SetAbilityValueType(1);
}

void ChangeAttrStatus::End()
{
    BaseStatus::End();

    // 상태적용이 끝났음을 클라이언트에게 알려준다.
    // 만일 현재 속성이 유지시간이 0이라면, 패킷을 보내지 않는다. 
    if (GetStateID() != eCHAR_STATE_INVALID && GetAbilityInfo()->m_iParam[2] != 0)
    {
        SendStatusDelBRD();
    }
    
    // Buff 계열의 어빌리티들을 처리한다.
    const eATTR_TYPE attr_id = GetAttrID();
    if (attr_id != eATTR_TYPE_INVALID && IsApply())
    {
        Character* const owner = GetOwner();
        if (owner != NULL)
        {
            SkillAttrCalculator calc(owner->GetAttr());
            calc.DelAttr(attr_id, GetAbilityValueType(), GetSumValue());
        }
    }
}