#include "StdAfx.h"
#include ".\buffcancelability.h"

#ifdef		__NA_S00015_20080828_NEW_HOPE_SHADOW

BuffCancelAbility::BuffCancelAbility(void)
{
}

BuffCancelAbility::~BuffCancelAbility(void)
{
}

BOOL	BuffCancelAbility::Execute( Character *pTarget, BYTE *pMsg, WORD &wMsgSize )
{
	//현재 타겟의 강화 Status중 하나를 없앤다.
	BaseStatus* pStatus = pTarget->GetStatusManager()->FindStatusByStateType( eSTATE_TYPE_STRENGTHENING );
	if( !pStatus )
		return FALSE;

	//만일 존재한다면 해당 상태를 해제한다.
	pStatus->StopStatus();

	return TRUE;
}

#endif
