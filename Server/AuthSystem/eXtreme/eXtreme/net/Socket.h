#ifndef  __eXtreme_Socket_H
#define  __eXtreme_Socket_H

#include <eXtreme/io/AsyncResult.h>

namespace eXtreme {

class Socket
{
public:
    typedef sockaddr_in     SocketAddr;

    enum ProtocolType
    {
        Protocol_None = 0,
        Protocol_TCP,
        Protocol_UDP
    };

public:
    Socket();
    ~Socket();

    bool   Create(ProtocolType type, bool overlapped = false);
    void   Close();

    bool   Attach(ProtocolType type, SOCKET handle, SocketAddr* addr);
    SOCKET Accept(DWORD ms, SocketAddr* addr);
    bool   Connect(char* ipAddress, unsigned short portNo);
    bool   Bind(char* ipAddress, unsigned short portNo);
    bool   Listen(int backLogCount = SOMAXCONN);


    int    Send(char* buffer, int length, AsyncResult* result);
    int    Recv(char* buffer, int bufSize, AsyncResult* result);


    HANDLE GetNativeHandle();
    SocketAddr* GetSocketAddr() { return &m_sockAddr; }
    char*  GetIPAddress() { return m_ipAddress; }
    void   GetBufferSize(int* sndBufSize, int* rcvBufSize);

    void   SetReuseAddr();
    void   SetIoctlSocket();
    void   SetLinger(bool flag);
    void   SetKeepAlive(bool flag);
    void   SetBufferSize(int sndBufSize, int rcvBufSize);

private:
    char         m_ipAddress[20];

    SocketAddr   m_sockAddr;
    SOCKET       m_socket;
    ProtocolType m_protocolType;
};

}

#endif