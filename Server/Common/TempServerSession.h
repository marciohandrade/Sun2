#pragma once
#include "serversession.h"

class TempServerSession :
	public ServerSession
{
	__DECL_CUSTOMPOOL_PTR( TempServerSession )
public:
	TempServerSession(void);
	virtual ~TempServerSession(void);

	inline virtual eSERVER_TYPE	GetServerType()		{ return TEMP_SERVER; }
protected:
	virtual void OnRecv( BYTE * pMsg, WORD wSize );
	virtual void Init();
private:
    // CHANGES: f110617.1L
    virtual void OnRedirect(DWORD network_index, MSG_SERVER_TYPE* const first_contact_msg);

	BOOL m_bFirst;
};

inline void TempServerSession::OnRedirect(DWORD, MSG_SERVER_TYPE* const)
{
    assert(!"this session can't use this method");
};