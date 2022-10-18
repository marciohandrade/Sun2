/**

    ���� ����
    ���� �ȵ�
    \SA STATE::ATTACK, StateHeroAttack

    @author
        -������

    @since

    @remarks
        -2005.06.07 �ҽ� ����(����)
        - 
*/

#pragma once
class StateBase;

class StateAttack :public StateBase
{
protected:
	StateAttack(void);
public:
	~StateAttack(void);
	__forceinline static StateAttack* GetInstance()	{	static StateAttack d;	return &d;	};

	virtual int		GetStateKind() {	return STATE::ATTACK;	}
	virtual BOOL	CanTransite(Object* pOwner, DWORD CurTime, StateBase* pToTransiteState);

	virtual STATECHANGE::TYPE	OnStart(Object* pOwner, DWORD CurTime);
	virtual STATEPROCESS::TYPE	OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick);
	virtual STATECHANGE::TYPE	OnEnd(Object* pOwner, DWORD CurTime);

	BOOL	m_bCriAttkAni;
#ifdef _YJ_MOD_BLOW_MOTION_001188
	BOOL	m_bHit;
	DWORD	m_dwStopTime;
	//int		m_iSpeed;
#endif //_YJ_MOD_BLOW_MOTION_001188
};
