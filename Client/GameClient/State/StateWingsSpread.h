
#pragma once

class StateBase;
class StateWingsSpread : public StateBase
{
	StateWingsSpread(void);
public:
	~StateWingsSpread(void);
	__forceinline static StateWingsSpread* GetInstance()	{	static StateWingsSpread d;	return &d;	};

	virtual int		GetStateKind() {	return STATE::WINGS_SPREAD;	}
	virtual BOOL	CanTransite(Object* pOwner, DWORD CurTime, StateBase* pToTransiteState);
    virtual bool    CanTransite(StateBase* from_state);

	virtual STATECHANGE::TYPE	OnStart(Object* pOwner, DWORD CurTime);
	virtual STATEPROCESS::TYPE	OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick);
	virtual STATECHANGE::TYPE	OnEnd(Object* pOwner, DWORD CurTime);
};

