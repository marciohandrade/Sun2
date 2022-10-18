#pragma once

#include <ServerSession.h>

//==================================================================================================

class GameAgentServerSession : public ServerSession
{
    __DECL_CUSTOMPOOL_PTR(GameAgentServerSession)
public:
    GameAgentServerSession(void);
    virtual ~GameAgentServerSession(void);

    inline virtual eSERVER_TYPE GetServerType() { return AGENT_SERVER; }

    virtual BOOL    SendPacket(MSG_BASE* pMsg, WORD wSize) {
        SHOW_AUTH_PACKET_FLOW_TRACE(pMsg, wSize);
        return ServerSession::SendPacket(pMsg, wSize);
    }
    virtual BOOL    SendPacket(MSG_BASE_INTERNAL* pMsg, WORD wSize) {
        SHOW_AUTH_INTER_PACKET_FLOW_TRACE(pMsg, wSize);
        return ServerSession::SendPacket(pMsg, wSize);
    }
    uint16_t GetGameAgentID() const;
    //void SetGameAgentID(uint16_t agent_id);

protected:
    // CHANGES: f110617.1L, the sub-sessions can control first_contact_msg as the needs
    virtual void OnRedirect(DWORD session_index, MSG_SERVER_TYPE* const first_contact_msg);
    virtual void    OnRecv(BYTE* pMsg, WORD wSize);
    virtual void    OnDisconnect();
    virtual void    OnAccept(DWORD dwNetworkIndex);

    // packet handler
private:
    // CHANGES: f101030.1L, moved from 'OnAccept' redirect logic
    void AuthenticateConnectedAgentServer(ulong ip_address, uint16_t listen_port);
#ifdef _NA000000_CONFIGURATION_TO_ACCEPT_MULTI_AGENT_ON_SAME_IP_ADDRESS
    // NOTE: f101030.1L, change the specification to accept multi-agent on same ip address
    // first sent protocol
    void    OnAU_AUTH_CONNECTOR_CHANGE_CONFIG_SYN(MSG_BASE_INTERNAL* pMsg, WORD size);
#endif
    void    OnGA_AUTH_NEWCLIENT_TO_GAMEAGENT_ACK(MSG_BASE_INTERNAL* pMsg, WORD size);
    void    OnGA_AUTH_PC_LOGIN_CMD(MSG_BASE_INTERNAL* pMsg, WORD size);
    void    OnGA_AUTH_PC_LOGOUT_CMD(MSG_BASE_INTERNAL* pMsg, WORD size);
    void    OnGA_AUTH_ASK_PC_LOGOUT_ACK(MSG_BASE_INTERNAL* pMsg, WORD size);
    void    OnGA_AUTH_BILLING_ALARM_ACK(MSG_BASE_INTERNAL* pMsg, WORD size);
    void    OnGA_AUTH_PC_NAME_NTF(MSG_BASE_INTERNAL* pMsg, WORD size);
    void    OnGA_AUTH_USER_COUNT_NTF(MSG_BASE_INTERNAL* pMsg, WORD size);
#ifdef __NA001334_090420_SERVER_SELECT_FACILITY__
    void    OnAU_AUTH_SERVER_SELECT_SYN(MSG_BASE_INTERNAL* pMsg, WORD size);
#endif
#ifdef __NA000000_SHOP_USERID_50_PASSING_PROC__
    void    OnAU_AUTH_COMMON_QUERY_USER_ID_SYN(MSG_BASE_INTERNAL* pMsg, WORD size);
#endif
    //
    uint16_t game_agent_id_;
};

//==================================================================================================

inline uint16_t GameAgentServerSession::GetGameAgentID() const {
    return game_agent_id_;
}

// CHANGES: f110617.1L, the sub-sessions can control first_contact_msg as the needs
inline void GameAgentServerSession::OnRedirect(DWORD, MSG_SERVER_TYPE* const)
{
    assert(!"this session can't use this method");
};
