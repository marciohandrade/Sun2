#ifndef SOLARCLIENTNETWORK_SOLARCLIENTNETWORKAPI_H_
#define SOLARCLIENTNETWORK_SOLARCLIENTNETWORKAPI_H_

#if _MSC_VER > 1000
    #pragma once
#endif

#include <winsock2.h>

//------------------------------------------------------------------------------
//! created : 2010/03/18
//! author : by i4u4me
//! remark : winaip 함수 frost lib 사용시 함수 마이그레이션
//------------------------------------------------------------------------------
namespace sun_network_api
{
    enum NetworkSecurity
    {
        kNetworkSecurity_none = 0,
        kNetworkSecurity_Frost      //! frost lib 사용
    };

    void set_network_security(NetworkSecurity network_security);
 
    void sun_initialize();
    void sun_finalize();
    void sun_set_username(const char* user_name);
    int sun_connect(SOCKET s,
                const struct sockaddr* name,
                int namelen);
    int sun_send(SOCKET s,
                const char* buf,
                int len,
                int flags);
    int sun_WSASend(SOCKET s,
                LPWSABUF lpBuffers,
                DWORD dwBufferCount,
                LPDWORD lpNumberOfBytesSent,
                DWORD dwFlags,
                LPWSAOVERLAPPED lpOverlapped,
                LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine);
    int sun_sendto(SOCKET s,
                const char* buf,
                int len,
                int flags,
                const struct sockaddr* to,
                int tolen);
    int sun_recv(SOCKET s,
                char* buf,
                int len,
                int flags);
    int sun_WSARecv(SOCKET s,
                LPWSABUF lpBuffers,
                DWORD dwBufferCount,
                LPDWORD lpNumberOfBytesRecvd,
                LPDWORD lpFlags,
                LPWSAOVERLAPPED lpOverlapped,
                LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine);
    int sun_recvfrom(SOCKET s,
                char* buf,
                int len,
                int flags,
                struct sockaddr* from,
                int* fromlen);

    //------------------------------------------------------------------------------
#ifdef _DEBUG
    class NetworkDebugFile
    {
    public:
        NetworkDebugFile(const char* filename, DWORD packet_size);
        ~NetworkDebugFile();
    private:
        DWORD start_time_;
        char filename_[256];
        DWORD packet_size_;
    };
#endif
    //------------------------------------------------------------------------------

};


#endif //SOLARCLIENTNETWORK_SOLARCLIENTNETWORKAPI_H_