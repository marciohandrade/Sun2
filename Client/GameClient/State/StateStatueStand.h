#pragma once

#ifdef _NA_008016_20150203_SUN_RANKING_SYSTEM_STATUE

class StateBase;
class StateStatueStand : public StateBase
{
	StateStatueStand(void);
public:
	~StateStatueStand(void);
	__forceinline static StateStatueStand* GetInstance() { static StateStatueStand d; return &d; };

	virtual int GetStateKind() { return STATE::STATUE_STAND; }
	virtual BOOL CanTransite(Object* owner_ptr, DWORD cur_time, StateBase* to_transite_state_ptr);

	virtual STATECHANGE::TYPE OnStart(Object* owner_ptr, DWORD cur_time);
	virtual STATEPROCESS::TYPE OnProcess(Object* owner_ptr, DWORD cur_time, DWORD tick);
	virtual STATECHANGE::TYPE OnEnd(Object* owner_ptr, DWORD cur_time);
};
#endif // _NA_008016_20150203_SUN_RANKING_SYSTEM_STATUE
