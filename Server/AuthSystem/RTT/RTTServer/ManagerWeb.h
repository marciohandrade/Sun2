#pragma once

#pragma warning(push)
#pragma warning(disable:4702)

#include <list>

#pragma warning(push)

#include <eXtreme/net/SessionManager.h>

#include "Web.h"

class ManagerWeb : public eXtreme::SessionManager
{
	Singleton( ManagerWeb );

public:
			bool						AddClient( Web* client );
			void						RemoveClient( Web* client );

			void						DisconnectAll();

protected:
	virtual	eXtreme::ServerSession*		CreateSession()
			{
				return new Web();
			}

	virtual	void						DeleteSession( eXtreme::ServerSession* session )
			{
				delete session;
			}

private:
	typedef	std::list	< Web* >		ListClient;

			ListClient					m_listClient;
};