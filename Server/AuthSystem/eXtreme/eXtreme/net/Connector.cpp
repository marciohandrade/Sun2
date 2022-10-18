#include "eXtreme/eXtreme.h"
#include "Connector.h"
#include "./ServerSession.h"
#include "eXtreme/threading/Guard.h"

using namespace eXtreme;

Connector::Connector()
: m_eventQueue(NULL)
{
}

Connector::~Connector()
{
    Destroy();
}

void
Connector::Create(EventQueue* eq)
{
    m_eventQueue = eq;
    m_pool.Create(10);

    Start();
}

void
Connector::Destroy()
{
    Terminate(0);
    m_eventQueue = 0;

    while(m_queue.size() > 0)
    {
        m_pool.Push(m_queue.front());
        m_queue.pop();
    }

    m_pool.Destroy();
}

void
Connector::Connect(eXServerSession* s, char* ipAddress, unsigned short portNo)
{
    ConnectInfo*    info = NULL;

    {
        Guard   < Mutex >   guard(m_mutexPool);

        info = m_pool.Pop();
        if(info == 0)
        {
            m_pool.IncSize(10);
            info = m_pool.Pop();
        }

        s->Create();

        ::strcpy(info->ipAddress, ipAddress);
        info->session   = s;
        info->portNo    = portNo;
    }

    {
        Guard   < Mutex >   guard(m_mutexQueue);

        m_queue.push(info);
    }
}

BOOL
Connector::Run()
{
    Guard< Mutex >  guard(m_mutexQueue);
    if(m_queue.size() == 0) 
        return TRUE;

    ConnectInfo *info = m_queue.front();
    m_queue.pop();

    ::memset(&info->session->m_resultConnect, 0, sizeof(info->session->m_resultConnect));
    info->session->m_resultConnect.eventType    = Event_Connect;
    info->session->m_resultConnect.handler      = info->session;

    Socket* s   = info->session->GetHandle();

    bool ret    = s->Connect(info->ipAddress, info->portNo);

    if(ret == true)
    {   
        info->session->m_resultConnect.transBytes = 1;
    }
    else
    {
        info->session->m_resultConnect.error = ::GetLastError();
    }

    m_eventQueue->PostCompletion(info->session, &info->session->m_resultConnect);

    {
        Guard< Mutex >  guard(m_mutexPool);

        ::memset(info, 0, sizeof(*info));
        m_pool.Push(info);
    }

    return FALSE;
}



