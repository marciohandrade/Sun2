#pragma once
#include "ServerSessionEx.h"

class AgentServerSession :
	public ServerSessionEx
{
	__DECL_CUSTOMPOOL_PTR( AgentServerSession )
public:
	AgentServerSession(void);
	virtual ~AgentServerSession(void);

	inline virtual eSERVER_TYPE	GetServerType()		{ return AGENT_SERVER; }
protected:
	virtual void OnRecv( BYTE * pMsg, WORD wSize );
};
