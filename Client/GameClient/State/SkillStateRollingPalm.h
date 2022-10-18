#pragma once

class StateSkill;

class SkillStateRollingPalm : public StateSkill
{
public:

    SkillStateRollingPalm();
    ~SkillStateRollingPalm();

    STATEPROCESS::TYPE	OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick);

private:
    BOOL m_IsMove;
};
