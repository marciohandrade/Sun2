#include "stdafx.h"
#include "IoHandler.h"
#include "Acceptor.h"
#include "Connector.h"
#include "SessionList.h"
#include "SessionPool.h"

#include "Session.h"
#include "SendBuffer.h"
#include "RecvBuffer.h"
#include "IOCPServer.h"
#include "NetworkObject.h"

//==================================================================================================
//  <IoHandler IMPLEMENTATION>
IoHandler::IoHandler()
{
    ZeroMemory(field_start_pos_, static_cast<size_t>(field_end_pos_ - field_start_pos_));
}

IoHandler::~IoHandler()
{
    if (!shutdown_) {
        Shutdown();
    }
    for (int i = 0 ; i < (sizeof(io_threads_) / sizeof(io_threads_[0])); ++i)
    {
        if (io_threads_[i]) {
            CloseHandle(io_threads_[i]);
        }
    }

    SAFE_DELETE(accept_session_pool_);
    SAFE_DELETE(connect_session_pool_);
    SAFE_DELETE(acceptor_);
    SAFE_DELETE(connector_);
    SAFE_DELETE(active_session_list_);
    SAFE_DELETE(accepted_session_list_);
    SAFE_DELETE(connect_success_list_);
    SAFE_DELETE(connect_fail_list_);
    SAFE_DELETE(temp_session_list_);
    SAFE_DELETE(temp_session_pending_list_);

    if (working_slots_for_sender_[0]) {
        ::free(working_slots_for_sender_[0]);
        working_slots_for_sender_[0] = 0;
    };
    if (working_slots_for_sender_[1]) {
        ::free(working_slots_for_sender_[1]);
        working_slots_for_sender_[1] = 0;
    };
}

void IoHandler::Init(IOCPServer* iocp_server, LPIOHANDLER_DESC description)
{
    iocp_server_ = iocp_server;

    // �ݹ� �Լ� ������ ��ȿ�� �˻�
    assert(!IsBadReadPtr(description->fnCreateAcceptedObject,
                         sizeof(description->fnCreateAcceptedObject)));
    assert(!IsBadReadPtr(description->fnDestroyAcceptedObject,
                         sizeof(description->fnDestroyAcceptedObject)));
    assert(!IsBadReadPtr(description->fnDestroyConnectedObject,
                         sizeof(description->fnDestroyConnectedObject)));

    // �ݹ� �Լ� ������ ����
    fnCreateAcceptedObject_ = description->fnCreateAcceptedObject;
    fnDestroyAcceptedObject_ = description->fnDestroyAcceptedObject;
    fnDestroyConnectedObject_ = description->fnDestroyConnectedObject;

    io_handler_key_ = description->dwIoHandlerKey;

    // ���� ����Ʈ ����
    active_session_list_ = new SessionList;
    accepted_session_list_ = new SessionList;
    connect_success_list_ = new SessionList;
    connect_fail_list_ = new SessionList;
    temp_session_list_ = new SessionList;
    temp_session_pending_list_ = new SessionList;

    // �ִ� ���� ���� �ο�
    max_number_of_accept_sessions_ = description->dwMaxAcceptSession;
    max_number_of_connect_sessions_ = description->dwMaxConnectSession;

    // ���� Ǯ ����
    accept_session_pool_ = new SessionPool(max_number_of_accept_sessions_,
        description->dwSendBufferSize,
        description->dwRecvBufferSize,
        description->dwMaxPacketSize,
        description->dwTimeOut,
        1,
        true);

    connect_session_pool_ = new SessionPool(max_number_of_connect_sessions_,
        description->dwSendBufferSize,
        description->dwRecvBufferSize,
        description->dwMaxPacketSize,
        description->dwTimeOut,
        accept_session_pool_->GetLength() + 1,
        false);

    ;{
        PACKET_HEADER header;
        header.size = (WORD)description->dwMaxPacketSize;
        assert((DWORD)header.size == description->dwMaxPacketSize);
    };

    // �ִ� ��Ŷ ũ�� ����
    max_packet_size_ = description->dwMaxPacketSize;
    //----------------------------------------------------------------------------------------------
    // NOTE: f110615.3L, added array to switch valid active list
    if (DWORD max_number_of_sessions = accept_session_pool_->GetNumberOfNodes() +
                                       connect_session_pool_->GetNumberOfNodes())
    {
        number_of_working_list_nodes_for_sender_ = max_number_of_sessions;
        size_t size_of_array = sizeof(Session*) * (max_number_of_sessions + 1);
        working_slots_for_sender_[0] = reinterpret_cast<Session**>(::malloc(size_of_array));
        working_slots_for_sender_[1] = reinterpret_cast<Session**>(::malloc(size_of_array));
        ZeroMemory(working_slots_for_sender_[0], size_of_array);
        ZeroMemory(working_slots_for_sender_[1], size_of_array);
        // oooo\0 - \0 is end delimiter
        working_list_for_sender_ = working_slots_for_sender_[0];
    };
    //----------------------------------------------------------------------------------------------
    // IOCP ����
    iocp_handle_ = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
    // IO ��Ŀ ������ ����
    assert(description->dwNumberOfIoThreads <= MAX_IO_WORKER_THREAD);
    number_of_io_threads_ = description->dwNumberOfIoThreads;

    for (DWORD i = 0; i < number_of_io_threads_; ++i)
    {
        unsigned int thread_id;
        io_threads_[i] = (HANDLE)_beginthreadex(NULL, 0, io_thread, this, 0, &thread_id);
    }
    // NOTE: the number of thread contexts are enough
    // to support the acceptor and the connector with each one.
    // ����� ����
    acceptor_ = new Acceptor;
    acceptor_->Init(this, (description->dwNumberOfAcceptThreads ? 1 : 0));

    // Ŀ���� ����   
    connector_ = new Connector;
    connector_->Init(this, (description->dwNumberOfConnectThreads ? 1 : 0));
}

