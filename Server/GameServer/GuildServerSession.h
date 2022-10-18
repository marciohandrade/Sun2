#pragma once
#if !defined(_NA002635_GZ_DZ_PROTOCOL_UINIFICATION)
#include <ServerSession.h>

class GuildServerSession :
	public ServerSession
{
	__DECL_CUSTOMPOOL_PTR( GuildServerSession )
public:
	GuildServerSession(void);
	virtual ~GuildServerSession(void);

	eSERVER_TYPE			GetServerType() { return GUILD_SERVER;	}

protected:
    // CHANGES: f110617.1L
    virtual void OnRedirect(DWORD network_index, MSG_SERVER_TYPE* const first_contact_msg);
	virtual VOID			OnConnect( BOOL bSuccess, DWORD dwSessionIndex );
	virtual VOID			OnDisconnect();
	virtual VOID			OnRecv( BYTE * pMsg, WORD wSize );
};
#endif //if !defined(_NA002635_GZ_DZ_PROTOCOL_UINIFICATION)