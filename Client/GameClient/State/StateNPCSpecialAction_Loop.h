#pragma once

class StateBase;

class StateNPCSpecialAction_Loop : public StateBase
{
public:
	StateNPCSpecialAction_Loop(void);
	virtual ~StateNPCSpecialAction_Loop(void);

	__forceinline static StateNPCSpecialAction_Loop * GetInstance()	{	static StateNPCSpecialAction_Loop d;	return &d;	};

	virtual int		GetStateKind() {	return STATE::NPC_SPECIAL_ACT_LOOP_STATE;	}
	virtual BOOL	CanTransite(Object* pOwner, DWORD CurTime, StateBase* pToTransiteState);

	virtual STATECHANGE::TYPE	OnStart(Object* pOwner, DWORD CurTime);
	virtual STATEPROCESS::TYPE	OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick);
	virtual STATECHANGE::TYPE	OnEnd(Object* pOwner, DWORD CurTime);
};
