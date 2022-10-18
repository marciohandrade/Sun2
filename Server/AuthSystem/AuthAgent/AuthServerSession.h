#pragma once

#include <serversession.h>

class AuthServerSession : public ServerSession
{
    __DECL_CUSTOMPOOL_PTR(AuthServerSession)
public:
    AuthServerSession(void);
    virtual ~AuthServerSession(void);

    inline virtual eSERVER_TYPE GetServerType() { return AUTH_SERVER; }

    virtual BOOL SendPacket(MSG_BASE* pMsg, WORD wSize) {
        SHOW_AUTH_PACKET_FLOW_TRACE(pMsg, wSize);
        return ServerSession::SendPacket(pMsg, wSize);
    }
    virtual BOOL SendPacket(MSG_BASE_INTERNAL* pMsg, WORD wSize) {
        SHOW_AUTH_INTER_PACKET_FLOW_TRACE(pMsg, wSize);
        return ServerSession::SendPacket(pMsg, wSize);
    } 

protected:
    // CHANGES: f110617.1L, the sub-sessions can control first_contact_msg as the needs
    virtual void OnRedirect(DWORD session_index, MSG_SERVER_TYPE* const first_contact_msg);
    virtual void OnRecv(BYTE* pMsg, WORD wSize);
    virtual void OnConnect(BOOL bSuccess, DWORD dwNetworkIndex);

private:
    void    OnSA_AUTH_AGENT_IP_LIST_NTF(MSG_BASE_INTERNAL* pMsg, WORD size);
    void    OnSA_AUTH_NEWCLIENT_TO_GAMEAGENT_SYN(MSG_BASE_INTERNAL* pMsg, WORD size);
    void    OnSA_AUTH_ASK_PC_LOGOUT_SYN(MSG_BASE_INTERNAL* pMsg, WORD size);
    void    OnSA_AUTH_BILLING_ALARM_CMD(MSG_BASE_INTERNAL* pMsg, WORD size);
#ifdef __NA001334_090420_SERVER_SELECT_FACILITY__
    void    OnAU_AUTH_SERVER_SELECT_ACK(MSG_BASE_INTERNAL* pMsg, WORD size);
#endif
    //__NA000000_SHOP_USERID_50_PASSING_PROC__
    void    OnAU_AUTH_COMMON_QUERY_USER_ID_ACK(MSG_BASE_INTERNAL* pMsg, WORD size);
    //_PCBANG_POLICY_CHANGE_POST_EXPIRED
    void    OnAU_AUTH_BILLING_CHANGE_CONTROL_SYN(MSG_BASE_INTERNAL* pMsg, WORD size);
};

// CHANGES: f110617.1L, the sub-sessions can control first_contact_msg as the needs
inline void AuthServerSession::OnRedirect(DWORD, MSG_SERVER_TYPE* const)
{
    assert(!"this session can't use this method");
};