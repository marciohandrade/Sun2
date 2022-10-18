#ifndef __BATTLE_SERVER_H__
#define __BATTLE_SERVER_H__


#pragma once

#include "GameServerEx.h"


class BattleServer : public GameServerEx
{
public:
	BattleServer();
	virtual ~BattleServer();

	eSERVER_TYPE GetServerType() { return BATTLE_SERVER; }
private:

	virtual BOOL	Init();
//	virtual void	Update( DWORD tick );
	virtual void	Release();

	virtual BOOL	KeyEvent( CHAR ch );

};


#endif // __BATTLE_SERVER_H__