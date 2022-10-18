#pragma once

#include <eXtreme/net/SessionManager.h>
#include "Skel.h"

class SessionMngSkel : public eXtreme::SessionManager
{
public:
			SessionMngSkel()	{}
	virtual	~SessionMngSkel()	{}

protected:
	virtual	eXtreme::ServerSession*		CreateSession()
			{
				return new Skel();
			}

	virtual	void						DeleteSession( eXtreme::ServerSession* session )
			{
				delete session;
			}
};