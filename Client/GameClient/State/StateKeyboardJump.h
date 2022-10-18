/**

    Ű���� ���� 

    @author
        -

    @since

    @remarks
        -2005.06.08 �׽�Ʈ�� �۾�(����)
        -2006.06.19 ���� �����丵 ����(����)

    @Todo
		-�浹 �޽� ���� ���ֿ� ���̽��� �浹 üũ �Ұ�.
		(�������� �����Ǿ�� �ϸ� ���� �� ������Ʈ �浹 üũ�� �ٿ�� �ڽ��� ��Ȯ���� ���� �浹 ó���� �ϰ� ����)

		-�������� ������� ���� Ÿ���� ��� �ǳ������� ������ �ۼ� �ְ�(������ �ؼ� �� ���� �ϴ� Ư�� ���� ó��) 
*/
#pragma once

#ifdef _NA_20120220_JUMP_RENEWAL
#include "StateKeyboardJump2.h"
#else

class StateBase;

class StateKeyboardJump : public StateBase
{
	StateKeyboardJump(void);
public:
	~StateKeyboardJump(void);
	__forceinline static StateKeyboardJump* GetInstance()	{	static StateKeyboardJump d;	return &d;	};

    static bool GetJumpDirection(OUT WzVector& direction, int flag, WzVector& foward);
    //! 
    static bool GetJumpDestPosByDirection(OUT WzVector& dest_pos, 
                                            WzVector& scr_pos, 
                                            WzVector& direction, 
                                            float player_height,
                                            float min_dist, 
                                            float max_dist);

    static BOOL    CheckDest();

    static int      CheckJumpTile(WzVector vSrc,WzVector vDest);

	virtual int		GetStateKind() {	return STATE::MOVE;	}
	virtual BOOL	CanTransite(Object* pOwner, DWORD CurTime, StateBase* pToTransiteState);
	
	virtual STATECHANGE::TYPE	OnStart(Object* pOwner, DWORD CurTime);
	virtual STATEPROCESS::TYPE	OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick);
	virtual STATECHANGE::TYPE	OnEnd(Object* pOwner, DWORD CurTime);

protected:
    void SetMotionBlur(Object* object, bool flag);
	void	OnFootStep(Object* pOwner);

	void    CalculateTurnAccelerate(DWORD dwTick);	
	BOOL    FindDest();
	BOOL    FindStand();
	BOOL    SendJumpPacket();
	BOOL    SendStopPacket();
	BOOL    CalCulateJump();
	BOOL    IsIntersect();
    BOOL    CheckRidingState(Player *pPlayer);
};
#endif

