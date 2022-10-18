#ifndef  __eXtreme_ServerNetwork_H
#define  __eXtreme_ServerNetwork_H

#include <eXtreme/net/Acceptor.h>
#include <eXtreme/net/SessionManager.h>
#include <eXtreme/net/Connector.h>

#include <map>

#define  MAX_DISPATCHER_COUNT		16

namespace eXtreme
{
	class ServerNetwork : private Acceptor
	{
	public:
				ServerNetwork();
		virtual	~ServerNetwork();

		virtual	void			Update();

				bool			Create( WORD sessionCnt, SessionManager* sm, DWORD threadCnt = 2 );
				void			Destroy();

				bool			OpenAcceptor( char* ip, u_short portNo );
				void			CloseAcceptor();

				void			ConnectSession( ServerSession* s, char* ipAddress, u_short portNo );
				bool			AddSessionEvent( ServerSession* s );

				void			PostCompletion( EventHandler* handler, AsyncResult* result );

	private:		
		//static	DWORD	WINAPI	Dispatcher( LPVOID parameter );
		static unsigned int __stdcall Dispatcher( LPVOID parameter );

		virtual	void			OnAccept( SOCKET s, Socket::SocketAddr& addr );

				bool			CreateSessionManager( int sessionCnt );


				EventQueue		m_eventQueue;
				SessionManager*	m_sessionManager;
				Connector		m_connector;

				HANDLE			m_handleDispatcher[MAX_DISPATCHER_COUNT];
				DWORD			m_threadId;
				DWORD			m_threadCount;
	};
}

#endif