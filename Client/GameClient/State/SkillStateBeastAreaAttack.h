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

class SkillStateBeastAreaAttack : public StateSkill
{
public:

	SkillStateBeastAreaAttack(int iLevel);
	~SkillStateBeastAreaAttack();

	virtual BOOL				CanTransite(Object* pOwner, DWORD CurTime, StateBase* pToTransiteState);
	virtual STATECHANGE::TYPE	OnStart(Object* pOwner, DWORD CurTime);
	virtual STATEPROCESS::TYPE	OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick);
	virtual STATECHANGE::TYPE	OnEnd(Object* pOwner, DWORD CurTime);

	int m_iHealCount;
};