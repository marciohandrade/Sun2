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
        connect�� ������ ȣ��Ǹ� �� ������ ConnSuccessList�� �߰��Ѵ�.
    @param  pServer
        ������ ������
*/
//==================================================================================================
void Connector::Init(IoHandler* io_handler, DWORD number_of_threads)
{
    assert(number_of_threads <= MAX_CONNECT_THREAD);

    io_handler_ = io_handler;
    number_of_threads_ = min(number_of_threads, MAX_CONNECT_THREAD);

    // ������ ���� ����� ���� ť ����
    if (connecting_list_) {
        delete connecting_list_;
    }
    connecting_list_ = new SessionList;

    // ��Ŀ ��������� �����Ѵ�.
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
        connect ��Ŀ �����带 �����ϰ� ������ �õ��Ѵ�.
    @param  session
        Ŀ��Ʈ�� �õ��ϱ� ���� ������ ������
    @param  szIP
        ������ �õ��� IP
    @param  port
        ������ ��Ʈ
*/
//==================================================================================================
void Connector::Connect(Session* session)
{
    // ������ ���� ��� ť�� �߰��Ѵ�.
    connecting_list_->Lock();
    connecting_list_->push_back(session);
    connecting_list_->Unlock();

    // ��Ŀ �����忡 Ŀ��Ʈ �̺�Ʈ�� ������.
    SetEvent(event_handles_[0]);
}


//==================================================================================================
/**
    @remarks
        connect ��Ŀ �����带 �����Ų��.
*/
//==================================================================================================
void Connector::Shutdown()
{
    shutdown_ = true;

    // ������ ���� �̺�Ʈ
    for (DWORD i = 0; i < number_of_threads_; ++i) {
        SetEvent(event_handles_[1]);
    }
    // ��� ��Ŀ �����尡 ����� ������ ��ٸ���.
    WaitForMultipleObjects(number_of_threads_, thread_handles_, true, INFINITE);
}