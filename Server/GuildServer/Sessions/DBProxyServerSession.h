#pragma once

#include "ServerSessionEx.h"

class DBProxyServerSession : public ServerSessionEx
{
public:
	DBProxyServerSession(void);
	virtual ~DBProxyServerSession(void);

	inline virtual eSERVER_TYPE	GetServerType()		{ return GAME_DBPROXY; }
private:
    // CHANGES: f110617.1L
    virtual void OnRedirect(DWORD network_index, MSG_SERVER_TYPE* const first_contact_msg);
};
