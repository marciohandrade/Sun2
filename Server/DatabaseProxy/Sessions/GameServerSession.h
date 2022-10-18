#ifndef __GAME_SERVER_SESSION_H__
#define __GAME_SERVER_SESSION_H__

#pragma once

#include "ChildServerSession.h"

class QueryResult;

class GameServerSession : public ChildServerSession
{
	__DECL_CUSTOMPOOL_PTR( GameServerSession )
public:
	GameServerSession();
	virtual ~GameServerSession();
    // CHANGES: f110617.1L
    virtual void OnRedirect(DWORD network_index, MSG_SERVER_TYPE* const first_contact_msg);

	eSERVER_TYPE		GetServerType() { return GAME_SERVER;	}
	void				OnRecv( BYTE * pMsg, WORD wSize );

protected:
	virtual void		OnDisconnect();
};

#endif // __GAME_SERVER_SESSION_H__
