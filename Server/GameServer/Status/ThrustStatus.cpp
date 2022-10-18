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

	// 공격 딜레이를 초기화한다.
	owner->SetActionDelay( 0 );

}

VOID ThrustStatus::Execute()
{
	AbilityStatus::Execute();
}



















