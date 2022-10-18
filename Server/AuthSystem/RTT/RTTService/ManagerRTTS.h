#pragma once

#include <eXtreme/net/SessionManager.h>
#include "RTTServer.h"

class ManagerRTTS : public eXtreme::SessionManager
{
public:
			ManagerRTTS()		{}
	virtual	~ManagerRTTS()		{}

protected:
	virtual	eXtreme::ServerSession*		CreateSession()
			{
				return new RTTServer();
			}

	virtual	void						DeleteSession( eXtreme::ServerSession* session )
			{
				delete session;
			}

	virtual	void				Update()
			{
				SessionManager::Update();
				SessionManager::UpdateInactive();
			}
};