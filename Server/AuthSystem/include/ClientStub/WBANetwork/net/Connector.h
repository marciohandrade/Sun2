#ifndef  __WBANetwork_Connector_H
#define  __WBANetwork_Connector_H

#include <queue>
#include <WBANetwork/WBANetwork.h>
#include <WBANetwork/util/Pool.h>
#include <WBANetwork/util/Mutex.h>
#include <WBANetwork/util/Thread.h>

#define  MAX_CONNECT_COUNT		16

namespace WBANetwork
{
	class EventQueue;
	class ServerSession;
	class Connector : public Thread
	{
	public:
				Connector();
		virtual	~Connector();

				bool		Create(EventQueue* eq, DWORD connectCnt);
				void		Destroy();

				void		Connect(ServerSession* s, const char* ipAddress, unsigned short portNo);

	protected:
		virtual	void		Run();

	private:
		typedef	struct _tagConnectInfo
		{
			ServerSession*	session;

			char			ipAddress[20];
			unsigned short	portNo;
		} ConnectInfo;

		EventQueue*						m_pEventQueue;

		std::queue	<ConnectInfo* >		m_Queue;
		Pool		<ConnectInfo >		m_Pool;

		Mutex							m_MutexQueue;
		Mutex							m_MutexPool;

		HANDLE							m_eventKill;
		HANDLE							m_HandleConnecter[MAX_CONNECT_COUNT];
		DWORD							m_ThreadCount;
	};
}

#endif