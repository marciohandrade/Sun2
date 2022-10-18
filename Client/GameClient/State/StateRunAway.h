/**

    ���Ͱ� Ư����ġ�� ������ �������� ���¸� ��Ÿ����

    @author
        -���翵

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

