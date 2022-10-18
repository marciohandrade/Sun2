#pragma once
#ifndef NETWORK_MODULE_SESSION_H
#define NETWORK_MODULE_SESSION_H

//==================================================================================================

#define SEND_POSTED         1
#define RECV_POSTED         2
#define ACCEPT_POSTED       3
#define DISCONNECT_POSTED   4

class SendBuffer;
class RecvBuffer;
class SessionPool;
class NetworkObject;
class Session;

/// OVERLAPPED 확장 구조체
struct OVERLAPPEDEX : public OVERLAPPED
{
    DWORD operation_type;
    Session* session;
};

//==================================================================================================
/// 기본적인 소켓 처리 기능을 갖고 있는 세션 클래스
/**
    @remarks
    이 클래스를 상속 받아 확장 유져 클래스를 만들 수 있다.
    @par
    - 세션을 상속받은 클래스에서는 다음의 함수들을 재정의 해서 사용한다.
    - OnAccept:     클라이언트가 accept되었을 때 호출된다.
    - OnDisConnect: 클라이언트의 접속이 해제 되었을 때 호출된다.
    - OnRecv:       클라리언트가 패킷을 수신했을 때 호출된다.
    - OnConnect:    다른 서버로의 connect가 성공 또는 실패했을 때 호출된다.
    - OnUpdate:     매프레임 IO 엔진이 Update될 때마다 호출된다.
    @note
    - ws2_32.lib 추가할 것
    - winsock2.h 포함할 것
*/
//==================================================================================================
//
class Session
{
    friend unsigned __stdcall io_thread(LPVOID param);
    friend class SessionPool;

public:
    /// timeout 값이 0이면 메세지 송신이 없어도 접속을 끊지 않는다.
    Session(DWORD send_buffer_size, DWORD recv_buffer_size, DWORD max_packet_size, DWORD timeout);
    virtual ~Session();

    void Init();
    void Reuse(HANDLE iocp_handle);
    bool Send(BYTE* pMsg, WORD size);
    bool SendEx(DWORD number_of_messages, BYTE** ppMsg, WORD* pwSize);
    bool PreSend();
    bool PreRecv();
    bool PreAccept(SOCKET listen_socket);
    SOCKET CreateSocket();
    bool ProcessReceivedPacket(int max_packet_size);
    void BindNetworkObject(NetworkObject* network_object);
    void UnbindNetworkObject();
    void OnAccept();
    void OnConnect(bool success);
    void OnLogString(const char* log_string, ...);

    int SetSockOpt(int level, int optname, const char FAR* optval, int optlen);
    void SetSocket(SOCKET socket);
    void SetSockAddr(SOCKADDR_IN& sockaddr);
    void CloseSocket();
    NetworkObject* GetNetworkObject() const;
    SendBuffer* GetSendBuffer() const; ///< 보내기 버퍼 포인터를 리턴한다.
    RecvBuffer* GetRecvBuffer() const; ///< 받기 버퍼 포인터를 리턴한다.
    SOCKET GetSocket() const; ///< 소켓 핸들을 리턴한다.
    SOCKADDR* GetSockAddr() const; ///< 소켓 주소의 포인터를 리턴한다.
    char* GetIP() const; ///< 세션에 물린 소켓의 IP를 스트링으로 반환

    DWORD GetIdleTick() const;
    bool IsOnIdle() const;
    DWORD GetIndex() const;
    bool IsAcceptSocket() const;
    void SetAcceptSocketFlag();
    void Remove();
    void ResetKillFlag();
    bool ShouldBeRemoved();
    void Disconnect(bool bGracefulDisconnect);
    bool HasDisconnectOrdered() const;
    // NOTE: f110616.5L,
    bool StoppedReception() const;
    void StopReception();
    void ResumeReception();

private:
    void SetIndex(DWORD index);
    void ResetTimeOut();

#pragma warning(push)
#pragma warning(disable : 4201)
    union {
        char field_start_pos_[1];
        long removed_; //volatile... reordered position, force a change memory state on update
    };
    NetworkObject* network_object_;
    SendBuffer* send_buffer_;
    RecvBuffer* recv_buffer_;

