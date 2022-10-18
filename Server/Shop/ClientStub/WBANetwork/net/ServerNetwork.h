#ifndef  __WBANetwork_ServerNetwork_H
#define  __WBANetwork_ServerNetwork_H

#include <map>
#include <WBANetwork/net/Acceptor.h>
#include <WBANetwork/util/EventQueue.h>

#define  MAX_DISPATCHER_COUNT		16

namespace WBANetwork
{
	typedef struct tag_AsyncResult AsyncResult;
	class Connector;
	class SessionManager;
	class ServerNetwork : private Acceptor
	{
	public:
				ServerNetwork();
		virtual	~ServerNetwork();

		virtual	void			Update();

				bool			Create(WORD sessionCnt, SessionManager* sm, DWORD threadCnt = 2, DWORD connectCnt = 0);
				void			Destroy();

				bool			OpenAcceptor(char* ip, u_short portNo);
				void			CloseAcceptor();

				void			ConnectSession(ServerSession* s, const char* ipAddress, u_short portNo);
				bool			AddSessionEvent(ServerSession* s);
				void			PostCompletion(EventHandler* handler, AsyncResult* result);

				SessionManager*	GetSessionManager() { return m_pSessionManager; }

				void			InactiveSession(ServerSession* session);

	private:
		static unsigned int __stdcall Dispatcher(LPVOID parameter);
		virtual	void			OnAccept(SOCKET s, Socket::SocketAddr& addr);
				bool			CreateSessionManager(int sessionCnt);

		EventQueue				m_EventQueue;
		SessionManager*			m_pSessionManager;
		Connector*				m_pConnector;

		HANDLE					m_HandleDispatcher[MAX_DISPATCHER_COUNT];
		DWORD					m_ThreadCount;
	};
}

#endif