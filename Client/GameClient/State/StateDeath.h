/**
    ���� ����

    \SA STATE::DEATH

    @author
        -������

    @since

    @remarks
        -2005.06.08 �ҽ� ����(����)
        - 
*/
#pragma once
class StateBase;

class StateDeath :
	public StateBase
{
	StateDeath(void);
public:
	~StateDeath(void);
	__forceinline static StateDeath* GetInstance()	{	static StateDeath d;	return &d;	};

	virtual int		GetStateKind() {	return STATE::DEATH;	}
	virtual BOOL	CanTransite(Object* pOwner, DWORD CurTime, StateBase* pToTransiteState);

	virtual STATECHANGE::TYPE	OnStart(Object* pOwner, DWORD CurTime);
	virtual STATEPROCESS::TYPE	OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick);
	virtual STATECHANGE::TYPE	OnEnd(Object* pOwner, DWORD CurTime);
};

