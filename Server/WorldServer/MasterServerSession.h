#pragma once

#include "ServerSession.h"

class MasterServerSession : public ServerSession
{
public:
	MasterServerSession();
	~MasterServerSession();

	virtual VOID				Init();
	virtual VOID				Release();
	virtual VOID				Update();
	virtual eSERVER_TYPE		GetServerType() { return MASTER_SERVER; }

	virtual VOID				OnRedirect();

protected:
	virtual VOID				OnConnect( BOOL bSuccess, DWORD dwNetworkIndex );
	virtual VOID				OnDisconnect();
	virtual	VOID				OnRecv( BYTE *pMsg, WORD wSize );

private:
	VOID						OnSendServerType();
	VOID						OnSendReqServerInfo();
};

