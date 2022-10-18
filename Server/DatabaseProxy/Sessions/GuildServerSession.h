#pragma once

#if !defined(_NA002635_GZ_DZ_PROTOCOL_UINIFICATION)

#include "ChildServerSession.h"

class GuildServerSession :
	public ChildServerSession
{
	__DECL_CUSTOMPOOL_PTR( GuildServerSession )
public:
	GuildServerSession(void);
	virtual ~GuildServerSession(void);

	eSERVER_TYPE		GetServerType() { return GUILD_SERVER;	}
    // CHANGES: f110617.1L
    virtual void OnRedirect(DWORD network_index, MSG_SERVER_TYPE* const first_contact_msg);
protected:
	VOID OnRecv( BYTE * pMsg, WORD wSize );
};

#endif //_NA002635_GZ_DZ_PROTOCOL_UINIFICATION