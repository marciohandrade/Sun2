#pragma once
#ifndef NETWORK_MODULE_IOCP_SERVER_H
#define NETWORK_MODULE_IOCP_SERVER_H

//==================================================================================================

class NetworkObject;
class IoHandler;

typedef NetworkObject* (*fnCallBackCreateAcceptedObject)();
typedef void (*fnCallBackDestroyAcceptedObject)(NetworkObject* network_object);
typedef void (*fnCallBackDestroyConnectedObject)(NetworkObject* network_object);

//==================================================================================================
/// I/O 핸들러 초기화에 필요한 정보를 담는 구조체
//
typedef struct tagIOHANDLER_DESC
{
    DWORD dwIoHandlerKey; ///< I/O 핸들러 키
    DWORD dwMaxAcceptSession; ///< 접속받을 최대 세션 수
    DWORD dwMaxConnectSession; ///< 다른 서버로 접속할 최대 세션 수
    DWORD dwSendBufferSize; ///< 보내기 버퍼 크기
    DWORD dwRecvBufferSize; ///< 받기 버퍼 크기
    DWORD dwTimeOut; ///< 접속 받은 세션의 타임 아웃 틱
    DWORD dwMaxPacketSize; ///< 한 패킷의 최대 크기
    DWORD dwNumberOfIoThreads; ///< I/O 워커 쓰레드 개수
    DWORD dwNumberOfAcceptThreads; ///< Accept 워커 쓰레드 개수
    DWORD dwNumberOfConnectThreads; ///< Connect 워커 쓰레드 개수
    fnCallBackCreateAcceptedObject      fnCreateAcceptedObject; ///< 접속시 NetworkObject 생성 콜백
    fnCallBackDestroyAcceptedObject     fnDestroyAcceptedObject; ///< 접속 해제시 NetworkObject 소멸 콜백
    fnCallBackDestroyConnectedObject    fnDestroyConnectedObject; ///< 다른 서버로 접속한 NetworkObject 객체에 대한 소멸 콜백
} IOHANDLER_DESC, *LPIOHANDLER_DESC;

//==================================================================================================
/// IOCP 서버 클래스
/**
    @remarks
    - IOCP 서버는 다수의 I/O 핸들러를 가질 수 있다.
        (2개 이상의 I/O 핸들러를 초기화할 경우 LPIOHANDLER_DESC를 배열로 초기화)
        - I/O 핸들러는 하나의 리슨 포트를 가진다.
        - Update를 호출할 때 모든 세션의 받기 버퍼를 검사하여 완성된 패킷을 처리한다.
    @see
        IoHandler
        tagIOHANDLER_DESC
    @history
        - 2006.3.6 : pimpl idiom으로 맴버 캡슐화 (taiyo)
*/
//==================================================================================================
//
class IOCPServer
{   
public:
    IOCPServer();
    virtual ~IOCPServer();

    BOOL Init(LPIOHANDLER_DESC description, DWORD number_of_io_handlers);
    BOOL StartListen(DWORD io_handler_key, char* ip_string, WORD port);
    void Update();
    void Shutdown();
    DWORD Connect(DWORD io_handler_key, NetworkObject* network_object, char* ip_string, WORD port);

    BOOL IsListening(DWORD io_handler_key);
    void SuspendListenThread(DWORD io_handler_key);
    void ResumeListenThread(DWORD io_handler_key);

    DWORD GetNumberOfConnections(DWORD io_handler_key);

    DWORD GetMaxSendBuffer(DWORD io_handler_key, DWORD session_index);
    DWORD GetCurSendBuffer(DWORD io_handler_key, DWORD session_index);
    DWORD GetMaxRecvBuffer(DWORD io_handler_key, DWORD session_index);
    DWORD GetCurRecvBuffer(DWORD io_handler_key, DWORD session_index);
    void Display(DWORD io_handler_key, DWORD over_boundary_length);

private:
    BOOL InitWinsock();
    void CreateIoHandler(LPIOHANDLER_DESC description);
    IoHandler* GetIoHandler(DWORD io_handler_key);
    DWORD GetMaxBuffer(DWORD io_handler_key, DWORD session_index,
                       BOOL is_send_buffer, BOOL is_select_max_buffer);

    struct InPimpl;
    InPimpl* in_impl_;
    friend unsigned __stdcall send_thread(LPVOID param);
};

//--------------------------------------------------------------------------------------------------

inline DWORD IOCPServer::GetMaxSendBuffer(DWORD io_handler_key, DWORD session_index)
{
    return GetMaxBuffer(io_handler_key, session_index, true, true);
}

inline DWORD IOCPServer::GetCurSendBuffer(DWORD io_handler_key, DWORD session_index)
{
    return GetMaxBuffer(io_handler_key, session_index, true, false);
}

inline DWORD IOCPServer::GetMaxRecvBuffer(DWORD io_handler_key, DWORD session_index)
{
    return GetMaxBuffer(io_handler_key, session_index, false, true);
}

inline DWORD IOCPServer::GetCurRecvBuffer(DWORD io_handler_key, DWORD session_index)
{
    return GetMaxBuffer(io_handler_key, session_index, false, false);
}

//==================================================================================================

#endif //NETWORK_MODULE_IOCP_SERVER_H