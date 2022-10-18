#pragma once

class StateBase;

class StateFireCracker : public StateBase
{
	StateFireCracker(void);
public:
	~StateFireCracker(void);
	__forceinline static StateFireCracker* GetInstance()	{	static StateFireCracker d;	return &d;	};

	virtual int		GetStateKind() {	return STATE::FIRECRACKER;	}
	virtual BOOL	CanTransite(Object* pOwner, DWORD CurTime, StateBase* pToTransiteState);

	virtual STATECHANGE::TYPE	OnStart(Object* pOwner, DWORD CurTime);
	virtual STATEPROCESS::TYPE	OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick);
	virtual STATECHANGE::TYPE	OnEnd(Object* pOwner, DWORD CurTime);
};

