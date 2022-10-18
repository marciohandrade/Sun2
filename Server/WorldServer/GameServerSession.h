#ifndef _GAMESERVERSESSION_H_
#define _GAMESERVERSESSION_H_

#pragma once

#include "ServerSession.h"

class GameServerSession : public ServerSession
{
public:
	GameServerSession();
	~GameServerSession();

	VOID						Init();
	VOID						Release();
	VOID						Update();

	eSERVER_TYPE				GetServerType() { return BATTLE_SERVER;	}

	virtual VOID				OnRedirect();

protected:
	virtual VOID				OnConnect( BOOL bSuccess, DWORD dwNetworkIndex );
	virtual VOID				OnDisconnect();
	virtual VOID				OnRecv( BYTE *pMsg, WORD wSize );

private:
	VOID						SendDBProxyServerInfo();
};

#endif // _GAMESERVERSESSION_H_