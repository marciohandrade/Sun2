#pragma once

class StateSkill;

class SkillStateDrillAttack : public StateSkill
{
public:

    SkillStateDrillAttack();
    ~SkillStateDrillAttack();

    STATEPROCESS::TYPE	OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick);

private:
    BOOL m_IsMove;
    BOOL m_IsDamaged;
};
