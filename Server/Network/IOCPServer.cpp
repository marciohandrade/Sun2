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
        InPimpl �ɹ� ����
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
        ���� ����Ʈ�� ��ȸ�ϸ� ������ ���ۿ� �ִ� ��Ŷ���� �����ϴ� ������
    @param  param
        IOCP ������ ������
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
        ������ �ʱ�ȭ�Ѵ�.
    @param  description
        ���� �ʱ�ȭ�� �ʿ��� ������ ���� ����ü�� ������
    @retval BOOL
        ���� �ʱ�ȭ�� �����ϰų� ���� ���� ������ �����ϸ� false�� �����Ѵ�.
*/
//==================================================================================================
BOOL IOCPServer::Init(LPIOHANDLER_DESC description, DWORD number_of_io_handlers)
{
    // ���� �ʱ�ȭ
    if (!InitWinsock()) {
        return false;
    }
    for (DWORD i = 0; i < number_of_io_handlers; ++i) {
        CreateIoHandler(description + i);
    }

    // Send ��Ŀ ������ ����
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
// ��� �߰�. ���� ��Ŷ���� ���¸� �˱����ؼ� �߰�!
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

    // Send ��Ŀ �����尡 ����� ������ ��ٸ���.
    WaitForSingleObject(in_impl_->send_thread_handle_, INFINITE);

    // IoHandler���� �����Ű�� �����͸� �����Ѵ�.
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
        Accept�� ���ǰ� Connect�� ���ǵ��� IOCP�� ����ϸ鼭 Ȱ�� ����Ʈ�� �ű��
        ��� ������ �ޱ� ���۸� �˻��Ͽ� ��Ŷ�� ó���ϴ� ���� �۾��� �Ѵ�.
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
        - connect �����带 ����Ͽ� ������ �õ��Ѵ�.
    @param  io_handler_key
        - ������ �õ��ϱ� ���� ������ ������ �ִ� I/O �ڵ鷯�� Ű
        - ���� ���� �Ǵ� ���н� OnConnect(BOOL)�� ȣ��ȴ�.
    @param  szIP
        ������ IP
    @param  port
        ������ ��Ʈ
    @retval DWORD
        ������ �õ��� ������ �ε���
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
        ������ �ʱ�ȭ�Ѵ�.
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
