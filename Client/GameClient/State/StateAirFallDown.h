#pragma once

class StateBase;

class StateAirFallDown : public StateBase
{
	StateAirFallDown( void );
public:
	virtual ~StateAirFallDown( void );

	static StateAirFallDown * GetInstance()	{ static StateAirFallDown d; return &d; }
	
	virtual int		GetStateKind() { return STATE::AIR_FALL_DOWN; }
	virtual BOOL	CanTransite(Object* pOwner, DWORD CurTime, StateBase* pToTransiteState);

	virtual STATECHANGE::TYPE	OnStart(Object* pOwner, DWORD CurTime);
	virtual STATEPROCESS::TYPE	OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick);
	virtual STATECHANGE::TYPE	OnEnd(Object* pOwner, DWORD CurTime);

};
