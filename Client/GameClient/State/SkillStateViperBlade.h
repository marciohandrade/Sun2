/**

	@author
		-지완

	@since

	@remarks
		-2006.01.08 신규 추가-지완 
*/
#pragma once

class StateSkill;

class SkillStateViperBlade : public StateSkill
{
public:

	SkillStateViperBlade() {}

	virtual STATECHANGE::TYPE	OnStart(Object* pOwner, DWORD CurTime);
};