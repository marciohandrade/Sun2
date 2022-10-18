/**

DAMAGE 상태
\SA STATE::IDLE

@author
-유재영

@since

@remarks
- 
*/

#pragma once

#include "statebase.h"

class StateBase;
class StateOwner;

class StateMapIdle : public StateBase
{
	StateMapIdle(void);

public:
	virtual ~StateMapIdle(void);

	__forceinline static StateMapIdle * GetInstance()	{	static StateMapIdle d;	return &d;	};

	virtual int		GetStateKind() {	return STATE::IDLE;	}
	virtual BOOL	CanTransite(Object* pOwner, DWORD CurTime, StateBase* pToTransiteState);

	virtual STATECHANGE::TYPE	OnStart(Object* pOwner, DWORD CurTime);
	virtual STATEPROCESS::TYPE	OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick);
	virtual STATECHANGE::TYPE	OnEnd(Object* pOwner, DWORD CurTime);
};
