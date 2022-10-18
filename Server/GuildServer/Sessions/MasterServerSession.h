#pragma once

#include "ServerSessionEx.h"

class MasterServerSession : public ServerSessionEx
{
public:
	MasterServerSession(void);
	virtual ~MasterServerSession(void);

	inline virtual eSERVER_TYPE	GetServerType()		{ return MASTER_SERVER; }

public:
    // CHANGES: f110617.1L
    virtual void OnRedirect(DWORD network_index, MSG_SERVER_TYPE* const first_contact_msg);
	VOID				OnConnect( BOOL bSuccess, DWORD dwSessionIndex );
	VOID				OnDisconnect();
};
