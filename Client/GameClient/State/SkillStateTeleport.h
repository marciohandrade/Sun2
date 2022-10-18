/**

    @author
        -지완

    @since

    @remarks
        -2005.07.18 클래스 추가   
*/

#pragma once

class StateSkill;

class SkillStateTeleport : public StateSkill
{
public:
	SkillStateTeleport() {}

	virtual STATECHANGE::TYPE	OnStart(Object* pOwner, DWORD CurTime);
};