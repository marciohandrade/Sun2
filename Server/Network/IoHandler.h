#pragma once
#ifndef NETWORK_MODULE_IOHANDLER_H
#define NETWORK_MODULE_IOHANDLER_H

//==================================================================================================

#include <process.h>

#define MAX_IO_WORKER_THREAD    16
#define EXTRA_ACCEPTEX_NUM      10

class Session;
class SessionPool;
class Acceptor;
class Connector;
class SessionList;
class IOCPServer;
class NetworkObject;
struct tagIOHANDLER_DESC;

typedef tagIOHANDLER_DESC IOHANDLER_DESC, *LPIOHANDLER_DESC;

typedef NetworkObject* (*fnCallBackCreateAcceptedObject)();
typedef void (*fnCallBackDestroyAcceptedObject)(NetworkObject* network_object);
typedef void (*fnCallBackDestroyConnectedObject)(NetworkObject* network_object);
//typedef void (*fnCallBackLog)(NetworkObject network_object, char* log_string);

//==================================================================================================
/// 리슨 소켓을 열고 접속을 처리하는 클래스
//  - I/O 핸들러는 하나의 포트를 가지고 리슨할 수 있으며 별도의 세션 풀을 가지고 있다.
//  - 접속이 이루어지면 세션풀에서 하나의 세션을 할당받아 소켓을 초기화한 후 TempSessionList에 추가한다.
//==================================================================================================
//
class IoHandler
{
public:
    IoHandler();
    ~IoHandler();

    void Init(IOCPServer* iocp_server, LPIOHANDLER_DESC description);
    BOOL StartListen(char* ip_string, WORD port);
    void Update();
    void Shutdown();
    DWORD Connect(NetworkObject* network_object, char* ip_string, WORD port);

    BOOL IsListening();
    void SuspendListenThread();
    void ResumeListenThread();

    DWORD GetNumberOfConnections() const;
    DWORD GetKey() const;
    Session* GetSession(DWORD session_index);
    SessionList* GetActiveSessionList();

private:
    Session* AllocAcceptSession();
    Session* AllocConnectSession();
    void FreeSession(Session* session);
    void ReuseSession(Session* session);
    void ProcessConnectSuccessList();
    void ProcessConnectFailList();
    void ProcessAcceptedSessionList();
    void ProcessActiveSessionList();
    void KickDeadSessions();
    void ProcessSend();
    void KickAllSessions();

    union {
        char field_start_pos_[1];
        bool shutdown_;
    };

    IOCPServer* iocp_server_;
    SessionPool* accept_session_pool_;
    SessionPool* connect_session_pool_;
    Acceptor* acceptor_;
    Connector* connector_;
    SessionList* active_session_list_;
    SessionList* accepted_session_list_;
    SessionList* connect_success_list_;
    SessionList* connect_fail_list_;
    SessionList* temp_session_list_;
    SessionList* temp_session_pending_list_;

    DWORD io_handler_key_;
    HANDLE iocp_handle_; // IOCP 핸들
    DWORD max_packet_size_;
    DWORD number_of_accept_sessions_;
    DWORD number_of_connect_sessions_;
    DWORD max_number_of_accept_sessions_;
    DWORD max_number_of_connect_sessions_;

    DWORD number_of_io_threads_; // IO 워커 쓰레드 개수
    HANDLE io_threads_[MAX_IO_WORKER_THREAD]; // IO 쓰레드 핸들

    DWORD number_of_working_list_nodes_for_sender_;
    Session** working_list_for_sender_;
    Session** working_slots_for_sender_[2];

    // 콜백 함수 포인터
    fnCallBackCreateAcceptedObject   fnCreateAcceptedObject_;
    fnCallBackDestroyAcceptedObject  fnDestroyAcceptedObject_;
    fnCallBackDestroyConnectedObject fnDestroyConnectedObject_;
    //
    char field_end_pos_[1];

    friend unsigned __stdcall io_thread(LPVOID param);
    friend unsigned __stdcall send_thread(LPVOID param);
    friend unsigned __stdcall accept_thread(LPVOID param);
    friend unsigned __stdcall connect_thread(LPVOID param);
};


inline DWORD IoHandler::GetNumberOfConnections() const {
    return number_of_accept_sessions_ + number_of_connect_sessions_;
}

inline DWORD IoHandler::GetKey() const {
    return io_handler_key_;
}

inline SessionList* IoHandler::GetActiveSessionList() {
    return active_session_list_;
}


#endif //NETWORK_MODULE_IOHANDLER_H
