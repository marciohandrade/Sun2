/**

    @author
        -����

    @since

    @remarks
		-2006.01.09 �ű� �߰�-���� 
 */

#pragma once

class StateSkill;

class SkillStateEarthQuake : public StateSkill
{
public:
	SkillStateEarthQuake(){}

	BOOL OnProcessCustemEvent( int nEventIndex, Object* pOwner, DWORD CurTime,DWORD dwTick );
};