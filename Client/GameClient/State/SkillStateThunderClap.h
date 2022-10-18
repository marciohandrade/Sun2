/**

    @author
        -������

    @since

    @remarks
        -2005.06.08 �ҽ� ����-���� 
        -2005.06.16 ���� �۾� ����-���� 
*/

#pragma once

class StateSkill;

class SkillStateThunderClap : public StateSkill
{
public:
	SkillStateThunderClap(int iLevel);
	~SkillStateThunderClap();

	virtual BOOL				CanTransite(Object* pOwner, DWORD CurTime, StateBase* pToTransiteState);
	virtual STATECHANGE::TYPE	OnStart(Object* pOwner, DWORD CurTime);
	virtual STATEPROCESS::TYPE	OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick);
	virtual STATECHANGE::TYPE	OnEnd(Object* pOwner, DWORD CurTime);

	int m_iHitProcessed;
	int m_iHitCount;
};