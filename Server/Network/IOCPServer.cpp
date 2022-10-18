#include "stdafx.h"
#include <conio.h>
#include <map>
#include "IOCPServer.h"
#include "IoHandler.h"
#include "SessionList.h"
#include "Session.h"
#include "SendBuffer.h"
#include "RecvBuffer.h"

typedef std::map<DWORD, IoHandler*,
                 std::less<DWORD>,
                 module::RedirectedAllocator<IoHandler*> > IOHANDLER_MAP;

//==================================================================================================
/**
    @remarks
        InPimpl 맴버 선언
*/
//==================================================================================================
struct IOCPServer::InPimpl
{
    HANDLE send_thread_handle_;
    bool shutdown_;
    IOHANDLER_MAP io_handler_table_;
};

//==================================================================================================
/**
    @remarks
        세션 리스트를 순회하며 보내기 버퍼에 있는 패킷들을 전송하는 쓰레드
    @param  param
        IOCP 서버의 포인터
*/
//==================================================================================================
unsigned __stdcall send_thread(LPVOID param)
{
    IOCPServer* const iocp_server = reinterpret_cast<IOCPServer*>(param);
    IOCPServer::InPimpl* const iocp_impl = iocp_server->in_impl_;
    while (iocp_impl->shutdown_ == false)
    {
        Sleep(1); // for context-switching, no busy-waiting
        IOHANDLER_MAP::const_iterator it = iocp_impl->io_handler_table_.begin();
        const IOHANDLER_MAP::const_iterator end = iocp_impl->io_handler_table_.end();
        for (; it != end; ++it)
        {
            IoHandler* const io_handler = it->second;
            io_handler->ProcessSend();
        }
    }

    return 0;
}

//==================================================================================================
//==================================================================================================
//==================================================================================================

IOCPServer::IOCPServer()
    : in_impl_(new InPimpl)
{
    in_impl_->shutdown_ = false;
    in_impl_->send_thread_handle_ = NULL;
}

IOCPServer::~IOCPServer()
{
    if (!in_impl_->shutdown_) {
        Shutdown();
    }
    if (in_impl_->send_thread_handle_) {
        CloseHandle(in_impl_->send_thread_handle_);
    }
    delete in_impl_; 
    in_impl_ = NULL;

    WSACleanup();
}

//==================================================================================================
/**
    @remarks
        서버를 초기화한다.
    @param  description
        서버 초기화에 필요한 정보를 담은 구조체의 포인터
    @retval BOOL
        윈속 초기화에 실패하거나 리슨 소켓 생성에 실패하면 false를 리턴한다.
*/
//==================================================================================================
BOOL IOCPServer::Init(LPIOHANDLER_DESC description, DWORD number_of_io_handlers)
{
    // 윈속 초기화
    if (!InitWinsock()) {
        return false;
    }
    for (DWORD i = 0; i < number_of_io_handlers; ++i) {
        CreateIoHandler(description + i);
    }

    // Send 워커 쓰레드 생성
    unsigned int thread_id;
    in_impl_->send_thread_handle_ = \
        (HANDLE)_beginthreadex(NULL, 0, &send_thread, (LPVOID)this, 0, &thread_id);

    return true;
}

void IOCPServer::CreateIoHandler(LPIOHANDLER_DESC description)
{
    IoHandler* const io_handler = new IoHandler;
    ;                io_handler->Init(this, description);

    in_impl_->io_handler_table_.insert(std::make_pair(io_handler->GetKey(), io_handler));
}

