
#pragma once

class StateBase;
class StateRiderSpread : public StateBase
{
	StateRiderSpread(void);
public:
	~StateRiderSpread(void);
	__forceinline static StateRiderSpread* GetInstance()	{	static StateRiderSpread d;	return &d;	};

	virtual int		GetStateKind() {	return STATE::RIDER_SPREAD;	}
	virtual BOOL	CanTransite(Object* pOwner, DWORD CurTime, StateBase* pToTransiteState);
    virtual bool    CanTransite(StateBase* from_state);

	virtual STATECHANGE::TYPE	OnStart(Object* pOwner, DWORD CurTime);
	virtual STATEPROCESS::TYPE	OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick);
	virtual STATECHANGE::TYPE	OnEnd(Object* pOwner, DWORD CurTime);
};

