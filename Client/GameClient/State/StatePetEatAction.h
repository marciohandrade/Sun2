#pragma once


class StateBase;

class StatePetEatAction : public StateBase
{
public:
	StatePetEatAction(void);
	virtual ~StatePetEatAction(void);

	__forceinline static StatePetEatAction * GetInstance()	{	static StatePetEatAction d;	return &d;	};

	virtual int		GetStateKind() {	return STATE::PET_EAT_ACTION_STATE;	}
	virtual BOOL	CanTransite(Object* pOwner, DWORD CurTime, StateBase* pToTransiteState);

	virtual STATECHANGE::TYPE	OnStart(Object* pOwner, DWORD CurTime);
	virtual STATEPROCESS::TYPE	OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick);
	virtual STATECHANGE::TYPE	OnEnd(Object* pOwner, DWORD CurTime);
};

