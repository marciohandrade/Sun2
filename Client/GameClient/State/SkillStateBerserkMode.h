/**

    @author
        -������

    @since

    @remarks
        -2005.06.08 �ҽ� ����-���� 
        -2005.06.16 ���� �۾� ����-����  
*/

#pragma once

class StateSkill;

class SkillStateBerserkMode : public StateSkill
{
public:
	SkillStateBerserkMode() {}

	STATECHANGE::TYPE	OnStart(Object* pOwner, DWORD CurTime);
	STATEPROCESS::TYPE	OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick);

private:
    float	m_speedScale;
    float	m_accelUp;			
    float	m_accelDown;		
    float   m_accel;		
    DWORD   m_dwCloneKey;
    DWORD   m_dwCloneLifeTime;
    float   m_CurScale;
    int		m_MotorRate;
};