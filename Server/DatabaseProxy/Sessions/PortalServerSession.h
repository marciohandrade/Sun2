#pragma once

#if !defined(_NA002635_GP_DP_PROTOCOL_UINIFICATION)
#include "ChildServerSession.h"

class PortalServerSession :
	 public ChildServerSession
{
	__DECL_CUSTOMPOOL_PTR( PortalServerSession )
public:
	PortalServerSession(void);
	~PortalServerSession(void);

	eSERVER_TYPE		GetServerType() { return PORTAL_SERVER;	}
protected:
	VOID OnRecv( BYTE * pMsg, WORD wSize );
};
#endif //if !defined(_NA002635_GP_DP_PROTOCOL_UINIFICATION)