#ifndef  __eXtreme_ServerSession_H
#define  __eXtreme_ServerSession_H

#include <eXtreme/io/EventQueue.h>
#include <eXtreme/util/CircuitQueue.h>
#include <eXtreme/util/Stream.h>
#include <eXtreme/threading/Mutex.h>

namespace eXtreme
{
	class ServerNetwork;
	class ServerSession : public EventHandler
	{
		friend class SessionManager;

	public:
				ServerSession( int sendBufSize, int recvBufSize );
		virtual	~ServerSession();

		//  Derived virtual function
		virtual	void			HandleEvent( AsyncResult* result );
		virtual	Socket*			GetHandle()							{	return &m_socket;						}
		virtual	char*			GetBuffer()							{	return 0;								}
		virtual	int				GetBufferSize()						{	return 0;								}

				void			Flush();

				void			Create();
				void			Create( SOCKET s, Socket::SocketAddr& addr );
				void			Close();

				void			Connect( char* ipAddress, unsigned short portNo );
				void			SetKill( DWORD err = 0 );

		virtual	bool			SendPacket( void* buffer, int size );
		virtual	bool			SendPacket( Stream& stream );

				char*			GetIPAddress()						{	return m_socket.GetIPAddress();			}
				DWORD			GetUID()							{	return m_uid;							}

				char*			GetSendBuffer()						{	return m_sendBuffer.GetReadPtr();		}
				char*			GetRecvBuffer()						{	return m_recvBuffer.GetWritePtr();		}
				DWORD			GetNextSendSize()					{	return m_sendBuffer.GetReadableSize();	}
				void			GetSendBufferSize( DWORD* remain, DWORD* max );
				void			GetRecvBufferSize( DWORD* remain, DWORD* max );

				DWORD			GetSendReadableSize()				{	return m_sendBuffer.GetReadableSize();	}
				DWORD			GetSendWritableSize()				{	return m_sendBuffer.GetWritableSize();	}

				DWORD			GetRecvReadableSize()				{	return m_recvBuffer.GetReadableSize();	}
				DWORD			GetRecvWritableSize()				{	return m_recvBuffer.GetWritableSize();	}

				void			SetMPU( DWORD size )				{	m_maxSizePerUpdate = size;				}
				DWORD			GetMPU()							{	return m_maxSizePerUpdate;				}


				AsyncResult		m_resultAccept, m_resultConnect, m_resultClose;
				AsyncResult		m_resultSend, m_resultRecv;

	protected:
		//  Member virtual function
		virtual	void			OnAccept()										{}
		virtual	void			OnConnect( bool /*success*/, DWORD /*error*/ )	{}
		virtual	void			OnSend( int /*size*/ )							{}
		virtual	void			OnReceive( char* /*buffer*/, int /*size*/ )		{}
		virtual	void			OnClose( DWORD /*error*/ )						{}
		virtual	void			Update();


		virtual	bool			IsValidPacket( char* ptr, DWORD recvBytes, DWORD* totalSize )
				{
					*totalSize = *( DWORD* )ptr;

					return ( recvBytes >= *totalSize );
				}

	private:
				void			Clear();
				void			WaitForRecv();


				DWORD			m_uid;
				Socket			m_socket;

				CircuitQueue	< char >	m_sendBuffer;
				CircuitQueue	< char >	m_recvBuffer;
				Mutex			m_mutexSend, m_mutexRecv;

				DWORD			m_sendBufSize, m_recvBufSize;
				bool			m_readyToSend;
				DWORD			m_sendCompletionSize, m_sendReqSize;
				char*			m_dequeueBuffer;
				DWORD			m_maxSizePerUpdate;

				ServerNetwork*	m_serverNetwork;
	};
}

#endif