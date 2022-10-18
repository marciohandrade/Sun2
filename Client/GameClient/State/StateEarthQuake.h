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

class StateEarthQuake :
	public StateBase
{
	StateEarthQuake(void);

public:
	virtual ~StateEarthQuake(void);
	__forceinline static StateEarthQuake* GetInstance()	{	static StateEarthQuake d;	return &d;	};
	
	virtual int		GetStateKind() {	return STATE::EARTH_QUAKE;	}
	virtual BOOL	CanTransite(Object* pOwner, DWORD CurTime, StateBase* pToTransiteState);

	virtual STATECHANGE::TYPE	OnStart(Object* pOwner, DWORD CurTime);
	virtual STATEPROCESS::TYPE	OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick);
	virtual STATECHANGE::TYPE	OnEnd(Object* pOwner, DWORD CurTime);
};
