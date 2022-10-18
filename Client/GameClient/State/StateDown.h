/**

    DAMAGE 상태

    \SA STATE::DAMAGE

    @author
        -박준태

    @since

    @remarks
        -2005.06.08 소스 정리(지완)
        - 
*/

#pragma once

class StateBase;

class StateDown :
	public StateBase
{
	StateDown(void);
public:
	~StateDown(void);
	__forceinline static StateDown* GetInstance()	{	static StateDown d;	return &d;	};

	virtual int		GetStateKind() {	return STATE::DOWN;	}
	virtual BOOL	CanTransite(Object* pOwner, DWORD CurTime, StateBase* pToTransiteState);

	virtual STATECHANGE::TYPE	OnStart(Object* pOwner, DWORD CurTime);
	virtual STATEPROCESS::TYPE	OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick);
	virtual STATECHANGE::TYPE	OnEnd(Object* pOwner, DWORD CurTime);

#ifdef _NA_008472_20150903_IMPROVED_BALANCE
private:
	eCHAR_STATE_TYPE state_type_;
#endif // _NA_008472_20150903_IMPROVED_BALANCE
};
