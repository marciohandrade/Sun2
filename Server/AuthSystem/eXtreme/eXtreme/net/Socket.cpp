#include "eXtreme/eXtreme.h"
#include "Socket.h"

using namespace eXtreme;

Socket::Socket()
    : m_socket(INVALID_SOCKET), m_protocolType(Protocol_None)
{
}

Socket::~Socket()
{
    Close();
}

bool
Socket::Create(ProtocolType type, bool overlapped)
{
    if(m_socket != INVALID_SOCKET || type == Protocol_None)
    {
        return false;
    }

    m_protocolType = type;
    switch(type)
    {
    case  Protocol_TCP:
        {
            if(overlapped == true)
                m_socket  = ::WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, 0, 0, WSA_FLAG_OVERLAPPED);
            else
                m_socket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

            if(m_socket == INVALID_SOCKET)
            {
                return false;
            }

            ::memset(&m_sockAddr, 0, sizeof(m_sockAddr));
        }
        break;

    case  Protocol_UDP:
        {
            if(overlapped == true)
                m_socket  = ::WSASocket(AF_INET, SOCK_DGRAM, IPPROTO_UDP, 0, 0, WSA_FLAG_OVERLAPPED);
            else
                m_socket = ::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

            if(m_socket == INVALID_SOCKET)
            {
                return false;
            }

            ::memset(&m_sockAddr, 0, sizeof(m_sockAddr));
        }
        break;
    }

    return true;
}

void
Socket::Close()
{
    if(m_socket == INVALID_SOCKET)
    {
        return;
    }

    ::closesocket(m_socket);

    m_socket        = INVALID_SOCKET;
    m_protocolType  = Protocol_None;
}

bool
Socket::Attach(ProtocolType type, SOCKET handle, SocketAddr* addr)
{
    if(type != Protocol_None)
    {
        m_socket        = handle;
        m_protocolType  = type;
    }

    if(addr != 0)
    {
        ::sprintf(m_ipAddress, "%d.%d.%d.%d",
                  addr->sin_addr.S_un.S_un_b.s_b1,
                  addr->sin_addr.S_un.S_un_b.s_b2,
                  addr->sin_addr.S_un.S_un_b.s_b3,
                  addr->sin_addr.S_un.S_un_b.s_b4);
        ::memcpy(&m_sockAddr, addr, sizeof(SocketAddr));
    }

    return true;
}

SOCKET
Socket::Accept(DWORD ms, SocketAddr* addr)
{
    struct timeval  timeout;
    fd_set          fds;
    int             len = sizeof(SocketAddr);

    FD_ZERO(&fds);
    FD_SET(m_socket, &fds);

    timeout.tv_sec  = ms / 1000;
    timeout.tv_usec = ms % 1000;

    if(::select(0, &fds, 0, 0, &timeout) == SOCKET_ERROR)
        return INVALID_SOCKET;

    if(FD_ISSET(m_socket, &fds))
    {
        SOCKET acceptedSocket = INVALID_SOCKET;
        //int  size = sizeof(m_sockAddr);

        try
        {
            acceptedSocket = ::accept(m_socket, (struct sockaddr*)addr, &len);

            if(addr != 0)
            {
                ::sprintf(m_ipAddress, "%d.%d.%d.%d",
                    addr->sin_addr.S_un.S_un_b.s_b1, addr->sin_addr.S_un.S_un_b.s_b2,
                    addr->sin_addr.S_un.S_un_b.s_b3, addr->sin_addr.S_un.S_un_b.s_b4);
            }
            else
            {
                throw ;
            }
        }
        catch(...)
        {
            return INVALID_SOCKET;
        }

        return acceptedSocket;
    }

    return INVALID_SOCKET;
}

bool
Socket::Connect(char* ipAddress, unsigned short portNo)
{
    if(m_socket == INVALID_SOCKET)
    {
        return false;
    }

    //struct    timeval t = { 1, 0 };
    fd_set  fds;


    FD_ZERO(&fds);
    FD_SET(m_socket, &fds);

    ::memset(&m_sockAddr, 0, sizeof(SocketAddr));
    m_sockAddr.sin_family           = AF_INET;
    m_sockAddr.sin_addr.s_addr      = ::inet_addr(ipAddress);
    m_sockAddr.sin_port             = ::htons(portNo);

    if(::connect(m_socket, (struct sockaddr*)&m_sockAddr, sizeof(m_sockAddr)) == SOCKET_ERROR)
    {
        return false;
    }

    ::sprintf(m_ipAddress, "%d.%d.%d.%d",
              m_sockAddr.sin_addr.S_un.S_un_b.s_b1,
              m_sockAddr.sin_addr.S_un.S_un_b.s_b2,
              m_sockAddr.sin_addr.S_un.S_un_b.s_b3,
              m_sockAddr.sin_addr.S_un.S_un_b.s_b4);

    return true;
}

