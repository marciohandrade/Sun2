/**

    @author
        -지완

    @since

    @remarks
    -2005.07.18 클래스 추가 완  
    
*/

#pragma once

class StateSkill;

class SkillStateFrozenShield : public StateSkill
{
public:
	SkillStateFrozenShield(){}

	BOOL OnProcessCustemEvent( int nEventIndex, Object* pOwner, DWORD CurTime,DWORD dwTick );
};