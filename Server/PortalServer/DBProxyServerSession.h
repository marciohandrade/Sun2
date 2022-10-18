#pragma once
#include "ServerSessionEx.h"

class DBProxyServerSession :
	public ServerSessionEx
{
	__DECL_CUSTOMPOOL_PTR( DBProxyServerSession )
public:
	DBProxyServerSession(void);
	virtual ~DBProxyServerSession(void);

	virtual void OnConnect( BOOL bSuccess, DWORD dwNetworkIndex );

	inline virtual eSERVER_TYPE	GetServerType()		{ return GAME_DBPROXY; }
protected:
	virtual void OnRecv( BYTE * pMsg, WORD wSize );
};
