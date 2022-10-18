#ifndef _GAMEDBPROXYSESSION_H_
#define _GAMEDBPROXYSESSION_H_

#include <ServerSession.h>

class GameDBProxySession : public ServerSession
{
	__DECL_CUSTOMPOOL_PTR( GameDBProxySession )
public:
	GameDBProxySession();
	~GameDBProxySession();

	eSERVER_TYPE			GetServerType() { return GAME_DBPROXY;	}

protected:
    // CHANGES: f110617.1L
    virtual void OnRedirect(DWORD network_index, MSG_SERVER_TYPE* const first_contact_msg);
	virtual VOID			OnConnect( BOOL bSuccess, DWORD dwSessionIndex );
	virtual VOID			OnDisconnect();
	virtual VOID			OnRecv( BYTE * pMsg, WORD wSize );
};

#endif //_GAMEDBPROXYSESSION_H_