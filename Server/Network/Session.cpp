#include "stdafx.h"
#include "Session.h"

#include "SessionPool.h"
#include "SendBuffer.h"
#include "RecvBuffer.h"
#include "NetworkObject.h"
#include "MsWinsockUtil.h"

//==================================================================================================
/**
    @remarks
        ���� ������.\n
        ������ ���ۿ� �ޱ� ���۸� �����ϰ� ��� �޸� �ð��� �����Ѵ�.
    @param  send_buffer_size
        ������ ���� ������
    @param  recv_buffer_size
        �ޱ� ���� ������
    @param  timeout
        �� �ð����� �������� ������ �޼����� ������ ������ ���´�.
        �� ���� 0�̸� �޼��� �۽��� ��� ������ ���� �ʴ´�.
*/
//==================================================================================================
Session::Session(DWORD send_buffer_size, DWORD recv_buffer_size,
                 DWORD max_packet_size, DWORD timeout)
{
    ZeroMemory(field_start_pos_, static_cast<size_t>(field_end_pos_ - field_start_pos_));
    //
    send_buffer_ = module::TAllocator<SendBuffer>::Allocate();
    send_buffer_->Create(send_buffer_size, max_packet_size);
    //
    recv_buffer_ = module::TAllocator<RecvBuffer>::Allocate();
    recv_buffer_->Create(recv_buffer_size, max_packet_size);

    timeout_ = timeout;
    socket_ = INVALID_SOCKET;
    accept_socket_ = false;

    //printf("Session Create[%u]\n", Session::m_dwSessionNum);
    //Session::m_dwSessionNum++;
}

Session::~Session()
{
    network_object_ = NULL;
    CloseSocket();

    module::TAllocator<SendBuffer>::DeAllocate(send_buffer_);
    module::TAllocator<RecvBuffer>::DeAllocate(recv_buffer_);
    ZeroMemory(field_start_pos_, static_cast<size_t>(field_end_pos_ - field_start_pos_));
    //printf("Session Removed[%u]\n", Session::m_dwSessionNum);
    //Session::m_dwSessionNum--;
}

//==================================================================================================
/**
    @remarks
        ������ �ʱ�ȭ�Ѵ�.
    @param  socket
        ���ǿ� �Ҵ�� ���� �ڵ�
    @param  sockaddr
        ���ǿ� �Ҵ�� ���� �ּ�
    @retval bool
        �־��� ���� �ڵ��� ��ȿ���� ���� ��� false�� �����Ѵ�.
*/
//==================================================================================================
void Session::Init()
{
    send_buffer_->Clear();
    recv_buffer_->Clear();

    ResetKillFlag();

    disconnect_ordered_ = false;
    stopped_reception_ = false;
}

void Session::Reuse(HANDLE iocp_handle)
{
    send_buffer_->Clear();
    recv_buffer_->Clear();
    //Disconnect�� ovl IO ����
    ZeroMemory(&disconnect_io_data_, sizeof(disconnect_io_data_));
    disconnect_io_data_.operation_type = DISCONNECT_POSTED;
    //----------------------------------------------------------------------------------------------
    // TransmitFile ������� �ʰ� ���� ���� ���鵵�� ����
    // TransmitFile�� �����ϸ� ��� IOCP�� ������ �־��ش�.
    if (!MsWinsockUtil::lpfnTransmitFile_static_(socket_, 0, 0, 0,
                                                 &disconnect_io_data_, 0,
                                                 TF_DISCONNECT | TF_REUSE_SOCKET) 
        && WSAGetLastError() != WSA_IO_PENDING)
    {
        CloseSocket();
        PostQueuedCompletionStatus(iocp_handle, 0, (ULONG_PTR)this, &disconnect_io_data_);
    };
    //----------------------------------------------------------------------------------------------
}

