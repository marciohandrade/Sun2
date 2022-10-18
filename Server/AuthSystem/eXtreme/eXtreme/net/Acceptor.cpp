#include "eXtreme/eXtreme.h"
#include "Acceptor.h"
#include "./ServerSession.h"

using namespace eXtreme;

Acceptor::Acceptor()
{
}

Acceptor::~Acceptor()
{
}

bool
Acceptor::Open(char* ip, u_short portNo, int sndBufSize, int rcvBufSize)
{
    if(m_handleThread != INVALID_HANDLE_VALUE)
    {
        return false;
    }

    if(m_listenSocket.Create(Socket::Protocol_TCP, true) == false)
    {
        CallbackErrorHandler(::GetLastError(), "[eXtreme::Acceptor::Create] Listen socket 생성 실패");

        return false;
    }

    //  Listen socket의 속성은 Accept시 생성되는 socket에 상속되므로, 미리 설정할 수 있다.
    if(sndBufSize > 0 && rcvBufSize > 0)
        m_listenSocket.SetBufferSize(sndBufSize, rcvBufSize);

    if(m_listenSocket.Bind(ip, portNo) == false)
    {
        char    desc[127] = {0, };
        ::sprintf(desc, "[eXtreme::Acceptor::Create] Socket binding 실패(ip:%s, port:%d)", ip, portNo);
        CallbackErrorHandler(::GetLastError(), desc);

        return false;
    }
    if(m_listenSocket.Listen() == false)
    {
        char    desc[127] = {0, };
        ::sprintf(desc, "[eXtreme::Acceptor::Create] Socket listening 실패(ip:%s, port:%d)", ip, portNo);
        CallbackErrorHandler(::GetLastError(), desc);
        return false;
    }

    Start();

    return true;
}

void
Acceptor::Close()
{
    m_listenSocket.Close();

    Terminate(0);
}

BOOL
Acceptor::Run()
{
    eXtreme::Socket::SocketAddr addr;

    SOCKET accepted = m_listenSocket.Accept(500, &addr);
    if(accepted == INVALID_SOCKET)
        return TRUE;

    OnAccept(accepted, addr);

    return TRUE;
}

















