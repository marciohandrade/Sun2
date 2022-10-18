/**

    몬스터가 특정위치로 빠르게 도망가는 상태를 나타낸다

    @author
        -유재영

    @since

    @remarks
        - 
*/
#pragma once

class StateBase;

class StateRunAway :
	public StateBase
{
protected:
	StateRunAway(void);
public:
	~StateRunAway(void);
	__forceinline static StateRunAway* GetInstance()	{	static StateRunAway d;	return &d;	};

	virtual int		GetStateKind() {	return STATE::RUNAWAY;	}
	virtual BOOL	CanTransite(Object* pOwner, DWORD CurTime, StateBase* pToTransiteState);

	//!이동에니메이션 셋팅
	/*!
	실제 이동경로 셋팅은 StateHeroMove 나 BattleScene::OnRecv_Sync에서 해준다.
	<B>아직 이동속도나 에니메이션 셋팅이 제대로 되어있지 않음</B>
	\SA STATE::MOVE, StateHeroMove
	*/
	virtual STATECHANGE::TYPE	OnStart(Object* pOwner, DWORD CurTime);

	//!에니메이션 플레이 및 이동을 진행시킨다.
	/*!
	실제 이동경로 셋팅은 StateHeroMove 나 BattleScene::OnRecv_Sync에서 해준다.
	*/
	virtual STATEPROCESS::TYPE	OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick);
	virtual STATECHANGE::TYPE	OnEnd(Object* pOwner, DWORD CurTime);
};

