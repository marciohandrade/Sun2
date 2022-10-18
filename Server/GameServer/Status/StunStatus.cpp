#include "stdafx.h"
#include "Ability.h"
#include "StunStatus.h"


VOID StunStatus::Start()
{
    Character* const owner = GetOwner();
    if (owner == NULL)
    {
        return;
    }

	// ���� �ٿ� ���·� ó���Ѵ�.
	AI_MSG_KNOCKDOWN downMsg;
	downMsg.dwKnockDownTick = GetApplicationTime();
	owner->SendAIMessage( &downMsg, sizeof(downMsg) );

	// �̵����̶�� �����.
	owner->StopMoving();

	// ���� �����̸� �ʱ�ȭ�Ѵ�.
	owner->SetActionDelay( 0 );
}

VOID StunStatus::End()
{
    Character* const owner = GetOwner();
    if (owner != NULL)
    {
        // ���� �ٿ� ���¸� �����Ѵ�. dwKnockDownTick�� 0���� ������ ���¿��� �����ȴ�.
        AI_MSG_KNOCKDOWN downMsg;
        downMsg.dwKnockDownTick = 0;
        owner->SendAIMessage( &downMsg, sizeof(downMsg) );
    }
	
	AbilityStatus::End();
}



















