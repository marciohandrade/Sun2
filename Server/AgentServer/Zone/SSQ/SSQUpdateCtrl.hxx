#pragma once

//==================================================================================================
// <SSQUpdateCtrl>
class SSQUpdateCtrl
{
protected:
    friend class WarControlManager; // <이건 임시 Updater 생성/치환/삭제 등등을 지원하는 함수 추가후 제거할 것.
    friend struct SSQControlManagerImpl;
    friend class SSQMsgRouterInAgent;
    friend struct OpSSQInfoController;
    typedef sTIME_INFO curtime_info;

    static const DWORD second = 1000;
    static const DWORD minute =   60 * second;

    enum eROOM_CTRL_STATUS
    {
        RCS_NONE                    = (0 << 0),
        RCS_ENABLE_ENTER            = (1 << 0), //  입장 가능 상태
        RCS_ENABLE_CREATE_PORTAL    = (1 << 1), //  포탈 생성 가능 상태
        RCS_BATTLE_STARTED          = (1 << 3), //  전투 시작된 상태
        RCS_BATTLE_END_REQUESTED    = (1 << 4), //  G->A로의 전투 종료 요청 상태
        RCS_CLEARING                = (1 << 5), //  A->G로의 클리어 요청 트랜잭션
        RCS_CLEARED                 = (1 << 6), //  G->A로의 클리어 완료 요청 상태
        RCS_LAST_FIELD              = (1 << 7), //  허용 가능 BitField Top
        RCS_VALUE_MASK              = 256 - 1,
    };
    BOOST_STATIC_ASSERT(RCS_VALUE_MASK >= RCS_LAST_FIELD);

public:
    SSQUpdateCtrl()
    {
        MigrationDefault(0, 0, 0, 0);
    }
    virtual ~SSQUpdateCtrl() {}


protected:
    struct State_Data
    {
        DWORD m_State;
        DWORD m_Begin, m_End;
        DWORD m_Next;

        inline static BOOLEAN IsAcceptSection(const curtime_info& tmInfo, const DWORD startTick,
                                              const DWORD begin, const DWORD end);
    };

    void MigrationDefault(const SSQCODE CodeOfSSQ,
                          SSQControlManagerImpl* const pMgrImpl,
                          SSQMsgRouterInAgent* const m_pMsgRouter,
                          SSQInfo* const pSSQInfo);
    static void Migration(SSQUpdateCtrl* const pDest, const SSQUpdateCtrl* pSrc)
    {
        pDest->MigrationDefault(pSrc->m_CodeOfSSQ, pSrc->m_pManager, pSrc->m_pMsgRouter,
                                pSrc->m_pSSQInfo);
        pDest->m_pSSQField = pSrc->m_pSSQField;
        pDest->m_ControlField = pSrc->m_ControlField;
        pDest->m_ProcedIndex = pSrc->m_ProcedIndex;
        pDest->m_IsFastTrack = pSrc->m_IsFastTrack;
    }

    static void RegisterUpdateCtrl(SSQUpdateCtrl* const pTarget);
public:
    virtual void Init(const curtime_info& tmInfo) = 0;
    virtual BOOL Update(const curtime_info& tmInfo) = 0;

    BOOL UpdateMsg(const curtime_info& tmInfo);
    void RecvedMsgProcess(const WAR_MSG_HDR* IN pStream);
    void RecvedMsgProcessSpecial(const WAR_MSG_HDR* IN pStream);

    inline const SSQCODE GetCodeOfSSQ() const { return m_CodeOfSSQ; }
    inline const ITimerBase& GetTimer() const { return m_Timer; }
    inline const SSQInfo* GetSSQInfo() const  { return m_pSSQInfo; }
    inline SSQMsgRouterInAgent* GetMsgRouter() const { return m_pMsgRouter; }
    inline eTIME_TYPE GetTimeType() const     { return m_CurType; }
    inline void SetTimeType(const eTIME_TYPE timeType) { m_CurType = timeType; }
    inline DungeonSSQ* GetSSQField() const    { return m_pSSQField; }
    inline void SetSSQField(DungeonSSQ* const pSSQField) { m_pSSQField = pSSQField; }

