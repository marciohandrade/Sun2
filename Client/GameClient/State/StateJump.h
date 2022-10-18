/**

    ���� ����
    \SA STATE::JUMP

    @author
        -������

    @since

    @remarks
        -2005.06.08 �ҽ� ����(����)
        - 
*/

#pragma once

class StateBase;

class StateJump :
	public StateBase
{
	StateJump(void);
public:
	~StateJump(void);
	__forceinline static StateJump* GetInstance()	{	static StateJump d;	return &d;	};

	virtual int		GetStateKind() {	return STATE::JUMP;	}
	virtual BOOL	CanTransite(Object* pOwner, DWORD CurTime, StateBase* pToTransiteState);

	virtual STATECHANGE::TYPE	OnStart(Object* pOwner, DWORD CurTime);
	virtual STATEPROCESS::TYPE	OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick);
	virtual STATECHANGE::TYPE	OnEnd(Object* pOwner, DWORD CurTime);
};
