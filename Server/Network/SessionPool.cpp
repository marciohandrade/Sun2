#include "stdafx.h"
#include "SessionPool.h"
#include "Session.h"
#include "SessionList.h"

//==================================================================================================
/**
    @remarks
    ������
    @param  nSize
    Ǯ�� ������ �� �ִ� �ν��Ͻ��� �ִ� ����
*/
//==================================================================================================
SessionPool::SessionPool(DWORD max_size,
                         DWORD send_buffer_size, DWORD recv_buffer_size, DWORD max_packet_size,
                         DWORD timeout_tick, DWORD index_start, BOOL is_accept_socket)
{
    max_size_ = 0;
    session_list_ = NULL;
    Create(max_size, send_buffer_size, recv_buffer_size, \
           max_packet_size, timeout_tick, index_start, is_accept_socket);
}

//==================================================================================================
/**
    @remarks
    �Ҹ���
*/
//==================================================================================================
SessionPool::~SessionPool()
{
    if (session_list_) {
        module::TAllocator<SessionList>::DeAllocate(session_list_);
        session_list_ = NULL;
    }
}

//==================================================================================================
/**
    @remarks
    ���� Ǯ ����
    @param  dwSize
    ������ ���� ����
*/
//==================================================================================================
void SessionPool::Create(DWORD max_size,
                         DWORD send_buffer_size, DWORD recv_buffer_size, DWORD max_packet_size,
                         DWORD timeout_tick, DWORD index_start, BOOL is_accept_socket)
{
    max_size_ = max_size;
    // ���� ����Ʈ ����
    session_list_ = module::TAllocator<SessionList>::Allocate();
    for (DWORD i = 0; i < max_size; ++i)
    {
        //                  multi-arg TAllocator simulation
        Session* session = new (::malloc(sizeof(Session))) Session(\
            send_buffer_size, recv_buffer_size, max_packet_size, timeout_tick);
        session->SetIndex(index_start + i);
        //
        if (is_accept_socket) {
            session->SetAcceptSocketFlag();
        }
        session_list_->push_back(session);
    }
}

//==================================================================================================
/**
    @remarks
    ���� Ǯ���� ������ �����͸� ������ �������ش�.
    @retval Session*
    ���� Ǯ�κ��� �Ҵ� ���� ������ ������
*/
//==================================================================================================
Session* SessionPool::Alloc()
{
    session_list_->Lock();
    //
    if (session_list_->empty())
    {
        session_list_->Unlock();
        return NULL;
    }
    //
    Session* session = session_list_->front();
    session_list_->pop_front();
    //printf("New ����Ǯ(%x) ���� ����(%d)\n", this, session_list_->size()); 

    session_list_->Unlock();
    return session;
}

//==================================================================================================
/**
    @remarks
    ����ϴ� ������ ���� Ǯ�� ��ȯ�Ѵ�.
    @param  session
    ���� Ǯ�� ��ȯ�� ������ ������
*/
//==================================================================================================
void SessionPool::Free(Session* session)
{
    session_list_->Lock();
    //
    assert(session_list_->size() < max_size_);
    session_list_->push_back(session);
    //
    session_list_->Unlock();
}

//==================================================================================================
/**
    @remarks
    ���� ���� Ǯ�� ���� �ִ� ������ ������ �����Ѵ�. (��� ������ ������ ��)
    @retval int
    ��� ������ ������ ��
*/
//==================================================================================================
int SessionPool::GetLength()
{
    session_list_->Lock();
    //
    int size = (int)(session_list_->size());
    session_list_->Unlock();
    //
    return size;
}
