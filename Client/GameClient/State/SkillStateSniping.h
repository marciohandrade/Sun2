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

class SkillStateSniping : public StateSkill
{
public:
	SkillStateSniping() {}

	BOOL OnProcessCustemEvent( int nEventIndex, Object* pOwner, DWORD CurTime,DWORD dwTick );
};