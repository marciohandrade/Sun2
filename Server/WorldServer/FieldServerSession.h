#ifndef _FIELDSERVERSESSION_H_
#define _FIELDSERVERSESSION_H_

#pragma once

#include "GameServerSession.h"

class FieldServerSession : public GameServerSession
{
public:
	FieldServerSession();
	~FieldServerSession();

	VOID						Init();
	VOID						Release();
	VOID						Update();

	eSERVER_TYPE				GetServerType() { return FIELD_SERVER;	}

	virtual VOID				OnRedirect();
	
protected:
	virtual VOID				OnConnect( BOOL bSuccess, DWORD dwNetworkIndex );
	virtual VOID				OnDisconnect();
	virtual VOID				OnRecv( BYTE *pMsg, WORD wSize );
};

#endif // _FIELDSERVERSESSION_H_