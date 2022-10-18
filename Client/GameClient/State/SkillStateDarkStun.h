#pragma once

class StateSkill;

class SkillStateDarkStun : public StateSkill
{
public:

	SkillStateDarkStun();
	~SkillStateDarkStun( void );

	STATECHANGE::TYPE	OnStart(Object* pOwner, DWORD CurTime);
	STATEPROCESS::TYPE	OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick);

private:

	BOOL m_bStartMove;
	int m_iIllusionCount;

	void CreateIllusion( Character *pChr, DWORD LiveTime, DWORD DisappearTime, WzVector* pwvPos = NULL);
};
