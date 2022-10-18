#include "StdAfx.h"
#include ".\darkstunability.h"
#include "StatusManager.h"
#include "AbilityStatus.h"

#ifdef __NA_S00015_20080828_NEW_HOPE_SHADOW

DarkStunAbility::DarkStunAbility(void)
{
}

DarkStunAbility::~DarkStunAbility(void)
{
}

BOOL DarkStunAbility::IsValidStateID()
{
	switch( GetStateID() )
	{
	case eCHAR_STATE_STUN2:
		return TRUE;
	default:
		return FALSE;
	}
}

BOOL	DarkStunAbility::Execute( Character *pTarget, BYTE *pMsg, WORD &wMsgSize )
{
	Ability::Execute( pTarget, pMsg, wMsgSize );

	// 타당한 상태인지 체크
	if( !IsValidStateID() )
		return FALSE;

	//1. 먼저 해당 타겟 앞으로 이동하고
	WzVector vTargetPos, vCurPos;

	if( !( m_pAttacker->GetObjectType() == PLAYER_OBJECT ) )
		return FALSE;

	m_pAttacker->GetPos( &vCurPos ); 
	pTarget->GetPos( &vTargetPos );

	if( !m_pAttacker->GetField()->TeleportObject( m_pAttacker, &vTargetPos ) )
		return FALSE;

	// 2. 해당 타겟에게 데미지를 주고,
	SKILL_RESULT_DAMAGE	*pDamageMsg = (SKILL_RESULT_DAMAGE*)pMsg;

	// 어빌러티 파라미터 설정
	eATTACK_TYPE	eAttackType = (eATTACK_TYPE)m_pBaseAbilityInfo->m_iOption1;
	DAMAGETYPE		wResultDamage = pTarget->Damaged( m_pAttacker, eAttackType, m_pBaseAbilityInfo->m_iParam[0], FALSE );

	// 3. 해당 타겟에게 STUN을 건다.
	BOOL bStun = false;
	if( random( 0, 100 ) <= ( m_pBaseAbilityInfo->m_iParam[1] / 10 ) )
		bStun = true;

	AbilityStatus* pAbilityStatus = NULL;

	if( bStun )
	{
		pAbilityStatus = pTarget->GetStatusManager()->AllocAbility( m_pAttacker, this );
		if( !pAbilityStatus )	return FALSE;
	}

	// 패킷구조체를 채운다.
	if( pMsg )
	{
		SKILL_RESULT_DARK_STUN *pStunMsg = (SKILL_RESULT_DARK_STUN*)pMsg;
		if( bStun )
		{
			pStunMsg->m_wStatusCode = pAbilityStatus->GetStateID();
			pStunMsg->m_iStatusTime = pAbilityStatus->GetApplicationTime();
		}
		else
		{
			pStunMsg->m_wStatusCode = 0;
			pStunMsg->m_iStatusTime = 0;
		}

		pStunMsg->m_wvCurPos = vCurPos;
		pStunMsg->m_wvDestPos = vTargetPos;
		pStunMsg->m_wDamage = wResultDamage;
		pStunMsg->m_dwTargetHP = pTarget->GetHP();
		pStunMsg->m_wvTargetPos = vTargetPos;
		wMsgSize = pStunMsg->GetSize();
	}
	
	return TRUE;
}

#endif
