#pragma once

#pragma warning(push)
#pragma warning(disable:4702)

#include <list>
#include <map>

#pragma warning(pop)

#include <eXtreme/net/SessionManager.h>

#include "RTTAgent.h"


class ManagerClient : public eXtreme::SessionManager
{
	Singleton( ManagerClient );

public:
			bool		AddClient( RTTAgent* client );
			void		RemoveClient( RTTAgent* client );

			void		AddMasterServerIP(DWORD dwWorldID, char* pIP);

			void		MakeClientList( RTTAgent* list[], DWORD bufferSize, DWORD* listCount );
			RTTAgent*	FindClient( DWORD type );
			RTTAgent*	FindClient( char* ipAddress );
			RTTAgent*	FindClient( DWORD dwWorldID, DWORD dwChannelID, DWORD dwServerType, DWORD dwServerID );
			void		Broadcast( char* buffer, int size );
			void		Broadcast( DWORD worldID, DWORD channelID, char* buffer, int size );
			void        Broadcast( DWORD worldID, DWORD channelID, DWORD serverType, DWORD serverID, char* buffer, int size );

			bool        SendToMasterClient( MSG_HEADER8& packet, DWORD dwWorldID = 0 );
			bool        SendToMasterClient( eXtreme::Stream& stream, DWORD dwWorldID = 0 );
			
			void		DisconnectAll();

protected:
	virtual	eXtreme::ServerSession*		CreateSession()
			{
				return new RTTAgent();
			}

	virtual	void						DeleteSession( eXtreme::ServerSession* session )
			{
				delete session;
			}

private:
	typedef	std::list	< RTTAgent* >		ListClient;

			ListClient						m_listClient;


	eXtreme::Mutex							m_mutexMasterClient;

	typedef	std::map	< DWORD, char* >	MapMasterClient;
			MapMasterClient					m_mapMasterClientIP;
};