
#pragma once

class StateBase;

class StateTransForm :
	public StateBase
{
	StateTransForm(void);
public:
	~StateTransForm(void);
	__forceinline static StateTransForm* GetInstance()	{	static StateTransForm d;	return &d;	};

	virtual int		GetStateKind() {	return STATE::TRANSFORM;	}
	virtual BOOL	CanTransite(Object* pOwner, DWORD CurTime, StateBase* pToTransiteState);

	virtual STATECHANGE::TYPE	OnStart(Object* pOwner, DWORD CurTime);
	virtual STATEPROCESS::TYPE	OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick);
	virtual STATECHANGE::TYPE	OnEnd(Object* pOwner, DWORD CurTime);


};


