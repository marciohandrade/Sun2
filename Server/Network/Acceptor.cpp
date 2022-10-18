#include "stdafx.h"
#include "Acceptor.h"
#include "IoHandler.h"
#include "IOCPServer.h"
#include "Session.h"
#include "RecvBuffer.h"
#include "SessionList.h"
#include "SessionPool.h"
#include "MsWinsockUtil.h"


Acceptor::Acceptor()
{
    listen_socket_ = INVALID_SOCKET;
    iocp_handle_ = INVALID_HANDLE_VALUE;
    for (DWORD i = 0; i < MAX_ACCEPT_THREAD; ++i) {
        thread_handles_[i] = INVALID_HANDLE_VALUE;
    }
    number_of_threads_ = 0;
}

Acceptor::~Acceptor()
{
    Shutdown();

    if (iocp_handle_ != INVALID_HANDLE_VALUE) {
        CloseHandle(iocp_handle_);
    }
    for (DWORD i = 0; i < number_of_threads_; ++i)
    {
        if (thread_handles_[i] != INVALID_HANDLE_VALUE) {
            CloseHandle(thread_handles_[i]);
        }
    }
}

void Acceptor::Init(IoHandler* io_handler, DWORD number_of_threads)
{
    assert(number_of_threads <= MAX_ACCEPT_THREAD);

    io_handler_ = io_handler;
    number_of_threads_ = number_of_threads;

    // Accept용 IOCP 생성
    iocp_handle_ = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);

    // 워커 쓰레드 생성
    unsigned int thread_id;
    for (DWORD i = 0; i < number_of_threads_; ++i)
    {
        thread_handles_[i] = \
            (HANDLE)_beginthreadex(NULL, 0, accept_thread,
                                   (LPVOID)this, 0/*CREATE_SUSPENDED*/, &thread_id);
    }

    // backlog가 다 찼을 때는 IOCP로 메시지를 받을 수 없으므로 이벤트 기반 리슨을 추가로 붙여 줘야 한다
}

//==================================================================================================
/**
    @remarks
        포트를 초기화한다.
    @param  ip_string
        접속할 IP
    @param  port
        접속할 포트
    @retval BOOL
        리슨에 실패하면 false 리턴
*/
//==================================================================================================
BOOL Acceptor::StartListen(const char* ip_string, WORD port)
{   
    // 멈춰 있으면 다시 구동시킨다.
    ResumeListenThread();

    // 리슨 소켓 생성
    if (!CreateListenSocket(ip_string, port)) {
        return false;
    }
    // IOCP에 리슨 소켓 등록
    CreateIoCompletionPort((HANDLE)listen_socket_, iocp_handle_, 1, 0);

    //Dll에서 함수 로드(AcceptEx, TransmitFile)
    MsWinsockUtil::LoadExtensionFunction(listen_socket_);

    return true;
}

//==================================================================================================
/**
    @remarks
        리슨 소켓을 생성하고 주소를 설정한다.
    @retval BOOL
        리슨 소켓 생성에 실패하면 false를 리턴한다.
        (IP와 port가 겹치는 경우일 것이다.)
*/
//==================================================================================================
BOOL Acceptor::CreateListenSocket(const char* ip_string, WORD port)
{
    // 이미 리슨 소켓이 생성되어 있으면 실패
    if (listen_socket_ != INVALID_SOCKET) {
        return false;
    }

    // 리슨 소켓 생성
    listen_socket_ = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
    if (listen_socket_ == INVALID_SOCKET) {
        return false;
    }
    //  // IO_PENDING중인 놈을 버림으로써 소켓 처리 속도를 높인다.
    //  // 이것을 위해 linger 옵션 적용
    //  LINGER  slinger;
    //
    //  slinger.l_linger = 5000;
    //  slinger.l_onoff = (USHORT)SO_LINGER;
    //  int ret = setsockopt(listen_socket_, SOL_SOCKET, SO_LINGER, (char*)&slinger, sizeof(LINGER));

#ifdef _WHATTHEHELL_NETWORK_20060519
    int nFlag;
    int ret = setsockopt(listen_socket_, SOL_SOCKET, SO_REUSEADDR, (char*)&nFlag, sizeof(nFlag));
#endif

    // set a network address
    ZeroMemory(&sockaddr_in_, sizeof(sockaddr_in_));

    sockaddr_in_.sin_family = AF_INET;
    sockaddr_in_.sin_addr.s_addr = (ip_string == NULL || strlen(ip_string) == 0)
                                 ?  htonl(INADDR_ANY) : inet_addr(ip_string);
    sockaddr_in_.sin_port = htons(port);

    // bind
    int err = ::bind(listen_socket_, (SOCKADDR *)&sockaddr_in_, sizeof(sockaddr_in_));
    if (err == SOCKET_ERROR) {
        ::closesocket(listen_socket_);
        return false;
    }
    // listen
    err = ::listen(listen_socket_, SOMAXCONN);
    if (err == SOCKET_ERROR) {
        ::closesocket(listen_socket_);
        return false;
    }

    return true;
}

//==================================================================================================
/**
    @remarks
        리슨 쓰레드를 잠시 멈춘다.
*/
//==================================================================================================
void Acceptor::SuspendListenThread()
{
    for (DWORD i = 0; i < number_of_threads_; ++i)
    {
        assert(thread_handles_[i] != INVALID_HANDLE_VALUE);
        SuspendThread(thread_handles_[i]);
    }
}

//==================================================================================================
/**
    @remarks
        리슨 쓰레드를 다시 구동시킨다.
*/
//==================================================================================================
void Acceptor::ResumeListenThread()
{
    for (DWORD i = 0; i < number_of_threads_; ++i)
    {
        assert(thread_handles_ != INVALID_HANDLE_VALUE);
        ResumeThread(thread_handles_[i]);
    }
}

//==================================================================================================
/**
    @remarks
        리슨 쓰레드를 종료시킨다.
*/
//==================================================================================================
void Acceptor::Shutdown()
{
    // 멈춰 있으면 다시 구동시킨다.
    ResumeListenThread();

    // 리슨 소켓 종료
    if (listen_socket_ != INVALID_SOCKET)
    {
        ::closesocket(listen_socket_);
        listen_socket_ = INVALID_SOCKET;
    }
    // acceptor 쓰레드 종료
    for (DWORD i = 0; i < number_of_threads_; ++i) {
        PostQueuedCompletionStatus(iocp_handle_, 0, 0, NULL);
    }
    // 모든 워커 쓰레드가 종료될 때까지 기다린다.
    WaitForMultipleObjects(number_of_threads_, thread_handles_, true, INFINITE);
}
