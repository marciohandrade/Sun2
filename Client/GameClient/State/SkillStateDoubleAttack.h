/**
    @author
        -������

    @since

    @remarks
        -2005.06.08 �ҽ� ����-���� 
        -2005.06.16 ���� �۾� ����-����  
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



