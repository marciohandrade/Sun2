#include "stdafx.h"
#include "StyleStun.h"
#include <PacketControl/WarMessages_Sync.h>

void StyleStun::Start()
{
    EtcStatus::Start();

    Character* const owner = GetOwner();
    if (owner != NULL)
    {
        // NPC���� ���� �޼����� ����
        AI_MSG_STUN stun_msg;
        stun_msg.dwStunTick = GetApplicationTime();
        owner->SendAIMessage(&stun_msg, sizeof(stun_msg));

        // �̵����̶�� �����.
        owner->StopMoving();

        // ���� �����̸� �ʱ�ȭ�Ѵ�.
        owner->SetActionDelay(0);
    }
}

void StyleStun::End()
{
    EtcStatus::End();

    Character* const owner = GetOwner();
    if (owner != NULL)
    {
        //���� : eCHAR_STATE_STYLE_STUN Ŭ��, ������ ���� �ڵ尡 �ٸ���.
        MSGSUB_SYNC_STATUS_REMOVE_BRD remove_brd;
        remove_brd.m_dwTargetKey = owner->GetObjectKey();
        remove_brd.m_wStatusCode = eCHAR_STATE_STUN;   
        owner->SendPacketAroundForSync(&remove_brd, TRUE);
    }
}