#ifndef _BATTLESERVERSESSION_H_
#define _BATTLESERVERSESSION_H_

#pragma once

#include "GameServerSession.h"

class BattleServerSession : public GameServerSession
{
public:
	BattleServerSession() {};
	~BattleServerSession() {};

	eSERVER_TYPE				GetServerType() { return BATTLE_SERVER;	}

};

#endif // _BATTLESERVERSESSION_H_