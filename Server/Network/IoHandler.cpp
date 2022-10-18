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

    // 콜백 함수 포인터 유효성 검사
    assert(!IsBadReadPtr(description->fnCreateAcceptedObject,
                         sizeof(description->fnCreateAcceptedObject)));
    assert(!IsBadReadPtr(description->fnDestroyAcceptedObject,
                         sizeof(description->fnDestroyAcceptedObject)));
    assert(!IsBadReadPtr(description->fnDestroyConnectedObject,
                         sizeof(description->fnDestroyConnectedObject)));

    // 콜백 함수 포인터 저장
    fnCreateAcceptedObject_ = description->fnCreateAcceptedObject;
    fnDestroyAcceptedObject_ = description->fnDestroyAcceptedObject;
    fnDestroyConnectedObject_ = description->fnDestroyConnectedObject;

    io_handler_key_ = description->dwIoHandlerKey;

    // 세션 리스트 생성
    active_session_list_ = new SessionList;
    accepted_session_list_ = new SessionList;
    connect_success_list_ = new SessionList;
    connect_fail_list_ = new SessionList;
    temp_session_list_ = new SessionList;
    temp_session_pending_list_ = new SessionList;

    // 최대 접속 제한 인원
    max_number_of_accept_sessions_ = description->dwMaxAcceptSession;
    max_number_of_connect_sessions_ = description->dwMaxConnectSession;

    // 세션 풀 생성
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

    // 최대 패킷 크기 설정
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
    // IOCP 생성
    iocp_handle_ = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
    // IO 워커 쓰레드 생성
    assert(description->dwNumberOfIoThreads <= MAX_IO_WORKER_THREAD);
    number_of_io_threads_ = description->dwNumberOfIoThreads;

    for (DWORD i = 0; i < number_of_io_threads_; ++i)
    {
        unsigned int thread_id;
        io_threads_[i] = (HANDLE)_beginthreadex(NULL, 0, io_thread, this, 0, &thread_id);
    }
    // NOTE: the number of thread contexts are enough
    // to support the acceptor and the connector with each one.
    // 억셉터 생성
    acceptor_ = new Acceptor;
    acceptor_->Init(this, (description->dwNumberOfAcceptThreads ? 1 : 0));

    // 커넥터 생성   
    connector_ = new Connector;
    connector_->Init(this, (description->dwNumberOfConnectThreads ? 1 : 0));
}

DWORD IoHandler::Connect(NetworkObject* network_object, char* ip_string, WORD port)
{
    // 해당 NetworkObject의 소켓이 이미 커넥트 시도중이면 리턴한다.
    if (network_object->session_ != NULL) {
        return 0;
    }
    // 소켓 생성
    SOCKET hSOCKET = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);

    // 주소 셋팅
    SOCKADDR_IN addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ip_string);
    addr.sin_port = htons(port);

    // 세션 할당
    Session* session = AllocConnectSession();
    assert((session != NULL) &&
           "세션이 모자라서 Connect 할 수 없습니다. dwMaxConnectSession 개수를 확인해주세요");

    // 세션에 소켓 및 주소 셋팅
    session->SetSocket(hSOCKET);
    session->SetSockAddr(addr);

    // 세션을 네트웍 오브젝트와 바인딩
    assert(network_object != NULL);
    session->BindNetworkObject(network_object);

    connector_->Connect(session);

    return session->GetIndex();
}

