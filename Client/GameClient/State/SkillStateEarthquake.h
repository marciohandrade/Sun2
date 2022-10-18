/**

    @author
        -지완

    @since

    @remarks
		-2006.01.09 신규 추가-지완 
 */

#pragma once

class StateSkill;

class SkillStateEarthQuake : public StateSkill
{
public:
	SkillStateEarthQuake(){}

	BOOL OnProcessCustemEvent( int nEventIndex, Object* pOwner, DWORD CurTime,DWORD dwTick );
};