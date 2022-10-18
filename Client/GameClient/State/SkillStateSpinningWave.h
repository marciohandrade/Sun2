/**

	@author
		-지완

	@since

	@remarks
		-2006.01.08 신규 추가-지완 
*/

#pragma once

class StateSkill;

class SkillStateSpinningWave : public StateSkill
{
public:
	SkillStateSpinningWave() {}

	STATECHANGE::TYPE	OnStart(Object* pOwner, DWORD CurTime);
	STATEPROCESS::TYPE	OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick);
    STATECHANGE::TYPE	OnEnd(Object* pOwner, DWORD CurTime);

private:
	DWORD  m_dwCurTime;
	DWORD  m_dwCurRotTime;

	
	DWORD  m_dwCopyTime;
	float  m_fStartAngle;
	DWORD  m_dwShotDelay;

};