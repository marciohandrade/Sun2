/**

DAMAGE 상태
\SA STATE::TRIGGER_STATE

@author
-유재영

@since

@remarks
- 
*/

#pragma once

class StateBase;

class StateTrigger :
	public StateBase
{
	StateTrigger(void);

public:
	virtual ~StateTrigger(void);
	__forceinline static StateTrigger* GetInstance()	{	static StateTrigger d;	return &d;	};
	
	virtual int		GetStateKind() {	return STATE::TRIGGER_STATE;	}
	virtual BOOL	CanTransite(Object* pOwner, DWORD CurTime, StateBase* pToTransiteState);

	virtual STATECHANGE::TYPE	OnStart(Object* pOwner, DWORD CurTime);
	virtual STATEPROCESS::TYPE	OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick);
	virtual STATECHANGE::TYPE	OnEnd(Object* pOwner, DWORD CurTime);
};
