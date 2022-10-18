/**

DAMAGE 상태
\SA STATE::FALL_OFF

@author
-김지완

@since

@remarks
- 
*/

#pragma once

class StateBase;

class StateFallOff :
	public StateBase
{
	StateFallOff(void);

public:
	virtual ~StateFallOff(void);
	__forceinline static StateFallOff* GetInstance()	{	static StateFallOff d;	return &d;	};
	
	virtual int		GetStateKind() {	return STATE::FALL_OFF;	}
	virtual BOOL	CanTransite(Object* pOwner, DWORD CurTime, StateBase* pToTransiteState);

	virtual STATECHANGE::TYPE	OnStart(Object* pOwner, DWORD CurTime);
	virtual STATEPROCESS::TYPE	OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick);
	virtual STATECHANGE::TYPE	OnEnd(Object* pOwner, DWORD CurTime);
};
