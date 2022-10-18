#pragma once

#include "UserInfo.Constants.h"

//////////////////////////////////////////////////////////////////////////
// <User에 연결된 LinkedServer 관리>
class ServerSessionEx;
class UserSession;
class User;

class LinkedServerSession
{
private:
	User*					m_pUser;
	ServerSessionEx*		m_pServerSession;
	DWORD					m_dwSessionIndex;
	//ITimeout				m_Timeout;
public:
	LinkedServerSession() : m_pUser(NULL), m_pServerSession(NULL), m_dwSessionIndex(0) {}
	LinkedServerSession(UserSession* pUser, DWORD dwServerSessionIndex);

    inline User*            GetLinkedUser() const { return m_pUser; }
    inline ServerSessionEx* GetLinkedServer() const { return m_pServerSession; }
    inline DWORD            GetLinkedServerSessionIndex() const { return m_dwSessionIndex; }
    // <SESSION CONTROL>
    BOOL        LinkUserWithServer(UserSession* pUser, DWORD dwServerSessionIndex);
    BOOL        LinkUserWithServer(UserSession* pUser, ServerSessionEx* pServerSession);
    inline void UnLinkServer() { m_pUser = NULL; m_pServerSession = NULL; m_dwSessionIndex = 0; }
    //
    BOOL        SendToServer(MSG_BASE_FORWARD* pMsg, WORD wSize);
private:
    BOOL        IsAcceptableUser(UserSession* pUser, BOOL bLogging = FALSE);
};

struct ClosingUserTransInfo
{
	bool is_transaction_;
	MAPCODE map_code_;
	FIELDCODE field_code_;
	WzVector user_pos_;
};

class ClosingUserTransController
{
public:
    ClosingUserTransController(ClosingUserTransInfo& trans_info) 
        : closing_user_trans_info_(trans_info) 
    {

    }

    inline void Init() 
    {
        closing_user_trans_info_.is_transaction_ = FALSE;
    }

    inline void EndTransaction() 
    {
        Init();
    }

    inline bool SetClosingUserTransaction(MAPCODE map_code, 
                                          FIELDCODE field_code, 
                                          const WzVector& user_pos)
    {
        bool is_success = true;
        is_success = is_success && !!(closing_user_trans_info_.map_code_ = map_code);
        is_success = is_success && !!(closing_user_trans_info_.field_code_ = field_code);
        closing_user_trans_info_.user_pos_ = user_pos;

        return closing_user_trans_info_.is_transaction_ = is_success;
    }

    inline bool IsTransaction() const 
    {
        return closing_user_trans_info_.is_transaction_;
    }
    
    inline MAPCODE GetMapCode() const 
    {
        return closing_user_trans_info_.map_code_;
    }
    
    inline FIELDCODE GetFieldCode() const 
    {
        return closing_user_trans_info_.field_code_;
    }
    
    inline WzVector& GetRandomPos() const 
    {
        return closing_user_trans_info_.user_pos_;
    }

private:
    ClosingUserTransInfo&	closing_user_trans_info_;

    ClosingUserTransController();
    __DISABLE_COPY(ClosingUserTransController);
};


struct PartyReturnInfo
{
    WzVector    ReturnPos;
    FIELDCODE   ReturnFieldCode;
    eZONETYPE   ReturnZoneType;
    KEYTYPE     ReturnZoneKey;
    void Init()
    {
        ReturnPos.x			= 0;
        ReturnPos.y			= 0;
        ReturnPos.z			= 0;
        ReturnFieldCode		= 0;
        ReturnZoneType		= eZONETYPE_MAX;
        ReturnZoneKey		= 0;
    }
};

//━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
//	<ISSQHandlerD>
//	-	SSQ Handler for User
//	-	특정 캐릭터에 대해서 SSQ 처리를 담당하는 처리 Proxy.
//	<Usage>
//	-	ISSQHandlerD& rSSQH;
//	-	if(rSSQH.IsAvailable())
//	-		rSSQH->(HandlerInterface)();
class User;
class ISSQHandler;

class ISSQHandlerD
{
public:
    ISSQHandlerD() : m_pUser(0), m_pISSQHandler(0) {}
    ISSQHandlerD(const ISSQHandlerD& ref) : m_pUser(ref.m_pUser), m_pISSQHandler(ref.m_pISSQHandler) {}
    ISSQHandlerD(User* const pUser, ISSQHandler* const pHandler) : m_pUser(pUser), m_pISSQHandler(pHandler) {}
    ~ISSQHandlerD() { m_pUser = 0; m_pISSQHandler = 0; }

	inline BOOLEAN  IsAvailable() const { return m_pUser && m_pISSQHandler; }

