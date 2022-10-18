#pragma once

/**
	@author
		-±¤¼ö

	@since
		2008.10.20 
	@remarks

*/

class StateSkill;

class SkillStateRush :	public StateSkill
{
public:
	SkillStateRush();
	virtual ~SkillStateRush(void);
	virtual BOOL				CanTransite(Object* pOwner, DWORD CurTime, StateBase* pToTransiteState);
	virtual STATECHANGE::TYPE	OnStart(Object* pOwner, DWORD CurTime);
	virtual STATEPROCESS::TYPE	OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick);
	virtual STATECHANGE::TYPE	OnEnd(Object* pOwner, DWORD CurTime);
};

