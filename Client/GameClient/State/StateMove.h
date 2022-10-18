/**

    Character의 이동상태를 나타낸다

    @author
        -박준태

    @since

    @remarks
        -2005.06.08 소스 정리(지완)
        - 
*/
#pragma once

class StateBase;

class StateMouseMove
{
protected:
    StateMouseMove() {}

    float GetMoveLength( Character* pCharacter, DWORD dwTick );
};

class StateMove :
	public StateBase
    , public StateMouseMove
    
{
protected:
	StateMove(void);
public:
	~StateMove(void);
	__forceinline static StateMove* GetInstance()	{	static StateMove d;	return &d;	};

	virtual int		GetStateKind() {	return STATE::MOVE;	}
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
