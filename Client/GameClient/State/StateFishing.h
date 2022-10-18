#pragma once

#ifdef _YJ_FISHING_SYSTEM_000933

class StateBase;

//////////////////////////////////////////////////////////////////////////
class StateFishing_Throw : public StateBase
{
public:
	StateFishing_Throw(void);
	virtual ~StateFishing_Throw(void);

	__forceinline static StateFishing_Throw * GetInstance()	{	static StateFishing_Throw d;	return &d;	};

	virtual int		GetStateKind() {	return STATE::FISHING_THROW;	}
	virtual BOOL	CanTransite(Object* pOwner, DWORD CurTime, StateBase* pToTransiteState);

	virtual STATECHANGE::TYPE	OnStart(Object* pOwner, DWORD CurTime);
	virtual STATEPROCESS::TYPE	OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick);
	virtual STATECHANGE::TYPE	OnEnd(Object* pOwner, DWORD CurTime);
};
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
class StateFishing_Stand : public StateBase
{
public:
	StateFishing_Stand(void);
	virtual ~StateFishing_Stand(void);

	__forceinline static StateFishing_Stand * GetInstance()	{	static StateFishing_Stand d;	return &d;	};

	virtual int		GetStateKind() {	return STATE::FISHING_STAND;	}
	virtual BOOL	CanTransite(Object* pOwner, DWORD CurTime, StateBase* pToTransiteState);

	virtual STATECHANGE::TYPE	OnStart(Object* pOwner, DWORD CurTime);
	virtual STATEPROCESS::TYPE	OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick);
	virtual STATECHANGE::TYPE	OnEnd(Object* pOwner, DWORD CurTime);
};
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
class StateFishing_Bite : public StateBase
{
public:
	StateFishing_Bite(void);
	virtual ~StateFishing_Bite(void);

	__forceinline static StateFishing_Bite * GetInstance()	{	static StateFishing_Bite d;	return &d;	};

	virtual int		GetStateKind() {	return STATE::FISHING_BITE;	}
	virtual BOOL	CanTransite(Object* pOwner, DWORD CurTime, StateBase* pToTransiteState);

	virtual STATECHANGE::TYPE	OnStart(Object* pOwner, DWORD CurTime);
	virtual STATEPROCESS::TYPE	OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick);
	virtual STATECHANGE::TYPE	OnEnd(Object* pOwner, DWORD CurTime);
};
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
class StateFishing_Success : public StateBase
{
public:
	StateFishing_Success(void);
	virtual ~StateFishing_Success(void);

	__forceinline static StateFishing_Success * GetInstance()	{	static StateFishing_Success d;	return &d;	};

	virtual int		GetStateKind() {	return STATE::FISHING_SUCCESS;	}
	virtual BOOL	CanTransite(Object* pOwner, DWORD CurTime, StateBase* pToTransiteState);

	virtual STATECHANGE::TYPE	OnStart(Object* pOwner, DWORD CurTime);
	virtual STATEPROCESS::TYPE	OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick);
	virtual STATECHANGE::TYPE	OnEnd(Object* pOwner, DWORD CurTime);
};
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
class StateFishing_Failed : public StateBase
{
public:
	StateFishing_Failed(void);
	virtual ~StateFishing_Failed(void);

	__forceinline static StateFishing_Failed * GetInstance()	{	static StateFishing_Failed d;	return &d;	};

	virtual int		GetStateKind() {	return STATE::FISHING_FAILED;	}
	virtual BOOL	CanTransite(Object* pOwner, DWORD CurTime, StateBase* pToTransiteState);

	virtual STATECHANGE::TYPE	OnStart(Object* pOwner, DWORD CurTime);
	virtual STATEPROCESS::TYPE	OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick);
	virtual STATECHANGE::TYPE	OnEnd(Object* pOwner, DWORD CurTime);
};
//////////////////////////////////////////////////////////////////////////

#endif //_YJ_FISHING_SYSTEM_000933