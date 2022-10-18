
#pragma once

class StateSkill;


class SkillStateDragonBead :	public StateSkill
{
public:
	SkillStateDragonBead() {}

	BOOL OnProcessCustemEvent( int nEventIndex, Object* pOwner, DWORD CurTime,DWORD dwTick );
};
