/**

    ��Ű�� �� ���� 
    @author
        -������

    @since

    @remarks
        -2005.06.08 �ҽ� ����(����)
        - 
*/

#pragma once

class StateSkill;

class SkillStateBloodRain : public StateSkill
{
public:
	SkillStateBloodRain() {}

	virtual STATECHANGE::TYPE	OnStart(Object* pOwner, DWORD CurTime);
	virtual STATEPROCESS::TYPE	OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick);

private:
	DWORD  m_dwShotDelay;
	int    m_iShotCount;
	int    m_iDamageCount;
};