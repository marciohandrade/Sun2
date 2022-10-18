#include "stdafx.h"
#include "StyleDown.h"


VOID StyleDown::Start()
{
    EtcStatus::Start();

    Character* const owner = GetOwner();
    if (owner != NULL)
    {
        // 몬스터 다운 상태로 처리한다.
        AI_MSG_KNOCKDOWN down_msg;
        down_msg.dwKnockDownTick = GetApplicationTime();
        owner->SendAIMessage(&down_msg, sizeof(down_msg));

        // 이동중이라면 멈춘다.
        owner->StopMoving();

        // 공격 딜레이를 초기화한다.
        owner->SetActionDelay(0);
    }
}

VOID StyleDown::End()
{
    Character* const owner = GetOwner();
    if (owner != NULL)
    {
        // 몬스터 다운 상태를 해제한다. dwKnockDownTick을 0으로 보내면 상태에서 해제된다.
        AI_MSG_KNOCKDOWN down_msg;
        down_msg.dwKnockDownTick = 0;
        owner->SendAIMessage(&down_msg, sizeof(down_msg));
    }
    
    EtcStatus::End();
}