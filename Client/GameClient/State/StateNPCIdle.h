/**

    DAMAGE 상태
    \SA STATE::NPC_SWING_STATE

    @author
		유재영

    @since

    @remarks 
*/


#pragma once
#include "statebase.h"

class StateNPCIdle :
	public StateBase
{
public:
	StateNPCIdle(void);
	virtual ~StateNPCIdle(void);

	__forceinline static StateNPCIdle * GetInstance()	{	static StateNPCIdle d;	return &d;	};

	virtual int		GetStateKind() {	return STATE::NPC_SWING_STATE;	}
	virtual BOOL	CanTransite(Object* pOwner, DWORD CurTime, StateBase* pToTransiteState);

	virtual STATECHANGE::TYPE	OnStart(Object* pOwner, DWORD CurTime);
	virtual STATEPROCESS::TYPE	OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick);
	virtual STATECHANGE::TYPE	OnEnd(Object* pOwner, DWORD CurTime);
};