///---------------------------------------------------------------------------
///---------------------------------------------------------------------------
// 담덕 추가. 현재 패킷버퍼 상태를 알기위해서 추가!
///---------------------------------------------------------------------------
///---------------------------------------------------------------------------
DWORD IOCPServer::GetMaxBuffer(DWORD io_handler_key, DWORD session_index,
                               BOOL is_send_buffer, BOOL is_select_max_buffer)
{
    IoHandler* const io_handler = GetIoHandler(io_handler_key);
    if (io_handler == NULL) {
        return 0;
    }
    //
    Session* const session = io_handler->GetSession(session_index);
    if (session == NULL) {
        return 0;
    }
    //
    DWORD max_buffer_size = 0;
    DWORD current_used_size = 0;
    if (is_send_buffer)
    {
        const SendBuffer* send_buffer = session->GetSendBuffer();
        // lockless check
        send_buffer->GetBufferSizeInfo(max_buffer_size, current_used_size);
    }
    else
    {
        const RecvBuffer* recv_buffer = session->GetRecvBuffer();
        // lockless check
        recv_buffer->GetBufferSizeInfo(max_buffer_size, current_used_size);
    }
    return is_select_max_buffer ? max_buffer_size : current_used_size;
}

void IOCPServer::Display(DWORD io_handler_key, DWORD over_boundary_length)
{
    IoHandler* const io_handler = GetIoHandler(io_handler_key);
    if (io_handler == NULL) {
        return;
    }
    SessionList* const active_sessions = io_handler->GetActiveSessionList();
    SESSION_LIST::const_iterator it = active_sessions->begin();
    const SESSION_LIST::const_iterator end = active_sessions->end();
    DWORD max_buffer_size = 0;
    DWORD current_used_size = 0;

    for ( ; it != end; ++it)
    {
        Session* session = *it;
        const SendBuffer* send_buffer = session->GetSendBuffer();
        // lockless check
        send_buffer->GetBufferSizeInfo(max_buffer_size, current_used_size);
        if (current_used_size >= over_boundary_length)
        {
            session->OnLogString("Session[%d] SendBuffer Size[%d]",
                                 session->GetIndex(), current_used_size);
        }
        const RecvBuffer* recv_buffer = session->GetRecvBuffer();
        // lockless check
        recv_buffer->GetBufferSizeInfo(max_buffer_size, current_used_size);
        if (current_used_size >= over_boundary_length)
        {
            session->OnLogString("Session[%d] RecvBuffer Size[%d]",
                                 session->GetIndex(), current_used_size);
        }
    }
}

IoHandler* IOCPServer::GetIoHandler(DWORD io_handler_key)
{
    const IOHANDLER_MAP& io_handler_table = in_impl_->io_handler_table_;
    IOHANDLER_MAP::const_iterator found = io_handler_table.find(io_handler_key);
    if (found != io_handler_table.end())
    {
        IoHandler* io_handler = found->second;
        return io_handler;
    }
    return NULL;
}

void IOCPServer::Shutdown()
{
    in_impl_->shutdown_ = true;

    // Send 워커 쓰레드가 종료될 때까지 기다린다.
    WaitForSingleObject(in_impl_->send_thread_handle_, INFINITE);

    // IoHandler들을 종료시키고 포인터를 삭제한다.
    IOHANDLER_MAP::const_iterator it = in_impl_->io_handler_table_.begin();
    const IOHANDLER_MAP::const_iterator end = in_impl_->io_handler_table_.end();
    for (; it != end; ++it)
    {
        IoHandler* io_handler = it->second;
        ;          io_handler->Shutdown();
        delete io_handler;
    }
    in_impl_->io_handler_table_.clear();
}

//==================================================================================================
/**
    @remarks
        Accept된 세션과 Connect된 세션들을 IOCP에 등록하면서 활성 리스트로 옮기고
        모든 세션의 받기 버퍼를 검사하여 패킷을 처리하는 등의 작업을 한다.
*/
//==================================================================================================
void IOCPServer::Update()
{
    IOHANDLER_MAP::const_iterator it = in_impl_->io_handler_table_.begin();
    const IOHANDLER_MAP::const_iterator end = in_impl_->io_handler_table_.end();
    for (; it != end; ++it)
    {
        IoHandler* io_handler = it->second;
        ;          io_handler->Update();
    }
}

