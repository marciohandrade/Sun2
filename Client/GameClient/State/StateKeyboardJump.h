/**

    키보드 점프 

    @author
        -

    @since

    @remarks
        -2005.06.08 테스트용 작업(지완)
        -2006.06.19 점프 리펙토링 시작(지완)

    @Todo
		-충돌 메쉬 만들어서 유닛에 레이쏴서 충돌 체크 할것.
		(엔진에서 지원되어야 하며 현재 맵 오브젝트 충돌 체크는 바운딩 박스로 정확하지 않은 충돌 처리를 하고 있음)

		-목적지와 출발지가 점프 타일일 경우 건너편으로 무조건 뛸수 있게(점프를 해서 꼭 가야 하는 특수 지형 처리) 
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

