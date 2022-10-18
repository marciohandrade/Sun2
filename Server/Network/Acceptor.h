#ifndef NETWORK_MODULE_IOGROUP_H
#define NETWORK_MODULE_IOGROUP_H

#include <process.h>
#include <Ws2tcpip.h>

//==================================================================================================

#define MAX_ACCEPT_THREAD   16

class Session;
class IoHandler;
class SessionPool;

typedef struct tagIOHANDLER_DESC IOHANDLER_DESC,
                                 *LPIOHANDLER_DESC;

//==================================================================================================
/// ���� ������ ���� ������ ó���ϴ� Ŭ����
//  - I/O �ڵ鷯�� �ϳ��� ��Ʈ�� ������ ������ �� ������ ������ ���� Ǯ�� ������ �ִ�.
//  - ������ �̷������ ����Ǯ���� �ϳ��� ������ �Ҵ�޾� ������ �ʱ�ȭ�� �� TempSessionList�� �߰��Ѵ�.
//
class Acceptor
{
public:
    Acceptor();
    ~Acceptor();

    void Init(IoHandler* io_handler, DWORD number_of_threads);
    BOOL StartListen(const char* ip_string, WORD port);
    void Shutdown();
    BOOL IsListening() { return listen_socket_ != INVALID_SOCKET; }
    void SuspendListenThread();
    void ResumeListenThread();
    SOCKET GetListenSocket() { return listen_socket_; }

private:
    BOOL CreateListenSocket(const char* ip_string, WORD port);

    IoHandler* io_handler_;
    HANDLE iocp_handle_;
    SOCKET listen_socket_;
    DWORD number_of_threads_;
    HANDLE thread_handles_[MAX_ACCEPT_THREAD];
    SOCKADDR_IN sockaddr_in_;
    friend unsigned __stdcall accept_thread(LPVOID param);
};

#endif //NETWORK_MODULE_IOGROUP_H
