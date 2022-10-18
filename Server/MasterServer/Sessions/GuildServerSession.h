#ifndef __GUILD_SERVER_SESSION_H__
#define __GUILD_SERVER_SESSION_H__

#pragma once

#include "ServerSessionEx.h"

class GuildServerSession : public ServerSessionEx
{
public:
	GuildServerSession() {};
	~GuildServerSession() {};

	virtual eSERVER_TYPE		GetServerType() { return GUILD_SERVER; }

protected:
    // CHANGES: f110617.1L
    virtual void OnRedirect(DWORD network_index, MSG_SERVER_TYPE* const first_contact_msg);
};

#endif //__GUILD_SERVER_SESSION_H__
