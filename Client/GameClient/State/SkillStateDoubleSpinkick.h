/**

    @author
        -������

    @since

    @remarks
        -2005.06.08 �ҽ� ����(����)
        - 
*/

#pragma once

class StateSkill;

class SkillStateDoubleSpinkick : public StateSkill
{
public:
	SkillStateDoubleSpinkick();
	~SkillStateDoubleSpinkick();

	virtual STATECHANGE::TYPE	OnStart(Object* pOwner, DWORD CurTime);
	virtual STATEPROCESS::TYPE	OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick);
	virtual STATECHANGE::TYPE	OnEnd(Object* pOwner, DWORD CurTime);

	HANDLE m_hTrailEffect;
};