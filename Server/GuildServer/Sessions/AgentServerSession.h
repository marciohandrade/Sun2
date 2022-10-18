#pragma once

#include "ServerSessionEx.h"

class AgentServerSession : public ServerSessionEx
{
public:
	AgentServerSession(void);
	virtual ~AgentServerSession(void);

	inline virtual eSERVER_TYPE	GetServerType()		{ return AGENT_SERVER; }
private:
    // CHANGES: f110617.1L
    virtual void OnRedirect(DWORD network_index, MSG_SERVER_TYPE* const first_contact_msg);
};
