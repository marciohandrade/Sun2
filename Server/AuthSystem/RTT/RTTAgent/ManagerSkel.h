#pragma once

#pragma warning(push)
#pragma warning(disable:4702)

#include <list>

#pragma warning(pop)

#include <eXtreme/net/SessionManager.h>

#include "Skel.h"

class ManagerSkel : public eXtreme::SessionManager
{
	Singleton( ManagerSkel );

public:
			bool		AddClient( Skel* client );
			void		RemoveClient( Skel* client );
			void		Clear();

			void		MakeClientList( Skel* list[], DWORD bufferSize, DWORD* listCount );
			Skel*		FindClient( DWORD type );
			void		Broadcast( char* buffer, int size );

			void		DisconnectAll();

protected:
	virtual	eXtreme::ServerSession*		CreateSession()
			{
				return new Skel();
			}

	virtual	void						DeleteSession( eXtreme::ServerSession* session )
			{
				delete session;
			}

private:
	typedef	std::list	< Skel* >		ListClient;

			ListClient					m_listClient;
};