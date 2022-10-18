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

    // Accept�� IOCP ����
    iocp_handle_ = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);

    // ��Ŀ ������ ����
    unsigned int thread_id;
    for (DWORD i = 0; i < number_of_threads_; ++i)
    {
        thread_handles_[i] = \
            (HANDLE)_beginthreadex(NULL, 0, accept_thread,
                                   (LPVOID)this, 0/*CREATE_SUSPENDED*/, &thread_id);
    }

    // backlog�� �� á�� ���� IOCP�� �޽����� ���� �� �����Ƿ� �̺�Ʈ ��� ������ �߰��� �ٿ� ��� �Ѵ�
}

//==================================================================================================
/**
    @remarks
        ��Ʈ�� �ʱ�ȭ�Ѵ�.
    @param  ip_string
        ������ IP
    @param  port
        ������ ��Ʈ
    @retval BOOL
        ������ �����ϸ� false ����
*/
//==================================================================================================
BOOL Acceptor::StartListen(const char* ip_string, WORD port)
{   
    // ���� ������ �ٽ� ������Ų��.
    ResumeListenThread();

    // ���� ���� ����
    if (!CreateListenSocket(ip_string, port)) {
        return false;
    }
    // IOCP�� ���� ���� ���
    CreateIoCompletionPort((HANDLE)listen_socket_, iocp_handle_, 1, 0);

    //Dll���� �Լ� �ε�(AcceptEx, TransmitFile)
    MsWinsockUtil::LoadExtensionFunction(listen_socket_);

    return true;
}

//==================================================================================================
/**
    @remarks
        ���� ������ �����ϰ� �ּҸ� �����Ѵ�.
    @retval BOOL
        ���� ���� ������ �����ϸ� false�� �����Ѵ�.
        (IP�� port�� ��ġ�� ����� ���̴�.)
*/
//==================================================================================================
BOOL Acceptor::CreateListenSocket(const char* ip_string, WORD port)
{
    // �̹� ���� ������ �����Ǿ� ������ ����
    if (listen_socket_ != INVALID_SOCKET) {
        return false;
    }

    // ���� ���� ����
    listen_socket_ = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
    if (listen_socket_ == INVALID_SOCKET) {
        return false;
    }
    //  // IO_PENDING���� ���� �������ν� ���� ó�� �ӵ��� ���δ�.
    //  // �̰��� ���� linger �ɼ� ����
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
        ���� �����带 ��� �����.
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
        ���� �����带 �ٽ� ������Ų��.
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
        ���� �����带 �����Ų��.
*/
//==================================================================================================
void Acceptor::Shutdown()
{
    // ���� ������ �ٽ� ������Ų��.
    ResumeListenThread();

    // ���� ���� ����
    if (listen_socket_ != INVALID_SOCKET)
    {
        ::closesocket(listen_socket_);
        listen_socket_ = INVALID_SOCKET;
    }
    // acceptor ������ ����
    for (DWORD i = 0; i < number_of_threads_; ++i) {
        PostQueuedCompletionStatus(iocp_handle_, 0, 0, NULL);
    }
    // ��� ��Ŀ �����尡 ����� ������ ��ٸ���.
    WaitForMultipleObjects(number_of_threads_, thread_handles_, true, INFINITE);
}
