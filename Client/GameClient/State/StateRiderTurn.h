
#pragma once

class StateBase;
class StateRiderTurn:	public StateBase
{
	StateRiderTurn(void);
public:
	~StateRiderTurn(void);
	__forceinline static StateRiderTurn* GetInstance()	{	static StateRiderTurn d;	return &d;	};

	virtual int		GetStateKind() {	return STATE::IDLE;	}
	virtual BOOL	CanTransite(Object* pOwner, DWORD CurTime, StateBase* pToTransiteState);

	virtual STATECHANGE::TYPE	OnStart(Object* pOwner, DWORD CurTime);
	virtual STATEPROCESS::TYPE	OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick);
	virtual STATECHANGE::TYPE	OnEnd(Object* pOwner, DWORD CurTime);

private:
	void	CheckTurnDirection(Player *pPlayer);

private:
	int		    m_nTurnState;
    float		m_fTurnSpeed;
};

