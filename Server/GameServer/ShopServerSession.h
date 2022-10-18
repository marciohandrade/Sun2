#pragma once

#include <serversession.h>

#if !defined _NA_0_20110613_GAME_SHOP_RENEWAL

class ShopServerSession : public ServerSession
{
	__DECL_CUSTOMPOOL_PTR( ShopServerSession )
public:
	ShopServerSession(void);
	~ShopServerSession(void);

	eSERVER_TYPE			GetServerType() { return SHOP_SERVER;	}

protected:
    // CHANGES: f110617.1L
    virtual void OnRedirect(DWORD network_index, MSG_SERVER_TYPE* const first_contact_msg);
	virtual VOID			OnConnect( BOOL bSuccess, DWORD dwSessionIndex );
	virtual VOID			OnDisconnect();
	virtual VOID			OnRecv( BYTE * pMsg, WORD wSize );

	virtual BOOL			Send( BYTE *pMsg, WORD wSize );

	virtual void			Update(){}
};

#endif //#if !defined _NA_0_20110613_GAME_SHOP_RENEWAL
