/**
	@author
		-±¤¼ö

	@since
		2008.09
	@remarks

*/

#pragma once

class StateSkill;


class SkillStateDarkForce :	public StateSkill
{
public:
	SkillStateDarkForce() {}

	BOOL OnProcessCustemEvent( int nEventIndex, Object* pOwner, DWORD CurTime,DWORD dwTick );

};