    //  Status Control
    inline BOOLEAN IsEnablePotalCreate() const;
    inline BOOLEAN IsEnableEnter() const;
    inline void BattleEndRequested() { m_ControlField |= RCS_BATTLE_END_REQUESTED; }
    inline void ProcessCleared()     { m_ControlField |= RCS_CLEARED; }

    //  interfaces for easy usage
    void UserEnter(User* const pUser);
    void UserLeave(User* const pUser);

protected:
    SSQCODE     m_CodeOfSSQ;
    eTIME_TYPE  m_CurType;
    ITimerBase  m_Timer;
    SSQControlManagerImpl* m_pManager;
    SSQMsgRouterInAgent* m_pMsgRouter;
    SSQInfo*    m_pSSQInfo;
    DungeonSSQ* m_pSSQField;
    DWORD m_ControlField; //  입장 상태 제어 필드
    BYTE    m_ProcedIndex;  //  입장 시간 4가지 중에서의 인덱스
    BOOLEAN m_IsFastTrack;  //  SSQ Fast Track, by GM
};


inline BOOLEAN
SSQUpdateCtrl::IsEnablePotalCreate() const
{
    return m_pSSQField &&
           (m_ControlField & (RCS_ENABLE_ENTER)) &&
           (m_ControlField & (RCS_ENABLE_CREATE_PORTAL)) &&
           !(m_ControlField & (RCS_BATTLE_END_REQUESTED | RCS_CLEARED));
}


inline BOOLEAN
SSQUpdateCtrl::IsEnableEnter() const
{
    return m_pSSQField &&
           (m_ControlField & (RCS_ENABLE_ENTER)) &&
           !(m_ControlField & (RCS_BATTLE_END_REQUESTED | RCS_CLEARED));
}


inline BOOLEAN
SSQUpdateCtrl::State_Data::IsAcceptSection(const curtime_info& tmInfo,
                                           const DWORD startTick,
                                           const DWORD begin, const DWORD end)
{
    __UNUSED(begin);
    //  원래는 #1과 같이 처리해야 하는데... 일단 #2) timeout만 고려하자.
    const DWORD offsetTick = tmInfo.CurTick - startTick;
    //return (begin<=offsetTick) && (offsetTick<end);   // #1
    return offsetTick < end ;                           // #2
}


//==================================================================================================
//
class SSQUpdateCtrl_None : public SSQUpdateCtrl
{
public:
    static const DWORD interval = 5 * second;
    static const eTIME_TYPE value = SSQControlInfo::TT_NONE;
    typedef SSQUpdateCtrl_None type;
    typedef SSQUpdateHandler_info< value, type, interval > updater_info;

    virtual void Init(const curtime_info& tmInfo);
    virtual BOOL Update(const curtime_info& tmInfo);
    inline void EnableFastTrack() { m_IsFastTrack = true; }
};

//==================================================================================================
//
class SSQUpdateCtrl_Notice : public SSQUpdateCtrl
{
public:
    static const DWORD broadcast_countdown_init = 5;
    static const DWORD broadcast_changeto_waiting_countdown_value = 0;
    static const DWORD interval = 5 * second - WarControlManager::INTERVALofUPDATE;
    static const DWORD interval_broadcast = 1 * minute;    // 1분마다 공지
    static const eTIME_TYPE value = SSQControlInfo::TT_NOTICE;
    typedef SSQUpdateCtrl_Notice type;
    typedef SSQUpdateHandler_info< value, type, interval > updater_info;

    SSQUpdateCtrl_Notice() {}

    virtual void Init(const curtime_info& tmInfo);
    virtual BOOL Update(const curtime_info& tmInfo);

private:
    void SendNoticeBroadcast(const SSQInfo::COUNT_T countdown_value);
    void Process_CreateSSQField();

protected:
    DWORD m_NextTimeout;
    DWORD m_StartTick;
    AcmTmFmt m_NextAcmTime;
    BYTE m_StartCountdown;
    BYTE m_Countdown;
};


//==================================================================================================
//
struct SSQCCUReporter
{
    typedef sTIME_INFO curtime_info;
    static const DWORD second = 1000;
    static const DWORD minute =   60 * second;

    ITimeout m_CCUReportTimer;
    DWORD m_EndTick;

    void Init(const curtime_info& tmInfo, SSQUpdateCtrl* const pUpdateCtrl);
    // (WAVERIX) (090720) (CHANGES) void->bool (true: continued, false: overthan endtick)
    bool Update(const curtime_info& tmInfo, SSQUpdateCtrl* const pUpdateCtrl);
};

