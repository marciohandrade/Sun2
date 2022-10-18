
#pragma once

class StateBase;
class StateRiderFurl : public StateBase
{
	StateRiderFurl(void);
public:
	~StateRiderFurl(void);
	__forceinline static StateRiderFurl* GetInstance()	{	static StateRiderFurl d;	return &d;	};

	virtual int		GetStateKind() {	return STATE::RIDER_FURL;	}
	virtual BOOL	CanTransite(Object* pOwner, DWORD CurTime, StateBase* pToTransiteState);

	virtual STATECHANGE::TYPE	OnStart(Object* pOwner, DWORD CurTime);
	virtual STATEPROCESS::TYPE	OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick);
	virtual STATECHANGE::TYPE	OnEnd(Object* pOwner, DWORD CurTime);
};

