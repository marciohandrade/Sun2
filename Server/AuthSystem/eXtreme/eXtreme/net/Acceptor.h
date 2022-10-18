#ifndef  __eXtreme_Acceptor_H
#define  __eXtreme_Acceptor_H

#include "Socket.h"
#include "eXtreme/threading/Thread.h"

namespace eXtreme {

class eXServerSession;
class Acceptor : public Thread
{
public:
    Acceptor();
    virtual ~Acceptor();


protected:
    virtual BOOL Run();
    virtual void OnAccept(SOCKET /*s*/, Socket::SocketAddr& /*addr*/){}


    bool Open(char* ip, u_short portNo, int sndBufSize = 0, int rcvBufSize = 0);
    void Close();

private:
    Socket m_listenSocket;
};

}

#endif