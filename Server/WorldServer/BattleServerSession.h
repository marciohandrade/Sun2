#ifndef _BATTLESERVERSESSION_H_
#define _BATTLESERVERSESSION_H_

#pragma once

#include "GameServerSession.h"

class BattleServerSession : public GameServerSession
{
public:
	BattleServerSession();
	~BattleServerSession();

	VOID						Init();
	VOID						Release();
	VOID						Update();

	eSERVER_TYPE				GetServerType() { return BATTLE_SERVER;	}

	virtual VOID				OnRedirect();

protected:
	virtual VOID				OnConnect( BOOL bSuccess, DWORD dwNetworkIndex );
	virtual VOID				OnDisconnect();
	virtual VOID				OnRecv( BYTE * pMsg, WORD wSize );
};

#endif // _BATTLESERVERSESSION_H_