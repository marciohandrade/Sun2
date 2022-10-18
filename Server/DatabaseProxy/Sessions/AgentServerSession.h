#ifndef __AGENT_SERVER_SESSION_H__
#define __AGENT_SERVER_SESSION_H__

#pragma once

#include "ChildServerSession.h"

class AgentServerSession : public ChildServerSession
{
	__DECL_CUSTOMPOOL_PTR( AgentServerSession )
public:
	AgentServerSession();
	virtual ~AgentServerSession();

	eSERVER_TYPE		GetServerType() { return AGENT_SERVER;	}

protected:
    virtual void OnRedirect(DWORD network_index, MSG_SERVER_TYPE* const first_contact_msg);
	virtual void		OnDisconnect();
	VOID				OnRecv( BYTE * pMsg, WORD wSize );
	
};

#endif // __AGENT_SERVER_SESSION_H__
