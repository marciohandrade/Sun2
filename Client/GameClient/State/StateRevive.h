#pragma once




class StateBase;

class StateRevive :
	public StateBase
{
	StateRevive(void);
public:
	~StateRevive(void);
	__forceinline static StateRevive* GetInstance()	{	static StateRevive d;	return &d;	};

	virtual int		GetStateKind() {	return STATE::REVIVE;	}
	virtual BOOL	CanTransite(Object* pOwner, DWORD CurTime, StateBase* pToTransiteState);

	virtual STATECHANGE::TYPE	OnStart(Object* pOwner, DWORD CurTime);
	virtual STATEPROCESS::TYPE	OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick);
	virtual STATECHANGE::TYPE	OnEnd(Object* pOwner, DWORD CurTime);


};


