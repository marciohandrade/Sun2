#pragma once

#include <ServerSession.h>

struct MSG_CF_AUTH_ID_PASSWORD_CHECK_SYN;
struct MSG_CF_AUTH_TOKEN_PASS_SYN;
struct MSG_CF_AUTH_REAUTH_PASS_SYN;
struct MSG_CF_AUTH_TOKEN_NORTHA_PASS_SYN;
struct MSG_CF_AUTH_TOKEN_GLOBAL_PASS_SYN;
struct MSG_CF_AUTH_TOKEN_JP_GAMECHU_SYN;
struct MSG_CF_AUTH_PASS_RU_INGAMBA_SYN;
struct MSG_CF_AUTH_ZY_PASSWORD_CHECK_SYN;

class ClientSession : public ServerSession
{
    __DECL_CUSTOMPOOL_PTR(ClientSession)
public:
    ClientSession(void);
    virtual ~ClientSession(void);

    virtual void Init();
    virtual void Release();
    virtual void Update();

    TCHAR* GetAccountID();
    void SetAccountID(TCHAR* account_id);

    void UnlockAuthQueryTransaction();  //__NA000000_090701_AUTH_QUERY_TRANSACTION__
    BOOLEAN IsAuthQueryTransaction();   //__NA000000_090701_AUTH_QUERY_TRANSACTION__

protected:
    // CHANGES: f110617.1L, the sub-sessions can control first_contact_msg as the needs
    virtual void OnRedirect(DWORD session_index, MSG_SERVER_TYPE* const first_contact_msg);
    virtual VOID OnDisconnect();
    virtual VOID OnAccept(DWORD dwNetworkIndex);
    virtual VOID OnRecv(BYTE* pMsg, WORD wSize);
    virtual eSERVER_TYPE GetServerType();

private:
    static DWORD MakeFrontId(DWORD loginFrontID);

    // 패킷 핸들러 ------------------------------------------------------------------------------
    void OnCF_AUTH_REQ_PATCH_VER_SYN(MSG_BASE_INTERNAL* pMsg, WORD size);
    void OnCF_AUTH_VERIFY_SYN(MSG_BASE_INTERNAL* pMsg, WORD size);

#if WAVERIX_AUTH_TEST_CODE || AUTH_TEST_CODE
public: //for test
#endif

//@brief: SubRoutine in 'OnCF_AUTH_ID_PASSWORD_CHECK_SYN'
    void OnCF_AUTH_AUTHENTICATION_SYN(MSG_BASE_INTERNAL* pMsg, WORD size);
    BOOLEAN Handling(const MSG_CF_AUTH_ID_PASSWORD_CHECK_SYN* recv_msg, WORD msg_size);
#ifdef __NA001334_090420_SERVER_SELECT_FACILITY__
    BOOLEAN Handling(const MSG_CF_AUTH_REAUTH_PASS_SYN* recv_msg, WORD msg_size);
#endif
#ifdef _JP_0_20100723_GAMECHU_AUTHFLOW_
    BOOLEAN Handling(const MSG_CF_AUTH_TOKEN_JP_GAMECHU_SYN* recv_msg, WORD msg_size);    
#endif
#ifdef _CHANNELING_AUTH_FLOW
    BOOLEAN Handling(const MSG_CF_AUTH_TOKEN_PASS_SYN* recv_msg, WORD msg_size);
#endif
#ifdef __NA000000_GLOBAL_AUTH_PROCESS__
    BOOLEAN Handling(const MSG_CF_AUTH_TOKEN_GLOBAL_PASS_SYN* recv_msg, WORD msg_size);
#endif
#ifdef __NA000000_NORTH_AMERICA_HANGAME_CHANNELING__
    BOOLEAN Handling(const MSG_CF_AUTH_TOKEN_NORTHA_PASS_SYN* recv_msg, WORD msg_size);
#endif
#ifdef _RU_INGAMBA_AUTH_PROCESS
    BOOLEAN Handling(const MSG_CF_AUTH_PASS_RU_INGAMBA_SYN* recv_msg, WORD msg_size);
#endif
#ifdef _CN_ZY_AUTH_PROCESS
    BOOLEAN Handling(const MSG_CF_AUTH_ZY_PASSWORD_CHECK_SYN* pRecvMsg, WORD size);
#endif //_CN_ZY_AUTH_PROCESS

    void OnCF_AUTH_SERVER_LIST_SYN(MSG_BASE_INTERNAL* pMsg, WORD size);
    void OnCF_AUTH_SERVER_SELECT_SYN(MSG_BASE_INTERNAL* pMsg, WORD size);
#ifdef _SECURE_AUTH
    void OnCF_AUTH_MATRIX_CARD_ACK(MSG_BASE_INTERNAL* pMsg, WORD size);
#endif
#ifdef _JP000000_ONETIMEPASSWORD_AUTHFLOW_
    void OnCF_AUTH_OTP_PASS_SYN(MSG_BASE_INTERNAL* pMsg, WORD size);
#endif
#ifdef __CN_20100909_SECURITY_TOKEN__
	void OnCF_AUTH_SECURITY_TOKEN_SYN(MSG_BASE_INTERNAL* pMsg, WORD size);
#endif

//#ifdef _SHAPE_AUTH
//  void OnCF_AUTH_SHAPE_ACK(MSG_BASE_INTERNAL* pMsg, WORD size);
//#endif
//#else
//  BOOLEAN OnCF_AUTH_ID_PASSWORD_CHECK_SYN(MSG_CF_AUTH_ID_PASSWORD_CHECK_SYN* pMsg, WORD size);
//#endif

private:
    util::Timer m_DisconnectTimer;
    TCHAR m_szAccountID[ID_MAX_LEN + 1];
    char m_szLocalIP[IP_MAX_LEN];
    bool m_CorrectVersion;
    BOOLEAN m_AuthQueryTransaction; //__NA000000_090701_AUTH_QUERY_TRANSACTION__
    __PROPERTY(DWORD, FrontKey);
};

inline TCHAR* 
ClientSession::GetAccountID() 
{ 
    return m_szAccountID; 
}

inline void 
ClientSession::SetAccountID(TCHAR* account_id) 
{ 
    ::_tcsncpy(m_szAccountID, account_id, _countof(m_szAccountID));
    m_szAccountID[_countof(m_szAccountID)-1] = '\0';
}

inline void 
ClientSession::UnlockAuthQueryTransaction() 
{ 
    m_AuthQueryTransaction = false; 
}

inline BOOLEAN 
ClientSession::IsAuthQueryTransaction() 
{ 
    return m_AuthQueryTransaction; 
}

inline eSERVER_TYPE 
ClientSession::GetServerType() 
{ 
    return CLIENT_SERVER; 
}

// CHANGES: f110617.1L, the sub-sessions can control first_contact_msg as the needs
inline void ClientSession::OnRedirect(DWORD, MSG_SERVER_TYPE* const)
{
    assert(!"this session can't use this method");
};
