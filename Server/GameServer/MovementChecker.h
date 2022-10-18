#pragma once
#ifndef GAMESERVER_MOVEMENT_CHECKER_H
#define GAMESERVER_MOVEMENT_CHECKER_H

#include "SkillSystem/CSSyncTuner.h"


//==================================================================================================
//  <MovementChecker>
//
class MovementChecker
{
public:
    static const DWORD INTERVAL_LASTED_ACCEPT_SYN      = 15;       //ms
    static const DWORD DELAY_MIN_MOVE_TRANSACTION      = 200;      //ms
    static const DWORD DELAY_MIN_DIR_TRANSACTION       = 100;      //ms
    static const DWORD DELAY_MIN_JUMP_TRANSACTION      = 500;      //ms

    static const DWORD DELAY_DEFAULT_KBMOVE_SYN        = 500;      //ms
    static const DWORD DELAY_MAX_PERMIT_LASTED_STATE   = DELAY_DEFAULT_KBMOVE_SYN;     //ms

    enum eMOVE_TYPE
    {
        KEYBOARD_MOVE       = 0,
        KEYBOARD_JUMP       ,
        KEYBOARD_JUMP_STOP  ,
        MOUSE_MOVE          ,
        MOVE_STOP           ,
    };
    enum eMOVE_CTRL
    {
        CANTACCEPTABLE      = 0,
        ACCEPT_STOP         = 1,
        ACCEPT_ALL          = ACCEPT_STOP|2,
    };

private:
    static const DWORD TIMEOUT_BIT_PASS    = 0x00000000;
    static const DWORD TIMEOUT_BIT_I       = 0x00000001;
    static const DWORD TIMEOUT_BIT_K       = 0x00000002;   // 키보드 불허
    static const DWORD TIMEOUT_BIT_J       = 0x00000008;   // 점프 불허
    static const DWORD TIMEOUT_BIT_M       = 0x00000010;   // 마우스 불허
    static const DWORD TIMEOUT_BIT_KJM     = TIMEOUT_BIT_K | TIMEOUT_BIT_J | TIMEOUT_BIT_M;    // JKM 모두 불허
    static const DWORD INSTANCE_BIT_K      = 0x00000200;   // 키보드 허용 
    static const DWORD INSTANCE_BIT_J      = 0x00000800;   // 점프 허용
    static const DWORD INSTANCE_BIT_M      = 0x00001000;   // 마우스 허용
    static const DWORD INSTANCE_BIT_KJM    = INSTANCE_BIT_K | INSTANCE_BIT_J | INSTANCE_BIT_M; // JKM 모두 사용 가능

private:
    static float    sfSYNC_HACK_DISTANCE2;              // by SCRIPT DATA
    static float    sfSYNC_FAIL_DISTANCE2;              // by SCRIPT DATA
    static float    sfSYNC_FAIL_DISTANCE_ACCEPT2;       // by SCRIPT DATA+a
    static float    sfSYNC_WARN_DISTANCE2;              // by SCRIPT DATA
    static DWORD    sdwFRAME_DELTA_TICK;
    static DWORD    sdwFRAME_POLE_TICK;
    static WzVector sWzVectorZero;

    //----------------------------------------------------------------------------------------------
    //
public:
    MovementChecker( Player* const pPlayer );
    ~MovementChecker();

    void Reset();

    //  [1] : 클라이언트가 보낸, 또는 서버가 계산한 이동된 값
    //  [2] : 서버가 현재 보유한 위치값 
    BOOL AddMovement( WzVector& wvCurPos/*, WzVector* pwvPosOfPlayer = NULL*/ );
    void AbusingNotify( RC::eSYNC_RESULT eRET, DWORD IN dwErrorLevel, float fDIST = .0f );
    nsSync::CSSyncTuner* GetSyncTuner();

    //----------------------------------------------------------------------------------------------
    //  FRAME 당 틱 갱신 시켜, 한번에 들어오는 경우에 대한 융통성 적용
    static void Update( DWORD dwDeltaTick );
    static BOOL PreCalcScript();
private:
    inline static DWORD DeltaTick() { return sdwFRAME_POLE_TICK; }

private:
    struct sPASSING_ARG;
    //----------------------------------------------------------------------------------------------
    //  계산용 기본 공통 로직
private:
    eMOVE_CTRL  CanStartAction( const eMOVE_TYPE mtTYPE, const WORD wAngle, const BYTE byMoveState, const BOOL bSpecial = FALSE );
    inline BOOL IsFiniteVector( const WzVector& rVector );
    inline BOOL IsFaultToleranceDifference( const BYTE byMoveState );
    BOOL        CheckSharedFault( FlowControl& INOUT flow );

    //----------------------------------------------------------------------------------------------
    //  클라 요청 엔트리
public:
    RC::eSYNC_RESULT OnRecvCG_SYNC_KBMOVE_SYN( MSG_CG_SYNC_KBMOVE_SYN* pRecvMsg, DWORD& OUT rdwErrorLevel );
    RC::eSYNC_RESULT OnRecvCG_SYNC_FAST_KBMOVE_SYN( MSG_CG_SYNC_FAST_KBMOVE_SYN* pRecvMsg, DWORD& OUT rdwErrorLevel );
    RC::eSYNC_RESULT OnRecvCG_SYNC_STOP_SYN( MSG_CG_SYNC_STOP_SYN* pRecvMsg, DWORD& OUT rdwErrorLevel );

