/**

	@author
		-����

	@since

	@remarks
		-2006.01.08 �ű� �߰�-���� 

*/

#pragma once

class StateSkill;

class SkillStateBladeFury : public StateSkill
{
public:
	SkillStateBladeFury() {}

	BOOL OnProcessCustemEvent( int nEventIndex, Object* pOwner, DWORD CurTime,DWORD dwTick );
};