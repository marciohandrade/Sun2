#pragma once 

class StateSkill;

#define MAX_CHARGE_ATTACK_CLONE	5

class SkillStateChargeAttack : public StateSkill
{
private:
	struct stChargeAttackClone
	{
		DWORD		dwCloneID;
		WzVector	wvDestPos;
	};

	stChargeAttackClone m_sChargeAttackClone[MAX_CHARGE_ATTACK_CLONE];

	bool			m_bReadyAttack;
	bool			m_bReadyMove;
	bool			m_bBackToTheStartPos;
	DWORD			m_dwAttackTick;
	int				m_iCloneCreateCount;

public:
	SkillStateChargeAttack() {}

	STATECHANGE::TYPE	OnStart(Object* pOwner, DWORD CurTime);
	STATEPROCESS::TYPE	OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick);
	STATECHANGE::TYPE	OnEnd(Object* pOwner, DWORD CurTime);

	BOOL OnProcessCustemEvent( int nEventIndex, Object* pOwner, DWORD CurTime,DWORD dwTick );

};