DWORD IoHandler::Connect(NetworkObject* network_object, char* ip_string, WORD port)
{
    // �ش� NetworkObject�� ������ �̹� Ŀ��Ʈ �õ����̸� �����Ѵ�.
    if (network_object->session_ != NULL) {
        return 0;
    }
    // ���� ����
    SOCKET hSOCKET = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);

    // �ּ� ����
    SOCKADDR_IN addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ip_string);
    addr.sin_port = htons(port);

    // ���� �Ҵ�
    Session* session = AllocConnectSession();
    assert((session != NULL) &&
           "������ ���ڶ� Connect �� �� �����ϴ�. dwMaxConnectSession ������ Ȯ�����ּ���");

    // ���ǿ� ���� �� �ּ� ����
    session->SetSocket(hSOCKET);
    session->SetSockAddr(addr);

    // ������ ��Ʈ�� ������Ʈ�� ���ε�
    assert(network_object != NULL);
    session->BindNetworkObject(network_object);

    connector_->Connect(session);

    return session->GetIndex();
}

//==================================================================================================
/**
    @remarks
        ���� ������ �����ϰ� ������ �����Ѵ�.
    @retval BOOL
        ���� ���� ������ �����ϸ� false�� �����Ѵ�.
    @param  ip_string
        ������ ����� IP
    @param  port
        ������ ����� ��Ʈ
*/
//==================================================================================================
BOOL IoHandler::StartListen(char* ip_string, WORD port)
{
    assert(max_number_of_accept_sessions_ > 0);

    // �̹� �������̶�� ���� ó��
    if (IsListening()) {
        return true;
    }
    if (!acceptor_->StartListen(ip_string, port))
    {
        assert(!"Listen socket creation failed.");
        return false;
    }

    while (Session* session = AllocAcceptSession()) {
        session->PreAccept(acceptor_->GetListenSocket());
    }
    return true;
}

BOOL IoHandler::IsListening()
{
    return acceptor_->IsListening();
}

void IoHandler::SuspendListenThread()
{
    acceptor_->SuspendListenThread();
}

void IoHandler::ResumeListenThread()
{
    acceptor_->ResumeListenThread();
}

///---------------------------------------------------------------------------
///---------------------------------------------------------------------------
// ��� �߰�. ������ �� �� ������ SendBuffer ������ �������ؼ� �������!
///---------------------------------------------------------------------------
///---------------------------------------------------------------------------
Session* IoHandler::GetSession(DWORD session_index)
{
    SESSION_LIST::iterator end = active_session_list_->end();
    for (SESSION_LIST::iterator it=active_session_list_->begin() ; it != end ; ++it)
    {
        Session* session = *it;
        if (session->GetIndex() == session_index) {
            return session;
        }
    }
    return NULL;
}

