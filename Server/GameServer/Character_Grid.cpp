#include "stdafx.h"
#include "Character.h"
#include "StatusManager.h"
#include "SummonManager.h"

// -----------------------------------------------------------------------------------------------------------
// for GameField/Sector grid

VOID Character::OnEnterField( GameField & IN rField, const WzVector & IN rwzVec, int iAngle )
{
	GetStatusManager()->SetCondition( eCHAR_CONDITION_STANDUP );

	Object::OnEnterField( rField, rwzVec, iAngle );
}

VOID Character::OnLeaveField()
{
	ASSERT( GetObjectKey() != 0 );

	// �ڽ��� ������ ���������� NPC�鿡�� pObject�� �ʵ带 �������� �˷��ش�.
	AI_MSG_LEAVE_FIELD AILeaveMsg;
	AILeaveMsg.dwObjectKey = GetObjectKey();
	SendToObservers( &AILeaveMsg, sizeof(AI_MSG_LEAVE_FIELD) );

	// �ʵ带 ������ ��ȯ������ ���ش�.
	SummonManager::Instance()->LeaveSummonNPCs( this );

	Object::OnLeaveField();
}

VOID Character::GetEtcStateTotalInfo(ETC_STATE_TOTAL_INFO* OUT rEtcStateTotalInfo)
{
    m_pStatusManager->SerialzieEtcStatusInfo(*rEtcStateTotalInfo);
}
