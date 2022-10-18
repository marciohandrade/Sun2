#pragma once 

class StateSkill;

class SkillStateSpritSlug : public StateSkill
{
public:
	SkillStateSpritSlug() {}

#ifdef _NA_006221_20121031_SPRITSLUG_SKILL_MODIFY
    STATECHANGE::TYPE	OnStart(Object* pOwner, DWORD CurTime);
#endif //_NA_006221_20121031_SPRITSLUG_SKILL_MODIFY

	BOOL OnProcessCustemEvent( int nEventIndex, Object* pOwner, DWORD CurTime,DWORD dwTick );

};
