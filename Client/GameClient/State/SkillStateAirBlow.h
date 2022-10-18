/**

    @author
        -������

    @since

    @remarks
        -2005.06.08 �ҽ� ����(����)
		-2006.01.12 ���� �۾�����(����)

*/

#pragma once

class StateSkill;

class SkillStateAirBlow : public StateSkill
{
public:
    SkillStateAirBlow() {}

	virtual STATECHANGE::TYPE	OnStart(Object* pOwner, DWORD CurTime);
	virtual STATEPROCESS::TYPE	OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick);
	virtual STATECHANGE::TYPE	OnEnd(Object* pOwner, DWORD CurTime);

#ifdef _NA_007619_20140916_PROCESS_CHANGE_AIRBLOW_DOWN
#else
protected:
	HANDLE m_hTrailEffect;
#endif // _NA_007619_20140916_PROCESS_CHANGE_AIRBLOW_DOWN
};