//==================================================================================================
/**
    @remarks
        ������̴� ���ǵ��� ��� ����Ǯ�� ��ȯ�Ѵ�.
*/
//==================================================================================================

Session* IoHandler::AllocAcceptSession()
{
    if (Session* session = accept_session_pool_->Alloc())
    {
        session->Init();
        return session;
    };
    return NULL;
}

Session* IoHandler::AllocConnectSession()
{
    if (Session* session = connect_session_pool_->Alloc())
    {
        session->Init();
        return session;
    };
    return NULL;
}

void IoHandler::FreeSession(Session* session)
{
    //printf("[FreeSession][%d]\n", (int)session->GetSocket());
    // NOTE: f110615.2L, the calling of Session::Init() moved to process on resource allocation.
    session->CloseSocket();
    if (session->IsAcceptSocket()) {
        accept_session_pool_->Free(session);
    }
    else {
        connect_session_pool_->Free(session);
    }
}

void IoHandler::ReuseSession(Session* session)
{
    //printf("[ReuseSession][%d]\n", (int)session->GetSocket());
    session->Reuse(iocp_handle_);
}

//==================================================================================================
/**
    @remarks
        ���ӿ� ������ ���� ����Ʈ ó��
    @par
        ConnectSuccessList�� �ִ� ���ǵ��� IOCP�� ����ϰ� OnConnect(true)�� ȣ���ϰ� ActiveSessionList�� �߰��Ѵ�.
*/
//==================================================================================================
void IoHandler::ProcessConnectSuccessList()
{
    // Ŀ��Ʈ�� ������ ���ǵ��� �ӽ� ����Ʈ�� �ű�
    connect_success_list_->Lock();
    temp_session_list_->splice(temp_session_list_->end(), *connect_success_list_);
    connect_success_list_->Unlock();
    //----------------------------------------------------------------------------------------------
    SESSION_LIST::iterator it = temp_session_list_->begin();
    SESSION_LIST::const_iterator end = temp_session_list_->end();
    while (it != end)
    {
        Session* const session = *it;

        // IOCP�� �ڵ� ���
        CreateIoCompletionPort((HANDLE)session->GetSocket(), iocp_handle_, (ULONG_PTR)session, 0);
        session->Init();
        if (session->PreRecv() == false)
        {
            session->OnLogString("[ProcessConnectSuccessList] PreRecv Fail!");
            it = temp_session_list_->erase(it);

            // ���� Ǯ�� ��ȯ
            FreeSession(session);
            session->OnConnect(false);
            continue;
        };
        // success
        session->OnConnect(true);
        ++it;
    };
    //----------------------------------------------------------------------------------------------
    if (temp_session_list_->empty() == false)
    {
        // ������ ����
        number_of_connect_sessions_ += (DWORD)temp_session_list_->size();

        // Ŀ��Ʈ�� ������ ���ǵ��� ActiveSessionList�� �߰��Ѵ�.
        active_session_list_->Lock();
        active_session_list_->splice(active_session_list_->end(), *temp_session_list_);
        active_session_list_->Unlock();
    };
}

//==================================================================================================
/**
    @remarks
        ���ӿ� ������ ���� ����Ʈ ó��
    @par
        ConnectFailList�� �ִ� ���ǵ鿡 ���� OnConnect(false)�� ȣ���Ѵ�.
*/
//==================================================================================================
void IoHandler::ProcessConnectFailList()
{
    connect_fail_list_->Lock();

    SESSION_LIST::iterator end = connect_fail_list_->end();
    for (SESSION_LIST::iterator it = connect_fail_list_->begin(); it != end ; ++it)
    {
        Session* session = *it;

        // ���� Ǯ�� ��ȯ
        FreeSession(session);

        session->OnConnect(false);
    }

    connect_fail_list_->clear();
    connect_fail_list_->Unlock();
}

