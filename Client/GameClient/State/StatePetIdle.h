#pragma once


class StateBase;

class StatePetIdle : public StateBase
{
public:
	StatePetIdle(void);
	virtual ~StatePetIdle(void);

	__forceinline static StatePetIdle * GetInstance()	{	static StatePetIdle d;	return &d;	};

	virtual int		GetStateKind() {	return STATE::PET_EAT_ACTION_STATE;	}
	virtual BOOL	CanTransite(Object* pOwner, DWORD CurTime, StateBase* pToTransiteState);

	virtual STATECHANGE::TYPE	OnStart(Object* pOwner, DWORD CurTime);
	virtual STATEPROCESS::TYPE	OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick);
	virtual STATECHANGE::TYPE	OnEnd(Object* pOwner, DWORD CurTime);
};

