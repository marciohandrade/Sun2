/**

    @author
        -박준태

    @since

    @remarks
        -2005.06.08 소스 정리(지완)
        - 
*/

#pragma once

class StateSkill;

class SkillStateDragonEye : public StateSkill
{
public:
	SkillStateDragonEye() {}

	STATECHANGE::TYPE	OnStart(Object* pOwner, DWORD CurTime);


	static void PlayNonstopEffect(Object* pOwner, Object *pTarget);

};