//==================================================================================================
/**
    @remarks
        TempSessionList�� �ִ� ���ǵ��� IOCP�� ����ϰ� ActiveSessionList�� �ű�� OnAccpet()�� ȣ���Ѵ�.
*/
//==================================================================================================
void IoHandler::ProcessAcceptedSessionList()
{
    // ���ӿ� ������ ���ǵ��� �޾Ƶ� �ӽ� ����Ʈ�� �ű�
    accepted_session_list_->Lock();
    temp_session_list_->splice(temp_session_list_->end(), *accepted_session_list_);
    accepted_session_list_->Unlock();

    SESSION_LIST::iterator end = temp_session_list_->end();
    // ���ӿ� ������ ���ǿ� ���� ó��
    for (SESSION_LIST::iterator it = temp_session_list_->begin(); it != end;)
    {
        Session* session = *it;

        // �ְ������� �ʰ��ϴ� ��� ����
        if (number_of_accept_sessions_ >= max_number_of_accept_sessions_)
        {
            session->OnLogString("connection full! no available accept socket!");
            it = temp_session_list_->erase(it);
            ReuseSession(session);
            continue;
        }

        // IOCP�� �ڵ� ���
        CreateIoCompletionPort((HANDLE)session->GetSocket(), iocp_handle_, (ULONG_PTR)session, 0);

        // Recv�� �����ϴ� ��� ó��
        if (!session->PreRecv())
        {
            session->OnLogString("[ProcessAcceptedSessionList] PreRecv Fail!");
            it = temp_session_list_->erase(it);
            ReuseSession(session);
            continue;
        }

        //--------------------------------
        // ���������� ���ӵ� ���� ó��
        //--------------------------------

        // ��Ʈ�� ������Ʈ ���� ��û
        NetworkObject* network_object = fnCreateAcceptedObject_();
        assert(network_object);

        // ��Ʈ�� ������Ʈ ���ε�
        session->BindNetworkObject(network_object);

        // ���ӽ� �ʱ�ȭ �� NetworkObject�� ���� ����
        session->OnAccept();

        // ������ ����
        ++number_of_accept_sessions_;
        // last
        ++it;
    }

    if (!temp_session_list_->empty())
    {
        // ���ӿ� ������ ���ǵ��� ActiveSessionList�� �߰�
        active_session_list_->Lock();
        active_session_list_->splice(active_session_list_->begin(), *temp_session_list_);
        active_session_list_->Unlock();
    }
}

//==================================================================================================
/**
    @remarks
        ���� accept �Ǵ� connect�Ǿ� Ȱ��ȭ�� ���ǵ��� ��ȸ�ϸ� ���� ��Ŷ ���� ó���Ѵ�.
*/
//==================================================================================================
void IoHandler::ProcessActiveSessionList()
{
    SESSION_LIST::iterator end = active_session_list_->end();
    for (SESSION_LIST::iterator it = active_session_list_->begin(); it!=end ; ++it)
    {
        Session* session = *it;

        if (session->ShouldBeRemoved()) {
            continue;
        }
        if (session->HasDisconnectOrdered())
        {
            // ������ ���۸� ���� ����� �ϴ� ������ ó��
            // NOTE: f110623.5L, added condition to prevent the deadlocked state.
            SendBuffer* const send_buffer = session->GetSendBuffer();
            if (bool empty_buffer = send_buffer->IsEmpty())
            {
            #ifdef __SUSUNLOVE_REMOVE_ONLOGSTRING_APPLY_20080110
                session->OnLogString("[REMOVE] BufferSize 0 \n");
            #else
              #ifdef _DEBUG
                session->OnLogString("[REMOVE] BufferSize 0 \n");
              #endif
            #endif
                session->Remove();
            }
        }
        else
        {
            if (session->IsAcceptSocket() && session->IsOnIdle())
            {
                session->OnLogString("Idle Session is disconnected");
                session->Remove();
                continue;
            }

            // �ޱ� ���� ó��
            if (!session->ProcessReceivedPacket(max_packet_size_))
            {
            #ifdef __SUSUNLOVE_REMOVE_ONLOGSTRING_APPLY_20080110
                session->OnLogString("[REMOVE] ProcessReceivedPacket\n");
            #else
              #ifdef _DEBUG
                session->OnLogString("[REMOVE] ProcessReceivedPacket\n");
              #endif
            #endif
                session->Remove();
            }
        }
    } // end for
}