BOOL IOCPServer::StartListen(DWORD io_handler_key, char* ip_string, WORD port)
{
    IOHANDLER_MAP::const_iterator found = in_impl_->io_handler_table_.find(io_handler_key);
    const bool must_be_find_io_handler = (found != in_impl_->io_handler_table_.end());
    assert(must_be_find_io_handler);
    if (must_be_find_io_handler == false) {
        return false;
    }
    IoHandler* io_handler = found->second;
    return io_handler->StartListen(ip_string, port);
}

//==================================================================================================
/**
    @remarks
        - connect 쓰레드를 사용하여 접속을 시도한다.
    @param  io_handler_key
        - 접속을 시도하기 위한 세션을 가지고 있는 I/O 핸들러의 키
        - 접속 성공 또는 실패시 OnConnect(BOOL)이 호출된다.
    @param  szIP
        접속할 IP
    @param  port
        접속할 포트
    @retval DWORD
        접속을 시도할 세션의 인덱스
*/
//==================================================================================================
DWORD IOCPServer::Connect(DWORD io_handler_key, NetworkObject* network_object,
                          char* ip_string, WORD port)
{
    if (network_object == NULL) {
        return 0;
    }
    IOHANDLER_MAP::const_iterator found = in_impl_->io_handler_table_.find(io_handler_key);
    const bool must_be_find_io_handler = (found != in_impl_->io_handler_table_.end());
    assert(must_be_find_io_handler);
    if (must_be_find_io_handler == false) {
        return 0;
    }
    IoHandler* io_handler = found->second;
    return io_handler->Connect(network_object, ip_string, port);
}

BOOL IOCPServer::IsListening(DWORD io_handler_key)
{
    IOHANDLER_MAP::const_iterator found = in_impl_->io_handler_table_.find(io_handler_key);
    const bool must_be_find_io_handler = (found != in_impl_->io_handler_table_.end());
    assert(must_be_find_io_handler);
    if (must_be_find_io_handler == false) {
        return false;
    }
    IoHandler* io_handler = found->second;
    return io_handler->IsListening();
}

void IOCPServer::SuspendListenThread(DWORD io_handler_key)
{
    IOHANDLER_MAP::const_iterator found = in_impl_->io_handler_table_.find(io_handler_key);
    const bool must_be_find_io_handler = (found != in_impl_->io_handler_table_.end());
    assert(must_be_find_io_handler);
    if (must_be_find_io_handler == false) {
        return;
    }
    IoHandler* io_handler = found->second;
    io_handler->SuspendListenThread();
}

void IOCPServer::ResumeListenThread(DWORD io_handler_key)
{
    IOHANDLER_MAP::const_iterator found = in_impl_->io_handler_table_.find(io_handler_key);
    const bool must_be_find_io_handler = (found != in_impl_->io_handler_table_.end());
    assert(must_be_find_io_handler);
    if (must_be_find_io_handler == false) {
        return;
    }
    IoHandler* io_handler = found->second;
    io_handler->ResumeListenThread();
}

DWORD IOCPServer::GetNumberOfConnections(DWORD io_handler_key)
{
    IOHANDLER_MAP::const_iterator found = in_impl_->io_handler_table_.find(io_handler_key);
    const bool must_be_find_io_handler = (found != in_impl_->io_handler_table_.end());
    assert(must_be_find_io_handler);
    if (must_be_find_io_handler == false) {
        return 0;
    }
    IoHandler* io_handler = found->second;
    return io_handler->GetNumberOfConnections();
}

//==================================================================================================
/**
    @remarks
        윈속을 초기화한다.
*/
//==================================================================================================
BOOL IOCPServer::InitWinsock()
{
    WSADATA wsaData;

    WSAStartup(MAKEWORD(2, 2), &wsaData);

    if (wsaData.wVersion != MAKEWORD(2, 2))
    {
        WSACleanup();
        return false;
    }

    return true;
}
