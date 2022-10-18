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

	// 몬스터 다운 상태로 처리한다.
	AI_MSG_KNOCKDOWN downMsg;
	downMsg.dwKnockDownTick = GetApplicationTime();
	owner->SendAIMessage( &downMsg, sizeof(downMsg) );

	// 이동중이라면 멈춘다.
	owner->StopMoving();

	// 공격 딜레이를 초기화한다.
	owner->SetActionDelay( 0 );
}

VOID StunStatus::End()
{
    Character* const owner = GetOwner();
    if (owner != NULL)
    {
        // 몬스터 다운 상태를 해제한다. dwKnockDownTick을 0으로 보내면 상태에서 해제된다.
        AI_MSG_KNOCKDOWN downMsg;
        downMsg.dwKnockDownTick = 0;
        owner->SendAIMessage( &downMsg, sizeof(downMsg) );
    }
	
	AbilityStatus::End();
}



















