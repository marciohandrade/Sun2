/**

    ���� ����
    \SA STATE::IDLE

    @author
        -������

    @since

    @remarks
        -2005.06.08 �ҽ� ����(����)
        - 
*/

#pragma once

class StateBase;

class StateIdle :
	public StateBase
{
	StateIdle(void);
public:
	~StateIdle(void);
	__forceinline static StateIdle* GetInstance()	{	static StateIdle d;	return &d;	};

	virtual int		GetStateKind() {	return STATE::IDLE;	}
	virtual BOOL	CanTransite(Object* pOwner, DWORD CurTime, StateBase* pToTransiteState);

	virtual STATECHANGE::TYPE	OnStart(Object* pOwner, DWORD CurTime);
	virtual STATEPROCESS::TYPE	OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick);
	virtual STATECHANGE::TYPE	OnEnd(Object* pOwner, DWORD CurTime);

	void    SetIdleAnimDelay(Object *pObj,float fAnim);
};
