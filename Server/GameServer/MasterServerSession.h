#ifndef _MASTERSERVERSESSION_H_
#define _MASTERSERVERSESSION_H_

#include <ServerSession.h>
#include "RemoteServerSession.h"

class MasterServerSession : public RemoteServerSession
{
	__DECL_CUSTOMPOOL_PTR( MasterServerSession )
public:
	MasterServerSession();
	~MasterServerSession();

	eSERVER_TYPE			GetServerType() { return MASTER_SERVER;	}

private:
    // CHANGES: f110617.1L
    virtual void OnRedirect(DWORD network_index, MSG_SERVER_TYPE* const first_contact_msg);
	VOID					OnConnect( BOOL bSuccess, DWORD dwSessionIndex );
	VOID					OnDisconnect();
	VOID					OnRecv( BYTE * pMsg, WORD wSize );

#ifdef __WAVERIX_MAYBE_UNUSED_SECTION__
private:
	std::string				m_strConnectIP;
	WORD					m_wConnectPort;
#endif
};

#endif //_MASTERSERVERSESSION_H_