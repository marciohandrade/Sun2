#pragma once

class StateSkill;

class SkillStateMysticDance : public StateSkill
{
public:
	SkillStateMysticDance();

private:
	virtual STATECHANGE::TYPE	OnStart(Object* pOwner, DWORD CurTime);
	virtual STATEPROCESS::TYPE	OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick);

	void Finalize(Object* pOwner);	// Β₯Αυ

	DWORD m_StartTime;
};
