#pragma once

class StateSkill;

class SkillStateFlyingCharging : public StateSkill
{
public:

    SkillStateFlyingCharging();
    ~SkillStateFlyingCharging();

    STATEPROCESS::TYPE	OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick);

private:
    BOOL m_IsMove;
    BOOL m_IsDamaged;
    float m_fRange;
    BOOL m_IsStarted;
};
