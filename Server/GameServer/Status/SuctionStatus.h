#pragma once

#include "AbilityStatus.h"

class SuctionStatus : public AbilityStatus
{
public:
	SuctionStatus(void);
	~SuctionStatus(void);

public:
	virtual VOID		Init( Character *pOwner, Character *pAttacker, Ability *pAbility );
	virtual VOID		Execute();
			VOID		SetTargetObjectKey( DWORD dwObjectKey ){ m_dwTargetObjectKey = dwObjectKey;  }

private:
	DAMAGETYPE			m_wPeriodicDamage;		// �ֱ� ������ ��ġ
	BYTE				m_bySuctionType;
	DWORD				m_dwTargetObjectKey;	// ��� ��� ������Ʈ Ű.
};

