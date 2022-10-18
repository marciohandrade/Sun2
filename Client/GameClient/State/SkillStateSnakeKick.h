#pragma once

class StateSkill;

class SkillStateSnakeKick : public StateSkill
{
public:

    SkillStateSnakeKick();
    ~SkillStateSnakeKick();

    STATEPROCESS::TYPE	OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick);

private:
    BOOL m_IsMove;
    BOOL m_IsDamaged;
};
