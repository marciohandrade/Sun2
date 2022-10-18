#include <eXtreme/Common.h>
#include <eXtreme/eXtreme.h>
#include <eXtreme/net/SessionManager.h>
#include <eXtreme/threading/Guard.h>

using namespace eXtreme;

SessionManager::SessionManager()
	: m_serverNetwork( NULL )
{
}

SessionManager::~SessionManager()
{
}

bool SessionManager::Create( int sessionCnt )
{
	Guard	< Mutex >	guard( m_mutexPool );
	ServerSession*		session = NULL;

	for(int i = 0 ; i < sessionCnt ; i++ )
	{
		session						= CreateSession();
		session->m_serverNetwork	= m_serverNetwork;

		m_pool.push_back( session );
	}

	return true;
}

void SessionManager::Destroy()
{
	ServerSession*			session = NULL;

	{
		Guard	< Mutex >		guard( m_mutexPool );
	//	ListPool::iterator		iter;

	//	for( iter = m_pool.begin() ; iter != m_pool.end() ; iter++ )
	//	{
	//		session						= *iter;
	//		session->m_serverNetwork	= 0;
	//		DeleteSession( session );
	//	}

	//	m_pool.clear();
		
		while(!m_pool.empty()){
			session = m_pool.front();
			m_pool.pop_front();
			session->m_serverNetwork = NULL;
			DeleteSession( session );
		}
	}

	{
		Guard	< Mutex >		guard( m_mutexActive );

		for(MapActive::iterator iter = m_mapActive.begin() ; iter != m_mapActive.end() ; iter++ )
		{
			session						= iter->second;
			session->m_serverNetwork	= 0;

			DeleteSession( session );
		}

		m_mapActive.clear();
	}

	m_serverNetwork = NULL;
}

ServerSession* SessionManager::Find( DWORD uid )
{
	Guard	< Mutex >		guard( m_mutexActive );

	MapActive::iterator iter = m_mapActive.find( uid );
	if( iter != m_mapActive.end() )
		return iter->second;

	return 0;
}

ServerSession* SessionManager::BindSession( SOCKET s, Socket::SocketAddr* addr )
{
	ServerSession*		session = NULL;

	{
		Guard	< Mutex >	guard( m_mutexPool );

		if( m_pool.size() == 0 )
			return 0;

		ListPool::iterator iter = m_pool.begin();
		session = *iter;
		m_pool.erase( iter );
	}


	session->Create( s, *addr );

	{
		Guard	< Mutex >	guard( m_mutexActive );

		DWORD uid					= session->GetUID();
		m_mapActive[uid]	= session;
	}


	return session;
}

bool SessionManager::ActiveSession( ServerSession* session )
{
	{
		Guard	< Mutex >	guard( m_mutexPool );
		bool				isFind = false;

		for(ListPool::iterator iter = m_pool.begin() ; iter != m_pool.end() ; iter++ )
		{
			if( *iter == session )
			{
				m_pool.erase( iter );
				isFind = true;

				break;
			}
		}

		if( isFind == false )
		{
			return false;
		}
	}

	{
		Guard	< Mutex >	guard( m_mutexActive );

		DWORD uid					= session->GetUID();
		m_mapActive[uid]	= session;
	}

	return true;
}

void SessionManager::InactiveSession( ServerSession* session )
{
	{
		Guard	< Mutex >	guard( m_mutexActive );

		MapActive::iterator iter = m_mapActive.find( session->GetUID() );
		if( iter != m_mapActive.end() )
		{
			//  Active의 session을 삭제하고 Pool에 넣는다.
			m_mapActive.erase( iter );

			{
				Guard	< Mutex >	guard( m_mutexPool );

				m_pool.push_back( session );
			}
		}
	}
}

void SessionManager::Update()
{
	Guard	< Mutex >		guard( m_mutexActive );

	if( m_mapActive.size() == 0 )
	{
		return;
	}

	ServerSession*			session = NULL;

	for(MapActive::iterator iter = m_mapActive.begin() ; iter != m_mapActive.end() ; iter++ )
	{
		session = iter->second;
		session->Update();
	}
}

void SessionManager::UpdateInactive()
{
	Guard	< Mutex >		guard( m_mutexPool );

	if( m_pool.size() == 0 )
	{
		return;
	}

	ServerSession*			session = NULL;

	for(ListPool::iterator iter = m_pool.begin() ; iter != m_pool.end() ; iter++ )
	{
		session = *iter;
		session->Update();
	}
}

ServerSession* SessionManager::CreateSession()
{
	return ( _new_dbg_ ServerSession( 8192, 8192 ) );
}

void SessionManager::DeleteSession( ServerSession* session )
{
	delete session;
}