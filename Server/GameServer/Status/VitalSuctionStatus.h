#pragma once

#include "AbilityStatus.h"

class VitalSuctionStatus : public AbilityStatus
{
public:
	VitalSuctionStatus(void);
	~VitalSuctionStatus(void);

public:
	virtual VOID		Init( Character *pOwner, Character *pAttacker, Ability *pAbility );
	virtual VOID		Execute();
	virtual	BOOL		CanRemove();

private:
    DWORD               suction_hp_;

	DWORD				m_dwAttackerObjectKey;
	GameField*			m_pGameField;
	WzVector			m_vGhost;								// øµ»•..
	WzVector			m_vGhostUnitDir;						// øµ»• ¥‹¿ß ∫§≈Õ.
	float				m_fSpeed;
	float				m_fAccelate;
	DWORD				m_dwFirstTick;
	DWORD				m_dwPreTick;
};

