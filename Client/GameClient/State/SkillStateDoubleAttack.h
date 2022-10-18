/**
    @author
        -박준태

    @since

    @remarks
        -2005.06.08 소스 정리-지완 
        -2005.06.16 로직 작업 시작-지완  
*/

#pragma once

class StateSkill;

class SkillStateDoubleAttack : public StateSkill
{
public:
	SkillStateDoubleAttack() {}

    BOOL OnProcessCustemEvent( int nEventIndex, Object* pOwner, DWORD CurTime,DWORD dwTick );

	STATECHANGE::TYPE	OnStart(Object* pOwner, DWORD CurTime);
    STATECHANGE::TYPE	OnEnd(Object* pOwner, DWORD CurTime);
};



