
#pragma once


enum eCollectState
{
	eCollectState_Start = 0,
	eCollectState_Loop  = 1,
	eCollectState_End   = 2,
	eCollectState_Max,
};

class StateBase;
class StateCollect :
	public StateBase
{
	StateCollect(void);
public:
	~StateCollect(void);
	__forceinline static StateCollect* GetInstance()	{	static StateCollect d;	return &d;	};

	virtual int		GetStateKind() {	return STATE::IDLE;	}
	virtual BOOL	CanTransite(Object* pOwner, DWORD CurTime, StateBase* pToTransiteState);

	virtual STATECHANGE::TYPE	OnStart(Object* pOwner, DWORD CurTime);
	virtual STATEPROCESS::TYPE	OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick);
	virtual STATECHANGE::TYPE	OnEnd(Object* pOwner, DWORD CurTime);

    static void SendCollectActionSyn(DWORD object_key, WzVector player_position);
    static void SendCollectSuccessSyn();
    static void SendCollectCancelSyn(TCHAR* debug_message);

protected:
	WzID		   m_wiAniCode[eCollectState_Max];
	WzID		   m_wiSucEffectCode;
	WzID		   m_wiStartEffectCode;	
	bool			m_bSuccess;
	bool			m_bMouseCancel;
	bool			m_bStampMode;

    static bool is_send_action_syn_;
};

