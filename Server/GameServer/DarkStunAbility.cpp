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

	// Ÿ���� �������� üũ
	if( !IsValidStateID() )
		return FALSE;

	//1. ���� �ش� Ÿ�� ������ �̵��ϰ�
	WzVector vTargetPos, vCurPos;

	if( !( m_pAttacker->GetObjectType() == PLAYER_OBJECT ) )
		return FALSE;

	m_pAttacker->GetPos( &vCurPos ); 
	pTarget->GetPos( &vTargetPos );

	if( !m_pAttacker->GetField()->TeleportObject( m_pAttacker, &vTargetPos ) )
		return FALSE;

	// 2. �ش� Ÿ�ٿ��� �������� �ְ�,
	SKILL_RESULT_DAMAGE	*pDamageMsg = (SKILL_RESULT_DAMAGE*)pMsg;

	// �����Ƽ �Ķ���� ����
	eATTACK_TYPE	eAttackType = (eATTACK_TYPE)m_pBaseAbilityInfo->m_iOption1;
	DAMAGETYPE		wResultDamage = pTarget->Damaged( m_pAttacker, eAttackType, m_pBaseAbilityInfo->m_iParam[0], FALSE );

	// 3. �ش� Ÿ�ٿ��� STUN�� �Ǵ�.
	BOOL bStun = false;
	if( random( 0, 100 ) <= ( m_pBaseAbilityInfo->m_iParam[1] / 10 ) )
		bStun = true;

	AbilityStatus* pAbilityStatus = NULL;

	if( bStun )
	{
		pAbilityStatus = pTarget->GetStatusManager()->AllocAbility( m_pAttacker, this );
		if( !pAbilityStatus )	return FALSE;
	}

	// ��Ŷ����ü�� ä���.
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
