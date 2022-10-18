/**

    Character�� �̵����¸� ��Ÿ����

    @author
        -������

    @since

    @remarks
        -2005.06.08 �ҽ� ����(����)
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

	//!�̵����ϸ��̼� ����
	/*!
	���� �̵���� ������ StateHeroMove �� BattleScene::OnRecv_Sync���� ���ش�.
	<B>���� �̵��ӵ��� ���ϸ��̼� ������ ����� �Ǿ����� ����</B>
	\SA STATE::MOVE, StateHeroMove
	*/
	virtual STATECHANGE::TYPE	OnStart(Object* pOwner, DWORD CurTime);

	//!���ϸ��̼� �÷��� �� �̵��� �����Ų��.
	/*!
	���� �̵���� ������ StateHeroMove �� BattleScene::OnRecv_Sync���� ���ش�.
	*/
	virtual STATEPROCESS::TYPE	OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick);
	virtual STATECHANGE::TYPE	OnEnd(Object* pOwner, DWORD CurTime);
};