//==================================================================================================
/**
    @remarks
        �ش� ��Ŷ�� ������ ���ۿ� ����.
    @param  pMsg
        ��Ŷ ������
    @param  size
        ��� ũ�⸦ ������ ��Ŷ ������
    @retval bool
        ���ۿ� ���������� ���⸦ ��ġ�� true�� �����ϰ� ������ ���۰� ���� ��� false�� �����Ѵ�.
*/
//==================================================================================================

bool Session::Send(BYTE* msg, WORD size)
{
    if (ShouldBeRemoved()) {
        return false;
    }
    const HEADER_SIZE_TYPE header_type = network_object_->GetHeaderSizeType();
    if (send_buffer_->WriteMessages((const BYTE**)&msg, &size, 1, header_type) == false)
    {
        MSG_BASE* msg_base = reinterpret_cast<MSG_BASE*>(msg);
        OnLogString("[Buffer Full On Send]|C:P=[%03u:%03u]|PacketSize=%u|Key=%u|", 
                    msg_base->m_byCategory, msg_base->m_byProtocol, size, msg_base->m_dwKey);
        Remove();
        return false;
    }

    return true;
}

bool Session::SendEx(DWORD number_of_messages, BYTE** msg_array, WORD* size_array)
{
    if (ShouldBeRemoved()) {
        return false;
    }
    // �̰��� ����ұ� ����... �ش� Array�� �ּҰ� �߸��Ǵ� ���� ����ϴ�.
    // ������ �� ���� ���빰�� ������ �� ũ��.
    // ������ �ϴ� ������.
    assert(!::IsBadReadPtr(msg_array, sizeof(msg_array) * number_of_messages));
    assert(!::IsBadReadPtr(size_array, sizeof(size_array) * number_of_messages));

    const HEADER_SIZE_TYPE header_type = network_object_->GetHeaderSizeType();
    if (send_buffer_->WriteMessages((const BYTE**)msg_array, size_array,
                                    number_of_messages, header_type) == false)
    {
        MSG_BASE* msg_base = reinterpret_cast<MSG_BASE*>(msg_array[0]);
        OnLogString("[Buffer Full On Send]|C:P=[%03u:%03u]|PacketSize=%u|Key=%u|", 
                    msg_base->m_byCategory, msg_base->m_byProtocol,
                    size_array[0], msg_base->m_dwKey);
        Remove();
        return false;
    }

    return true;
}

bool Session::ProcessReceivedPacket(int max_packet_size)
{
    const HEADER_SIZE_TYPE header_type = network_object_->GetHeaderSizeType();
    DWORD incorrect_packet_size = 0;
    // �ޱ� ���ۿ� �ִ� ��Ŷ���� ������ ó���Ѵ�.
    while (BYTE* packet = recv_buffer_->GetFirstPacketPtr(header_type, &incorrect_packet_size))
    {
        PACKET_HEADER* const header = reinterpret_cast<PACKET_HEADER*>(packet);
        MSG_BASE* const msg_base = reinterpret_cast<MSG_BASE*>(packet + sizeof(PACKET_HEADER));
        const int body_size = (header_type == HEADER_SIZE_BODY)
                            ?   header->size
                            :   header->size - sizeof(PACKET_HEADER);
        const int total_size = (header_type == HEADER_SIZE_BODY)
                             ?  header->size + sizeof(PACKET_HEADER)
                             :  header->size;
        ////
        //if (body_size <= 0)
        //{
        //    OnLogString("Mimimum Packet Size Error(%d)", body_size);
        //    return false;
        //};
        //if (total_size > max_packet_size)
        //{
        //    OnLogString("Max Packet Size(%u/%u) Over : "
        //                "[PrePacket] Size[%d] Category[%d] Protocol[%d]",
        //                total_size, max_packet_size,
        //                backup_net_msg_size_, backup_net_category_, backup_net_protocol_);
        //    return false;
        //};
        //
        network_object_->OnRecv(packet + sizeof(PACKET_HEADER), static_cast<WORD>(body_size));
        //
        recv_buffer_->PopHeadMessage(total_size);
        //if (recv_buffer_->PopHeadMessage(total_size) == false)
        //{
        //    OnLogString("RemoveFirstPacket Error(%d) : "
        //                "[PrePacket] Size[%d] Category[%d] Protocol[%d]",
        //                body_size, backup_net_msg_size_,
        //                backup_net_category_, backup_net_protocol_);
        //    return false;
        //};
        //
        ResetTimeOut();
        ;{
            backup_net_category_ = msg_base->m_byCategory;
            backup_net_protocol_ = msg_base->m_byProtocol;
            backup_net_msg_size_ = body_size;
        };
        incorrect_packet_size = 0;
    }; // end 'foreach pop messages'
    //----------------------------------------------------------------------------------------------
    if (incorrect_packet_size)
    {
        OnLogString("the received packet header is incorrect, max_expected_size=%u, read_size=%u|"
                    "previous packet info (%3u:%3u, Size=%5u)|",
                    max_packet_size,
                    ((incorrect_packet_size == ULONG_MAX) ? 0 : incorrect_packet_size),
                    backup_net_category_, backup_net_protocol_, backup_net_msg_size_);
        return false; //_NA_007112_20140312_MODIFY_PROCESSRECEIVEDPACKET
    };

    return true;
}

