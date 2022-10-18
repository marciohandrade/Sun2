#ifndef _ACCOUNTDBPROXYSESSION_H_
#define _ACCOUNTDBPROXYSESSION_H_

#include "ServerSession.h"

class FunctionMap;

//=================================================================================================
// Account DB Proxy에 connect하기 위한 세션 클래스
//=================================================================================================
class AccountDBProxySession : public ServerSession
{
public:
	AccountDBProxySession();
	~AccountDBProxySession();

	virtual VOID				Init();
	virtual VOID				Release();
	virtual VOID				Update();
	virtual eSERVER_TYPE		GetServerType() { return ACCOUNT_DBPROXY; }

protected:
	virtual VOID				OnAccept( DWORD dwNetworkIndex );
	virtual VOID				OnConnect( BOOL bSuccess, DWORD dwNetworkIndex );
	virtual VOID				OnDisconnect();
	virtual	VOID				OnRecv( BYTE *pMsg, WORD wSize );

private:
	VOID						SendReqServerInfo();
};

#endif // _ACCOUNTDBPROXYSESSION_H_