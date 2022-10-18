#pragma once

#include <ServerSession.h>
#include "BillingServerCommonImpl.h"

//#ifdef _NEW_BILLING_SYSTEM
//
//struct MSG_BASE_BILLING;
//
//class BillingServerSession : public ServerSession
//{
//    enum { BILLING_SUN_CODE = 113, };
//public:
//    BillingServerSession(void);
//    ~BillingServerSession(void);
//
//    inline virtual eSERVER_TYPE GetServerType() { return BILLING_SERVER; }
//
//    BOOL    SendPacket(MSG_BASE_BILLING* pMsg, WORD wSize);
//    VOID    SendLogin(AccountInfo *pAccountInfo);
//    VOID    SendLogout(AccountInfo *pAccountInfo);
//
//protected:
//    VOID    OnConnect(BOOL bSuccess, DWORD dwNetworkIndex);
//    VOID    OnDisconnect();
//    VOID    OnRecv(BYTE* pMsg, WORD wSize);
//
//private:
//    VOID    OnBTOGExpireTermNTF(AccountInfo* pAccountInfo);
//    VOID    OnBTOGAlarmExpireTermNTF(AccountInfo* pAccountInfo, double dRestTime);
//    VOID    OnBS_BILLING_USER_STATUS_NTF(MSG_BASE_BILLING* pMsg, WORD wSize);
//
//    AccountInfo* FindLogoutWaitAccount(DWORD userGuid);
//
//private:
//    typedef    std::map<DWORD, AccountInfo*>  ACCOUNTINFO_MAP;
//    // OnUserStatus를 받기 전에 로그아웃을 시도하려는 유저 리스트
//    ACCOUNTINFO_MAP m_mapLogoutWaitAccount;
//};
//
//#else //!_NEW_BILLING_SYSTEM

struct MSG_BASE_BILLING;

class BillingServerSession : public ServerSession
{
    enum { BILLING_INFO_SEND_DELAY_TIME = 180000,
           BILLING_SUN_CODE = 2, };
public:
    BillingServerSession(void);
    ~BillingServerSession(void);

    inline virtual eSERVER_TYPE GetServerType() { return BILLING_SERVER; }


    void    SendLogin(AccountInfo& info);
    void    SendLogout(AccountInfo& info);

    BOOL    SendPacket(MSG_BASE_BILLING* pMsg, WORD wSize);

    //_PCBANG_POLICY_CHANGE_POST_EXPIRED2
    void    QueryRestPoint(AccountInfo* account);

protected:
    // CHANGES: f110617.1L, the sub-sessions can control first_contact_msg as the needs
    virtual void OnRedirect(DWORD session_index, MSG_SERVER_TYPE* const first_contact_msg);
    void    OnRecv(BYTE* pMsg, WORD wSize);
    void    OnConnect(BOOL bSuccess, DWORD dwNetworkIndex);
    void    OnDisconnect();

private:
    void    Update();

    void    MakeList();
    void    SendDeduction();

public: // for test
    void    OnBS_BILLING_EXPIRED_NTF(MSG_BASE_BILLING* pMsg, WORD size);
    void    OnBS_BILLING_EXPIRED_ALRAM_NTF(MSG_BASE_BILLING* pMsg, WORD size);
    //_PCBANG_POLICY_CHANGE_POST_EXPIRED2
    void    OnBS_BILLING_RESTPOINT_ACK(MSG_BASE_BILLING* pMsg, WORD size);
private:
    BillingServerCommonImpl BillingCommonImpl;
#ifdef _BILLING_CODE_REFACTORING_
    typedef STLX_DEQUE<AccountInfo*>    AccountInfoQueue;

    AccountInfoQueue m_AccountInfoPendings;
    AccountInfoQueue m_AccountInfoPool;
#else //!_BILLING_CODE_REFACTORING_
    typedef std::vector<AccountInfo*>   VectAccountInfo;
    typedef std::queue<AccountInfo*>    PoolAccountInfo;

    VectAccountInfo m_vectAccountInfo;
    PoolAccountInfo m_poolAccountInfo;
#endif //!_BILLING_CODE_REFACTORING_

    util::Timer m_BillingTimer;
};

//#endif //!_NEW_BILLING_SYSTEM

// CHANGES: f110617.1L, the sub-sessions can control first_contact_msg as the needs
inline void BillingServerSession::OnRedirect(DWORD, MSG_SERVER_TYPE* const)
{
    assert(!"this session can't use this method");
}

