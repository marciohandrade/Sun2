#pragma once

#include "NPC.h"

class Monster : public NPC
{
public:		Monster();
	virtual	~Monster();

	virtual	VOID			InitMonster( WORD NPCCode, BYTE byMoveType, DWORD dwMoveAreaID );
protected:
	virtual	BOOL			Init();

public:
	virtual	BOOL			Update( DWORD dwDeltaTick );

	virtual	VOID			Reuse()		{ Release(); }

	virtual	VOID			OnEnterField( GameField & IN rField, const WzVector & IN rwzVec, int iAngle=-1 );
	virtual	VOID			OnLeaveField();

	virtual	VOID			OnDead();

	virtual	VOID			SetRegenPos( const WzVector& regenPos )			{ m_RegenPos = regenPos; }
	WzVector&				GetRegenPos()									{ return m_RegenPos; }
	virtual	BOOL			IsOutOfWanderRadius();
	virtual	BOOL			IsOutOfRegenLocationLimit( WzVector &vTargetPos );
			VOID			StartSummonedTimer(DWORD time);

protected:
	virtual	VOID			Release();

private:
	WzVector				m_RegenPos;		// 생성될때 최초로 field에 추가된 위치: 몬스터의 귀환? 좌표로 이용.
	BOOL					m_bSummoned;
	util::SimpleTimer		m_LiveIdle;
};
