/**

    베이스 스킬  
    @author
        -박준태,지완 

    @since

    @remarks
        -2005.06.19 enter,leave,공용 멤버 변수 추가 -지완
        -2005.06.20 파일 이동-지완  
		-2005.12.15 순수 가상 클래스였으나 특별한 스킬 연출이 없는한 인스턴스를 생성해서 공용으로 쓴다.
		-2005.12.15 하위 클래스에서 호출 되던 SendSkillPacket을 상위로 올림-지완 

*/

#pragma once

class Clone;
class Character;
class StateBase;
class SkillScriptInfo;
struct EffectInfo;

#define MAX_HIT 12



class StateSkill : public StateBase
{
public:
	StateSkill();
    virtual ~StateSkill();

public:
    SLOTCODE GetSkillClassCode() {return m_dwSkillClassCode; }
    SLOTCODE GetSkillCode() { return m_dwSkillCode; }

private:
	int							GetStateKind() { return STATE::SKILL; }
	void                		Initialize(Object* pOwner);
	void						OnForceChange(Object* pOwner);

protected:
	void						Finalize(Object* pOwner);	// 욕나오네

protected:

	virtual BOOL				CanTransite(Object* pOwner, DWORD CurTime, StateBase* pToTransiteState) { return TRUE; }
    virtual STATECHANGE::TYPE	OnStart(Object* pOwner, DWORD CurTime);
    virtual STATEPROCESS::TYPE	OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick);
    virtual STATECHANGE::TYPE	OnEnd(Object* pOwner, DWORD CurTime);

    virtual STATEPROCESS::TYPE  LeaveProcess(Object* pOwner, DWORD CurTime,DWORD dwTick);   //지완 추가 
    virtual VOID                EnterProcess(Object* pOwner, DWORD CurTime,DWORD dwTick);


protected:
	virtual BOOL				OnProcessCustemEvent( int nEventIndex, Object* pOwner, DWORD CurTime,DWORD dwTick ) { return TRUE; }


protected:
	// 이 함수는 스킬을 직접 사용하는경우, 즉 Hero인 경우에만 작동이 되므로 g_pHero를 바로 사용한다
	BOOL                		SendSkillPacket();

	// 대상의 근접위치(Range)를 구한다. (돌진기의 목표위치를 구할떄..)
	WzVector					GetTargetNearPosition( const WzVector& StartPosition, Character* pTarget, float Range );

	void						ProcessStartAttackSerial(Character* pCharacter);
	void						ProcessStartAnimation(Character* pCharacter);
	void						ProcessStartDirection(Character* pCharacter);
	void						ProcessStartSkillEffect(Character* pCharacter);

	BOOL						ProcessMove(Character* pCharacter,float fSpeed,DWORD dwTick);
	void						ProcessAnimationEvent(Character* pCharacter, DWORD CurTime, DWORD dwTick );

	SkillScriptInfo*			GetSkillInfo() { return m_pSkillScript; }

	BOOL                		StartSkillAnimation(Character* pCharacter, BOOL bLoop = FALSE );

	BOOL                		CheckEvent();
	BOOL                		ProcessHit(Character* pCharacter,int HitCount);
	BOOL                		CheckNotProcessedHitEvent(int HitCount);

	void						ReleaseCastingEffect();

	float						GetAnimationRate() { return m_fRet; }
	void						SetAnimationRate( float fRate ) { m_fRet = fRate; }

private:

	SkillScriptInfo* m_pSkillScript;

	WzEvent		m_event;					//타격 점등의 공격 프레임 이벤트 
	float		m_fRet;						//에니메이션 진척도    

protected:
	SLOTCODE	m_dwSkillCode;
	SLOTCODE	m_dwSkillClassCode;

    int			m_iHitProcessed;
    int			m_iHitCount;

	BOOL		m_isCastAni;				//캐스트 에니가 있나
	DWORD		m_dwTotalCastTime;			//캐스트 시간 
	DWORD		m_dwCurCastTime;			//캐스트 시간 
	HANDLE		m_hCastHandle[2];			// 캐스팅 이펙트를 직접 삭제하기 위한 핸들
	DWORD       m_dwStartProjectileKey;


private:
	void	_TestRecvDamageSkillEffect(Character* pCharacter);
};

