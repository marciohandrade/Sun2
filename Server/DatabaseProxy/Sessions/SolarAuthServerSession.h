#ifndef __SOLARAUTH_SERVER_SESSION_H__
#define __SOLARAUTH_SERVER_SESSION_H__

#pragma once

#include "ChildServerSession.h"

class SolarAuthServerSession : public ChildServerSession
{
	__DECL_CUSTOMPOOL_PTR( SolarAuthServerSession )
public:
	SolarAuthServerSession();
	virtual ~SolarAuthServerSession();

	eSERVER_TYPE		GetServerType() { return SOLARAUTH_SERVER;	}
    // CHANGES: f110617.1L
    virtual void OnRedirect(DWORD network_index, MSG_SERVER_TYPE* const first_contact_msg);
	virtual VOID		OnConnect( BOOL bSuccess, DWORD dwNetworkIndex );
private:
	VOID				OnRecv( BYTE * pMsg, WORD wSize );
};

#endif // __SOLARAUTH_SERVER_SESSION_H__