    DWORD session_index_;
    SOCKET socket_;
    bool accept_socket_;
    bool disconnect_ordered_;
    long stopped_reception_; // NOTE: f110616.1L, close socket because of a missing event

    OVERLAPPEDEX send_io_data_;
    OVERLAPPEDEX recv_io_data_;
    OVERLAPPEDEX accept_io_data_;
    OVERLAPPEDEX disconnect_io_data_;
    DWORD last_sync_tick_;
    DWORD timeout_;

    // 임시 테스트 용
    struct {
        int backup_net_category_;
        int backup_net_protocol_;
        int backup_net_msg_size_;
    };
    SOCKADDR_IN sockaddr_in_;
    char field_end_pos_[1];
    //  static DWORD m_dwSessionNum;
#pragma warning(pop)
};

//==================================================================================================

inline void Session::SetSocket(SOCKET socket)
{
    socket_ = socket;
}

inline void Session::SetSockAddr(SOCKADDR_IN& sockaddr)
{
    sockaddr_in_ = sockaddr;
}

inline void Session::CloseSocket()
{
    closesocket(socket_);
    socket_ = INVALID_SOCKET;
}

inline NetworkObject* Session::GetNetworkObject() const
{
    return network_object_;
}

inline SendBuffer* Session::GetSendBuffer() const
{
    return send_buffer_;
} ///< 보내기 버퍼 포인터를 리턴한다.

inline RecvBuffer* Session::GetRecvBuffer() const
{
    return recv_buffer_;
} ///< 받기 버퍼 포인터를 리턴한다.

inline SOCKET Session::GetSocket() const
{
    return socket_;
} ///< 소켓 핸들을 리턴한다.

inline SOCKADDR* Session::GetSockAddr() const
{
    return (SOCKADDR*)&sockaddr_in_;
} ///< 소켓 주소의 포인터를 리턴한다.

inline char* Session::GetIP() const
{
    return inet_ntoa(sockaddr_in_.sin_addr);
} ///< 세션에 물린 소켓의 IP를 스트링으로 반환

inline DWORD Session::GetIdleTick() const
{
    return timeout_? last_sync_tick_ + timeout_ : 0;
}

inline bool Session::IsOnIdle() const
{
    return timeout_ ? last_sync_tick_ + timeout_ < GetTickCount()
                    : false;
}

inline DWORD Session::GetIndex() const
{
    return session_index_;
}

inline bool Session::IsAcceptSocket() const
{
    return accept_socket_;
}

inline void Session::SetAcceptSocketFlag()
{
    accept_socket_ = true;
}

inline void Session::ResetKillFlag()
{
    InterlockedExchange(&removed_, false);
    InterlockedExchange(&stopped_reception_, false);
}

inline bool Session::ShouldBeRemoved()
{
    return removed_ != false;
}

inline void Session::Remove()
{ 
    InterlockedExchange(&removed_, true); 
}

inline bool Session::HasDisconnectOrdered() const
{
    return disconnect_ordered_;
}

inline bool Session::StoppedReception() const
{
    return (stopped_reception_ != false);
}

// NOTE: f110616.5L, stop messages reception because of insufficient buffer
inline void Session::StopReception()
{
    InterlockedExchange(&stopped_reception_, true);
}

// NOTE: f110616.5L, thie methods support external reception events.
inline void Session::ResumeReception()
{
    if (stopped_reception_) {
        InterlockedExchange(&stopped_reception_, (this->PreRecv() == false));
    };
}

inline void Session::SetIndex(DWORD index)
{
    session_index_ = index;
}

inline void Session::ResetTimeOut()
{
    last_sync_tick_ = GetTickCount();
}

#endif //NETWORK_MODULE_SESSION_H
