/**

    @author
        -������

    @since

    @remarks
        -2005.06.08 �ҽ� ����(����)
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