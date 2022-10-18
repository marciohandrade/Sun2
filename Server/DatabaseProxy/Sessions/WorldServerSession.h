#pragma once
#include "ChildServerSession.h"

class WorldServerSession : public ChildServerSession
{
	__DECL_CUSTOMPOOL_PTR( WorldServerSession )
public:
	WorldServerSession(void);
	virtual ~WorldServerSession(void);

	eSERVER_TYPE		GetServerType() { return WORLD_SERVER;	}
    // CHANGES: f110617.1L
    virtual void OnRedirect(DWORD network_index, MSG_SERVER_TYPE* const first_contact_msg);

protected:
	VOID OnRecv( BYTE * pMsg, WORD wSize );
};
