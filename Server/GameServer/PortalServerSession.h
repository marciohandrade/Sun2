#pragma once

#if !defined(_NA002635_GP_DP_PROTOCOL_UINIFICATION)

#include <ServerSession.h>

class PortalServerSession :
	public ServerSession
{
	__DECL_CUSTOMPOOL_PTR( PortalServerSession )
public:
	PortalServerSession(void);
	virtual ~PortalServerSession(void);

	eSERVER_TYPE			GetServerType() { return PORTAL_SERVER;	}

protected:
	virtual VOID			OnConnect( BOOL bSuccess, DWORD dwSessionIndex );
	virtual VOID			OnDisconnect();
	virtual VOID			OnRecv( BYTE * pMsg, WORD wSize );
};

#endif //if !defined(_NA002635_GP_DP_PROTOCOL_UINIFICATION)