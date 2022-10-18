#pragma once

#include <ServerSessions/ServerSessionEx.h>

class AuthAgentServerSession : public ServerSessionEx
{
public:
	AuthAgentServerSession();
	~AuthAgentServerSession();

	eSERVER_TYPE	GetServerType() { return AUTH_AGENT_SERVER;	}
    virtual void Update();

	virtual BOOL	SendPacket( MSG_BASE * pMsg, WORD wSize );

protected:
	virtual VOID	Init();	
	virtual VOID	Release();
    // CHANGES: f110617.1L
    virtual void OnRedirect(DWORD network_index, MSG_SERVER_TYPE* const first_contact_msg);
	virtual VOID	OnRecv( BYTE * pMsg, WORD wSize );
	virtual VOID	OnConnect( BOOL bSuccess, DWORD dwNetworkIndex );
	virtual VOID	OnDisconnect();
    virtual void SendHeartbeatPacket();

private:
    // _NA_002455_20110328_AUTH_HEART_BEAT
    void ClearHeartBeatResponse();
    util::Timer heart_beat_response_timer_;
    bool is_waiting_heart_beat_response_;
};
