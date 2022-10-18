#pragma once

class StateSkill;

class SkillStateMonsterTackle : public StateSkill
{
public:
    SkillStateMonsterTackle() {}

    STATECHANGE::TYPE	OnStart(Object* pOwner, DWORD CurTime);
    STATEPROCESS::TYPE	OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick);

protected:
    int    m_dwDustTime;
    DWORD  m_dwCopyTime;
    DWORD  m_color;
    BOOL   m_bCreateEffect;
};