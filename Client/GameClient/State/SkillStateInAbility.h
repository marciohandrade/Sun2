#pragma once 


class StateSkill;

class SkillStateInAbility : public StateSkill
{
public:
	SkillStateInAbility(){}

	BOOL OnProcessCustemEvent( int nEventIndex, Object* pOwner, DWORD CurTime,DWORD dwTick );

};
