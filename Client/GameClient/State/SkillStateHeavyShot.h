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

class SkillStateHeavyShot : public StateSkill
{
public:
	SkillStateHeavyShot() {}

	BOOL OnProcessCustemEvent( int nEventIndex, Object* pOwner, DWORD CurTime,DWORD dwTick );
};