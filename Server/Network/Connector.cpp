#include "stdafx.h"
#include "Connector.h"
#include "Session.h"
#include "SessionList.h"
#include "IoHandler.h"
#include "IOCPServer.h"

//==================================================================================================

Connector::Connector()
    : io_handler_(NULL),
      connecting_list_(NULL),
      shutdown_(false),
      number_of_threads_(0)
{
    for (int i = 0;
         i < static_cast<int>(sizeof(thread_handles_) / sizeof(thread_handles_[0]));
         ++i)
    {
        thread_handles_[i] = INVALID_HANDLE_VALUE;
    }

    event_handles_[0] = CreateEvent(NULL, false, false, NULL);
    event_handles_[1] = CreateEvent(NULL, false, false, NULL);
}

Connector::~Connector()
{
    if (shutdown_ == false) {
        Shutdown();
    }
    for (DWORD i = 0; i < number_of_threads_; ++i)
    {
        HANDLE* handle_it = &thread_handles_[i];
        if (*handle_it != INVALID_HANDLE_VALUE)
        {
            CloseHandle(*handle_it);
            *handle_it = INVALID_HANDLE_VALUE; // (f100615.1L) add a routine for an initialization
        }
    }

    CloseHandle(event_handles_[0]);
    CloseHandle(event_handles_[1]);
    SAFE_DELETE(connecting_list_);
}

//==================================================================================================
/**
    @remarks
        connect에 성공시 호출되며 그 세션을 ConnSuccessList에 추가한다.
    @param  pServer
        서버의 포인터
*/
//==================================================================================================
void Connector::Init(IoHandler* io_handler, DWORD number_of_threads)
{
    assert(number_of_threads <= MAX_CONNECT_THREAD);

    io_handler_ = io_handler;
    number_of_threads_ = min(number_of_threads, MAX_CONNECT_THREAD);

    // 접속을 위해 대기할 세션 큐 생성
    if (connecting_list_) {
        delete connecting_list_;
    }
    connecting_list_ = new SessionList;

    // 워커 쓰레드들을 생성한다.
    unsigned thread_id;
    for (DWORD i = 0; i < number_of_threads_; ++i)
    {
        thread_handles_[i] = \
            (HANDLE)_beginthreadex(NULL, 0, connect_thread, (LPVOID)this, 0, &thread_id);
    }
}

//==================================================================================================
/**
    @remarks
        connect 워커 쓰레드를 생성하고 접속을 시도한다.
    @param  session
        커넥트를 시도하기 위한 세션의 포인터
    @param  szIP
        접속을 시도할 IP
    @param  port
        접속할 포트
*/
//==================================================================================================
void Connector::Connect(Session* session)
{
    // 세션을 접속 대기 큐에 추가한다.
    connecting_list_->Lock();
    connecting_list_->push_back(session);
    connecting_list_->Unlock();

    // 워커 쓰레드에 커넥트 이벤트를 보낸다.
    SetEvent(event_handles_[0]);
}


//==================================================================================================
/**
    @remarks
        connect 워커 쓰레드를 종료시킨다.
*/
//==================================================================================================
void Connector::Shutdown()
{
    shutdown_ = true;

    // 쓰레드 종료 이벤트
    for (DWORD i = 0; i < number_of_threads_; ++i) {
        SetEvent(event_handles_[1]);
    }
    // 모든 워커 쓰레드가 종료될 때까지 기다린다.
    WaitForMultipleObjects(number_of_threads_, thread_handles_, true, INFINITE);
}