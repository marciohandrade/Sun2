#pragma once

class StateSkill;

class SkillStateTackle : public StateSkill
{
public:
	SkillStateTackle();

	STATECHANGE::TYPE	OnStart(Object* pOwner, DWORD CurTime);
	STATEPROCESS::TYPE	OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick);

private:
    BOOL m_bStartMove;
    DWORD m_dwCloneKey;
    DWORD m_dwCrushTime;

    void _AddHitEffect( Character* pCharacter );
};



