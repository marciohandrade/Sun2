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

class SkillStateMultiTarget : public StateSkill
{
public:
	SkillStateMultiTarget();
	~SkillStateMultiTarget();

	virtual STATECHANGE::TYPE	OnStart(Object* pOwner, DWORD CurTime);
	virtual STATEPROCESS::TYPE	OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick);

private:
	DWORD m_dwAttackSerial;
};