/**

	@author
		-지완

	@since

	@remarks
		-2006.01.09 신규 추가-지완 
*/
#pragma once

class StateSkill;

class SkillStateExplozenAttack : public StateSkill
{
public:

	SkillStateExplozenAttack();
	~SkillStateExplozenAttack();

	virtual BOOL				CanTransite(Object* pOwner, DWORD CurTime, StateBase* pToTransiteState);
	virtual STATECHANGE::TYPE	OnStart(Object* pOwner, DWORD CurTime);
	virtual STATEPROCESS::TYPE	OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick);
	virtual STATECHANGE::TYPE	OnEnd(Object* pOwner, DWORD CurTime);

protected:
	BOOL		m_bFrontMove;
	BOOL		m_bUpMove;
	BOOL		m_bDownMove;
	int			m_iIllusionCount;
	DWORD		m_dwCopyTime;
	DWORD		m_dwCurTime;
	WzVector	m_vStartpos;
	WzVector	m_vTargetpos;
	float       m_fHeight;
	float       m_fRate;

};