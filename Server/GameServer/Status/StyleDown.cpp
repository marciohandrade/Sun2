#include "stdafx.h"
#include "StyleDown.h"


VOID StyleDown::Start()
{
    EtcStatus::Start();

    Character* const owner = GetOwner();
    if (owner != NULL)
    {
        // ���� �ٿ� ���·� ó���Ѵ�.
        AI_MSG_KNOCKDOWN down_msg;
        down_msg.dwKnockDownTick = GetApplicationTime();
        owner->SendAIMessage(&down_msg, sizeof(down_msg));

        // �̵����̶�� �����.
        owner->StopMoving();

        // ���� �����̸� �ʱ�ȭ�Ѵ�.
        owner->SetActionDelay(0);
    }
}

VOID StyleDown::End()
{
    Character* const owner = GetOwner();
    if (owner != NULL)
    {
        // ���� �ٿ� ���¸� �����Ѵ�. dwKnockDownTick�� 0���� ������ ���¿��� �����ȴ�.
        AI_MSG_KNOCKDOWN down_msg;
        down_msg.dwKnockDownTick = 0;
        owner->SendAIMessage(&down_msg, sizeof(down_msg));
    }
    
    EtcStatus::End();
}