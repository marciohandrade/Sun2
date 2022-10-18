#pragma once

class StateBase;

class StateSpecialAction : public StateBase
{
public:
	StateSpecialAction(void);
	virtual ~StateSpecialAction(void);

	__forceinline static StateSpecialAction * GetInstance()	{	static StateSpecialAction d;	return &d;	};

	virtual int		GetStateKind() {	return STATE::NPC_SPECIAL_ACT_STATE;	}
	virtual BOOL	CanTransite(Object* pOwner, DWORD CurTime, StateBase* pToTransiteState);

	virtual STATECHANGE::TYPE	OnStart(Object* pOwner, DWORD CurTime);
	virtual STATEPROCESS::TYPE	OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick);
	virtual STATECHANGE::TYPE	OnEnd(Object* pOwner, DWORD CurTime);
};
