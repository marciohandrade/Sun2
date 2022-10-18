#ifndef  __eXtreme_SessionManager_H
#define  __eXtreme_SessionManager_H

#include <map>
#include <list>

#include <eXtreme/net/ServerSession.h>
#include <eXtreme/threading/Mutex.h>

namespace eXtreme
{
	class SessionManager
	{
		friend class ServerNetwork;

	public:
				ServerSession*		Find( DWORD uid );

				ServerSession*		BindSession( SOCKET s, Socket::SocketAddr* addr );
				bool				ActiveSession( ServerSession* session );
				void				InactiveSession( ServerSession* session );

				int					GetActiveSessionCnt()		{	return ( int )m_mapActive.size();	}
				int					GetInactiveSessionCnt()		{	return ( int )m_pool.size();		}

	protected:
				SessionManager();
		virtual	~SessionManager();

		virtual	ServerSession*		CreateSession();
		virtual	void				DeleteSession( ServerSession* session );

		virtual	void				Update();
		virtual	void				UpdateInactive();

	private:
				bool				Create( int sessionCnt );
				void				Destroy();


		typedef	std::map	< DWORD, ServerSession* >	MapActive;
		typedef std::list	< ServerSession* >			ListPool;

				MapActive			m_mapActive;
				ListPool			m_pool;
				Mutex				m_mutexActive, m_mutexPool;
				ServerNetwork*		m_serverNetwork;
	};
}

#endif