#pragma once

class StateSkill;

class SkillStateRisingSerpent : public StateSkill
{
public:

    SkillStateRisingSerpent();
    ~SkillStateRisingSerpent();

    STATECHANGE::TYPE OnStart(Object* pOwner, DWORD CurTime);
    STATEPROCESS::TYPE	OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick);
};
