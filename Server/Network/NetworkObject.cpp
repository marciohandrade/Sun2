#include "stdafx.h"

#include "NetworkObject.h"
#include "Session.h"

NetworkObject::NetworkObject()
{
    session_ = NULL;
    header_size_type_ = HEADER_SIZE_BODY;
    temp_stored_ip_ = 0;
}

NetworkObject::~NetworkObject()
{
}

void NetworkObject::Disconnect(BOOL bGracefulDisconnect)
{
    if (session_) {
        session_->Disconnect(bGracefulDisconnect != false);
    }
}

void NetworkObject::OnDisconnect()
{
    header_size_type_ = HEADER_SIZE_BODY;
}

BOOL NetworkObject::Send(BYTE* pMsg, WORD size)
{
    if (!session_ || session_->HasDisconnectOrdered() || session_->ShouldBeRemoved()) {
        return false;
    }
    return session_->Send(pMsg, size);
}

BOOL NetworkObject::SendEx(DWORD number_of_messages, BYTE **ppMsg, WORD *pwSize)
{
    if (!session_ || session_->HasDisconnectOrdered() || session_->ShouldBeRemoved()) {
        return false;
    }
    return session_->SendEx(number_of_messages, ppMsg, pwSize);
}

void NetworkObject::Redirect(NetworkObject* network_object)
{
    assert(network_object != NULL && "NULL 포인터로 Redirect 시도");
    assert(session_ != NULL);

    session_->BindNetworkObject(network_object);
}

char* NetworkObject::GetIP()
{
    if (session_) {
        return session_->GetIP();
    }
    else {
        return NULL;
    }
}

DWORD NetworkObject::GetIPAddress()
{
    if (session_)
    {
        SOCKADDR_IN* pSA_IN = (SOCKADDR_IN*)session_->GetSockAddr();
        return (DWORD)pSA_IN->sin_addr.s_addr;
    }
    return 0;
}

WORD NetworkObject::GetPort()
{
    if (session_)
    {
        SOCKADDR_IN* pSA_IN = (SOCKADDR_IN*)session_->GetSockAddr();
        return (WORD)pSA_IN->sin_port;
    }
    return 0;
}

#include "SendBuffer.h"
#include "RecvBuffer.h"

void NetworkObject::GetSendBufferInfo(BUFFER_INFO& bufferINFO)
{
    if (session_) {
        const SendBuffer* send_buffer = session_->GetSendBuffer();
        send_buffer->GetBufferSizeInfo(bufferINFO.m_dwMaxBufferSize, bufferINFO.m_dwCurrentUsedSize);
    }
    else {
        bufferINFO.m_dwMaxBufferSize = 0;
        bufferINFO.m_dwCurrentUsedSize = 0;
    }
}

void NetworkObject::GetRecvBufferInfo(BUFFER_INFO& bufferINFO)
{
    if (session_) {
        const RecvBuffer* recv_buffer = session_->GetRecvBuffer();
        recv_buffer->GetBufferSizeInfo(bufferINFO.m_dwMaxBufferSize, bufferINFO.m_dwCurrentUsedSize);
    }
    else {
        bufferINFO.m_dwMaxBufferSize = 0;
        bufferINFO.m_dwCurrentUsedSize = 0;
    }
}

int NetworkObject::SetSockOpt(int level, int optname, const char FAR *optval, int optlen)
{
    if (session_) {
        return session_->SetSockOpt(level, optname, optval, optlen);
    }
    else {
        return SOCKET_ERROR;
    }
}

void NetworkObject::SetSession(Session* session)
{ 
    session_ = session; 

    if (session_) {
        temp_stored_ip_ = GetIPAddress();
    }
}

// CHANGES: f110615.2L, added to solve high-load network bottleneck problem
bool NetworkObject::UseSendPendingBuffer(DWORD pending_buffer_size)
{
    if (session_ == NULL) {
        return false;
    };
    return session_->GetSendBuffer()->UsePendingBuffer(pending_buffer_size);
}

// CHANGES: f110616.3L, added to solve high-load network bottleneck problem
bool NetworkObject::UseRecvDualBuffer(DWORD second_buffer_size)
{
    if (session_ == NULL) {
        return false;
    };
    return session_->GetRecvBuffer()->UseDualBuffer(second_buffer_size);
}
