/**


\SA STATE::EARTH_QUAKE

@author
-±èÁö¿Ï

@since

@remarks
- 
*/

#pragma once

class StateBase;

class StateFallDown :
	public StateBase
{
	StateFallDown(void);

public:
	virtual ~StateFallDown(void);
	__forceinline static StateFallDown* GetInstance()	{	static StateFallDown d;	return &d;	};
	
	virtual int		GetStateKind() {	return STATE::FALL_DOWN;	}
	virtual BOOL	CanTransite(Object* pOwner, DWORD CurTime, StateBase* pToTransiteState);

	virtual STATECHANGE::TYPE	OnStart(Object* pOwner, DWORD CurTime);
	virtual STATEPROCESS::TYPE	OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick);
	virtual STATECHANGE::TYPE	OnEnd(Object* pOwner, DWORD CurTime);
};
