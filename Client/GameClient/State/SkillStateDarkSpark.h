

/**
	@author
		-±¤¼ö

	@since
		2008.09.24 
	@remarks
		
*/
#pragma once

class StateSkill;


class SkillStateDarkSpark :	public StateSkill
{
public:
	SkillStateDarkSpark() {}

	BOOL OnProcessCustemEvent( int nEventIndex, Object* pOwner, DWORD CurTime,DWORD dwTick );

};
