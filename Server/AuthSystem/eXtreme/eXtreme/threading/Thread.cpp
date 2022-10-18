#include "eXtreme/eXtreme.h"
#include "Thread.h"


using namespace eXtreme;

Thread::Thread()
{
    m_threadID = 0;
    m_handleThread = INVALID_HANDLE_VALUE;
    m_hExitSynEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    m_hExitAckEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
}

Thread::~Thread()
{
    if(m_handleThread!=INVALID_HANDLE_VALUE)
    {
        CloseHandle(m_handleThread);
    }

    CloseHandle(m_hExitSynEvent);
    CloseHandle(m_hExitAckEvent);
}

bool
Thread::Start()
{
    if(m_handleThread != INVALID_HANDLE_VALUE)
    {
        ::CloseHandle(m_handleThread);
        m_handleThread = INVALID_HANDLE_VALUE;
    }

    unsigned int uiThreadID = 0;
    m_handleThread = (HANDLE)_beginthreadex(0, 0, Runner, (LPVOID)this, 0, &uiThreadID);

    m_threadID = uiThreadID;

    ResetEvent(m_hExitSynEvent);

    if(m_handleThread == 0)
    {
        return false;
    }

    return true;
}

BOOL
Thread::Terminate(DWORD exitCode)
{
    if(m_handleThread == INVALID_HANDLE_VALUE)
        return TRUE;

    SetEvent(m_hExitSynEvent);

    // Thread�� ������ ���Ḧ ��ġ�� m_hExitAckEvent�� ���������� ��ٸ���.
    if(WAIT_TIMEOUT == ::WaitForSingleObject(m_hExitAckEvent, 10000))
    {
        // 10�ʸ� ��ٷ��� ������ ������ ������ �����Ų��.
        return ::TerminateThread(m_handleThread, exitCode);
    }

    m_handleThread  = INVALID_HANDLE_VALUE;

    return TRUE;
}

bool
Thread::WaitForExitEvent()
{
    if(WaitForSingleObject(m_hExitSynEvent, 10) == WAIT_OBJECT_0) 
        return true;

    return false;
}

void
Thread::NotifyExitState()
{
    SetEvent(m_hExitAckEvent);
}

unsigned int __stdcall
Thread::Runner(LPVOID parameter)
{
    Thread * pThread = (Thread*)parameter;

    while(TRUE)
    {
        if(pThread->Run())
        {
            // Update�� �����ؾ� ���Ḧ �� �� �ִ�.
            if(pThread->WaitForExitEvent())
                break;
        }
    }

    pThread->NotifyExitState();

    return 0;
}


