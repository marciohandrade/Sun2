#pragma once 


class StateSkill;

class SkillStateSaintAid : public StateSkill
{
public:
	SkillStateSaintAid() {}

	BOOL OnProcessCustemEvent( int nEventIndex, Object* pOwner, DWORD CurTime,DWORD dwTick );

};
