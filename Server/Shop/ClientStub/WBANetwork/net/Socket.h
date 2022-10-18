#ifndef  WBANETWORK_SOCKET_H_
#define  WBANETWORK_SOCKET_H_

#include <Winsock2.h>

namespace WBANetwork
{
	typedef struct tag_AsyncResult AsyncResult;
	class Socket
	{
	public:
		typedef	sockaddr_in SocketAddr;

		enum ProtocolType
		{
			Protocol_None = 0,
			Protocol_TCP,
			Protocol_UDP
		};

	public:
		Socket();
		~Socket();

		bool				Create(ProtocolType type, bool overlapped = false);
		void				Close();

		bool				Attach(ProtocolType type, SOCKET handle, SocketAddr* addr);
		SOCKET				Accept(DWORD ms, SocketAddr* addr);
		bool				Connect(char* ipAddress, unsigned short portNo);
		bool				Bind(char* ipAddress, unsigned short portNo);
		bool				Listen(int backLogCount = SOMAXCONN);
		DWORD				SetKeepAlive(u_long onoff, u_long KeepaliveTime, u_long Keepaliveinterval);

		int					Send(PBYTE buffer, int length, AsyncResult* result);
		int					Recv(PBYTE buffer, int bufSize, AsyncResult* result);


		HANDLE				GetNativeHandle();
		SocketAddr*			GetSocketAddr()		{ return &m_SockAddr;		}
		char*				GetIPAddress()		{ return m_szIpAddress;		}
		void				GetBufferSize(int* sndBufSize, int* rcvBufSize);

		void				SetReuseAddr();
		void				SetIoctlSocket();
		void				SetLinger(bool flag);
		void				SetKeepAlive(bool flag);
		void				SetBufferSize(int sndBufSize, int rcvBufSize);

		bool				IsClose();

	private:
		char				m_szIpAddress[20];

		SocketAddr			m_SockAddr;
		SOCKET				m_Socket;
		ProtocolType		m_ProtocolType;
	};
}

#endif