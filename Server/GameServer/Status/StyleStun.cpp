#include "stdafx.h"
#include "StyleStun.h"
#include <PacketControl/WarMessages_Sync.h>

void StyleStun::Start()
{
    EtcStatus::Start();

    Character* const owner = GetOwner();
    if (owner != NULL)
    {
        // NPC에게 스턴 메세지를 보냄
        AI_MSG_STUN stun_msg;
        stun_msg.dwStunTick = GetApplicationTime();
        owner->SendAIMessage(&stun_msg, sizeof(stun_msg));

        // 이동중이라면 멈춘다.
        owner->StopMoving();

        // 공격 딜레이를 초기화한다.
        owner->SetActionDelay(0);
    }
}

void StyleStun::End()
{
    EtcStatus::End();

    Character* const owner = GetOwner();
    if (owner != NULL)
    {
        //주의 : eCHAR_STATE_STYLE_STUN 클라, 서버간 상태 코드가 다르다.
        MSGSUB_SYNC_STATUS_REMOVE_BRD remove_brd;
        remove_brd.m_dwTargetKey = owner->GetObjectKey();
        remove_brd.m_wStatusCode = eCHAR_STATE_STUN;   
        owner->SendPacketAroundForSync(&remove_brd, TRUE);
    }
}