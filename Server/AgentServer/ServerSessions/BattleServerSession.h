#pragma once

#include "GameServerSession.h"

class BattleServerSession : public GameServerSession
{
public:
	BattleServerSession();
	~BattleServerSession();

	eSERVER_TYPE	GetServerType() { return BATTLE_SERVER;	}

public:
	virtual VOID	Init();

//---------------------------------------
// 아래부터는 개선할 것
    DWORD GetNumberOfRooms() const;
    void increasingRoom();
    void decreasingRoom();

private:
    // CHANGES: f110617.1L
    virtual void OnRedirect(DWORD network_index, MSG_SERVER_TYPE* const first_contact_msg);

    DWORD m_dwNumberOfRooms;
};


inline DWORD BattleServerSession::GetNumberOfRooms() const {
    return m_dwNumberOfRooms;
}

inline void BattleServerSession::increasingRoom()
{
    ++m_dwNumberOfRooms;
}

inline void BattleServerSession::decreasingRoom()
{   // CHANGES: f110721.4L, fixed type underflow problems,
    // this problem may confuse BattleServer load balancing.
    if (m_dwNumberOfRooms) {
        --m_dwNumberOfRooms;
    }
}
