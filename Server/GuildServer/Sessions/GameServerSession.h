#pragma once

#include "ServerSessionEx.h"

class GameServerSession	: public ServerSessionEx
{
public:
	GameServerSession(void);
	virtual ~GameServerSession(void);
	inline virtual eSERVER_TYPE	GetServerType()		{ return GAME_SERVER; }
protected:
    // CHANGES: f110617.1L
    virtual void OnRedirect(DWORD network_index, MSG_SERVER_TYPE* const first_contact_msg);
	virtual VOID	OnLogString( char *pszLog );
};