//==================================================================================================
/**
    @remarks
        IOCP�� send�� �ɾ� ���´�.
    @retval bool
    - ������ ���۰� ����ְų� ���� �غ� �ȵ� �����̸� �׳� true�� �����Ѵ�.
    - send�� �����ϸ� false�� �����Ѵ�.
*/
//==================================================================================================
bool Session::PreSend()
{
    // NOTE: this method is called in send_thread
    module::RingBufferInfo sendable_info;
    BOOST_STATIC_ASSERT(sizeof(sendable_info) == sizeof(WSABUF) * 2);
    if (send_buffer_->PrepareSend(&sendable_info) == false) {
        return true;
    };
    //
    ZeroMemory(&send_io_data_, sizeof(send_io_data_));
    send_io_data_.operation_type = SEND_POSTED;
    //
    DWORD number_of_wsabuf_count = 1 + (sendable_info.second.len != 0);
    DWORD send_bytes = 0;
    DWORD flags = 0;
    //
    int result = ::WSASend(socket_, &sendable_info.first, number_of_wsabuf_count,
                           &send_bytes, flags, &send_io_data_, NULL);
    // CHANGES: f110623.5L, it should be the tic-toc processing.
    if (result == SOCKET_ERROR)
    {
        int wsa_error = WSAGetLastError();
        if (wsa_error != WSA_IO_PENDING) {
            //send_buffer_->ReleaseSendingLock();
            return false;
        };
    };
    return true;
}

//==================================================================================================
/**
    @remarks
        IOCP�� recv�� �ɾ� ���´�.
    @retval bool
        recv�� �����ϸ� false�� �����Ѵ�.
*/
//==================================================================================================
bool Session::PreRecv()
{
    module::RingBufferInfo writable_info;
    BOOST_STATIC_ASSERT(sizeof(writable_info) == sizeof(WSABUF) * 2);
    if (recv_buffer_->PrepareRecv(&writable_info) == false)
    {
        OnLogString("[PreRecv] RecvBuffer Full!");
        return false;
    }
    DWORD number_of_wsabuf_count = 1 + (writable_info.second.len != 0);
    DWORD recvBytes = 0;
    DWORD flags = 0;

    ZeroMemory(&recv_io_data_, sizeof(recv_io_data_));
    recv_io_data_.operation_type = RECV_POSTED;

    int ret = WSARecv(socket_, &writable_info.first, number_of_wsabuf_count,
                      &recvBytes, &flags, &recv_io_data_, NULL);
    if (ret == SOCKET_ERROR)
    {
        int wsa_error = WSAGetLastError();
        if (wsa_error != WSA_IO_PENDING)
        {   // ������ �뷮�� ������?
            return false;
        }
    };
    return true;
}

