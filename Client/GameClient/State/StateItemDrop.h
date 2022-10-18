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

#include "StateBase.h"

class StateBase;

class StateItemDrop : public StateBase
{
	StateItemDrop(void);
public:
	virtual ~StateItemDrop(void);

	__forceinline static StateItemDrop * GetInstance()	{	static StateItemDrop d;	return &d;	};

	virtual int		GetStateKind() {	return STATE::ITEM_DROP_STATE;	}
	virtual BOOL	CanTransite(Object* pOwner, DWORD CurTime, StateBase* pToTransiteState);

	virtual STATECHANGE::TYPE	OnStart(Object* pOwner, DWORD CurTime);
	virtual STATEPROCESS::TYPE	OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick);
	virtual STATECHANGE::TYPE	OnEnd(Object* pOwner, DWORD CurTime);
};
