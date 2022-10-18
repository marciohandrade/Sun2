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
    virtual void OnConnect( BOOL bSuccess, DWORD dwNetworkIndex );
    virtual VOID OnDisconnect();
    virtual void OnRecv(BYTE* pMsg, WORD wSize);

private:
    void OnSF_AUTH_GAME_VERSION_NTF(MSG_BASE_INTERNAL* pMsg, WORD size);
    void OnSF_AUTH_CONTROL_IPLIST_NTF(MSG_BASE_INTERNAL* pMsg, WORD size);
    void OnSF_AUTH_ID_PASSWORD_CHECK_ACK(MSG_BASE_INTERNAL* pMsg, WORD size);
    void OnSF_AUTH_SERVER_LIST_NTF(MSG_BASE_INTERNAL* pMsg, WORD size);
    void OnSF_AUTH_SERVER_SELECT_ACK(MSG_BASE_INTERNAL* pMsg, WORD size);
    void OnSF_AUTH_CLIENT_DISCONNECT_CMD(MSG_BASE_INTERNAL* pMsg, WORD size);
#ifdef _SECURE_AUTH
    void OnSF_AUTH_MATRIX_CARD_SYN(MSG_BASE_INTERNAL* pMsg, WORD size);
#endif
#ifdef _JP000000_ONETIMEPASSWORD_AUTHFLOW_
    void OnAU_AUTH_OTP_PASS_CMD(MSG_BASE_INTERNAL* pMsg, WORD size);
    void OnAU_AUTH_OTP_PASS_ACK(MSG_BASE_INTERNAL* pMsg, WORD size);
#endif
#ifdef __CN_20100909_SECURITY_TOKEN__	
	void OnAU_AUTH_SECURITY_TOKEN_ACK(MSG_BASE_INTERNAL* pMsg, WORD size);
#endif //__CN_20100909_SECURITY_TOKEN__	

//#ifdef _SHAPE_AUTH
//    void OnSF_AUTH_SHAPE_SYN(MSG_BASE_INTERNAL* pMsg, WORD size);
//#endif
};

// CHANGES: f110617.1L, the sub-sessions can control first_contact_msg as the needs
inline void AuthServerSession::OnRedirect(DWORD, MSG_SERVER_TYPE* const)
{
    assert(!"this session can't use this method");
};
