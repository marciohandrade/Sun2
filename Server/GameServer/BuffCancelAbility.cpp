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
	//���� Ÿ���� ��ȭ Status�� �ϳ��� ���ش�.
	BaseStatus* pStatus = pTarget->GetStatusManager()->FindStatusByStateType( eSTATE_TYPE_STRENGTHENING );
	if( !pStatus )
		return FALSE;

	//���� �����Ѵٸ� �ش� ���¸� �����Ѵ�.
	pStatus->StopStatus();

	return TRUE;
}

#endif
