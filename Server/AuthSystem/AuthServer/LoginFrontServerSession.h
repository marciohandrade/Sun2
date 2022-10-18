#pragma once

#include <serversession.h>

class LoginFrontServerSession : public ServerSession
{
public:
    LoginFrontServerSession(void);
    ~LoginFrontServerSession(void);
    virtual eSERVER_TYPE GetServerType();
    void SendServerList();

protected:
    // CHANGES: f110617.1L, the sub-sessions can control first_contact_msg as the needs
    virtual void OnRedirect(DWORD session_index, MSG_SERVER_TYPE* const first_contact_msg);
    void OnRecv(BYTE * pMsg, WORD wSize);
    void OnDisconnect();
    void Update();

//packet handler -----------------------------------------------------------------------------------
//Handling------------------------------------------------------------------------------------------
    void    OnFS_AUTH_AUTHENTICATION_SYN(MSG_BASE_INTERNAL* pMsg, WORD size);
    BOOLEAN Handling(const MSG_FS_AUTH_ID_PASSWORD_CHECK_SYN* recv_msg, WORD msg_size);
#ifdef _CHANNELING_AUTH_FLOW
    BOOLEAN Handling(const MSG_FS_AUTH_TOKEN_PASS_SYN* recv_msg, WORD msg_size);
#endif
#ifdef __NA000000_GLOBAL_AUTH_PROCESS__
    BOOLEAN Handling(const MSG_FS_AUTH_TOKEN_GLOBAL_PASS_SYN* recv_msg, WORD msg_size);
#endif
#ifdef __NA001334_090420_SERVER_SELECT_FACILITY__
    BOOLEAN Handling(const MSG_FS_AUTH_REAUTH_PASS_SYN* recv_msg, WORD msg_size);
#endif
#ifdef __NA000000_NORTH_AMERICA_HANGAME_CHANNELING__
    BOOLEAN Handling(const MSG_FS_AUTH_TOKEN_NORTHA_PASS_SYN* recv_msg, WORD msg_size);
#endif
#ifdef _JP_0_20100723_GAMECHU_AUTHFLOW_
    BOOLEAN Handling(const MSG_FS_AUTH_TOKEN_JP_GAMECHU_SYN* recv_msg, WORD msg_size);
#endif
#ifdef _RU_INGAMBA_AUTH_PROCESS
    BOOLEAN Handling(const MSG_FS_AUTH_PASS_RU_INGAMBA_SYN* recv_msg, WORD msg_size);
#endif
#ifdef _CN_ZY_AUTH_PROCESS
    BOOLEAN Handling(const MSG_FS_AUTH_ZY_PASSWORD_CHECK_SYN* recv_msg, WORD msg_size);
#endif
//--------------------------------------------------------------------------------------------------
    void    OnFS_AUTH_SERVER_SELECT_SYN(MSG_BASE_INTERNAL* pMsg, WORD size);
    void    OnFS_AUTH_CLIENT_DISCONNECT_CMD(MSG_BASE_INTERNAL* pMsg, WORD size);
#ifdef _SECURE_AUTH
    void    OnFS_AUTH_MATRIX_CARD_ACK(MSG_BASE_INTERNAL* pMsg, WORD size);
#endif
#ifdef _JP000000_ONETIMEPASSWORD_AUTHFLOW_
    void OnAU_AUTH_OTP_PASS_SYN(MSG_BASE_INTERNAL* pMsg, WORD size);
#endif
#ifdef __CN_20100909_SECURITY_TOKEN__
	void OnAU_AUTH_SECURITY_TOKEN_SYN(MSG_BASE_INTERNAL* pMsg, WORD size);
#endif
//  void    OnFS_AUTH_ID_PASSWORD_CHECK_SYN(MSG_BASE_INTERNAL* pMsg, WORD size);
//#ifdef _SHAPE_AUTH
//  void    OnFS_AUTH_SHAPE_ACK(MSG_BASE_INTERNAL* pMsg, WORD size);
//#endif

private:
    enum { SERVER_LIST_DELAY_TIME = 10*1000 };
    util::Timer m_ServerListTimer;

};

inline eSERVER_TYPE 
LoginFrontServerSession::GetServerType() 
{ 
    return LOGIN_FRONT_SERVER; 
}
