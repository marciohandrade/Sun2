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
/// 리슨 소켓을 열고 접속을 처리하는 클래스
//  - I/O 핸들러는 하나의 포트를 가지고 리슨할 수 있으며 별도의 세션 풀을 가지고 있다.
//  - 접속이 이루어지면 세션풀에서 하나의 세션을 할당받아 소켓을 초기화한 후 TempSessionList에 추가한다.
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
