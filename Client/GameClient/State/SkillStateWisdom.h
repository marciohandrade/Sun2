/**

    @author
        -����

    @since

    @remarks
        -2005.07.18 Ŭ���� �߰�    
*/

#pragma once

class StateSkill;

class SkillStateWisdom : public StateSkill
{
public:
	SkillStateWisdom() {}

	BOOL OnProcessCustemEvent( int nEventIndex, Object* pOwner, DWORD CurTime,DWORD dwTick );

};