bool Session::PreAccept(SOCKET listen_socket)
{
    if (socket_ == INVALID_SOCKET) {
        SetSocket(CreateSocket());
    }

    Init();

    DWORD bytes_received = 0;
    int nLen = 0;
    BYTE* pRecvBuf = NULL;
    //Session ���� Recv Ptr���� �¾�..
    recv_buffer_->GetRecvParam(&pRecvBuf, nLen);

    //AcceptEx �� ovl IO ����
    ZeroMemory(&accept_io_data_, sizeof(accept_io_data_));
    accept_io_data_.operation_type = ACCEPT_POSTED;
    accept_io_data_.session = this; //�ڽ��� ptr

    //printf("[ACCEPT][%d] Session::PreAccept, removed_=%d\n", (int)socket_, (int)removed_);

    //AcceptEx�� ���� Add
    int nRet = MsWinsockUtil::lpfnAccepteEx_static_(listen_socket, socket_, 
        pRecvBuf, 0,
        sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16,
        &bytes_received,          &accept_io_data_);

    if (nRet == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING) {
        return false;
    }

    return true;
}

SOCKET Session::CreateSocket() 
{
    SOCKET new_socket = \
        WSASocket(AF_INET, SOCK_STREAM, IPPROTO_IP, NULL, 0, WSA_FLAG_OVERLAPPED); 

    if (new_socket == INVALID_SOCKET) {
        return new_socket;
    }

    /*
    int nRet = 0;
    int nZero = 0;
    nZero = 0;
    nRet = setsockopt(new_socket, SOL_SOCKET, SO_SNDBUF, (char *)&nZero, sizeof(nZero));

    if (nRet == SOCKET_ERROR) 
    {
    return INVALID_SOCKET;
    }
    */
    return new_socket;
}

void Session::BindNetworkObject(NetworkObject* network_object)
{
    network_object_ = network_object;
    network_object->SetSession(this);
}

void Session::UnbindNetworkObject()
{
    if (network_object_ == NULL) {
        return;
    }
    network_object_->SetSession(NULL);
    network_object_ = NULL;
}

void Session::OnAccept()
{
    ResetKillFlag();
    ResetTimeOut();
    network_object_->OnAccept(GetIndex());
    //printf("[Session::OnAccept][%d] removed_=%d\n", (int)socket_, (int)removed_);
}

void Session::OnConnect(bool success)
{
    NetworkObject* network_object = network_object_;
    if (!success) {
        UnbindNetworkObject();
    }
    network_object->OnConnect(success, GetIndex());
}

void Session::OnLogString(const char* log_string, ...)
{
    if (!network_object_) {
        return;
    }
    char string_buffer[512];
    va_list arg_start;

    va_start(arg_start, log_string);
    _vsnprintf(string_buffer, sizeof(string_buffer) / sizeof(string_buffer[0]),
               log_string, arg_start);
    va_end(arg_start);

    //printf("%s(%s)\n", szBuffer, GetIP());

    network_object_->OnLogString(string_buffer);
}

void Session::Disconnect(bool bGracefulDisconnect)
{ 
    if (bGracefulDisconnect)  { 
        disconnect_ordered_ = true; 
    } 
    else {
        Remove(); 
    }
#ifdef __SUSUNLOVE_REMOVE_ONLOGSTRING_APPLY_20080110
    OnLogString("[Disconnect] Is Graceful %u\n", bGracefulDisconnect);
#else
  #ifdef _DEBUG
    OnLogString("[Disconnect] Is Graceful %u\n", bGracefulDisconnect);
  #endif
#endif
}

int Session::SetSockOpt(int level, int optname, const char FAR *optval, int optlen)
{
    return setsockopt(socket_, level, optname, optval, optlen); 
}

