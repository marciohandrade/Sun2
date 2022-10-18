#pragma once

#include <serversession.h>

class RTTAServerSession :
	public ServerSession
{
public:
	RTTAServerSession(void);
	virtual ~RTTAServerSession(void);

	inline virtual eSERVER_TYPE	GetServerType() { return OP_SERVER; }

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
	virtual void	OnConnect(BOOL bSuccess, DWORD dwNetworkIndex);
	
private:
	void	SendCertifyReq();

	// 패킷 핸들러
	void	OnRS_RTT_CERTIFY_ACK(MSG_BASE_INTERNAL* pMsg, WORD size);
	void	OnRS_RTT_ASK_PC_LOGOUT_SYN(MSG_BASE_INTERNAL* pMsg, WORD size);
	void	OnRS_RTT_ASK_CONTROL_IPLIST_RELOAD_SYN(MSG_BASE_INTERNAL* pMsg, WORD size);
	void	OnRS_RTT_ASK_GAME_VERSION_RELOAD_SYN(MSG_BASE_INTERNAL* pMsg, WORD size);
};

// CHANGES: f110617.1L, the sub-sessions can control first_contact_msg as the needs
inline void RTTAServerSession::OnRedirect(DWORD, MSG_SERVER_TYPE* const)
{
    assert(!"this session can't use this method");
};