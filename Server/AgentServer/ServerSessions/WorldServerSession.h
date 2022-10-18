#pragma once

#include <ServerSessions/ServerSessionEx.h>

class WorldServerSession : public ServerSessionEx
{
public:
	WorldServerSession();
	~WorldServerSession();

	eSERVER_TYPE		GetServerType() { return WORLD_SERVER;	}

protected:
    // CHANGES: f110617.1L
    virtual void OnRedirect(DWORD network_index, MSG_SERVER_TYPE* const first_contact_msg);
	VOID				OnConnect( BOOL bSuccess, DWORD dwSessionIndex );
	virtual VOID		OnDisconnect();
};
