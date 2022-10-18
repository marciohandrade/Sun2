#pragma once

#include <serverframe.h>
#include "SALib_dll.h"
#include <Singleton.h>

#include <WxMessageIpc.h>

class RTTAServerSession;

#ifdef __NA_20100201_INTERGRATEDBILLING_
class IntegratedBillingServerSession;
#else
class BillingServerSession;
#endif

class LoginFrontServerSession;
struct DB_CONTROL_IP_INFO;

#if !defined(_AUTH_GLOBAL)
enum {
    AUTH_READY_GAMEVERSION,
    AUTH_READY_CONTROL_IPLIST,
    AUTH_READY_RECOVER_STS,
    AUTH_READY_MAX
};

#elif defined(_AUTH_GLOBAL)
enum {
    AUTH_READY_GAMEVERSION,
    AUTH_READY_CONTROL_IPLIST,
    AUTH_READY_MAX
};
#else
#endif

#ifdef _RU_INGAMBA_AUTH_PROCESS
class IngambaAuthConnector;
#endif

//==================================================================================================

class AuthFrame : public ServerFrame, public util::Singleton<AuthFrame>
{
public:
    AuthFrame();
    ~AuthFrame();

    static NetworkObject*   DoAllocServerSession();
    static VOID DoFreeServerSession(NetworkObject* pNetObject);
    static VOID DoFreeConnectSession(NetworkObject* pNetObject);

    static NetworkObject*   DoAllocClient_NotUsed(){ return NULL; }
    static VOID DoFreeClient_NotUsed(NetworkObject* pNetObject) { __UNUSED(pNetObject); }
    static VOID DoFreeConnect_NotUsed(NetworkObject* pNetObject) { __UNUSED(pNetObject); }

    virtual eSERVER_TYPE    GetServerType() { return AUTH_SERVER; }

    BOOL IsRTTAServerConnected();
    BOOL IsBillingConnected();

    BOOL SendToSession(DWORD idx, MSG_BASE_INTERNAL* pMsg, WORD wSize);
    VOID SendToSessionType(eSERVER_TYPE type, MSG_BASE_INTERNAL* pMsg, WORD wSize, DWORD dwExceptIndex=0);
    BOOL SendRTTAServer(MSG_BASE_INTERNAL* pMsg, WORD wSize);

    BOOL ConnectTo(eSERVER_TYPE type);
    ServerSession* GetSession(eSERVER_TYPE type);

    bool IsAllowSts(USER_STS sts);
    void ReloadAllowedSetting();

#ifdef _SECURE_AUTH
    void SendMatrixCardAuth(LoginFrontServerSession* pSession, AccountInfo* pAccount, DWORD dwFrontUserID, BYTE btMatrixAuthFailCount, BYTE bFirst);
#endif

//#ifdef _SHAPE_AUTH
//    void ReloadShapeImage();
//    void SendShapeAuthImage(LoginFrontServerSession* pSession, AccountInfo* pAccount, DWORD dwFrontUserID, BYTE btShapeAuthFailCount, BYTE bFirst);
//#endif

    VOID SendGameVersion_ToLoginFront(DWORD dwServerIndex = 0);

    VOID AddControlIp(DB_CONTROL_IP_INFO* pDBIPInfo);
    VOID ReleaseControlIp();
    VOID SendControlIp_ToLoginFront(DWORD dwServerIndex = 0);

    DWORD GetAuthKey() { return m_AuthKey; }
    DWORD GetRecoverTimeout() { return m_RecoverTimeout; }
    DB_INFO* GetDBInfo(DB_TYPE Type) { return &m_DBInfo[Type]; }

    //----------------------------------------------------------------------------------------------
    // (Properties) (WAVERIX) (090830) (CHANGES) __PROPERTY 대체
    DWORD GetBlockLimit() const { return m_BlockLimit; }
    DWORD GetBlockTime() const { return m_BlockTime; }
    DWORD GetAuthLimitTime() const { return m_AuthLimitTime; }
    BOOLEAN IsChangePcRoomBillingStatus() const { return m_ChangePcRoomBillingStatus; }

#ifdef _RU_INGAMBA_AUTH_PROCESS
    IngambaAuthConnector* GetIngambaAuthConnector() {return ingamba_auth_connector_;}
#endif
#ifdef __NA_20100201_INTERGRATEDBILLING_
    util::WxMessageIpc* GetBillingIpc();
#endif

protected:
    virtual BOOL Init();
    virtual BOOL Update(DWORD tick);
    virtual void Release();

    virtual BOOL KeyEvent(CHAR ch);
    VOID DisplayServerInfo();
    VOID DisplayVersionInfo();
    VOID DisplayControlIPInfo();

    virtual ServerSession* AllocServerSession(eSERVER_TYPE);
    virtual void FreeServerSession(ServerSession*);

    virtual void SendServerType(ServerSession* pSession) { __UNUSED(pSession); }

    BOOL CreateSunLog();

private:
    typedef util::PoolRouter<MAX_SERVER>    SERVER_SESSION_ROUTER;
    SERVER_SESSION_ROUTER*  m_pPoolRouter;

    RTTAServerSession*      m_pRTTAServerSession;
#ifdef __NA_20100201_INTERGRATEDBILLING_
    IntegratedBillingServerSession*   m_pBillingServerSession;
    util::WxMessageIpc* billing_ipc_;
#else
    BillingServerSession*   m_pBillingServerSession;
#endif  

#ifdef _RU_INGAMBA_AUTH_PROCESS
    IngambaAuthConnector* ingamba_auth_connector_;
#endif


    DWORD   m_BlockLimit;  // 연속 인증 실패 계정 블록 회수
    DWORD   m_BlockTime;   // 계정 블록 시간(분 단위)
    DWORD   m_AuthLimitTime;
    BOOLEAN m_ChangePcRoomBillingStatus; // _PCBANG_POLICY_CHANGE_POST_EXPIRED 테스트 목적

    __REF_PROPERTY(GAME_VERSION, VerProtocol);
    __REF_PROPERTY(GAME_VERSION, VerLauncher);
    __REF_PROPERTY(GAME_VERSION, VerMajor);
    __REF_PROPERTY(GAME_VERSION, VerMinor);
//#ifdef _SHAPE_AUTH
// __REF_PROPERTY(RMSA_dll, ShapeMgr);    // 아래서 사용하는 매크로는 include 폴더에 CommonDef.h 파일에 있다
//#endif

    enum { MAX_ALLOWEDSTS_RELOAD_DELAY_TIME = 10*60*1000, };
    bool        m_allowSts[256];
    util::Timer m_AllowedStsTimer;
    util::Timer m_ReloadSATimer;

    DWORD m_AuthKey;
    DWORD m_RecoverTimeout;

    DB_INFO m_DBInfo[DB_TYPE_MAX];

    // 차단/허용 IP 정보
    typedef std::list<CONTROL_IP_INFO *>        ListControlIp;
    ListControlIp m_listControlIp;
};


#ifdef __NA_20100201_INTERGRATEDBILLING_
inline util::WxMessageIpc* AuthFrame::GetBillingIpc()
{
    return billing_ipc_;
};
#endif