bool
Socket::Bind(char* ipAddress, unsigned short portNo)
{
    ::memset(&m_sockAddr, 0, sizeof(m_sockAddr));
    m_sockAddr.sin_family      = AF_INET;
    m_sockAddr.sin_addr.s_addr = ::inet_addr(ipAddress);
    m_sockAddr.sin_port        = ::htons(portNo);

    int error = ::bind(m_socket, (const sockaddr*)&m_sockAddr, sizeof(m_sockAddr));
    ::sprintf(m_ipAddress, "%d.%d.%d.%d",
              m_sockAddr.sin_addr.S_un.S_un_b.s_b1,
              m_sockAddr.sin_addr.S_un.S_un_b.s_b2,
              m_sockAddr.sin_addr.S_un.S_un_b.s_b3,
              m_sockAddr.sin_addr.S_un.S_un_b.s_b4);

    return (error != SOCKET_ERROR);
}

bool
Socket::Listen(int backLogCount)
{
    return (::listen(m_socket, backLogCount) != SOCKET_ERROR);
}

int
Socket::Send(char* buffer, int length, AsyncResult* result)
{
    WSABUF  wsaBuf;
    wsaBuf.buf  = buffer;
    wsaBuf.len  = length;

    /*
    2006.08.09
    modify by guiun
    WSAGetLastError 호출을 SOCKET_ERROR 검출 이후로 변경을 한다.
    */
    int ret = ::WSASend(m_socket, &wsaBuf, 1, &result->transBytes, 0, (LPOVERLAPPED)result, 0);
    result->error = ERROR_SUCCESS;//result->error = ::WSAGetLastError();
    if(ret == SOCKET_ERROR)
    {
        result->error = ::WSAGetLastError();
        if(result->error != ERROR_IO_PENDING)   // Error 처리
        {
            return 0;
        }
    }

    return result->transBytes;
}

int
Socket::Recv(char* buffer, int bufSize, AsyncResult* result)
{
    WSABUF  wsaBuf;
    wsaBuf.buf  = buffer;
    wsaBuf.len  = bufSize;

    /*
    2006.08.09
    modify by guiun
    WSAGetLastError 호출을 SOCKET_ERROR 검출 이후로 변경을 한다.
    */

    DWORD   flag = 0;
    int ret = ::WSARecv(m_socket, &wsaBuf, 1, &result->transBytes, &flag, (LPOVERLAPPED)result, 0);
    result->error = ERROR_SUCCESS;//result->error = ::WSAGetLastError();
    if(ret == SOCKET_ERROR)
    {
        result->error = ::WSAGetLastError();
        if(result->error != ERROR_IO_PENDING)   // Error 처리
        {
            return 0;
        }
    }

    return result->transBytes;
}

HANDLE
Socket::GetNativeHandle()
{
    HANDLE handle = (m_socket == INVALID_SOCKET) 
                  ? INVALID_HANDLE_VALUE 
                  : (HANDLE)m_socket;

    return handle;
}

void
Socket::GetBufferSize(int* sndBufSize, int* rcvBufSize)
{
    int len = sizeof(int);

    ::getsockopt(m_socket, SOL_SOCKET, SO_SNDBUF, (char*)sndBufSize, &len);
    ::getsockopt(m_socket, SOL_SOCKET, SO_RCVBUF, (char*)rcvBufSize, &len);
}

void
Socket::SetReuseAddr()
{
    BOOL reuse = TRUE;

    ::setsockopt(m_socket, SOL_SOCKET, SO_REUSEADDR, (const char*)&reuse, sizeof(reuse));
}

void
Socket::SetIoctlSocket()
{
    unsigned long   argp = 1;

    ::ioctlsocket(m_socket, FIONBIO, &argp);
}

void
Socket::SetLinger(bool flag)
{
    LINGER  opt =
    {
        flag, 0
    };

    ::setsockopt(m_socket, SOL_SOCKET, SO_LINGER, (char*)&opt, sizeof(opt));
}

void
Socket::SetKeepAlive(bool flag)
{
    int argp = flag;

    ::setsockopt(m_socket, SOL_SOCKET, SO_KEEPALIVE, (char*)&argp, sizeof(argp));
}

void
Socket::SetBufferSize(int sndBufSize, int rcvBufSize)
{
    ::setsockopt(m_socket, SOL_SOCKET, SO_SNDBUF, (char*)&sndBufSize, sizeof(int));
    ::setsockopt(m_socket, SOL_SOCKET, SO_RCVBUF, (char*)&rcvBufSize, sizeof(int));
}

