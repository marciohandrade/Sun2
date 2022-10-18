/**

    DAMAGE ����
    \SA STATE::DAMAGE

    @author
       -������

    @since

    @remarks
        -2005.06.07 �ҽ� ����(����)
        - 
*/

#pragma once
class StateBase;

class StateAir : public StateBase
{
	StateAir(void);
public:
	~StateAir(void);

	__forceinline static StateAir*	GetInstance(){	static StateAir d;	return &d;	};

	virtual int			GetStateKind() {	return STATE::DAMAGE;	}
	virtual BOOL		CanTransite(Object* pOwner, DWORD CurTime, StateBase* pToTransiteState);
	
	virtual STATECHANGE::TYPE	OnStart(Object* pOwner, DWORD CurTime);
	virtual STATEPROCESS::TYPE	OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick);
	virtual STATECHANGE::TYPE	OnEnd(Object* pOwner, DWORD CurTime);
};
