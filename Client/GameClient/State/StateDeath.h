/**
    죽음 상태

    \SA STATE::DEATH

    @author
        -박준태

    @since

    @remarks
        -2005.06.08 소스 정리(지완)
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

