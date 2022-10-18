#pragma once
#include "ServerSessionEx.h"

class MasterServerSession :
	public ServerSessionEx
{
	__DECL_CUSTOMPOOL_PTR( MasterServerSession )
public:
	MasterServerSession(void);
	virtual ~MasterServerSession(void);

	inline virtual eSERVER_TYPE	GetServerType()		{ return MASTER_SERVER; }
protected:
	virtual void OnRecv( BYTE * pMsg, WORD wSize );

public:
	VOID				Init();
	VOID				Release();

public:
	VOID				OnConnect( BOOL bSuccess, DWORD dwSessionIndex );
	VOID				OnDisconnect();
};
