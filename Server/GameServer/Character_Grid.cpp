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

	// 자신을 포함한 인접섹터의 NPC들에게 pObject가 필드를 떠났음을 알려준다.
	AI_MSG_LEAVE_FIELD AILeaveMsg;
	AILeaveMsg.dwObjectKey = GetObjectKey();
	SendToObservers( &AILeaveMsg, sizeof(AI_MSG_LEAVE_FIELD) );

	// 필드를 떠날때 소환정보를 때준다.
	SummonManager::Instance()->LeaveSummonNPCs( this );

	Object::OnLeaveField();
}

VOID Character::GetEtcStateTotalInfo(ETC_STATE_TOTAL_INFO* OUT rEtcStateTotalInfo)
{
    m_pStatusManager->SerialzieEtcStatusInfo(*rEtcStateTotalInfo);
}
