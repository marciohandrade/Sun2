/**

    @author
        -박준태

    @since

    @remarks
        -2005.06.08 소스 정리(지완)
        - 
*/

#pragma once

class StateSkill;

class SkillStateMachSlash : public StateSkill
{
public:
	SkillStateMachSlash() {}

	STATECHANGE::TYPE	OnStart(Object* pOwner, DWORD CurTime);
	STATEPROCESS::TYPE	OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick);

	void CreateIllusion( Character *pChr, DWORD LiveTime, DWORD DisappearTime );

private:
	int m_iIllusionCount;

};