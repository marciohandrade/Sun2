#pragma once

#include <eXtreme/net/SessionManager.h>
#include "Stub.h"

class SessionMngStub : public eXtreme::SessionManager
{
public:
			SessionMngStub()		{}
	virtual	~SessionMngStub()		{}

protected:
	virtual	eXtreme::ServerSession*		CreateSession()
			{
				return new Stub();
			}

	virtual	void						DeleteSession( eXtreme::ServerSession* session )
			{
				delete session;
			}
};