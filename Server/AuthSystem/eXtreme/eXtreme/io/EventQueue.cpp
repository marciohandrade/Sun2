#include "eXtreme/eXtreme.h"
#include "EventQueue.h"

using namespace eXtreme;

EventQueue::EventQueue()
    : m_handleQueue(INVALID_HANDLE_VALUE)
{
}

EventQueue::~EventQueue()
{
    Close();
}

bool
EventQueue::Create(int threadCount)
{
    if(m_handleQueue != INVALID_HANDLE_VALUE)
    {
        return false;
    }

    m_handleQueue = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, threadCount);
    if(m_handleQueue == 0)
    {
        DWORD dwErrorCode = ::GetLastError();
        CallbackErrorHandler(dwErrorCode,
                             "[eXtreme::EventQueue::Create] CreateIoCompletionPort 积己 角菩");

        m_handleQueue = INVALID_HANDLE_VALUE;

        return false;
    }

    return true;
}

void
EventQueue::Close()
{
    if(m_handleQueue == INVALID_HANDLE_VALUE)
    {
        return;
    }

    ::CloseHandle(m_handleQueue);
    m_handleQueue = INVALID_HANDLE_VALUE;
}

bool
EventQueue::AddEvent(EventHandler* handler)
{
    HANDLE ret = ::CreateIoCompletionPort(handler->GetHandle()->GetNativeHandle(),
                                          m_handleQueue, (ULONG_PTR)handler, 0);
    if(ret == 0)
    {
        DWORD   err = ::GetLastError();

        if(err != ERROR_INVALID_PARAMETER){
            CallbackErrorHandler(err,
                                 "[eXtreme::EventQueue::AddEvent] CreateIoCompletionPort 积己 角菩");
        }
    }

    return (ret != 0);
}

void
EventQueue::RemoveEvent(EventHandler* handler)
{
    handler->GetHandle()->Close();
}

EventHandler*
EventQueue::GetCompletionEvent(AsyncResult** result, int ms)
{
    if(result == 0 || m_handleQueue == INVALID_HANDLE_VALUE)
        return 0;

    DWORD transBytes = 0;
    ULONG key = 0;

    BOOL ret = ::GetQueuedCompletionStatus(m_handleQueue,
        &transBytes, (PULONG_PTR)&key, (LPOVERLAPPED*)result, ms);
    EventHandler* handler = (EventHandler*)(u_int64)key;

    if(ret == TRUE && key != NULL && *result != NULL)
    {
        (*result)->transBytes = transBytes;
        (*result)->error      = ::GetLastError();
    }
    else if(key != 0 && ::GetLastError() != ERROR_OPERATION_ABORTED)
    {
        DWORD error = ::GetLastError();

        if(error != ERROR_IO_PENDING && error != ERROR_SUCCESS && *result != 0)
        {
            (*result)->error      = error;
            (*result)->transBytes = transBytes;
            (*result)->handler = handler;

            return handler;
        }
    }

    return handler;
}

void
EventQueue::PostCompletion(EventHandler* handler, AsyncResult* result)
{
    BOOL bResult = 0;
    if(result != 0)
    {
        bResult = ::PostQueuedCompletionStatus(m_handleQueue,
                                               result->transBytes, (ULONG_PTR)handler, result);
    }
    else
    {
        bResult = ::PostQueuedCompletionStatus(m_handleQueue,
                                               0, (ULONG_PTR)handler, 0);
    }

    if(bResult == FALSE)
    {   
    }
}

