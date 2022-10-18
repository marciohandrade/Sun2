/**

    Hero�� ��쿡�� StateMove��� �� Ŭ������ ���

    @author
        -������

    @since

    @remarks
        -2005.06.08 �ҽ� ����(����)
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

	//!Hero::GetTargetPosition�� �����ؼ� GetPath�� ���� �̵����(��ã��)�� ã�Ƴ���.
	virtual STATECHANGE::TYPE	OnStart(Object* pOwner, DWORD CurTime);
	virtual STATEPROCESS::TYPE	OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick);
	virtual STATECHANGE::TYPE	OnEnd(Object* pOwner, DWORD CurTime);
};

