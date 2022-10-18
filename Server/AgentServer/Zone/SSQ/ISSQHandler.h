#pragma once

//==================================================================================================
//  <
class SSQMsgRouterInAgent;

//==================================================================================================
//  <ISSQHandler>
//
//  <Objective>
//  - Warp Step : timeout 1minute
//  - blah~
//
class ISSQHandler
{
public:
    static const DWORD second = 1000;
    static const DWORD minute =   60 * second;
    static const BOOLEAN UPDATE_SUCCESS = TRUE;
    static const BOOLEAN UPDATE_FAIL    = FALSE;

    enum eSTATE {
        USR_NONE_STATE              ,
        MONSTER_STATE               , // Monster 공간
        USR_REGISTERED_SSQ_INDEXER  , // CG_ZONE_EX_SSQ_JOIN_INFO_SYN 수신시
        USR_PORTAL_OBJECT_CREATED   , // AG_ZONE_SSQ_JOIN_INFO_ACK 수신후 Warp 가능 설정
        USR_PORTAL_TRANSACTION      , // CG_ZONE_EX_SSQ_JOIN_SYN 이후 CG_ZONE_EX_SSQ_JOIN_ACK까지.
        USR_ENTERING_SSQ_FIELD      , // AG_SYNC_PLAYER_ACK -> On_enteruser 발동시 -> 이후 entered이며, zone::users
        USR_ENTERED_SSQ_FIELD       , // CG_ZONE_EX_SSQ_JOIN_SYN 수신시
        USR_ENTERED_ON_DEAD         , // Player사망시
        USR_LEAVED_SSQ_FIELD        , // SSQ FIELD에서 빠져나갔을 경우,
        USR_PORTAL_FAILED           , // 포탈 통한 입장 실패(timeout, etc...)
        USR_CLEAR_INFO              , // 여러 가지 이유로 SSQ존에서 빠져나갔을 때... 좀더 생각하고 적용하기로 한다.
    };

    template< eSTATE state > struct State {
        static const eSTATE value = state;
    };

    template<>  struct State<MONSTER_STATE>
    {
        static const eSTATE value = MONSTER_STATE;
        //static const DWORD timeout = 10 * second;
        struct sInfo {
            //PVOID m_pObject;
            BOOLEAN m_IsPlayer;
            DWORD   m_ObjectKey;
            const SSQMsgRouterInAgent* m_MsgRouter;
        } INFO;

        State(const BYTE objectKey, const SSQMsgRouterInAgent* pRouter)
        {
            //INFO.m_pObject = pUser;
            INFO.m_IsPlayer = FALSE;
            INFO.m_ObjectKey = objectKey;
            INFO.m_MsgRouter = pRouter;
        }
    };

    template<>  struct State<USR_REGISTERED_SSQ_INDEXER>
    {
        static const eSTATE value = USR_REGISTERED_SSQ_INDEXER;
        static const DWORD timeout = 10 * second;
        struct sInfo {
            User*   m_pUser;
            BOOLEAN m_IsPlayer;
            DWORD   m_ObjectKey;
            const SSQMsgRouterInAgent* m_MsgRouter;
            POSTYPE m_PosOfItem;
            SLOTCODE m_ItemCode;
        } INFO;

        State(User* const pUser, const DWORD objectKey, const SSQMsgRouterInAgent* pRouter,
              const POSTYPE posOfItem)
        {
            INFO.m_pUser    = pUser;
            INFO.m_IsPlayer = TRUE;
            INFO.m_ObjectKey = objectKey;
            INFO.m_MsgRouter = pRouter;
            INFO.m_PosOfItem = posOfItem;
            INFO.m_ItemCode = 0;    // 나중에 게임서버 응답을 받고 그 정보로 설정된다.
        }
    };

    template<>  struct State<USR_PORTAL_OBJECT_CREATED>
    {
        static const eSTATE value = USR_PORTAL_OBJECT_CREATED;
        static const DWORD timeout = 50 * second;
        struct sInfo {
            POSTYPE m_PosOfItem;
        } INFO;
    };

    template<>  struct State<USR_PORTAL_TRANSACTION>
    {
        static const eSTATE value = USR_PORTAL_TRANSACTION;
        static const DWORD timeout = 10 * minute;    // 중간에 제거되지 않도록 장시간 건다.
        struct sInfo {
            POSTYPE m_PosOfItem;
        } INFO;
    };

