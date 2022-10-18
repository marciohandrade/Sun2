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
/// I/O �ڵ鷯 �ʱ�ȭ�� �ʿ��� ������ ��� ����ü
//
typedef struct tagIOHANDLER_DESC
{
    DWORD dwIoHandlerKey; ///< I/O �ڵ鷯 Ű
    DWORD dwMaxAcceptSession; ///< ���ӹ��� �ִ� ���� ��
    DWORD dwMaxConnectSession; ///< �ٸ� ������ ������ �ִ� ���� ��
    DWORD dwSendBufferSize; ///< ������ ���� ũ��
    DWORD dwRecvBufferSize; ///< �ޱ� ���� ũ��
    DWORD dwTimeOut; ///< ���� ���� ������ Ÿ�� �ƿ� ƽ
    DWORD dwMaxPacketSize; ///< �� ��Ŷ�� �ִ� ũ��
    DWORD dwNumberOfIoThreads; ///< I/O ��Ŀ ������ ����
    DWORD dwNumberOfAcceptThreads; ///< Accept ��Ŀ ������ ����
    DWORD dwNumberOfConnectThreads; ///< Connect ��Ŀ ������ ����
    fnCallBackCreateAcceptedObject      fnCreateAcceptedObject; ///< ���ӽ� NetworkObject ���� �ݹ�
    fnCallBackDestroyAcceptedObject     fnDestroyAcceptedObject; ///< ���� ������ NetworkObject �Ҹ� �ݹ�
    fnCallBackDestroyConnectedObject    fnDestroyConnectedObject; ///< �ٸ� ������ ������ NetworkObject ��ü�� ���� �Ҹ� �ݹ�
} IOHANDLER_DESC, *LPIOHANDLER_DESC;

//==================================================================================================
/// IOCP ���� Ŭ����
/**
    @remarks
    - IOCP ������ �ټ��� I/O �ڵ鷯�� ���� �� �ִ�.
        (2�� �̻��� I/O �ڵ鷯�� �ʱ�ȭ�� ��� LPIOHANDLER_DESC�� �迭�� �ʱ�ȭ)
        - I/O �ڵ鷯�� �ϳ��� ���� ��Ʈ�� ������.
        - Update�� ȣ���� �� ��� ������ �ޱ� ���۸� �˻��Ͽ� �ϼ��� ��Ŷ�� ó���Ѵ�.
    @see
        IoHandler
        tagIOHANDLER_DESC
    @history
        - 2006.3.6 : pimpl idiom���� �ɹ� ĸ��ȭ (taiyo)
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