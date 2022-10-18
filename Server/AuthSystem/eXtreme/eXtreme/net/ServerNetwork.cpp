#include "eXtreme/eXtreme.h"
#include "ServerNetwork.h"

using namespace eXtreme;

ServerNetwork::ServerNetwork()
: m_sessionManager(NULL), m_threadCount(0)
{
}

ServerNetwork::~ServerNetwork()
{
}

bool
ServerNetwork::Create(WORD sessionCnt, SessionManager* sm, DWORD threadCnt)
{
    if(threadCnt > MAX_DISPATCHER_COUNT)
        threadCnt = MAX_DISPATCHER_COUNT;

    m_threadCount = threadCnt;

    if(m_eventQueue.Create((DWORD)threadCnt) == false)
    {
        return false;
    }

    m_sessionManager                    = sm;
    m_sessionManager->m_serverNetwork   = this;

    m_connector.Create(&m_eventQueue);

    //  SessionManager를 생성한다.
    if(m_sessionManager->Create(sessionCnt) == false)
    {
        m_connector.Destroy();
        m_eventQueue.Close();

        return false;
    }

    for(u_int i = 0 ; i < threadCnt ; i++)
    {
        unsigned int uiThreadID = 0;
        m_handleDispatcher[i] = (HANDLE)_beginthreadex(0, 0, Dispatcher, (LPVOID)this, 0, &uiThreadID);
        m_threadID = uiThreadID;

        if(0 == m_handleDispatcher[i])
        {

        }
    }

    return true;
}

void
ServerNetwork::Destroy()
{
    u_int       i = 0;

    m_connector.Destroy();

    CloseAcceptor();


    for(i = 0 ; i < m_threadCount ; i++)
        m_eventQueue.PostCompletion(0, 0);

    for(i = 0 ; i < m_threadCount ; i++)
    {
        if(::WaitForSingleObject(m_handleDispatcher[i], 1000) != WAIT_OBJECT_0)
        {
            TerminateThread(m_handleDispatcher[i], 0);
            m_handleDispatcher[i]   = INVALID_HANDLE_VALUE;
            m_threadId              = 0;
        }
    }

    if(m_sessionManager != 0)
    {
        m_sessionManager->Destroy();
        m_sessionManager = 0;
    }
    m_eventQueue.Close();
}

bool
ServerNetwork::OpenAcceptor(char* ip, u_short portNo)
{
    return Acceptor::Open(ip, portNo);
}

void
ServerNetwork::CloseAcceptor()
{
    Acceptor::Close();
}

void
ServerNetwork::OnAccept(SOCKET s, Socket::SocketAddr& addr)
{
    eXServerSession* session = m_sessionManager->BindSession(s, &addr);
    if(session == 0)
    {
        ::closesocket(s);
        return;
    }

    AddSessionEvent(session);

    session->m_resultAccept.eventType   = Event_Accept;
    session->m_resultAccept.error       = 0;
    session->m_resultAccept.handler     = session;
    session->m_resultAccept.transBytes  = 0;
    m_eventQueue.PostCompletion(session, &session->m_resultAccept);
}

unsigned int __stdcall
ServerNetwork::Dispatcher(LPVOID parameter)
{
    ServerNetwork*  instance = (ServerNetwork*)(parameter);
    EventHandler*   handler = NULL;
    AsyncResult*    result = NULL;

    while(1)
    {
        handler = instance->m_eventQueue.GetCompletionEvent(&result, INFINITE);
        if(handler == 0)
            break;


        handler->HandleEvent(result);


        if(result->eventType == Event_Connect && result->transBytes == 1)
        {
            instance->m_sessionManager->ActiveSession((eXServerSession*)handler);
        }
        else if(result->eventType == Event_Close)
        {
            instance->m_sessionManager->InactiveSession((eXServerSession*)handler);
        }
    }

    return 0;
}

void
ServerNetwork::Update()
{
    if(m_sessionManager != NULL)
    {
        m_sessionManager->Update();
    }
}

void
ServerNetwork::ConnectSession(eXServerSession* s, char* ipAddress, unsigned short portNo)
{
    m_connector.Connect(s, ipAddress, portNo);
}

bool
ServerNetwork::AddSessionEvent(eXServerSession* s)
{
    return m_eventQueue.AddEvent(s);
}

void
ServerNetwork::PostCompletion(EventHandler* handler, AsyncResult* result)
{
    m_eventQueue.PostCompletion(handler, result);
}