    enum eJUMP_TYPE { JUMP_NORMAL, JUMP_STOP };
    RC::eSYNC_RESULT OnRecvCG_SYNC_JUMP_SYN( MSG_CG_SYNC_JUMP_SYN* pRecvMsg, DWORD& OUT rdwErrorLevel, eJUMP_TYPE jumpType = JUMP_NORMAL );
    RC::eSYNC_RESULT OnRecvCG_SYNC_JUMP_STOP_SYN( MSG_CG_SYNC_JUMP_STOP_SYN* pRecvMsg, DWORD& OUT rdwErrorLevel );

    RC::eSYNC_RESULT OnRecvCG_SYNC_MOVE_SYN( MSG_CG_SYNC_MOVE_SYN* pRecvMsg, DWORD& OUT rdwErrorLevel );
    RC::eSYNC_RESULT OnRecvCG_SYNC_TARGET_MOVE_SYN( MSG_CG_SYNC_TARGET_MOVE_SYN* pRecvMsg, DWORD& OUT rdwErrorLevel );

    //----------------------------------------------------------------------------------------------
    //  사례별 복합 계산 로직



#ifdef _NA_0_20110803_PLAYER_MOVEMENT_SYNC
    private:
        RC::eSYNC_RESULT do_fast_run(ushort angle, uchar fast_state);
        RC::eSYNC_RESULT do_run(ushort angle, uchar keyboard_state);
        RC::eSYNC_RESULT check_state_enable_move();
        void send_move_fail(RC::eSYNC_RESULT result);
        bool is_same_position(WzVector& client_position);
        void apply_angle(ushort angle);

        float  angle_;

    public:
        void KeyBoard_Move(MSG_CG_SYNC_KBMOVE_SYN* recv_message);
        void KeyBoard_Move(MSG_CG_SYNC_FAST_KBMOVE_SYN* recv_message);
        void Move_Stop(MSG_CG_SYNC_STOP_SYN* recv_message);
        bool Update_Moving_Turn(uint delta_tick);
#endif //_NA_0_20110803_PLAYER_MOVEMENT_SYNC



private:
    RC::eSYNC_RESULT MoveCommon(
        BOOL             bIsTargetMove,
        eCHAR_MOVE_STATE eState,         // CMS_RUN, ... 이동 상태
        WzVector& IN OUT wvCurPosR,      // 현재 좌표
        WzVector& IN OUT wvDestPosR,     // 이동할 좌표
        INT              iNumRoute,      // PathRoute 개수
        TILEINDEX*       piRoute,        // PathRoute 배열
        DWORD& OUT       rdwErrorLevel
        );

    RC::eSYNC_RESULT KBMoveCommonCHECK(
        WzVector&   wvCurPos,
        BYTE IN     byMoveState,            // 이동 상태 (전/후진, 좌우진, 조합 이동 등등...)
        BOOL& OUT   bNeedMoveCalc,          // 방향만 바꿨는지, 실제 이동 계산이 요구되는지...
        DWORD& OUT  rdwErrorLevel
        );
    void KBMoveCommonDIR(
        WzVector& OUT wvDirection,          // 결과, 이동 방향 벡터 설정
        WORD IN wAngle,                     // 이동하려는 각도
        BOOL IN bFastMove,                  // FastMove 설정 여부
        BYTE IN byMoveState                 // 이동 상태 (전/후진, 좌우진, 조합 이동 등등...)
        );
    inline BOOL KBMoveCommonDIST(
        WzVector& OUT wvDirection           // 결과, 이동 방향 벡터 설정
        );
    RC::eSYNC_RESULT KBMoveCommonUPDATE( Player* pPlayer, WzVector& wvCurPos );

private:
    Player* m_pPlayer; // 검사 대상
    BOOL    m_bFirst;  // 검사 시작 여부 설정

    // 단위 시간 내 수신된 메시지 평가 필드
public:
    struct sMSG_TIMER_INFO
    {
        ITimeout AcceptTimer;
        DWORD    ReceivedCount;

        inline void  Reset();
        inline DWORD Increase();
    };
    // 이동 평가 필드
public:
    struct sPATH_INFO
    {
        DWORD    LatestAcceptTick;           // 가장 최근 갱신된 시각
        WzVector LatestAcceptPosition;       // 가장 최근 갱신된 위치
        float    fAccumulatedDistance;       // 누적 오차

        BOOL SetupLatestState(
            MovementChecker* pChecker,  //
            float fDistOfCS,            // 서버와 클라이언트 위치 오차
            BYTE byMoveState,           // 이동 상태
            WzVector& wvCurPos,         // 서버가 인식하는 현재 좌표
            WzVector& wvPos             // 클라가 요청한 현재 좌표
            );
    };
    inline sPATH_INFO*  GetServerPathExpUpdate()    { return &PATH_EXP; }

private:
    sMSG_TIMER_INFO INTERNAL;           // JUMP
    sMSG_TIMER_INFO MOVEKEYBOARD;       // 방향키, FASTMOVE
    WORD            MOVEKEYBOARD_ANGLE;
    BYTE            MOVEKEYBOARD_STATE; // 방향이 다를 경우 ACCEPT
    sMSG_TIMER_INFO MOVEKEYBOARDJ;      // JUMP
    sMSG_TIMER_INFO MOVEMOUSE;          // 마우스, TARGET MOVE

    sPATH_INFO PATH_EXP;
    //_NA000000_SPEED_HACK_PREVENTION_SUPPORT
    nsSync::CSSyncTuner sync_tuner_;
    //----------------------------------------------------------------------------------------------
    friend struct sPATH_INFO;
    __DISABLE_COPY(MovementChecker);
};

//==================================================================================================

inline nsSync::CSSyncTuner* MovementChecker::GetSyncTuner() {
    return &sync_tuner_;
};

#endif //GAMESERVER_MOVEMENT_CHECKER_H