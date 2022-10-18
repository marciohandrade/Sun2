#ifndef __MASTER_SERVER_SESSION_H__
#define __MASTER_SERVER_SESSION_H__

#pragma once

#include "ChildServerSession.h"

class MasterServerSession : public ChildServerSession
{
	__DECL_CUSTOMPOOL_PTR( MasterServerSession )
public:
	MasterServerSession();
	virtual ~MasterServerSession();

	eSERVER_TYPE		GetServerType() { return MASTER_SERVER;	}

    // CHANGES: f110617.1L
    virtual void OnRedirect(DWORD network_index, MSG_SERVER_TYPE* const first_contact_msg);
	virtual VOID		OnConnect( BOOL bSuccess, DWORD dwNetworkIndex );
	virtual VOID		OnDisconnect();
private:
	VOID				OnRecv( BYTE * pMsg, WORD wSize );
};

#endif // __MASTER_SERVER_SESSION_H__