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

class SkillStateSonicBlade : public StateSkill
{
public:
	SkillStateSonicBlade() {}

	BOOL OnProcessCustemEvent( int nEventIndex, Object* pOwner, DWORD CurTime,DWORD dwTick );
};