//==================================================================================================
//
class SSQUpdateCtrl_Waiting : public SSQUpdateCtrl// : public SSQUpdateCtrl_Notice
{
private:
    enum eSSQ_WAITING_EVENT_TYPE
    {
        SWET_START_WAITING_NODE         , //  전투 대기 상태 시작
        SWET_NOTICE_COUNTDOWN           , //  입장 종료 공지 2분 전, 1분 전
        SWET_ENTERENCE_CLOSED           , //  입장 종료 공지
        SWET_FINAL_ENTERENCE_TIMEOUT    , //  클라
        SWET_GAMBLE_PROCESS             , //  갬블 추첨 기간
        SWET_BATTLE_GRACE_PERIOD        , //  전투 시작전 유예기간
        SWET_BATTLE_START_JUST_BEFORE   , //  전투 직전 (5" 전으로 설정하자.)
        SWET_BATTLE_COUNTDOWN           , //  전투 카운트다운 5"
        SWET_UPPERBOUND
    };
    typedef eSSQ_WAITING_EVENT_TYPE     SWET;
public:
    static const DWORD interval = 1 * second;

    static const DWORD enterable_limit_enter_notice = 2 * minute;
    static const DWORD enterable_limit_countdown_notice = 4 * minute; // x분 남았습니다. 공지 (2분 전, 1분 전)
    static const DWORD enterable_limit_portal_close = 4 * minute + 5 * second; // 입장 종료 (포탈 생성 막는다)
    static const DWORD enterable_limit_with_portal_delay_tick = 5 * minute; //  4*minute + 50*second;    // 4' 50''
    static const DWORD enter_ack_timeout_limit = 6 * minute; //  enter_ack이 오는 시점이 제한 시간을 넘어서는 안된다.
    static const DWORD gameble_timeout_limit = enter_ack_timeout_limit + (25 * second);
    static const DWORD battle_grace_period = 6 * minute + 50 * second;
    static const DWORD battle_countdown_timeout = 7 * minute;
    BOOST_STATIC_ASSERT((enterable_limit_enter_notice < enterable_limit_countdown_notice) &&
                        (enterable_limit_countdown_notice < enterable_limit_portal_close) &&
                        (enterable_limit_portal_close < enterable_limit_with_portal_delay_tick) &&
                        (enterable_limit_with_portal_delay_tick < enter_ack_timeout_limit) &&
                        (enter_ack_timeout_limit < gameble_timeout_limit) &&
                        (gameble_timeout_limit < battle_grace_period) &&
                        (battle_grace_period < battle_countdown_timeout));
    
    static const eTIME_TYPE value = SSQControlInfo::TT_WAITING;
    typedef SSQUpdateCtrl_Waiting type;
    typedef SSQUpdateHandler_info< value, type, interval > updater_info;

    SSQUpdateCtrl_Waiting() {}
    virtual void Init(const curtime_info& tmInfo);
    virtual BOOL Update(const curtime_info& tmInfo);

    inline void SetGambleResult(const MSGSUB_SSQ_S2S_GAMBLE_RESULT& rMsg) { m_GambleResult = rMsg; }
private:
    void Process_Change_BattleState(const curtime_info& tmInfo);
    void Process_Change_ClearState(const curtime_info& tmInfo);    // 이중 분기: 전투 시작 조건 불만족시
    inline void Clear_WaitingState_Vars();

    #define WAITING_STATE_PROCNODE(CURR, NEXT)    \
        void SSQUpdateCtrl_Waiting::StateProc_##CURR##_to_##NEXT(const curtime_info& tmInfo)
    #define WAITING_STATE_PROCNODE_CALL(CURR, NEXT, arg)    \
        StateProc_##CURR##_to_##NEXT(arg)

