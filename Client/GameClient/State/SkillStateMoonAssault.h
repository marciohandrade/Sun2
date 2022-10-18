/**

    @author
        -박준태

    @since

    @remarks
        -2005.06.08 소스 정리(지완)
        - 
*/

#pragma once

class StateSkill;

class SkillStateMoonAssault : public StateSkill
{
public:
	SkillStateMoonAssault();
	~SkillStateMoonAssault();

	//virtual BOOL				CanTransite(Object* pOwner, DWORD CurTime, StateBase* pToTransiteState);
	//virtual STATECHANGE::TYPE	OnStart(Object* pOwner, DWORD CurTime);
	//virtual STATEPROCESS::TYPE	OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick);
	//virtual STATECHANGE::TYPE	OnEnd(Object* pOwner, DWORD CurTime);

	//int m_iHitProcessed;
	//int m_iHitCount;
	//HANDLE m_hTrailEffect;
};