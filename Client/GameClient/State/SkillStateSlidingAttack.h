#pragma once

class StateSkill;

class SkillStateSlidingAttack : public StateSkill
{
public:

    SkillStateSlidingAttack();
    ~SkillStateSlidingAttack();


private:
    virtual STATECHANGE::TYPE	OnStart(Object* pOwner, DWORD CurTime);
    virtual STATEPROCESS::TYPE	OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick);

private:
    BOOL m_bStartMove;
    BOOL m_bHit;
};
