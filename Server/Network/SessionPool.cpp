#include "stdafx.h"
#include "SessionPool.h"
#include "Session.h"
#include "SessionList.h"

//==================================================================================================
/**
    @remarks
    생성자
    @param  nSize
    풀이 수용할 수 있는 인스턴스의 최대 개수
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
    소멸자
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
    세션 풀 생성
    @param  dwSize
    생성할 세션 개수
*/
//==================================================================================================
void SessionPool::Create(DWORD max_size,
                         DWORD send_buffer_size, DWORD recv_buffer_size, DWORD max_packet_size,
                         DWORD timeout_tick, DWORD index_start, BOOL is_accept_socket)
{
    max_size_ = max_size;
    // 세션 리스트 생성
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
    세션 풀에서 세션의 포인터를 꺼내서 리턴해준다.
    @retval Session*
    세션 풀로부터 할당 받은 세션의 포인터
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
    //printf("New 세션풀(%x) 남은 세션(%d)\n", this, session_list_->size()); 

    session_list_->Unlock();
    return session;
}

//==================================================================================================
/**
    @remarks
    사용하던 세션을 세션 풀에 반환한다.
    @param  session
    세션 풀에 반환할 세션의 포인터
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
    현재 세션 풀에 남아 있는 세션의 개수를 리턴한다. (사용 가능한 세션의 수)
    @retval int
    사용 가능한 세션의 수
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
