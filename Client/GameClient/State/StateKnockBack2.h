/**

DAMAGE ����

\SA STATE::DAMAGE

@author
-����ȯ
*/
#pragma once
class StateBase;

// �⺻ �˹� 
class StateKnockBack
	: public StateBase
{
	StateKnockBack(void);
public:
	~StateKnockBack(void);
	__forceinline static StateKnockBack* GetInstance()	{	static StateKnockBack d;	return &d;	};

	virtual int		GetStateKind() {	return STATE::KNOCKBACK;	}
	virtual BOOL	CanTransite(Object* pOwner, DWORD CurTime, StateBase* pToTransiteState) { return TRUE; }

	virtual STATECHANGE::TYPE	OnStart(Object* pOwner, DWORD CurTime);
	virtual STATEPROCESS::TYPE	OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick);
};

// ���ư� �κη����� �˹�
class StateKnockBack2 :
	public StateBase
{

	StateKnockBack2(void);
public:
	~StateKnockBack2(void);
	__forceinline static StateKnockBack2* GetInstance()	{	static StateKnockBack2 d;	return &d;	};

	virtual int		GetStateKind() {	return STATE::KNOCKBACK2;	}
	virtual BOOL	CanTransite(Object* pOwner, DWORD CurTime, StateBase* pToTransiteState) { return TRUE; }

	virtual STATECHANGE::TYPE	OnStart(Object* pOwner, DWORD CurTime);
	virtual STATEPROCESS::TYPE	OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick);

};

// Ǯ��
class StateKnockBack3 :
	public StateBase
{

	StateKnockBack3(void);
public:
	~StateKnockBack3(void);
	__forceinline static StateKnockBack3* GetInstance()	{	static StateKnockBack3 d;	return &d;	};

	virtual int		GetStateKind() {	return STATE::KNOCKBACK3;	}
	virtual BOOL	CanTransite(Object* pOwner, DWORD CurTime, StateBase* pToTransiteState);

	virtual STATECHANGE::TYPE	OnStart(Object* pOwner, DWORD CurTime);
	virtual STATEPROCESS::TYPE	OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick);

	BOOL m_IsBound;
};
