#include "stdafx.h"
#include ".\area.h"

#include "Battle.h"

CArea::CArea(void)
{
}

CArea::~CArea(void)
{
}

BOOL CTempAttackArea::IsInside(WzVector* pPos)
{
	//	��ä�� ��� ���� �˻�
	return IsPositionInSector( &m_VecSrcPostion, &m_VecSrcDir, pPos, m_fRadian );
}