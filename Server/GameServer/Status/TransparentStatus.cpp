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

    // �� �ȿ��� �ӵ� �����Ѵ�.
	AbilityStatus::Start();
    
	//�� �ڽŰ� ��ó �����鿡�� ���� ���� ���¶�°��� �˸���.
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
        // �� �ȿ��� �ӵ� ���� �ӵ��� �����Ѵ�.
        owner->GetStatusManager()->Status.On_CharTransOn( false );
    }

	AbilityStatus::End();
}

