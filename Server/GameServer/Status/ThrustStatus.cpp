#include "stdafx.h"
#include "Ability.h"
#include "ThrustStatus.h"


VOID ThrustStatus::Start()
{
    Character* const owner = GetOwner();
    if (owner == NULL)
    {
        return;
    }

	// ���� �����̸� �ʱ�ȭ�Ѵ�.
	owner->SetActionDelay( 0 );

}

VOID ThrustStatus::Execute()
{
	AbilityStatus::Execute();
}



















