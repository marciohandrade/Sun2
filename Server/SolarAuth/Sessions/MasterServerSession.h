
#pragma once

#include "ServerSession.h"

class MasterServerSession : public ServerSessionEx
{
public:
	MasterServerSession();
	~MasterServerSession();

	virtual VOID				Init();
	virtual VOID				Release();
	virtual VOID				Update();
	virtual eSERVER_TYPE		GetServerType() { return MASTER_SERVER; }

	//virtual VOID				OnRedirect();

    // CHANGES: f110617.1L
    virtual void OnRedirect(DWORD network_index, MSG_SERVER_TYPE* const first_contact_msg);
protected:
	virtual VOID				OnAccept( DWORD dwNetworkIndex );
	virtual VOID				OnConnect( BOOL bSuccess, DWORD dwNetworkIndex );
	virtual VOID				OnDisconnect();
	virtual	VOID				OnRecv( BYTE* pMsg, WORD wSize );
public:
	virtual BOOL				SendPacket( MSG_BASE* pMsg, WORD wSize );

private:
	VOID						OnSendServerType();
	VOID						OnSendReqServerInfo();
};