    WAITING_STATE_PROCNODE(SWET_START_WAITING_NODE,         SWET_NOTICE_COUNTDOWN);
    WAITING_STATE_PROCNODE(SWET_NOTICE_COUNTDOWN,           SWET_ENTERENCE_CLOSED);
    WAITING_STATE_PROCNODE(SWET_ENTERENCE_CLOSED,           SWET_FINAL_ENTERENCE_TIMEOUT);
    WAITING_STATE_PROCNODE(SWET_FINAL_ENTERENCE_TIMEOUT,    SWET_GAMBLE_PROCESS);
    WAITING_STATE_PROCNODE(SWET_GAMBLE_PROCESS,             SWET_BATTLE_GRACE_PERIOD);
    WAITING_STATE_PROCNODE(SWET_BATTLE_GRACE_PERIOD,        SWET_BATTLE_START_JUST_BEFORE);
    WAITING_STATE_PROCNODE(SWET_BATTLE_START_JUST_BEFORE,   SWET_BATTLE_COUNTDOWN);

    DWORD m_StartTick; // 대기 시간이 된 시점의 TickCount
    SSQCCUReporter m_CCUReport;
    struct { // sub updater arguments
        DWORD m_EventType; // (SWET) 대기 시간에는 다양한 이벤트가 존재한다. 그런 이벤트 관리용
        DWORD m_EventArg0; // 해당 이벤트에서 사용할 임시 변수
        DWORD m_EventArg1; // 해당 이벤트에서 사용할 임시 변수
    };
    State_Data m_StateData[SWET_UPPERBOUND];    // 각종 이벤트 시간 제어를 위한 임시 공간
    MSGSUB_SSQ_S2S_GAMBLE_RESULT m_GambleResult;
};


//==================================================================================================
//
class SSQUpdateCtrl_Battle : public SSQUpdateCtrl
{
public:
    static const DWORD interval = 100;
    static const eTIME_TYPE value = SSQControlInfo::TT_BATTLE;
    typedef SSQUpdateCtrl_Battle    type;
    typedef SSQUpdateHandler_info< value, type, interval > updater_info;

    virtual void Init(const curtime_info& tmInfo);
    virtual BOOL Update(const curtime_info& tmInfo);
    //void SendRemainTimeInfo(const curtime_info& tmInfo);

private:
    WzTmFmt m_BattleCloseTimeFlag;  // AcmTime
    WzTmFmt m_BattleCloseCountdown; // 
    SSQCCUReporter m_CCUReport;
};

//==================================================================================================
//
class SSQUpdateCtrl_Clear : public SSQUpdateCtrl
{
public:
    static const DWORD interval = 5 * second;
    static const DWORD timeout_post_battle = 2 * minute; // (15')이내로 끝나는 battle상태 이후 최종 정리 단계 timeout
    static const eTIME_TYPE value = SSQControlInfo::TT_CLEAR;
    typedef SSQUpdateCtrl_Clear type;
    typedef SSQUpdateHandler_info< value, type, interval > updater_info;

    virtual void Init(const curtime_info& tmInfo);
    virtual BOOL  Update(const curtime_info& tmInfo);

private:
    void _Step_Wait_Cleared(const curtime_info& tmInfo);
    void _Step_Next(const curtime_info& tmInfo);

    enum eStep { eStep_Init, eStep_Wait_Cleared, eStep_Next, };
    eStep m_ClearStep;
};

//==================================================================================================
//
typedef SSQUpdateCtrl_None::updater_info    SSQUpdateCtrl_None_Info;
typedef SSQUpdateCtrl_Notice::updater_info  SSQUpdateCtrl_Notice_Info;
typedef SSQUpdateCtrl_Waiting::updater_info SSQUpdateCtrl_Waiting_Info;
typedef SSQUpdateCtrl_Battle::updater_info  SSQUpdateCtrl_Battle_Info;
typedef SSQUpdateCtrl_Clear::updater_info   SSQUpdateCtrl_Clear_Info;

#undef SSQ_UPDATE_CTRL_NODE
//#define SSQ_UPDATE_NODE(_type)    blah~
#define SSQ_UPDATE_CTRL_LIST()  \
    SSQ_UPDATE_CTRL_NODE(SSQUpdateCtrl_None_Info)   \
    SSQ_UPDATE_CTRL_NODE(SSQUpdateCtrl_Notice_Info) \
    SSQ_UPDATE_CTRL_NODE(SSQUpdateCtrl_Waiting_Info)\
    SSQ_UPDATE_CTRL_NODE(SSQUpdateCtrl_Battle_Info) \
    SSQ_UPDATE_CTRL_NODE(SSQUpdateCtrl_Clear_Info)
