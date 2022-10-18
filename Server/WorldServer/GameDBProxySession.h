#ifndef _GAMEDBPROXYSESSION_H_
#define _GAMEDBPROXYSESSION_H_

#include "ServerSession.h"

class FunctionMap;

//=================================================================================================
// Game DB Proxy�� ������ �޾� ó���ϱ� ���� Ŭ����
//=================================================================================================
class GameDBProxySession : public ServerSession
{
public:
	GameDBProxySession();
	~GameDBProxySession();

	virtual VOID				Init();
	virtual VOID				Release();
	virtual VOID				Update();
	virtual eSERVER_TYPE		GetServerType() { return GAME_DBPROXY; }

	virtual VOID				OnRedirect();

protected:
	virtual VOID				OnConnect( BOOL bSuccess, DWORD dwNetworkIndex );
	virtual VOID				OnDisconnect();
	virtual	VOID				OnRecv( BYTE *pMsg, WORD wSize );

private:
};

#endif //_GAMEDBPROXYSESSION_H_