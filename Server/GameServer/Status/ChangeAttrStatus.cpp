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

    // ���������� �������� Ŭ���̾�Ʈ���� �˷��ش�.
    // ���� ���� �Ӽ��� �����ð��� 0�̶��, ��Ŷ�� ������ �ʴ´�. 
    if (GetStateID() != eCHAR_STATE_INVALID && GetAbilityInfo()->m_iParam[2] != 0)
    {
        SendStatusDelBRD();
    }
    
    // Buff �迭�� �����Ƽ���� ó���Ѵ�.
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