//==================================================================================================
/**
    @remarks
        ���� �ð� �̻� ��Ŷ �۽��� ���� ���ǵ��� �����Ѵ�.
*/
//==================================================================================================
void IoHandler::KickDeadSessions()
{
    //----------------------------------------------------------------------------------------------
    // NOTE: f110615.3L, added array to switch valid active list
    Session** working_list = (this->working_list_for_sender_ != working_slots_for_sender_[0]) ?
        working_slots_for_sender_[0] : working_slots_for_sender_[1];

    //----------------------------------------------------------------------------------------------
    // ���ŵ� ���ǵ��� ActiveSessionList���� �����ϰ� �ӽ� ����Ʈ�� ��´�.
    active_session_list_->Lock();
    ;{
        Session** working_list_it = working_list;
        SESSION_LIST::iterator it = active_session_list_->begin();
        SESSION_LIST::const_iterator end = active_session_list_->end();
        while (it != end)
        {
            Session* session = *it;
            if (session->ShouldBeRemoved())
            {
                it = active_session_list_->erase(it);
                temp_session_list_->push_back(session);
                continue;
            };
            // NOTE: f110616.5L
            session->ResumeReception();
            *working_list_it++ = session;
            ++it;
        };
        *working_list_it = NULL;
    };
    active_session_list_->Unlock();
    //
    InterlockedExchangePointer((PVOID*)&this->working_list_for_sender_, working_list);
    //
#if 0
    active_session_list_->Lock();
    ;{
        SESSION_LIST::iterator it = active_session_list_->begin();
        SESSION_LIST::const_iterator end = active_session_list_->end();
        while (it != end)
        {
            Session* session = *it;
            if (session->ShouldBeRemoved())
            {
                it = active_session_list_->erase(it);
                temp_session_list_->push_back(session);
                continue;
            }
            ++it;
        }
    };
    active_session_list_->Unlock();
#endif
    //----------------------------------------------------------------------------------------------
    ;{
        if (temp_session_pending_list_->empty() == false) {
            temp_session_list_->splice(temp_session_list_->end(), *temp_session_pending_list_);
        };
        SESSION_LIST::iterator it = temp_session_list_->begin();
        SESSION_LIST::const_iterator end = temp_session_list_->end();

        // ���ŵ� ���ǵ鿡 ���� ó��
        while (it != end)
        {
            Session* const session = *it;
            // NOTE: f110623.5L, the below checker has caused the deadlocked state.
            // 'f110615.1L, to prevent the removing session while sending' that is previous notice
            // can solve the 'Clear' method.
#if 0
            // NOTE: f110615.1L, added condition to prevent the removing session while sending.
            if (SendBuffer* send_buffer = session->GetSendBuffer())
            {   // NOTE: short-term interlock checking, it may occur
                // that send_thread has been send preparation after 'session->ShouldBeRemoved()'
                if (send_buffer->HasBeenSendWorking())
                {
                    it = temp_session_list_->erase(it);
                    temp_session_pending_list_->push_back(session);
                    continue;
                };
            };
#endif
            const bool is_accept_socket = session->IsAcceptSocket();
            // ������ ����
            (is_accept_socket) ? --number_of_accept_sessions_ : --number_of_connect_sessions_;

            NetworkObject* const network_object = session->GetNetworkObject();

            // ��Ʈ�� ������Ʈ ����ε�
            session->UnbindNetworkObject();

            // �ݹ��Լ� ȣ�� ���� ������ free ���ش�. �ݹ� �ȿ��� Connect��û�� ���� �����Ƿ�..
            // accept�� �����̸� ������ �����Ͽ� �ٽ� accept�� �ɰ�, connect�� �����̸� Ǯ�� ��ȯ�Ѵ�.
            if (is_accept_socket && !shutdown_) {
                ReuseSession(session);
            }
            else {
                FreeSession(session);
            }
            // NetworkObject�� ��Ŀ��Ʈ ����
            network_object->OnDisconnect();

            // ��Ʈ�� ������Ʈ �Ҹ� ��û       
            if (is_accept_socket) {
                fnDestroyAcceptedObject_(network_object); 
            }
            else {
                fnDestroyConnectedObject_(network_object); 
            };

            ++it;
        }; //foreach 'temp_session_list'

        temp_session_list_->clear();
    };
}

