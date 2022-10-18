#pragma once

#include <serversession.h>

class AuthAgentServerSession :
	public ServerSession
{
public:
	AuthAgentServerSession(void);
	virtual ~AuthAgentServerSession(void);

	inline virtual eSERVER_TYPE	GetServerType() { return AUTH_AGENT_SERVER; }

    virtual BOOL	SendPacket(MSG_BASE* pMsg, WORD wSize) {
        SHOW_AUTH_PACKET_FLOW_TRACE(pMsg, wSize);
        return ServerSession::SendPacket(pMsg, wSize);
    }
    virtual BOOL	SendPacket(MSG_BASE_INTERNAL* pMsg, WORD wSize) {
        SHOW_AUTH_INTER_PACKET_FLOW_TRACE(pMsg, wSize);
        return ServerSession::SendPacket(pMsg, wSize);
    } 

protected:
    // CHANGES: f110617.1L, the sub-sessions can control first_contact_msg as the needs
    virtual void OnRedirect(DWORD session_index, MSG_SERVER_TYPE* const first_contact_msg);
	virtual void	OnRecv(BYTE* pMsg, WORD wSize);
	virtual void	OnDisconnect();
	
private:
	void	OnAS_AUTH_GAMEAGENT_LOGIN_CMD(MSG_BASE_INTERNAL* pMsg, WORD size);
	void	OnAS_AUTH_NEWCLIENT_TO_GAMEAGENT_ACK(MSG_BASE_INTERNAL* pMsg, WORD size);
	void	OnAS_AUTH_PC_LOGIN_CMD(MSG_BASE_INTERNAL* pMsg, WORD size);
	void	OnAS_AUTH_PC_LOGOUT_CMD(MSG_BASE_INTERNAL* pMsg, WORD size);
	void	OnAS_AUTH_ASK_PC_LOGOUT_ACK(MSG_BASE_INTERNAL* pMsg, WORD size);
	void	OnAS_AUTH_PC_NAME_NTF(MSG_BASE_INTERNAL* pMsg, WORD size);
	void	OnAS_AUTH_USER_COUNT_NTF(MSG_BASE_INTERNAL* pMsg, WORD size);
#ifdef __NA001334_090420_SERVER_SELECT_FACILITY__
    void    OnAU_AUTH_SERVER_SELECT_SYN(MSG_BASE_INTERNAL* pMsg, WORD size);
#endif
#ifdef __NA000000_SHOP_USERID_50_PASSING_PROC__
    void    OnAU_AUTH_COMMON_QUERY_USER_ID_SYN(MSG_BASE_INTERNAL* pMsg, WORD size);
#endif
};
