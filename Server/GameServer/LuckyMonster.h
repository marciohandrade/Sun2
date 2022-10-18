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
	DWORD				m_dwRunAwayRegenTime;	// 도망가면서 사라지는 경우의 리젠타임
	util::Timer			m_ExpireTimer;			// 소멸타이머
	util::Timer			m_RetreatTimer;			// Retreat타이머
#ifdef __KR_0_20090519_EASY_GET_LUCKYMONSTER_
	DWORD				m_RegenHP;
#endif
};




