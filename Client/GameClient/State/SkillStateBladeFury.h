/**

	@author
		-지완

	@since

	@remarks
		-2006.01.08 신규 추가-지완 

*/

#pragma once

class StateSkill;

class SkillStateBladeFury : public StateSkill
{
public:
	SkillStateBladeFury() {}

	BOOL OnProcessCustemEvent( int nEventIndex, Object* pOwner, DWORD CurTime,DWORD dwTick );
};