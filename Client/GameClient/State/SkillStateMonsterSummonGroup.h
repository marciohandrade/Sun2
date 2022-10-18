/**

    @author
        -Áö¿Ï 

    @since

    @remarks
        -
        - 
*/

#pragma once

class StateSkill;

class SkillStateMonsterSummonGroup : public StateSkill
{
public:

	SkillStateMonsterSummonGroup(int iLevel);
	~SkillStateMonsterSummonGroup();

	virtual BOOL				CanTransite(Object* pOwner, DWORD CurTime, StateBase* pToTransiteState);
	virtual STATECHANGE::TYPE	OnStart(Object* pOwner, DWORD CurTime);
	virtual STATEPROCESS::TYPE	OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick);
	virtual STATECHANGE::TYPE	OnEnd(Object* pOwner, DWORD CurTime);

	int m_iHealCount;
};