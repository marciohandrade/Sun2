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

    // ��ã�⸦ �̿��� �׽�Ʈ
    static BOOL TestMoveablePosition( const WzVector& StartPosition, IN OUT WzVector& TargetPosition );
    static BOOL TestObjectCollision( const WzVector& StartPosition, const WzVector& TargetPosition );

    static BOOL CalcJumpMoving( Player* pPlayer, const WzVector& CurrentPosition, DWORD dwTick);


    static bool MovePositionOnSimulatedPosition( Player* pPlayer, float fTick, OUT WzVector& Position, IN const WzVector& Direction, float& fMoveForce, float& fUpperForce );

    static void MovePositionOnJump( IN const WzVector& Direction, IN float fTick, IN OUT WzVector& Position, IN OUT float& fMoveForce, IN OUT float& fUpperForce );
    //static BOOL TestPosition( const WzVector& TestPosition );
    static BOOL TestObjectCollision( const WzVector& TestPosition );
    

    // T�� P1, P2 ���̿� �ִ°� (z�� ����, T�� P1���� P2 �������� �̵�)
    static BOOL IsInCource( WzVector P1, WzVector P2, WzVector T );

    BOOL    CheckRidingState(Player *pPlayer);
};
#endif

