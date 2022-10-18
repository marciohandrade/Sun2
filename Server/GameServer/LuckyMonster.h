#pragma once

#include "Monster.h"
#include <Timer.h>


class LuckyMonster : public Monster
{
public:
	LuckyMonster();
	virtual ~LuckyMonster() {}

	virtual VOID		InitMonster( WORD NPCCode, BYTE byMoveType, DWORD dwMoveAreaID );
	virtual BOOL		Update( DWORD dwDeltaTick );

	DWORD				GetRunAwayRegenTime()	{ return m_dwRunAwayRegenTime; }
	VOID				SetExpireTime( DWORD dwInterval );

protected:
	virtual BOOL		Init();

protected:
    virtual void     _Damaged(DamageArgs* damage_arg);
	virtual VOID		OnDead();

private:
	int					GetRunAwayAngle();

#ifdef __KR_0_20090519_EASY_GET_LUCKYMONSTER_
	BOOL				CheckExpireTimeStart();
#endif

private:
	DWORD				m_dwRunAwayRegenTime;	// �������鼭 ������� ����� ����Ÿ��
	util::Timer			m_ExpireTimer;			// �Ҹ�Ÿ�̸�
	util::Timer			m_RetreatTimer;			// RetreatŸ�̸�
#ifdef __KR_0_20090519_EASY_GET_LUCKYMONSTER_
	DWORD				m_RegenHP;
#endif
};




