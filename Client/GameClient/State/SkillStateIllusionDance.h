/**

    dragon knight �нż�!
    @author
        -������

    @since

    @remarks
        -2005.06.08 �ҽ� ����(����)
        - 
*/

#pragma once

class StateSkill;

class SkillStateIllusionDance : public StateSkill
{
public:
	struct Illusion
	{
		BOOL     bStart;
		DWORD    dwDelay;
		DWORD    dwCloneKey;
		WzVector vIllusionStart;
		WzVector vIllusionEnd;
	};

	SkillStateIllusionDance() {}

	STATECHANGE::TYPE	OnStart(Object* pOwner, DWORD CurTime);
	STATEPROCESS::TYPE	OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick);
	STATECHANGE::TYPE	OnEnd(Object* pOwner, DWORD CurTime);
    STATEPROCESS::TYPE  LeaveProcess(Object* pOwner, DWORD CurTime,DWORD dwTick);   //���� �߰� 
    VOID                EnterProcess(Object* pOwner, DWORD CurTime,DWORD dwTick);

private:
	void	MakeIllusion(int iIllusionIndex);
	void	ProcessIllusion(Object* pOwner, DWORD dwTick);

	int		m_iAttackCount;
	int		m_iDamageProcessCount;
#ifdef _NA_007294_20140613_DRAGONKNIGHT_RENEWAL
#else
	DWORD	m_dwLastingDelay;
#endif // _NA_007294_20140613_DRAGONKNIGHT_RENEWAL

	static const int	MAX_ILLUSION_ATTACK = 5;
	Illusion			m_arIllusion[MAX_ILLUSION_ATTACK];
};