	inline User*    GetUser() const { return m_pUser; }
	inline ISSQHandler* GetHandler() const { return m_pISSQHandler; }
	inline ISSQHandler* const operator->() { return m_pISSQHandler; }
private:
    User*           m_pUser;
    ISSQHandler*    m_pISSQHandler;
};


class PortalUserInfo
{
public:
    enum eCommand {
        eCommand_CreatePortal,
        eCommand_ChangePortal,
        eCommand_EnterPortal,
        eCommand_ClosePortal,
        eCommand_GetState,
        eCommand_UserClose,
    };
    enum eState {
        eState_Success,
        // 세부 예외 처리 제어는 차후에 하기로 하고 일단 실패로만 처리해보자.
        eState_ChangedPortal,
        eState_Transaction,
        eState_NoRelevant,      // 포탈 생성 이후 파티 변화시 해당 사항 없는 경우
        //eState_DifferentZone,
        //eState_TooLong,...이것은 처리할지 여부 확인 필요
        //eState_DisabledAction,  // for disconnect
        //eState_NotFoundPortal,  // for in close process
        eState_Fail,        // User == 0
    };

    PortalUserInfo()
        : m_pUser(0)
        , m_PortalKey(INVALID_PORTAL_KEY)
        , m_State(eState_Fail)
    {}
    ~PortalUserInfo();

    inline void SetUser(User* pUser)
    {
        m_pUser = pUser;
        m_PortalKey = INVALID_PORTAL_KEY;
        m_State = eState_Fail;
    }

    inline PORTALKEY GetPortalKey() const { return m_PortalKey; }
    BOOLEAN IsValidState();
    eState TryChangeState(eCommand command, PORTALKEY portalKey, PORTALKEY* OUT prevPortalKey);

private:
    User*       m_pUser;
    PORTALKEY   m_PortalKey;
    eState      m_State;
};

#ifdef __NA001334_090420_SERVER_SELECT_FACILITY__
namespace nsPacket {

struct ReAuthInfo : public ReAuthInfoBase
{
public:
    enum eReAuthSlot {
        eReAuthSlot_AuthServerSide,
        eReAuthSlot_GameServerSide,
        eReAuthSlot_Count,      // upperbound
        eReAuthSlot_AllSide,    // special command for User Control
    };
    typedef eReAuthStep ReAuthStepSlot[eReAuthSlot_Count];

    ReAuthInfo();
    ~ReAuthInfo();

    void Clear();
    inline ReAuthKeyType GetReAuthKey() const { return m_ReAuthKey; }
    inline BOOLEAN IsExpired() { return m_Timeout.IsExpired(); };

    inline const ReAuthStepSlot& CurrentStep() const { return m_ReAuthSteps; }
    inline const ReAuthTokenStream* GetReAuthStream() const { return m_pReAuthStream; }

private:
    //inline BOOLEAN ChangeStep(eReAuthSlot where, eReAuthStep step);
    void SetReAuthStream(const ReAuthTokenStream* arg);

    BOOLEAN m_bUseReAuth;
    ReAuthKeyType m_ReAuthKey;
    ITimeout m_Timeout;
    ReAuthStepSlot m_ReAuthSteps;  // Client가 서버 처리보다 먼저 이뤄지는 상황 지연 처리 목적
    ReAuthTokenStream* m_pReAuthStream;

    friend class User;
};


//inline BOOLEAN
//ReAuthInfo::ChangeStep(eReAuthSlot where, eReAuthStep step)
//{
//    BOOLEAN bAccept = where < eReAuthSlot_Count;
//    if(bAccept == false)
//        return false;
//
//    bAccept = step < eReAuthStep_Count;
//    if(bAccept == false)
//        return false;
//
//    m_ReAuthSteps[where] = step;
//    return true;
//}

}; //end of namespace
#endif

#if !defined _NA_0_20110613_GAME_SHOP_RENEWAL

#ifdef __NA000000_SHOP_USERID_50_PASSING_PROC__
struct MSG_CG_SHOP_SEND_GIFT_SYN;

class ShopUserQuerySlot
{
public:
    ShopUserQuerySlot()
        : transaction_(0)
        , message_(0)
    {}

    ~ShopUserQuerySlot() { assert(message_ == 0); }

    BOOLEAN QueryStart(const MSG_CG_SHOP_SEND_GIFT_SYN* message);
    BOOLEAN IsTransaction() const { return transaction_; }
    const MSG_CG_SHOP_SEND_GIFT_SYN* GetMessage() { return message_; }
    void QueryEnd();

private:
    BOOLEAN transaction_;
    MSG_CG_SHOP_SEND_GIFT_SYN* message_;
};
#endif

#endif //#if !defined _NA_0_20110613_GAME_SHOP_RENEWAL
