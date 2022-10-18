#ifndef  __eXtreme_Connector_H
#define  __eXtreme_Connector_H

#include <queue>

#include "eXtreme/util/Pool.h"
#include "eXtreme/io/AsyncResult.h"
#include "eXtreme/threading/Mutex.h"
#include "eXtreme/threading/Thread.h"

namespace eXtreme {

class EventQueue;
class eXServerSession;
class Connector : public Thread
{
public:
    Connector();
    virtual ~Connector();

    void Create(EventQueue* eq);
    void Destroy();

    void Connect(eXServerSession* s, char* ipAddress, unsigned short portNo);

protected:
    virtual BOOL Run();

private:
    typedef struct _tagConnectInfo
    {
        eXServerSession* session;
        char            ipAddress[20];
        unsigned short  portNo;
    } ConnectInfo;

    EventQueue* m_eventQueue;

    std::queue< ConnectInfo* > m_queue;
    Pool< ConnectInfo > m_pool;

    Mutex m_mutexQueue;
    Mutex m_mutexPool;
};

}

#endif