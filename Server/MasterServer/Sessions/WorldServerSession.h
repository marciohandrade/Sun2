#ifndef __WORLD_SERVER_SESSION_H__
#define __WORLD_SERVER_SESSION_H__

#pragma once

#include "ServerSessionEx.h"

class WorldServerSession : public ServerSessionEx
{
public:
	WorldServerSession() {};
	~WorldServerSession() {};

	virtual eSERVER_TYPE		GetServerType() { return WORLD_SERVER; }

protected:
    // CHANGES: f110617.1L
    virtual void OnRedirect(DWORD network_index, MSG_SERVER_TYPE* const first_contact_msg);
};

#endif //__WORLD_SERVER_SESSION_H__
