#pragma once

#pragma warning(push)
#pragma warning(disable:4702)

#include <list>

#pragma warning(push)

#include <eXtreme/net/SessionManager.h>

#include "RTTService.h"

class ManagerService : public eXtreme::SessionManager
{
	Singleton( ManagerService );

public:
			bool						AddClient( RTTService* client );
			void						RemoveClient( RTTService* client );
			RTTService*					FindClient( char* ipAddress );

			void						BroadcastRTTAStart();
			void						BroadcastRTTAStop();
			void						Broadcast( char* buffer, int size );
			void						Broadcast( DWORD worldID, DWORD channelID, char* buffer, int size );
			void						Broadcast( DWORD worldID, char* buffer, int size );

			void						DisconnectAll();

protected:
	virtual	eXtreme::ServerSession*		CreateSession()
			{
				return new RTTService();
			}

	virtual	void						DeleteSession( eXtreme::ServerSession* session )
			{
				delete session;
			}

private:
	typedef	std::list	< RTTService* >		ListClient;

			ListClient						m_listClient;
};