#pragma once

#include <ServerSessions/ServerSessionEx.h>

class DBProxyServerSession : public ServerSessionEx
{
public:
	DBProxyServerSession();
	~DBProxyServerSession();
	
	eSERVER_TYPE		GetServerType() { return GAME_DBPROXY;	}
	
protected:
    // CHANGES: f110617.1L
    virtual void OnRedirect(DWORD network_index, MSG_SERVER_TYPE* const first_contact_msg);
	virtual VOID		OnConnect( BOOL bSuccess, DWORD dwNetworkIndex );
	virtual VOID		OnDisconnect();

protected:
	VOID				SendAllUsersInfo();
};