//==================================================================================================
/**
    @remarks
        리슨 소켓을 생성하고 리슨을 시작한다.
    @retval BOOL
        리슨 소켓 생성에 실패하면 false를 리턴한다.
    @param  ip_string
        리슨에 사용할 IP
    @param  port
        리슨에 사용할 포트
*/
//==================================================================================================
BOOL IoHandler::StartListen(char* ip_string, WORD port)
{
    assert(max_number_of_accept_sessions_ > 0);

    // 이미 리슨중이라면 성공 처리
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
// 담덕 추가. 세션을 얻어서 그 세션의 SendBuffer 정보를 보기위해서 만들었음!
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
        사용중이던 세션들을 모두 세션풀에 반환한다.
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
        접속에 성공한 세션 리스트 처리
    @par
        ConnectSuccessList에 있는 세션들을 IOCP에 등록하고 OnConnect(true)를 호출하고 ActiveSessionList에 추가한다.
*/
//==================================================================================================
void IoHandler::ProcessConnectSuccessList()
{
    // 커넥트에 성공한 세션들을 임시 리스트로 옮김
    connect_success_list_->Lock();
    temp_session_list_->splice(temp_session_list_->end(), *connect_success_list_);
    connect_success_list_->Unlock();
    //----------------------------------------------------------------------------------------------
    SESSION_LIST::iterator it = temp_session_list_->begin();
    SESSION_LIST::const_iterator end = temp_session_list_->end();
    while (it != end)
    {
        Session* const session = *it;

        // IOCP에 핸들 등록
        CreateIoCompletionPort((HANDLE)session->GetSocket(), iocp_handle_, (ULONG_PTR)session, 0);
        session->Init();
        if (session->PreRecv() == false)
        {
            session->OnLogString("[ProcessConnectSuccessList] PreRecv Fail!");
            it = temp_session_list_->erase(it);

            // 세션 풀에 반환
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
        // 동접수 증가
        number_of_connect_sessions_ += (DWORD)temp_session_list_->size();

        // 커넥트에 성공한 세션들을 ActiveSessionList에 추가한다.
        active_session_list_->Lock();
        active_session_list_->splice(active_session_list_->end(), *temp_session_list_);
        active_session_list_->Unlock();
    };
}

//==================================================================================================
/**
    @remarks
        접속에 실패한 세션 리스트 처리
    @par
        ConnectFailList에 있는 세션들에 대해 OnConnect(false)을 호출한다.
*/
//==================================================================================================
void IoHandler::ProcessConnectFailList()
{
    connect_fail_list_->Lock();

    SESSION_LIST::iterator end = connect_fail_list_->end();
    for (SESSION_LIST::iterator it = connect_fail_list_->begin(); it != end ; ++it)
    {
        Session* session = *it;

        // 세션 풀에 반환
        FreeSession(session);

        session->OnConnect(false);
    }

    connect_fail_list_->clear();
    connect_fail_list_->Unlock();
}

//==================================================================================================
/**
    @remarks
        TempSessionList에 있는 세션들을 IOCP에 등록하고 ActiveSessionList로 옮기고 OnAccpet()를 호출한다.
*/
//==================================================================================================
void IoHandler::ProcessAcceptedSessionList()
{
    // 접속에 성공한 세션들을 받아둔 임시 리스트로 옮김
    accepted_session_list_->Lock();
    temp_session_list_->splice(temp_session_list_->end(), *accepted_session_list_);
    accepted_session_list_->Unlock();

    SESSION_LIST::iterator end = temp_session_list_->end();
    // 접속에 성공한 세션에 대한 처리
    for (SESSION_LIST::iterator it = temp_session_list_->begin(); it != end;)
    {
        Session* session = *it;

        // 최고동접수를 초과하는 경우 실패
        if (number_of_accept_sessions_ >= max_number_of_accept_sessions_)
        {
            session->OnLogString("connection full! no available accept socket!");
            it = temp_session_list_->erase(it);
            ReuseSession(session);
            continue;
        }

        // IOCP에 핸들 등록
        CreateIoCompletionPort((HANDLE)session->GetSocket(), iocp_handle_, (ULONG_PTR)session, 0);

        // Recv에 실패하는 경우 처리
        if (!session->PreRecv())
        {
            session->OnLogString("[ProcessAcceptedSessionList] PreRecv Fail!");
            it = temp_session_list_->erase(it);
            ReuseSession(session);
            continue;
        }

        //--------------------------------
        // 성공적으로 접속된 세션 처리
        //--------------------------------

        // 네트웍 오브젝트 생성 요청
        NetworkObject* network_object = fnCreateAcceptedObject_();
        assert(network_object);

        // 네트웍 오브젝트 바인딩
        session->BindNetworkObject(network_object);

        // 접속시 초기화 및 NetworkObject로 접속 통지
        session->OnAccept();

        // 동접수 증가
        ++number_of_accept_sessions_;
        // last
        ++it;
    }

    if (!temp_session_list_->empty())
    {
        // 접속에 성공한 세션들을 ActiveSessionList에 추가
        active_session_list_->Lock();
        active_session_list_->splice(active_session_list_->begin(), *temp_session_list_);
        active_session_list_->Unlock();
    }
}

//==================================================================================================
/**
    @remarks
        현재 accept 또는 connect되어 활성화된 세션들을 순회하며 받은 패킷 등을 처리한다.
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
            // 보내기 버퍼를 비우고 끊어야 하는 세션의 처리
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

            // 받기 버퍼 처리
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
        일정 시간 이상 패킷 송신이 없는 세션들을 제거한다.
*/
//==================================================================================================
void IoHandler::KickDeadSessions()
{
    //----------------------------------------------------------------------------------------------
    // NOTE: f110615.3L, added array to switch valid active list
    Session** working_list = (this->working_list_for_sender_ != working_slots_for_sender_[0]) ?
        working_slots_for_sender_[0] : working_slots_for_sender_[1];

    //----------------------------------------------------------------------------------------------
    // 제거될 세션들을 ActiveSessionList에서 제거하고 임시 리스트에 담는다.
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

        // 제거된 세션들에 대한 처리
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
            // 동접수 감소
            (is_accept_socket) ? --number_of_accept_sessions_ : --number_of_connect_sessions_;

            NetworkObject* const network_object = session->GetNetworkObject();

            // 네트웍 오브젝트 언바인딩
            session->UnbindNetworkObject();

            // 콜백함수 호출 전에 세션을 free 해준다. 콜백 안에서 Connect요청할 수도 있으므로..
            // accept용 세션이면 소켓을 재사용하여 다시 accept를 걸고, connect용 세션이면 풀에 반환한다.
            if (is_accept_socket && !shutdown_) {
                ReuseSession(session);
            }
            else {
                FreeSession(session);
            }
            // NetworkObject로 디스커넥트 통지
            network_object->OnDisconnect();

            // 네트웍 오브젝트 소멸 요청       
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
    // ActiveSessionList의 세션의 보내기 퍼버 전송
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
            session->Remove(); // 이것은 active_session_list_의 제거가 아니므로 end값 별도 할당해서 사용한다.
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
            session->Remove(); // 이것은 active_session_list_의 제거가 아니므로 end값 별도 할당해서 사용한다.
        }
    }
    active_session_list_->Unlock();
#endif //0
}

//==================================================================================================
/**
    @remarks
        모든 세션들을 제거한다.
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
        Accept된 세션과 Connect된 세션들을 IOCP에 등록하면서 ActiveSessionList로 옮기고
        모든 세션의 받기 버퍼를 검사하여 패킷을 처리하는 등의 작업을 한다.
    @par
        이 함수는 주기적으로 호출해 주어야 한다.
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

    // IOCP 쓰레드 종료
    for (DWORD i = 0; i < number_of_io_threads_; ++i)
    {
        PostQueuedCompletionStatus(iocp_handle_, 0, 0, NULL);
    }

    // IO 워커 쓰레드가 모두 종료될 때까지 기다린다.
    WaitForMultipleObjects(number_of_io_threads_, io_threads_, true, INFINITE);
}