void IoHandler::ProcessSend()
{
    // need Switch Buffer Logic
    // ActiveSessionList�� ������ ������ �۹� ����
    //----------------------------------------------------------------------------------------------
    // NOTE: f110615.3L, used array to lockless for each buffer control
    long number_of_max_working_array = this->number_of_working_list_nodes_for_sender_;
    Session** io_sessions = this->working_list_for_sender_;
    while (Session* session = *io_sessions++)
    {
        if (number_of_max_working_array-- <= 0) {
            break;
        };
        if (session->ShouldBeRemoved()) {
            continue;
        };
        if (session->PreSend() == false) 
        {
        #ifdef __SUSUNLOVE_REMOVE_ONLOGSTRING_APPLY_20080110
            printf("[REMOVE][%d] session->PreSend() == false\n", (int)session->GetSocket());
        #endif
        #ifdef _DEBUG
            session->OnLogString("[REMOVE][%u] session->PreSend() == false\n", (int)session->GetSocket());
        #endif
            session->Remove(); // �̰��� active_session_list_�� ���Ű� �ƴϹǷ� end�� ���� �Ҵ��ؼ� ����Ѵ�.
        };
    };
#if 0
    active_session_list_->Lock();

    SESSION_LIST::iterator end = active_session_list_->end();
    for (SESSION_LIST::iterator it=active_session_list_->begin(); it!=end ; ++it)
    {
        Session* session = *it;

        if (session->ShouldBeRemoved()) {
            continue;
        }
        if (session->PreSend() == false) 
        {
        #ifdef __SUSUNLOVE_REMOVE_ONLOGSTRING_APPLY_20080110
            printf("[REMOVE][%d] session->PreSend() == false\n", (int)session->GetSocket());
        #endif
        #ifdef _DEBUG
            session->OnLogString("[REMOVE][%u] session->PreSend() == false\n", (int)session->GetSocket());
        #endif
            session->Remove(); // �̰��� active_session_list_�� ���Ű� �ƴϹǷ� end�� ���� �Ҵ��ؼ� ����Ѵ�.
        }
    }
    active_session_list_->Unlock();
#endif //0
}

//==================================================================================================
/**
    @remarks
        ��� ���ǵ��� �����Ѵ�.
*/
//==================================================================================================
void IoHandler::KickAllSessions()
{
    SESSION_LIST::iterator end = active_session_list_->end();
    for (SESSION_LIST::iterator it = active_session_list_->begin(); it != end ; ++it)
    {
        Session* session = *it;

    #ifdef __SUSUNLOVE_REMOVE_ONLOGSTRING_APPLY_20080110
        session->OnLogString("[REMOVE] KickAllSessions() \n");
    #endif
        session->Remove();
    }
}

//==================================================================================================
/**
    @remarks
        Accept�� ���ǰ� Connect�� ���ǵ��� IOCP�� ����ϸ鼭 ActiveSessionList�� �ű��
        ��� ������ �ޱ� ���۸� �˻��Ͽ� ��Ŷ�� ó���ϴ� ���� �۾��� �Ѵ�.
    @par
        �� �Լ��� �ֱ������� ȣ���� �־�� �Ѵ�.
*/
//==================================================================================================
void IoHandler::Update()
{
    ProcessActiveSessionList();

    if (!accepted_session_list_->empty()) {
        ProcessAcceptedSessionList();
    }
    if (!connect_success_list_->empty()) {
        ProcessConnectSuccessList();
    }
    if (!connect_fail_list_->empty()) {
        ProcessConnectFailList();
    }

    KickDeadSessions();
}


void IoHandler::Shutdown()
{
    shutdown_ = true;

    KickAllSessions();

    ProcessActiveSessionList();

    KickDeadSessions();

    acceptor_->Shutdown();

    connector_->Shutdown();

    // IOCP ������ ����
    for (DWORD i = 0; i < number_of_io_threads_; ++i)
    {
        PostQueuedCompletionStatus(iocp_handle_, 0, 0, NULL);
    }

    // IO ��Ŀ �����尡 ��� ����� ������ ��ٸ���.
    WaitForMultipleObjects(number_of_io_threads_, io_threads_, true, INFINITE);
}

