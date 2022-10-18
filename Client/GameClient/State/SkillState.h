/**

    ���̽� ��ų  
    @author
        -������,���� 

    @since

    @remarks
        -2005.06.19 enter,leave,���� ��� ���� �߰� -����
        -2005.06.20 ���� �̵�-����  
		-2005.12.15 ���� ���� Ŭ���������� Ư���� ��ų ������ ������ �ν��Ͻ��� �����ؼ� �������� ����.
		-2005.12.15 ���� Ŭ�������� ȣ�� �Ǵ� SendSkillPacket�� ������ �ø�-���� 

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
	void						Finalize(Object* pOwner);	// �峪����

protected:

	virtual BOOL				CanTransite(Object* pOwner, DWORD CurTime, StateBase* pToTransiteState) { return TRUE; }
    virtual STATECHANGE::TYPE	OnStart(Object* pOwner, DWORD CurTime);
    virtual STATEPROCESS::TYPE	OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick);
    virtual STATECHANGE::TYPE	OnEnd(Object* pOwner, DWORD CurTime);

    virtual STATEPROCESS::TYPE  LeaveProcess(Object* pOwner, DWORD CurTime,DWORD dwTick);   //���� �߰� 
    virtual VOID                EnterProcess(Object* pOwner, DWORD CurTime,DWORD dwTick);


protected:
	virtual BOOL				OnProcessCustemEvent( int nEventIndex, Object* pOwner, DWORD CurTime,DWORD dwTick ) { return TRUE; }


protected:
	// �� �Լ��� ��ų�� ���� ����ϴ°��, �� Hero�� ��쿡�� �۵��� �ǹǷ� g_pHero�� �ٷ� ����Ѵ�
	BOOL                		SendSkillPacket();

	// ����� ������ġ(Range)�� ���Ѵ�. (�������� ��ǥ��ġ�� ���ҋ�..)
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

	WzEvent		m_event;					//Ÿ�� ������ ���� ������ �̺�Ʈ 
	float		m_fRet;						//���ϸ��̼� ��ô��    

protected:
	SLOTCODE	m_dwSkillCode;
	SLOTCODE	m_dwSkillClassCode;

    int			m_iHitProcessed;
    int			m_iHitCount;

	BOOL		m_isCastAni;				//ĳ��Ʈ ���ϰ� �ֳ�
	DWORD		m_dwTotalCastTime;			//ĳ��Ʈ �ð� 
	DWORD		m_dwCurCastTime;			//ĳ��Ʈ �ð� 
	HANDLE		m_hCastHandle[2];			// ĳ���� ����Ʈ�� ���� �����ϱ� ���� �ڵ�
	DWORD       m_dwStartProjectileKey;


private:
	void	_TestRecvDamageSkillEffect(Character* pCharacter);
};

