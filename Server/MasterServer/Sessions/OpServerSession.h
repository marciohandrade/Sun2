#ifndef __OP_SERVER_SESSION_H__
#define __OP_SERVER_SESSION_H__

#pragma once

#include "ServerSessionEx.h"

class OpServerSession : public ServerSessionEx
{
public:
	OpServerSession() {};
	~OpServerSession() {};

	virtual eSERVER_TYPE		GetServerType() { return OP_SERVER; }

protected:
	//-------------------------------------------------------------------------------------------------
	// NetworkObject
	virtual VOID				OnConnect( BOOL bSuccess, DWORD dwNetworkIndex );
	virtual VOID				OnDisconnect();

	virtual	VOID				OnRecv( BYTE* pMsg, WORD wSize );

	virtual VOID				SendHeartbeatPacket() {}
protected:
	struct SendFunc
	{
		static VOID				AllServerInfo_OnLIVE( OpServerSession* pServerSession );
	};
    // CHANGES: f110617.1L
    virtual void OnRedirect(DWORD network_index, MSG_SERVER_TYPE* const first_contact_msg);
};

#endif //__OP_SERVER_SESSION_H__
