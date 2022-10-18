#pragma once

#include <ServerSessions/ServerSessionEx.h>

class GameServerSession : public ServerSessionEx
{
public:
	GameServerSession();
	~GameServerSession();

	eSERVER_TYPE	GetServerType() { return BATTLE_SERVER;	}

protected:
	virtual VOID	Init();
	virtual VOID	Release();

protected:
	virtual VOID	OnDisconnect();

protected:
	VOID			RemoveRelatedUsers();
};

