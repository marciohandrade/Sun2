/**

	@author
		-����

	@since

	@remarks
		-2006.01.08 �ű� �߰�-���� 
*/


#pragma once

class StateSkill;

class SkillStateOverLimit : public StateSkill
{
public:
	SkillStateOverLimit();

	BOOL OnProcessCustemEvent( int nEventIndex, Object* pOwner, DWORD CurTime,DWORD dwTick );

	STATEPROCESS::TYPE	OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick);

private:
	WzVector m_vDest;
	BOOL     m_bMove;
	DWORD  m_dwCurTime;

};