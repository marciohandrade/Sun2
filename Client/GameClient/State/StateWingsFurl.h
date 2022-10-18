
#pragma once

class StateBase;
class StateWingsFurl : public StateBase
{
	StateWingsFurl(void);
public:
	~StateWingsFurl(void);
	__forceinline static StateWingsFurl* GetInstance()	{	static StateWingsFurl d;	return &d;	};

	virtual int		GetStateKind() {	return STATE::WINGS_FURL;	}
	virtual BOOL	CanTransite(Object* pOwner, DWORD CurTime, StateBase* pToTransiteState);

	virtual STATECHANGE::TYPE	OnStart(Object* pOwner, DWORD CurTime);
	virtual STATEPROCESS::TYPE	OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick);
	virtual STATECHANGE::TYPE	OnEnd(Object* pOwner, DWORD CurTime);
};

