#pragma once

class StateSkill;

class SkillStateGuardianShield : public StateSkill
{
public:

    SkillStateGuardianShield(){}

    BOOL OnProcessCustemEvent( int nEventIndex, Object* pOwner, DWORD CurTime,DWORD dwTick );
};
