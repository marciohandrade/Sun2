#pragma once 


class StateSkill;

class SkillStatePurge : public StateSkill
{
public:
	SkillStatePurge(){}

	STATECHANGE::TYPE	OnStart(Object* pOwner, DWORD CurTime);
	STATEPROCESS::TYPE	OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick);

	BOOL OnProcessCustemEvent( int nEventIndex, Object* pOwner, DWORD CurTime,DWORD dwTick );

private:
	WzVector			m_wvTargetPos;

};
