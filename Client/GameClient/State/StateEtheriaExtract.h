#pragma once

class StateBase;

enum eExtract
{
    kExtart_Start = 0,
    kExtart_Loop,
    kExtart_End,
};

class StateBase;
class StateEtheriaExtract:	public StateBase
{
    StateEtheriaExtract(void);
public:
    ~StateEtheriaExtract(void);
    __forceinline static StateEtheriaExtract* GetInstance()	{	static StateEtheriaExtract d;	return &d;	};

    virtual int		GetStateKind() {	return STATE::IDLE;	}
    virtual BOOL	CanTransite(Object* pOwner, DWORD CurTime, StateBase* pToTransiteState);

    virtual STATECHANGE::TYPE	OnStart(Object* pOwner, DWORD CurTime);
    virtual STATEPROCESS::TYPE	OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick);
    virtual STATECHANGE::TYPE	OnEnd(Object* pOwner, DWORD CurTime);
private:
    BOOL  m_bOldLockState;

};
