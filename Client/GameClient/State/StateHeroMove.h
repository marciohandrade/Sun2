/**

    Hero일 경우에는 StateMove대신 이 클래스를 사용

    @author
        -박준태

    @since

    @remarks
        -2005.06.08 소스 정리(지완)
        - 
*/
#pragma once

class StateBase;
class StateMove;

int LineCross(Wz2DLine Line1,Wz2DLine Line2);
int InfiniteLineCross(Wz2DLine InfLine,Wz2DLine Line);

class StateHeroMove :
	public StateMove
{
	StateHeroMove(void);
public:
	~StateHeroMove(void);
	__forceinline static StateHeroMove* GetInstance()	{	static StateHeroMove d;	return &d;	};

	virtual int		GetStateKind() {	return STATE::MOVE;	}
	virtual BOOL	CanTransite(Object* pOwner, DWORD CurTime, StateBase* pToTransiteState);

	//!Hero::GetTargetPosition을 참조해서 GetPath를 통해 이동경로(길찾기)를 찾아낸다.
	virtual STATECHANGE::TYPE	OnStart(Object* pOwner, DWORD CurTime);
	virtual STATEPROCESS::TYPE	OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick);
	virtual STATECHANGE::TYPE	OnEnd(Object* pOwner, DWORD CurTime);
};

