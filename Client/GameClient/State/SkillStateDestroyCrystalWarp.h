#pragma once

class StateSkill;

class SkillStateDestroyCrystalWarp : public StateSkill
{
public:

	SkillStateDestroyCrystalWarp();
	virtual ~SkillStateDestroyCrystalWarp( void );

	virtual BOOL				CanTransite(Object* pOwner, DWORD CurTime, StateBase* pToTransiteState);
	virtual STATECHANGE::TYPE	OnStart(Object* pOwner, DWORD CurTime);
	virtual STATEPROCESS::TYPE	OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick);
	virtual STATECHANGE::TYPE	OnEnd(Object* pOwner, DWORD CurTime);
};