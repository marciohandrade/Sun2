/**

    공격 점프 상태

    \SA STATE::ATTACKJUMP

    @author
        -유재영

    @since
        -2006. 6. 26.

    @remarks
        - 
*/
#pragma once


class StateBase;

class StateAttackJump :
    public StateBase
{
    StateAttackJump(void);
public:
    ~StateAttackJump(void);
    __forceinline static StateAttackJump* GetInstance()	{	static StateAttackJump d;	return &d;	};

    virtual int		GetStateKind() {	return STATE::ATTACKJUMP;	}
    virtual BOOL	CanTransite(Object* pOwner, DWORD CurTime, StateBase* pToTransiteState);

    virtual STATECHANGE::TYPE	OnStart(Object* pOwner, DWORD CurTime);
    virtual STATEPROCESS::TYPE	OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick);
    virtual STATECHANGE::TYPE	OnEnd(Object* pOwner, DWORD CurTime);

private:
};



