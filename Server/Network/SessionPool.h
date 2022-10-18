#pragma once
#ifndef NETWORK_MODULE_SESSIONPOOL_H
#define NETWORK_MODULE_SESSIONPOOL_H

//==================================================================================================

class Session;
class Session;
class SessionList;

//==================================================================================================
/// ���ǵ��� �ִ� ������ŭ �̸� �Ҵ��ؼ� �����ϴ� ���� Ǯ Ŭ����
//      ����Ǯ�� ������ �ִ� �޸��� �Ҹ��� ����Ǯ���� ����Ѵ�.
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
