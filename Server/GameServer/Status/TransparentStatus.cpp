#include "StdAfx.h"
#include ".\transparentstatus.h"
#include "PacketStruct_CG.h"
#include "StatusManager.h"

TransparentStatus::TransparentStatus(void)
{
}

TransparentStatus::~TransparentStatus(void)
{
}

VOID TransparentStatus::Start()
{
    Character* const owner = GetOwner();
    if (owner == NULL)
    {
        return;
    }

    // 이 안에서 속도 감속한다.
	AbilityStatus::Start();
    
	//내 자신과 근처 유저들에게 내가 투명 상태라는것을 알린다.
	owner->GetStatusManager()->Status.On_CharTransOn( true );

    ;{
        MSGSUB_SKILL_TRANSPARENT_BRD msg_brd;
        msg_brd.Clear();
        msg_brd.m_dwObjectKey = owner->GetObjectKey();
        owner->SendPacketAroundForSync(&msg_brd);
    };
}

VOID TransparentStatus::End()
{
    Character* const owner = GetOwner();
    if (owner != NULL)
    {
        // 이 안에서 속도 원래 속도로 복귀한다.
        owner->GetStatusManager()->Status.On_CharTransOn( false );
    }

	AbilityStatus::End();
}

