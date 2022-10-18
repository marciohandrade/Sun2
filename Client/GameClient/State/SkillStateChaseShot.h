#pragma once


class StateSkill;

class SkillStateChaseShot : public StateSkill
{
public:
	SkillStateChaseShot() {}

	BOOL OnProcessCustemEvent( int nEventIndex, Object* pOwner, DWORD CurTime,DWORD dwTick );
};
