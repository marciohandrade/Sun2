#pragma once

/**

    DAMAGE 상태
    \SA STATE::NPC_SWING_STATE

    @author
		유재영

    @since

    @remarks 
*/

class StateBase;

class StateNPCResponse : public StateBase
{
public:
	StateNPCResponse(void);
	virtual ~StateNPCResponse(void);

	__forceinline static StateNPCResponse * GetInstance()	{	static StateNPCResponse d;	return &d;	};

	virtual int		GetStateKind() {	return STATE::NPC_RESPONSE_STATE;	}
	virtual BOOL	CanTransite(Object* pOwner, DWORD CurTime, StateBase* pToTransiteState);

	virtual STATECHANGE::TYPE	OnStart(Object* pOwner, DWORD CurTime);
	virtual STATEPROCESS::TYPE	OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick);
	virtual STATECHANGE::TYPE	OnEnd(Object* pOwner, DWORD CurTime);
};
