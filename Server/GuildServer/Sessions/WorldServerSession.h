#pragma once

#include "ServerSessionEx.h"

class WorldServerSession : public ServerSessionEx
{
public:
	WorldServerSession(void);
	virtual ~WorldServerSession(void);

	inline virtual eSERVER_TYPE	GetServerType()		{ return WORLD_SERVER; }
private:
    // CHANGES: f110617.1L
    virtual void OnRedirect(DWORD network_index, MSG_SERVER_TYPE* const first_contact_msg);
};
