#pragma once

#include <ServerSessions/ServerSessionEx.h>

class MasterServerSession : public ServerSessionEx
{
public:
	MasterServerSession();
	~MasterServerSession();

	eSERVER_TYPE		GetServerType() { return MASTER_SERVER;	}

protected:
    // CHANGES: f110617.1L
    virtual void OnRedirect(DWORD network_index, MSG_SERVER_TYPE* const first_contact_msg);
	VOID				OnConnect( BOOL bSuccess, DWORD dwSessionIndex );
	VOID				OnDisconnect();
};

