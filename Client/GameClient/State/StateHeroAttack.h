/**
    ���ΰ�(Hero)���� ���� ����

    \SA STATE::ATTACK, StateAttack

    @author
        -������

    @since

    @remarks
        -2005.06.08 �ҽ� ����(����)
        - 
*/
#pragma once

class StateBase;

class StateHeroAttack : public StateAttack
{
	StateHeroAttack(void);
public:
	~StateHeroAttack(void);
	__forceinline static StateHeroAttack* GetInstance()	{	static StateHeroAttack d;	return &d;	};

	virtual int		GetStateKind() {	return STATE::ATTACK;	}
	virtual BOOL	CanTransite(Object* pOwner, DWORD CurTime, StateBase* pToTransiteState);

	virtual STATECHANGE::TYPE	OnStart(Object* pOwner, DWORD CurTime);
	virtual STATEPROCESS::TYPE	OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick);
	virtual STATECHANGE::TYPE	OnEnd(Object* pOwner, DWORD CurTime);

	BOOL    m_bTrail;
};