    template<>  struct State<USR_ENTERING_SSQ_FIELD>
    {
        static const eSTATE value = USR_ENTERING_SSQ_FIELD;
        static const DWORD timeout =  5 * minute;
        struct sInfo {
        } INFO;
    };

    template<>  struct State<USR_ENTERED_SSQ_FIELD>
    {
        static const eSTATE value = USR_ENTERED_SSQ_FIELD;    // proc-handler (DungeonSSQ)
        struct sInfo {
        } INFO;
    };

    template<>  struct State<USR_ENTERED_ON_DEAD>
    {
        static const eSTATE value = USR_ENTERED_ON_DEAD;
        static const DWORD timeout =  5 * second;
        struct sInfo {
            POSTYPE                m_PosOfItem;
        } INFO;
    };

    template<>  struct State<USR_LEAVED_SSQ_FIELD>
    {
        static const eSTATE value = USR_LEAVED_SSQ_FIELD;        // proc-handler (DungeonSSQ)
        struct sInfo {
        } INFO;
    };

    template<>  struct State<USR_PORTAL_FAILED>
    {
        static const eSTATE value = USR_PORTAL_FAILED;
        struct sInfo {
        } INFO;
    };

    template<>  struct State<USR_CLEAR_INFO>
    {
        static const eSTATE value = USR_CLEAR_INFO;
        struct sInfo {
        } INFO;
    };

    ISSQHandler();
    ~ISSQHandler();

    //----------------------------------------------------------------------------------------------
    //  <INTERFACEs>
    inline eSTATE CurrentState() { return m_eState; }
    //template< eSTATE state >
    //BOOLEAN   ChangeStatus(const State<state>& state_info);
    BOOLEAN ChangeStatus(const State<MONSTER_STATE>& state_info);

    BOOLEAN ChangeStatus(const State<USR_REGISTERED_SSQ_INDEXER>& state_info);
    BOOLEAN ChangeStatus(const State<USR_PORTAL_OBJECT_CREATED>& state_info);
    BOOLEAN ChangeStatus(const State<USR_PORTAL_TRANSACTION>& state_info);
    BOOLEAN ChangeStatus(const State<USR_ENTERING_SSQ_FIELD>& state_info);
    BOOLEAN ChangeStatus(const State<USR_ENTERED_SSQ_FIELD>& state_info);
    BOOLEAN ChangeStatus(const State<USR_LEAVED_SSQ_FIELD>& state_info);
    BOOLEAN ChangeStatus(const State<USR_ENTERED_ON_DEAD>& state_info);
    BOOLEAN ChangeStatus(const State<USR_PORTAL_FAILED>& state_info);
    //BOOLEAN ChangeStatus(const State<USR_CLEAR_INFO>& state_info);

    inline BOOLEAN  IsUserObject() const { return m_FLD_USR_INFO.m_IsPlayer; }
    BOOLEAN IsPreparedReqTransaction();
    BOOLEAN IsWarpObjectAlived();
    BOOLEAN IsWarpTransaction();
    inline VOID     SetUsedItemCode(const SLOTCODE itemCode) { m_FLD_USR_INFO.m_ItemCode = itemCode; }
    inline SLOTCODE GetUsedItemCode() const { return m_FLD_USR_INFO.m_ItemCode; }

    BOOLEAN Update();  // FALSE면 제거
    VOID    Release(); // Update실패시 이 루틴을 먼저 호출해 준다.

private:
    inline VOID ClearDupInfo() { ZeroMemory(&m_Dup, sizeof(m_Dup)); }
    eSTATE  m_eState;
    ITimeout m_Timer;    // Shared Function by State
    State<USR_REGISTERED_SSQ_INDEXER>::sInfo m_FLD_USR_INFO;
    union {
        State<USR_PORTAL_OBJECT_CREATED>::sInfo FLD_PORTAL_OBJECT;
        State<USR_PORTAL_TRANSACTION>::sInfo  FLD_PORTAL_TRANS;
        State<USR_ENTERING_SSQ_FIELD>::sInfo  FLD_SSQ_FIELD_ENTTRANS;
        State<USR_ENTERED_SSQ_FIELD>::sInfo FLD_SSQ_FIELD;
        State<USR_LEAVED_SSQ_FIELD>::sInfo  FLD_SSQ_FIELD_LEAVED;
        State<USR_ENTERED_ON_DEAD>::sInfo   FLD_ON_DEAD;
        State<USR_PORTAL_FAILED>::sInfo FLD_PORTAL_FAIL;
        State<USR_CLEAR_INFO>::sInfo    FLD_CLEAR_INFO;
    }   m_Dup;
};

