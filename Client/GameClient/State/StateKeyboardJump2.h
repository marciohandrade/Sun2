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

class StateBase;

class StateKeyboardJump : public StateBase
{
	StateKeyboardJump(void);
public:
	~StateKeyboardJump(void);
	__forceinline static StateKeyboardJump* GetInstance()	{	static StateKeyboardJump d;	return &d;	};

    static BOOL GetJumpDirection(OUT WzVector& direction, int flag, WzVector& foward);
    //! 
    static bool GetJumpDestPosByDirection(OUT WzVector& dest_pos, WzVector& scr_pos, WzVector& direction, float player_height, float min_dist, float max_dist);

   static BOOL    CheckJumpTile(WzVector vSrc,WzVector vDest);

   static BOOL SimulateJump( const WzVector& StartPosition, const WzVector& Direction, OUT WzVector& ResultPosition, OUT std::vector<WzVector>& SimulatedPositionArray );

	virtual int		GetStateKind() {	return STATE::MOVE;	}
	virtual BOOL	CanTransite(Object* pOwner, DWORD CurTime, StateBase* pToTransiteState);
	
	virtual STATECHANGE::TYPE	OnStart(Object* pOwner, DWORD CurTime);
	virtual STATEPROCESS::TYPE	OnProcess(Object* pOwner, DWORD CurTime,DWORD dwTick);
	virtual STATECHANGE::TYPE	OnEnd(Object* pOwner, DWORD CurTime);

protected:
    void    SetMotionBlur(Object* object, bool flag);
	void	OnFootStep(Object* pOwner);

	void    CalculateTurnAccelerate(DWORD dwTick);	
	BOOL    SendJumpPacket();
	BOOL    SendStopPacket();
	BOOL    CalculateTargetPosition();


    static BOOL FindEnablePathPosition( const WzVector& StartPosition, const WzVector& TargetPosition, OUT WzVector& ResultPosition );
    static BOOL TestEnablePathPosition( const WzVector& TestPosition );

    static BOOL TestMoveableTerrainPosition( const WzVector& StartPosition, IN OUT WzVector& Position );

    // 길찾기를 이용한 테스트
    static BOOL TestMoveablePosition( const WzVector& StartPosition, IN OUT WzVector& TargetPosition );
    static BOOL TestObjectCollision( const WzVector& StartPosition, const WzVector& TargetPosition );

    static BOOL CalcJumpMoving( Player* pPlayer, const WzVector& CurrentPosition, DWORD dwTick);


    static bool MovePositionOnSimulatedPosition( Player* pPlayer, float fTick, OUT WzVector& Position, IN const WzVector& Direction, float& fMoveForce, float& fUpperForce );

    static void MovePositionOnJump( IN const WzVector& Direction, IN float fTick, IN OUT WzVector& Position, IN OUT float& fMoveForce, IN OUT float& fUpperForce );
    //static BOOL TestPosition( const WzVector& TestPosition );
    static BOOL TestObjectCollision( const WzVector& TestPosition );
    

    // T가 P1, P2 사이에 있는가 (z축 무시, T는 P1에서 P2 방향으로 이동)
    static BOOL IsInCource( WzVector P1, WzVector P2, WzVector T );

    BOOL    CheckRidingState(Player *pPlayer);
};
#endif

