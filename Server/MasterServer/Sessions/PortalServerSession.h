#ifndef __PORTAL_SERVER_SESSION_H__
#define __PORTAL_SERVER_SESSION_H__

#pragma once
#if !defined(_NA002635_GP_DP_PROTOCOL_UINIFICATION)
#include "ServerSessionEx.h"

class PortalServerSession : public ServerSessionEx
{
public:
	PortalServerSession() {};
	~PortalServerSession() {};

	virtual eSERVER_TYPE		GetServerType() { return PORTAL_SERVER; }

public:
	VOID	OnRedirect();
protected:
};
#endif

#endif //__PORTAL_SERVER_SESSION_H__
