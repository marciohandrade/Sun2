/**

    @author
        -지완

    @since

    @remarks
        -2005.07.18 클래스 추가   
*/

#pragma once

class StateSkill;

class SkillStateFireBall : public StateSkill
{
public:
	SkillStateFireBall() {}

	BOOL OnProcessCustemEvent( int nEventIndex, Object* pOwner, DWORD CurTime,DWORD dwTick );
};