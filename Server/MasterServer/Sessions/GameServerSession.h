#ifndef _GAMESERVERSESSION_H_
#define _GAMESERVERSESSION_H_

#pragma once

#include "ServerSessionEx.h"

class GameStatisticsData;

class GameServerSession : public ServerSessionEx
{
public:
	GameServerSession() {};
	~GameServerSession() {};

	eSERVER_TYPE				GetServerType() { return GAME_SERVER;	}

	// <INTERFACEs>
	virtual VOID				Init();
	virtual VOID				Release();

public:
	GameStatisticsData*			GetStatisticsData() { return m_pStatisticsData; }

protected:
    // CHANGES: f110617.1L
    virtual void OnRedirect(DWORD network_index, MSG_SERVER_TYPE* const first_contact_msg);

	GameStatisticsData*			m_pStatisticsData;
};

#endif // _GAMESERVERSESSION_H_