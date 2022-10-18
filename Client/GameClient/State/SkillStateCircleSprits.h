#pragma once 


class StateSkill;

class SkillStateCircleSprits : public StateSkill
{
public:
	SkillStateCircleSprits(){}

	BOOL OnProcessCustemEvent( int nEventIndex, Object* pOwner, DWORD CurTime,DWORD dwTick );
};
