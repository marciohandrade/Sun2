#pragma once

#ifdef _DEBUG
    #define WAVERIX_DEBUG_CODE(code)    code
#else
    #define WAVERIX_DEBUG_CODE(code)
#endif

#define WAVERIX_SSQ_CODE_BACKUP        (0)

class User;
class GameServerSession;
class WorldServerSession;
class DungeonSSQ;

//==================================================================================================
//  <SSQControl Command List>
//
struct SSQCtrlCmd
{
    struct Created {};
    struct Destroy {};
    struct UserState {
        User* const pUser;
        BOOLEAN     isEnter;
        UserState(User* const _pUser, const BOOLEAN _isEnter)
            : pUser(_pUser), isEnter(_isEnter) {}
        struct Enter;
        struct Leave;
        __DISABLE_COPY(UserState);
    };
    enum eEvent { OnAG_RoomJoinAck, OnMax };
    struct UserEvent {
        User* const  pUser;
        const eEvent event;
        UserEvent(User* const _pUser, const eEvent _event)
            : pUser(_pUser), event(_event) {}

        struct AG_RoomJoinAck;
        __DISABLE_COPY(UserEvent);
    };
    struct UserState::Enter : public UserState {
        Enter(User* const pUser) : UserState(pUser, TRUE) {}
        __DISABLE_COPY(Enter);
    };
    struct UserState::Leave : public UserState {
        Leave(User* const pUser) : UserState(pUser, FALSE) {}
        __DISABLE_COPY(Leave);
    };

    struct UserEvent::AG_RoomJoinAck : public UserEvent {
        AG_RoomJoinAck(User* const pUser) : UserEvent(pUser, OnAG_RoomJoinAck) {}
        __DISABLE_COPY(AG_RoomJoinAck);
    };
};

struct SSQSessionCmd
{
    struct FromClient;
    struct FromGame;
    struct FromWorld;
};

struct SSQSessionCmd::FromClient
{
    FromClient(User* const pSession)
        : m_pSession(pSession) {}
    User* m_pSession;
};

struct SSQSessionCmd::FromGame
{
    FromGame(GameServerSession* const pSession)
        : m_pSession(pSession) {}
    GameServerSession* m_pSession;
};

struct SSQSessionCmd::FromWorld
{
    FromWorld(WorldServerSession* const pSession)
        : m_pSession(pSession) {}
    WorldServerSession* m_pSession;
};

struct SSQControlManagerImpl;


//==================================================================================================
//  <WarControlManager>
//
class WarControlManager : public IWorkNode
{
public:
    static const DWORD  INTERVALofUPDATE    = 15; // 15ms
    WarControlManager();
    virtual ~WarControlManager();
    static WarControlManager* Instance() { return ms_pWarControlManager; }
    //----------------------------------------------------------------------------------------------
    //  <IWorkNode Interface>
    static WORK_KEY GetWorkKey() { return ms_WorkKey; }
    static VOID     SetWorkKey(const WORK_KEY workKey) { ms_WorkKey = workKey; }
    virtual BOOL    DoInit();
    virtual BOOL    DoProcess();
    virtual BOOL    IsComplete();
    virtual BOOL    DoRelease();
    // Control Interface
    virtual VOID    OnMsg(sSIG_MSG* pSIG_MSG);

    //----------------------------------------------------------------------------------------------
    //  <INTERFACEs>
    VOID Update();
    VOID OnRecv(const MSG_BASE* const pMsg, const WORD wSize, const SSQSessionCmd::FromClient& pSession);
    VOID OnRecv(const MSG_BASE* const pMsg, const WORD wSize, const SSQSessionCmd::FromGame& pSession);
    VOID OnRecv(const MSG_BASE* const pMsg, const WORD wSize, const SSQSessionCmd::FromWorld& pSession);
    VOID Serialize();
    BOOL SendPacket(const MSG_BASE* pMsg, const WORD wSize);

    //----------------------------------------------------------------------------------------------
    //  <INTERFACEs>
    BOOLEAN IsEnableSSQField(const MAPCODE CodeOfSSQ);

    static VOID OnMsg(const SSQCtrlCmd::Created&, DungeonSSQ* const pDungeonSSQ);
    static VOID OnMsg(const SSQCtrlCmd::Destroy&, DungeonSSQ* const pDungeonSSQ);
    static VOID OnMsg(const SSQCtrlCmd::UserState&, DungeonSSQ* const pDungeonSSQ);
    static VOID OnMsg(const SSQCtrlCmd::UserEvent&, DungeonSSQ* const pDungeonSSQ);
    static const nsSSQ::SSQTicketInfo& GetTicketInfo();

private:
    SSQControlManagerImpl* m_pSSQImpl;
    static WORK_KEY ms_WorkKey;
    static WarControlManager* ms_pWarControlManager;
    friend struct SSQControlManagerImpl;
};

