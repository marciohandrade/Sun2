/**

    Ű����� �����϶� 

    @author
        -������

    @since

    @remarks
        -2005.06.08 �ҽ� ����(����)
        -2006.11.6  Ű���� �̵� �����丵 �� ����ȭ ���� ����(����) 
*/
#pragma once

class StateBase;


int LineCross(Wz2DLine Line1,Wz2DLine Line2);
int InfiniteLineCross(Wz2DLine InfLine,Wz2DLine Line);


class StateKeyboardMove : public StateBase
{
	StateKeyboardMove(void);
public:
	~StateKeyboardMove(void);
	__forceinline static StateKeyboardMove* GetInstance()	{	static StateKeyboardMove d;	return &d;	};

	virtual int		GetStateKind() {	return STATE::MOVE;	}
	virtual BOOL	CanTransite(Object* pOwner, DWORD CurTime, StateBase* pToTransiteState);
	
	virtual STATECHANGE::TYPE	OnStart(Object* pOwner, DWORD CurTime);
	virtual STATEPROCESS::TYPE	OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick);
	virtual STATECHANGE::TYPE	OnEnd(Object* pOwner, DWORD CurTime);

protected:
    bool    SEND_CG_SYNC_KBMOVE_SYN();
    BOOL    SendKeyboardMovePacket(Player* pPlayer, DWORD dwTick);
	BOOL    CheckActionQueue(Player* pPlayer);
	void    SetHeroKeyboardMoveValue();
	void    SetKeyboardMoveValue(Player* pPlayer);
	void    CheckHeroCameraRotateOption();

	void    CalculateTurnAccelerate(Player* pPlayer,DWORD dwTick);
	BOOL    CalculateValue(Player* pPlayer, 
                            DWORD dwTick, 
                            OUT WzID& wzGetMoveAnimation, 
                            OUT WzVector& vGetDirection,
                            OUT ePLAYER_ANI& eAni);

	void    EnterProcess(Player* pPlayer, DWORD CurTime,DWORD dwTick);
	void    LeaveProcess(Player* pPlayer, DWORD CurTime,DWORD dwTick);
	BOOL    ThrustCanBeMove(Player* pPlayer,DWORD dwTick,WzVector oldPos,int iOldTile);
};

