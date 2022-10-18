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

class SkillStateManaDrain : public StateSkill
{
public:
	SkillStateManaDrain() {}

	BOOL OnProcessCustemEvent( int nEventIndex, Object* pOwner, DWORD CurTime,DWORD dwTick );

};