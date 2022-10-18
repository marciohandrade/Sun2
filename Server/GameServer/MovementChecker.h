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
    static const DWORD TIMEOUT_BIT_K       = 0x00000002;   // Ű���� ����
    static const DWORD TIMEOUT_BIT_J       = 0x00000008;   // ���� ����
    static const DWORD TIMEOUT_BIT_M       = 0x00000010;   // ���콺 ����
    static const DWORD TIMEOUT_BIT_KJM     = TIMEOUT_BIT_K | TIMEOUT_BIT_J | TIMEOUT_BIT_M;    // JKM ��� ����
    static const DWORD INSTANCE_BIT_K      = 0x00000200;   // Ű���� ��� 
    static const DWORD INSTANCE_BIT_J      = 0x00000800;   // ���� ���
    static const DWORD INSTANCE_BIT_M      = 0x00001000;   // ���콺 ���
    static const DWORD INSTANCE_BIT_KJM    = INSTANCE_BIT_K | INSTANCE_BIT_J | INSTANCE_BIT_M; // JKM ��� ��� ����

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

    //  [1] : Ŭ���̾�Ʈ�� ����, �Ǵ� ������ ����� �̵��� ��
    //  [2] : ������ ���� ������ ��ġ�� 
    BOOL AddMovement( WzVector& wvCurPos/*, WzVector* pwvPosOfPlayer = NULL*/ );
    void AbusingNotify( RC::eSYNC_RESULT eRET, DWORD IN dwErrorLevel, float fDIST = .0f );
    nsSync::CSSyncTuner* GetSyncTuner();

    //----------------------------------------------------------------------------------------------
    //  FRAME �� ƽ ���� ����, �ѹ��� ������ ��쿡 ���� ���뼺 ����
    static void Update( DWORD dwDeltaTick );
    static BOOL PreCalcScript();
private:
    inline static DWORD DeltaTick() { return sdwFRAME_POLE_TICK; }

private:
    struct sPASSING_ARG;
    //----------------------------------------------------------------------------------------------
    //  ���� �⺻ ���� ����
private:
    eMOVE_CTRL  CanStartAction( const eMOVE_TYPE mtTYPE, const WORD wAngle, const BYTE byMoveState, const BOOL bSpecial = FALSE );
    inline BOOL IsFiniteVector( const WzVector& rVector );
    inline BOOL IsFaultToleranceDifference( const BYTE byMoveState );
    BOOL        CheckSharedFault( FlowControl& INOUT flow );

    //----------------------------------------------------------------------------------------------
    //  Ŭ�� ��û ��Ʈ��
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
    //  ��ʺ� ���� ��� ����



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
        eCHAR_MOVE_STATE eState,         // CMS_RUN, ... �̵� ����
        WzVector& IN OUT wvCurPosR,      // ���� ��ǥ
        WzVector& IN OUT wvDestPosR,     // �̵��� ��ǥ
        INT              iNumRoute,      // PathRoute ����
        TILEINDEX*       piRoute,        // PathRoute �迭
        DWORD& OUT       rdwErrorLevel
        );

    RC::eSYNC_RESULT KBMoveCommonCHECK(
        WzVector&   wvCurPos,
        BYTE IN     byMoveState,            // �̵� ���� (��/����, �¿���, ���� �̵� ���...)
        BOOL& OUT   bNeedMoveCalc,          // ���⸸ �ٲ����, ���� �̵� ����� �䱸�Ǵ���...
        DWORD& OUT  rdwErrorLevel
        );
    void KBMoveCommonDIR(
        WzVector& OUT wvDirection,          // ���, �̵� ���� ���� ����
        WORD IN wAngle,                     // �̵��Ϸ��� ����
        BOOL IN bFastMove,                  // FastMove ���� ����
        BYTE IN byMoveState                 // �̵� ���� (��/����, �¿���, ���� �̵� ���...)
        );
    inline BOOL KBMoveCommonDIST(
        WzVector& OUT wvDirection           // ���, �̵� ���� ���� ����
        );
    RC::eSYNC_RESULT KBMoveCommonUPDATE( Player* pPlayer, WzVector& wvCurPos );

private:
    Player* m_pPlayer; // �˻� ���
    BOOL    m_bFirst;  // �˻� ���� ���� ����

    // ���� �ð� �� ���ŵ� �޽��� �� �ʵ�
public:
    struct sMSG_TIMER_INFO
    {
        ITimeout AcceptTimer;
        DWORD    ReceivedCount;

        inline void  Reset();
        inline DWORD Increase();
    };
    // �̵� �� �ʵ�
public:
    struct sPATH_INFO
    {
        DWORD    LatestAcceptTick;           // ���� �ֱ� ���ŵ� �ð�
        WzVector LatestAcceptPosition;       // ���� �ֱ� ���ŵ� ��ġ
        float    fAccumulatedDistance;       // ���� ����

        BOOL SetupLatestState(
            MovementChecker* pChecker,  //
            float fDistOfCS,            // ������ Ŭ���̾�Ʈ ��ġ ����
            BYTE byMoveState,           // �̵� ����
            WzVector& wvCurPos,         // ������ �ν��ϴ� ���� ��ǥ
            WzVector& wvPos             // Ŭ�� ��û�� ���� ��ǥ
            );
    };
    inline sPATH_INFO*  GetServerPathExpUpdate()    { return &PATH_EXP; }

private:
    sMSG_TIMER_INFO INTERNAL;           // JUMP
    sMSG_TIMER_INFO MOVEKEYBOARD;       // ����Ű, FASTMOVE
    WORD            MOVEKEYBOARD_ANGLE;
    BYTE            MOVEKEYBOARD_STATE; // ������ �ٸ� ��� ACCEPT
    sMSG_TIMER_INFO MOVEKEYBOARDJ;      // JUMP
    sMSG_TIMER_INFO MOVEMOUSE;          // ���콺, TARGET MOVE

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