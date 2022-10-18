#pragma once
#ifndef NETWORK_MODULE_NETWORK_OBJECT_H
#define NETWORK_MODULE_NETWORK_OBJECT_H

//==================================================================================================

#include "NetworkDefine.h"

class Session;

//==================================================================================================
/// NetworkObject
//  - fnCreateAcceptedObject �ݹ� �Լ����� �� Ŭ������ ��ӹ��� Ŭ������ �ν��Ͻ���
//    ��ȯ�ϸ� ��Ʈ�� ����� �ϴ� Session Ŭ������ ���ε� �ȴ�.
//  - 4���� ���� �Լ�(OnAccept, OnDisconnect, OnRecv, OnConnect)�� ������ �ؼ� ����Ѵ�.
//
//==================================================================================================

class NetworkObject
{
public:
    NetworkObject();
    virtual ~NetworkObject();

    void Disconnect(BOOL bGracefulDisconnect = true);
    BOOL Send(BYTE* pMsg, WORD size);
    BOOL SendEx(DWORD number_of_messages, BYTE** ppMsg, WORD* pwSize);
    void Redirect(NetworkObject* network_object);

    char* GetIP();
    DWORD GetIPAddress();
    WORD GetPort();
    DWORD GetTempIPAddress() { return temp_stored_ip_; }

    int SetSockOpt(int level, int optname, const char FAR* optval, int optlen);

    struct BUFFER_INFO
    {
        DWORD m_dwMaxBufferSize;
        DWORD m_dwCurrentUsedSize;
    };

    void GetSendBufferInfo(BUFFER_INFO& bufferINFO);
    void GetRecvBufferInfo(BUFFER_INFO& bufferINFO);

    void SetHeaderSizeType(HEADER_SIZE_TYPE type) { header_size_type_ = type; }
    HEADER_SIZE_TYPE GetHeaderSizeType() { return header_size_type_; }

    // CHANGES: f110615.2L, added to solve high-load network bottleneck problem
    bool UseSendPendingBuffer(DWORD pending_buffer_size);
    // CHANGES: f110616.3L, added to solve high-load network bottleneck problem
    bool UseRecvDualBuffer(DWORD second_buffer_size);
protected:
    virtual void OnAccept(DWORD network_index) { network_index; }
    virtual void OnDisconnect();
    virtual void OnConnect(BOOL success, DWORD network_index) { success, network_index; }

private:
    virtual void OnRecv(BYTE* pMsg, WORD size) = 0;
    virtual void OnLogString(char* log_string) { log_string; }

    void SetSession(Session* session);

    Session* session_;
    HEADER_SIZE_TYPE header_size_type_; // __SUSUNLOVE_HEADER_SIZE_MODIFY_20080924
    DWORD temp_stored_ip_; // session_�� NULL�� �Ǿ ���� IP ������ ���� �ֵ��� ����Ѵ�.
    //
    friend class Session;
    friend class IoHandler;
};

#endif //NETWORK_MODULE_NETWORK_OBJECT_H