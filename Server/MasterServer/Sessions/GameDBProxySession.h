#ifndef _GAMEDBPROXYSESSION_H_
#define _GAMEDBPROXYSESSION_H_

#include "ServerSessionEx.h"

//=================================================================================================
// Game DB Proxy�� ������ �޾� ó���ϱ� ���� Ŭ����
//=================================================================================================
class GameDBProxySession : public ServerSessionEx
{
public:
	GameDBProxySession() {};
	~GameDBProxySession() {};

	virtual eSERVER_TYPE		GetServerType() { return GAME_DBPROXY; }

protected:
    // CHANGES: f110617.1L
    virtual void OnRedirect(DWORD network_index, MSG_SERVER_TYPE* const first_contact_msg);
};

#endif //_GAMEDBPROXYSESSION_H_