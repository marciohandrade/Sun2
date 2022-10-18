
#pragma once

class StateBase;
class StateSummonRider:	public StateBase
{
	StateSummonRider(void);
public:
	~StateSummonRider(void);
	__forceinline static StateSummonRider* GetInstance()	{	static StateSummonRider d;	return &d;	};

	virtual int		GetStateKind() {	return STATE::IDLE;	}
	virtual BOOL	CanTransite(Object* pOwner, DWORD CurTime, StateBase* pToTransiteState);

	virtual STATECHANGE::TYPE	OnStart(Object* pOwner, DWORD CurTime);
	virtual STATEPROCESS::TYPE	OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick);
	virtual STATECHANGE::TYPE	OnEnd(Object* pOwner, DWORD CurTime);
private:
	DWORD m_dwStartTime;
	DWORD m_dwMaxTime;
    BOOL  m_bOldLockState;
    BOOL  m_bWaitResult;
};

