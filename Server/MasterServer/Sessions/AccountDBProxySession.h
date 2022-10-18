#ifndef _ACCOUNTDBPROXYSESSION_H_
#define _ACCOUNTDBPROXYSESSION_H_

#include "ServerSessionEx.h"

//=================================================================================================
// Account DB Proxy에 connect하기 위한 세션 클래스
//=================================================================================================
class AccountDBProxySession : public ServerSessionEx
{
public:
	AccountDBProxySession()	{};
	~AccountDBProxySession()	{};

	virtual eSERVER_TYPE		GetServerType() { return ACCOUNT_DBPROXY; }

protected:
	//-------------------------------------------------------------------------------------------------
	// NetworkObject
    // CHANGES: f110617.1L
    virtual void OnRedirect(DWORD network_index, MSG_SERVER_TYPE* const first_contact_msg);
	virtual VOID				OnConnect( BOOL bSuccess, DWORD dwNetworkIndex );
	virtual VOID				OnDisconnect();

protected:
	struct SendFunc
	{
		static VOID				RequestServerInfo( AccountDBProxySession* pServerSession );
	};
};

#endif // _ACCOUNTDBPROXYSESSION_H_