/**

    @author
        -지완

    @since

    @remarks
        - 스킬 종류와 상관 없이 하나의 클래스로 간다
        - 
*/

#pragma once

class StateSkill;

class SkillStateMonsterSkill : public StateSkill
{
public:

	SkillStateMonsterSkill();
	~SkillStateMonsterSkill();

	virtual STATECHANGE::TYPE	OnStart(Object* pOwner, DWORD CurTime);
	virtual STATEPROCESS::TYPE	OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick);
	virtual STATECHANGE::TYPE	OnEnd(Object* pOwner, DWORD CurTime);

protected:

	// 각 스킬 타입별로 필요한 초기화를 한다.
	BOOL    InitSkillForType(Object* pOwner);
	BOOL    EndSkillForType(Object* pOwner);

	BYTE    SkillHitCount(Object* pOwner);
	void    SetDirection(Object* pOwner);

	// 스킬별 프로세스 

	void	OnBeastMasterDeadlyStrikeProcess(Object* pOwner, DWORD CurTime,DWORD dwTick);
	void    OnSlaveArcherRecoveryShot(Object* pOwner, DWORD CurTime,DWORD dwTick);
	void    OnDragonZombieUpperBlow(Object* pOwner, DWORD CurTime,DWORD dwTick);

	void    OnEvilSorcererFireBall(Object* pOwner, DWORD CurTime,DWORD dwTick);
	void    OnEvilSorcererPressingWall(Object* pOwner, DWORD CurTime,DWORD dwTick);
	void    OnRedamorWarriorSonicBlade(Object* pOwner, DWORD CurTime,DWORD dwTick);
	
	void    OnHeavyHunterSnipiing(Object* pOwner, DWORD CurTime,DWORD dwTick);
	void    OnHeavyNoiseShot(Object* pOwner, DWORD CurTime,DWORD dwTick);
	void    OnHeavyWhistleShot(Object* pOwner, DWORD CurTime,DWORD dwTick);

	void    OnSlaveArcherHeavyShot(Object* pOwner, DWORD CurTime,DWORD dwTick);
	void	OnPillOff(Object* pOwner, DWORD CurTime,DWORD dwTick);
	void    OnFlameRulerExplorzenKnockle(Object* pOwner, DWORD CurTime,DWORD dwTick);

	void    OnFlameRulerFireWall(Object* pOwner, DWORD CurTime,DWORD dwTick);

	void    OnDragonZombieSoulOfDragon(Object* pOwner, DWORD CurTime,DWORD dwTick);
	void    OnCursedPriestIceSpray(Object* pOwner, DWORD CurTime,DWORD dwTick);

	void    OnCursedPriestIceStorm(Object* pOwner, DWORD CurTime,DWORD dwTick);
	void    OnMasterOfCastleBlizzard(Object* pOwner, DWORD CurTime,DWORD dwTick);

	void    OnMasterOfCastleFrostRing(Object* pOwner, DWORD CurTime,DWORD dwTick);
	void    OnMasterOfCastleIceMissile(Object* pOwner, DWORD CurTime,DWORD dwTick);
	
	void    OnFrameRulerMeteoShower(Object* pOwner, DWORD CurTime,DWORD dwTick);

	STATEPROCESS::TYPE 	OnFellenWarrionDashAttace( Object * pOwner , DWORD CurTime , DWORD dwTick);
	void	OnCursedArcherPoisonArrow(Object* pOwner , DWORD CurTime, DWORD dwTick);
	void	OnDarkWarriorBeat(Object * pOwner, DWORD CurTime, DWORD dwTick);
	void	OnArchbishopOfDarknessDoom(Object * pOwner, DWORD CurTime, DWORD dwTick);
	void	OnBindedArcherStunShot(Object* pOwner, DWORD CurTime, DWORD dwTick);
	void	OnFireCerberusFireBreath( Object* pOwner, DWORD CurTime, DWORD dwTick);

	//특별한 상황에 붙잡고 있어야 하는것들
	STATEPROCESS::TYPE    OnTackle(Object* pOwner, DWORD CurTime,DWORD dwTick);


	// 특별한 연출이 필요 없는 발사체 스킬
	void    OnShot(Object* pOwner, DWORD CurTime,DWORD dwTick);
	// 특별한 연출이 없은 타격계 스킬 
	void    OnHit(Object* pOwner, DWORD CurTime,DWORD dwTick);

	int		m_iHealCount;

	// 태클
	int		m_dwDustTime;
	DWORD	m_dwCopyTime;
	DWORD	m_color;
	DWORD	m_dwCurTime;

	HANDLE  m_hEffectHandle;
};