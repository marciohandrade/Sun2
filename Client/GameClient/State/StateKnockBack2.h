/**

DAMAGE 상태

\SA STATE::DAMAGE

@author
-전보환
*/
#pragma once
class StateBase;

// 기본 넉백 
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

// 날아가 널부러지는 넉백
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

// 풀링
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
