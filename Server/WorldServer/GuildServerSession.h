#ifndef _GUILDSERVERSESSION_H_
#define _GUILDSERVERSESSION_H_

#pragma once
#if !defined(_NA002635_GZ_DZ_PROTOCOL_UINIFICATION)
#include "ServerSession.h"

class GuildServerSession : public ServerSession
{
public:
	GuildServerSession();
	~GuildServerSession();

	VOID						Init();
	VOID						Release();
	VOID						Update();

	eSERVER_TYPE				GetServerType() { return GUILD_SERVER;	}

	virtual VOID				OnRedirect();

protected:
	virtual VOID				OnConnect( BOOL bSuccess, DWORD dwNetworkIndex );
	virtual VOID				OnDisconnect();
	virtual VOID				OnRecv( BYTE *pMsg, WORD wSize );
};
#endif
#endif // _GUILDSERVERSESSION_H_