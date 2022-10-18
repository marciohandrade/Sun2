#pragma once
#include "ServerSessionEx.h"

class HandlerSet;

class GameServerSession :
	public ServerSessionEx
{
	__DECL_CUSTOMPOOL_PTR( GameServerSession )
public:
	GameServerSession(void);
	virtual ~GameServerSession(void);

	inline virtual eSERVER_TYPE	GetServerType()		{ return GAME_SERVER; }
protected:
	virtual void OnRecv( BYTE * pMsg, WORD wSize );
};
