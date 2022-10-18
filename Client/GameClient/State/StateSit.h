/**

    평상시 상태
    \SA STATE::SIT

    @author
        -김지완

    @since

    @remarks
*/

#pragma once

class StateBase;

class StateSit :
	public StateBase
{
	StateSit(void);
public:
	~StateSit(void);
	__forceinline static StateSit* GetInstance()	{	static StateSit d;	return &d;	};

	virtual int		GetStateKind() {	return STATE::IDLE;	}
	virtual BOOL	CanTransite(Object* pOwner, DWORD CurTime, StateBase* pToTransiteState);

	virtual STATECHANGE::TYPE	OnStart(Object* pOwner, DWORD CurTime);
	virtual STATEPROCESS::TYPE	OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick);
	virtual STATECHANGE::TYPE	OnEnd(Object* pOwner, DWORD CurTime);

#ifdef _NA_008350_20150617_SPA_NEW_STYLE_MODE_ANIMATION
    STATEPROCESS::TYPE	ProcessSpaSitAnimation(Player* pPlayer);
#endif //_NA_008350_20150617_SPA_NEW_STYLE_MODE_ANIMATION

protected:
	BOOL m_bSitting;
};
