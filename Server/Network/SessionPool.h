#pragma once
#ifndef NETWORK_MODULE_SESSIONPOOL_H
#define NETWORK_MODULE_SESSIONPOOL_H

//==================================================================================================

class Session;
class Session;
class SessionList;

//==================================================================================================
/// 세션들을 최대 개수만큼 미리 할당해서 관리하는 세션 풀 클래스
//      세션풀이 가지고 있던 메모리의 소멸은 세션풀에서 담당한다.
//==================================================================================================

class SessionPool
{
public:
    SessionPool(DWORD max_size,
                DWORD send_buffer_size, DWORD recv_buffer_size, DWORD max_packet_size,
                DWORD timeout_tick, DWORD index_start, BOOL is_accept_socket);
    ~SessionPool();

    Session* Alloc();
    void Free(Session* session);
    int GetLength();
    DWORD GetNumberOfNodes() const;

private:
    void Create(DWORD max_size,
                DWORD send_buffer_size, DWORD recv_buffer_size, DWORD max_packet_size,
                DWORD timeout_tick, DWORD index_start, BOOL is_accept_socket);

    DWORD max_size_;
    SessionList* session_list_;
};


inline DWORD SessionPool::GetNumberOfNodes() const {
    return max_size_;
};

#endif //NETWORK_MODULE_SESSIONPOOL_H
