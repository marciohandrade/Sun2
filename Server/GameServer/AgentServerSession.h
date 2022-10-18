#ifndef __AGENT_SERVER_SESSION_H__
#define __AGENT_SERVER_SESSION_H__

#include <ServerSession.h>

class AgentServerSession : public ServerSession
{
	__DECL_CUSTOMPOOL_PTR( AgentServerSession )
public:
	AgentServerSession();
	~AgentServerSession();

	eSERVER_TYPE			GetServerType() { return AGENT_SERVER;	}

protected:
    // CHANGES: f110617.1L
    virtual void OnRedirect(DWORD network_index, MSG_SERVER_TYPE* const first_contact_msg);
	virtual VOID			OnConnect( BOOL bSuccess, DWORD dwSessionIndex );
	virtual VOID			OnDisconnect();
	virtual VOID			OnRecv( BYTE * pMsg, WORD wSize );
	virtual VOID			OnLogString( char *pszLog );

	virtual BOOL			Send( BYTE *pMsg, WORD wSize );
};

#endif //__AGENT_SERVER_SESSION_H__