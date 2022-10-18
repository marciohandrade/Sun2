#pragma once

class StateSkill;

class SkillStateDashStun : public StateSkill
{
public:
	SkillStateDashStun();

private:
	virtual STATECHANGE::TYPE	OnStart(Object* pOwner, DWORD CurTime);
	virtual STATEPROCESS::TYPE	OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick);

private:
	BOOL m_bStartMove;
	DWORD m_dwCloneKey;
	DWORD m_dwCrushTime;

	void _AddHitEffect( Character* pCharacter );
};
