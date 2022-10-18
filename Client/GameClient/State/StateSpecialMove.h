/**

    구르기 ,슬라이딩등의 특수 이동 

    @author
        -김지완

    @since

    @remarks
*/
#pragma once

class StateBase;

class StateSpecialMove : public StateBase
{
	StateSpecialMove(void);
public:
	~StateSpecialMove(void);
	__forceinline static StateSpecialMove* GetInstance()	{	static StateSpecialMove d;	return &d;	};

	virtual int		GetStateKind() {	return STATE::SPECIALMOVE;	}
	virtual BOOL	CanTransite(Object* pOwner, DWORD CurTime, StateBase* pToTransiteState);
	
	virtual STATECHANGE::TYPE	OnStart(Object* pOwner, DWORD CurTime);
	virtual STATEPROCESS::TYPE	OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick);
	virtual STATECHANGE::TYPE	OnEnd(Object* pOwner, DWORD CurTime);

private:
	void	OnProcessEffect(Object* pOwner, DWORD CurTime,DWORD dwTick);

protected:

	
	int    m_iDustTime;
	DWORD  m_dwCopyTime;
	BOOL   m_bCreateEffect;
};

