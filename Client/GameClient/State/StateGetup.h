/**

    DAMAGE ����

    \SA STATE::DAMAGE

    @author
        -������

    @since

    @remarks
        -2005.06.08 �ҽ� ����(����)
        - 
*/

#pragma once

class StateBase;


class StateGetup :
	public StateBase
{
	StateGetup(void);
public:
	~StateGetup(void);
	__forceinline static StateGetup* GetInstance()	{	static StateGetup d;	return &d;	};

	virtual int		GetStateKind() {	return STATE::GETUP;	}
	virtual BOOL	CanTransite(Object* pOwner, DWORD CurTime, StateBase* pToTransiteState);

	virtual STATECHANGE::TYPE	OnStart(Object* pOwner, DWORD CurTime);
	virtual STATEPROCESS::TYPE	OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick);
	virtual STATECHANGE::TYPE	OnEnd(Object* pOwner, DWORD CurTime);
};
